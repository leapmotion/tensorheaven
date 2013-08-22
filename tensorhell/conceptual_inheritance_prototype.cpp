#include <iostream>
#include <string>

#include "tenh/core.hpp"

#include "tenh/meta/typelist.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/meta/typestringof.hpp"

using namespace std;
using namespace Tenh;

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
    }

    return 0;
}
