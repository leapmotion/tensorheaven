#include <iostream>
#include <string>

#include "tenh/core.hpp"

#include "tenh/meta/typelist.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/meta/typestringof.hpp"

using namespace std;
using namespace Tenh;




namespace Tenh {

template <typename T> struct IsMap_t { static bool const V = false; };

// a map having empty domain -- evaluating this map will produce a compile error
struct EmptyMap
{
    typedef EmptyTypeList DomainElementTypeList;
    typedef EmptyTypeList CodomainElementTypeList;

    static std::string type_as_string () { return "EmptyMap"; }
};

// recursive definition of Map_t
template <typename DomainElement, typename CodomainElement, typename RestOfMap = EmptyMap>
struct Map_t
{
private:
    enum { _ = Lvd::Meta::Assert<IsMap_t<RestOfMap>::V>::v };

    typedef TypeList_t<DomainElement,TypeList_t<CodomainElement> > DomainCodomainElementPair;
public:
    typedef TypeList_t<DomainCodomainElementPair,
                       typename RestOfMap::DomainCodomainElementPairTypeList> DomainCodomainElementPairTypeList;
private:
    typedef typename Unzip_t<DomainCodomainElementPairTypeList>::T UnzippedDomainCodomainElementPairTypeList;
public:
    typedef typename UnzippedDomainCodomainElementPairTypeList::HeadType DomainElementTypeList;
    typedef typename UnzippedDomainCodomainElementPairTypeList::BodyTypeList::HeadType CodomainElementTypeList;
    // make sure that DomainElementTypeList contains no duplicates (necessary for the map to be well-defined)
    enum { __ = Lvd::Meta::Assert<(ElementsHavingMultiplicity_t<DomainElementTypeList,1>::T::LENGTH == DomainElementTypeList::LENGTH)>::v };

    static std::string type_as_string () { return "Map_t( " + domain_codomain_element_pairs_as_string() + " )"; }
    static std::string domain_codomain_element_pairs_as_string ()
    {
        std::string retval('(' + TypeStringOf_t<DomainElement>::eval() + " |-> " + TypeStringOf_t<CodomainElement>::eval() + ')');
        if (RestOfMap::DomainCodomainElementPairTypeList::LENGTH > 0)
            retval += ", " + RestOfMap::domain_codomain_element_pairs_as_string();
        return retval;
    }
};

// one of the base cases
template <typename DomainElement, typename CodomainElement>
struct Map_t<DomainElement,CodomainElement,EmptyMap>
{
    typedef TypeList_t<DomainElement,TypeList_t<CodomainElement> > DomainCodomainElementPair;
    typedef TypeList_t<DomainCodomainElementPair> DomainCodomainElementPairTypeList;
    typedef TypeList_t<DomainElement> DomainElementTypeList;
    typedef TypeList_t<CodomainElement> CodomainElementTypeList;

    static std::string type_as_string () { return "Map_t( " + domain_codomain_element_pairs_as_string() + " )"; }
    static std::string domain_codomain_element_pairs_as_string ()
    {
        return '(' + TypeStringOf_t<DomainElement>::eval() + " |-> " + TypeStringOf_t<CodomainElement>::eval() + ')';
    }
};

template <typename DomainElement, typename CodomainElement, typename RestOfMap> struct IsMap_t<Map_t<DomainElement,CodomainElement,RestOfMap> > { static bool const V = true; };
template <> struct IsMap_t<EmptyMap> { static bool const V = true; };

template <typename Map, typename InputDomainElement>
struct EvalMap_t
{
private:
    enum { _ = Lvd::Meta::Assert<IsMap_t<Map>::V>::v && Lvd::Meta::Assert<(Map::DomainElementTypeList::LENGTH > 0)>::v };
    static Uint32 const INDEX_OF_INPUT_DOMAIN_ELEMENT = FirstMatchingIn_t<typename Map::DomainElementTypeList,InputDomainElement>::INDEX;
public:
    typedef typename Map::CodomainElementTypeList::template El_t<INDEX_OF_INPUT_DOMAIN_ELEMENT>::T T;
};


// construct a Map_t from an ordered domain element type list and a
// corresponding ordered codomain element type list.
template <typename DomainElementTypeList, typename CodomainElementTypeList, typename RestOfMap = EmptyMap>
struct MapConstructor_t
{
    enum { _ = Lvd::Meta::Assert<(DomainElementTypeList::LENGTH == CodomainElementTypeList::LENGTH)>::v };
    typedef Map_t<typename DomainElementTypeList::HeadType,
                  typename CodomainElementTypeList::HeadType,
                  typename MapConstructor_t<typename DomainElementTypeList::BodyTypeList,
                                            typename CodomainElementTypeList::BodyTypeList,
                                            RestOfMap>::T> T;
};

template <typename RestOfMap>
struct MapConstructor_t<EmptyTypeList,EmptyTypeList,RestOfMap>
{
    typedef RestOfMap T;
};



template <typename Map, typename CandidateDomainElement>
struct MapDomainContains_t
{
    static bool const V = Map::DomainElementTypeList::template Contains_t<CandidateDomainElement>::V;
};


// creates a new TypeList_t with Map applied to each element
template <typename Map, typename TypeList>
struct EvalMapOnTypeList_t
{
    typedef TypeList_t<typename EvalMap_t<Map,typename TypeList::HeadType>::T,
                       typename EvalMapOnTypeList_t<Map,typename TypeList::BodyTypeList>::T> T;
};

template <typename Map>
struct EvalMapOnTypeList_t<Map,EmptyTypeList>
{
    typedef EmptyTypeList T;
};


// induces a new Map_t defined on the union of the given maps' domains.
// maps having intesecting domains must act identically on the intersections.
template <typename MapTypeList>
struct MapUnion_t
{
private:
    enum { _ = Lvd::Meta::Assert<IsATypeList_t<MapTypeList>::V>::v };
    typedef typename MapTypeList::HeadType HeadMapType;
    typedef typename MapUnion_t<typename MapTypeList::BodyTypeList>::T MapUnionOfBodyTypeList;
    typedef typename IntersectionAsSets_t<typename HeadMapType::DomainElementTypeList,
                                          typename MapUnionOfBodyTypeList::DomainElementTypeList>::T DomainIntersectionTypeList;
    typedef typename EvalMapOnTypeList_t<HeadMapType,DomainIntersectionTypeList>::T HeadMapEvaluation;
    typedef typename EvalMapOnTypeList_t<MapUnionOfBodyTypeList,DomainIntersectionTypeList>::T MapUnionOfBodyTypeListEvaluation;
    // ensure that the maps act identically on the domains' intersection.
    enum { __ = Lvd::Meta::Assert<(Lvd::Meta::TypesAreEqual<HeadMapEvaluation,MapUnionOfBodyTypeListEvaluation>::v)>::v };

    typedef typename SetSubtraction_t<typename HeadMapType::DomainElementTypeList,DomainIntersectionTypeList>::T HeadMapTypeOnlyDomain;
    typedef typename EvalMapOnTypeList_t<HeadMapType,HeadMapTypeOnlyDomain>::T HeadMapTypeOnlyDomainEvaluation;
public:
    typedef typename MapConstructor_t<HeadMapTypeOnlyDomain,HeadMapTypeOnlyDomainEvaluation,MapUnionOfBodyTypeList>::T T;
};

template <typename HeadMapType>
struct MapUnion_t<TypeList_t<HeadMapType> >
{
    enum { _ = Lvd::Meta::Assert<IsMap_t<HeadMapType>::V>::v };
    typedef HeadMapType T;
};

template <>
struct MapUnion_t<EmptyTypeList>
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
template <typename Concept> struct Unique##ConceptName##StructureOf_f { typedef typename UniqueConceptualStructureOf_f<Concept,Is##ConceptName##_p>::T T; }

// Monoid_c

template <typename Identity_, typename Operation_, bool IS_ABELIAN_>
struct Monoid_c
{
    typedef EmptyTypeList ParentTypeList;
    typedef EmptyTypeList DisambiguationMap;

    typedef Identity_ Identity;
    typedef Operation_ Operation;
    static bool const IS_ABELIAN = IS_ABELIAN_;

    static std::string type_as_string ()
    {
        return "Monoid_c<" + TypeStringOf_t<Identity>::eval() + ','
                           + TypeStringOf_t<Operation>::eval() + ','
                           + (IS_ABELIAN ? "IS_ABELIAN" : "IS_NOT_ABELIAN") + '>';
    }
};

template <typename Identity, typename Operation, bool IS_ABELIAN> 
struct IsConcept_f<Monoid_c<Identity,Operation,IS_ABELIAN> >
{ static bool const V = true; };

template <typename T> struct IsMonoid_f { static bool const V = false; };

template <typename Identity, typename Operation, bool IS_ABELIAN> 
struct IsMonoid_f<Monoid_c<Identity,Operation,IS_ABELIAN> >
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
    typedef TypeList_t<As_Monoid> ParentTypeList;

    typedef Identity_ Identity;
    typedef Operation_ Operation;
    typedef Inversion_ Inversion;
    static bool const IS_ABELIAN = IS_ABELIAN_;

    static std::string type_as_string ()
    {
        return "Group_c<" + TypeStringOf_t<Identity>::eval() + ','
                          + TypeStringOf_t<Operation>::eval() + ','
                          + TypeStringOf_t<Inversion>::eval() + ','
                          + (IS_ABELIAN ? "IS_ABELIAN" : "IS_NOT_ABELIAN") + '>';
    }
};

template <typename Identity, typename Operation, typename Inversion, bool IS_ABELIAN> 
struct IsConcept_f<Group_c<Identity,Operation,Inversion,IS_ABELIAN> >
{ static bool const V = true; };

template <typename T> struct IsGroup_f { static bool const V = false; };

template <typename Identity, typename Operation, typename Inversion, bool IS_ABELIAN> 
struct IsGroup_f<Group_c<Identity,Operation,Inversion,IS_ABELIAN> >
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Group);
// special convenience macros
#define IS_GROUP_UNIQUELY(Concept) HasUniqueGroupStructure_f<Concept>::V
#define AS_GROUP(Concept) UniqueGroupStructureOf_f<Concept>::T

// Ring_c

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
    typedef TypeList_t<As_AdditiveGroup,TypeList_t<As_MultiplicativeMonoid> > ParentTypeList;

    typedef AdditiveIdentity_ AdditiveIdentity;
    typedef Addition_ Addition;
    typedef AdditiveInversion_ AdditiveInversion; 
    typedef MultiplicativeIdentity_ MultiplicativeIdentity;
    typedef Multiplication_ Multiplication;
    static bool const IS_COMMUTATIVE = IS_COMMUTATIVE_;

    static std::string type_as_string ()
    {
        return "Ring_c<" + TypeStringOf_t<AdditiveIdentity>::eval() + ','
                         + TypeStringOf_t<Addition>::eval() + ','
                         + TypeStringOf_t<AdditiveInversion>::eval() + ','
                         + TypeStringOf_t<MultiplicativeIdentity>::eval() + ','
                         + TypeStringOf_t<Multiplication>::eval() + ','
                         + (IS_COMMUTATIVE ? "IS_COMMUTATIVE" : "IS_NOT_COMMUTATIVE") + '>';
    }
};

template <typename AdditiveIdentity, 
          typename Addition, 
          typename AdditiveInversion, 
          typename MultiplicativeIdentity, 
          typename Multiplication, 
          bool IS_COMMUTATIVE>
struct IsConcept_f<Ring_c<AdditiveIdentity,Addition,AdditiveInversion,MultiplicativeIdentity,Multiplication,IS_COMMUTATIVE> >
{ static bool const V = true; };

template <typename T> struct IsRing_f { static bool const V = false; };

template <typename AdditiveIdentity, 
          typename Addition, 
          typename AdditiveInversion, 
          typename MultiplicativeIdentity, 
          typename Multiplication, 
          bool IS_COMMUTATIVE>
struct IsRing_f<Ring_c<AdditiveIdentity,Addition,AdditiveInversion,MultiplicativeIdentity,Multiplication,IS_COMMUTATIVE> >
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Ring);
// special convenience macros
#define IS_RING_UNIQUELY(Concept) HasUniqueRingStructure_f<Concept>::V
#define AS_RING(Concept) UniqueRingStructureOf_f<Concept>::T

// Field_c

template <typename AdditiveIdentity_, 
          typename Addition_, 
          typename AdditiveInversion_, 
          typename MultiplicativeIdentity_, 
          typename Multiplication_,
          typename MultiplicativeInverse_>
struct Field_c
{
private:
    typedef Group_c<AdditiveIdentity_,Addition_,AdditiveInversion_,true> As_AdditiveGroup;
    typedef Group_c<MultiplicativeIdentity_,Multiplication_,MultiplicativeInverse_,true> As_MultiplicativeGroup;
public:
    typedef TypeList_t<As_AdditiveGroup,TypeList_t<As_MultiplicativeGroup> > ParentTypeList;

    typedef AdditiveIdentity_ AdditiveIdentity;
    typedef Addition_ Addition;
    typedef AdditiveInversion_ AdditiveInversion; 
    typedef MultiplicativeIdentity_ MultiplicativeIdentity;
    typedef Multiplication_ Multiplication;
    typedef MultiplicativeInverse_ MultiplicativeInverse;

    static std::string type_as_string ()
    {
        return "Field_c<" + TypeStringOf_t<AdditiveIdentity>::eval() + ','
                          + TypeStringOf_t<Addition>::eval() + ','
                          + TypeStringOf_t<AdditiveInversion>::eval() + ','
                          + TypeStringOf_t<MultiplicativeIdentity>::eval() + ','
                          + TypeStringOf_t<Multiplication>::eval() + ','
                          + TypeStringOf_t<MultiplicativeInverse>::eval() + '>';
    }
};

template <typename AdditiveIdentity, 
          typename Addition, 
          typename AdditiveInversion, 
          typename MultiplicativeIdentity, 
          typename Multiplication,
          typename MultiplicativeInverse>
struct IsConcept_f<Field_c<AdditiveIdentity,Addition,AdditiveInversion,MultiplicativeIdentity,Multiplication,MultiplicativeInverse> >
{ static bool const V = true; };

template <typename T> struct IsField_f { static bool const V = false; };

template <typename AdditiveIdentity, 
          typename Addition, 
          typename AdditiveInversion, 
          typename MultiplicativeIdentity, 
          typename Multiplication,
          typename MultiplicativeInverse>
struct IsField_f<Field_c<AdditiveIdentity,Addition,AdditiveInversion,MultiplicativeIdentity,Multiplication,MultiplicativeInverse> >
{ static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Field);
// special convenience macros
#define IS_FIELD_UNIQUELY(Concept) HasUniqueFieldStructure_f<Concept>::V
#define AS_FIELD(Concept) UniqueFieldStructureOf_f<Concept>::T

// main function section

#define MAKE_ID_STRUCT(Name) struct Name { static std::string type_as_string () { return #Name; } }

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

    cout << FORMAT_VALUE(TypeStringOf_t<Monoid>::eval()) << '\n';
    cout << FORMAT_VALUE(TypeStringOf_t<Group>::eval()) << '\n';
    cout << FORMAT_VALUE(TypeStringOf_t<Ring>::eval()) << '\n';
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
    cout << FORMAT_VALUE(TypeStringOf_t<AncestorsOfMonoid>::eval()) << '\n';
    cout << FORMAT_VALUE(TypeStringOf_t<AncestorsOfGroup>::eval()) << '\n';
    cout << FORMAT_VALUE(TypeStringOf_t<AncestorsOfRing>::eval()) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((TypeStringOf_t<AncestorsSatisfyingPredicate_f<Monoid,IsMonoid_p>::T>::eval())) << '\n';
    cout << FORMAT_VALUE((TypeStringOf_t<AncestorsSatisfyingPredicate_f<Group,IsMonoid_p>::T>::eval())) << '\n';
    cout << FORMAT_VALUE((TypeStringOf_t<AncestorsSatisfyingPredicate_f<Ring,IsMonoid_p>::T>::eval())) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((TypeStringOf_t<ConceptualStructuresOf_f<Monoid,IsMonoid_p>::T>::eval())) << '\n';
    cout << FORMAT_VALUE((HasConceptualStructure_f<Monoid,IsMonoid_p>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueConceptualStructure_f<Monoid,IsMonoid_p>::V)) << '\n';
    cout << FORMAT_VALUE((TypeStringOf_t<UniqueConceptualStructureOf_f<Monoid,IsMonoid_p>::T>::eval())) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((TypeStringOf_t<MonoidStructuresOf_f<Monoid>::T>::eval())) << '\n';
    cout << FORMAT_VALUE((HasMonoidStructure_f<Monoid>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueMonoidStructure_f<Monoid>::V)) << '\n';
    cout << FORMAT_VALUE((TypeStringOf_t<UniqueMonoidStructureOf_f<Monoid>::T>::eval())) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(IS_MONOID_UNIQUELY(Monoid)) << '\n';
    cout << FORMAT_VALUE(TypeStringOf_t<AS_MONOID(Monoid)>::eval()) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((TypeStringOf_t<ConceptualStructuresOf_f<Monoid,IsGroup_p>::T>::eval())) << '\n';
    cout << FORMAT_VALUE((HasConceptualStructure_f<Monoid,IsGroup_p>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueConceptualStructure_f<Monoid,IsGroup_p>::V)) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((TypeStringOf_t<ConceptualStructuresOf_f<Group,IsMonoid_p>::T>::eval())) << '\n';
    cout << FORMAT_VALUE((HasConceptualStructure_f<Group,IsMonoid_p>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueConceptualStructure_f<Group,IsMonoid_p>::V)) << '\n';
    cout << FORMAT_VALUE((TypeStringOf_t<UniqueConceptualStructureOf_f<Group,IsMonoid_p>::T>::eval())) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(IS_MONOID_UNIQUELY(Group)) << '\n';
    cout << FORMAT_VALUE(TypeStringOf_t<AS_MONOID(Group)>::eval()) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((TypeStringOf_t<ConceptualStructuresOf_f<Ring,IsMonoid_p>::T>::eval())) << '\n';
    cout << FORMAT_VALUE((HasConceptualStructure_f<Ring,IsMonoid_p>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueConceptualStructure_f<Ring,IsMonoid_p>::V)) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE((TypeStringOf_t<ConceptualStructuresOf_f<Field,IsGroup_p>::T>::eval())) << '\n';
    cout << FORMAT_VALUE((HasConceptualStructure_f<Field,IsGroup_p>::V)) << '\n';
    cout << FORMAT_VALUE((HasUniqueConceptualStructure_f<Field,IsGroup_p>::V)) << '\n';
    cout << '\n';

    // some test usage to get various properties of conceptual structures
    {
        cout << FORMAT_VALUE(TypeStringOf_t<UniqueGroupStructureOf_f<Ring>::T>::eval()) << '\n';
        cout << FORMAT_VALUE(TypeStringOf_t<AS_GROUP(Ring)>::eval()) << '\n';
        cout << FORMAT_VALUE(TypeStringOf_t<UniqueGroupStructureOf_f<Ring>::T::Identity>::eval()) << '\n';
        cout << FORMAT_VALUE(TypeStringOf_t<AS_GROUP(Ring)::Identity>::eval()) << '\n';
        // typedef UniqueGroupStructureOf_f<Ring>::T::Identity MultiplicativeIdentity;
        typedef AS_GROUP(Ring)::Identity MultiplicativeIdentity;
        cout << FORMAT_VALUE(TypeStringOf_t<MultiplicativeIdentity>::eval()) << '\n';
        cout << '\n';
    }

    // testing Map_t
    {
        {
            cout << FORMAT_VALUE(TypeStringOf_t<EmptyMap>::eval()) << '\n';
            // typedef EvalMap_t<EmptyMap,int>::T ThisShouldCauseACompileError;
        }

        {
            typedef Map_t<int,float> SimpleMap;
            cout << FORMAT_VALUE(TypeStringOf_t<SimpleMap>::eval()) << '\n';

            typedef EvalMap_t<SimpleMap,int>::T ShouldBe_float;
            cout << FORMAT_VALUE(TypeStringOf_t<ShouldBe_float>::eval()) << '\n';
            Lvd::Meta::Assert<(Lvd::Meta::TypesAreEqual<ShouldBe_float,float>::v)>();
            // typedef EvalMap_t<SimpleMap,bool>::T ThisShouldCauseACompileError;

            cout << FORMAT_VALUE((MapDomainContains_t<SimpleMap,int>::V)) << '\n';
            cout << FORMAT_VALUE((MapDomainContains_t<SimpleMap,int>::V)) << '\n';
        }

        {
            typedef Map_t<int,float,Map_t<char,double> > TwoElementMap;
            cout << FORMAT_VALUE(TypeStringOf_t<TwoElementMap>::eval()) << '\n';

            typedef EvalMap_t<TwoElementMap,int>::T ShouldBe_float;
            typedef EvalMap_t<TwoElementMap,char>::T ShouldBe_double;
            cout << FORMAT_VALUE(TypeStringOf_t<ShouldBe_float>::eval()) << '\n';
            cout << FORMAT_VALUE(TypeStringOf_t<ShouldBe_double>::eval()) << '\n';
            Lvd::Meta::Assert<(Lvd::Meta::TypesAreEqual<ShouldBe_float,float>::v)>();
            Lvd::Meta::Assert<(Lvd::Meta::TypesAreEqual<ShouldBe_double,double>::v)>();
            // typedef EvalMap_t<TwoElementMap,bool>::T ThisShouldCauseACompileError;
            cout << '\n';
        }

        {
            cout << "testing MapConstructor_t\n";
            typedef Map_t<int,float> Map;
            cout << FORMAT_VALUE(TypeStringOf_t<Map>::eval()) << '\n';
            cout << FORMAT_VALUE((TypeStringOf_t<MapConstructor_t<EmptyTypeList,EmptyTypeList,EmptyMap>::T>::eval())) << '\n';
            cout << FORMAT_VALUE((TypeStringOf_t<MapConstructor_t<EmptyTypeList,EmptyTypeList,Map>::T>::eval())) << '\n';

            typedef TypeList_t<char,TypeList_t<bool> > Domain;
            typedef TypeList_t<float,TypeList_t<double> > Codomain;
            cout << FORMAT_VALUE(TypeStringOf_t<Domain>::eval()) << '\n';
            cout << FORMAT_VALUE(TypeStringOf_t<Codomain>::eval()) << '\n';
            cout << FORMAT_VALUE((TypeStringOf_t<MapConstructor_t<Domain,Codomain,EmptyMap>::T>::eval())) << '\n';
            cout << FORMAT_VALUE((TypeStringOf_t<MapConstructor_t<Domain,Codomain,Map>::T>::eval())) << '\n';
            cout << '\n';
        }

        {
            cout << "testing MapUnion_t\n";
            typedef MapUnion_t<EmptyTypeList>::T M;
            cout << FORMAT_VALUE(TypeStringOf_t<M>::eval()) << '\n';
            typedef Map_t<int,float,Map_t<bool,char> > Map1;
            typedef Map_t<int,float,Map_t<char,double> > Map2;
            typedef Map_t<int,double,Map_t<char,double> > Map3;
            cout << FORMAT_VALUE(TypeStringOf_t<Map1>::eval()) << '\n';
            cout << FORMAT_VALUE(TypeStringOf_t<Map2>::eval()) << '\n';
            cout << FORMAT_VALUE(TypeStringOf_t<MapUnion_t<TypeList_t<Map1> >::T>::eval()) << '\n';
            cout << FORMAT_VALUE((TypeStringOf_t<MapUnion_t<TypeList_t<Map1,TypeList_t<Map2> > >::T>::eval())) << '\n';
            // typedef MapUnion_t<TypeList_t<Map1,TypeList_t<Map3> > >::T ThisShouldCauseACompileError;
            cout << '\n';
        }
    }

    return 0;
}
