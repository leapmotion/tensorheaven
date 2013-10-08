// ///////////////////////////////////////////////////////////////////////////
// tenh/reindex.hpp by Victor Dods, created 2013/10/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_REINDEX_HPP_
#define TENH_REINDEX_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/dimindex.hpp"
#include "tenh/list.hpp"
#include "tenh/meta/typelist.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// metafunctions for renaming sets of abstract indices
// ///////////////////////////////////////////////////////////////////////////

// this is really Map_e but without a domain check (has nothing to do with indices)
// TODO: Map could have an optional DONT_CHECK_DOMAIN_ param.
template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
struct AbstractIndexMap_e
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE((And_f<typename OnEach_f<DomainAbstractIndexTypeList_,IsAbstractIndex_e>::T>::V),
                                      MUST_BE_TYPELIST_OF_ABSTRACT_INDEX_TYPES,
                                      DOMAIN),
        STATIC_ASSERT_IN_ENUM__UNIQUE((And_f<typename OnEach_f<CodomainAbstractIndexTypeList_,IsAbstractIndex_e>::T>::V),
                                      MUST_BE_TYPELIST_OF_ABSTRACT_INDEX_TYPES,
                                      CODOMAIN),
        STATIC_ASSERT_IN_ENUM(!ContainsDuplicates_t<DomainAbstractIndexTypeList_>::V, DOMAIN_INDICES_MUST_NOT_CONTAIN_DUPLICATES)
    };
public:
    template <typename AbstractIndex_>
    struct Eval_f
    {
        // don't actually check that AbstractIndex_ is in DomainAbstractIndexTypeList_, since
        // the If_f below instantiates this type even if the type isn't "used" by the If_f.
        typedef typename Element_f<CodomainAbstractIndexTypeList_,IndexOfFirstOccurrence_f<DomainAbstractIndexTypeList_,AbstractIndex_>::V>::T T;
    };
};

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e "forward declaration"
// ///////////////////////////////////////////////////////////////////////////

// this metafunction will be specialized to do index renaming on a bunch of different types
template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
struct Reindex_e
{
    // the default definition is to do nothing
    template <typename ThingThatHasIndices_>
    struct Eval_f
    {
        typedef ThingThatHasIndices_ T;
    };
};

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e for AbstractIndex_c
// ///////////////////////////////////////////////////////////////////////////

// If AbstractIndexMap_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
// is the identity map on its domain (which basically means that CodomainAbstractIndexTypeList_
// is identical to DomainAbstractIndexTypeList_), make sure this is the identity
// map on its domain.
template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <AbstractIndexSymbol SYMBOL_>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>::Eval_f<AbstractIndex_c<SYMBOL_> >
{
private:
    typedef AbstractIndex_c<SYMBOL_> AbstractIndex;
    typedef AbstractIndexMap_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> AbstractIndexMap;
    static bool const ABSTRACT_INDEX_MAP_IS_IDENTITY = TypesAreEqual_f<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>::V;
    // if AbstractIndexMap is the identity on its domain, then assigning OFFSET to
    // zero will cause this Reindex_c map to be the identity on its domain.
    static AbstractIndexSymbol const OFFSET =
        ABSTRACT_INDEX_MAP_IS_IDENTITY ?
        0 :
        Max_f<typename OnEach_f<CodomainAbstractIndexTypeList_,SymbolOf_e>::T,AbstractIndexSymbol>::V;
public:
    typedef typename If_f<Contains_f<DomainAbstractIndexTypeList_,AbstractIndex>::V,
                          typename AbstractIndexMap::template Eval_f<AbstractIndex>::T,
                          AbstractIndex_c<OFFSET + SYMBOL_> >::T T;
};

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e for DimIndex_t, induced by the one for AbstractIndex_c
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <AbstractIndexSymbol SYMBOL_, Uint32 DIM_>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>::Eval_f<DimIndex_t<SYMBOL_,DIM_> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
    typedef typename Reindex::template Eval_f<AbstractIndex_c<SYMBOL_> >::T MappedAbstractIndex;
public:
    typedef DimIndex_t<SymbolOf_f<MappedAbstractIndex>::V,DIM_> T;
};

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e for TypeList_t -- applies Reindex_e to each element
// ///////////////////////////////////////////////////////////////////////////

// this is defined so that the operations of renaming indices and constructing
// a TypeList_t commute.
template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename HeadType_, typename BodyTypeList_>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>::Eval_f<TypeList_t<HeadType_,BodyTypeList_> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
    typedef TypeList_t<HeadType_,BodyTypeList_> TypeList;
public:
    typedef typename OnEach_f<TypeList,Reindex>::T T;
};

// NOTE: the default implementation of Reindex_e<...>::Eval_f<T> is correct for EmptyTypeList

// ///////////////////////////////////////////////////////////////////////////
// function which reindexes actual instances -- default definition is a no-op.
// ///////////////////////////////////////////////////////////////////////////

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename ThingThatHasIndices_>
ThingThatHasIndices_ const &reindexed (ThingThatHasIndices_ const &x)
{
    return x;
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename ThingThatHasIndices_>
ThingThatHasIndices_ &reindexed (ThingThatHasIndices_ &x)
{
    return x;
}

// ///////////////////////////////////////////////////////////////////////////
// overload to reindex AbstractIndex_c objects
// ///////////////////////////////////////////////////////////////////////////

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          AbstractIndexSymbol SYMBOL_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<AbstractIndex_c<SYMBOL_> >::T
    reindexed (AbstractIndex_c<SYMBOL_> const &i)
{
    return typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                    ::template Eval_f<AbstractIndex_c<SYMBOL_> >::T();
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          AbstractIndexSymbol SYMBOL_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<AbstractIndex_c<SYMBOL_> >::T
    reindexed (AbstractIndex_c<SYMBOL_> &i)
{
    return typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                    ::template Eval_f<AbstractIndex_c<SYMBOL_> >::T();
}

// ///////////////////////////////////////////////////////////////////////////
// overload to reindex DimIndex_t objects
// ///////////////////////////////////////////////////////////////////////////

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          AbstractIndexSymbol SYMBOL_,
          Uint32 DIM_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<DimIndex_t<SYMBOL_,DIM_> >::T
    reindexed (DimIndex_t<SYMBOL_,DIM_> const &i)
{
    return typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                    ::template Eval_f<DimIndex_t<SYMBOL_,DIM_> >::T(i.value(), DONT_CHECK_RANGE);
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          AbstractIndexSymbol SYMBOL_,
          Uint32 DIM_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<DimIndex_t<SYMBOL_,DIM_> >::T
    reindexed (DimIndex_t<SYMBOL_,DIM_> &i)
{
    return typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                    ::template Eval_f<DimIndex_t<SYMBOL_,DIM_> >::T(i.value(), DONT_CHECK_RANGE);
}

// ///////////////////////////////////////////////////////////////////////////
// overload to reindex TypeList_t INSTANCES -- applies reindexed to each element
// ///////////////////////////////////////////////////////////////////////////

// unfortunately you have to make a const and a non-const version of each

// this is defined so that the operations of TypeList_t construction and reindexing commute.
template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename HeadType_,
          typename BodyTypeList_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<TypeList_t<HeadType_,BodyTypeList_> >::T
    reindexed (TypeList_t<HeadType_,BodyTypeList_> const &)
{
    return typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                    ::template Eval_f<TypeList_t<HeadType_,BodyTypeList_> >::T();
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename HeadType_,
          typename BodyTypeList_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<TypeList_t<HeadType_,BodyTypeList_> >::T
    reindexed (TypeList_t<HeadType_,BodyTypeList_> &)
{
    return typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                    ::template Eval_f<TypeList_t<HeadType_,BodyTypeList_> >::T();
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename HeadType_,
          typename BodyTypeList_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<EmptyTypeList>::T
    reindexed (EmptyTypeList const &)
{
    // this is always EmptyTypeList, but I don't want to put Reindex_e's "guts" here redundantly
    return typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                    ::template Eval_f<EmptyTypeList>::T();
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename HeadType_,
          typename BodyTypeList_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<TypeList_t<HeadType_,BodyTypeList_> >::T
    reindexed (EmptyTypeList &)
{
    // this is always EmptyTypeList, but I don't want to put Reindex_e's "guts" here redundantly
    return typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                    ::template Eval_f<EmptyTypeList>::T();
}

/*
// ///////////////////////////////////////////////////////////////////////////
// overload to reindex List_t types -- applies reindexed to each element
// ///////////////////////////////////////////////////////////////////////////

// unfortunately you have to make a const and a non-const version of each

// this is defined so that the operations of list construction and reindexing commute.
template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename TypeList_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<List_t<TypeList_> >::T
    reindexed (List_t<TypeList_> const &list)
{
    return reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(list.head()) |
           reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(list.body());
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename TypeList_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<List_t<TypeList_> >::T
    reindexed (List_t<TypeList_> &list)
{
    return reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(list.head()) |
           reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(list.body());
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<EmptyList>::T
    reindexed (EmptyList const &list)
{
    return typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                    ::template Eval_f<EmptyList>::T();
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<EmptyList>::T
    reindexed (EmptyList &list)
{
    return typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                    ::template Eval_f<EmptyList>::T();
}

*/
} // end of namespace Tenh

#endif // TENH_REINDEX_HPP_
