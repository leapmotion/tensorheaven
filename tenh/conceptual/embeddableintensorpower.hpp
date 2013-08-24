// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/embeddableintensorpower.hpp by Ted Nitz, created 2013/08/22
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_EMBEDDABLEINTENSORPOWER_HPP_
#define TENH_CONCEPTUAL_EMBEDDABLEINTENSORPOWER_HPP_

#include "tenh/core.hpp"

#include "tenh/meta/typelist_utility.hpp"
#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"

namespace Tenh {

// something that has a [natural] embedding into a particular tensor power.  e.g.
// simple tensors, symmetric powers, some linear subspace of a tensor power, etc.
// NOTE: because of a problem with recursive definitions, the conceptual structure
// forgetful functors for tensor products and powers can't be used here, so the
// template parameters must be "exactly" the requested types.
template <typename Exactly_TensorPowerOfVectorSpaces_, typename Exactly_TensorProductOfVectorSpaces_>
struct EmbeddableInTensorPowerOfVectorSpaces_c
{
private:
    // NOTE: we can't static-assert that the template parameters are the types that they're required to be,
    // since the concept hierarchy isn't fully available in this file.
    typedef EmbeddableInTensorProductOfVectorSpaces_c<Exactly_TensorProductOfVectorSpaces_> As_EmbeddableInTensorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_EmbeddableInTensorProductOfVectorSpaces> ParentTypeList;

    typedef Exactly_TensorPowerOfVectorSpaces_ TensorPowerOfVectorSpaces;

    static std::string type_as_string ()
    {
        return "EmbeddableInTensorPowerOfVectorSpaces_c<" + TypeStringOf_t<Exactly_TensorPowerOfVectorSpaces_>::eval() + ','
                                                          + TypeStringOf_t<Exactly_TensorProductOfVectorSpaces_>::eval() + '>';
    }
};

template <typename Exactly_TensorPowerOfVectorSpaces_, typename Exactly_TensorProductOfVectorSpaces_>
struct IsConcept_f<EmbeddableInTensorPowerOfVectorSpaces_c<Exactly_TensorPowerOfVectorSpaces_,Exactly_TensorProductOfVectorSpaces_> >
{ static bool const V = true; };

template <typename T> struct IsEmbeddableInTensorPowerOfVectorSpaces_f { static bool const V = false; };
template <typename Exactly_TensorPowerOfVectorSpaces_, typename Exactly_TensorProductOfVectorSpaces_> struct IsEmbeddableInTensorPowerOfVectorSpaces_f<EmbeddableInTensorPowerOfVectorSpaces_c<Exactly_TensorPowerOfVectorSpaces_,Exactly_TensorProductOfVectorSpaces_> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(EmbeddableInTensorPowerOfVectorSpaces);
// special convenience macros
#define IS_EMBEDDABLE_IN_TENSOR_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueEmbeddableInTensorPowerOfVectorSpacesStructure_f<Concept>::V
#define AS_EMBEDDABLE_IN_TENSOR_POWER_OF_VECTOR_SPACES(Concept) UniqueEmbeddableInTensorPowerOfVectorSpacesStructureOf_f<Concept>::T

// NOTE: because the class of embeddable-in-tensor-powers-of-vector-spaces includes nonlinear
// spaces such as the space of simple tensors, there is no general notion of Dual here.  That
// could only be had if the embeddable was a vector space.


// something that has a [natural] embedding into a particular tensor power of based vector spaces.
// e.g. everything in the class of embeddable-in-tensor-powers-of-vector-spaces, but also things
// like diagonal 2-tensor power of based vector spaces.
// NOTE: because of a problem with recursive definitions, the conceptual structure
// forgetful functors for tensor products and powers can't be used here, so the
// template parameters must be "exactly" the requested types.
template <typename Exactly_TensorPowerOfBasedVectorSpaces_,
          typename Exactly_TensorPowerOfVectorSpaces_,
          typename Exactly_TensorProductOfBasedVectorSpaces_,
          typename Exactly_TensorProductOfVectorSpaces_>
struct EmbeddableInTensorPowerOfBasedVectorSpaces_c
{
private:
    // NOTE: we can't static-assert that the template parameters are the types that they're required to be,
    // since the concept hierarchy isn't fully available in this file.
    typedef EmbeddableInTensorPowerOfVectorSpaces_c<Exactly_TensorPowerOfVectorSpaces_,
                                                    Exactly_TensorProductOfVectorSpaces_> As_EmbeddableInTensorPowerOfVectorSpaces;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<Exactly_TensorPowerOfBasedVectorSpaces_,
                                                           Exactly_TensorProductOfVectorSpaces_> As_EmbeddableInTensorProductOfBasedVectorSpaces;
public:
    typedef TypeList_t<As_EmbeddableInTensorPowerOfVectorSpaces,
            TypeList_t<As_EmbeddableInTensorProductOfBasedVectorSpaces> > ParentTypeList;

    typedef Exactly_TensorPowerOfBasedVectorSpaces_ TensorPowerOfBasedVectorSpaces;

    static std::string type_as_string ()
    {
        return "EmbeddableInTensorPowerOfBasedVectorSpaces_c<" + TypeStringOf_t<Exactly_TensorPowerOfBasedVectorSpaces_>::eval() + ','
                                                               + TypeStringOf_t<Exactly_TensorPowerOfVectorSpaces_>::eval() + ','
                                                               + TypeStringOf_t<Exactly_TensorProductOfBasedVectorSpaces_>::eval() + ','
                                                               + TypeStringOf_t<Exactly_TensorProductOfVectorSpaces_>::eval() + '>';
    }
};

template <typename Exactly_TensorPowerOfBasedVectorSpaces_,
          typename Exactly_TensorPowerOfVectorSpaces_,
          typename Exactly_TensorProductOfBasedVectorSpaces_,
          typename Exactly_TensorProductOfVectorSpaces_>
struct IsConcept_f<EmbeddableInTensorPowerOfBasedVectorSpaces_c<Exactly_TensorPowerOfBasedVectorSpaces_,Exactly_TensorPowerOfVectorSpaces_,Exactly_TensorProductOfBasedVectorSpaces_,Exactly_TensorProductOfVectorSpaces_> >
{ static bool const V = true; };

template <typename T> struct IsEmbeddableInTensorPowerOfBasedVectorSpaces_f { static bool const V = false; };
template <typename Exactly_TensorPowerOfBasedVectorSpaces_,
          typename Exactly_TensorPowerOfVectorSpaces_,
          typename Exactly_TensorProductOfBasedVectorSpaces_,
          typename Exactly_TensorProductOfVectorSpaces_> struct IsEmbeddableInTensorPowerOfBasedVectorSpaces_f<EmbeddableInTensorPowerOfBasedVectorSpaces_c<Exactly_TensorPowerOfBasedVectorSpaces_,Exactly_TensorPowerOfVectorSpaces_,Exactly_TensorProductOfBasedVectorSpaces_,Exactly_TensorProductOfVectorSpaces_> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(EmbeddableInTensorPowerOfBasedVectorSpaces);
// special convenience macros
#define IS_EMBEDDABLE_IN_TENSOR_POWER_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueEmbeddableInTensorPowerOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_EMBEDDABLE_IN_TENSOR_POWER_OF_BASED_VECTOR_SPACES(Concept) UniqueEmbeddableInTensorPowerOfBasedVectorSpacesStructureOf_f<Concept>::T

// TODO (?): LinearlyEmbeddableInTensorPowerOfVectorSpaces_c -- this may possibly have a naturally induced Dual

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_EMBEDDABLEINTENSORPOWER_HPP_
