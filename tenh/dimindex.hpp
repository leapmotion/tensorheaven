// ///////////////////////////////////////////////////////////////////////////
// tenh/dimindex.hpp by Victor Dods, created 2013/08/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_DIMINDEX_HPP_
#define TENH_DIMINDEX_HPP_

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp"
#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// this is really only for use inside expression template types, in order to construct MultiIndex_t types.
template <char SYMBOL_, Uint32 DIM_>
struct DimIndex_t
    :
    public ComponentIndex_t<DIM_>,
    public AbstractIndex_c<SYMBOL_>
{
    typedef ComponentIndex_t<DIM_> Parent_ComponentIndex_t;
    typedef AbstractIndex_c<SYMBOL_> Parent_AbstractIndex_c;

    using Parent_ComponentIndex_t::COMPONENT_COUNT;
    static Uint32 const DIM = Parent_ComponentIndex_t::COMPONENT_COUNT;
    using Parent_AbstractIndex_c::SYMBOL;

    DimIndex_t () : Parent_ComponentIndex_t(), Parent_AbstractIndex_c() { }
    explicit DimIndex_t (Uint32 i, bool check_range = CHECK_RANGE) : Parent_ComponentIndex_t(i, check_range) { }
    DimIndex_t (ComponentIndex_t<DIM> const &i) : Parent_ComponentIndex_t(i), Parent_AbstractIndex_c() { }

    static std::string type_as_string () { return std::string("DimIndex_t<'") + SYMBOL + "'," + AS_STRING(DIM) + '>'; }
};

template <typename T> struct IsADimIndex_f { static bool const V = false; };
template <char SYMBOL, Uint32 DIM> struct IsADimIndex_f<DimIndex_t<SYMBOL,DIM> > { static bool const V = true; };

template <char SYMBOL, Uint32 DIM> struct IsAComponentIndex_t<DimIndex_t<SYMBOL,DIM> > { static bool const V = true; };
template <char SYMBOL, Uint32 DIM> struct IsAbstractIndex_f<DimIndex_t<SYMBOL,DIM> > { static bool const V = true; };

template <typename DimIndexTypeList>
struct Parent_ComponentIndex_TypeListOf_t
{
    enum { STATIC_ASSERT_IN_ENUM(IsADimIndex_f<typename DimIndexTypeList::HeadType>::V, MUST_BE_DIM_INDEX) };
    typedef TypeList_t<typename DimIndexTypeList::HeadType::Parent_Index,
                       typename Parent_ComponentIndex_TypeListOf_t<typename DimIndexTypeList::BodyTypeList>::T> T;
};

template <>
struct Parent_ComponentIndex_TypeListOf_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};

template <typename FactorTypeList, typename AbstractIndexTypeList>
struct DimIndexTypeListOf_t
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((FactorTypeList::LENGTH == AbstractIndexTypeList::LENGTH), MUST_HAVE_EQUAL_LENGTHS),
        STATIC_ASSERT_IN_ENUM(HasBasedVectorSpaceStructure_f<typename FactorTypeList::HeadType>::V, MUST_BE_BASED_VECTOR_SPACE)
    };
    typedef TypeList_t<DimIndex_t<AbstractIndexTypeList::HeadType::SYMBOL,FactorTypeList::HeadType::DIM>,
                       typename DimIndexTypeListOf_t<typename FactorTypeList::BodyTypeList,
                                                     typename AbstractIndexTypeList::BodyTypeList>::T> T;
};

template <>
struct DimIndexTypeListOf_t<EmptyTypeList,EmptyTypeList>
{
    typedef EmptyTypeList T;
};

template <typename TypeList>
struct EachTypeIsADimIndex_f
{
    static bool const V = IsADimIndex_f<typename TypeList::HeadType>::V &&
                          EachTypeIsADimIndex_f<typename TypeList::BodyTypeList>::V;
};

template <>
struct EachTypeIsADimIndex_f<EmptyTypeList>
{
    static bool const V = true;
};


template <typename DimIndexTypeList>
struct AbstractIndicesOfDimIndexTypeList_t
{
    enum { STATIC_ASSERT_IN_ENUM(EachTypeIsADimIndex_f<DimIndexTypeList>::V, MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES) };
    typedef TypeList_t<AbstractIndex_c<DimIndexTypeList::HeadType::SYMBOL>,
                       typename AbstractIndicesOfDimIndexTypeList_t<typename DimIndexTypeList::BodyTypeList>::T> T;
};

template <>
struct AbstractIndicesOfDimIndexTypeList_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};


} // end of namespace Tenh

#endif // TENH_DIMINDEX_HPP_
