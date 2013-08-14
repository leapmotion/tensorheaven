// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/embeddableintensorproduct.hpp by Victor Dods, created 2013/08/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_EMBEDDABLEINTENSORPRODUCT_HPP_
#define TENH_CONCEPTUAL_EMBEDDABLEINTENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/tensorproduct.hpp"

namespace Tenh {

// something that has a [natural] embedding into a particular tensor product.  e.g.
// simple tensors, symmetric products, some linear subspace of a tensor product, etc.
template <typename TensorProductOfVectorSpaces_>
struct EmbeddableInTensorProductOfVectorSpaces_c
{
    enum { STATIC_ASSERT_IN_ENUM(IsATensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces_>::V, MUST_BE_TENSOR_PRODUCT_OF_VECTOR_SPACES) };

    typedef TensorProductOfVectorSpaces_ TensorProductOfVectorSpaces;

    static std::string type_as_string ()
    {
        return "EmbeddableInTensorProductOfVectorSpaces_c<" + TypeStringOf_t<TensorProductOfVectorSpaces>::eval() + '>';
    }
};

template <typename T> struct IsEmbeddableInTensorProductOfVectorSpaces_c { static bool const V = false; };
template <typename TensorProductOfVectorSpaces> struct IsEmbeddableInTensorProductOfVectorSpaces_c<EmbeddableInTensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces> > { static bool const V = true; };

// NOTE: because the class of embeddable-in-tensor-products-of-vector-spaces includes nonlinear 
// spaces such as the space of simple tensors, there is no general notion of Dual here.  That 
// could only be had if the embeddable was a vector space.


// something that has a [natural] embedding into a particular tensor product of based vector spaces.
// e.g. everything in the class of embeddable-in-tensor-products-of-vector-spaces, but also things
// like diagonal 2-tensor product of based vector spaces.
template <typename TensorProductOfBasedVectorSpaces_>
struct EmbeddableInTensorProductOfBasedVectorSpaces_c
    : 
    public EmbeddableInTensorProductOfVectorSpaces_c<TensorProductOfBasedVectorSpaces_>
{
    typedef EmbeddableInTensorProductOfVectorSpaces_c<TensorProductOfBasedVectorSpaces_> Parent_EmbeddableInTensorProduct;

    enum { STATIC_ASSERT_IN_ENUM(IsATensorProductOfBasedVectorSpaces_c<TensorProductOfBasedVectorSpaces_>::V, MUST_BE_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES) };

    typedef typename Parent_EmbeddableInTensorProduct::TensorProductOfVectorSpaces TensorProductOfVectorSpaces;
    typedef TensorProductOfBasedVectorSpaces_ TensorProductOfBasedVectorSpaces;

    static std::string type_as_string ()
    {
        return "EmbeddableInTensorProductOfBasedVectorSpaces_c<" + TypeStringOf_t<TensorProductOfBasedVectorSpaces>::eval() + '>';
    }
};

template <typename TensorProductOfBasedVectorSpaces> struct IsEmbeddableInTensorProductOfVectorSpaces_c<EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorProductOfBasedVectorSpaces> > { static bool const V = true; };

template <typename T> struct IsEmbeddableInTensorProductOfBasedVectorSpaces_c { static bool const V = false; };
template <typename TensorProductOfBasedVectorSpaces> struct IsEmbeddableInTensorProductOfBasedVectorSpaces_c<EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorProductOfBasedVectorSpaces> > { static bool const V = true; };


// TODO (?): LinearlyEmbeddableInTensorProductOfVectorSpaces_c -- this may possibly have a naturally induced Dual

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_EMBEDDABLEINTENSORPRODUCT_HPP_
