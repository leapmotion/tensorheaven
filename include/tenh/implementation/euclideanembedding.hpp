// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/euclideanembedding.hpp by Victor Dods, created 2013/09/18
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_EUCLIDEANEMBEDDING_HPP_
#define TENH_IMPLEMENTATION_EUCLIDEANEMBEDDING_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/innerproduct.hpp"
#include "tenh/implementation/tensorproduct.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// forward declaration for the EuclideanEmbedding_f metafunction (should
// return an ImplementationOf_t type that uses ProceduralArray_t)
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

    static std::string type_as_string () { return "EuclideanSpaceId_t<" + FORMAT(DIMENSION_) + '>'; }
};

template <Uint32 DIMENSION_>
struct BasedEuclideanSpace_f
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,DIMENSION_,EuclideanSpaceId_t<DIMENSION_>>,OrthonormalBasis_c<Generic>> T;
private:
    BasedEuclideanSpace_f();
};

// metafunction for easily determining the 2-tensor factors for a Euclidean embedding
// a linear map from BasedVectorSpace_ to BasedEuclideanSpace_f
template <typename BasedVectorSpace_>
struct EuclideanEmbedding2TensorFactorTyple_f
{
    typedef Typle_t<typename BasedEuclideanSpace_f<DimensionOf_f<BasedVectorSpace_>::V>::T,
                    typename DualOf_f<BasedVectorSpace_>::T> T;
private:
    EuclideanEmbedding2TensorFactorTyple_f();
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
    typedef typename EuclideanEmbedding2TensorFactorTyple_f<BasedVectorSpace>::T EuclideanEmbedding2TensorFactorTyple;
    typedef typename Diagonal2TensorProductOfBasedVectorSpaces_f<EuclideanEmbedding2TensorFactorTyple>::T Diagonal2Tensor;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<Diagonal2Tensor>::V,
                                 constant_component_generator_evaluator<Scalar_,DimensionOf_f<Diagonal2Tensor>::V,1>,
                                 StandardInnerProduct> ComponentGenerator;
    EuclideanEmbedding_f();
public:
    typedef ImplementationOf_t<Diagonal2Tensor,Scalar_,UseProceduralArray_t<ComponentGenerator> > T;
};

// ///////////////////////////////////////////////////////////////////////////
// induced Euclidean embedding on TensorProductOfBasedVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

// meta-function helper for induced Euclidean embeddings

template <typename FactorTyple_, typename InnerProductIdTyple_, typename Scalar_>
struct EuclideanEmbeddingOfEachInTyple_f
{
private:
    enum { STATIC_ASSERT_IN_ENUM(Length_f<FactorTyple_>::V == Length_f<InnerProductIdTyple_>::V, LENGTHS_MUST_BE_EQUAL) };
    EuclideanEmbeddingOfEachInTyple_f();
    typedef typename EuclideanEmbedding_f<typename Head_f<FactorTyple_>::T,
                                          typename Head_f<InnerProductIdTyple_>::T,
                                          Scalar_>::T HeadEuclideanEmbedding;
    typedef typename EuclideanEmbeddingOfEachInTyple_f<typename BodyTyple_f<FactorTyple_>::T,
                                                       typename BodyTyple_f<InnerProductIdTyple_>::T,
                                                       Scalar_>::T BodyEuclideanEmbeddingTyple;
public:
    typedef typename HeadBodyTyple_f<HeadEuclideanEmbedding,BodyEuclideanEmbeddingTyple>::T T;
};

template <typename Scalar_>
struct EuclideanEmbeddingOfEachInTyple_f<Typle_t<>,Typle_t<>,Scalar_>
{
    typedef Typle_t<> T;
private:
    EuclideanEmbeddingOfEachInTyple_f();
};

// TODO: do specialization for ORDER_ == 0 (but this would depend on the basis being "field unit"
// (i.e. the 0th tensor power is the field itself and 1 is the basis vector)).
template <typename FactorTyple_, typename InnerProductIdTyple_, typename Scalar_>
struct EuclideanEmbedding_f<TensorProductOfBasedVectorSpaces_c<FactorTyple_>,TensorProduct_c<InnerProductIdTyple_>,Scalar_>
{
private:
    typedef typename EuclideanEmbeddingOfEachInTyple_f<FactorTyple_,InnerProductIdTyple_,Scalar_>::T EuclideanEmbeddingTyple;
    EuclideanEmbedding_f();
public:
    typedef typename TensorProductOfProcedural2Tensors_f<EuclideanEmbeddingTyple>::T T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_EUCLIDEANEMBEDDING_HPP_
