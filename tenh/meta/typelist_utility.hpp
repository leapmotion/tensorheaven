// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typelist_utility.hpp by Victor Dods, created 2013/03/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_META_TYPELIST_UTILITY_HPP_
#define TENH_META_TYPELIST_UTILITY_HPP_

#include "tenh/core.hpp"
#include "tenh/meta/typelist.hpp"

namespace Tenh {


// this requires that each type in FromTypeList has a default constructor.
// if the constructors for each of the elements in each type list have no
// side effects, then ostensibly this code will be optimized out.
template <typename FromHeadType, typename FromBodyTypeList,
          typename ToHeadType, typename ToBodyTypeList>
inline void compile_time_check_that_there_is_a_type_conversion (TypeList_t<FromHeadType,FromBodyTypeList> const &from, TypeList_t<ToHeadType,ToBodyTypeList> const &to)
{
    FromHeadType from_head;
    ToHeadType head_conversion_checker(from_head);
    // tail recursion
    compile_time_check_that_there_is_a_type_conversion(FromBodyTypeList(), ToBodyTypeList());
}

template <typename FromHeadType,
          typename ToHeadType>
inline void compile_time_check_that_there_is_a_type_conversion (TypeList_t<FromHeadType> const &from, TypeList_t<ToHeadType> const &to)
{
    // base case
    FromHeadType from_head;
    ToHeadType head_conversion_checker(from_head);
}

inline void compile_time_check_that_there_is_a_type_conversion (EmptyTypeList const &from, EmptyTypeList const &to)
{
    // nothing needs to be done, there are no types to check
}





// returns the last element of a TypeList_t
template <typename TypeList>
struct TailOfTypeList_t
{
    enum { STATIC_ASSERT_IN_ENUM((TypeList::LENGTH > 0), LENGTH_MUST_BE_POSITIVE) };

    typedef typename TailOfTypeList_t<typename TypeList::BodyTypeList>::T T;
};

template <typename HeadType>
struct TailOfTypeList_t<TypeList_t<HeadType> >
{
    typedef HeadType T;
};



// returns a TypeList_t missing its tail (last element)
template <typename TypeList>
struct AllButTailOfTypeList_t
{
    enum { STATIC_ASSERT_IN_ENUM((TypeList::LENGTH > 0), LENGTH_MUST_BE_POSITIVE) };

    typedef TypeList_t<typename TypeList::HeadType,typename AllButTailOfTypeList_t<typename TypeList::BodyTypeList>::T> T;
};

template <typename HeadType>
struct AllButTailOfTypeList_t<TypeList_t<HeadType> >
{
    typedef EmptyTypeList T;
};





template <typename TypeList>
struct ReversedTypeList_t
{
    typedef TypeList_t<typename TailOfTypeList_t<TypeList>::T,typename ReversedTypeList_t<typename AllButTailOfTypeList_t<TypeList>::T>::T> T;
};

template <>
struct ReversedTypeList_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};




template <typename TypeList, typename UsedTypeList = EmptyTypeList>
struct UniqueTypesIn_t
{
private:
    typedef typename TypeList::HeadType HeadType;
    typedef typename TypeList::BodyTypeList BodyTypeList;
    typedef TypeList_t<HeadType,UsedTypeList> NextUsedTypeList;
    typedef typename UniqueTypesIn_t<BodyTypeList,NextUsedTypeList>::T RemainingUniqueTypeList;
public:
    typedef typename If<(UsedTypeList::template Contains_t<HeadType>::V),
                        RemainingUniqueTypeList,
                        TypeList_t<HeadType,RemainingUniqueTypeList> >::T T;
};

template <typename UsedTypeList>
struct UniqueTypesIn_t<EmptyTypeList,UsedTypeList>
{
    typedef EmptyTypeList T;
};



// input: TypeList_t TypeList, typename Type -- output: a count of how many times Type occured in TypeList
template <typename TypeList, typename Type>
struct Occurrence_t
{
    static Uint32 const COUNT =
        (TypesAreEqual<typename TypeList::HeadType,Type>::V ? 1 : 0)
        + Occurrence_t<typename TypeList::BodyTypeList,Type>::COUNT;
};

template <typename Type>
struct Occurrence_t<EmptyTypeList,Type>
{
    static Uint32 const COUNT = 0;
};


// input: TypeList_t TypeList, typename Type -- output: the Uint32 INDEX such that TypeList::El_t<INDEX>::T is the first occurrence of Type
// will compile-time assert if there is no occurrence
template <typename TypeList, typename Type>
struct FirstMatchingIn_t
{
    enum { STATIC_ASSERT_IN_ENUM((Occurrence_t<TypeList,Type>::COUNT > 0), TYPE_MUST_APPEAR_IN_TYPELIST) };
    static Uint32 const INDEX = If<TypesAreEqual<typename TypeList::HeadType,Type>::V,
                                   FirstMatchingIn_t<TypeList_t<typename TypeList::HeadType>,typename TypeList::HeadType>,
                                   FirstMatchingIn_t<typename TypeList::BodyTypeList,Type> >::T::INDEX
                                +
                                (TypesAreEqual<typename TypeList::HeadType,Type>::V ? 0 : 1);
                                // this offset is what gets past HeadType if there is no match here
};

template <typename HeadType, typename Type>
struct FirstMatchingIn_t<TypeList_t<HeadType>,Type>
{
    enum { STATIC_ASSERT_IN_ENUM((TypesAreEqual<HeadType,Type>::V), TYPE_MUST_APPEAR_IN_TYPELIST) };
    static Uint32 const INDEX = 0;
};




// input: TypeList_t TypeList, typename UniqueTypeList, Uint32 MULTIPLICITY -- output: the types in UniqueTypeList which occur
// exactly MULTIPLICITY times in TypeList
template <typename TypeList, typename UniqueTypeList, Uint32 MULTIPLICITY>
struct ElementsOfTypeListHavingMultiplicity_t
{
    typedef typename ElementsOfTypeListHavingMultiplicity_t<TypeList,typename UniqueTypeList::BodyTypeList,MULTIPLICITY>::T InBody;
    typedef typename If<(Occurrence_t<TypeList,typename UniqueTypeList::HeadType>::COUNT == MULTIPLICITY),
                        TypeList_t<typename UniqueTypeList::HeadType,InBody>,
                        InBody>::T T;
};

template <typename TypeList, Uint32 MULTIPLICITY>
struct ElementsOfTypeListHavingMultiplicity_t<TypeList,EmptyTypeList,MULTIPLICITY>
{
    typedef EmptyTypeList T;
};

// input: TypeList_t TypeList -- output: a TypeList_t containing the elements of TypeList which occur exactly MULTIPLICITY times
template <typename TypeList, Uint32 MULTIPLICITY>
struct ElementsHavingMultiplicity_t
{
    typedef typename UniqueTypesIn_t<TypeList>::T UniqueTypeList;
    typedef typename ElementsOfTypeListHavingMultiplicity_t<TypeList,UniqueTypeList,MULTIPLICITY>::T T;
};




// Predicate must be a struct having a template <> struct Eval_t { static bool const V; };
template <typename TypeList, typename Predicate>
struct ElementsOfTypeListSatisfyingPredicate_t
{
    typedef typename ElementsOfTypeListSatisfyingPredicate_t<typename TypeList::BodyTypeList,Predicate>::T ElementsInBodyTypeListSatisfyingPredicate;
    typedef typename If<Predicate::template Eval_t<typename TypeList::HeadType>::V,
                        TypeList_t<typename TypeList::HeadType,ElementsInBodyTypeListSatisfyingPredicate>,
                        ElementsInBodyTypeListSatisfyingPredicate>::T T;
};

template <typename HeadType, typename Predicate>
struct ElementsOfTypeListSatisfyingPredicate_t<TypeList_t<HeadType>,Predicate>
{
    typedef typename If<Predicate::template Eval_t<HeadType>::V,
                        TypeList_t<HeadType>,
                        EmptyTypeList>::T T;
};

template <typename Predicate>
struct ElementsOfTypeListSatisfyingPredicate_t<EmptyTypeList,Predicate>
{
    typedef EmptyTypeList T;
};


// used to negate the predicates used in ElementsOfTypeListSatisfyingPredicate_t
template <typename Predicate>
struct NegationOfPredicate_t
{
    template <typename T> struct Eval_t { static bool const V = !Predicate::template Eval_t<T>::V; };
};




// determines if there are duplicate types in the given type list
template <typename TypeList>
struct ContainsDuplicates_t
{
    static bool const V = UniqueTypesIn_t<TypeList>::T::LENGTH < TypeList::LENGTH;
};




// concatenates two TypeList_t
template <typename FirstTypeList, typename SecondTypeList>
struct ConcatenationOfTypeLists_t
{
    typedef typename FirstTypeList::HeadType FirstTypeListHead;
    typedef typename FirstTypeList::BodyTypeList FirstTypeListBody;
    typedef TypeList_t<FirstTypeListHead,typename ConcatenationOfTypeLists_t<FirstTypeListBody,SecondTypeList>::T> T;
};

template <typename FirstTypeListHead, typename SecondTypeList>
struct ConcatenationOfTypeLists_t<TypeList_t<FirstTypeListHead>,SecondTypeList>
{
    typedef TypeList_t<FirstTypeListHead,SecondTypeList> T;
};

template <typename SecondTypeList>
struct ConcatenationOfTypeLists_t<EmptyTypeList,SecondTypeList>
{
    typedef SecondTypeList T;
};



// determines if the types of A are contained in B
template <typename TypeListA, typename TypeListB>
struct IsASubsetOf_t
{
    static bool const V = TypeListB::template Contains_t<typename TypeListA::HeadType>::V && IsASubsetOf_t<typename TypeListA::BodyTypeList,TypeListB>::V;
};

template <typename TypeListAHead, typename TypeListB>
struct IsASubsetOf_t<TypeList_t<TypeListAHead>,TypeListB>
{
    static bool const V = TypeListB::template Contains_t<TypeListAHead>::V;
};

template <typename TypeListB>
struct IsASubsetOf_t<EmptyTypeList,TypeListB>
{
    static bool const V = true;
};




// determines if the type lists contain the same types (set equality, ignoring repeated elements)
template <typename TypeListA, typename TypeListB>
struct AreEqualAsSets_t
{
    static bool const V = IsASubsetOf_t<TypeListA,TypeListB>::V && IsASubsetOf_t<TypeListB,TypeListA>::V;
};



template <typename TypeListA, typename TypeListB>
struct HasNontrivialIntersectionAsSets_t
{
    static bool const V = TypeListB::template Contains_t<typename TypeListA::HeadType>::V ||
                          HasNontrivialIntersectionAsSets_t<typename TypeListA::BodyTypeList,TypeListB>::V;
};

template <typename TypeListB>
struct HasNontrivialIntersectionAsSets_t<EmptyTypeList,TypeListB>
{
    static bool const V = false;
};




template <typename TypeListA, typename TypeListB>
struct IntersectionAsSets_t
{
private:
    typedef typename IntersectionAsSets_t<typename TypeListA::BodyTypeList,TypeListB>::T RecursionTypeList;
public:
    typedef typename If<TypeListB::template Contains_t<typename TypeListA::HeadType>::V,
                        TypeList_t<typename TypeListA::HeadType,RecursionTypeList>,
                        RecursionTypeList>::T T;
};

template <typename TypeListB>
struct IntersectionAsSets_t<EmptyTypeList,TypeListB>
{
    typedef EmptyTypeList T;
};


// renders the subtraction of TypeListB from TypeListA, as sets
template <typename TypeListA, typename TypeListB>
struct SetSubtraction_t
{
private:
    typedef typename If<TypeListB::template Contains_t<typename TypeListA::HeadType>::V,
                        EmptyTypeList,
                        TypeList_t<typename TypeListA::HeadType> >::T HeadTypeList;
public:
    typedef typename ConcatenationOfTypeLists_t<HeadTypeList,typename SetSubtraction_t<typename TypeListA::BodyTypeList,TypeListB>::T>::T T;
};

template <typename TypeListB>
struct SetSubtraction_t<EmptyTypeList,TypeListB>
{
    typedef EmptyTypeList T;
};



template <typename TypeLists>
struct EachTypeListHasEqualLength_t
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsTypeList_f<TypeLists>::V, MUST_BE_TYPELIST)
    };
    typedef typename TypeLists::HeadType First;
    typedef typename TypeLists::BodyTypeList::HeadType Second;
public:
    static bool const V = (First::LENGTH == Second::LENGTH) &&
                          EachTypeListHasEqualLength_t<typename TypeLists::BodyTypeList>::V;
};

template <typename HeadType>
struct EachTypeListHasEqualLength_t<TypeList_t<HeadType> >
{
    static bool const V = true; // trivially true
};

template <>
struct EachTypeListHasEqualLength_t<EmptyTypeList>
{
    static bool const V = true; // vacuously true
};


// makes a TypeList_t of the HeadType of each TypeList_t in the list
template <typename TypeLists>
struct HeadTypeOfEach_t
{
private:
    typedef typename TypeLists::HeadType FirstTypeList;
    typedef typename TypeLists::BodyTypeList RestOfTypeLists;
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<TypeLists>::V, MUST_BE_TYPELIST, TYPELISTS),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<FirstTypeList>::V, MUST_BE_TYPELIST, FIRSTTYPELIST),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<RestOfTypeLists>::V, MUST_BE_TYPELIST, RESTOFTYPELISTS),
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
public:
    typedef TypeList_t<typename FirstTypeList::HeadType,typename HeadTypeOfEach_t<RestOfTypeLists>::T> T;
};

template <typename RestOfTypeLists>
struct HeadTypeOfEach_t<TypeList_t<EmptyTypeList,RestOfTypeLists> >
{
private:
    typedef TypeList_t<EmptyTypeList,RestOfTypeLists> TypeLists;
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<TypeLists>::V, MUST_BE_TYPELIST, TYPELISTS),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<RestOfTypeLists>::V, MUST_BE_TYPELIST, RESTOFTYPELISTS),
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
public:
    typedef EmptyTypeList T;
};

template <>
struct HeadTypeOfEach_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};


// makes a TypeList_t of the BodyTypeList of each TypeList_t in the list
template <typename TypeLists>
struct BodyTypeListOfEach_t
{
private:
    typedef typename TypeLists::HeadType FirstTypeList;
    typedef typename TypeLists::BodyTypeList RestOfTypeLists;
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<TypeLists>::V, MUST_BE_TYPELIST, TYPELISTS),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<FirstTypeList>::V, MUST_BE_TYPELIST, FIRSTTYPELIST),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<RestOfTypeLists>::V, MUST_BE_TYPELIST, RESTOFTYPELISTS),
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
public:
    typedef TypeList_t<typename FirstTypeList::BodyTypeList,typename BodyTypeListOfEach_t<RestOfTypeLists>::T> T;
};

template <typename RestOfTypeLists>
struct BodyTypeListOfEach_t<TypeList_t<EmptyTypeList,RestOfTypeLists> >
{
private:
    typedef TypeList_t<EmptyTypeList,RestOfTypeLists> TypeLists;
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<TypeLists>::V, MUST_BE_TYPELIST, TYPELISTS),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<RestOfTypeLists>::V, MUST_BE_TYPELIST, RESTOFTYPELISTS),
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
public:
    typedef EmptyTypeList T;
};

template <>
struct BodyTypeListOfEach_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};


// "zips" a list of typelists into a typelist of tuples (implemented via TypeList_t).
// the heads of all the typelists are put into the head element, etc.
// e.g. (('a','b','c'),(1,2,3)) turns into (('a',1),('b',2),('c',3)).
template <typename TypeLists>
struct Zip_t
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
    typedef typename HeadTypeOfEach_t<TypeLists>::T HeadTypes;
    typedef typename BodyTypeListOfEach_t<TypeLists>::T BodyTypeLists;
public:
    typedef TypeList_t<HeadTypes,typename Zip_t<BodyTypeLists>::T> T;
};

template <typename RestOfTypeLists>
struct Zip_t<TypeList_t<EmptyTypeList,RestOfTypeLists> >
{
private:
    typedef TypeList_t<EmptyTypeList,RestOfTypeLists> TypeLists;
    enum
    {
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
public:
    typedef EmptyTypeList T;
};

template <>
struct Zip_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};


// the inverse to Zip_t -- e.g. (('a',1),('b',2),('c',3)) turns into (('a','b','c'),(1,2,3)).
template <typename ZippedTypeLists>
struct Unzip_t
{
    typedef typename Zip_t<ZippedTypeLists>::T T; // Zip_t is its own inverse
};



template <typename Type, Uint32 LENGTH>
struct UniformTypeListOfLength_t
{
    typedef TypeList_t<Type,typename UniformTypeListOfLength_t<Type,LENGTH-1>::T> T;
};

template <typename Type>
struct UniformTypeListOfLength_t<Type,0>
{
    typedef EmptyTypeList T;
};



template <typename TypeList> struct TypeListIsUniform_t;

// V is true iff each type in the typelist is the same
template <typename HeadType, typename BodyTypeList>
struct TypeListIsUniform_t<TypeList_t<HeadType,BodyTypeList> >
{
    static bool const V = TypesAreEqual<HeadType,typename BodyTypeList::HeadType>::V &&
                          TypeListIsUniform_t<BodyTypeList>::V;
};

template <typename HeadType>
struct TypeListIsUniform_t<TypeList_t<HeadType> >
{
    static bool const V = true;
};

template <>
struct TypeListIsUniform_t<EmptyTypeList>
{
    static bool const V = true;
};

template <typename Type, Uint32 MULTIPLICITY>
struct TypeListWithMultiplicity_t
{
    typedef TypeList_t<Type,typename TypeListWithMultiplicity_t<Type,MULTIPLICITY-1>::T> T;
};

template <typename Type>
struct TypeListWithMultiplicity_t<Type,0>
{
    typedef EmptyTypeList T;
};


template <typename TypeList, typename Predicate>
struct EachTypeSatisfies_f
{
    enum { STATIC_ASSERT_IN_ENUM(IsTypeList_f<TypeList>::V, MUST_BE_TYPELIST) };
    static bool const V = Predicate::template Eval_t<typename TypeList::HeadType>::V && EachTypeSatisfies_f<typename TypeList::BodyTypeList,Predicate>::V;
};

template <typename Predicate>
struct EachTypeSatisfies_f<EmptyTypeList, Predicate>
{
    static bool const V = true;
};

} // end of namespace Tenh

#endif // TENH_META_TYPELIST_UTILITY_HPP_
