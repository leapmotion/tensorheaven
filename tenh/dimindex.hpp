// ///////////////////////////////////////////////////////////////////////////
// tenh/dimindex.hpp by Victor Dods, created 2013/08/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////
/// @file dimindex.hpp
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
/// @brief Contains DimIndex_t and associated meta functions.
#ifndef TENH_DIMINDEX_HPP_
#define TENH_DIMINDEX_HPP_

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp"
#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

/// @brief This is really only for use inside expression template types, in order to construct MultiIndex_t types.
/// @tparam SYMBOL_ the symbol for the associated AbstractIndex_c index.
/// @tparam DIM_ the dimension for the associated ComponentIndex_t.
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
template <AbstractIndexSymbol SYMBOL_, Uint32 DIM_>
struct DimIndex_t
    :
    public ComponentIndex_t<DIM_>,
    public AbstractIndex_c<SYMBOL_> // NOTE: this should probably not be inherited in C++, but via ParentTypeList (i.e. through concepts)
{
    /// Accessor for parent type.
    typedef ComponentIndex_t<DIM_> Parent_ComponentIndex_t;
    /// Accessor for parent type.
    typedef AbstractIndex_c<SYMBOL_> Parent_AbstractIndex_c;

    using Parent_ComponentIndex_t::COMPONENT_COUNT;
    static Uint32 const DIM = Parent_ComponentIndex_t::COMPONENT_COUNT;
    using Parent_AbstractIndex_c::SYMBOL;

    DimIndex_t () : Parent_ComponentIndex_t(), Parent_AbstractIndex_c() { }
    explicit DimIndex_t (Uint32 i, bool check_range = CHECK_RANGE) : Parent_ComponentIndex_t(i, check_range) { }
    DimIndex_t (ComponentIndex_t<DIM> const &i) : Parent_ComponentIndex_t(i), Parent_AbstractIndex_c() { }

    /// For use with TypeStringOf_t.
    static std::string type_as_string () { return std::string("DimIndex_t<'") + SYMBOL + "'," + AS_STRING(DIM) + '>'; }
};

/// @brief Test to see if a type is a DimIndex_t.
/// @tparam T the type to test.
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
template <typename T> struct IsDimIndex_f { static bool const V = false; };
/// @cond false
template <AbstractIndexSymbol SYMBOL, Uint32 DIM> struct IsDimIndex_f<DimIndex_t<SYMBOL,DIM> > { static bool const V = true; };

template <AbstractIndexSymbol SYMBOL, Uint32 DIM> struct IsComponentIndex_f<DimIndex_t<SYMBOL,DIM> > { static bool const V = true; };
template <AbstractIndexSymbol SYMBOL, Uint32 DIM> struct IsAbstractIndex_f<DimIndex_t<SYMBOL,DIM> > { static bool const V = true; };
/// @endcond

/// @brief Predicate version of IsDimIndex_f.
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
struct IsDimIndex_p
{
    template <typename T>
    struct Eval_t
    {
        static bool const V = IsDimIndex_f<T>::V;
    };
};

/// @brief Takes a TypeList_t of DimIndex_t and returns a TypeList_t of the associated parent ComponentIndex_t.
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
template <typename DimIndexTypeList>
struct Parent_ComponentIndex_TypeListOf_t
{
    enum { STATIC_ASSERT_IN_ENUM(IsDimIndex_f<typename DimIndexTypeList::HeadType>::V, MUST_BE_DIM_INDEX) };
    typedef TypeList_t<typename DimIndexTypeList::HeadType::Parent_Index,
                       typename Parent_ComponentIndex_TypeListOf_t<typename DimIndexTypeList::BodyTypeList>::T> T;
};

/// @cond false
template <>
struct Parent_ComponentIndex_TypeListOf_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @brief Takes a TypeList_t of ComponentIndex_t and a TypeList_t of AbstractIndex_c and produces a TypeList_t of DimIndex_t.
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
template <typename FactorTypeList, typename AbstractIndexTypeList>
struct DimIndexTypeListOf_t
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((FactorTypeList::LENGTH == AbstractIndexTypeList::LENGTH), MUST_HAVE_EQUAL_LENGTHS),
        STATIC_ASSERT_IN_ENUM(HasBasedVectorSpaceStructure_f<typename FactorTypeList::HeadType>::V, MUST_BE_BASED_VECTOR_SPACE)
    };
    typedef TypeList_t<DimIndex_t<AbstractIndexTypeList::HeadType::SYMBOL,DimensionOf_f<typename FactorTypeList::HeadType >::V>,
                       typename DimIndexTypeListOf_t<typename FactorTypeList::BodyTypeList,
                                                     typename AbstractIndexTypeList::BodyTypeList>::T> T;
};

/// @cond false
template <>
struct DimIndexTypeListOf_t<EmptyTypeList,EmptyTypeList>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @brief Takes a TypeList_t of DimIndex_t and returns a TypeList_t of AbstractIndex_c.
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
template <typename DimIndexTypeList>
struct AbstractIndicesOfDimIndexTypeList_t
{
    enum { STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<DimIndexTypeList,IsDimIndex_p>::V), MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES) };
    typedef TypeList_t<AbstractIndex_c<DimIndexTypeList::HeadType::SYMBOL>,
                       typename AbstractIndicesOfDimIndexTypeList_t<typename DimIndexTypeList::BodyTypeList>::T> T;
};

/// @cond false
template <>
struct AbstractIndicesOfDimIndexTypeList_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};
/// @endcond

} // end of namespace Tenh

#endif // TENH_DIMINDEX_HPP_
