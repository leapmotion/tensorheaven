// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/concept.hpp by Ted Nitz, created 2013/08/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_CONCEPT_HPP_
#define TENH_CONCEPTUAL_CONCEPT_HPP_

#include "tenh/core.hpp"

#include "tenh/meta/typelist.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/meta/typestringof.hpp"

// This header defines helper meta functions fo defining and using conceptual inheritence in mathematical code.
// Here is an example object template for the concept Concept Example, which has a Parent Concept structure.
/*
template <typename Parameter_>
struct ConceptExample_c
{
private:
    typedef ParentConcept_c<Parameter_> As_ParentConcept;
public:
    typedef TypeList_t<As_ParentConcept> ParentTypeList;

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

// a Concept is a struct that has a ParentTypeList (which is a TypeList_t)
template <typename T> struct IsConcept_f { static bool const V = false; };

// parent-traversing metafunctions

template <typename ParentTypeList>
struct AncestorsOf_Recursive_f;

// "ancestors" of Concept include Concept as a trivial ancestor
template <typename Concept>
struct AncestorsOf_f
{
    enum { _ = Lvd::Meta::Assert<IsConcept_f<Concept>::V>::v };
    typedef TypeList_t<Concept,typename AncestorsOf_Recursive_f<typename Concept::ParentTypeList>::T> T;
};

template <typename ParentTypeList>
struct AncestorsOf_Recursive_f
{
    // depth-first traversal of the ancestor tree
    typedef typename ConcatenationOfTypeLists_t<typename AncestorsOf_f<typename ParentTypeList::HeadType>::T,
                                                typename AncestorsOf_Recursive_f<typename ParentTypeList::BodyTypeList>::T>::T T;
};

template <typename HeadType>
struct AncestorsOf_Recursive_f<TypeList_t<HeadType> >
{
    // depth-first traversal of the ancestor tree
    typedef typename AncestorsOf_f<HeadType>::T T;
};

template <>
struct AncestorsOf_Recursive_f<EmptyTypeList>
{
    // depth-first traversal of the ancestor tree
    typedef EmptyTypeList T;
};

template <typename Concept, typename Predicate>
struct AncestorsSatisfyingPredicate_f
{
    typedef typename ElementsOfTypeListSatisfyingPredicate_t<typename AncestorsOf_f<Concept>::T,Predicate>::T T;
};

// for recursively retrieving various conceptual structures

template <typename Concept, typename ConceptualStructurePredicate>
struct ConceptualStructuresOf_f
{
    enum { _ = Lvd::Meta::Assert<IsConcept_f<Concept>::V>::v }; // TODO: check that ConceptualStructurePredicate actually is one
    typedef typename UniqueTypesIn_t<typename AncestorsSatisfyingPredicate_f<Concept,ConceptualStructurePredicate>::T>::T T;
};

template <typename Concept, typename ConceptualStructurePredicate>
struct HasConceptualStructure_f
{
    static bool const V = ConceptualStructuresOf_f<Concept,ConceptualStructurePredicate>::T::LENGTH > 0;
};

template <typename Concept, typename ConceptualStructurePredicate>
struct HasUniqueConceptualStructure_f
{
    static bool const V = ConceptualStructuresOf_f<Concept,ConceptualStructurePredicate>::T::LENGTH == 1;
};

template <typename Concept, typename ConceptualStructurePredicate>
struct UniqueConceptualStructureOf_f
{
    enum { _ = Lvd::Meta::Assert<HasUniqueConceptualStructure_f<Concept,ConceptualStructurePredicate>::V>::v };
    typedef typename ConceptualStructuresOf_f<Concept,ConceptualStructurePredicate>::T::HeadType T;
};

// easy way to provide Concept-specific structure metafunctions
#define DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ConceptName) \
struct Is##ConceptName##_p \
{ \
    template <typename T> \
    struct Eval_t \
    { \
        static bool const V = Is##ConceptName##_f<T>::V; \
    }; \
}; \
\
template <typename Concept> struct ConceptName##StructuresOf_f { typedef typename ConceptualStructuresOf_f<Concept,Is##ConceptName##_p>::T T; }; \
template <typename Concept> struct Has##ConceptName##Structure_f { static bool const V = HasConceptualStructure_f<Concept,Is##ConceptName##_p>::V; }; \
template <typename Concept> struct HasUnique##ConceptName##Structure_f { static bool const V = HasUniqueConceptualStructure_f<Concept,Is##ConceptName##_p>::V; }; \
template <typename Concept> struct Unique##ConceptName##StructureOf_f \
{ \
    enum { STATIC_ASSERT_IN_ENUM(HasUnique##ConceptName##Structure_f<Concept>::V, MUST_HAVE_UNIQUE_CONCEPTUAL_STRUCTURE) }; \
    typedef typename UniqueConceptualStructureOf_f<Concept,Is##ConceptName##_p>::T T; \
}

// ///////////////////////////////////////////////////////////////////////////
// conceptual property accessor machinery
// ///////////////////////////////////////////////////////////////////////////

// TODO: consolidate this with Lvd::Meta::Value and Ted's int-carrying value
template <typename T_, T_ V_>
struct TypedValue_t
{
    typedef T_ T;
    static T_ const V = V_;

    static std::string type_as_string () { return "TypedValue_t<" + TypeStringOf_t<T_>::eval() + ',' + AS_STRING(V_) + '>'; }
};

// sentinel "value" type
struct NullValue { static std::string type_as_string () { return "NullValue"; } };

// default definition of the given property of exactly the given concept (but
// NOT its ancestors).  particular concepts should specialize this to provide
// definitions.  if the property is a value (e.g. Uint32), then the type should
// be TypedValue_t<...>
template <typename Concept_, typename PropertyId_>
struct BaseProperty_f
{
    typedef NullValue T;
};

template <typename TypeList_, typename PropertyId_>
struct PropertyOfEachInTypeList_f
{
    typedef TypeList_t<typename BaseProperty_f<typename TypeList_::HeadType,PropertyId_>::T,
                       typename PropertyOfEachInTypeList_f<typename TypeList_::BodyTypeList,PropertyId_>::T> T;
};

template <typename PropertyId_>
struct PropertyOfEachInTypeList_f<EmptyTypeList,PropertyId_>
{
    typedef EmptyTypeList T;
};

// gives a list of the unique values of the given property, taken from all ancestors.
template <typename Concept_, typename PropertyId_>
struct MultiProperty_f
{
private:
    typedef typename PropertyOfEachInTypeList_f<typename AncestorsOf_f<Concept_>::T,PropertyId_>::T PropertyOfEach;
    typedef typename UniqueTypesIn_t<PropertyOfEach>::T UniquePropertyTypeList;
public:
    typedef typename SetSubtraction_t<UniquePropertyTypeList,TypeList_t<NullValue> >::T T;
};

// if MultiProperty_f has exactly one element, returns that.
template <typename Concept_, typename PropertyId_>
struct Property_f
{
private:
    typedef typename MultiProperty_f<Concept_,PropertyId_>::T MultiProperty;
    enum { STATIC_ASSERT_IN_ENUM(MultiProperty::LENGTH == 1, PROPERTY_IS_NOT_WELL_DEFINED) };
public:
    typedef typename MultiProperty::HeadType T;
};

// if Property_f is a TypedValue_t, returns the value.
template <typename Concept_, typename PropertyId_>
struct PropertyValue_f
{
private:
    typedef typename Property_f<Concept_,PropertyId_>::T ValueType;
public:
    static typename ValueType::T const V = ValueType::V;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_CONCEPT_HPP_