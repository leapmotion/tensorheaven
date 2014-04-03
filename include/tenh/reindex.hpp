// ///////////////////////////////////////////////////////////////////////////
// tenh/reindex.hpp by Victor Dods, created 2013/10/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_REINDEX_HPP_
#define TENH_REINDEX_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/dimindex.hpp"
#include "tenh/tuple.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// metafunctions for renaming sets of abstract indices
// ///////////////////////////////////////////////////////////////////////////

// this is really Map_e but without a domain check (has nothing to do with indices)
// TODO: Map could have an optional DONT_CHECK_DOMAIN_ param.
template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
struct AbstractIndexMap_e
{
    static_assert(Length_f<DomainAbstractIndexTyple_>::V > 0, "DomainAbstractIndexTyple_ must be nonempty");
    static_assert(!ContainsDuplicates_f<DomainAbstractIndexTyple_>::V, "DomainAbstractIndexTyple_ must not contain duplicates");
    static_assert(Length_f<DomainAbstractIndexTyple_>::V == Length_f<CodomainAbstractIndexTyple_>::V, "DomainAbstractIndexTyple_ and CodomainAbstractIndexTyple_ must have same length");
    static_assert(And_f<typename OnEach_f<DomainAbstractIndexTyple_,IsAbstractIndex_e>::T>::V, "DomainAbstractIndexTyple_ must be Typle_t of AbstractIndex_c types");
    static_assert(And_f<typename OnEach_f<CodomainAbstractIndexTyple_,IsAbstractIndex_e>::T>::V, "CodomainAbstractIndexTyple_ must be Typle_t of AbstractIndex_c types");
private:
    AbstractIndexMap_e();
public:
    template <typename AbstractIndex_>
    struct Eval_f
    {
        // don't actually check that AbstractIndex_ is in DomainAbstractIndexTyple_, since
        // the If_f below instantiates this type even if the type isn't "used" by the If_f.
        typedef typename Element_f<CodomainAbstractIndexTyple_,IndexOfFirstOccurrence_f<DomainAbstractIndexTyple_,AbstractIndex_>::V>::T T;
    private:
        Eval_f();
    };
    template <typename AbstractIndex_>
    typename Eval_f<AbstractIndex_>::T operator () (AbstractIndex_ const &)
    {
        return typename Eval_f<AbstractIndex_>::T();
    }
};

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e "forward declaration"
// ///////////////////////////////////////////////////////////////////////////

// this metafunction will be specialized to do index renaming on a bunch of different types
template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
struct Reindex_e
{
    // the default definition is to do nothing
    template <typename ThingThatHasIndices_>
    struct Eval_f
    {
        typedef ThingThatHasIndices_ T;
    private:
        Eval_f();
    };
    template <typename ThingThatHasIndices_>
    typename Eval_f<ThingThatHasIndices_>::T operator () (ThingThatHasIndices_ const &)
    {
        return typename Eval_f<ThingThatHasIndices_>::T();
    }
private:
    Reindex_e();
};

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e for AbstractIndex_c
// ///////////////////////////////////////////////////////////////////////////

// If AbstractIndexMap_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
// is the identity map on its domain (which basically means that CodomainAbstractIndexTyple_
// is identical to DomainAbstractIndexTyple_), make sure this is the identity
// map on its domain.
template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <AbstractIndexSymbol SYMBOL_>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>::Eval_f<AbstractIndex_c<SYMBOL_>>
{
private:
    typedef AbstractIndex_c<SYMBOL_> AbstractIndex;
    typedef AbstractIndexMap_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> AbstractIndexMap;
    static bool const ABSTRACT_INDEX_MAP_IS_IDENTITY = TypesAreEqual_f<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>::V;
    // if AbstractIndexMap is the identity on its domain, then assigning OFFSET to
    // zero will cause this Reindex_c map to be the identity on its domain.
    static AbstractIndexSymbol const OFFSET =
        ABSTRACT_INDEX_MAP_IS_IDENTITY ?
        0 :
        Max_f<typename OnEach_f<CodomainAbstractIndexTyple_,SymbolOf_e>::T,AbstractIndexSymbol>::V;
    Eval_f();
public:
    typedef typename If_f<Contains_f<DomainAbstractIndexTyple_,AbstractIndex>::V,
                          typename AbstractIndexMap::template Eval_f<AbstractIndex>,
                          Type_t<AbstractIndex_c<OFFSET+SYMBOL_>>>::T::T T;
};

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e for DimIndex_t, induced by the one for AbstractIndex_c
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <AbstractIndexSymbol SYMBOL_, Uint32 DIM_>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>::Eval_f<DimIndex_t<SYMBOL_,DIM_>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    typedef typename Reindex::template Eval_f<AbstractIndex_c<SYMBOL_>>::T MappedAbstractIndex;
    Eval_f();
public:
    typedef DimIndex_t<SymbolOf_f<MappedAbstractIndex>::V,DIM_> T;
};

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e for Typle_t -- applies Reindex_e to each element
// ///////////////////////////////////////////////////////////////////////////

// this is defined so that the operations of renaming indices and constructing
// a Typle_t commute.
template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename... Types_>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>::Eval_f<Typle_t<Types_...>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    typedef Typle_t<Types_...> Typle;
    Eval_f();
public:
    typedef typename OnEach_f<Typle,Reindex>::T T;
};

// NOTE: the default implementation of Reindex_e<...>::Eval_f<T> is correct for Typle_t<>

// ///////////////////////////////////////////////////////////////////////////
// function which reindexes actual instances -- default definition is a no-op.
// ///////////////////////////////////////////////////////////////////////////

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename ThingThatHasIndices_>
ThingThatHasIndices_ const &reindexed (ThingThatHasIndices_ const &x)
{
    return x;
}

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename ThingThatHasIndices_>
ThingThatHasIndices_ &reindexed (ThingThatHasIndices_ &x)
{
    return x;
}

// ///////////////////////////////////////////////////////////////////////////
// overload to reindex AbstractIndex_c objects
// ///////////////////////////////////////////////////////////////////////////

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          AbstractIndexSymbol SYMBOL_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<AbstractIndex_c<SYMBOL_>>::T
    reindexed (AbstractIndex_c<SYMBOL_> const &i)
{
    return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                    ::template Eval_f<AbstractIndex_c<SYMBOL_>>::T();
}

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          AbstractIndexSymbol SYMBOL_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<AbstractIndex_c<SYMBOL_>>::T
    reindexed (AbstractIndex_c<SYMBOL_> &i)
{
    return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                    ::template Eval_f<AbstractIndex_c<SYMBOL_>>::T();
}

// ///////////////////////////////////////////////////////////////////////////
// overload to reindex DimIndex_t objects
// ///////////////////////////////////////////////////////////////////////////

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          AbstractIndexSymbol SYMBOL_,
          Uint32 DIM_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<DimIndex_t<SYMBOL_,DIM_>>::T
    reindexed (DimIndex_t<SYMBOL_,DIM_> const &i)
{
    return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                    ::template Eval_f<DimIndex_t<SYMBOL_,DIM_>>::T(i.value(), CheckRange::FALSE);
}

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          AbstractIndexSymbol SYMBOL_,
          Uint32 DIM_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<DimIndex_t<SYMBOL_,DIM_>>::T
    reindexed (DimIndex_t<SYMBOL_,DIM_> &i)
{
    return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                    ::template Eval_f<DimIndex_t<SYMBOL_,DIM_>>::T(i.value(), CheckRange::FALSE);
}

// ///////////////////////////////////////////////////////////////////////////
// overload to reindex Typle_t INSTANCES -- applies reindexed to each element
// ///////////////////////////////////////////////////////////////////////////

// unfortunately you have to make a const and a non-const version of each

// this is defined so that the operations of Typle_t construction and reindexing commute.
template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename... Types_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<Typle_t<Types_...>>::T
    reindexed (Typle_t<Types_...> const &)
{
    return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                    ::template Eval_f<Typle_t<Types_...>>::T();
}

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename... Types_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<Typle_t<Types_...>>::T
    reindexed (Typle_t<Types_...> &)
{
    return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                    ::template Eval_f<Typle_t<Types_...>>::T();
}

// template <typename DomainAbstractIndexTyple_,
//           typename CodomainAbstractIndexTyple_,
//           typename HeadType_,
//           typename BodyTyple_>
// typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
//          ::template Eval_f<EmptyTyple>::T
//     reindexed (EmptyTyple const &)
// {
//     // this is always Typle_t<>, but I don't want to put Reindex_e's "guts" here redundantly
//     return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
//                     ::template Eval_f<EmptyTyple>::T();
// }

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<Typle_t<>>::T
    reindexed (Typle_t<> const &)
{
    // this is always Typle_t<>, but I don't want to put Reindex_e's "guts" here redundantly
    return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                    ::template Eval_f<Typle_t<>>::T();
}

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<Typle_t<>>::T
    reindexed (Typle_t<> &)
{
    // this is always Typle_t<>, but I don't want to put Reindex_e's "guts" here redundantly
    return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                    ::template Eval_f<Typle_t<>>::T();
}

/*
// ///////////////////////////////////////////////////////////////////////////
// overload to reindex Tuple_t types -- applies reindexed to each element
// ///////////////////////////////////////////////////////////////////////////

// unfortunately you have to make a const and a non-const version of each

// this is defined so that the operations of tuple construction and reindexing commute.
template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename Typle_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<Tuple_t<Typle_>>::T
    reindexed (Tuple_t<Typle_> const &tuple)
{
    return reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(tuple.head()) |
           reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(tuple.body());
}

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename Typle_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<Tuple_t<Typle_>>::T
    reindexed (Tuple_t<Typle_> &tuple)
{
    return reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(tuple.head()) |
           reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(tuple.body());
}

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<EmptyTuple>::T
    reindexed (EmptyTuple const &tuple)
{
    return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                    ::template Eval_f<EmptyTuple>::T();
}

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<EmptyTuple>::T
    reindexed (EmptyTuple &tuple)
{
    return typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                    ::template Eval_f<EmptyTuple>::T();
}

*/
} // end of namespace Tenh

#endif // TENH_REINDEX_HPP_
