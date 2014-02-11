// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/concept.hpp by Ted Nitz, created 2013/08/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_CONCEPT_HPP_
#define TENH_CONCEPTUAL_CONCEPT_HPP_

#include "tenh/core.hpp"

#include "tenh/meta/typestringof.hpp"
#include "tenh/meta/typle.hpp"
#include "tenh/meta/typle_utility.hpp"

// This header defines helper meta functions fo defining and using conceptual inheritence in mathematical code.
// Here is an example object template for the concept Concept Example, which has a Parent Concept structure.
/*
template <typename Parameter_>
struct ConceptExample_c
{
private:
    typedef ParentConcept_c<Parameter_> As_ParentConcept;
public:
    typedef Typle_t<As_ParentConcept> ParentTyple;

    // ...
};

template <typename Parameter_>
struct IsConcept_f<ConceptExample_c<Parameter_> >
{ static bool const V = true; };

template <typename T> struct IsConceptExample_f { static bool const V = false; };

template <typename Parameter_>
struct IsConceptExample_f<ConceptExample_c<Parameter_> >
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ConceptExample);
// special convenience macros
#define IS_CONCEPT_EXAMPLE_UNIQUELY(Concept) HasUniqueConceptExampleStructure_f<Concept>::V
#define AS_CONCEPT_EXAMPLE(Concept) UniqueConceptExampleStructureOf_f<Concept>::T

*/


// TODO: Integrate disambiguation maps
namespace Tenh {

// a Concept is a struct that has a ParentTyple (which is a Typle_t)
template <typename T>
struct IsConcept_f
{
    static bool const V = false;
private:
    IsConcept_f();
};

// default definition for non-concepts -- no parents
template <typename Concept_, bool ACTUALLY_IS_CONCEPT_ = IsConcept_f<Concept_>::V>
struct ParentTypleOfConcept_f;

// template specialization for non-concepts -- Typle_t<>
template <typename Concept_>
struct ParentTypleOfConcept_f<Concept_,false>
{
    typedef Typle_t<> T;
};

// template specialization for concepts -- uses Concept_::ParentTyple
template <typename Concept_>
struct ParentTypleOfConcept_f<Concept_,true>
{
    typedef typename Concept_::ParentTyple T;
};

// parent-traversing metafunctions

template <typename ParentTyple_>
struct AncestorsOf_Recursive_f;

// "ancestors" of Concept include Concept as a trivial ancestor -- if Concept
// isn't actually a concept, T will be Typle_t<>
template <typename Concept_>
struct AncestorsOf_f
{
private:
    AncestorsOf_f();
public:
    typedef typename HeadBodyTyple_f<Concept_,
                                     typename AncestorsOf_Recursive_f<typename ParentTypleOfConcept_f<Concept_>::T>::T>::T T;
};

template <typename ParentTyple_>
struct AncestorsOf_Recursive_f
{
    // depth-first traversal of the ancestor tree
    typedef typename Concat2Typles_f<typename AncestorsOf_f<typename Head_f<ParentTyple_>::T>::T,
                                     typename AncestorsOf_Recursive_f<typename BodyTyple_f<ParentTyple_>::T>::T>::T T;
private:
    AncestorsOf_Recursive_f();
};

template <typename Head_>
struct AncestorsOf_Recursive_f<Typle_t<Head_> >
{
    // depth-first traversal of the ancestor tree
    typedef typename AncestorsOf_f<Head_>::T T;
private:
    AncestorsOf_Recursive_f();
};

template <>
struct AncestorsOf_Recursive_f<Typle_t<>>
{
    // depth-first traversal of the ancestor tree
    typedef Typle_t<> T;
private:
    AncestorsOf_Recursive_f();
};

template <typename Concept_, typename Predicate_e_>
struct AncestorsSatisfyingPredicate_f
{
    typedef typename ElementsOfTypleSatisfying_f<typename AncestorsOf_f<Concept_>::T,Predicate_e_>::T T;
private:
    AncestorsSatisfyingPredicate_f();
};

// for recursively retrieving various conceptual structures

// note that if Concept isn't actually a concept, then T will be Typle_t<>.
template <typename Concept_, typename ConceptualStructurePredicate_e_>
struct ConceptualStructuresOf_f
{
private:
    ConceptualStructuresOf_f();
public:
    typedef typename UniqueTypesIn_f<typename AncestorsSatisfyingPredicate_f<Concept_,ConceptualStructurePredicate_e_>::T>::T T;
};

template <typename Concept_, typename ConceptualStructurePredicate_e_>
struct HasConceptualStructure_f
{
    static bool const V = Length_f<typename ConceptualStructuresOf_f<Concept_,ConceptualStructurePredicate_e_>::T>::V > 0;
private:
    HasConceptualStructure_f();
};

template <typename Concept_, typename ConceptualStructurePredicate_e_>
struct HasUniqueConceptualStructure_f
{
    static bool const V = Length_f<typename ConceptualStructuresOf_f<Concept_,ConceptualStructurePredicate_e_>::T>::V == 1;
private:
    HasUniqueConceptualStructure_f();
};

template <typename Concept_, typename ConceptualStructurePredicate_e_>
struct UniqueConceptualStructureOf_f
{
    static_assert(HasUniqueConceptualStructure_f<Concept_,ConceptualStructurePredicate_e_>::V, "UniqueConceptualStructureOf_f is only well-defined if there is exactly one such structure");
private:
    UniqueConceptualStructureOf_f();
public:
    typedef typename Head_f<typename ConceptualStructuresOf_f<Concept_,ConceptualStructurePredicate_e_>::T>::T T;
};

// easy way to provide Concept-specific structure metafunctions
#define DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ConceptName) \
MAKE_1_ARY_VALUE_EVALUATOR(Is##ConceptName); \
template <typename Concept_> struct ConceptName##StructuresOf_f \
{ \
    typedef typename ConceptualStructuresOf_f<Concept_,Is##ConceptName##_e>::T T; \
private: \
    ConceptName##StructuresOf_f(); \
}; \
template <typename Concept_> struct Has##ConceptName##Structure_f \
{ \
    static bool const V = HasConceptualStructure_f<Concept_,Is##ConceptName##_e>::V; \
private: \
    Has##ConceptName##Structure_f(); \
}; \
template <typename Concept_> struct HasUnique##ConceptName##Structure_f \
{ \
    static bool const V = HasUniqueConceptualStructure_f<Concept_,Is##ConceptName##_e>::V; \
private: \
    HasUnique##ConceptName##Structure_f(); \
}; \
template <typename Concept_> struct Unique##ConceptName##StructureOf_f \
{ \
    typedef typename UniqueConceptualStructureOf_f<Concept_,Is##ConceptName##_e>::T T; \
private: \
    Unique##ConceptName##StructureOf_f(); \
}; \
MAKE_1_ARY_TYPE_EVALUATOR(ConceptName##StructuresOf); \
MAKE_1_ARY_VALUE_EVALUATOR(Has##ConceptName##Structure); \
MAKE_1_ARY_VALUE_EVALUATOR(HasUnique##ConceptName##Structure); \
MAKE_1_ARY_TYPE_EVALUATOR(Unique##ConceptName##StructureOf)

// ///////////////////////////////////////////////////////////////////////////
// conceptual property accessor machinery
// ///////////////////////////////////////////////////////////////////////////

// default definition of the given property of exactly the given concept (but
// NOT its ancestors).  particular concepts should specialize this to provide
// definitions.  if the property is a value (e.g. Uint32), then the type should
// be Value_t<...>
template <typename Concept_, typename PropertyId_>
struct BaseProperty_f
{
    typedef NullValue T;
private:
    BaseProperty_f();
};

template <typename Typle_, typename PropertyId_>
struct BasePropertyOfEachInTyple_f
{
    typedef typename HeadBodyTyple_f<typename BaseProperty_f<typename Head_f<Typle_>::T,PropertyId_>::T,
                                     typename BasePropertyOfEachInTyple_f<typename BodyTyple_f<Typle_>::T,PropertyId_>::T>::T T;
private:
    BasePropertyOfEachInTyple_f();
};

template <typename PropertyId_>
struct BasePropertyOfEachInTyple_f<Typle_t<>,PropertyId_>
{
    typedef Typle_t<> T;
private:
    BasePropertyOfEachInTyple_f();
};

// gives a typle of the unique values of the given property, taken from all ancestors.
template <typename Concept_, typename PropertyId_>
struct MultiProperty_f
{
private:
    typedef typename BasePropertyOfEachInTyple_f<typename AncestorsOf_f<Concept_>::T,PropertyId_>::T PropertyOfEach;
    typedef typename UniqueTypesIn_f<PropertyOfEach>::T UniquePropertyTyple;
    MultiProperty_f();
public:
    typedef typename SetSubtraction_f<UniquePropertyTyple,Typle_t<NullValue> >::T T;
};

// if MultiProperty_f has exactly one element, returns that.
template <typename Concept_, typename PropertyId_>
struct Property_f
{
private:
    typedef typename MultiProperty_f<Concept_,PropertyId_>::T MultiProperty;
    static_assert(Length_f<MultiProperty>::V == 1, "property is not well-defined");
    Property_f();
public:
    typedef typename Head_f<MultiProperty>::T T;
};

// if Property_f is a Value_t, returns the value.
template <typename Concept_, typename PropertyId_>
struct PropertyValue_f
{
private:
    typedef typename Property_f<Concept_,PropertyId_>::T ValueType;
    PropertyValue_f();
public:
    static typename ValueType::T const V = ValueType::V;
};

// constructs a Typle_t whose elements are the results of Property_f on each element
template <typename Typle_, typename PropertyId_>
struct PropertyOfEachInTyple_f
{
    typedef typename HeadBodyTyple_f<typename Property_f<typename Head_f<Typle_>::T,PropertyId_>::T,
                                     typename PropertyOfEachInTyple_f<typename BodyTyple_f<Typle_>::T,PropertyId_>::T>::T T;
private:
    PropertyOfEachInTyple_f();
};

// base case
template <typename PropertyId_>
struct PropertyOfEachInTyple_f<Typle_t<>,PropertyId_>
{
    typedef Typle_t<> T;
private:
    PropertyOfEachInTyple_f();
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_CONCEPT_HPP_