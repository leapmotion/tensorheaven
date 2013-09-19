// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/euclideanembedding.hpp by Victor Dods, created 2013/09/18
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_EUCLIDEANEMBEDDING_HPP_
#define TENH_IMPLEMENTATION_EUCLIDEANEMBEDDING_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/diagonal2tensor.hpp"
#include "tenh/implementation/innerproduct.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// forward declaration for the EuclideanEmbedding_f metafunction (should
// return an ImplementationOf_t type that uses ImmutableArray_t)
// ///////////////////////////////////////////////////////////////////////////

template <typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_> struct EuclideanEmbedding_f;

// ///////////////////////////////////////////////////////////////////////////
// identifiers and meta-functions for naming Euclidean space(s)
// ///////////////////////////////////////////////////////////////////////////

// an ID for uniquely specifying a finite-dimensional Euclidean space
template <Uint32 DIMENSION_>
struct EuclideanSpaceId_t
{
    static Uint32 const DIMENSION = DIMENSION_;

    static std::string type_as_string () { return "EuclideanSpaceId_t<" + AS_STRING(DIMENSION_) + '>'; }
};

struct StandardBasisId { static std::string type_as_string () { return "StandardBasis"; } };

template <Uint32 DIMENSION_>
struct BasedEuclideanSpace_f
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,DIMENSION_,EuclideanSpaceId_t<DIMENSION_> >,OrthonormalBasis_c<StandardBasisId> > T;
};

// metafunction for easily determining the 2-tensor factors for a Euclidean embedding
// a linear map from BasedVectorSpace_ to BasedEuclideanSpace_f
template <typename BasedVectorSpace_>
struct EuclideanEmbedding2TensorFactorTypeList_f
{
    typedef TypeList_t<typename BasedEuclideanSpace_f<DimensionOf_f<BasedVectorSpace_>::V>::T,
            TypeList_t<typename DualOf_f<BasedVectorSpace_>::T> > T;
};

// ///////////////////////////////////////////////////////////////////////////
// Euclidean embedding for based vector spaces having orthonormal basis
// ///////////////////////////////////////////////////////////////////////////

// template specialization for inner product on Euclidean space
template <typename VectorSpace_, typename OrthonormalBasisId_, typename Scalar_>
struct EuclideanEmbedding_f<BasedVectorSpace_c<VectorSpace_,OrthonormalBasis_c<OrthonormalBasisId_> >,StandardInnerProduct,Scalar_>
{
private:
    typedef BasedVectorSpace_c<VectorSpace_,OrthonormalBasis_c<OrthonormalBasisId_> > BasedVectorSpace;
    typedef typename EuclideanEmbedding2TensorFactorTypeList_f<BasedVectorSpace>::T EuclideanEmbedding2TensorFactorTypeList;
    typedef typename Diagonal2TensorProductOfBasedVectorSpaces_f<EuclideanEmbedding2TensorFactorTypeList>::T Diagonal2Tensor;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<Diagonal2Tensor>::V,
                                 constant_component_generator_evaluator<Scalar_,DimensionOf_f<Diagonal2Tensor>::V,1>,
                                 StandardInnerProduct> ComponentGenerator;
public:
    typedef ImplementationOf_t<Diagonal2Tensor,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

// ///////////////////////////////////////////////////////////////////////////
// induced Euclidean embedding on TensorProductOfBasedVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

namespace ComponentGeneratorEvaluator {

// the fact that this is a global function (instead of a static method inside
// the below EuclideanEmbedding_f specialization) is a workaround for GCC not accepting
// a static method as a template param (which works in clang).
template <typename Endomorphism_, typename FactorTypeList_, typename InnerProductIdTypeList_, typename Scalar_>
Scalar_ euclidean_embedding_induced_on_tensor_product (ComponentIndex_t<DimensionOf_f<Endomorphism_>::V> const &i)
{
    typedef ComponentIndex_t<DimensionOf_f<Endomorphism_>::V> ComponentIndex;
    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList_> TensorProductOfBasedVectorSpaces;
    typedef typename EuclideanEmbedding_f<typename FactorTypeList_::HeadType,
                                          typename InnerProductIdTypeList_::HeadType,
                                          Scalar_>::T HeadEuclideanEmbedding;
    HeadEuclideanEmbedding head_euclidean_embedding;
    typedef typename EuclideanEmbedding_f<TensorProductOfBasedVectorSpaces_c<typename FactorTypeList_::BodyTypeList>,
                                          TensorProduct_c<typename InnerProductIdTypeList_::BodyTypeList>,
                                          Scalar_>::T BodyEuclideanEmbedding;
    BodyEuclideanEmbedding body_euclidean_embedding;
    AbstractIndex_c<'a'> a;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'B'> B;
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'Q'> Q;
    AbstractIndex_c<'r'> r;
    AbstractIndex_c<'s'> s;
    AbstractIndex_c<'T'> T;
    return (head_euclidean_embedding(a).split(a,j|k) * body_euclidean_embedding(B).split(B,P|Q))
            .bundle(j|P,TensorProductOfBasedVectorSpaces(),r)
            .bundle(k|Q,TensorProductOfBasedVectorSpaces(),s)
            .bundle(r|s,Endomorphism_(),T)[MultiIndex_t<TypeList_t<ComponentIndex> >(i)];
}

} // end of namespace ComponentGeneratorEvaluator

template <typename FactorTypeList_, typename InnerProductIdTypeList_, typename Scalar_>
struct EuclideanEmbedding_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,TensorProduct_c<InnerProductIdTypeList_>,Scalar_>
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(FactorTypeList_::LENGTH == InnerProductIdTypeList_::LENGTH, LENGTHS_MUST_BE_EQUAL),
        STATIC_ASSERT_IN_ENUM((FactorTypeList_::LENGTH > 0), ORDER_MUST_BE_POSITIVE) // TODO: allow ORDER_ == 0
    };
    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList_> TensorProductOfBasedVectorSpaces;
    typedef typename EuclideanEmbedding2TensorFactorTypeList_f<TensorProductOfBasedVectorSpaces>::T EndomorphismFactorTypeList;
    typedef TensorProductOfBasedVectorSpaces_c<EndomorphismFactorTypeList> Endomorphism;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<Endomorphism>::V,
                                 ComponentGeneratorEvaluator::euclidean_embedding_induced_on_tensor_product<Endomorphism,
                                                                                                            FactorTypeList_,
                                                                                                            InnerProductIdTypeList_,
                                                                                                            Scalar_>,
                                 TensorProduct_c<InnerProductIdTypeList_> > ComponentGenerator;
public:
    typedef ImplementationOf_t<Endomorphism,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

// base case for order-1 tensor products.

namespace ComponentGeneratorEvaluator {

template <typename Endomorphism_, typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_>
Scalar_ euclidean_embedding_induced_on_1st_tensor_power (ComponentIndex_t<DimensionOf_f<Endomorphism_>::V> const &i)
{
    typedef typename EuclideanEmbedding2TensorFactorTypeList_f<BasedVectorSpace_>::T EndomorphismFactorTypeList;
    typedef TensorProductOfBasedVectorSpaces_c<EndomorphismFactorTypeList> Endomorphism;
    STATIC_ASSERT_TYPES_ARE_EQUAL(Endomorphism_, Endomorphism);
    typedef typename EuclideanEmbedding_f<BasedVectorSpace_,InnerProductId_,Scalar_>::T HeadEuclideanEmbedding;
    HeadEuclideanEmbedding head_euclidean_embedding;
    return head_euclidean_embedding[i];
}

} // end of namespace ComponentGeneratorEvaluator

// template specialization for ORDER_ == 1 (which is really just the original euclidean embedding)
// TODO: do specialization for ORDER_ == 0 (but this would depend on the basis being "field unit"
// (i.e. the 0th tensor power is the field itself and 1 is the basis vector)).
template <typename BasedVectorSpace_, typename InnerProductId_, typename Scalar_>
struct EuclideanEmbedding_f<TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedVectorSpace_> >,TensorProduct_c<TypeList_t<InnerProductId_> >,Scalar_>
{
private:
    typedef typename EuclideanEmbedding2TensorFactorTypeList_f<BasedVectorSpace_>::T EndomorphismFactorTypeList;
    typedef TensorProductOfBasedVectorSpaces_c<EndomorphismFactorTypeList> Endomorphism;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<Endomorphism>::V,
                                 ComponentGeneratorEvaluator::euclidean_embedding_induced_on_1st_tensor_power<Endomorphism,
                                                                                                              BasedVectorSpace_,
                                                                                                              InnerProductId_,
                                                                                                              Scalar_>,
                                 typename TensorPower_f<1,InnerProductId_>::T> ComponentGenerator;
public:
    typedef ImplementationOf_t<Endomorphism,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_EUCLIDEANEMBEDDING_HPP_
