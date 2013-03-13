#ifndef TYPELIST_UTILITY_HPP_
#define TYPELIST_UTILITY_HPP_

#include "lvd.hpp"
#include "typelist.hpp"

// returns the last element of a TypeList_t
template <typename TypeList>
struct TailOfTypeList_t
{
    enum { _ = Lvd::Meta::Assert<(TypeList::LENGTH > 0)>::v }; // must be nonempty list
    
    typedef typename TailOfTypeList_t<typename TypeList::Body>::T T;
};

template <typename Head>
struct TailOfTypeList_t<TypeList_t<Head> >
{
    typedef Head T;
};



// returns a TypeList_t missing its tail (last element)
template <typename TypeList>
struct AllButTailOfTypeList_t
{
    enum { _ = Lvd::Meta::Assert<(TypeList::LENGTH > 0)>::v }; // must be nonempty list
    
    typedef TypeList_t<typename TypeList::Head,typename AllButTailOfTypeList_t<typename TypeList::Body>::T> T;
};

template <typename Head>
struct AllButTailOfTypeList_t<TypeList_t<Head> >
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
    typedef typename ReversedUniqueTypesIn_t<typename TypeList::Body>::T ReversedUniqueTypesInBody;
    typedef typename Lvd::Meta::If<(ReversedUniqueTypesInBody::template Contains_t<typename TypeList::Head>::V),
                                   ReversedUniqueTypesInBody,
                                   TypeList_t<typename TypeList::Head,ReversedUniqueTypesInBody> >::T T;
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
    typedef typename TypeList::Head Head;
    typedef typename TypeList::Body Body;
    typedef TypeList_t<Head,UsedTypeList> NextUsedTypeList;
    typedef typename UniqueTypesIn_t<Body,NextUsedTypeList>::T RemainingUniqueTypeList;
public:
    typedef typename Lvd::Meta::If<(UsedTypeList::template Contains_t<Head>::V),
                          RemainingUniqueTypeList,
                          TypeList_t<Head,RemainingUniqueTypeList> >::T T;
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
        (Lvd::Meta::TypesAreEqual<typename TypeList::Head,Type>::v ? 1 : 0)
        + Occurrence_t<typename TypeList::Body,Type>::COUNT;
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
    typedef typename ElementsOfListHavingMultiplicity_t<TypeList,typename UniqueTypeList::Body,MULTIPLICITY>::T InBody;
    typedef typename Lvd::Meta::If<(Occurrence_t<TypeList,typename UniqueTypeList::Head>::COUNT == MULTIPLICITY),
                                   TypeList_t<typename UniqueTypeList::Head,InBody>,
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





// concatenates two TypeList_t
template <typename FirstTypeList, typename SecondTypeList>
struct ConcatenationOfTypeLists_t
{
    typedef typename FirstTypeList::Head FirstTypeListHead;
    typedef typename FirstTypeList::Body FirstTypeListBody;
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

#endif // TYPELIST_UTILITY_HPP_
