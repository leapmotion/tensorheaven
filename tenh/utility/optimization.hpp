// ///////////////////////////////////////////////////////////////////////////
// tenh/utility/optimization.hpp by Victor Dods, created 2013/12/09
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_UTILITY_OPTIMIZATION_HPP_
#define TENH_UTILITY_OPTIMIZATION_HPP_

#include "tenh/core.hpp"

#include <iostream>

#include "tenh/implementation/innerproduct.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"
#include "tenh/interop/eigen_invert.hpp"

namespace Tenh {

// adaptive minimization which uses, in order of availability/preference,
// 1. Newton's method, 2. conjugate gradient, and 3. gradient descent.
template <typename InnerProductId_, typename ObjectiveFunction_, typename BasedVectorSpace_, typename Scalar_, typename GuessUseArrayType_, typename Derived_>
ImplementationOf_t<BasedVectorSpace_,Scalar_> minimize (ObjectiveFunction_ const &func,
                                                        ImplementationOf_t<BasedVectorSpace_,Scalar_,GuessUseArrayType_,Derived_> const &guess,
                                                        Scalar_ const tolerance)
{
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_> VectorType;
    typedef typename InnerProduct_f<BasedVectorSpace_,InnerProductId_,Scalar_>::T VectorInnerProductType;
    typedef ImplementationOf_t<typename DualOf_f<BasedVectorSpace_>::T,Scalar_> CoVectorType;
    typedef typename InnerProduct_f<typename DualOf_f<BasedVectorSpace_>::T,InnerProductId_,Scalar_>::T CoVectorInnerProductType;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,BasedVectorSpace_> Sym2;
    typedef typename ObjectiveFunction_::D2 HessianType;
    typedef ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<2,BasedVectorSpace_>,Scalar_> HessianInverseType;
    STATIC_ASSERT_TYPES_ARE_EQUAL(VectorType, typename ObjectiveFunction_::V);
    STATIC_ASSERT_TYPES_ARE_EQUAL(Scalar_, typename ObjectiveFunction_::Out);
    static int const LINE_SEARCH_SAMPLE_COUNT = 50;
    static Scalar_ const STEP_SCALE = Scalar_(1);
    static Scalar_ const MAX_STEP_SIZE = Scalar_(-1);
    // static Scalar_ const MAX_STEP_SIZE = Scalar_(1);
    static int const MAX_ITERATION_COUNT = 20;
    static Scalar_ const GRADIENT_DESCENT_STEP_SIZE = Scalar_(1);
    static bool const PRINT_DEBUG_OUTPUT = false;
    static Scalar_ const EPSILON = 1e-5;

    VectorInnerProductType vector_innerproduct;
    CoVectorInnerProductType covector_innerproduct;

    AbstractIndex_c<'a'> a;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;

    VectorType current_approximation = guess;
    int iteration_count = 0;
    int gradient_descent = 0;
    int conjugate_gradient = 0;
    int newtons_method = 0;
    bool tolerance_was_attained = false;

    while (iteration_count < MAX_ITERATION_COUNT)
    {
        Scalar_ current_value = func.function(current_approximation);
        CoVectorType g = func.D_function(current_approximation);
        Scalar_ g_squared_norm = g(i)*covector_innerproduct.split(i|j)*g(j);
        Scalar_ g_norm = std::sqrt(g_squared_norm);

        if (PRINT_DEBUG_OUTPUT)
        {
            std::cout << current_approximation << "\n" << g << " " << g_norm <<  " " << current_value << std::endl;
        }

        if (g_norm <= tolerance)
        {
            tolerance_was_attained = true;
            break;
        }

        HessianType h(func.D2_function(current_approximation));
        HessianInverseType hinv(Static<WithoutInitialization>::SINGLETON);
        VectorType step(Static<WithoutInitialization>::SINGLETON);
        Scalar_ d;

        if (invert_2tensor(h, hinv))
        {
            step(i).no_alias() = hinv.split(i|j) * -g(j);
            d = h(a)*(step(i)*step(j)).bundle(i|j,Sym2(),a);
        }
        else
        {
            d = 5; // bigger than EPSILON.
        }

        //std::cerr << "positive definiteness along step: " << d << '\n';
        if (d < EPSILON) // h isn't postive definite along step so fall back to conjugate gradient
        {
            VectorType v(Static<WithoutInitialization>::SINGLETON);
            v(j).no_alias() = g(i) * covector_innerproduct.split(i|j);
            // d = g(i)*v(i);
            d = v(i) * h.split(i|j) * v(j);

            if (d < EPSILON) // h isn't positive definite along g either, gradient descent
            {
                if (PRINT_DEBUG_OUTPUT)
                {
                    std::cout << "Doing Gradient descent." << std::endl;
                    ++gradient_descent;
                }
                step(i).no_alias() = -GRADIENT_DESCENT_STEP_SIZE * g(i) / g_norm;
            }
            else // h is positive definite along g, use conjugate gradient
            {
                if (PRINT_DEBUG_OUTPUT)
                {
                    std::cout << "Using conjugate gradient." << std::endl;
                    ++conjugate_gradient;
                }
                //Scalar_ v_squared_norm = v(i)*vector_innerproduct(a).split(a,i|j)*v(j);
                //Scalar_ v_norm = std::sqrt(v_squared_norm);

                // step(i).no_alias() = -g_squared_norm / v_squared_norm * v(i);
                step(i).no_alias() = (-g_squared_norm / d) * v(i);
            }
        }
        else // h is positive definite along step, use it as is.
        {
            if (PRINT_DEBUG_OUTPUT)
            {
                std::cout << "Newton's method." << std::endl << h.split(i|j) << std::endl;
                ++newtons_method;
            }
        }

        step(i).no_alias() = STEP_SCALE * step(i);

        Scalar_ step_norm = std::sqrt(vector_innerproduct.split(i|j)*step(i)*step(j));

        if (MAX_STEP_SIZE > 0 && step_norm > MAX_STEP_SIZE)
        {
            if (PRINT_DEBUG_OUTPUT)
            {
                std::cout << "Clamping step length to " << MAX_STEP_SIZE << std::endl;
            }
            step(i).no_alias() = MAX_STEP_SIZE * step(i) / step_norm;
            step_norm = std::sqrt(vector_innerproduct.split(i|j)*step(i)*step(j));
        }

        //std::cerr << "step = " << step << ", step size = " << step_norm << '\n';

        // {
        //     std::cerr << "line search errors: ";
        //     Scalar_ quad_a = func.D2_function(current_approximation).split(i|j)*step(i)*step(j)/2;
        //     Scalar_ quad_b = func.D_function(current_approximation)(i)*step(i);
        //     Scalar_ quad_c = func.function(current_approximation);
        //     // print out error between quadratic approx and actual function value
        //     for (Uint32 it = 0; it <= LINE_SEARCH_SAMPLE_COUNT; ++it)
        //     {
        //         VectorType x(Static<WithoutInitialization>::SINGLETON);
        //         Scalar_ t = Scalar_(it) / LINE_SEARCH_SAMPLE_COUNT;
        //         x(i).no_alias() = current_approximation(i) + t * step(i);
        //         Scalar_ actual_value = func.function(x);
        //         Scalar_ approximate_value = quad_c + quad_b*t + quad_a*sqr(t);
        //         std::cerr << approximate_value - actual_value << ' ';
        //     }
        //     std::cerr << '\n';
        // }

//         {
//             std::cerr << "approximation offsets: ";
//             Scalar_ quad_a = func.D2_function(current_approximation).split(i|j)*step(i)*step(j)/2;
//             Scalar_ quad_b = func.D_function(current_approximation)(i)*step(i);
//             // print out error between quadratic approx and actual function value
//             for (Uint32 it = 0; it <= LINE_SEARCH_SAMPLE_COUNT; ++it)
//             {
//                 VectorType x(Static<WithoutInitialization>::SINGLETON);
//                 Scalar_ t = Scalar_(it) / LINE_SEARCH_SAMPLE_COUNT;
// //                x(i).no_alias() = current_approximation(i) + t * step(i);
//                 Scalar_ approximation_offset = quad_b*t + quad_a*sqr(t);
//                 std::cerr << approximation_offset << ' ';
//             }
//             std::cerr << '\n';
//         }

//         {
//             std::cerr << "approximation offsets (geometric sequence): ";
//             Scalar_ quad_a = func.D2_function(current_approximation).split(i|j)*step(i)*step(j)/2;
//             Scalar_ quad_b = func.D_function(current_approximation)(i)*step(i);
//             // print out error between quadratic approx and actual function value
//             for (Uint32 it = 0; it <= LINE_SEARCH_SAMPLE_COUNT; ++it)
//             {
//                 VectorType x(Static<WithoutInitialization>::SINGLETON);
//                 Scalar_ t = std::pow(Scalar_(2), -Scalar_(it));
// //                x(i).no_alias() = current_approximation(i) + t * step(i);
//                 Scalar_ approximation_offset = quad_b*t + quad_a*sqr(t);
//                 std::cerr << "t = " << t << ": " << approximation_offset << '\n';
//             }
//             std::cerr << '\n';
//         }

        VectorType line_search_step(Static<WithoutInitialization>::SINGLETON);
        VectorType partial_step(fill_with(0));
        VectorType x(Static<WithoutInitialization>::SINGLETON);
        line_search_step(i).no_alias() = step(i) / LINE_SEARCH_SAMPLE_COUNT;
        int it;
        for (it = 0; it < LINE_SEARCH_SAMPLE_COUNT; ++it)
        {
            partial_step(i).no_alias() += line_search_step(i);
            x(i).no_alias() = current_approximation(i) + partial_step(i);
            Scalar_ next_value = func.function(x);

            if (next_value > current_value)
            {
                current_value = next_value;
                step = partial_step;
                break;
            }
            current_value = next_value;
        }
        if (PRINT_DEBUG_OUTPUT)
        {
            std::cout << "used " << it << " steps in line search\n";
            if (it != 0 && it != LINE_SEARCH_SAMPLE_COUNT)
            {
                std::cout << "Did a partial line search\n";
            }
        }

        current_approximation(i) += step(i);

        ++iteration_count;
    }

    if (PRINT_DEBUG_OUTPUT)
    {
        std::cout << "optimize took " << iteration_count << " steps " << newtons_method << " were Newton's method, " << conjugate_gradient << " were conjugate gradient, and " << gradient_descent << " were gradient descent." <<  std::endl;
    }

    return current_approximation;
}

} // end of namespace Tenh

#endif // TENH_UTILITY_OPTIMIZATION_HPP_
