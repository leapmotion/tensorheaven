// ///////////////////////////////////////////////////////////////////////////
// tenh/utility/cayley_transform.hpp by Ted Nitz, created 2013/12/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_UTILITY_CAYLEY_TRANSFORM_HPP_
#define TENH_UTILITY_CAYLEY_TRANSFORM_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/identity.hpp"
#include "tenh/implementation/innerproduct.hpp"
#include "tenh/utility/functions.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// "hat" Lie algebra morphism from (V, \times) to (so(3), [.,.]), where
// so(3) \subset V \otimes V*, where V = R^3.  This has the property that
// hat(x)*y = x \times y (cross product).
// ///////////////////////////////////////////////////////////////////////////

namespace ComponentGeneratorEvaluator {

  template <typename BasedVectorSpace_, typename HatMorphism_, typename Scalar_>
  Scalar_ hat (ComponentIndex_t<DimensionOf_f<HatMorphism_>::V> const &i)
  {
    typedef typename DualOf_f<BasedVectorSpace_>::T DualOfBasedVectorSpace;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<BasedVectorSpace_,DualOfBasedVectorSpace>> EndomorphismOfBasedVectorSpace;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<EndomorphismOfBasedVectorSpace,DualOfBasedVectorSpace>> HatMorphism;
    STATIC_ASSERT_TYPES_ARE_EQUAL(HatMorphism, HatMorphism_);

    // with EndomorphismOfBasedVectorSpaces indexed as
    // [ [ 0 1 2 ]
    //   [ 3 4 5 ]
    //   [ 6 7 8 ] ]
    // it follows that the hat morphism is indexed as
    // [ [  0  1  2  3  4  5  6  7  8 ]
    //   [  9 10 11 12 13 14 15 16 17 ]
    //   [ 18 19 20 21 22 23 24 25 26 ] ]
    // and the hat morphism's matrix looks like
    // [ [  0  0  0  0  0 -1  0  1  0 ]
    //   [  0  0  1  0  0  0 -1  0  0 ]
    //   [  0 -1  0  1  0  0  0  0  0 ] ]
    // so the 1 constants go in slots 7, 11, 21, while the -1 constants go in slots 5, 15, 19.
    switch (i.value())
    {
      case 7:
      case 11:
      case 21:
        return Scalar_(1);
      case 5:
      case 15:
      case 19:
        return Scalar_(-1);
      default:
        return Scalar_(0);
    }
  }

} // end of namespace ComponentGeneratorEvaluator

struct HatId { static std::string type_as_string () { return "Hat"; } };

template <typename BasedVectorSpace_, typename Scalar_>
struct HatTensor_f
{
private:

  enum { STATIC_ASSERT_IN_ENUM((DimensionOf_f<BasedVectorSpace_>::V == 3), DIMENSION_MUST_BE_EXACTLY_3) };

  typedef typename DualOf_f<BasedVectorSpace_>::T DualOfBasedVectorSpace;
  typedef TensorProductOfBasedVectorSpaces_c<Typle_t<BasedVectorSpace_,DualOfBasedVectorSpace>> Endomorphism;
  typedef TensorProductOfBasedVectorSpaces_c<Typle_t<Endomorphism,DualOfBasedVectorSpace>> HatMorphism;

  typedef ComponentGenerator_t<Scalar_,
                               DimensionOf_f<HatMorphism>::V,
                               Tenh::ComponentGeneratorEvaluator::hat<BasedVectorSpace_,HatMorphism,Scalar_>,
                               HatId> ComponentGenerator;
  HatTensor_f();
public:
  typedef ImplementationOf_t<HatMorphism,Scalar_,UseProceduralArray_t<ComponentGenerator>> T;
};

// Lie algebra morphism from (R^3, \times) to (so(3), [.,.]) - as a 2-tensor
// TODO: figure out if this make sense to have for arbitrary 3-dimensional based vector spaces
template <typename Derived_, typename Scalar_, typename BasedVectorSpace_, ComponentQualifier COMPONENT_QUALIFIER_>
ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<Typle_t<BasedVectorSpace_,typename DualOf_f<BasedVectorSpace_>::T>>,
                                                      Scalar_,
                                                      UseMemberArray_t<ComponentsAreConst::FALSE>>
hat (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x)
{
  STATIC_ASSERT(DimensionOf_f<BasedVectorSpace_>::V == 3, DIMENSION_MUST_BE_EXACTLY_3);
  // for brevity
  typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<Typle_t<BasedVectorSpace_,typename DualOf_f<BasedVectorSpace_>::T>>,
                             Scalar_,
                             UseMemberArray_t<ComponentsAreConst::FALSE>> T;
  typedef typename Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_>::ComponentIndex c;
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

// ///////////////////////////////////////////////////////////////////////////
// end of "hat" stuff
// ///////////////////////////////////////////////////////////////////////////

template <typename BasedVectorSpace_, typename Scalar_>
struct J_t
{
private:

  enum { _ = Assert<DimensionOf_f<BasedVectorSpace_>::V == 3>::V };

public:

  typedef FunctionObjectType_m<BasedVectorSpace_,Scalar_,Scalar_> FunctionObjectType;

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

  J_t ()
  :
  m_D2(Static<WithoutInitialization>::SINGLETON)
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'p'> p;
    // this really should use the natural embedding of the diagonal (inner product) into Sym2Dual
    m_D2(p) = (Scalar_(-2)*m_inner_product.split(i*j)).bundle(i*j,typename D2::Concept(),p);
  }

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  Out function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    return Scalar_(1) - m_inner_product(x, x);
  }

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  D1 D_function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    D1 retval(Static<WithoutInitialization>::SINGLETON);
    retval(j).no_alias() = x(i)*m_D2.split(i*j); //Scalar_(-2)*x(i)*m_inner_product.split(i*j);
    return retval;
  }

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  D2 D2_function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    return m_D2;
  }

private:

  typename InnerProduct_f<BasedVectorSpace_,StandardInnerProduct,Scalar_>::T m_inner_product;
  D2 m_D2;
};

template <typename BasedVectorSpace_, typename Scalar_>
struct K_t
{
private:

  enum { _ = Assert<DimensionOf_f<BasedVectorSpace_>::V == 3>::V };

public:

  typedef FunctionObjectType_m<BasedVectorSpace_,Scalar_,Scalar_> FunctionObjectType;

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

  K_t ()
  :
  m_form(Static<WithoutInitialization>::SINGLETON)
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'p'> p;
    // this really should use the natural embedding of the diagonal (inner product) into Sym2Dual
    m_form(p) = (Scalar_(2)*m_inner_product.split(i*j)).bundle(i*j,typename D2::Concept(),p);
  }

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  Out function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    return Scalar_(1) / (Scalar_(1) + m_inner_product(x, x));
  }

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  D1 D_function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    D1 retval(Static<WithoutInitialization>::SINGLETON);
    retval(j).no_alias() = -sqr(function(x))*x(i)*m_form.split(i*j);
    return retval;
  }

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  D2 D2_function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    Scalar_ f_of_x(function(x));
    AbstractIndex_c<'a'> a;
    AbstractIndex_c<'b'> b;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'p'> p;
    D2 retval(Static<WithoutInitialization>::SINGLETON);
    retval(p).no_alias() = sqr(f_of_x)
                           * (  Scalar_(2) * f_of_x * (m_form.split(j*a)*x(a)*x(b)*m_form.split(b*k))
                             .bundle(j*k,typename D2::Concept(),p)
                           - m_form(p));
    return retval;
  }

private:

  typename InnerProduct_f<BasedVectorSpace_,StandardInnerProduct,Scalar_>::T m_inner_product;
  D2 m_form;
};

template <typename BasedVectorSpace_, typename Scalar_>
struct N_t
{
private:

  enum { _ = Assert<DimensionOf_f<BasedVectorSpace_>::V == 3>::V };

public:

  typedef TensorProductOfBasedVectorSpaces_c<Typle_t<BasedVectorSpace_,typename DualOf_f<BasedVectorSpace_>::T>> CodomainSpace;
  typedef FunctionObjectType_m<BasedVectorSpace_,CodomainSpace,Scalar_> FunctionObjectType;

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

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  Out function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'p'> p;
    Out retval(Static<WithoutInitialization>::SINGLETON);
    retval(i*j).no_alias() = m_J.function(x)*m_identity.split(i*j)
    + Scalar_(2) * (x(i)*m_inner_product.split(j*p)*x(p) + hat(x)(i*j));
    return retval;
  }

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  D1 D_function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'p'> p;
    AbstractIndex_c<'A'> A;
    AbstractIndex_c<'B'> B;
    D1 retval(Static<WithoutInitialization>::SINGLETON);
    retval(B*k).no_alias() = (  m_identity.split(i*j)*m_J.D_function(x)(k)
                              + Scalar_(2) * (  m_identity.split(i*k)*m_inner_product.split(j*p)*x(p)
                                              + x(i)*m_inner_product.split(j*k)
                                              + m_hat_tensor(A*k).split(A,i*j)))
                              .bundle(i*j,typename Out::Concept(),B);
    return retval;
  }

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  D2 D2_function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;
    AbstractIndex_c<'p'> p;
    AbstractIndex_c<'B'> B;
    AbstractIndex_c<'C'> C;
    D2 retval(Static<WithoutInitialization>::SINGLETON);
    retval(C*p).no_alias() = m_identity(B).split(B,C)*m_J.D2_function(x)(p)
                             + Scalar_(2)
                               * (  (m_identity.split(i*k)*m_inner_product.split(j*l))
                                 .bundle(i*j,typename Out::Concept(),C)
                                 .bundle(k*l,typename Sym2_DualOfV::Concept(),p)
                             + (m_identity.split(i*l)*m_inner_product.split(j*k))
                               .bundle(i*j,typename Out::Concept(),C)
                               .bundle(k*l,typename Sym2_DualOfV::Concept(),p));
    return retval;
  }

private:

  J_t<BasedVectorSpace_,Scalar_> m_J;
  typename Identity_f<BasedVectorSpace_,Scalar_>::T m_identity;
  typename InnerProduct_f<BasedVectorSpace_,StandardInnerProduct,Scalar_>::T m_inner_product;
  typename InnerProduct_f<DualOfBasedVectorSpace,StandardInnerProduct,Scalar_>::T m_inner_product_inverse;
  typename HatTensor_f<BasedVectorSpace_,Scalar_>::T m_hat_tensor;
};

template <typename BasedVectorSpace_, typename Scalar_>
struct CayleyTransform_t
{
private:

  enum { _ = Assert<DimensionOf_f<BasedVectorSpace_>::V == 3>::V };

public:

  typedef TensorProductOfBasedVectorSpaces_c<Typle_t<BasedVectorSpace_,typename DualOf_f<BasedVectorSpace_>::T>> CodomainSpace;
  typedef FunctionObjectType_m<BasedVectorSpace_,CodomainSpace,Scalar_> FunctionObjectType;

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

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  Out function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    Out retval(Static<WithoutInitialization>::SINGLETON);

    retval(i).no_alias() = m_K.function(x) * m_N.function(x)(i);

    return retval;
  }

  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  D1 D_function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    D1 retval(Static<WithoutInitialization>::SINGLETON);

    //retval(i*j).no_alias() = D_K(x)(j)*N(x)(i) + K(x)*D_N(x)(i*j); // this should work but Tenh complains about the ordering
    retval(i*j).no_alias() = m_N.function(x)(i)*m_K.D_function(x)(j) + m_K.function(x)*m_N.D_function(x).split(i*j);

    return retval;
  }
  template <typename Derived_, ComponentQualifier COMPONENT_QUALIFIER_>
  D2 D2_function (Vector_i<Derived_,Scalar_,BasedVectorSpace_,COMPONENT_QUALIFIER_> const &x) const
  {
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'p'> p;
    AbstractIndex_c<'C'> C;
    D2 retval(Static<WithoutInitialization>::SINGLETON);

    retval(C*p).no_alias() = m_N.function(x)(C)*m_K.D2_function(x)(p)
                             + (m_N.D_function(x)(C*k)*m_K.D_function(x)(j)).bundle(j*k,typename Sym2_DualOfV::Concept(),p)
                             + (m_K.D_function(x)(k)*m_N.D_function(x)(C*j)).bundle(j*k,typename Sym2_DualOfV::Concept(),p)
                             + m_K.function(x)*m_N.D2_function(x)(C*p);

    return retval;
  }

private:

  K_t<BasedVectorSpace_,Scalar_> m_K;
  N_t<BasedVectorSpace_,Scalar_> m_N;
  typename Identity_f<BasedVectorSpace_,Scalar_>::T m_identity;
  typename InnerProduct_f<BasedVectorSpace_,StandardInnerProduct,Scalar_>::T m_inner_product;
  typename InnerProduct_f<DualOfBasedVectorSpace,StandardInnerProduct,Scalar_>::T m_inner_product_inverse;
  typename HatTensor_f<BasedVectorSpace_,Scalar_>::T m_hat_tensor;
};

} // end of namespace Tenh

#endif // TENH_UTILITY_CAYLEY_TRANSFORM_HPP_
