// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typelist_utility.hpp by Victor Dods, created 2013/03/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_META_TYPELIST_UTILITY_HPP_
#define TENH_META_TYPELIST_UTILITY_HPP_

#include "tenh/core.hpp"
#include "tenh/meta/typelist.hpp"

namespace Tenh {

// returns the last element of a TypeList_t
template <typename TypeList>
struct TailOfTypeList_t
{
    enum { _ = Lvd::Meta::Assert<(TypeList::LENGTH > 0)>::v }; // must be nonempty list

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
    enum { _ = Lvd::Meta::Assert<(TypeList::LENGTH > 0)>::v }; // must be nonempty list

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



/*
// input: TypeList_t TypeList -- output: a TypeList_t having exactly one of each of the elements of TypeList, in
// the reverse order in which they occurred.
template <typename TypeList>
struct ReversedUniqueTypesIn_t
{
    typedef typename ReversedUniqueTypesIn_t<typename TypeList::BodyTypeList>::T ReversedUniqueTypesInBody;
    typedef typename Lvd::Meta::If<(ReversedUniqueTypesInBody::template Contains_t<typename TypeList::HeadType>::V),
                                   ReversedUniqueTypesInBody,
                                   TypeList_t<typename TypeList::HeadType,ReversedUniqueTypesInBody> >::T T;
};

template <>
struct ReversedUniqueTypesIn_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};
*/








template <typename TypeList, typename UsedTypeList = EmptyTypeList>
struct UniqueTypesIn_t
{
private:
    typedef typename TypeList::HeadType HeadType;
    typedef typename TypeList::BodyTypeList BodyTypeList;
    typedef TypeList_t<HeadType,UsedTypeList> NextUsedTypeList;
    typedef typename UniqueTypesIn_t<BodyTypeList,NextUsedTypeList>::T RemainingUniqueTypeList;
public:
    typedef typename Lvd::Meta::If<(UsedTypeList::template Contains_t<HeadType>::V),
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
        (Lvd::Meta::TypesAreEqual<typename TypeList::HeadType,Type>::v ? 1 : 0)
        + Occurrence_t<typename TypeList::BodyTypeList,Type>::COUNT;
};

template <typename Type>
struct Occurrence_t<EmptyTypeList,Type>
{
    static Uint32 const COUNT = 0;
};

// input: TypeList_t TypeList, typename UniqueTypeList, Uint32 MULTIPLICITY -- output: the types in UniqueTypeList which occur
// exactly MULTIPLICITY times in TypeList
template <typename TypeList, typename UniqueTypeList, Uint32 MULTIPLICITY>
struct ElementsOfListHavingMultiplicity_t
{
    typedef typename ElementsOfListHavingMultiplicity_t<TypeList,typename UniqueTypeList::BodyTypeList,MULTIPLICITY>::T InBody;
    typedef typename Lvd::Meta::If<(Occurrence_t<TypeList,typename UniqueTypeList::HeadType>::COUNT == MULTIPLICITY),
                                   TypeList_t<typename UniqueTypeList::HeadType,InBody>,
                                   InBody>::T T;
};

template <typename TypeList, Uint32 MULTIPLICITY>
struct ElementsOfListHavingMultiplicity_t<TypeList,EmptyTypeList,MULTIPLICITY>
{
    typedef EmptyTypeList T;
};

// input: TypeList_t TypeList -- output: a TypeList_t containing the elements of TypeList which occur exactly MULTIPLICITY times
template <typename TypeList, Uint32 MULTIPLICITY>
struct ElementsHavingMultiplicity_t
{
    typedef typename UniqueTypesIn_t<TypeList>::T UniqueTypeList;
    typedef typename ElementsOfListHavingMultiplicity_t<TypeList,UniqueTypeList,MULTIPLICITY>::T T;
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



// renders the subtraction of TypeListB from TypeListA, as sets
template <typename TypeListA, typename TypeListB>
struct SetSubtraction_t
{
private:
    typedef typename Lvd::Meta::If<TypeListB::template Contains_t<typename TypeListA::HeadType>::V,
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

} // end of namespace Tenh

#endif // TENH_META_TYPELIST_UTILITY_HPP_
