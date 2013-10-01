// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/embeddableintensorproduct.hpp by Victor Dods, created 2013/08/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_EMBEDDABLEINTENSORPRODUCT_HPP_
#define TENH_CONCEPTUAL_EMBEDDABLEINTENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"

namespace Tenh {

// something that has a [natural] embedding into a particular tensor product.  e.g.
// simple tensors, symmetric products, some linear subspace of a tensor product, etc.
// NOTE: because of a problem with recursive definitions, the conceptual structure
// forgetful functors for tensor products and powers can't be used here, so the
// template parameters must be "exactly" the requested types.
template <typename Exactly_TensorProductOfVectorSpaces_>
struct EmbeddableInTensorProductOfVectorSpaces_c
{
    // NOTE: we can't static-assert that the template parameters are the types that they're required to be,
    // since the concept hierarchy isn't fully available in this file.
    typedef EmptyTypeList ParentTypeList;

    typedef Exactly_TensorProductOfVectorSpaces_ TensorProductOfVectorSpaces;

    static std::string type_as_string ()
    {
        return "EmbeddableInTensorProductOfVectorSpaces_c<" + type_string_of<Exactly_TensorProductOfVectorSpaces_>() + '>';
    }
};

template <typename TensorProductOfVectorSpaces_>
struct IsConcept_f<EmbeddableInTensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces_> >
{ static bool const V = true; };

template <typename T> struct IsEmbeddableInTensorProductOfVectorSpaces_f { static bool const V = false; };
template <typename TensorProductOfVectorSpaces_> struct IsEmbeddableInTensorProductOfVectorSpaces_f<EmbeddableInTensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces_> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(EmbeddableInTensorProductOfVectorSpaces);
// special convenience macros
#define IS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueEmbeddableInTensorProductOfVectorSpacesStructure_f<Concept>::V
#define AS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_VECTOR_SPACES(Concept) UniqueEmbeddableInTensorProductOfVectorSpacesStructureOf_f<Concept>::T

// NOTE: because the class of embeddable-in-tensor-products-of-vector-spaces includes nonlinear
// spaces such as the space of simple tensors, there is no general notion of Dual here.  That
// could only be had if the embeddable was a vector space.


// something that has a [natural] embedding into a particular tensor product of based vector spaces.
// e.g. everything in the class of embeddable-in-tensor-products-of-vector-spaces, but also things
// like diagonal 2-tensor product of based vector spaces.
// NOTE: because of a problem with recursive definitions, the conceptual structure
// forgetful functors for tensor products and powers can't be used here, so the
// template parameters must be "exactly" the requested types.
template <typename Exactly_TensorProductOfBasedVectorSpaces_, typename Exactly_TensorProductOfVectorSpaces_>
struct EmbeddableInTensorProductOfBasedVectorSpaces_c
{
private:
    // NOTE: we can't static-assert that the template parameters are the types that they're required to be,
    // since the concept hierarchy isn't fully available in this file.
    typedef EmbeddableInTensorProductOfVectorSpaces_c<Exactly_TensorProductOfVectorSpaces_> As_EmbeddableInTensorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_EmbeddableInTensorProductOfVectorSpaces> ParentTypeList;

    typedef Exactly_TensorProductOfBasedVectorSpaces_ TensorProductOfBasedVectorSpaces;

    static std::string type_as_string ()
    {
        return "EmbeddableInTensorProductOfBasedVectorSpaces_c<" + type_string_of<Exactly_TensorProductOfBasedVectorSpaces_>() + ','
                                                                 + type_string_of<Exactly_TensorProductOfVectorSpaces_>() + '>';
    }
};

template <typename Exactly_TensorProductOfBasedVectorSpaces_, typename Exactly_TensorProductOfVectorSpaces_>
struct IsConcept_f<EmbeddableInTensorProductOfBasedVectorSpaces_c<Exactly_TensorProductOfBasedVectorSpaces_,Exactly_TensorProductOfVectorSpaces_> >
{ static bool const V = true; };

template <typename T> struct IsEmbeddableInTensorProductOfBasedVectorSpaces_f { static bool const V = false; };
template <typename Exactly_TensorProductOfBasedVectorSpaces_, typename Exactly_TensorProductOfVectorSpaces_> struct IsEmbeddableInTensorProductOfBasedVectorSpaces_f<EmbeddableInTensorProductOfBasedVectorSpaces_c<Exactly_TensorProductOfBasedVectorSpaces_,Exactly_TensorProductOfVectorSpaces_> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(EmbeddableInTensorProductOfBasedVectorSpaces);
// special convenience macros
#define IS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueEmbeddableInTensorProductOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(Concept) UniqueEmbeddableInTensorProductOfBasedVectorSpacesStructureOf_f<Concept>::T

// TODO (?): LinearlyEmbeddableInTensorProductOfVectorSpaces_c -- this may possibly have a naturally induced Dual

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_EMBEDDABLEINTENSORPRODUCT_HPP_
