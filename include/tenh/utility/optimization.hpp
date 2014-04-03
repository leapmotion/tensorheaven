// ///////////////////////////////////////////////////////////////////////////
// tenh/utility/optimization.hpp by Victor Dods, created 2013/12/09
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_UTILITY_OPTIMIZATION_HPP_
#define TENH_UTILITY_OPTIMIZATION_HPP_

#include "tenh/core.hpp"

#if _WIN32
#include <float.h>
#else
#include <cmath>
#endif

#include <iostream>

#include "tenh/implementation/innerproduct.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"
#include "tenh/interop/eigen_invert.hpp"
#include "tenh/interop/eigen_svd.hpp"

static bool const PRINT_DEBUG_OUTPUT = true;

#define DEBUG_OUTPUT(x) \
if (PRINT_DEBUG_OUTPUT) \
{ \
    std::cerr << x; \
}

namespace Tenh {

template <typename T>
bool isNaN(T const &x)
{
    return false;
}

inline bool isNaN(float const &x)
{
#if _WIN32
    return _isnan(x) != 0;
#else
    return std::isnan(x);
#endif
}

inline bool isNaN(double const &x)
{
#if _WIN32
    return _isnan(x) != 0;
#else
    return std::isnan(x);
#endif
}

inline bool isNaN(long double const &x)
{
#if _WIN32
    return _isnan(x) != 0;
#else
    return std::isnan(x);
#endif
}

// ///////////////////////////////////////////////////////////////////////////
// line searches
// ///////////////////////////////////////////////////////////////////////////

namespace LineSearch {

// Tests the function after one step -- if the function at that position is
// greater than the current position, divides the step size through by the
// specified amount and tries again.  The goal is to leave position with a
// value such that the function is less than that of position's initial value.
// If max_iteration_count is exceeded, then position is left unchanged.
// The return value is the success of the algorithm, i.e. if the position was
// updated in the step direction such that the function takes a lower value.
template <typename ObjectiveFunction_,
          typename Derived1_,
          typename Derived2_,
          typename Scalar_,
          typename BasedVectorSpace_,
          ComponentQualifier COMPONENT_QUALIFIER_>
bool geometric_step (ObjectiveFunction_ const &f,
                     Vector_i<Derived1_,Scalar_,BasedVectorSpace_,ComponentQualifier::NONCONST_MEMORY> &position,
                     Vector_i<Derived2_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &step,
                     Scalar_ scale_factor,
                     Uint32 max_iteration_count)
{
    assert(scale_factor > Scalar_(0) && scale_factor < Scalar_(1) && "scale_factor must be a positive number less than 1");
    assert(max_iteration_count > 0 && "max_iteration_count must be positive");
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_> V;
    AbstractIndex_c<'i'> i;
    Scalar_ accumulated_scale_factor(1);
    Scalar_ current_value(f.function(position));
    DEBUG_OUTPUT("    LineSearch::geometric_step; current function value = " << current_value << '\n');
    V next_position(Static<WithoutInitialization>::SINGLETON);
    for (Uint32 iteration_count = 0; iteration_count < max_iteration_count; ++iteration_count, accumulated_scale_factor *= scale_factor)
    {
        next_position(i).no_alias() = position(i) + accumulated_scale_factor * step(i);
        Scalar_ next_value(f.function(next_position));
        DEBUG_OUTPUT(   "    LineSearch::geometric_step; for accumulated_scale_factor = " << accumulated_scale_factor
                     << ", function value is " << next_value << '\n');
        // if the next value is less than the current value, set the position and return success
        if (next_value < current_value)
        {
            position = next_position;
            DEBUG_OUTPUT("    LineSearch::geometric_step succeeded on iteration " << iteration_count << '\n');
            return true;
        }
    }

    // max iteration count was exceeded, so return failure
    DEBUG_OUTPUT("    LineSearch::geometric_step failed after " << max_iteration_count << " iterations (scale_factor = " << scale_factor << ")\n");
    return false;
}

template <typename ObjectiveFunction_,
          typename Derived1_,
          typename Derived2_,
          typename Scalar_,
          typename BasedVectorSpace_,
          ComponentQualifier COMPONENT_QUALIFIER_>
bool uniform_step (ObjectiveFunction_ const &f,
                   Vector_i<Derived1_,Scalar_,BasedVectorSpace_,ComponentQualifier::NONCONST_MEMORY> &position,
                   Vector_i<Derived2_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &step,
                   Uint32 substep_count = 1)
{
    assert(substep_count > 0 && "must have positive substep_count");
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_> V;
    V partial_step(Static<WithoutInitialization>::SINGLETON);
    AbstractIndex_c<'i'> i;
    partial_step(i).no_alias() = step(i) / substep_count;

    V current_position(position);
    V next_position(position);
    for (Uint32 step_count = 0; step_count < substep_count; ++step_count)
    {
        next_position(i).no_alias() += partial_step(i);
        // first, check if the next value is bigger than the current value.
        // if it is, then bisect
        Scalar_ current_value(f.function(current_position));
        Scalar_ next_value(f.function(next_position));
        if (next_value >= current_value)
        {
            // if the first step failed to decrease, return with failure.
            if (step_count == 0)
            {
                DEBUG_OUTPUT("    LineSearch::uniform_step failed on first step (out of " << substep_count << " possible)\n");
                return false;
            }
            // otherwise a non-first step failed to decrease, so use the
            // current position and return with success.
            else
            {
                DEBUG_OUTPUT("    LineSearch::uniform_step succeeded on step " << step_count+1 << " (out of " << substep_count << " possible)\n");
                position(i).no_alias() = current_position(i);
                return true;
            }
        }
        current_position(i).no_alias() = next_position(i);
    }

    // all steps decreased, so set the final step position and return with success
    DEBUG_OUTPUT("    LineSearch::uniform_step succeeded for all steps (out of " << substep_count << " possible)\n");
    position(i).no_alias() = next_position(i);
    return true;
}

}

// ///////////////////////////////////////////////////////////////////////////
// optimization methods
// ///////////////////////////////////////////////////////////////////////////

// adaptive minimization which uses, in order of availability/preference,
// 1. Newton's method, 2. conjugate gradient, and 3. gradient descent.
template <typename InnerProductId_, typename ObjectiveFunction_, typename BasedVectorSpace_, typename Scalar_, typename GuessUseArrayType_, typename Derived_>
ImplementationOf_t<BasedVectorSpace_,Scalar_> minimize (ObjectiveFunction_ const &func,
                                                        ImplementationOf_t<BasedVectorSpace_,Scalar_,GuessUseArrayType_,Derived_> const &guess,
                                                        Scalar_ tolerance,
                                                        Scalar_ *minimum = nullptr)
{
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_> VectorType;
    typedef typename InnerProduct_f<BasedVectorSpace_,InnerProductId_,Scalar_>::T VectorInnerProductType;
    typedef ImplementationOf_t<typename DualOf_f<BasedVectorSpace_>::T,Scalar_> CoVectorType;
    typedef typename InnerProduct_f<typename DualOf_f<BasedVectorSpace_>::T,InnerProductId_,Scalar_>::T CoVectorInnerProductType;
    typedef typename ObjectiveFunction_::D2 HessianType;
    typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<Typle_t<typename DualOf_f<BasedVectorSpace_>::T,
                                                                          typename DualOf_f<BasedVectorSpace_>::T>>,Scalar_> Hessian2TensorType;
    static_assert(TypesAreEqual_f<VectorType,typename ObjectiveFunction_::V>::V, "types must match");
    static_assert(TypesAreEqual_f<Scalar_,typename ObjectiveFunction_::Out>::V, "types must match");
    static Scalar_ const LINE_SEARCH_GEOMETRIC_STEP_FACTOR = Scalar_(0.5);
    static Uint32 const LINE_SEARCH_GEOMETRIC_STEP_MAX_ITERATION_COUNT = 10;
    static int const LINE_SEARCH_UNIFORM_STEP_SUBSTEP_COUNT = 50;
    static Scalar_ const STEP_SCALE = Scalar_(1);
    static Scalar_ const MAX_STEP_SIZE = Scalar_(-1);
    // static Scalar_ const MAX_STEP_SIZE = Scalar_(1);
    static int const MAX_ITERATION_COUNT = 20;
    static Scalar_ const GRADIENT_DESCENT_STEP_SIZE = Scalar_(0.25);
    static Scalar_ const EPSILON = 1e-10;

    VectorInnerProductType vector_innerproduct;
    CoVectorInnerProductType covector_innerproduct;

    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;

    VectorType current_approximation = guess;
    int iteration_count = 0;
    int gradient_descent = 0;
    int conjugate_gradient = 0;
    int newtons_method = 0;
    // bool tolerance_was_attained = false;

    while (iteration_count < MAX_ITERATION_COUNT)
    {
        Scalar_ current_value = func.function(current_approximation);
        CoVectorType g = func.D_function(current_approximation);
        CoVectorType minus_g(Static<WithoutInitialization>::SINGLETON);
        Scalar_ g_squared_norm = covector_innerproduct(g, g);
        Scalar_ g_norm = std::sqrt(g_squared_norm);
        minus_g(i) = -g(i);

        DEBUG_OUTPUT(   "minimize: iteration " << iteration_count
                     << ", current_approximation = " << current_approximation
                     << "\n    gradient = " << g
                     << ", norm of gradient = " << g_norm
                     << ", current function value = " << current_value << '\n');

        if (g_norm <= tolerance)
        {
            // tolerance_was_attained = true;
            if (minimum != nullptr)
                *minimum = current_value;
            break;
        }

        HessianType h(func.D2_function(current_approximation));
        Hessian2TensorType h2(Static<WithoutInitialization>::SINGLETON);
        VectorType step(Static<WithoutInitialization>::SINGLETON);
        Scalar_ d;
        h2(i*j) = h.split(i*j);

        // TODO; this could be replaced with something that uses symmetry of h
        SVD_solve(h2,step,minus_g);
        d = h(step, step);

        if (isNaN(d) || d < EPSILON) // h isn't postive definite along step so fall back to conjugate gradient
        {
            VectorType v(Static<WithoutInitialization>::SINGLETON);
            v(j).no_alias() = g(i) * covector_innerproduct.split(i*j);
            d = h(v, v);

            if (isNaN(d) || d < EPSILON) // h isn't positive definite along g either, gradient descent
            {
                step(i).no_alias() = -GRADIENT_DESCENT_STEP_SIZE * g(i) / g_norm;
                DEBUG_OUTPUT("    Gradient descent; step = " << step << '\n');
                ++gradient_descent;
            }
            else // h is positive definite along g, use conjugate gradient
            {
                step(i).no_alias() = (-g_squared_norm / d) * v(i);
                DEBUG_OUTPUT("    Conjugate gradient; d = " << d << ", step = " << step << '\n');
                ++conjugate_gradient;
            }
        }
        else // h is positive definite along step, use it as is.
        {
            DEBUG_OUTPUT("    Newton's method; step = " << step << '\n');
            ++newtons_method;
        }

        step(i).no_alias() = STEP_SCALE * step(i);

        Scalar_ step_norm = std::sqrt(vector_innerproduct(step, step)); //std::sqrt(vector_innerproduct.split(i*j)*step(i)*step(j));

        if (MAX_STEP_SIZE > 0 && step_norm > MAX_STEP_SIZE)
        {
            DEBUG_OUTPUT("    Clamping step length to " << MAX_STEP_SIZE << '\n');
            step(i).no_alias() = MAX_STEP_SIZE * step(i) / step_norm;
            step_norm = MAX_STEP_SIZE;
        }

        bool line_search_success =
            LineSearch::uniform_step(func,
                                     current_approximation,
                                     step,
                                     LINE_SEARCH_UNIFORM_STEP_SUBSTEP_COUNT);
        if (!line_search_success)
        {
            step(i).no_alias() = step(i) / LINE_SEARCH_UNIFORM_STEP_SUBSTEP_COUNT;
            line_search_success =
                LineSearch::geometric_step(func,
                                           current_approximation,
                                           step,
                                           LINE_SEARCH_GEOMETRIC_STEP_FACTOR,
                                           LINE_SEARCH_GEOMETRIC_STEP_MAX_ITERATION_COUNT);
            // TODO: do something if there's still a failure
        }

        ++iteration_count;
    }

    DEBUG_OUTPUT(   "    minimize took " << iteration_count << " steps; "
                 << newtons_method << " were Newton's method, "
                 << conjugate_gradient << " were conjugate gradient, and "
                 << gradient_descent << " were gradient descent." << '\n');

    return current_approximation;
}

/*
template <typename InnerProductId_, typename ObjectiveFunction_, typename BasedVectorSpace_, typename Scalar_, typename GuessUseArrayType_, typename Derived_>
ImplementationOf_t<BasedVectorSpace_,Scalar_>
    gradient_descent (ObjectiveFunction_ const &func,
                      ImplementationOf_t<BasedVectorSpace_,Scalar_,GuessUseArrayType_,Derived_> const &guess,
                      Scalar_ norm_grad_tolerance,
                      Scalar_ max_step_size = Scalar_(1),
                      Uint32 max_step_count = 200)
{
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_> VectorType;
    typedef typename InnerProduct_f<BasedVectorSpace_,InnerProductId_,Scalar_>::T VectorInnerProductType;
    typedef ImplementationOf_t<typename DualOf_f<BasedVectorSpace_>::T,Scalar_> CoVectorType;
    typedef typename InnerProduct_f<typename DualOf_f<BasedVectorSpace_>::T,InnerProductId_,Scalar_>::T CoVectorInnerProductType;
    static_assert(TypesAreEqual_f<VectorType,typename ObjectiveFunction_::V>::V, "types must match");
    static_assert(TypesAreEqual_f<Scalar_,typename ObjectiveFunction_::Out>::V, "types must match");
    static int const LINE_SEARCH_SAMPLE_COUNT = 50;
    static Scalar_ const STEP_SCALE = Scalar_(1);
    static bool const PRINT_DEBUG_OUTPUT = false;
    static Scalar_ const EPSILON = 1e-5;

    VectorInnerProductType vector_innerproduct;
    CoVectorInnerProductType covector_innerproduct;

    AbstractIndex_c<'a'> a;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;

    VectorType current_approximation = guess;
    VectorType gradient(Static<WithoutInitialization>::SINGLETON);
    int step_count = 0;
    bool tolerance_was_attained = false;

    while (step_count < max_step_count)
    {
        grad(j) = func.D_function(current_approximation)(i)*covector_innerproduct.split(i*j);
        Scalar_ norm_grad = std::sqrt(vector_innerproduct(grad, grad));
        if (norm_grad <= norm_grad_tolerance)
        {
            tolerance_was_attained = true;
            break;
        }



        ++step_count;
    }
}
*/
// TEMP

// ///////////////////////////////////////////////////////////////////////////
// norm and squared norm functions, which require an inner product to use
// ///////////////////////////////////////////////////////////////////////////

// this is sort of hacky -- it would probably be better to make an
// InnerProductSpace_c conceptual type and make ImplementationOf_t aware
// of it.
// NOTE: this won't work for complex types
template <typename InnerProductId_,
          typename Derived_,
          typename Scalar_,
          typename BasedVectorSpace_,
          ComponentQualifier COMPONENT_QUALIFIER_>
Scalar_ squared_norm (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &v)
{
    typename InnerProduct_f<BasedVectorSpace_,InnerProductId_,Scalar_>::T inner_product;
    return inner_product(v, v);//inner_product.split(i*j)*v(i)*v(j); // doesn't take advantage of symmetry
}

// NOTE: this won't work for complex types
template <typename InnerProductId_,
          typename Derived_,
          typename Scalar_,
          typename BasedVectorSpace_,
          ComponentQualifier COMPONENT_QUALIFIER_>
typename AssociatedFloatingPointType_t<Scalar_>::T
    norm (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &v)
{
    return std::sqrt(squared_norm<InnerProductId_>(v));
}

// ///////////////////////////////////////////////////////////////////////////

// uniform distribution in [0,1]
inline void randomize (float &x)
{
    x = float(rand()) / RAND_MAX;
}

// uniform distribution in [0,1]
inline void randomize (double &x)
{
    x = double(rand()) / RAND_MAX;
}

// uniform distribution in [0,1]
inline void randomize (long double &x)
{
    x = static_cast<long double>(rand()) / RAND_MAX;
}

// open annulus of given inner and outer radii in the vector space
template <typename InnerProductId_, typename Derived_, typename Scalar_, typename BasedVectorSpace_, ComponentQualifier COMPONENT_QUALIFIER_>
void randomize (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> &x,
                Scalar_ const &inner_radius,
                Scalar_ const &outer_radius)
{
    static_assert(COMPONENT_QUALIFIER_ != ComponentQualifier::PROCEDURAL, "must not use procedural components");
    Scalar_ squared_inner_radius = sqr(inner_radius);
    Scalar_ squared_outer_radius = sqr(outer_radius);
    Scalar_ sqn;
    do
    {
        // generate a random vector in the cube [-1,1]^n, where n is the dimension of the space
        for (typename Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_>::ComponentIndex i; i.is_not_at_end(); ++i)
        {
            randomize(x[i]); // puts x[i] within [0,1]
            x[i] *= Scalar_(2)*outer_radius;
            x[i] -= outer_radius;
        }
        sqn = squared_norm<InnerProductId_>(x);
    } // if the randomly generated point is outside of the annulus, try again
    while (sqn <= squared_inner_radius || squared_outer_radius <= sqn);
}


template <typename InnerProductId_,
          typename ObjectiveFunction_,
          typename BasedVectorSpace_,
          typename GuessUseArrayType_,
          typename Derived_>
ImplementationOf_t<BasedVectorSpace_,typename ObjectiveFunction_::Scalar>
    totally_random_minimization (ObjectiveFunction_ const &func,
                                 ImplementationOf_t<BasedVectorSpace_,typename ObjectiveFunction_::Scalar,GuessUseArrayType_,Derived_> const &initial_guess,
                                 typename ObjectiveFunction_::Scalar search_radius,
                                 Uint32 sample_count = 10000,
                                 Uint32 max_iteration_count = 8,
                                 typename ObjectiveFunction_::Scalar *minimum = nullptr)
{
    typedef typename ObjectiveFunction_::Scalar Scalar;
    assert(search_radius > Scalar(0) && "search_radius must be positive");

    typedef ImplementationOf_t<BasedVectorSpace_,Scalar> V;

    V random_center(initial_guess);
    V minimizer(initial_guess);
    Scalar min(func.function(minimizer));

    // TODO: maybe make some threshold e such that if the change in minimum
    // is less than e, the iteration stops.

    for (Uint32 iteration_count = 0; iteration_count < max_iteration_count; ++iteration_count)
    {
        for (Uint32 sample = 0; sample < sample_count; ++sample)
        {
            V x(Static<WithoutInitialization>::SINGLETON);
            randomize<InnerProductId_>(x, Scalar(0), search_radius);
            AbstractIndex_c<'i'> i;
            x(i) += random_center(i);
            Scalar value(func.function(x));
            if (value < min)
            {
                min = value;
                minimizer = x;
            }
        }

        // recenter and narrow search radius
        random_center = minimizer;
        search_radius /= Scalar(2);
    }

    if (minimum != nullptr)
        *minimum = min;

    return minimizer;
}

} // end of namespace Tenh

#endif // TENH_UTILITY_OPTIMIZATION_HPP_
