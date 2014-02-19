#include <iostream>
#include <string>

#include "tenh/core.hpp"

#include "tenh/meta/typle.hpp"
#include "tenh/meta/typle_utility.hpp"
#include "tenh/meta/typestringof.hpp"

using namespace std;
using namespace Tenh;


namespace Tenh {

template <typename T> struct IsMap_t { static bool const V = false; };

// a map having empty domain -- evaluating this map will produce a compile error
struct EmptyMap
{
    typedef Typle_t<> DomainElementTyple;
    typedef Typle_t<> CodomainElementTyple;

    static std::string type_as_string () { return "EmptyMap"; }
};

// recursive definition of Map_t
template <typename DomainElement, typename CodomainElement, typename RestOfMap = EmptyMap>
struct Map_t
{
private:
    enum { _ = Assert<IsMap_t<RestOfMap>::V>::V };

    typedef Typle_t<DomainElement,CodomainElement> DomainCodomainElementPair;
public:
    typedef typename HeadBodyTyple_f<DomainCodomainElementPair,
                                            typename RestOfMap::DomainCodomainElementPairTyple>::T DomainCodomainElementPairTyple;
private:
    typedef typename Unzip_f<DomainCodomainElementPairTyple>::T UnzippedDomainCodomainElementPairTyple;
public:
    typedef typename Element_f<UnzippedDomainCodomainElementPairTyple,0>::T DomainElementTyple;
    typedef typename Element_f<UnzippedDomainCodomainElementPairTyple,1>::T CodomainElementTyple;
    // make sure that DomainElementTyple contains no duplicates (necessary for the map to be well-defined)
    enum { __ = Assert<(Length_f<typename ElementsHavingMultiplicity_f<DomainElementTyple,1>::T>::V == Length_f<DomainElementTyple>::V)>::V };

    static std::string type_as_string () { return "Map_t( " + domain_codomain_element_pairs_as_string() + " )"; }
    static std::string domain_codomain_element_pairs_as_string ()
    {
        std::string retval('(' + type_string_of<DomainElement>() + " |-> " + type_string_of<CodomainElement>() + ')');
        if (Length_f<typename RestOfMap::DomainCodomainElementPairTyple>::V > 0)
            retval += ", " + RestOfMap::domain_codomain_element_pairs_as_string();
        return retval;
    }
};

// one of the base cases
template <typename DomainElement, typename CodomainElement>
struct Map_t<DomainElement,CodomainElement,EmptyMap>
{
    typedef Typle_t<DomainElement,CodomainElement> DomainCodomainElementPair;
    typedef Typle_t<DomainCodomainElementPair> DomainCodomainElementPairTyple;
    typedef Typle_t<DomainElement> DomainElementTyple;
    typedef Typle_t<CodomainElement> CodomainElementTyple;

    static std::string type_as_string () { return "Map_t( " + domain_codomain_element_pairs_as_string() + " )"; }
    static std::string domain_codomain_element_pairs_as_string ()
    {
        return '(' + type_string_of<DomainElement>() + " |-> " + type_string_of<CodomainElement>() + ')';
    }
};

template <typename DomainElement, typename CodomainElement, typename RestOfMap> struct IsMap_t<Map_t<DomainElement,CodomainElement,RestOfMap>> { static bool const V = true; };
template <> struct IsMap_t<EmptyMap> { static bool const V = true; };

template <typename Map, typename InputDomainElement>
struct EvalMap_t
{
private:
    enum { _ = Assert<IsMap_t<Map>::V>::V && Assert<(Length_f<typename Map::DomainElementTyple>::V > 0)>::V };
    static Uint32 const INDEX_OF_INPUT_DOMAIN_ELEMENT = IndexOfFirstOccurrence_f<typename Map::DomainElementTyple,InputDomainElement>::V;
public:
    typedef typename Element_f<typename Map::CodomainElementTyple,INDEX_OF_INPUT_DOMAIN_ELEMENT>::T T;
};


// construct a Map_t from an ordered domain element typle and a
// corresponding ordered codomain element typle.
template <typename DomainElementTyple, typename CodomainElementTyple, typename RestOfMap = EmptyMap>
struct MapConstructor_f
{
    enum { _ = Assert<(Length_f<DomainElementTyple>::V == Length_f<CodomainElementTyple>::V)>::V };
    typedef Map_t<typename Head_f<DomainElementTyple>::T,
                  typename Head_f<CodomainElementTyple>::T,
                  typename MapConstructor_f<typename BodyTyple_f<DomainElementTyple>::T,
                                            typename BodyTyple_f<CodomainElementTyple>::T,
                                            RestOfMap>::T> T;
};

template <typename RestOfMap>
struct MapConstructor_f<Typle_t<>,Typle_t<>,RestOfMap>
{
    typedef RestOfMap T;
};



template <typename Map, typename CandidateDomainElement>
struct MapDomainContains_t
{
    static bool const V = Contains_f<typename Map::DomainElementTyple,CandidateDomainElement>::V;
};


// creates a new Typle_t with Map applied to each element
template <typename Map, typename Typle>
struct EvalMapOnTyple_f
{
    typedef typename HeadBodyTyple_f<typename EvalMap_t<Map,typename Head_f<Typle>::T>::T,
                                     typename EvalMapOnTyple_f<Map,typename BodyTyple_f<Typle>::T>::T>::T T;
};

template <typename Map>
struct EvalMapOnTyple_f<Map,Typle_t<>>
{
    typedef Typle_t<> T;
};


// induces a new Map_t defined on the union of the given maps' domains.
// maps having intesecting domains must act identically on the intersections.
template <typename MapTyple>
struct MapUnion_f
{
private:
    enum { _ = Assert<IsTyple_f<MapTyple>::V>::V };
    typedef typename Head_f<MapTyple>::T HeadMapType;
    typedef typename MapUnion_f<typename MapTyple::BodyTyple>::T MapUnionOfBodyTyple;
    typedef typename SetIntersection_f<typename HeadMapType::DomainElementTyple,
                                       typename MapUnionOfBodyTyple::DomainElementTyple>::T DomainIntersectionTyple;
    typedef typename EvalMapOnTyple_f<HeadMapType,DomainIntersectionTyple>::T HeadMapEvaluation;
    typedef typename EvalMapOnTyple_f<MapUnionOfBodyTyple,DomainIntersectionTyple>::T MapUnionOfBodyTypleEvaluation;
    // ensure that the maps act identically on the domains' intersection.
    enum { __ = Assert<(TypesAreEqual_f<HeadMapEvaluation,MapUnionOfBodyTypleEvaluation>::V)>::V };

    typedef typename SetSubtraction_f<typename HeadMapType::DomainElementTyple,DomainIntersectionTyple>::T HeadMapTypeOnlyDomain;
    typedef typename EvalMapOnTyple_f<HeadMapType,HeadMapTypeOnlyDomain>::T HeadMapTypeOnlyDomainEvaluation;
public:
    typedef typename MapConstructor_f<HeadMapTypeOnlyDomain,HeadMapTypeOnlyDomainEvaluation,MapUnionOfBodyTyple>::T T;
};

template <typename HeadMapType>
struct MapUnion_f<Typle_t<HeadMapType>>
{
    enum { _ = Assert<IsMap_t<HeadMapType>::V>::V };
    typedef HeadMapType T;
};

template <>
struct MapUnion_f<Typle_t<>>
{
    typedef EmptyMap T;
};

} // end of namespace Tenh




/*
The point of this prototype is to test out a design for "concepts" which implement
a hierarchy of mathematical structures.  For example, a Ring has an additive Group
structure and a multiplicative Monoid structure, and because a Group has a unique
Monoid structure, a Ring has two distinct Monoid structures.

When mathematical structures/relationships are expressed in the form of concepts,
the various structures of a particular mathematical object can be retrieved.  This
is essentially the C++ implementation of the forgetful functors from a specific
category to a more general one (e.g. VectorSpace -> Group).

STILL TO BE DESIGNED: When a concept has more than one forgetful functor of a
particular type (e.g. Field -> Group (additive) and Field -> Group (multiplicative)),
there needs to be a way to specify which one should be used.  Probably by some
labeling scheme on each structure.
*/

// for creating domain types for StructureDisambiguationMap
#define MAKE_ID_STRUCT(Name) struct Name { static std::string type_as_string () { return #Name; } }

#define AS_NAMED_STRUCTURE(Concept,StructureName) EvalMap_t<TotalStructureDisambiguationMapOf_f<Concept>::T,StructureName>::T

// a Concept is a struct that has a ParentTyple (which is a Typle_t)
template <typename T> struct IsConcept_f { static bool const V = false; };

// parent-traversing metafunctions

template <typename ParentTyple>
struct AncestorsOf_Recursive_f;

// "ancestors" of Concept include Concept as a trivial ancestor
template <typename Concept>
struct AncestorsOf_f
{
    enum { _ = Assert<IsConcept_f<Concept>::V>::V };
    typedef typename HeadBodyTyple_f<Concept,typename AncestorsOf_Recursive_f<typename Concept::ParentTyple>::T>::T T;
};

template <typename ParentTyple>
struct AncestorsOf_Recursive_f
{
    // depth-first traversal of the ancestor tree
    typedef typename Concat2Typles_f<typename AncestorsOf_f<typename Head_f<ParentTyple>::T>::T,
                                     typename AncestorsOf_Recursive_f<typename BodyTyple_f<ParentTyple>::T>::T>::T T;
};

template <typename HeadType>
struct AncestorsOf_Recursive_f<Typle_t<HeadType>>
{
    // depth-first traversal of the ancestor tree
    typedef typename AncestorsOf_f<HeadType>::T T;
};

template <>
struct AncestorsOf_Recursive_f<Typle_t<>>
{
    // depth-first traversal of the ancestor tree
    typedef Typle_t<> T;
};

template <typename Concept, typename Predicate>
struct AncestorsSatisfyingPredicate_f
{
    typedef typename ElementsSatisfying_f<typename AncestorsOf_f<Concept>::T,Predicate>::T T;
};


template <typename ParentTyple>
struct StructureDisambiguationMapsOf_Recursive_f;

// "structure disambiguation maps" of Concept include Concept's StructureDisambiguationMap
template <typename Concept>
struct StructureDisambiguationMapsOf_f
{
    enum { _ = Assert<IsConcept_f<Concept>::V>::V };
    typedef typename HeadBodyTyple_f<typename Concept::StructureDisambiguationMap,
                                     typename StructureDisambiguationMapsOf_Recursive_f<typename Concept::ParentTyple>::T>::T T;
};

template <typename ParentTyple>
struct StructureDisambiguationMapsOf_Recursive_f
{
    // depth-first traversal of the ancestor tree
    enum { _ = Assert<IsConcept_f<typename Head_f<ParentTyple>::T>::V>::V };
    typedef typename Concat2Typles_f<typename StructureDisambiguationMapsOf_f<typename Head_f<ParentTyple>::T>::T,
                                     typename StructureDisambiguationMapsOf_Recursive_f<typename BodyTyple_f<ParentTyple>::T>::T>::T T;
};

template <typename HeadType>
struct StructureDisambiguationMapsOf_Recursive_f<Typle_t<HeadType>>
{
    // depth-first traversal of the ancestor tree
    typedef typename StructureDisambiguationMapsOf_f<HeadType>::T T;
};

template <>
struct StructureDisambiguationMapsOf_Recursive_f<Typle_t<>>
{
    // depth-first traversal of the ancestor tree
    typedef Typle_t<> T;
};


template <typename Concept>
struct TotalStructureDisambiguationMapOf_f
{
    typedef typename MapUnion_f<typename StructureDisambiguationMapsOf_f<Concept>::T>::T T;
};



// for recursively retrieving various conceptual structures

template <typename Concept, typename ConceptualStructurePredicate>
struct ConceptualStructuresOf_f
{
    enum { _ = Assert<IsConcept_f<Concept>::V>::V }; // TODO: check that ConceptualStructurePredicate actually is one
    typedef typename UniqueTypesIn_f<typename AncestorsSatisfyingPredicate_f<Concept,ConceptualStructurePredicate>::T>::T T;
};

template <typename Concept, typename ConceptualStructurePredicate>
struct HasConceptualStructure_f
{
    static bool const V = Length_f<typename ConceptualStructuresOf_f<Concept,ConceptualStructurePredicate>::T>::V > 0;
};

template <typename Concept, typename ConceptualStructurePredicate>
struct HasUniqueConceptualStructure_f
{
    static bool const V = Length_f<typename ConceptualStructuresOf_f<Concept,ConceptualStructurePredicate>::T>::V == 1;
};

template <typename Concept, typename ConceptualStructurePredicate>
struct UniqueConceptualStructureOf_f
{
    enum { _ = Assert<HasUniqueConceptualStructure_f<Concept,ConceptualStructurePredicate>::V>::V };
    typedef typename Head_f<typename ConceptualStructuresOf_f<Concept,ConceptualStructurePredicate>::T>::T T;
};

// easy way to provide Concept-specific structure metafunctions
#define DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ConceptName) \
MAKE_1_ARY_VALUE_EVALUATOR(Is##ConceptName); \
template <typename Concept> struct ConceptName##StructuresOf_f { typedef typename ConceptualStructuresOf_f<Concept,Is##ConceptName##_e>::T T; }; \
template <typename Concept> struct Has##ConceptName##Structure_f { static bool const V = HasConceptualStructure_f<Concept,Is##ConceptName##_e>::V; }; \
template <typename Concept> struct HasUnique##ConceptName##Structure_f { static bool const V = HasUniqueConceptualStructure_f<Concept,Is##ConceptName##_e>::V; }; \
template <typename Concept> struct Unique##ConceptName##StructureOf_f { typedef typename UniqueConceptualStructureOf_f<Concept,Is##ConceptName##_e>::T T; }

// Monoid_c

template <typename Identity_, typename Operation_, bool IS_ABELIAN_>
struct Monoid_c
{
    typedef Typle_t<> ParentTyple;
    typedef EmptyMap StructureDisambiguationMap;

    typedef Identity_ Identity;
    typedef Operation_ Operation;
    static bool const IS_ABELIAN = IS_ABELIAN_;

    static std::string type_as_string ()
    {
        return "Monoid_c<" + type_string_of<Identity>() + ','
                           + type_string_of<Operation>() + ','
                           + (IS_ABELIAN ? "IS_ABELIAN" : "IS_NOT_ABELIAN") + '>';
    }
};

template <typename Identity, typename Operation, bool IS_ABELIAN>
struct IsConcept_f<Monoid_c<Identity,Operation,IS_ABELIAN>>
{ static bool const V = true; };

template <typename T> struct IsMonoid_f { static bool const V = false; };

template <typename Identity, typename Operation, bool IS_ABELIAN>
struct IsMonoid_f<Monoid_c<Identity,Operation,IS_ABELIAN>>
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Monoid);
// special convenience macros
#define IS_MONOID_UNIQUELY(Concept) HasUniqueMonoidStructure_f<Concept>::V
#define AS_MONOID(Concept) UniqueMonoidStructureOf_f<Concept>::T

// Group_c

template <typename Identity_, typename Operation_, typename Inversion_, bool IS_ABELIAN_>
struct Group_c
{
private:
    typedef Monoid_c<Identity_,Operation_,IS_ABELIAN_> As_Monoid;
public:
    typedef Typle_t<As_Monoid> ParentTyple;
    typedef EmptyMap StructureDisambiguationMap;

    typedef Identity_ Identity;
    typedef Operation_ Operation;
    typedef Inversion_ Inversion;
    static bool const IS_ABELIAN = IS_ABELIAN_;

    static std::string type_as_string ()
    {
        return "Group_c<" + type_string_of<Identity>() + ','
                          + type_string_of<Operation>() + ','
                          + type_string_of<Inversion>() + ','
                          + (IS_ABELIAN ? "IS_ABELIAN" : "IS_NOT_ABELIAN") + '>';
    }
};

template <typename Identity, typename Operation, typename Inversion, bool IS_ABELIAN>
struct IsConcept_f<Group_c<Identity,Operation,Inversion,IS_ABELIAN>>
{ static bool const V = true; };

template <typename T> struct IsGroup_f { static bool const V = false; };

template <typename Identity, typename Operation, typename Inversion, bool IS_ABELIAN>
struct IsGroup_f<Group_c<Identity,Operation,Inversion,IS_ABELIAN>>
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Group);
// special convenience macros
#define IS_GROUP_UNIQUELY(Concept) HasUniqueGroupStructure_f<Concept>::V
#define AS_GROUP(Concept) UniqueGroupStructureOf_f<Concept>::T

// Ring_c

MAKE_ID_STRUCT(AdditiveMonoid);
MAKE_ID_STRUCT(MultiplicativeMonoid);

template <typename AdditiveIdentity_,
          typename Addition_,
          typename AdditiveInversion_,
          typename MultiplicativeIdentity_,
          typename Multiplication_,
          bool IS_COMMUTATIVE_>
struct Ring_c
{
private:
    typedef Group_c<AdditiveIdentity_,Addition_,AdditiveInversion_,true> As_AdditiveGroup;
    typedef Monoid_c<MultiplicativeIdentity_,Multiplication_,IS_COMMUTATIVE_> As_MultiplicativeMonoid;
public:
    typedef Typle_t<As_AdditiveGroup,As_MultiplicativeMonoid> ParentTyple;
    typedef Map_t<AdditiveMonoid,typename AS_MONOID(As_AdditiveGroup),
            Map_t<MultiplicativeMonoid,As_MultiplicativeMonoid>> StructureDisambiguationMap;

    typedef AdditiveIdentity_ AdditiveIdentity;
    typedef Addition_ Addition;
    typedef AdditiveInversion_ AdditiveInversion;
    typedef MultiplicativeIdentity_ MultiplicativeIdentity;
    typedef Multiplication_ Multiplication;
    static bool const IS_COMMUTATIVE = IS_COMMUTATIVE_;

    static std::string type_as_string ()
    {
        return "Ring_c<" + type_string_of<AdditiveIdentity>() + ','
                         + type_string_of<Addition>() + ','
                         + type_string_of<AdditiveInversion>() + ','
                         + type_string_of<MultiplicativeIdentity>() + ','
                         + type_string_of<Multiplication>() + ','
                         + (IS_COMMUTATIVE ? "IS_COMMUTATIVE" : "IS_NOT_COMMUTATIVE") + '>';
    }
};

template <typename AdditiveIdentity,
          typename Addition,
          typename AdditiveInversion,
          typename MultiplicativeIdentity,
          typename Multiplication,
          bool IS_COMMUTATIVE>
struct IsConcept_f<Ring_c<AdditiveIdentity,Addition,AdditiveInversion,MultiplicativeIdentity,Multiplication,IS_COMMUTATIVE>>
{ static bool const V = true; };

template <typename T> struct IsRing_f { static bool const V = false; };

template <typename AdditiveIdentity,
          typename Addition,
          typename AdditiveInversion,
          typename MultiplicativeIdentity,
          typename Multiplication,
          bool IS_COMMUTATIVE>
struct IsRing_f<Ring_c<AdditiveIdentity,Addition,AdditiveInversion,MultiplicativeIdentity,Multiplication,IS_COMMUTATIVE>>
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Ring);
// special convenience macros
#define IS_RING_UNIQUELY(Concept) HasUniqueRingStructure_f<Concept>::V
#define AS_RING(Concept) UniqueRingStructureOf_f<Concept>::T

// Field_c

MAKE_ID_STRUCT(AdditiveGroup);
MAKE_ID_STRUCT(MultiplicativeGroup);

template <typename AdditiveIdentity_,
          typename Addition_,
          typename AdditiveInversion_,
          typename MultiplicativeIdentity_,
          typename Multiplication_,
          typename MultiplicativeInverse_>
struct Field_c
{
private:
    typedef Ring_c<AdditiveIdentity_,Addition_,AdditiveInversion_,MultiplicativeIdentity_,Multiplication_,true> As_Ring;
    typedef Group_c<MultiplicativeIdentity_,Multiplication_,MultiplicativeInverse_,true> As_MultiplicativeGroup;
public:
    typedef Typle_t<As_Ring,As_MultiplicativeGroup> ParentTyple;
    typedef Map_t<AdditiveGroup,typename AS_GROUP(As_Ring),
            Map_t<MultiplicativeGroup,As_MultiplicativeGroup>> StructureDisambiguationMap;

    typedef AdditiveIdentity_ AdditiveIdentity;
    typedef Addition_ Addition;
    typedef AdditiveInversion_ AdditiveInversion;
    typedef MultiplicativeIdentity_ MultiplicativeIdentity;
    typedef Multiplication_ Multiplication;
    typedef MultiplicativeInverse_ MultiplicativeInverse;

    static std::string type_as_string ()
    {
        return "Field_c<" + type_string_of<AdditiveIdentity>() + ','
                          + type_string_of<Addition>() + ','
                          + type_string_of<AdditiveInversion>() + ','
                          + type_string_of<MultiplicativeIdentity>() + ','
                          + type_string_of<Multiplication>() + ','
                          + type_string_of<MultiplicativeInverse>() + '>';
    }
};

template <typename AdditiveIdentity,
          typename Addition,
          typename AdditiveInversion,
          typename MultiplicativeIdentity,
          typename Multiplication,
          typename MultiplicativeInverse>
struct IsConcept_f<Field_c<AdditiveIdentity,Addition,AdditiveInversion,MultiplicativeIdentity,Multiplication,MultiplicativeInverse>>
{ static bool const V = true; };

template <typename T> struct IsField_f { static bool const V = false; };

template <typename AdditiveIdentity,
          typename Addition,
          typename AdditiveInversion,
          typename MultiplicativeIdentity,
          typename Multiplication,
          typename MultiplicativeInverse>
struct IsField_f<Field_c<AdditiveIdentity,Addition,AdditiveInversion,MultiplicativeIdentity,Multiplication,MultiplicativeInverse>>
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Field);
// special convenience macros
#define IS_FIELD_UNIQUELY(Concept) HasUniqueFieldStructure_f<Concept>::V
#define AS_FIELD(Concept) UniqueFieldStructureOf_f<Concept>::T

// main function section

MAKE_ID_STRUCT(Zero);
MAKE_ID_STRUCT(ZeroMatrix);
MAKE_ID_STRUCT(Add);
MAKE_ID_STRUCT(Negate);
MAKE_ID_STRUCT(One);
MAKE_ID_STRUCT(IdentityMatrix);
MAKE_ID_STRUCT(Multiply);
MAKE_ID_STRUCT(Invert);

int main (int argc, char **argv)
{
    typedef Monoid_c<IdentityMatrix,Multiply,false> Monoid;
    typedef Group_c<ZeroMatrix,Add,Negate,true> Group;
    typedef Ring_c<ZeroMatrix,Add,Negate,IdentityMatrix,Multiply,false> Ring;
    typedef Field_c<Zero,Add,Negate,One,Multiply,Invert> Field;

    cout << FORMAT_VALUE(type_string_of<Monoid>()) << '\n';
    cout << FORMAT_VALUE(type_string_of<Group>()) << '\n';
    cout << FORMAT_VALUE(type_string_of<Ring>()) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(IsMonoid_f<Monoid>::V) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(IsMonoid_f<Group>::V) << '\n';
    cout << FORMAT_VALUE(IsGroup_f<Group>::V) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(IsMonoid_f<Ring>::V) << '\n';
    cout << FORMAT_VALUE(IsGroup_f<Ring>::V) << '\n';
    cout << FORMAT_VALUE(IsRing_f<Ring>::V) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(IsMonoid_f<Field>::V) << '\n';
    cout << FORMAT_VALUE(IsGroup_f<Field>::V) << '\n';
    cout << FORMAT_VALUE(IsRing_f<Field>::V) << '\n';
    cout << FORMAT_VALUE(IsField_f<Field>::V) << '\n';
    cout << '\n';

    typedef AncestorsOf_f<Monoid>::T AncestorsOfMonoid;
    typedef AncestorsOf_f<Group>::T AncestorsOfGroup;
    typedef AncestorsOf_f<Ring>::T AncestorsOfRing;
    cout << FORMAT_VALUE(type_string_of<AncestorsOfMonoid>()) << '\n';
    cout << FORMAT_VALUE(type_string_of<AncestorsOfGroup>()) << '\n';
    cout << FORMAT_VALUE(type_string_of<AncestorsOfRing>()) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((type_string_of<AncestorsSatisfyingPredicate_f<Monoid,IsMonoid_e>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<AncestorsSatisfyingPredicate_f<Group,IsMonoid_e>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<AncestorsSatisfyingPredicate_f<Ring,IsMonoid_e>::T>())) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((type_string_of<ConceptualStructuresOf_f<Monoid,IsMonoid_e>::T>())) << '\n';
    cout << FORMAT_VALUE((HasConceptualStructure_f<Monoid,IsMonoid_e>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueConceptualStructure_f<Monoid,IsMonoid_e>::V)) << '\n';
    cout << FORMAT_VALUE((type_string_of<UniqueConceptualStructureOf_f<Monoid,IsMonoid_e>::T>())) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((type_string_of<MonoidStructuresOf_f<Monoid>::T>())) << '\n';
    cout << FORMAT_VALUE((HasMonoidStructure_f<Monoid>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueMonoidStructure_f<Monoid>::V)) << '\n';
    cout << FORMAT_VALUE((type_string_of<UniqueMonoidStructureOf_f<Monoid>::T>())) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(IS_MONOID_UNIQUELY(Monoid)) << '\n';
    cout << FORMAT_VALUE(type_string_of<AS_MONOID(Monoid)>()) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((type_string_of<ConceptualStructuresOf_f<Monoid,IsGroup_e>::T>())) << '\n';
    cout << FORMAT_VALUE((HasConceptualStructure_f<Monoid,IsGroup_e>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueConceptualStructure_f<Monoid,IsGroup_e>::V)) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((type_string_of<ConceptualStructuresOf_f<Group,IsMonoid_e>::T>())) << '\n';
    cout << FORMAT_VALUE((HasConceptualStructure_f<Group,IsMonoid_e>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueConceptualStructure_f<Group,IsMonoid_e>::V)) << '\n';
    cout << FORMAT_VALUE((type_string_of<UniqueConceptualStructureOf_f<Group,IsMonoid_e>::T>())) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(IS_MONOID_UNIQUELY(Group)) << '\n';
    cout << FORMAT_VALUE(type_string_of<AS_MONOID(Group)>()) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((type_string_of<ConceptualStructuresOf_f<Ring,IsMonoid_e>::T>())) << '\n';
    cout << FORMAT_VALUE((HasConceptualStructure_f<Ring,IsMonoid_e>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueConceptualStructure_f<Ring,IsMonoid_e>::V)) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((type_string_of<ConceptualStructuresOf_f<Field,IsGroup_e>::T>())) << '\n';
    cout << FORMAT_VALUE((HasConceptualStructure_f<Field,IsGroup_e>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueConceptualStructure_f<Field,IsGroup_e>::V)) << '\n';
    cout << '\n';

    // some test usage to get various properties of conceptual structures
    {
        cout << FORMAT_VALUE(type_string_of<UniqueGroupStructureOf_f<Ring>::T>()) << '\n';
        cout << FORMAT_VALUE(type_string_of<AS_GROUP(Ring)>()) << '\n';
        cout << FORMAT_VALUE(type_string_of<UniqueGroupStructureOf_f<Ring>::T::Identity>()) << '\n';
        cout << FORMAT_VALUE(type_string_of<AS_GROUP(Ring)::Identity>()) << '\n';
        // typedef UniqueGroupStructureOf_f<Ring>::T::Identity MultiplicativeIdentity;
        typedef AS_GROUP(Ring)::Identity MultiplicativeIdentity;
        cout << FORMAT_VALUE(type_string_of<MultiplicativeIdentity>()) << '\n';
        cout << '\n';
    }

    {
        cout << "testing StructureDisambiguationMapsOf_f\n";
        cout << FORMAT_VALUE(type_string_of<StructureDisambiguationMapsOf_f<Monoid>::T>()) << '\n';
        cout << FORMAT_VALUE(type_string_of<StructureDisambiguationMapsOf_f<Group>::T>()) << '\n';
        cout << FORMAT_VALUE(type_string_of<StructureDisambiguationMapsOf_f<Ring>::T>()) << '\n';
        cout << FORMAT_VALUE(type_string_of<StructureDisambiguationMapsOf_f<Field>::T>()) << '\n';
        cout << '\n';
    }

    {
        cout << "testing TotalStructureDisambiguationMapOf_f\n";
        cout << FORMAT_VALUE(type_string_of<TotalStructureDisambiguationMapOf_f<Monoid>::T>()) << '\n';
        cout << FORMAT_VALUE(type_string_of<TotalStructureDisambiguationMapOf_f<Group>::T>()) << '\n';
        cout << FORMAT_VALUE(type_string_of<TotalStructureDisambiguationMapOf_f<Ring>::T>()) << '\n';
        cout << FORMAT_VALUE(type_string_of<TotalStructureDisambiguationMapOf_f<Field>::T>()) << '\n';
        cout << '\n';
    }

    // testing Map_t
    {
        {
            cout << FORMAT_VALUE(type_string_of<EmptyMap>()) << '\n';
            // typedef EvalMap_t<EmptyMap,int>::T ThisShouldCauseACompileError;
        }

        {
            typedef Map_t<int,float> SimpleMap;
            cout << FORMAT_VALUE(type_string_of<SimpleMap>()) << '\n';

            typedef EvalMap_t<SimpleMap,int>::T ShouldBe_float;
            cout << FORMAT_VALUE(type_string_of<ShouldBe_float>()) << '\n';
            Assert<(TypesAreEqual_f<ShouldBe_float,float>::V)>();
            // typedef EvalMap_t<SimpleMap,bool>::T ThisShouldCauseACompileError;

            cout << FORMAT_VALUE((MapDomainContains_t<SimpleMap,int>::V)) << '\n';
            cout << FORMAT_VALUE((MapDomainContains_t<SimpleMap,int>::V)) << '\n';
        }

        {
            typedef Map_t<int,float,Map_t<char,double>> TwoElementMap;
            cout << FORMAT_VALUE(type_string_of<TwoElementMap>()) << '\n';

            typedef EvalMap_t<TwoElementMap,int>::T ShouldBe_float;
            typedef EvalMap_t<TwoElementMap,char>::T ShouldBe_double;
            cout << FORMAT_VALUE(type_string_of<ShouldBe_float>()) << '\n';
            cout << FORMAT_VALUE(type_string_of<ShouldBe_double>()) << '\n';
            Assert<(TypesAreEqual_f<ShouldBe_float,float>::V)>();
            Assert<(TypesAreEqual_f<ShouldBe_double,double>::V)>();
            // typedef EvalMap_t<TwoElementMap,bool>::T ThisShouldCauseACompileError;
            cout << '\n';
        }

        {
            cout << "testing MapConstructor_f\n";
            typedef Map_t<int,float> Map;
            cout << FORMAT_VALUE(type_string_of<Map>()) << '\n';
            cout << FORMAT_VALUE((type_string_of<MapConstructor_f<Typle_t<>,Typle_t<>,EmptyMap>::T>())) << '\n';
            cout << FORMAT_VALUE((type_string_of<MapConstructor_f<Typle_t<>,Typle_t<>,Map>::T>())) << '\n';

            typedef Typle_t<char,bool> Domain;
            typedef Typle_t<float,double> Codomain;
            cout << FORMAT_VALUE(type_string_of<Domain>()) << '\n';
            cout << FORMAT_VALUE(type_string_of<Codomain>()) << '\n';
            cout << FORMAT_VALUE((type_string_of<MapConstructor_f<Domain,Codomain,EmptyMap>::T>())) << '\n';
            cout << FORMAT_VALUE((type_string_of<MapConstructor_f<Domain,Codomain,Map>::T>())) << '\n';
            cout << '\n';
        }

        {
            cout << "testing MapUnion_f\n";
            typedef MapUnion_f<Typle_t<>>::T M;
            cout << FORMAT_VALUE(type_string_of<M>()) << '\n';
            typedef Map_t<int,float,Map_t<bool,char>> Map1;
            typedef Map_t<int,float,Map_t<char,double>> Map2;
//             typedef Map_t<int,double,Map_t<char,double>> Map3;
            cout << FORMAT_VALUE(type_string_of<Map1>()) << '\n';
            cout << FORMAT_VALUE(type_string_of<Map2>()) << '\n';
            cout << FORMAT_VALUE(type_string_of<MapUnion_f<Typle_t<Map1>>::T>()) << '\n';
            cout << FORMAT_VALUE((type_string_of<MapUnion_f<Typle_t<Map1,Map2>>::T>())) << '\n';
            // typedef MapUnion_f<Typle_t<Map1,Map3>>::T ThisShouldCauseACompileError;
            cout << '\n';
        }

        {
            cout << "testing AS_NAMED_STRUCTURE\n";
            cout << FORMAT_VALUE(type_string_of<AS_NAMED_STRUCTURE(Ring,AdditiveMonoid)>()) << '\n';
            cout << FORMAT_VALUE(type_string_of<AS_NAMED_STRUCTURE(Ring,MultiplicativeMonoid)>()) << '\n';
            cout << '\n';

            cout << FORMAT_VALUE(type_string_of<AS_NAMED_STRUCTURE(Field,AdditiveMonoid)>()) << '\n';
            cout << FORMAT_VALUE(type_string_of<AS_NAMED_STRUCTURE(Field,MultiplicativeMonoid)>()) << '\n';
            cout << FORMAT_VALUE(type_string_of<AS_NAMED_STRUCTURE(Field,AdditiveGroup)>()) << '\n';
            cout << FORMAT_VALUE(type_string_of<AS_NAMED_STRUCTURE(Field,MultiplicativeGroup)>()) << '\n';
            cout << '\n';
        }
    }

    return 0;
}
