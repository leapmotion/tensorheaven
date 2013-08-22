// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/embeddableintensorproduct.hpp by Victor Dods, created 2013/08/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_EMBEDDABLEINTENSORPRODUCT_HPP_
#define TENH_CONCEPTUAL_EMBEDDABLEINTENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/tensorproduct.hpp"

namespace Tenh {

// something that has a [natural] embedding into a particular tensor product.  e.g.
// simple tensors, symmetric products, some linear subspace of a tensor product, etc.
template <typename TensorProductOfVectorSpaces_>
struct EmbeddableInTensorProductOfVectorSpaces_c
{
    typedef EmptyTypeList ParentTypeList;

    enum { STATIC_ASSERT_IN_ENUM(HasTensorProductOfVectorSpacesStructure_f<TensorProductOfVectorSpaces_>::V, MUST_BE_TENSOR_PRODUCT_OF_VECTOR_SPACES) };

    typedef TensorProductOfVectorSpaces_ TensorProductOfVectorSpaces;

    static std::string type_as_string ()
    {
        return "EmbeddableInTensorProductOfVectorSpaces_c<" + TypeStringOf_t<TensorProductOfVectorSpaces>::eval() + '>';
    }
};

template <typename TensorProductOfVectorSpaces_>
struct IsConcept_f<EmbeddableInTensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces_> >
{ static bool const V = true; };

template <typename T> struct IsEmbeddableInTensorProductOfVectorSpaces_f { static bool const V = false; };
template <typename TensorProductOfVectorSpaces> struct IsEmbeddableInTensorProductOfVectorSpaces_f<EmbeddableInTensorProductOfVectorSpaces_c<TensorProductOfVectorSpaces> > { static bool const V = true; };

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
template <typename TensorProductOfBasedVectorSpaces_>
struct EmbeddableInTensorProductOfBasedVectorSpaces_c
{
private:
    typedef EmbeddableInTensorProductOfVectorSpaces_c<TensorProductOfBasedVectorSpaces_> As_EmbeddableInTensorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_EmbeddableInTensorProductOfVectorSpaces> ParentTypeList;

    enum { STATIC_ASSERT_IN_ENUM(HasTensorProductOfBasedVectorSpacesStructure_f<TensorProductOfBasedVectorSpaces_>::V, MUST_BE_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES) };


    typedef typename As_EmbeddableInTensorProductOfVectorSpaces::TensorProductOfVectorSpaces TensorProductOfVectorSpaces;
    typedef TensorProductOfBasedVectorSpaces_ TensorProductOfBasedVectorSpaces;
    static Uint32 const ORDER = TensorProductOfBasedVectorSpaces::ORDER;
    typedef typename TensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;
    // TODO: other typedefs/values?

    static std::string type_as_string ()
    {
        return "EmbeddableInTensorProductOfBasedVectorSpaces_c<" + TypeStringOf_t<TensorProductOfBasedVectorSpaces>::eval() + '>';
    }
};

template <typename TensorProductOfBasedVectorSpaces_>
struct IsConcept_f<EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorProductOfBasedVectorSpaces_> >
{ static bool const V = true; };

template <typename T> struct IsEmbeddableInTensorProductOfBasedVectorSpaces_f { static bool const V = false; };
template <typename TensorProductOfBasedVectorSpaces> struct IsEmbeddableInTensorProductOfBasedVectorSpaces_f<EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorProductOfBasedVectorSpaces> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(EmbeddableInTensorProductOfBasedVectorSpaces);
// special convenience macros
#define IS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueEmbeddableInTensorProductOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(Concept) UniqueEmbeddableInTensorProductOfBasedVectorSpacesStructureOf_f<Concept>::T

// TODO (?): LinearlyEmbeddableInTensorProductOfVectorSpaces_c -- this may possibly have a naturally induced Dual

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_EMBEDDABLEINTENSORPRODUCT_HPP_
