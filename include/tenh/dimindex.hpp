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
    public AbstractIndex_c<SYMBOL_> // NOTE: this should probably not be inherited in C++, but via ParentTyple (i.e. through concepts)
{
    /// Accessor for parent type.
    typedef ComponentIndex_t<DIM_> Parent_ComponentIndex_t;
    /// Accessor for parent type.
    typedef AbstractIndex_c<SYMBOL_> Parent_AbstractIndex_c;

    using Parent_ComponentIndex_t::COMPONENT_COUNT;
    static Uint32 const DIM = Parent_ComponentIndex_t::COMPONENT_COUNT;
    using Parent_AbstractIndex_c::SYMBOL;

    DimIndex_t () : Parent_ComponentIndex_t(), Parent_AbstractIndex_c() { }
    explicit DimIndex_t (Uint32 i, CheckRange check_range = CheckRange::TRUE) : Parent_ComponentIndex_t(i, check_range) { }
    DimIndex_t (ComponentIndex_t<DIM> const &i) : Parent_ComponentIndex_t(i), Parent_AbstractIndex_c() { }

    /// For use with TypeStringOf_t.
    static std::string type_as_string (bool verbose)
    {
        return std::string("DimIndex_t<") + abstract_index_symbol_as_string(SYMBOL) + ',' 
                                          + FORMAT(DIM) + '>';
    }
};

/// @brief Test to see if a type is a DimIndex_t.
/// @tparam T the type to test.
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
template <typename T> struct IsDimIndex_f
{
    static bool const V = false;
private:
    IsDimIndex_f();
};
/// @cond false
template <AbstractIndexSymbol SYMBOL, Uint32 DIM> struct IsDimIndex_f<DimIndex_t<SYMBOL,DIM>>
{
    static bool const V = true;
private:
    IsDimIndex_f();
};

template <AbstractIndexSymbol SYMBOL, Uint32 DIM> struct IsComponentIndex_f<DimIndex_t<SYMBOL,DIM>>
{
    static bool const V = true;
private:
    IsComponentIndex_f();
};
template <AbstractIndexSymbol SYMBOL, Uint32 DIM> struct IsAbstractIndex_f<DimIndex_t<SYMBOL,DIM>>
{
    static bool const V = true;
private:
    IsAbstractIndex_f();
};
/// @endcond

MAKE_1_ARY_VALUE_EVALUATOR(IsDimIndex);

// BaseProperty_f accessors

template <AbstractIndexSymbol SYMBOL_, Uint32 DIM_>
struct BaseProperty_f<DimIndex_t<SYMBOL_,DIM_>,Symbol>
{
    typedef Value_t<AbstractIndexSymbol,SYMBOL_> T;
};

/// @brief Takes a Typle_t of DimIndex_t and returns a Typle_t of the associated parent ComponentIndex_t.
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
template <typename DimIndexTyple_>
struct Parent_ComponentIndex_TypleOf_f
{
    static_assert(IsDimIndex_f<typename Head_f<DimIndexTyple_>::T>::V, "DimIndexTyple_ must contain only DimIndex_t types");
    typedef typename HeadBodyTyple_f<typename Head_f<DimIndexTyple_>::T::Parent_Index,
                                     typename Parent_ComponentIndex_TypleOf_f<typename BodyTyple_f<DimIndexTyple_>::T>::T>::T T;
private:
    Parent_ComponentIndex_TypleOf_f();
};

/// @cond false
template <>
struct Parent_ComponentIndex_TypleOf_f<Typle_t<>>
{
    typedef Typle_t<> T;
private:
    Parent_ComponentIndex_TypleOf_f();
};
/// @endcond


/// @brief Takes a Typle_t of ComponentIndex_t and a Typle_t of AbstractIndex_c and produces a Typle_t of DimIndex_t.
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
// TODO: implement this using some sort of compound construction
template <typename FactorTyple_, typename AbstractIndexTyple_>
struct DimIndexTypleOf_f
{
    static_assert(Length_f<FactorTyple_>::V == Length_f<AbstractIndexTyple_>::V, "FactorTyple_ and AbstractIndexTyple_ must have equal lengths");
    static_assert(HasBasedVectorSpaceStructure_f<typename Head_f<FactorTyple_>::T>::V, "FactorTyple_ must contain types having based vector spaces structures");
private:
    DimIndexTypleOf_f();
    typedef DimIndex_t<Head_f<AbstractIndexTyple_>::T::SYMBOL,
                       DimensionOf_f<typename Head_f<FactorTyple_>::T>::V> HeadDimIndex;
public:
    typedef typename HeadBodyTyple_f<HeadDimIndex,
                                     typename DimIndexTypleOf_f<typename BodyTyple_f<FactorTyple_>::T,
                                                                typename BodyTyple_f<AbstractIndexTyple_>::T>::T>::T T;
};

/// @cond false
template <>
struct DimIndexTypleOf_f<Typle_t<>,Typle_t<>>
{
    typedef Typle_t<> T;
private:
    DimIndexTypleOf_f();
};
/// @endcond


/// @brief Takes a Typle_t of DimIndex_t and returns a Typle_t of AbstractIndex_c.
/// @headerfile dimindex.hpp "tenh/dimindex.hpp"
template <typename DimIndexTyple_>
struct AbstractIndicesOfDimIndexTyple_f
{
    static_assert(IsTyple_f<DimIndexTyple_>::V, "DimIndexTyple_ must be a Typle_t");
    static_assert(EachTypeSatisfies_f<DimIndexTyple_,IsDimIndex_e>::V, "each type must be a DimIndex_t");
    typedef typename HeadBodyTyple_f<AbstractIndex_c<Head_f<DimIndexTyple_>::T::SYMBOL>,
                                     typename AbstractIndicesOfDimIndexTyple_f<typename BodyTyple_f<DimIndexTyple_>::T>::T>::T T;
private:
    AbstractIndicesOfDimIndexTyple_f();
};

/// @cond false
template <>
struct AbstractIndicesOfDimIndexTyple_f<Typle_t<>>
{
    typedef Typle_t<> T;
private:
    AbstractIndicesOfDimIndexTyple_f();
};
/// @endcond

} // end of namespace Tenh

#endif // TENH_DIMINDEX_HPP_
