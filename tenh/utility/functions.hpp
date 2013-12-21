// ///////////////////////////////////////////////////////////////////////////
// tenh/utility/optimization.hpp by Ted Nitz, created 2013/12/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_UTILITY_FUNCTIONS_HPP_
#define TENH_UTILITY_FUNCTIONS_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/innerproduct.hpp"
#include "tenh/utility/optimization.hpp"

namespace Tenh {

// for arbitrary codomain
template <typename ParameterSpace_, typename CodomainSpace_, typename Scalar_>
struct FunctionObjectType_m
{
  typedef typename DualOf_f<ParameterSpace_>::T DualOfBasedVectorSpace;
  typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfBasedVectorSpace> Sym2Dual;
  typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<CodomainSpace_,
                                             TypeList_t<DualOfBasedVectorSpace> > > Differential1;
  typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<CodomainSpace_,
                                             TypeList_t<Sym2Dual> > > Differential2;
  typedef ParameterSpace_ Domain;
  typedef CodomainSpace_ CoDomain;
  typedef Scalar_ Scalar;

  typedef ImplementationOf_t<Domain,Scalar_,UseMemberArray> V;
  typedef ImplementationOf_t<DualOfBasedVectorSpace,Scalar_,UseMemberArray> DualOfV;
  typedef ImplementationOf_t<Sym2Dual,Scalar_,UseMemberArray> Sym2_DualOfV;
  typedef ImplementationOf_t<Domain,Scalar_,UseMemberArray> In;
  typedef ImplementationOf_t<CoDomain,Scalar_,UseMemberArray> Out;
  typedef ImplementationOf_t<Differential1,Scalar_,UseMemberArray> D1;
  typedef ImplementationOf_t<Differential2,Scalar_,UseMemberArray> D2;
};

// template specialization for when CodomainSpace_ is Scalar_
template <typename ParameterSpace_, typename Scalar_>
struct FunctionObjectType_m<ParameterSpace_,Scalar_,Scalar_>
{
  typedef typename DualOf_f<ParameterSpace_>::T DualOfBasedVectorSpace;
  typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfBasedVectorSpace> Sym2Dual;
  typedef DualOfBasedVectorSpace Differential1;
  typedef Sym2Dual Differential2;

  typedef ParameterSpace_ Domain;
  typedef Scalar_ CoDomain;
  typedef Scalar_ Scalar;

  typedef ImplementationOf_t<ParameterSpace_,Scalar_,UseMemberArray> V;
  typedef ImplementationOf_t<DualOfBasedVectorSpace,Scalar_,UseMemberArray> DualOfV;
  typedef ImplementationOf_t<Sym2Dual,Scalar_,UseMemberArray> Sym2_DualOfV;
  typedef ImplementationOf_t<ParameterSpace_,Scalar_,UseMemberArray> In;
  typedef Scalar_ Out;
  typedef DualOfV D1;
  typedef Sym2_DualOfV D2;
};

// for arbitrary codomain
template <typename ParameterSpace_, typename CodomainSpace_, typename CodomainInnerProductId_, typename Scalar_, typename FunctionObject_>
struct TaylorPolynomialVerifier_t
{
  typedef FunctionObjectType_m<ParameterSpace_,CodomainSpace_,Scalar_> FunctionObjectType;

  typedef typename FunctionObjectType::DualOfBasedVectorSpace DualOfBasedVectorSpace;
  typedef typename FunctionObjectType::Sym2Dual Sym2Dual;
  typedef typename FunctionObjectType::Differential1 Differential1;
  typedef typename FunctionObjectType::Differential2 Differential2;
  typedef typename FunctionObjectType::Domain Domain;
  typedef typename FunctionObjectType::CoDomain CoDomain;
  typedef typename FunctionObjectType::Scalar Scalar;
  typedef typename FunctionObjectType::V V;
  typedef typename FunctionObjectType::DualOfV DualOfV;
  typedef typename FunctionObjectType::Sym2_DualOfV Sym2_DualOfV;
  typedef typename FunctionObjectType::In In;
  typedef typename FunctionObjectType::Out Out;
  typedef typename FunctionObjectType::D1 D1;
  typedef typename FunctionObjectType::D2 D2;

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Out evaluate_0th_order (FunctionObject_ const &o,
                          Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                          Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &evaluation_point) const
  {
    return o.function(based_at_point);
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Out evaluate_1st_order (FunctionObject_ const &o,
                          Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                          Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &evaluation_point) const
  {
    AbstractIndex_c<'i'> i;
    V delta(Static<WithoutInitialization>::SINGLETON);

    delta(i).no_alias() = evaluation_point(i) - based_at_point(i);

    return evaluate_1st_order_via_delta(o, based_at_point, delta);
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Out evaluate_2nd_order (FunctionObject_ const &o,
                          Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                          Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &evaluation_point) const
  {
    AbstractIndex_c<'i'> i;
    Out retval(Static<WithoutInitialization>::SINGLETON);
    V delta(Static<WithoutInitialization>::SINGLETON);

    delta(i).no_alias() = evaluation_point(i) - based_at_point(i);

    return evaluate_2nd_order_via_delta(o, based_at_point, delta);
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Out evaluate_0th_order_via_delta (FunctionObject_ const &o,
                                    Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                    Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &delta) const
  {
    return o.function(based_at_point);
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Out evaluate_1st_order_via_delta (FunctionObject_ const &o,
                                    Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                    Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &delta) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    Out retval(Static<WithoutInitialization>::SINGLETON);

    retval(i).no_alias() = o.function(based_at_point)(i)
                           + o.D_function(based_at_point)(i*j)*delta(j);

    return retval;
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Out evaluate_2nd_order_via_delta (FunctionObject_ const &o,
                                    Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                    Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &delta) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'p'> p;
    Out retval(Static<WithoutInitialization>::SINGLETON);

    retval(i).no_alias() = o.function(based_at_point)(i)
                           + o.D_function(based_at_point)(i*j)*delta(j)
                           + o.D2_function(based_at_point)(i*p).split(p,j*k)*delta(j)*delta(k)/2;

    return retval;
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE_>
  Scalar_ verify_gradient (FunctionObject_ const &o,
                           Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE_> const &at_point) const
  {
    Scalar_ big_o_bound(0);
    static Uint32 const SAMPLES = 100;
    for (Uint32 i = 0; i < SAMPLES; ++i)
    {
      V delta(Static<WithoutInitialization>::SINGLETON);
      V evaluation_point(Static<WithoutInitialization>::SINGLETON);
      AbstractIndex_c<'j'> j;

      randomize<StandardInnerProduct>(delta, Scalar_(1)/100, Scalar_(2)/100);
      evaluation_point(j).no_alias() = at_point(j) + delta(j);
      // Scalar_ actual_function_value(function(at_point + delta)); // TODO: make this work?
      Out actual_function_value(o.function(evaluation_point));

      // Scalar_ estimated_function_value(evaluate_1st_order(at_point, evaluation_point));
      Out estimated_function_value(evaluate_1st_order_via_delta(o, at_point, delta));

      // std::cerr << FORMAT_VALUE(actual_function_value) << ", " << FORMAT_VALUE(estimated_function_value) << '\n';
      Out error(Static<WithoutInitialization>::SINGLETON);
      error(j).no_alias() = actual_function_value(j) - estimated_function_value(j);
      Scalar_ asymptotic_ratio = norm<CodomainInnerProductId_>(error)
                                 / squared_norm<StandardInnerProduct>(delta);

      big_o_bound = std::max(big_o_bound, asymptotic_ratio);
    }
    //std::cerr << "verify_gradient(" << FORMAT_VALUE(at_point) << "): " << FORMAT_VALUE(big_o_bound) << '\n';
    return big_o_bound;
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE_>
  Scalar_ verify_hessian (FunctionObject_ const &o,
                          Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE_> const &at_point) const
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
      evaluation_point(j).no_alias() = at_point(j) + delta(j);

      // Scalar_ actual_function_value(function(at_point + delta)); // TODO: make this work?
      Out actual_function_value(o.function(evaluation_point));

      // Scalar_ estimated_function_value(evaluate_2nd_order(at_point, evaluation_point));
      Out estimated_function_value(evaluate_2nd_order_via_delta(o, at_point, delta));

      Out error(Static<WithoutInitialization>::SINGLETON);
      error(j).no_alias() = actual_function_value(j) - estimated_function_value(j);
      // std::cerr << FORMAT_VALUE(actual_function_value) << ", " << FORMAT_VALUE(estimated_function_value) << '\n';
      Scalar_ asymptotic_ratio = norm<CodomainInnerProductId_>(error)
                                 / cube(norm<StandardInnerProduct>(delta));
      // squared_norm<StandardInnerProduct>(delta);

      big_o_bound = std::max(big_o_bound, asymptotic_ratio);
      // std::cerr << FORMAT_VALUE(at_point) << ", " << FORMAT_VALUE(norm<StandardInnerProduct>(delta)) << ", " << FORMAT_VALUE(big_o_bound) << '\n';
    }
    // std::cerr << "verify_hessian(" << FORMAT_VALUE(at_point) << "): " << FORMAT_VALUE(big_o_bound) << '\n';
    return big_o_bound;
  }
};

// for when CodomainSpace_ is Scalar_
template <typename ParameterSpace_, typename Scalar_, typename FunctionObject_>
struct TaylorPolynomialVerifier_t<ParameterSpace_,Scalar_,StandardInnerProduct,Scalar_,FunctionObject_>
{
  typedef FunctionObjectType_m<ParameterSpace_,Scalar_,Scalar_> FunctionObjectType;

  typedef typename FunctionObjectType::DualOfBasedVectorSpace DualOfBasedVectorSpace;
  typedef typename FunctionObjectType::Sym2Dual Sym2Dual;
  typedef typename FunctionObjectType::Differential1 Differential1;
  typedef typename FunctionObjectType::Differential2 Differential2;
  typedef typename FunctionObjectType::Domain Domain;
  typedef typename FunctionObjectType::CoDomain CoDomain;
  typedef typename FunctionObjectType::Scalar Scalar;
  typedef typename FunctionObjectType::V V;
  typedef typename FunctionObjectType::DualOfV DualOfV;
  typedef typename FunctionObjectType::Sym2_DualOfV Sym2_DualOfV;
  typedef typename FunctionObjectType::In In;
  typedef typename FunctionObjectType::Out Out;
  typedef typename FunctionObjectType::D1 D1;
  typedef typename FunctionObjectType::D2 D2;


  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Scalar_ evaluate_0th_order (FunctionObject_ const &o,
                              Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                              Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &evaluation_point) const
  {
    return o.function(based_at_point);
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Scalar_ evaluate_1st_order (FunctionObject_ const &o,
                              Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                              Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &evaluation_point) const
  {
    AbstractIndex_c<'i'> i;
    V delta(Static<WithoutInitialization>::SINGLETON);
    delta(i).no_alias() = evaluation_point(i) - based_at_point(i);
    return evaluate_1st_order_via_delta(o, based_at_point, delta);
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Scalar_ evaluate_2nd_order (FunctionObject_ const &o,
                              Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                              Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &evaluation_point) const
  {
    AbstractIndex_c<'i'> i;
    V delta(Static<WithoutInitialization>::SINGLETON);

    delta(i).no_alias() = evaluation_point(i) - based_at_point(i);

    return evaluate_2nd_order_via_delta(o, based_at_point, delta);
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Scalar_ evaluate_0th_order_via_delta (FunctionObject_ const &o,
                                        Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                        Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &delta) const
  {
    return o.function(based_at_point);
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Scalar_ evaluate_1st_order_via_delta (FunctionObject_ const &o,
                                        Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                        Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &delta) const
  {
    AbstractIndex_c<'i'> i;

    return o.function(based_at_point)
           + o.D_function(based_at_point)(i)*delta(i);
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE0_,
  typename Derived1_, bool COMPONENTS_ARE_IMMUTABLE1_>
  Scalar_ evaluate_2nd_order_via_delta (FunctionObject_ const &o,
                                        Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE0_> const &based_at_point,
                                        Vector_i<Derived1_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE1_> const &delta) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'p'> p;

    return o.function(based_at_point)
           + o.D_function(based_at_point)(i)*delta(i)
           + o.D2_function(based_at_point)(p).split(p,i*j)*delta(i)*delta(j)/2;
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE_>
  Scalar_ verify_gradient (FunctionObject_ const &o,
                           Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE_> const &at_point) const
  {
    Scalar_ big_o_bound(0);
    static Uint32 const SAMPLES = 10;
    for (Uint32 i = 0; i < SAMPLES; ++i)
    {
      V delta(Static<WithoutInitialization>::SINGLETON);
      V evaluation_point(Static<WithoutInitialization>::SINGLETON);
      AbstractIndex_c<'j'> j;

      randomize<StandardInnerProduct>(delta, Scalar_(1)/100, Scalar_(10)/100);
      evaluation_point(j).no_alias() = at_point(j) + delta(j);
      // Scalar_ actual_function_value(function(at_point + delta)); // TODO: make this work?
      Scalar_ actual_function_value(o.function(evaluation_point));
      // Scalar_ estimated_function_value(evaluate_1st_order(at_point, evaluation_point));
      Scalar_ estimated_function_value(evaluate_1st_order_via_delta(o, at_point, delta));
      Scalar_ error(actual_function_value - estimated_function_value);
      // std::cerr << FORMAT_VALUE(actual_function_value) << ", " << FORMAT_VALUE(estimated_function_value) << '\n';
      Scalar_ asymptotic_ratio = fabs(error)
                                 / squared_norm<StandardInnerProduct>(delta);
      // norm<StandardInnerProduct>(delta);
      big_o_bound = std::max(big_o_bound, asymptotic_ratio);
    }
    // std::cerr << "verify_gradient(" << FORMAT_VALUE(at_point) << "): " << FORMAT_VALUE(big_o_bound) << '\n';
    return big_o_bound;
  }

  template <typename Derived0_, bool COMPONENTS_ARE_IMMUTABLE_>
  Scalar_ verify_hessian (FunctionObject_ const &o,
                          Vector_i<Derived0_,Scalar_,ParameterSpace_,COMPONENTS_ARE_IMMUTABLE_> const &at_point) const
  {
    Scalar_ big_o_bound(0);
    static Uint32 const SAMPLES = 100;
    for (Uint32 i = 0; i < SAMPLES; ++i)
    {
      V delta(Static<WithoutInitialization>::SINGLETON);
      V evaluation_point(Static<WithoutInitialization>::SINGLETON);
      AbstractIndex_c<'j'> j;
      // use a solid annulus of radius [1/100, 2/200] around at_point
      randomize<StandardInnerProduct>(delta, Scalar_(1)/100, Scalar_(10)/100);
      evaluation_point(j) = at_point(j) + delta(j);
      // Scalar_ actual_function_value(function(at_point + delta)); // TODO: make this work?
      Scalar_ actual_function_value(o.function(evaluation_point));
      // Scalar_ estimated_function_value(evaluate_2nd_order(at_point, evaluation_point));
      Scalar_ estimated_function_value(evaluate_2nd_order_via_delta(o, at_point, delta));
      Scalar_ error(actual_function_value - estimated_function_value);
      // std::cerr << FORMAT_VALUE(actual_function_value) << ", " << FORMAT_VALUE(estimated_function_value) << '\n';
      Scalar_ asymptotic_ratio = fabs(error)
                                 / cube(norm<StandardInnerProduct>(delta));
      // squared_norm<StandardInnerProduct>(delta);

      big_o_bound = std::max(big_o_bound, asymptotic_ratio);
      // std::cerr << FORMAT_VALUE(at_point) << ", " << FORMAT_VALUE(norm<StandardInnerProduct>(delta)) << ", " << FORMAT_VALUE(big_o_bound) << '\n';
    }
    //std::cerr << "verify_hessian(" << FORMAT_VALUE(at_point) << "): " << FORMAT_VALUE(big_o_bound) << '\n';
    return big_o_bound;
  }
};

template <typename FunctionObject_, typename CoDomainInnerProductId_>
struct TaylorPolynomialVerifier_f
{
  typedef TaylorPolynomialVerifier_t<typename FunctionObject_::Domain,
                                     typename FunctionObject_::CoDomain,
                                     CoDomainInnerProductId_,
                                     typename FunctionObject_::Scalar,
                                     FunctionObject_> T;
};

template <typename OuterFunctionType_, typename InnerFunctionType_>
struct FunctionComposition_t
{
private:
  enum { STATIC_ASSERT_IN_ENUM((TypesAreEqual_f<typename OuterFunctionType_::Domain, typename InnerFunctionType_::CoDomain>::V), TYPES_MUST_BE_EQUAL) };

public:
  typedef FunctionObjectType_m<typename InnerFunctionType_::Domain,
                               typename OuterFunctionType_::CoDomain,
                               typename InnerFunctionType_::Scalar> FunctionObjectType;

  typedef typename FunctionObjectType::DualOfBasedVectorSpace DualOfBasedVectorSpace;
  typedef typename FunctionObjectType::Sym2Dual Sym2Dual;
  typedef typename FunctionObjectType::Differential1 Differential1;
  typedef typename FunctionObjectType::Differential2 Differential2;
  typedef typename FunctionObjectType::Domain Domain;
  typedef typename FunctionObjectType::CoDomain CoDomain;
  typedef typename FunctionObjectType::Scalar Scalar;
  typedef typename FunctionObjectType::V V;
  typedef typename FunctionObjectType::DualOfV DualOfV;
  typedef typename FunctionObjectType::Sym2_DualOfV Sym2_DualOfV;
  typedef typename FunctionObjectType::In In;
  typedef typename FunctionObjectType::Out Out;
  typedef typename FunctionObjectType::D1 D1;
  typedef typename FunctionObjectType::D2 D2;

  FunctionComposition_t (OuterFunctionType_ const &outer, InnerFunctionType_ const &inner)
  :
  m_outer(outer),
  m_inner(inner)
  { }

  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
  Out function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    return m_outer.function(m_inner.function(x));
  }

  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_, class = int, class = typename EnableIf_f<TypesAreEqual_f<typename OuterFunctionType_::Out, Scalar>::V>::T>
  D1 D_function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;

    D1 retval(Static<WithoutInitialization>::SINGLETON);
    retval(j) = m_outer.D_function(m_inner.function(x))(i) * m_inner.D_function(x)(i*j);

    return retval;
  }

  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_, class = typename EnableIf_f<!TypesAreEqual_f<typename OuterFunctionType_::Out, Scalar>::V>::T>
  D1 D_function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;

    D1 retval(Static<WithoutInitialization>::SINGLETON);
    retval(i*k) = m_outer.D_function(m_inner.function(x))(i*j) * m_inner.D_function(x)(j*k);

    return retval;
  }


  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_, class = int,
  class = typename EnableIf_f<TypesAreEqual_f<typename OuterFunctionType_::Out, Scalar>::V>::T>
  D2 D2_function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;
    AbstractIndex_c<'q'> q;
    D2 retval(Static<WithoutInitialization>::SINGLETON);

    typename InnerFunctionType_::Out inner_function_value(m_inner.function(x));
    typename InnerFunctionType_::D1 D_inner_function(m_inner.D_function(x));

    // double chain rule
    retval(q) = (  m_outer.D2_function(inner_function_value).split(i*j)
                 * D_inner_function(i*k)
                 * D_inner_function(j*l))
                .bundle(k*l,typename D2::Concept(),q)
                + m_outer.D_function(inner_function_value)(i)
                  * m_inner.D2_function(x)(i*q);

    return retval;
  }

  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_,
  class = typename EnableIf_f<!TypesAreEqual_f<typename OuterFunctionType_::Out, Scalar>::V>::T>
  D2 D2_function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    AbstractIndex_c<'c'> c;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    AbstractIndex_c<'i'> i;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    AbstractIndex_c<'j'> j;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    AbstractIndex_c<'k'> k;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    AbstractIndex_c<'l'> l;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    AbstractIndex_c<'p'> p;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    AbstractIndex_c<'q'> q;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    D2 retval(Static<WithoutInitialization>::SINGLETON);;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    typename InnerFunctionType_::Out inner_function_value(m_inner.function(x));;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    typename InnerFunctionType_::D1 D_inner_function(m_inner.D_function(x));;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    // double chain rule;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    retval(c*q) = ( m_outer.D2_function(inner_function_value)(c*p).split(p,i*j)
                   * D_inner_function(i*k)
                   * D_inner_function(j*l))
                  .bundle(k*l,Sym2Dual(),q)
                  + m_outer.D_function(inner_function_value)(c*i)
                   * m_inner.D2_function(x)(i*q);;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    return retval;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  }

private:
  OuterFunctionType_ const &m_outer;
  InnerFunctionType_ const &m_inner;
};

template <typename LeftFunctionType_, typename RightFunctionType_>
struct FunctionDirectSum_t
{
  typedef DirectSumOfBasedVectorSpaces_c<TypeList_t<typename LeftFunctionType_::Domain,
                                         TypeList_t<typename RightFunctionType_::Domain> > > DomainType;
  typedef DirectSumOfBasedVectorSpaces_c<TypeList_t<typename LeftFunctionType_::CoDomain,
                                         TypeList_t<typename RightFunctionType_::CoDomain> > > CoDomainType;
  typedef FunctionObjectType_m<DomainType,CoDomainType,typename LeftFunctionType_::Scalar> FunctionObjectType;

  typedef typename FunctionObjectType::DualOfBasedVectorSpace DualOfBasedVectorSpace;
  typedef typename FunctionObjectType::Sym2Dual Sym2Dual;
  typedef typename FunctionObjectType::Differential1 Differential1;
  typedef typename FunctionObjectType::Differential2 Differential2;
  typedef typename FunctionObjectType::Domain Domain;
  typedef typename FunctionObjectType::CoDomain CoDomain;
  typedef typename FunctionObjectType::Scalar Scalar;
  typedef typename FunctionObjectType::V V;
  typedef typename FunctionObjectType::DualOfV DualOfV;
  typedef typename FunctionObjectType::Sym2_DualOfV Sym2_DualOfV;
  typedef typename FunctionObjectType::In In;
  typedef typename FunctionObjectType::Out Out;
  typedef typename FunctionObjectType::D1 D1;
  typedef typename FunctionObjectType::D2 D2;

  typedef ImplementationOf_t<typename LeftFunctionType_::Domain, Scalar> Left;
  typedef typename LeftFunctionType_::D1 Left_D1;
  typedef typename LeftFunctionType_::D2 Left_D2;
  typedef ImplementationOf_t<typename RightFunctionType_::Domain, Scalar> Right;
  typedef typename RightFunctionType_::D1 Right_D1;
  typedef typename RightFunctionType_::D2 Right_D2;

  FunctionDirectSum_t(LeftFunctionType_ const & left, RightFunctionType_ const & right)
  :
  m_left(left),
  m_right(right)
  { }

  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
  Out function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    Out retval(Static<WithoutInitialization>::SINGLETON);
    Left left(Static<WithoutInitialization>::SINGLETON);
    Right right(Static<WithoutInitialization>::SINGLETON);

    left(i) = x.as_derived().template el<0>()(i);
    right(i) = x.as_derived().template el<1>()(i);

    retval.template el<0>()(i) = m_left.function(left)(i);
    retval.template el<1>()(i) = m_right.function(right)(i);

    return retval;
  }

  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
  D1 D_function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    AbstractIndex_c<'a'> a;
    D1 retval(Static<WithoutInitialization>::SINGLETON);
    Left left(Static<WithoutInitialization>::SINGLETON);
    Left_D1 left_d1(Static<WithoutInitialization>::SINGLETON);
    Right right(Static<WithoutInitialization>::SINGLETON);
    Right_D1 right_d1(Static<WithoutInitialization>::SINGLETON);

    left(a) = x.as_derived().template el<0>()(a);
    right(a) = x.as_derived().template el<1>()(a);

    left_d1(a) = m_left.D_function(left)(a);
    right_d1(a) = m_right.D_function(right)(a);

    //retval((i+j)*(k+l)) = right_d1(i*k) + left_d1(j*l);

    for (int i = 0; i < DimensionOf_f<CoDomain>::V; ++i)
    {
      for (int j = 0; j < DimensionOf_f<DualOfBasedVectorSpace>::V; ++j)
      {
        if (i < DimensionOf_f<typename LeftFunctionType_::CoDomain>::V && j < DimensionOf_f<typename LeftFunctionType_::DualOfBasedVectorSpace>::V)
        {
          retval[typename D1::MultiIndex(i,j,CHECK_RANGE)] = left_d1[typename Left_D1::MultiIndex(i,j,CHECK_RANGE)];
        }
        else if (i >= DimensionOf_f<typename LeftFunctionType_::CoDomain>::V && j >= DimensionOf_f<typename LeftFunctionType_::DualOfBasedVectorSpace>::V)
        {
          retval[typename D1::MultiIndex(i,j,CHECK_RANGE)] =
          right_d1[typename Right_D1::MultiIndex(i - DimensionOf_f<typename LeftFunctionType_::CoDomain>::V,
                                                 j - DimensionOf_f<typename LeftFunctionType_::DualOfBasedVectorSpace>::V,
                                                 CHECK_RANGE)];
        }
        else
        {
          retval[typename D1::MultiIndex(i,j,CHECK_RANGE)] = Scalar(0);
        }
      }
    }
    return retval;
  }

  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
  D2 D2_function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<CoDomain,
                                               TypeList_t<DualOfBasedVectorSpace,
                                               TypeList_t<DualOfBasedVectorSpace> > > > SplitD2Type;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<typename LeftFunctionType_::CoDomain,
                                               TypeList_t<typename LeftFunctionType_::DualOfBasedVectorSpace,
                                               TypeList_t<typename LeftFunctionType_::DualOfBasedVectorSpace> > > > LeftSplitD2Type;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<typename RightFunctionType_::CoDomain,
                                               TypeList_t<typename RightFunctionType_::DualOfBasedVectorSpace,
                                               TypeList_t<typename RightFunctionType_::DualOfBasedVectorSpace> > > > RightSplitD2Type;
    typedef ImplementationOf_t<SplitD2Type, Scalar> SplitD2;
    typedef ImplementationOf_t<LeftSplitD2Type, Scalar> LeftSplitD2;
    typedef ImplementationOf_t<RightSplitD2Type, Scalar> RightSplitD2;

    AbstractIndex_c<'a'> a;
    AbstractIndex_c<'b'> b;
    AbstractIndex_c<'c'> c;
    AbstractIndex_c<'d'> d;

    D2 retval(Static<WithoutInitialization>::SINGLETON);
    SplitD2 tmp(Static<WithoutInitialization>::SINGLETON);
    Left left(Static<WithoutInitialization>::SINGLETON);
    Left_D2 left_d2(Static<WithoutInitialization>::SINGLETON);
    LeftSplitD2 left_split_d2(Static<WithoutInitialization>::SINGLETON);
    Right right(Static<WithoutInitialization>::SINGLETON);
    Right_D2 right_d2(Static<WithoutInitialization>::SINGLETON);
    RightSplitD2 right_split_d2(Static<WithoutInitialization>::SINGLETON);

    left(a) = x.as_derived().template el<0>()(a);
    right(a) = x.as_derived().template el<1>()(a);

    left_d2(a) = m_left.D2_function(left)(a);
    right_d2(a) = m_right.D2_function(right)(a);

    left_split_d2(a*b*c) = left_d2(a*d).split(d,b*c);
    right_split_d2(a*b*c) = right_d2(a*d).split(d,b*c);

    for (int i = 0; i < DimensionOf_f<CoDomain>::V; ++i)
    {
      for (int j = 0; j < DimensionOf_f<DualOfBasedVectorSpace>::V; ++j)
      {
        for (int k = 0; k < DimensionOf_f<DualOfBasedVectorSpace>::V; ++k)
        {
          if (i < DimensionOf_f<typename LeftFunctionType_::CoDomain>::V && j < DimensionOf_f<typename LeftFunctionType_::DualOfBasedVectorSpace>::V && k < DimensionOf_f<typename LeftFunctionType_::DualOfBasedVectorSpace>::V)
          {
            tmp[typename SplitD2::MultiIndex(i,j,k,CHECK_RANGE)] = left_split_d2[typename LeftSplitD2::MultiIndex(i,j,k,CHECK_RANGE)];
          }
          else if (i >= DimensionOf_f<typename LeftFunctionType_::CoDomain>::V && j >= DimensionOf_f<typename LeftFunctionType_::DualOfBasedVectorSpace>::V && k >= DimensionOf_f<typename LeftFunctionType_::DualOfBasedVectorSpace>::V)
          {
            tmp[typename SplitD2::MultiIndex(i,j,k,CHECK_RANGE)]
            = right_split_d2[typename RightSplitD2::MultiIndex(i - DimensionOf_f<typename LeftFunctionType_::CoDomain>::V,
                                                               j - DimensionOf_f<typename LeftFunctionType_::DualOfBasedVectorSpace>::V,
                                                               k - DimensionOf_f<typename LeftFunctionType_::DualOfBasedVectorSpace>::V,
                                                               CHECK_RANGE)];
          }
          else
          {
            tmp[typename SplitD2::MultiIndex(i,j,k,CHECK_RANGE)] = Scalar(0);
          }
        }
      }
    }
    retval(a*b) = tmp(a*c*d).bundle(c*d,Sym2Dual(),b);
    return retval;
  }

private:
  LeftFunctionType_ const &m_left;
  RightFunctionType_ const &m_right;
};


template <typename BasedVectorSpace_,typename Scalar_>
struct DiagonalFunction_t
{
  typedef DirectSumOfBasedVectorSpaces_c<TypeList_t<BasedVectorSpace_, TypeList_t<BasedVectorSpace_> > > CoDomainType;
  typedef FunctionObjectType_m<BasedVectorSpace_, CoDomainType, Scalar_> FunctionObjectType;

  typedef typename FunctionObjectType::DualOfBasedVectorSpace DualOfBasedVectorSpace;
  typedef typename FunctionObjectType::Sym2Dual Sym2Dual;
  typedef typename FunctionObjectType::Differential1 Differential1;
  typedef typename FunctionObjectType::Differential2 Differential2;
  typedef typename FunctionObjectType::Domain Domain;
  typedef typename FunctionObjectType::CoDomain CoDomain;
  typedef typename FunctionObjectType::Scalar Scalar;
  typedef typename FunctionObjectType::V V;
  typedef typename FunctionObjectType::DualOfV DualOfV;
  typedef typename FunctionObjectType::Sym2_DualOfV Sym2_DualOfV;
  typedef typename FunctionObjectType::In In;
  typedef typename FunctionObjectType::Out Out;
  typedef typename FunctionObjectType::D1 D1;
  typedef typename FunctionObjectType::D2 D2;

  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
  Out function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    Out retval(Static<WithoutInitialization>::SINGLETON);

    retval.template el<0>()(i) = x(i);
    retval.template el<1>()(i) = x(i);
    
    return retval;
  }

  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
  D1 D_function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    D1 retval(Static<WithoutInitialization>::SINGLETON);

    for (int i = 0; i < DimensionOf_f<CoDomain>::V; ++i)
    {
      for (int j = 0; j < DimensionOf_f<DualOfBasedVectorSpace>::V; ++j)
      {
        if (i == j || i - DimensionOf_f<DualOfBasedVectorSpace>::V == j)
        {
          retval[typename D1::MultiIndex(i,j,CHECK_RANGE)] = Scalar(1);
        }
        else
        {
          retval[typename D1::MultiIndex(i,j,CHECK_RANGE)] = Scalar(0);
        }
      }
    }
    return retval;
  }

  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
  D2 D2_function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    return D2(fill_with(0));
  }
};


template <typename BasedVectorSpace_, typename Scalar_>
struct IdentityFunction_t
{
  typedef FunctionObjectType_m<BasedVectorSpace_, BasedVectorSpace_, Scalar_> FunctionObjectType;

  typedef typename FunctionObjectType::DualOfBasedVectorSpace DualOfBasedVectorSpace;
  typedef typename FunctionObjectType::Sym2Dual Sym2Dual;
  typedef typename FunctionObjectType::Differential1 Differential1;
  typedef typename FunctionObjectType::Differential2 Differential2;
  typedef typename FunctionObjectType::Domain Domain;
  typedef typename FunctionObjectType::CoDomain CoDomain;
  typedef typename FunctionObjectType::Scalar Scalar;
  typedef typename FunctionObjectType::V V;
  typedef typename FunctionObjectType::DualOfV DualOfV;
  typedef typename FunctionObjectType::Sym2_DualOfV Sym2_DualOfV;
  typedef typename FunctionObjectType::In In;
  typedef typename FunctionObjectType::Out Out;
  typedef typename FunctionObjectType::D1 D1;
  typedef typename FunctionObjectType::D2 D2;

  IdentityFunction_t ()
  :
  m_D1(Static<WithoutInitialization>::SINGLETON)
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    
    typename Identity_f<BasedVectorSpace_, Scalar_>::T identity;
    
    m_D1(i*j) = identity(k).split(k,i*j);
  }
  
  
  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
  Out function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    return x.as_derived();
  }
  
  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
  D1 D_function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    return m_D1;
  }
  
  template <typename Derived_, bool COMPONENTS_ARE_IMMUTABLE_>
  D2 D2_function (Vector_i<Derived_,Scalar,Domain,COMPONENTS_ARE_IMMUTABLE_> const &x) const
  {
    return D2(FillWith_t<Scalar_>(0));
  }
  
private:
  
  D1 m_D1;
};


} // end of namespace Tenh

#endif // TENH_UTILITY_FUNCTIONS_HPP_
