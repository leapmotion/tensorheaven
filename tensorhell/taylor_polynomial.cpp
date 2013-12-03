#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

#include "applications/polynomial.hpp"

#include "tenh/implementation/identity.hpp"
#include "tenh/implementation/innerproduct.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"

using namespace Tenh;

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
          bool COMPONENTS_ARE_IMMUTABLE_>
Scalar_ squared_norm (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &v)
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'p'> p;
    typename InnerProduct_f<BasedVectorSpace_,InnerProductId_,Scalar_>::T inner_product;
    return inner_product(p).split(p,i|j)*v(i)*v(j); // doesn't take advantage of symmetry
}

// NOTE: this won't work for complex types
template <typename InnerProductId_,
          typename Derived_,
          typename Scalar_,
          typename BasedVectorSpace_,
          bool COMPONENTS_ARE_IMMUTABLE_>
typename AssociatedFloatingPointType_t<Scalar_>::T
    norm (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &v)
{
    return std::sqrt(squared_norm<InnerProductId_>(v));
}

// ///////////////////////////////////////////////////////////////////////////

/*
design goals
------------

- given a function and a gradient, verify that the gradient is the gradient for the function
  via numerical estimation -- using asymptotic bounds
- given a function and a gradient and a hessian, verify that the gradient and hessian are
  actually the ones for that function, analogous to previous method.
- implement Newton's method
- implement conjugate gradient method
- implement gradient descent
- implement adaptive thingy (Newton's method, congjugate gradient method, gradient descent)
*/

// uniform distribution in [0,1]
void randomize (float &x)
{
    x = float(rand()) / RAND_MAX;
}

// uniform distribution in [0,1]
void randomize (double &x)
{
    x = double(rand()) / RAND_MAX;
}

// uniform distribution in [0,1]
void randomize (long double &x)
{
    x = static_cast<long double>(rand()) / RAND_MAX;
}

// open annulus of given inner and outer radii in the vector space -- false
// is the parameter value for COMPONENTS_ARE_IMMUTABLE
template <typename InnerProductId_, typename Derived_, typename Scalar_, typename BasedVectorSpace_>
void randomize (Vector_i<Derived_,Scalar_,BasedVectorSpace_,false> &x,
                Scalar_ const &inner_radius,
                Scalar_ const &outer_radius)
{
    Scalar_ squared_inner_radius = sqr(inner_radius);
    Scalar_ squared_outer_radius = sqr(outer_radius);
    Scalar_ sqn;
    do
    {
        // generate a random vector in the cube [-1,1]^n, where n is the dimension of the space
        for (typename Vector_i<Derived_,Scalar_,BasedVectorSpace_,false>::ComponentIndex i; i.is_not_at_end(); ++i)
        {
            randomize(x[i]); // puts x[i] within [0,1]
            x[i] *= Scalar_(2)*outer_radius;
            x[i] -= outer_radius;
        }
        sqn = squared_norm<InnerProductId_>(x);
    } // if the randomly generated point is outside of the annulus, try again
    while (sqn <= squared_inner_radius || squared_outer_radius <= sqn);
}

template <typename BasedVectorSpace_, typename Scalar_, typename Derived_>
struct TaylorPolynomialVerifier_t
{
    typedef typename DualOf_f<BasedVectorSpace_>::T DualOfBasedVectorSpace;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfBasedVectorSpace> Sym2Dual;

    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_,UseMemberArray> V;
    typedef ImplementationOf_t<DualOfBasedVectorSpace,Scalar_,UseMemberArray> DualOfV;
    typedef ImplementationOf_t<Sym2Dual,Scalar_,UseMemberArray> Sym2_DualOfV;

    template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
              typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
    Scalar_ evaluate_0th_order (Vector_i<Derived0_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                Vector_i<Derived1_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &evaluation_point) const
    {
        return as_derived().function(based_at_point);
    }

    template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
              typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
    Scalar_ evaluate_1st_order (Vector_i<Derived0_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                Vector_i<Derived1_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &evaluation_point) const
    {
        AbstractIndex_c<'i'> i;
        return as_derived().function(based_at_point)
             + as_derived().gradient(based_at_point)(i)*(evaluation_point(i) - based_at_point(i));
    }

    template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
              typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
    Scalar_ evaluate_2nd_order (Vector_i<Derived0_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                Vector_i<Derived1_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &evaluation_point) const
    {
        V delta(Static<WithoutInitialization>::SINGLETON);
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        delta(i) = evaluation_point(i) - based_at_point(i);
        return as_derived().function(based_at_point)
             + as_derived().gradient(based_at_point)(i)*delta(i)
             + as_derived().hessian(based_at_point)(p).split(p,i|j)*delta(i)*delta(j)/2;
    }

    template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
              typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
    Scalar_ evaluate_0th_order_via_delta (Vector_i<Derived0_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                          Vector_i<Derived1_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &delta) const
    {
        return as_derived().function(based_at_point);
    }

    template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
              typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
    Scalar_ evaluate_1st_order_via_delta (Vector_i<Derived0_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                          Vector_i<Derived1_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &delta) const
    {
        AbstractIndex_c<'i'> i;
        return as_derived().function(based_at_point)
             + as_derived().gradient(based_at_point)(i)*delta(i);
    }

    template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
              typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
    Scalar_ evaluate_2nd_order_via_delta (Vector_i<Derived0_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                          Vector_i<Derived1_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &delta) const
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        return as_derived().function(based_at_point)
             + as_derived().gradient(based_at_point)(i)*delta(i)
             + as_derived().hessian(based_at_point)(p).split(p,i|j)*delta(i)*delta(j)/2;
    }

    template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE_>
    Scalar_ verify_gradient (Vector_i<Derived0_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &at_point) const
    {
        Scalar_ big_o_bound(0);
        static Uint32 const SAMPLES = 10;
        for (Uint32 i = 0; i < SAMPLES; ++i)
        {
            V delta(Static<WithoutInitialization>::SINGLETON);
            V evaluation_point(Static<WithoutInitialization>::SINGLETON);
            AbstractIndex_c<'j'> j;
            randomize<StandardInnerProduct>(delta, Scalar_(1)/100, Scalar_(2)/100);
            evaluation_point(j) = at_point(j) + delta(j);
            // Scalar_ actual_function_value(function(at_point + delta)); // TODO: make this work?
            Scalar_ actual_function_value(as_derived().function(evaluation_point));
//             Scalar_ estimated_function_value(evaluate_1st_order(at_point, evaluation_point));
            Scalar_ estimated_function_value(evaluate_1st_order_via_delta(at_point, delta));
//             std::cerr << FORMAT_VALUE(actual_function_value) << ", " << FORMAT_VALUE(estimated_function_value) << '\n';
            Scalar_ asymptotic_ratio = fabs(actual_function_value - estimated_function_value)
                                       /
                                       squared_norm<StandardInnerProduct>(delta);
//                                        norm<StandardInnerProduct>(delta);
            big_o_bound = std::max(big_o_bound, asymptotic_ratio);
        }
        //std::cerr << "verify_gradient(" << FORMAT_VALUE(at_point) << "): " << FORMAT_VALUE(big_o_bound) << '\n';
        return big_o_bound;
    }

    template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE_>
    Scalar_ verify_hessian (Vector_i<Derived0_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &at_point) const
    {
        Scalar_ big_o_bound(0);
        static Uint32 const SAMPLES = 100;
        for (Uint32 i = 0; i < SAMPLES; ++i)
        {
            V delta(Static<WithoutInitialization>::SINGLETON);
            V evaluation_point(Static<WithoutInitialization>::SINGLETON);
            AbstractIndex_c<'j'> j;
            // use a solid annulus of radius [1/100, 2/200] around at_point
            randomize<StandardInnerProduct>(delta, Scalar_(1)/100, Scalar_(2)/100);
            evaluation_point(j) = at_point(j) + delta(j);
            // Scalar_ actual_function_value(function(at_point + delta)); // TODO: make this work?
            Scalar_ actual_function_value(as_derived().function(evaluation_point));
//             Scalar_ estimated_function_value(evaluate_2nd_order(at_point, evaluation_point));
            Scalar_ estimated_function_value(evaluate_2nd_order_via_delta(at_point, delta));
//             std::cerr << FORMAT_VALUE(actual_function_value) << ", " << FORMAT_VALUE(estimated_function_value) << '\n';
            Scalar_ asymptotic_ratio = fabs(actual_function_value - estimated_function_value)
                                       /
                                       cube(norm<StandardInnerProduct>(delta));
//                                        squared_norm<StandardInnerProduct>(delta);

            big_o_bound = std::max(big_o_bound, asymptotic_ratio);
//             std::cerr << FORMAT_VALUE(at_point) << ", " << FORMAT_VALUE(norm<StandardInnerProduct>(delta)) << ", " << FORMAT_VALUE(big_o_bound) << '\n';
        }
        //std::cerr << "verify_hessian(" << FORMAT_VALUE(at_point) << "): " << FORMAT_VALUE(big_o_bound) << '\n';
        return big_o_bound;
    }

    Derived_ const &as_derived () const { return *static_cast<Derived_ const *>(this); }
    Derived_ &as_derived () { return *static_cast<Derived_ *>(this); }
};

template <typename BasedVectorSpace_, typename Scalar_>
struct QuadraticFunction_t
    :
    public TaylorPolynomialVerifier_t<BasedVectorSpace_,Scalar_,QuadraticFunction_t<BasedVectorSpace_,Scalar_> >
{
    typedef TaylorPolynomialVerifier_t<BasedVectorSpace_,Scalar_,QuadraticFunction_t<BasedVectorSpace_,Scalar_> > Parent;

    typedef typename Parent::V V;
    typedef typename Parent::DualOfV DualOfV;
    typedef typename Parent::Sym2_DualOfV Sym2_DualOfV;

    QuadraticFunction_t ()
        :
//         m(Static<WithoutInitialization>::SINGLETON)
        m(Scalar_(0))
    {
        // won't necessarily be positive definite or even nondegenerate.
        //randomize<StandardInnerProduct>(m, Scalar_(1), Scalar_(2));
//         for (typename Sym2_DualOfV::ComponentIndex i; i.is_not_at_end(); ++i)
//             randomize(m[i]);

//         m[typename Sym2_DualOfV::ComponentIndex(0)] = 1;
//         m[typename Sym2_DualOfV::ComponentIndex(2)] = 1;
//         m[typename Sym2_DualOfV::ComponentIndex(5)] = 1;

        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        for (Uint32 k = 0; k < V::DIM; ++k)
        {
            DualOfV a(Static<WithoutInitialization>::SINGLETON);
            randomize<StandardInnerProduct>(a, Scalar_(1), Scalar_(2));
            m(p) += (a(i)*a(j)).bundle(i|j,typename Sym2_DualOfV::Concept(),p);
        }
//         std::cerr << FORMAT_VALUE(m(p).split(p,i|j)) << '\n';
    }

    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    Scalar_ function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        // "canonical" quadratic form whose hessian is equal to m.
        return x(i)*m(p).split(p,i|j)*x(j) / 2;
    }

    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    DualOfV gradient (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        DualOfV retval(Static<WithoutInitialization>::SINGLETON);
        retval(j) = x(i)*m(p).split(p,i|j);
        return retval;
    }

    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    Sym2_DualOfV hessian (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        return m;
    }

private:

    Sym2_DualOfV m;
};

template <typename BasedVectorSpace_, typename Scalar_>
struct SombreroFunction_t
    :
    public TaylorPolynomialVerifier_t<BasedVectorSpace_,Scalar_,SombreroFunction_t<BasedVectorSpace_,Scalar_> >
{
    typedef TaylorPolynomialVerifier_t<BasedVectorSpace_,Scalar_,SombreroFunction_t<BasedVectorSpace_,Scalar_> > Parent;

    typedef typename Parent::V V;
    typedef typename Parent::DualOfV DualOfV;
    typedef typename Parent::Sym2_DualOfV Sym2_DualOfV;

    SombreroFunction_t ()
        :
//         m(Static<WithoutInitialization>::SINGLETON)
        m(Scalar_(0))
    {
        // won't necessarily be positive definite or even nondegenerate.
        //randomize<StandardInnerProduct>(m, Scalar_(1), Scalar_(2));
//         for (typename Sym2_DualOfV::ComponentIndex i; i.is_not_at_end(); ++i)
//             randomize(m[i]);

//         m[typename Sym2_DualOfV::ComponentIndex(0)] = Scalar_(1);
//         m[typename Sym2_DualOfV::ComponentIndex(2)] = Scalar_(1);
//         m[typename Sym2_DualOfV::ComponentIndex(5)] = Scalar_(1);
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        for (Uint32 k = 0; k < V::DIM; ++k)
        {
            DualOfV a(Static<WithoutInitialization>::SINGLETON);
            randomize<StandardInnerProduct>(a, Scalar_(1), Scalar_(2));
            m(p) += (a(i)*a(j)).bundle(i|j,typename Sym2_DualOfV::Concept(),p);
        }
//         std::cerr << FORMAT_VALUE(m(p).split(p,i|j)) << '\n';
    }

    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    Scalar_ function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        return Scalar_(1) / (Scalar_(1) + x(i)*m(p).split(p,i|j)*x(j) / 2);
    }

    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    DualOfV gradient (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        DualOfV retval(Static<WithoutInitialization>::SINGLETON);
        retval(j) = -sqr(function(x)) * x(i)*m(p).split(p,i|j);
        return retval;
    }

    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    Sym2_DualOfV hessian (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'l'> l;
        AbstractIndex_c<'p'> p;
        AbstractIndex_c<'q'> q;
        AbstractIndex_c<'r'> r;
        Sym2_DualOfV retval(Static<WithoutInitialization>::SINGLETON);
        Scalar_ f_of_x(function(x));
        retval(r) = sqr(f_of_x) * (Scalar_(2) * f_of_x * (m(p).split(p,i|j)*x(j)*x(k)*m(q).split(q,k|l)).bundle(i|l,typename Sym2_DualOfV::Concept(),r) - m(r));
        return retval;
    }

private:

    Sym2_DualOfV m;
};

template <typename BasedVectorSpace_, typename Scalar_>
struct CayleyTransformParameterizedQuadraticFunction_t
    :
    public TaylorPolynomialVerifier_t<BasedVectorSpace_,Scalar_,CayleyTransformParameterizedQuadraticFunction_t<BasedVectorSpace_,Scalar_> >
{
private:

    enum { _ = Assert<DimensionOf_f<BasedVectorSpace_>::V == 3>::V };

public:

    typedef TaylorPolynomialVerifier_t<BasedVectorSpace_,Scalar_,CayleyTransformParameterizedQuadraticFunction_t<BasedVectorSpace_,Scalar_> > Parent;

    typedef typename Parent::DualOfBasedVectorSpace DualOfBasedVectorSpace;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedVectorSpace_,TypeList_t<DualOfBasedVectorSpace> > > EndomorphismOfBasedVectorSpace;
    typedef typename Parent::Sym2Dual Sym2Dual;

    typedef typename Parent::V V;
    typedef typename Parent::DualOfV DualOfV;
    typedef typename Parent::Sym2_DualOfV Sym2_DualOfV;
    typedef ImplementationOf_t<EndomorphismOfBasedVectorSpace,Scalar_,UseMemberArray> T;

    CayleyTransformParameterizedQuadraticFunction_t ()
        :
        m_constant_term(0),
        m_linear_term(0),
        m_quadratic_term(0)
    { }

    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    Scalar_ function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        AbstractIndex_c<'i'> i;
        T cayley_transform(Static<WithoutInitialization>::SINGLETON);
        cayley_transform(i) = K(x) * H(x)(i);
        return quadratic_function(cayley_transform);
    }

    // template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    // DualOfV gradient (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    // {
    // }

    // template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    // Sym2_DualOfV hessian (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    // {
    // }

private:

    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    Scalar_ objective_function (Tensor_i<Derived_,Scalar_,EndomorphismOfBasedVectorSpace,COMPONENTS_ARE_IMMUTABLE_> const &t) const
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        // just take squared norm of the tensor for now.  TODO: real function
        typedef TypeList_t<StandardInnerProduct,TypeList_t<StandardInnerProduct> > InnerProductFactorList;
        typedef TensorProduct_c<InnerProductFactorList> InnerProductId;
        // typedef typename InnerProduct_f<EndomorphismOfBasedVectorSpace,
        //                                 TensorProduct_c<InnerProductFactorList>,
        //                                 Scalar_>::T b;
        // return t(i)*b(p).split(p,i|j)*t(j);
        return squared_norm<InnerProductId>(t);
    }
    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    Scalar_ J (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        return Scalar_(1) - x(i)*m_inner_product(p).split(p,i|j)*x(j);
    }
    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    Scalar_ K (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        return Scalar_(1) + x(i)*m_inner_product(p).split(p,i|j)*x(j);
    }
    // Lie algebra morphism from (R^3, \times) to (so(3), [.,.])
    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    T Hat (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        // for brevity
        typedef typename Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_>::ComponentIndex c;
        typedef typename T::ComponentIndex C;
        T retval(Static<WithoutInitialization>::SINGLETON);
        retval[C(0)] = Scalar_(0);
        retval[C(1)] = -x[c(2)];
        retval[C(2)] =  x[c(1)];
        retval[C(3)] =  x[c(2)];
        retval[C(4)] = Scalar_(0);
        retval[C(5)] = -x[c(0)];
        retval[C(6)] = -x[c(1)];
        retval[C(7)] =  x[c(0)];
        retval[C(8)] = Scalar_(0);
        return retval;
    }
    template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
    T N (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENTS_ARE_IMMUTABLE_> const &x) const
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'p'> p;
        AbstractIndex_c<'q'> q;
        T retval(Static<WithoutInitialization>::SINGLETON);
        retval(i|k).no_alias() = J(x)*m_identity(p).split(i|k) 
                               + Scalar_(2) * (x(i)*x(j)*m_inner_product(q).split(q,j|k) + Hat(x));
        return retval;
    }

    Scalar_ m_constant_term;
    DualOfV m_linear_term;
    Sym2_DualOfV m_quadratic_term;
    typename Identity_f<BasedVectorSpace_,Scalar_>::T m_identity;
    typename InnerProduct_f<BasedVectorSpace_,StandardInnerProduct,Scalar_>::T m_inner_product;
    typename InnerProduct_f<DualOfBasedVectorSpace,StandardInnerProduct,Scalar_>::T m_inner_product_inverse;
};

int main (int argc, char **argv)
{
    typedef long double Scalar;
    static Uint32 const DIM = 3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,DIM,Generic>,OrthonormalBasis_c<Generic> > BasedVectorSpace;
    typedef ImplementationOf_t<BasedVectorSpace,Scalar> V;

    static Uint32 const SAMPLES = 100;
    std::cerr << "QuadraticFunction_t\n";
    for (Uint32 j = 0; j < 10; ++j)
    {
        Scalar big_o_bound(0);
        QuadraticFunction_t<BasedVectorSpace,Scalar> helpy;
        for (Uint32 i = 0; i < SAMPLES; ++i)
        {
            V at_point(Static<WithoutInitialization>::SINGLETON);
            randomize<StandardInnerProduct>(at_point, Scalar(0), Scalar(10));
            big_o_bound = std::max(big_o_bound, helpy.verify_gradient(at_point));
        }
        std::cerr << "max big-o-bound for gradient verification = " << big_o_bound << '\n';
        big_o_bound = Scalar(0);
        for (Uint32 i = 0; i < SAMPLES; ++i)
        {
            V at_point(Static<WithoutInitialization>::SINGLETON);
            randomize<StandardInnerProduct>(at_point, Scalar(0), Scalar(10));
            big_o_bound = std::max(big_o_bound, helpy.verify_hessian(at_point));
        }
        std::cerr << "max big-o-bound for hessian verification = " << big_o_bound << '\n';
    }
    std::cerr << '\n';
    std::cerr << "SombreroFunction_t\n";
    for (Uint32 j = 0; j < 10; ++j)
    {
        Scalar big_o_bound(0);
        SombreroFunction_t<BasedVectorSpace,Scalar> helpy;
        for (Uint32 i = 0; i < SAMPLES; ++i)
        {
            V at_point(Static<WithoutInitialization>::SINGLETON);
            randomize<StandardInnerProduct>(at_point, Scalar(0), Scalar(10));
            big_o_bound = std::max(big_o_bound, helpy.verify_gradient(at_point));
        }
        std::cerr << "max big-o-bound for gradient verification = " << big_o_bound << '\n';
        big_o_bound = Scalar(0);
        for (Uint32 i = 0; i < SAMPLES; ++i)
        {
            V at_point(Static<WithoutInitialization>::SINGLETON);
            randomize<StandardInnerProduct>(at_point, Scalar(0), Scalar(10));
            big_o_bound = std::max(big_o_bound, helpy.verify_hessian(at_point));
        }
        std::cerr << "max big-o-bound for hessian verification = " << big_o_bound << '\n';
    }
    return 0;
}
