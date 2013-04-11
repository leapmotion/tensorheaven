// ///////////////////////////////////////////////////////////////////////////
// tenh/euclideanembedding.hpp by Victor Dods, created 2013/04/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_EUCLIDEANEMBEDDING_HPP_
#define TENH_EUCLIDEANEMBEDDING_HPP_

#include "tenh/core.hpp"
#include "tenh/interface/tensor.hpp"

namespace Tenh {

template <typename Vector>
struct EuclideanEmbedding_t;

// convenience meta-function to save a lot of really annoying and redundant typing
template <typename Vector>
struct EuclideanEmbedding_Parent_Tensor_i
{
    typedef Tensor_i<EuclideanEmbedding_t<Vector>,
                     TypeList_t<Vector,TypeList_t<Vector> >,
                     Vector::DIM*Vector::DIM> T;  // does "dimension" really apply to this?
                                                  // -- yes, in as much as it gives the dimension
                                                  // of the vector index.
};
/*
// NOTE: while this is a tensor, it isn't a tensor space, and so it technically shouldn't be used as a factor
// type in a tensor product.  this is essentially a constant value -- it has only const accessors and can't be written to.
template <typename Vector>
struct EuclideanEmbedding_t : public EuclideanEmbedding_Parent_Tensor_i<Vector>::T
{
    typedef typename EuclideanEmbedding_Parent_Tensor_i<Vector>::T Parent_Tensor_i;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::Index Index;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::FactorIndexTypeList FactorIndexTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::DEGREE;

    Scalar operator [] (MultiIndex const &m) const { return m.template el<0>() == m.template el<1>() ? Scalar(1) : Scalar(0); }

    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Scalar component_access_without_range_check (Uint32 i) const
    {
        Uint32 row = i / Vector::DIM;
        Uint32 col = i % Vector::DIM;
        return row == col ? Scalar(1) : Scalar(0);
    }

    // NOTE: these may be unnecessary/undesired, because this type does not represent a vector space
//     using Parent_Tensor_i::component_is_immutable_zero;
//     using Parent_Tensor_i::scalar_factor_for_component;
//     using Parent_Tensor_i::vector_index_of;
};
*/
} // end of namespace Tenh

#endif // TENH_EUCLIDEANEMBEDDING_HPP_
