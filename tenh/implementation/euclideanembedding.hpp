// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/euclideanembedding.hpp by Victor Dods, created 2013/09/18
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_EUCLIDEANEMBEDDING_HPP_
#define TENH_IMPLEMENTATION_EUCLIDEANEMBEDDING_HPP_

#include "tenh/core.hpp"

#include "tenh/implementation/innerproduct.hpp"
#include "tenh/implementation/tensorproduct.hpp"

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

template <Uint32 DIMENSION_>
struct BasedEuclideanSpace_f
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,DIMENSION_,EuclideanSpaceId_t<DIMENSION_> >,OrthonormalBasis_c<Generic> > T;
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

// meta-function helper for induced Euclidean embeddings

template <typename FactorTypeList_, typename InnerProductIdTypeList_, typename Scalar_>
struct EuclideanEmbeddingOfEachInTypeList_f
{
private:
    enum { STATIC_ASSERT_IN_ENUM(FactorTypeList_::LENGTH == InnerProductIdTypeList_::LENGTH, LENGTHS_MUST_BE_EQUAL) };
public:
    typedef TypeList_t<typename EuclideanEmbedding_f<typename FactorTypeList_::HeadType,
                                                     typename InnerProductIdTypeList_::HeadType,
                                                     Scalar_>::T,
                       typename EuclideanEmbeddingOfEachInTypeList_f<typename FactorTypeList_::BodyTypeList,
                                                                     typename InnerProductIdTypeList_::BodyTypeList,
                                                                     Scalar_>::T> T;
};

template <typename Scalar_>
struct EuclideanEmbeddingOfEachInTypeList_f<EmptyTypeList,EmptyTypeList,Scalar_>
{
    typedef EmptyTypeList T;
};

// TODO: do specialization for ORDER_ == 0 (but this would depend on the basis being "field unit"
// (i.e. the 0th tensor power is the field itself and 1 is the basis vector)).
template <typename FactorTypeList_, typename InnerProductIdTypeList_, typename Scalar_>
struct EuclideanEmbedding_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,TensorProduct_c<InnerProductIdTypeList_>,Scalar_>
{
private:
    typedef typename EuclideanEmbeddingOfEachInTypeList_f<FactorTypeList_,InnerProductIdTypeList_,Scalar_>::T EuclideanEmbeddingTypeList;
public:
    typedef typename TensorProductOfImmutable2Tensors_f<EuclideanEmbeddingTypeList>::T T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_EUCLIDEANEMBEDDING_HPP_
