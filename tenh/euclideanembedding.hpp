// ///////////////////////////////////////////////////////////////////////////
// tenh/euclideanembedding.hpp by Victor Dods, created 2013/04/08
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_EUCLIDEANEMBEDDING_HPP_
#define TENH_EUCLIDEANEMBEDDING_HPP_

#include "tenh/core.hpp"
#include "tenh/interface/tensor.hpp"

namespace Tenh {

// only provide a forward declaration of the EuclideanEmbedding_t template --
// particular implementations must be provided for each vector type (and basis).
// there is one provided for Vector_t<Scalar,DIM,StandardEuclideanBasis>
// and for each induced tensor type.
template <typename Vector>
struct EuclideanEmbedding_t;

template <typename Vector>
struct BasisOfEuclideanEmbedding_t
{
    static std::string type_as_string () { return "BasisOfEuclideanEmbedding<" + TypeStringOf_t<Vector>::eval() + '>'; }
};

// convenience meta-function to save a lot of really annoying and redundant typing
template <typename Vector>
struct EuclideanEmbedding_Parent_Tensor_i
{
    typedef Tensor_i<EuclideanEmbedding_t<Vector>,
                     TypeList_t<typename Vector::WithStandardEuclideanBasis,TypeList_t<Vector> >, // TEMP KLUDGE
                     Vector::DIM*Vector::DIM,
                     BasisOfEuclideanEmbedding_t<Vector> > T; // does "dimension" really apply to this?
                                                              // -- yes, in as much as it gives the dimension
                                                              // of the vector index.
};

} // end of namespace Tenh

#endif // TENH_EUCLIDEANEMBEDDING_HPP_
