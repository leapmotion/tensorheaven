// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/tensorproduct.hpp by Victor Dods, created 2013/09/18
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_TENSORPRODUCT_HPP_
#define TENH_IMPLEMENTATION_TENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/implementationof.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vector.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// helper metafunctions
// ///////////////////////////////////////////////////////////////////////////

template <typename TypeList_>
struct ConceptOfEachTypeIn_f
{
    typedef TypeList_t<typename TypeList_::HeadType::Concept,
                       typename ConceptOfEachTypeIn_f<typename TypeList_::BodyTypeList>::T> T;
};

template <>
struct ConceptOfEachTypeIn_f<EmptyTypeList>
{
    typedef EmptyTypeList T;
};

template <typename TypeList_>
struct ScalarOfEachTypeIn_f
{
    typedef TypeList_t<typename TypeList_::HeadType::Scalar,
                       typename ScalarOfEachTypeIn_f<typename TypeList_::BodyTypeList>::T> T;
};

template <>
struct ScalarOfEachTypeIn_f<EmptyTypeList>
{
    typedef EmptyTypeList T;
};

template <typename TypeList_>
struct EachTypeUsesImmutableArray_f
{
    static bool const V = IsUseImmutableArray_f<typename TypeList_::HeadType::UseArrayType>::V &&
                          EachTypeUsesImmutableArray_f<typename TypeList_::BodyTypeList>::V;
};

template <>
struct EachTypeUsesImmutableArray_f<EmptyTypeList>
{
    static bool const V = true; // vacuously true
};

// ///////////////////////////////////////////////////////////////////////////
// TensorProductOfImmutableVectors_f
// ///////////////////////////////////////////////////////////////////////////

template <typename ImmutableVectorImplementationTypeList_>
struct TensorProductOfImmutableVectors_f;

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
    typedef TypeList_t<HeadImmutableVectorImplementationType_> ImmutableVectorImplementationTypeList_;
    typedef typename ConceptOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ConceptTypeList;
    typedef typename ScalarOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ScalarTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM((ImmutableVectorImplementationTypeList_::LENGTH > 0), LENGTH_MUST_BE_POSITIVE),
        STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<ConceptTypeList,IsBasedVectorSpace_p>::V), MUST_BE_TYPELIST_OF_BASED_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(TypeListIsUniform_t<ScalarTypeList>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesImmutableArray_f<ImmutableVectorImplementationTypeList_>::V), MUST_BE_TYPELIST_OF_IMMUTABLE_VECTOR_IMPLEMENTATIONS)
    };

    typedef TensorProductOfBasedVectorSpaces_c<ConceptTypeList> TensorProductOfBasedVectorSpaces;
    typedef typename ScalarTypeList::HeadType Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<TensorProductOfBasedVectorSpaces>::V,
                                 ComponentGeneratorEvaluator::tensor_product_of_single_immutable_vector<ImmutableVectorImplementationTypeList_,
                                                                                                        TensorProductOfBasedVectorSpaces,
                                                                                                        Scalar>,
                                 TensorProduct_c<ImmutableVectorImplementationTypeList_> > ComponentGenerator;
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
    typedef typename ConceptOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ConceptTypeList;
    typedef typename ScalarOfEachTypeIn_f<ImmutableVectorImplementationTypeList_>::T ScalarTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM((ImmutableVectorImplementationTypeList_::LENGTH > 0), LENGTH_MUST_BE_POSITIVE),
        STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<ConceptTypeList,IsBasedVectorSpace_p>::V), MUST_BE_TYPELIST_OF_BASED_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(TypeListIsUniform_t<ScalarTypeList>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesImmutableArray_f<ImmutableVectorImplementationTypeList_>::V), MUST_BE_TYPELIST_OF_IMMUTABLE_VECTOR_IMPLEMENTATIONS)
    };

    typedef TensorProductOfBasedVectorSpaces_c<ConceptTypeList> TensorProductOfBasedVectorSpaces;
    typedef typename ScalarTypeList::HeadType Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<TensorProductOfBasedVectorSpaces>::V,
                                 ComponentGeneratorEvaluator::tensor_product_of_immutable_vectors<ImmutableVectorImplementationTypeList_,
                                                                                                  TensorProductOfBasedVectorSpaces,
                                                                                                  Scalar>,
                                 TensorProduct_c<ImmutableVectorImplementationTypeList_> > ComponentGenerator;
    typedef ImplementationOf_t<TensorProductOfBasedVectorSpaces,Scalar,UseImmutableArray_t<ComponentGenerator> > T;
};
/*
// ///////////////////////////////////////////////////////////////////////////
// standard inner product on a based vector space having an orthonormal basis
// ///////////////////////////////////////////////////////////////////////////

// corresponds to the identity matrix
struct StandardInnerProduct { static std::string type_as_string () { return "StandardInnerProduct"; } };

namespace ComponentGeneratorEvaluator {

// this would be better defined as a private static method in the specialization of InnerProduct_f below,
// but g++ complains about that being invalid in a const expression (whereas clang++ is fine with it)
template <typename EmbeddableInTensorProductOfBasedVectorSpaces_, typename Scalar_>
Scalar_ standard_inner_product (ComponentIndex_t<DimensionOf_f<EmbeddableInTensorProductOfBasedVectorSpaces_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<EmbeddableInTensorProductOfBasedVectorSpaces_>::V> ComponentIndex;
    // for using bundle_index_map -- the use of UsePreallocatedArray is somewhat arbitrary,
    // since only the static method bundle_index_map will be used.
    typedef ImplementationOf_t<EmbeddableInTensorProductOfBasedVectorSpaces_,Scalar_,UsePreallocatedArray> BootstrappingImplementation;
    typedef typename BootstrappingImplementation::MultiIndex::IndexTypeList IndexTypeList;
    typedef MultiIndex_t<IndexTypeList> MultiIndex;
    MultiIndex m(BootstrappingImplementation::template bundle_index_map<IndexTypeList,ComponentIndex>(i));
    return m.template el<0>() == m.template el<1>() ? Scalar_(1) : Scalar_(0);
}

} // end of namespace ComponentGeneratorEvaluator

// template specialization for standard inner product on a based vector space having orthonormal basis
template <typename VectorSpace_, typename OrthonormalBasisId_, typename Scalar_>
struct InnerProduct_f<BasedVectorSpace_c<VectorSpace_,OrthonormalBasis_c<OrthonormalBasisId_> >,StandardInnerProduct,Scalar_>
{
private:
    typedef BasedVectorSpace_c<VectorSpace_,OrthonormalBasis_c<OrthonormalBasisId_> > BasedVectorSpace;
    typedef typename DualOf_f<BasedVectorSpace>::T DualOfBasedVectorSpace;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfBasedVectorSpace> SymmetricPower;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<SymmetricPower>::V,
                                 ComponentGeneratorEvaluator::standard_inner_product<SymmetricPower,Scalar_>,
                                 StandardInnerProduct> ComponentGenerator;
public:
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

// ///////////////////////////////////////////////////////////////////////////
// induced inner product on TensorProductOfBasedVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

namespace ComponentGeneratorEvaluator {

// the fact that this is a global function (instead of a static method inside
// the below InnerProduct_f specialization) is a workaround for GCC not accepting
// a static method as a template param (which works in clang).
template <typename SymmetricPower_, typename FactorTypeList_, typename InnerProductIdTypeList_, typename Scalar_>
Scalar_ inner_product_induced_on_tensor_product (ComponentIndex_t<DimensionOf_f<SymmetricPower_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<SymmetricPower_>::V> ComponentIndex;
    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList_> TensorProductOfBasedVectorSpaces;
    typedef typename InnerProduct_f<typename FactorTypeList_::HeadType,
                                    typename InnerProductIdTypeList_::HeadType,
                                    Scalar_>::T HeadInnerProduct;
    HeadInnerProduct head_inner_product;
    typedef typename InnerProduct_f<TensorProductOfBasedVectorSpaces_c<typename FactorTypeList_::BodyTypeList>,
                                    TensorProduct_c<typename InnerProductIdTypeList_::BodyTypeList>,
                                    Scalar_>::T BodyInnerProduct;
    BodyInnerProduct body_inner_product;
    AbstractIndex_c<'a'> a;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'B'> B;
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'Q'> Q;
    AbstractIndex_c<'r'> r;
    AbstractIndex_c<'s'> s;
    AbstractIndex_c<'T'> T;
    return (head_inner_product(a).split(a,j|k) * body_inner_product(B).split(B,P|Q))
            .bundle(j|P,TensorProductOfBasedVectorSpaces(),r)
            .bundle(k|Q,TensorProductOfBasedVectorSpaces(),s)
            .bundle(r|s,SymmetricPower_(),T)[MultiIndex_t<TypeList_t<ComponentIndex> >(i)];
}

} // end of namespace ComponentGeneratorEvaluator

template <typename FactorTypeList_, typename InnerProductIdTypeList_, typename Scalar_>
struct InnerProduct_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,TensorProduct_c<InnerProductIdTypeList_>,Scalar_>
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(FactorTypeList_::LENGTH == InnerProductIdTypeList_::LENGTH, LENGTHS_MUST_BE_EQUAL),
        STATIC_ASSERT_IN_ENUM((FactorTypeList_::LENGTH > 0), ORDER_MUST_BE_POSITIVE) // TODO: allow ORDER_ == 0
    };
    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList_> TensorProductOfBasedVectorSpaces;
    typedef typename DualOf_f<TensorProductOfBasedVectorSpaces>::T DualOfTensorProductOfBasedVectorSpaces;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfTensorProductOfBasedVectorSpaces> SymmetricPower;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<SymmetricPower>::V,
                                 ComponentGeneratorEvaluator::inner_product_induced_on_tensor_product<SymmetricPower,
                                                                                                      FactorTypeList_,
                                                                                                      InnerProductIdTypeList_,
                                                                                                      Scalar_>,
                                 TensorProduct_c<InnerProductIdTypeList_> > ComponentGenerator;
public:
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

// base case for order-1 tensor products.

namespace ComponentGeneratorEvaluator {

template <typename EmbeddableInTensorProductOfBasedVectorSpaces_, typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_>
Scalar_ inner_product_induced_on_1st_tensor_power (ComponentIndex_t<DimensionOf_f<EmbeddableInTensorProductOfBasedVectorSpaces_>::V> const &i)
{
    typedef SymmetricPowerOfBasedVectorSpace_c<2,BasedVectorSpace_> SymmetricPower;
    STATIC_ASSERT(DimensionOf_f<SymmetricPower>::V == DimensionOf_f<EmbeddableInTensorProductOfBasedVectorSpaces_>::V,
                  DIMENSIONS_MUST_MATCH);
    typedef typename InnerProduct_f<BasedVectorSpace_,InnerProductId_,Scalar_>::T HeadInnerProduct;
    HeadInnerProduct head_inner_product;
    return head_inner_product[i];
}

} // end of namespace ComponentGeneratorEvaluator

// template specialization for ORDER_ == 1 (which is really just the original inner product)
// TODO: do specialization for ORDER_ == 0 (but this would depend on the basis being "field unit basis"
// (i.e. the 0th tensor power is the field itself and the multiplicative identity of the field is the basis vector)).
template <typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_>
struct InnerProduct_f<TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedVectorSpace_> >,TensorProduct_c<TypeList_t<InnerProductId_> >,Scalar_>
{
private:
    typedef typename TensorPowerOfBasedVectorSpace_f<1,BasedVectorSpace_>::T TensorProductOfBasedVectorSpaces;
    typedef typename DualOf_f<TensorProductOfBasedVectorSpaces>::T DualOfTensorProductOfBasedVectorSpaces;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOfTensorProductOfBasedVectorSpaces> SymmetricPower;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<SymmetricPower>::V,
                                 ComponentGeneratorEvaluator::inner_product_induced_on_1st_tensor_power<SymmetricPower,
                                                                                                        BasedVectorSpace_,
                                                                                                        InnerProductId_,
                                                                                                        Scalar_>,
                                 typename TensorPower_f<1,InnerProductId_>::T> ComponentGenerator;
public:
    typedef ImplementationOf_t<SymmetricPower,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};
*/
} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_TENSORPRODUCT_HPP_
