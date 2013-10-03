// ///////////////////////////////////////////////////////////////////////////
// tenh/indexrenamer.hpp by Victor Dods, created 2013/10/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INDEXRENAMER_HPP_
#define TENH_INDEXRENAMER_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/dimindex.hpp"
#include "tenh/meta/typelist.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// metafunctions for renaming sets of abstract indices
// ///////////////////////////////////////////////////////////////////////////

// this is really Map_e but without a domain check (has nothing to do with indices)
// TODO: Map could have an optional DONT_CHECK_DOMAIN_ param.
template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
struct BaseAbstractIndexRenamer_e
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
// IndexRenamer_e "forward declaration"
// ///////////////////////////////////////////////////////////////////////////

// this metafunction will be specialized to do index renaming on a bunch of different types
template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
struct IndexRenamer_e
{
    // the default definition is to do nothing
    template <typename ThingThatHasIndices_>
    struct Eval_f
    {
        typedef ThingThatHasIndices_ T;
    };
};

// ///////////////////////////////////////////////////////////////////////////
// IndexRenamer_e for AbstractIndex_c
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <AbstractIndexSymbol SYMBOL_>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>::Eval_f<AbstractIndex_c<SYMBOL_> >
{
private:
    typedef AbstractIndex_c<SYMBOL_> AbstractIndex;
    typedef BaseAbstractIndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> BaseIndexRenamer;
    static AbstractIndexSymbol const OFFSET = Max_f<typename OnEach_f<CodomainAbstractIndexTypeList_,SymbolOf_e>::T,
                                                    AbstractIndexSymbol>::V;
public:
    typedef typename If_f<Contains_f<DomainAbstractIndexTypeList_,AbstractIndex>::V,
                          typename BaseIndexRenamer::template Eval_f<AbstractIndex>::T,
                          AbstractIndex_c<OFFSET + SYMBOL_> >::T T;
};

// ///////////////////////////////////////////////////////////////////////////
// IndexRenamer_e for DimIndex_t, induced by the one for AbstractIndex_c
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <AbstractIndexSymbol SYMBOL_, Uint32 DIM_>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>::Eval_f<DimIndex_t<SYMBOL_,DIM_> >
{
private:
    typedef IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> IndexRenamer;
    typedef typename IndexRenamer::template Eval_f<AbstractIndex_c<SYMBOL_> >::T MappedAbstractIndex;
public:
    typedef DimIndex_t<SymbolOf_f<MappedAbstractIndex>::V,DIM_> T;
};

// ///////////////////////////////////////////////////////////////////////////
// IndexRenamer_e for TypeList_t -- applies IndexRenamer_e to each element
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename HeadType_, typename BodyTypeList_>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>::Eval_f<TypeList_t<HeadType_,BodyTypeList_> >
{
private:
    typedef IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> IndexRenamer;
    typedef TypeList_t<HeadType_,BodyTypeList_> TypeList;
public:
    typedef typename OnEach_f<TypeList,IndexRenamer>::T T;
};

} // end of namespace Tenh

#endif // TENH_INDEXRENAMER_HPP_
