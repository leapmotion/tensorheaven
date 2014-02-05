// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/tensorproduct.hpp by Victor Dods, created 2013/09/18
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_TENSORPRODUCT_HPP_
#define TENH_IMPLEMENTATION_TENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/scalarbased2tensorproduct.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vector.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// TensorProductOfProceduralVectors_f
// ///////////////////////////////////////////////////////////////////////////

template <typename ProceduralVectorImplementationTyple_> struct TensorProductOfProceduralVectors_f;

// base case (1-tensor product)

namespace ComponentGeneratorEvaluator {

template <typename ProceduralVectorImplementationTyple_, typename TensorProductOfBasedVectorSpaces_, typename Scalar_>
Scalar_ tensor_product_of_single_procedural_vector (ComponentIndex_t<DimensionOf_f<TensorProductOfBasedVectorSpaces_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<TensorProductOfBasedVectorSpaces_>::V> ComponentIndex;
    typedef typename ConceptOfEachTypeIn_f<ProceduralVectorImplementationTyple_>::T ConceptTyple;
    typedef TensorProductOfBasedVectorSpaces_c<ConceptTyple> TensorProductOfBasedVectorSpaces;
    STATIC_ASSERT_TYPES_ARE_EQUAL(TensorProductOfBasedVectorSpaces_, TensorProductOfBasedVectorSpaces);
    typedef typename Hippo::Head_f<ProceduralVectorImplementationTyple_>::T HeadImplementation;
    HeadImplementation head_implementation;
    AbstractIndex_c<'a'> a;
    return head_implementation(a)[MultiIndex_t<Typle_t<ComponentIndex>>(i)];
}

} // end of namespace ComponentGeneratorEvaluator

template <typename HeadProceduralVectorImplementationType_>
struct TensorProductOfProceduralVectors_f<Typle_t<HeadProceduralVectorImplementationType_> >
{
private:
    typedef Typle_t<HeadProceduralVectorImplementationType_> ProceduralVectorImplementationTyple_;
    typedef typename ConceptOfEachTypeIn_f<ProceduralVectorImplementationTyple_>::T ConceptTyple;
    typedef typename ScalarOfEachTypeIn_f<ProceduralVectorImplementationTyple_>::T ScalarTyple;

    enum
    {
        STATIC_ASSERT_IN_ENUM((Hippo::Length_f<ProceduralVectorImplementationTyple_>::V > 0), LENGTH_MUST_BE_POSITIVE),
        STATIC_ASSERT_IN_ENUM((Hippo::EachTypeSatisfies_f<ConceptTyple,IsBasedVectorSpace_e>::V), MUST_BE_TYPLE_OF_BASED_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(Hippo::TypleIsUniform_f<ScalarTyple>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesProceduralArray_f<ProceduralVectorImplementationTyple_>::V), MUST_BE_TYPLE_OF_PROCEDURAL_IMPLEMENTATIONS)
    };

    typedef TensorProductOfBasedVectorSpaces_c<ConceptTyple> TensorProductOfBasedVectorSpaces;
    typedef typename Hippo::Head_f<ScalarTyple>::T Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<TensorProductOfBasedVectorSpaces>::V,
                                 ComponentGeneratorEvaluator::tensor_product_of_single_procedural_vector<ProceduralVectorImplementationTyple_,
                                                                                                         TensorProductOfBasedVectorSpaces,
                                                                                                         Scalar>,
                                 TensorProduct_c<ProceduralVectorImplementationTyple_> > ComponentGenerator;
    TensorProductOfProceduralVectors_f();
public:
    typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces,Scalar,UseProceduralArray_t<ComponentGenerator> > T;
};

// recursive case

namespace ComponentGeneratorEvaluator {

template <typename ProceduralVectorImplementationTyple_, typename TensorProductOfBasedVectorSpaces_, typename Scalar_>
Scalar_ tensor_product_of_procedural_vectors (ComponentIndex_t<DimensionOf_f<TensorProductOfBasedVectorSpaces_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<TensorProductOfBasedVectorSpaces_>::V> ComponentIndex;
    typedef typename ConceptOfEachTypeIn_f<ProceduralVectorImplementationTyple_>::T ConceptTyple;
    typedef TensorProductOfBasedVectorSpaces_c<ConceptTyple> TensorProductOfBasedVectorSpaces;
    STATIC_ASSERT_TYPES_ARE_EQUAL(TensorProductOfBasedVectorSpaces_, TensorProductOfBasedVectorSpaces);
    typedef typename Hippo::Head_f<ProceduralVectorImplementationTyple_>::T HeadImplementation;
    typedef typename TensorProductOfProceduralVectors_f<typename Hippo::BodyTyple_f<ProceduralVectorImplementationTyple_>::T>::T BodyImplementation;
    HeadImplementation head_implementation;
    BodyImplementation body_implementation;
    AbstractIndex_c<'a'> a;
    AbstractIndex_c<'b'> b;
    AbstractIndex_c<'P'> P;
    return (head_implementation(a)*body_implementation(b))
           .bundle_with_no_type_check(a*b,TensorProductOfBasedVectorSpaces_(),P)
           [MultiIndex_t<Typle_t<ComponentIndex>>(i)];
}

} // end of namespace ComponentGeneratorEvaluator

template <typename ProceduralVectorImplementationTyple_>
struct TensorProductOfProceduralVectors_f
{
private:
    typedef typename ConceptOfEachTypeIn_f<ProceduralVectorImplementationTyple_>::T ConceptTyple;
    typedef typename ScalarOfEachTypeIn_f<ProceduralVectorImplementationTyple_>::T ScalarTyple;

    enum
    {
        STATIC_ASSERT_IN_ENUM((Hippo::Length_f<ProceduralVectorImplementationTyple_>::V > 0), LENGTH_MUST_BE_POSITIVE),
        STATIC_ASSERT_IN_ENUM((Hippo::EachTypeSatisfies_f<ConceptTyple,IsBasedVectorSpace_e>::V), MUST_BE_TYPLE_OF_BASED_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(Hippo::TypleIsUniform_f<ScalarTyple>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesProceduralArray_f<ProceduralVectorImplementationTyple_>::V), MUST_BE_TYPLE_OF_PROCEDURAL_IMPLEMENTATIONS)
    };

    typedef TensorProductOfBasedVectorSpaces_c<ConceptTyple> TensorProductOfBasedVectorSpaces;
    typedef typename Hippo::Head_f<ScalarTyple>::T Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<TensorProductOfBasedVectorSpaces>::V,
                                 ComponentGeneratorEvaluator::tensor_product_of_procedural_vectors<ProceduralVectorImplementationTyple_,
                                                                                                  TensorProductOfBasedVectorSpaces,
                                                                                                  Scalar>,
                                 TensorProduct_c<ProceduralVectorImplementationTyple_> > ComponentGenerator;
    TensorProductOfProceduralVectors_f();
public:
    typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces,Scalar,UseProceduralArray_t<ComponentGenerator> > T;
};

// ///////////////////////////////////////////////////////////////////////////
// "parallel" tensor product of 2-tensors (i.e. tensor product of linear maps)
// ///////////////////////////////////////////////////////////////////////////

template <typename Procedural2TensorImplementationTyple_> struct TensorProductOfProcedural2Tensors_f;

// if all the procedural 2-tensor implementations are diagonal 2-tensors, then
// the result can be a diagonal 2-tensor.  if all the implementations are
// symmetric 2-tensors, the result can be a symmetric 2-tensor, etc.  if there
// are only symmetric and diagonal 2-tensors, then the result has to be
// symmetric.  in general, there is a "meet" of 2-tensor types which gives the
// common tensor type in which their tensor product lives.  for example, the
// tensor product of a symmetric and an antisymmetric can only be (in the
// current state of the conceptual hierarchy) a non-symmetric tensor.

template <typename Procedural2TensorImplementationTyple_> struct ConceptualTypeOfTensorProductOfProcedural2Tensors_f;

// template specialization for tensor products of scalar 2-tensors
template <typename Factor0_,
          typename Factor1_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename... Procedural2TensorImplementationTypes_>
struct ConceptualTypeOfTensorProductOfProcedural2Tensors_f<
    Typle_t<ImplementationOf_t<Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>,
            Procedural2TensorImplementationTypes_...>>
{
private:
    typedef Typle_t<ImplementationOf_t<Scalar2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>,
            Procedural2TensorImplementationTypes_...> Procedural2TensorImplementationTyple;
    typedef typename ConceptOfEachTypeIn_f<Procedural2TensorImplementationTyple>::T ConceptTyple;
    enum { STATIC_ASSERT_IN_ENUM((Hippo::EachTypeSatisfies_f<ConceptTyple,IsScalar2TensorProductOfBasedVectorSpaces_e>::V), MUST_BE_TYPLE_OF_SCALAR_2_TENSORS) };
    typedef typename FactorNOfEachTypeIn_f<0,ConceptTyple>::T Factor0Typle;
    typedef typename FactorNOfEachTypeIn_f<1,ConceptTyple>::T Factor1Typle;
    typedef TensorProductOfBasedVectorSpaces_c<Factor0Typle> Factor0TensorProduct;
    typedef TensorProductOfBasedVectorSpaces_c<Factor1Typle> Factor1TensorProduct;
    ConceptualTypeOfTensorProductOfProcedural2Tensors_f();
public:
    typedef Scalar2TensorProductOfBasedVectorSpaces_c<Factor0TensorProduct,Factor1TensorProduct> T;
};

// template specialization for tensor products of diagonal 2-tensors
template <typename Factor0_,
          typename Factor1_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename... Procedural2TensorImplementationBodyTypes_>
struct ConceptualTypeOfTensorProductOfProcedural2Tensors_f<
    Typle_t<ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>,
            Procedural2TensorImplementationBodyTypes_...>>
{
private:
    typedef Typle_t<ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>,
            Procedural2TensorImplementationBodyTypes_...> Procedural2TensorImplementationTyple;
    typedef typename ConceptOfEachTypeIn_f<Procedural2TensorImplementationTyple>::T ConceptTyple;
    enum { STATIC_ASSERT_IN_ENUM((Hippo::EachTypeSatisfies_f<ConceptTyple,IsDiagonal2TensorProductOfBasedVectorSpaces_e>::V), MUST_BE_TYPLE_OF_DIAGONAL_2_TENSORS) };
    typedef typename FactorNOfEachTypeIn_f<0,ConceptTyple>::T Factor0Typle;
    typedef typename FactorNOfEachTypeIn_f<1,ConceptTyple>::T Factor1Typle;
    typedef TensorProductOfBasedVectorSpaces_c<Factor0Typle> Factor0TensorProduct;
    typedef TensorProductOfBasedVectorSpaces_c<Factor1Typle> Factor1TensorProduct;
    ConceptualTypeOfTensorProductOfProcedural2Tensors_f();
public:
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0TensorProduct,Factor1TensorProduct> T;
};

// template specialization for tensor products of general 2-tensors
template <typename Factor0_,
          typename Factor1_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename... Procedural2TensorImplementationBodyTypes_>
struct ConceptualTypeOfTensorProductOfProcedural2Tensors_f<
    Typle_t<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<Typle_t<Factor0_,Factor1_>>,Scalar_,UseArrayType_,Derived_>,
            Procedural2TensorImplementationBodyTypes_...>>
{
private:
    typedef Typle_t<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<Typle_t<Factor0_,Factor1_>>,Scalar_,UseArrayType_,Derived_>,
            Procedural2TensorImplementationBodyTypes_...> Procedural2TensorImplementationTyple;
    typedef typename ConceptOfEachTypeIn_f<Procedural2TensorImplementationTyple>::T ConceptTyple;
    enum
    {
        STATIC_ASSERT_IN_ENUM((Hippo::EachTypeSatisfies_f<ConceptTyple,IsTensorProductOfBasedVectorSpaces_e>::V), MUST_BE_TYPLE_OF_DIAGONAL_2_TENSORS),
        STATIC_ASSERT_IN_ENUM(EachTypeIsA2TensorProductOfBasedVectorSpaces_f<ConceptTyple>::V, MUST_BE_TYPLE_OF_2_TENSORS)
    };
    typedef typename FactorNOfEachTypeIn_f<0,ConceptTyple>::T Factor0Typle;
    typedef typename FactorNOfEachTypeIn_f<1,ConceptTyple>::T Factor1Typle;
    typedef TensorProductOfBasedVectorSpaces_c<Factor0Typle> Factor0TensorProduct;
    typedef TensorProductOfBasedVectorSpaces_c<Factor1Typle> Factor1TensorProduct;
    ConceptualTypeOfTensorProductOfProcedural2Tensors_f();
public:
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<Factor0TensorProduct,Factor1TensorProduct>> T;
};

namespace ComponentGeneratorEvaluator {

template <typename Procedural2TensorImplementationTyple_, typename ConceptualTypeOfTensorProduct_, typename Scalar_>
Scalar_ tensor_product_of_2_tensors (ComponentIndex_t<DimensionOf_f<ConceptualTypeOfTensorProduct_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<ConceptualTypeOfTensorProduct_>::V> ComponentIndex;
    typedef typename ConceptualTypeOfTensorProductOfProcedural2Tensors_f<Procedural2TensorImplementationTyple_>::T ConceptualTypeOfTensorProduct;
    STATIC_ASSERT_TYPES_ARE_EQUAL(ConceptualTypeOfTensorProduct_, ConceptualTypeOfTensorProduct);
    typedef typename FactorTypleOf_f<ConceptualTypeOfTensorProduct_>::T FactorTyple;
    STATIC_ASSERT((Hippo::Length_f<FactorTyple>::V == 2), LENGTH_MUST_BE_EXACTLY_2);
    typedef typename Hippo::Element_f<FactorTyple,0>::T Factor0;
    typedef typename Hippo::Element_f<FactorTyple,1>::T Factor1;
    typedef typename Hippo::Head_f<Procedural2TensorImplementationTyple_>::T HeadImplementation;
    typedef typename TensorProductOfProcedural2Tensors_f<typename Hippo::BodyTyple_f<Procedural2TensorImplementationTyple_>::T>::T BodyImplementation;
    HeadImplementation head_implementation;
    BodyImplementation body_implementation;
    AbstractIndex_c<'u'> u;
    AbstractIndex_c<'v'> v;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'Q'> Q;
    AbstractIndex_c<'r'> r;
    return (head_implementation.split(u*v)*body_implementation.split(k*l))
           .bundle_with_no_type_check(u*k,Factor0(),P)
           .bundle_with_no_type_check(v*l,Factor1(),Q)
           .bundle(P*Q,ConceptualTypeOfTensorProduct_(),r)
           [MultiIndex_t<Typle_t<ComponentIndex>>(i)];
}

} // end of namespace ComponentGeneratorEvaluator

// general/recursive case
template <typename Procedural2TensorImplementationTyple_>
struct TensorProductOfProcedural2Tensors_f
{
private:
    typedef typename ConceptOfEachTypeIn_f<Procedural2TensorImplementationTyple_>::T ConceptTyple;
    typedef typename ScalarOfEachTypeIn_f<Procedural2TensorImplementationTyple_>::T ScalarTyple;
    typedef typename ConceptualTypeOfTensorProductOfProcedural2Tensors_f<Procedural2TensorImplementationTyple_>::T ConceptualTypeOfTensorProduct;

    enum
    {
        STATIC_ASSERT_IN_ENUM(Hippo::TypleIsUniform_f<ScalarTyple>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesProceduralArray_f<Procedural2TensorImplementationTyple_>::V), MUST_BE_TYPLE_OF_PROCEDURAL_IMPLEMENTATIONS)
    };

    typedef typename Hippo::Head_f<ScalarTyple>::T Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<ConceptualTypeOfTensorProduct>::V,
                                 ComponentGeneratorEvaluator::tensor_product_of_2_tensors<Procedural2TensorImplementationTyple_,
                                                                                          ConceptualTypeOfTensorProduct,
                                                                                          Scalar>,
                                 TensorProduct_c<Procedural2TensorImplementationTyple_>> ComponentGenerator;
private:
    TensorProductOfProcedural2Tensors_f();
public:
    typedef ImplementationOf_t<ConceptualTypeOfTensorProduct,Scalar,UseProceduralArray_t<ComponentGenerator> > T;
};

namespace ComponentGeneratorEvaluator {

template <typename Procedural2TensorImplementationTyple_, typename ConceptualTypeOfTensorProduct_, typename Scalar_>
Scalar_ tensor_product_of_single_2_tensor (ComponentIndex_t<DimensionOf_f<ConceptualTypeOfTensorProduct_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<ConceptualTypeOfTensorProduct_>::V> ComponentIndex;
    typedef typename ConceptualTypeOfTensorProductOfProcedural2Tensors_f<Procedural2TensorImplementationTyple_>::T ConceptualTypeOfTensorProduct;
    STATIC_ASSERT_TYPES_ARE_EQUAL(ConceptualTypeOfTensorProduct_, ConceptualTypeOfTensorProduct);
    typedef typename Hippo::Head_f<Procedural2TensorImplementationTyple_>::T HeadImplementation;
    HeadImplementation head_implementation;
    AbstractIndex_c<'a'> a;
    return head_implementation(a)[MultiIndex_t<Typle_t<ComponentIndex>>(i)];
}

} // end of namespace ComponentGeneratorEvaluator

// template specialization for tensor product of a single 2-tensor
template <typename HeadProcedural2TensorImplementationType_>
struct TensorProductOfProcedural2Tensors_f<Typle_t<HeadProcedural2TensorImplementationType_>>
{
private:
    typedef Typle_t<HeadProcedural2TensorImplementationType_> Procedural2TensorImplementationTyple;
    typedef typename ConceptOfEachTypeIn_f<Procedural2TensorImplementationTyple>::T ConceptTyple;
    typedef typename ScalarOfEachTypeIn_f<Procedural2TensorImplementationTyple>::T ScalarTyple;
    typedef typename ConceptualTypeOfTensorProductOfProcedural2Tensors_f<Procedural2TensorImplementationTyple>::T ConceptualTypeOfTensorProduct;

    enum
    {
        STATIC_ASSERT_IN_ENUM(Hippo::TypleIsUniform_f<ScalarTyple>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesProceduralArray_f<Procedural2TensorImplementationTyple>::V), MUST_BE_TYPLE_OF_PROCEDURAL_IMPLEMENTATIONS)
    };

    typedef typename Hippo::Head_f<ScalarTyple>::T Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<ConceptualTypeOfTensorProduct>::V,
                                 ComponentGeneratorEvaluator::tensor_product_of_single_2_tensor<Procedural2TensorImplementationTyple,
                                                                                                ConceptualTypeOfTensorProduct,
                                                                                                Scalar>,
                                 TensorProduct_c<Procedural2TensorImplementationTyple>> ComponentGenerator;
public:
    typedef ImplementationOf_t<ConceptualTypeOfTensorProduct,Scalar,UseProceduralArray_t<ComponentGenerator> > T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_TENSORPRODUCT_HPP_
