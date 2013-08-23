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
#include "tenh/conceptual/tensorpower.hpp"

namespace Tenh {

// something that has a [natural] embedding into a particular tensor power.  e.g.
// simple tensors, symmetric powers, some linear subspace of a tensor power, etc.
template <typename TensorPowerOfVectorSpaces_>
struct EmbeddableInTensorPowerOfVectorSpaces_c
{
private:
    // TODO: Figure out why this is failing.
//    enum { STATIC_ASSERT_IN_ENUM(IS_TENSOR_POWER_OF_VECTOR_SPACES_UNIQUELY(TensorPowerOfVectorSpaces_), MUST_BE_TENSOR_POWER_OF_VECTOR_SPACES) };
    typedef EmbeddableInTensorProductOfVectorSpaces_c<TensorPowerOfVectorSpaces_> As_EmbeddableInTensorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_EmbeddableInTensorProductOfVectorSpaces> ParentTypeList;

    typedef TensorPowerOfVectorSpaces_ TensorPowerOfVectorSpaces;

    static std::string type_as_string ()
    {
        return "EmbeddableInTensorPowerOfVectorSpaces_c<" + TypeStringOf_t<TensorPowerOfVectorSpaces_>::eval() + '>';
    }
};

template <typename TensorPowerOfVectorSpaces_>
struct IsConcept_f<EmbeddableInTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_> >
{ static bool const V = true; };

template <typename T> struct IsEmbeddableInTensorPowerOfVectorSpaces_f { static bool const V = false; };
template <typename TensorPowerOfVectorSpaces> struct IsEmbeddableInTensorPowerOfVectorSpaces_f<EmbeddableInTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces> > { static bool const V = true; };

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
template <typename TensorPowerOfBasedVectorSpaces_>
struct EmbeddableInTensorPowerOfBasedVectorSpaces_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_TENSOR_POWER_OF_BASED_VECTOR_SPACES_UNIQUELY(TensorPowerOfBasedVectorSpaces_), MUST_BE_TENSOR_POWER_OF_BASED_VECTOR_SPACES) };
    typedef EmbeddableInTensorPowerOfVectorSpaces_c<TensorPowerOfBasedVectorSpaces_> As_EmbeddableInTensorPowerOfVectorSpaces;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorPowerOfBasedVectorSpaces_> As_EmbeddableInTensorProductOfBasedVectorSpaces;
public:
    typedef TypeList_t<As_EmbeddableInTensorPowerOfVectorSpaces,
            TypeList_t<As_EmbeddableInTensorProductOfBasedVectorSpaces> > ParentTypeList;

    typedef typename As_EmbeddableInTensorPowerOfVectorSpaces::TensorPowerOfVectorSpaces TensorPowerOfVectorSpaces;
    typedef TensorPowerOfBasedVectorSpaces_ TensorPowerOfBasedVectorSpaces;
    static Uint32 const ORDER = TensorPowerOfBasedVectorSpaces::ORDER;
    typedef typename TensorPowerOfBasedVectorSpaces::FactorTypeList FactorTypeList;
    // TODO: other typedefs/values?

    static std::string type_as_string ()
    {
        return "EmbeddableInTensorPowerOfBasedVectorSpaces_c<" + TypeStringOf_t<TensorPowerOfBasedVectorSpaces>::eval() + '>';
    }
};

template <typename TensorPowerOfBasedVectorSpaces_>
struct IsConcept_f<EmbeddableInTensorPowerOfBasedVectorSpaces_c<TensorPowerOfBasedVectorSpaces_> >
{ static bool const V = true; };

template <typename T> struct IsEmbeddableInTensorPowerOfBasedVectorSpaces_f { static bool const V = false; };
template <typename TensorPowerOfBasedVectorSpaces> struct IsEmbeddableInTensorPowerOfBasedVectorSpaces_f<EmbeddableInTensorPowerOfBasedVectorSpaces_c<TensorPowerOfBasedVectorSpaces> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(EmbeddableInTensorPowerOfBasedVectorSpaces);
// special convenience macros
#define IS_EMBEDDABLE_IN_TENSOR_POWER_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueEmbeddableInTensorPowerOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_EMBEDDABLE_IN_TENSOR_POWER_OF_BASED_VECTOR_SPACES(Concept) UniqueEmbeddableInTensorPowerOfBasedVectorSpacesStructureOf_f<Concept>::T

// TODO (?): LinearlyEmbeddableInTensorPowerOfVectorSpaces_c -- this may possibly have a naturally induced Dual

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_EMBEDDABLEINTENSORPOWER_HPP_
