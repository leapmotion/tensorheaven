// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/tensorproduct.hpp by Victor Dods, created 2013/09/18
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_TENSORPRODUCT_HPP_
#define TENH_IMPLEMENTATION_TENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vector.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// TensorProductOfImmutableVectors_f
// ///////////////////////////////////////////////////////////////////////////

template <typename ImmutableVectorImplementationTypeList_> struct TensorProductOfImmutableVectors_f;

// base case (1-tensor product)

namespace ComponentGeneratorEvaluator {

template <typename ImmutableVectorImplementationTypeList_, typename TensorProductOfBasedVectorSpaces_, typename Scalar_>
Scalar_ tensor_product_of_single_immutable_vector (ComponentIndex_t<DimensionOf_f<TensorProductOfBasedVectorSpaces_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<TensorProductOfBasedVectorSpaces_>::V> ComponentIndex;
    typedef typename ConceptOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ConceptTypeList;
    typedef typename ScalarOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ScalarTypeList;
    typedef TensorProductOfBasedVectorSpaces_c<ConceptTypeList> TensorProductOfBasedVectorSpaces;
    STATIC_ASSERT_TYPES_ARE_EQUAL(TensorProductOfBasedVectorSpaces_, TensorProductOfBasedVectorSpaces);
    typedef typename ImmutableVectorImplementationTypeList_::HeadType HeadImplementation;
    HeadImplementation head_implementation;
    AbstractIndex_c<'a'> a;
    return head_implementation(a)[MultiIndex_t<TypeList_t<ComponentIndex> >(i)];
}

} // end of namespace ComponentGeneratorEvaluator

template <typename HeadImmutableVectorImplementationType_>
struct TensorProductOfImmutableVectors_f<TypeList_t<HeadImmutableVectorImplementationType_> >
{
private:
    typedef TypeList_t<HeadImmutableVectorImplementationType_> ImmutableVectorImplementationTypeList_;
    typedef typename ConceptOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ConceptTypeList;
    typedef typename ScalarOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ScalarTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM((ImmutableVectorImplementationTypeList_::LENGTH > 0), LENGTH_MUST_BE_POSITIVE),
        STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<ConceptTypeList,IsBasedVectorSpace_p>::V), MUST_BE_TYPELIST_OF_BASED_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(TypeListIsUniform_t<ScalarTypeList>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesImmutableArray_f<ImmutableVectorImplementationTypeList_>::V), MUST_BE_TYPELIST_OF_IMMUTABLE_IMPLEMENTATIONS)
    };

    typedef TensorProductOfBasedVectorSpaces_c<ConceptTypeList> TensorProductOfBasedVectorSpaces;
    typedef typename ScalarTypeList::HeadType Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<TensorProductOfBasedVectorSpaces>::V,
                                 ComponentGeneratorEvaluator::tensor_product_of_single_immutable_vector<ImmutableVectorImplementationTypeList_,
                                                                                                        TensorProductOfBasedVectorSpaces,
                                                                                                        Scalar>,
                                 TensorProduct_c<ImmutableVectorImplementationTypeList_> > ComponentGenerator;
    TensorProductOfImmutableVectors_f();
public:
    typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces,Scalar,UseImmutableArray_t<ComponentGenerator> > T;
};

// recursive case

namespace ComponentGeneratorEvaluator {

template <typename ImmutableVectorImplementationTypeList_, typename TensorProductOfBasedVectorSpaces_, typename Scalar_>
Scalar_ tensor_product_of_immutable_vectors (ComponentIndex_t<DimensionOf_f<TensorProductOfBasedVectorSpaces_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<TensorProductOfBasedVectorSpaces_>::V> ComponentIndex;
    typedef typename ConceptOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ConceptTypeList;
    typedef typename ScalarOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ScalarTypeList;
    typedef TensorProductOfBasedVectorSpaces_c<ConceptTypeList> TensorProductOfBasedVectorSpaces;
    STATIC_ASSERT_TYPES_ARE_EQUAL(TensorProductOfBasedVectorSpaces_, TensorProductOfBasedVectorSpaces);
    typedef typename ImmutableVectorImplementationTypeList_::HeadType HeadImplementation;
    typedef typename TensorProductOfImmutableVectors_f<typename ImmutableVectorImplementationTypeList_::BodyTypeList>::T BodyImplementation;
    HeadImplementation head_implementation;
    BodyImplementation body_implementation;
    AbstractIndex_c<'a'> a;
    AbstractIndex_c<'b'> b;
    AbstractIndex_c<'P'> P;
    return (head_implementation(a)*body_implementation(b)).bundle(a|b,TensorProductOfBasedVectorSpaces_(),P)[MultiIndex_t<TypeList_t<ComponentIndex> >(i)];
}

} // end of namespace ComponentGeneratorEvaluator

template <typename ImmutableVectorImplementationTypeList_>
struct TensorProductOfImmutableVectors_f
{
private:
    typedef typename ConceptOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ConceptTypeList;
    typedef typename ScalarOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ScalarTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM((ImmutableVectorImplementationTypeList_::LENGTH > 0), LENGTH_MUST_BE_POSITIVE),
        STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<ConceptTypeList,IsBasedVectorSpace_p>::V), MUST_BE_TYPELIST_OF_BASED_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(TypeListIsUniform_t<ScalarTypeList>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesImmutableArray_f<ImmutableVectorImplementationTypeList_>::V), MUST_BE_TYPELIST_OF_IMMUTABLE_IMPLEMENTATIONS)
    };

    typedef TensorProductOfBasedVectorSpaces_c<ConceptTypeList> TensorProductOfBasedVectorSpaces;
    typedef typename ScalarTypeList::HeadType Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<TensorProductOfBasedVectorSpaces>::V,
                                 ComponentGeneratorEvaluator::tensor_product_of_immutable_vectors<ImmutableVectorImplementationTypeList_,
                                                                                                  TensorProductOfBasedVectorSpaces,
                                                                                                  Scalar>,
                                 TensorProduct_c<ImmutableVectorImplementationTypeList_> > ComponentGenerator;
    TensorProductOfImmutableVectors_f();
public:
    typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces,Scalar,UseImmutableArray_t<ComponentGenerator> > T;
};

// ///////////////////////////////////////////////////////////////////////////
// "parallel" tensor product of 2-tensors (i.e. tensor product of linear maps)
// ///////////////////////////////////////////////////////////////////////////

template <typename Immutable2TensorImplementationTypeList_> struct TensorProductOfImmutable2Tensors_f;

// if all the immutable 2-tensor implementations are diagonal 2-tensors, then
// the result can be a diagonal 2-tensor.  if all the implementations are
// symmetric 2-tensors, the result can be a symmetric 2-tensor, etc.  if there
// are only symmetric and diagonal 2-tensors, then the result has to be
// symmetric.  in general, there is a "meet" of 2-tensor types which gives the
// common tensor type in which their tensor product lives.  for example, the
// tensor product of a symmetric and an antisymmetric can only be (in the
// current state of the conceptual hierarchy) a non-symmetric tensor.

template <typename Immutable2TensorImplementationTypeList_> struct ConceptualTypeOfTensorProductOfImmutable2Tensors_f;

// template specialization for tensor products of diagonal 2-tensors
template <typename Factor1_,
          typename Factor2_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename Immutable2TensorImplementationBodyTypeList_>
struct ConceptualTypeOfTensorProductOfImmutable2Tensors_f<
    TypeList_t<ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_>,
               Immutable2TensorImplementationBodyTypeList_> >
{
private:
    typedef TypeList_t<ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_>,
                       Immutable2TensorImplementationBodyTypeList_> Immutable2TensorImplementationTypeList;
    typedef typename ConceptOfEachTypeIn_f<Immutable2TensorImplementationTypeList>::T ConceptTypeList;
    enum { STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<ConceptTypeList,IsDiagonal2TensorProductOfBasedVectorSpaces_p>::V), MUST_BE_TYPELIST_OF_DIAGONAL_2_TENSORS) };
    typedef typename FactorNOfEachTypeIn_f<0,ConceptTypeList>::T Factor0TypeList;
    typedef typename FactorNOfEachTypeIn_f<1,ConceptTypeList>::T Factor1TypeList;
    typedef TensorProductOfBasedVectorSpaces_c<Factor0TypeList> Factor0TensorProduct;
    typedef TensorProductOfBasedVectorSpaces_c<Factor1TypeList> Factor1TensorProduct;
    ConceptualTypeOfTensorProductOfImmutable2Tensors_f();
public:
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0TensorProduct,Factor1TensorProduct> T;
};

// template specialization for tensor products of general 2-tensors
template <typename Factor1_,
          typename Factor2_,
          typename Scalar_,
          typename UseArrayType_,
          typename Derived_,
          typename Immutable2TensorImplementationBodyTypeList_>
struct ConceptualTypeOfTensorProductOfImmutable2Tensors_f<
    TypeList_t<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,TypeList_t<Factor2_> > >,Scalar_,UseArrayType_,Derived_>,
               Immutable2TensorImplementationBodyTypeList_> >
{
private:
    typedef TypeList_t<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,TypeList_t<Factor2_> > >,Scalar_,UseArrayType_,Derived_>,
                       Immutable2TensorImplementationBodyTypeList_> Immutable2TensorImplementationTypeList;
    typedef typename ConceptOfEachTypeIn_f<Immutable2TensorImplementationTypeList>::T ConceptTypeList;
    enum
    {
        STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<ConceptTypeList,IsTensorProductOfBasedVectorSpaces_p>::V), MUST_BE_TYPELIST_OF_DIAGONAL_2_TENSORS),
        STATIC_ASSERT_IN_ENUM(EachTypeIsA2TensorProductOfBasedVectorSpaces_f<ConceptTypeList>::V, MUST_BE_TYPELIST_OF_2_TENSORS)
    };
    typedef typename FactorNOfEachTypeIn_f<0,ConceptTypeList>::T Factor0TypeList;
    typedef typename FactorNOfEachTypeIn_f<1,ConceptTypeList>::T Factor1TypeList;
    typedef TensorProductOfBasedVectorSpaces_c<Factor0TypeList> Factor0TensorProduct;
    typedef TensorProductOfBasedVectorSpaces_c<Factor1TypeList> Factor1TensorProduct;
    ConceptualTypeOfTensorProductOfImmutable2Tensors_f();
public:
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor0TensorProduct,TypeList_t<Factor1TensorProduct> > > T;
};

namespace ComponentGeneratorEvaluator {

template <typename Immutable2TensorImplementationTypeList_, typename ConceptualTypeOfTensorProduct_, typename Scalar_>
Scalar_ tensor_product_of_2_tensors (ComponentIndex_t<DimensionOf_f<ConceptualTypeOfTensorProduct_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<ConceptualTypeOfTensorProduct_>::V> ComponentIndex;
    typedef typename ConceptualTypeOfTensorProductOfImmutable2Tensors_f<Immutable2TensorImplementationTypeList_>::T ConceptualTypeOfTensorProduct;
    STATIC_ASSERT_TYPES_ARE_EQUAL(ConceptualTypeOfTensorProduct_, ConceptualTypeOfTensorProduct);
    typedef typename FactorTypeListOf_f<ConceptualTypeOfTensorProduct_>::T FactorTypeList;
    STATIC_ASSERT((FactorTypeList::LENGTH == 2), LENGTH_MUST_BE_EXACTLY_2);
    typedef typename FactorTypeList::HeadType Factor1;
    typedef typename FactorTypeList::BodyTypeList::HeadType Factor2;
    typedef typename Immutable2TensorImplementationTypeList_::HeadType HeadImplementation;
    typedef typename TensorProductOfImmutable2Tensors_f<typename Immutable2TensorImplementationTypeList_::BodyTypeList>::T BodyImplementation;
    HeadImplementation head_implementation;
    BodyImplementation body_implementation;
    AbstractIndex_c<'A'> A;
    AbstractIndex_c<'B'> B;
    AbstractIndex_c<'u'> u;
    AbstractIndex_c<'v'> v;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'Q'> Q;
    AbstractIndex_c<'r'> r;
    return (head_implementation(A).split(A,u|v)*body_implementation(B).split(B,k|l))
           .bundle(u|k,Factor1(),P)
           .bundle(v|l,Factor2(),Q)
           .bundle(P|Q,ConceptualTypeOfTensorProduct_(),r)
           [MultiIndex_t<TypeList_t<ComponentIndex> >(i)];
}

} // end of namespace ComponentGeneratorEvaluator

// general/recursive case
template <typename Immutable2TensorImplementationTypeList_>
struct TensorProductOfImmutable2Tensors_f
{
private:
    typedef typename ConceptOfEachTypeIn_f<Immutable2TensorImplementationTypeList_>::T ConceptTypeList;
    typedef typename ScalarOfEachTypeIn_f<Immutable2TensorImplementationTypeList_>::T ScalarTypeList;
    typedef typename ConceptualTypeOfTensorProductOfImmutable2Tensors_f<Immutable2TensorImplementationTypeList_>::T ConceptualTypeOfTensorProduct;

    enum
    {
        STATIC_ASSERT_IN_ENUM(TypeListIsUniform_t<ScalarTypeList>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesImmutableArray_f<Immutable2TensorImplementationTypeList_>::V), MUST_BE_TYPELIST_OF_IMMUTABLE_IMPLEMENTATIONS)
    };

    typedef typename ScalarTypeList::HeadType Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<ConceptualTypeOfTensorProduct>::V,
                                 ComponentGeneratorEvaluator::tensor_product_of_2_tensors<Immutable2TensorImplementationTypeList_,
                                                                                          ConceptualTypeOfTensorProduct,
                                                                                          Scalar>,
                                 TensorProduct_c<Immutable2TensorImplementationTypeList_> > ComponentGenerator;
private:
    TensorProductOfImmutable2Tensors_f();
public:
    typedef ImplementationOf_t<ConceptualTypeOfTensorProduct,Scalar,UseImmutableArray_t<ComponentGenerator> > T;
};

namespace ComponentGeneratorEvaluator {

template <typename Immutable2TensorImplementationTypeList_, typename ConceptualTypeOfTensorProduct_, typename Scalar_>
Scalar_ tensor_product_of_single_2_tensor (ComponentIndex_t<DimensionOf_f<ConceptualTypeOfTensorProduct_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<ConceptualTypeOfTensorProduct_>::V> ComponentIndex;
    typedef typename ConceptualTypeOfTensorProductOfImmutable2Tensors_f<Immutable2TensorImplementationTypeList_>::T ConceptualTypeOfTensorProduct;
    STATIC_ASSERT_TYPES_ARE_EQUAL(ConceptualTypeOfTensorProduct_, ConceptualTypeOfTensorProduct);
    typedef typename Immutable2TensorImplementationTypeList_::HeadType HeadImplementation;
    HeadImplementation head_implementation;
    AbstractIndex_c<'a'> a;
    return head_implementation(a)[MultiIndex_t<TypeList_t<ComponentIndex> >(i)];
}

} // end of namespace ComponentGeneratorEvaluator

// template specialization for tensor product of a single 2-tensor
template <typename HeadImmutable2TensorImplementationType_>
struct TensorProductOfImmutable2Tensors_f<TypeList_t<HeadImmutable2TensorImplementationType_> >
{
private:
    typedef TypeList_t<HeadImmutable2TensorImplementationType_> Immutable2TensorImplementationTypeList;
    typedef typename ConceptOfEachTypeIn_f<Immutable2TensorImplementationTypeList>::T ConceptTypeList;
    typedef typename ScalarOfEachTypeIn_f<Immutable2TensorImplementationTypeList>::T ScalarTypeList;
    typedef typename ConceptualTypeOfTensorProductOfImmutable2Tensors_f<Immutable2TensorImplementationTypeList>::T ConceptualTypeOfTensorProduct;

    enum
    {
        STATIC_ASSERT_IN_ENUM(TypeListIsUniform_t<ScalarTypeList>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesImmutableArray_f<Immutable2TensorImplementationTypeList>::V), MUST_BE_TYPELIST_OF_IMMUTABLE_IMPLEMENTATIONS)
    };

    typedef typename ScalarTypeList::HeadType Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<ConceptualTypeOfTensorProduct>::V,
                                 ComponentGeneratorEvaluator::tensor_product_of_single_2_tensor<Immutable2TensorImplementationTypeList,
                                                                                                ConceptualTypeOfTensorProduct,
                                                                                                Scalar>,
                                 TensorProduct_c<Immutable2TensorImplementationTypeList> > ComponentGenerator;
public:
    typedef ImplementationOf_t<ConceptualTypeOfTensorProduct,Scalar,UseImmutableArray_t<ComponentGenerator> > T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_TENSORPRODUCT_HPP_
