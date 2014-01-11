// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typelist.hpp by Victor Dods, created 2013/02/16
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////
/// @file typelist.hpp
/// @headerfile typelist.hpp "tenh/meta/typelist.hpp"
/// @brief Contains basic classes for creating compile type lists of types.
#ifndef TENH_META_TYPELIST_HPP_
#define TENH_META_TYPELIST_HPP_

#include <ostream>
#include <string>

#include "tenh/core.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

/// @struct EmptyTypeList typelist.hpp "tenh/meta/typelist.hpp"
/// @brief A type which acts like a TypeList_t which contains no types.
/// @details For detailed information on the members of EmptyTypeList see the documentation for TypeList_t
struct EmptyTypeList
{
    /// @cond false
    // there is no HeadType
    typedef EmptyTypeList BodyTypeList;
    static Uint32 const LENGTH = 0;

    static Uint32 length () { return LENGTH; } // this is necessary to avoid a linker error complaining about undefined LENGTH member

    static std::string type_as_string (bool verbose, bool with_angle_brackets = true) { return with_angle_brackets ? "TypeList_t<>" : ""; }
    /// @endcond
};


/// @struct TypeList_t typelist.hpp "tenh/meta/typelist.hpp"
/// @brief A compile time list of types, generated with tail recursion.
/// @tparam HeadType_ the first type in the list.
/// @tparam BodyTypeList_ a TypeList_t (or EmptyTypeList) of the remaining types.
template <typename HeadType_, typename BodyTypeList_ = EmptyTypeList>
struct TypeList_t
{
    /// Accessor for the first type in the list.
    typedef HeadType_ HeadType;
    /// Accessor for the body TypeList_t.
    typedef BodyTypeList_ BodyTypeList;
    /// The length of the typelist
    static Uint32 const LENGTH = 1+BodyTypeList::LENGTH;

    static Uint32 length () { return LENGTH; }

    /// For use with TypeStringOf_t.
    static std::string type_as_string (bool verbose, bool with_angle_brackets = true)
    {
        return std::string(with_angle_brackets ? "TypeList_t<" : "") +
               type_string_of<HeadType>() +
               std::string(BodyTypeList::LENGTH > 0 ? "," : "") +
               BodyTypeList::type_as_string(verbose, false) + // no brackets, so it's not cluttered from nesting
               std::string(with_angle_brackets ? ">" : "");
    }
};


/// @struct IsTypeList_f typelist.hpp "tenh/meta/typelist.hpp"
/// @brief Simple function to determine if a type is either a TypeList_t or an EmptyTypeList.
/// @tparam T the type to test.
template <typename T> struct IsTypeList_f { static bool const V = false; };
/// @cond false
template <typename HeadType, typename BodyTypeList> struct IsTypeList_f<TypeList_t<HeadType,BodyTypeList> > { static bool const V = IsTypeList_f<BodyTypeList>::V; };
template <typename HeadType> struct IsTypeList_f<TypeList_t<HeadType> > { static bool const V = true; };
template <> struct IsTypeList_f<EmptyTypeList> { static bool const V = true; };
/// @endcond

// ///////////////////////////////////////////////////////////////////////////
// operator overloads for creating instances of TypeList_t at runtime
// (e.g. abstract multiindices, like in expression templates)
// ///////////////////////////////////////////////////////////////////////////

template <typename FirstTypeList_, typename SecondTypeList_>
struct ConcatenationOfTypeLists_t;


template <typename LhsHeadType_, typename LhsBodyTypeList_, typename RhsHeadType_, typename RhsBodyTypeList_>
typename ConcatenationOfTypeLists_t<TypeList_t<LhsHeadType_,LhsBodyTypeList_>,TypeList_t<RhsHeadType_,RhsBodyTypeList_> >::T
    operator | (TypeList_t<LhsHeadType_,LhsBodyTypeList_> const &, TypeList_t<RhsHeadType_,RhsBodyTypeList_> const &)
{
    return typename ConcatenationOfTypeLists_t<TypeList_t<LhsHeadType_,LhsBodyTypeList_>,TypeList_t<RhsHeadType_,RhsBodyTypeList_> >::T();
}

template <typename RhsHeadType_, typename RhsBodyTypeList_>
TypeList_t<RhsHeadType_,RhsBodyTypeList_> operator | (EmptyTypeList const &, TypeList_t<RhsHeadType_,RhsBodyTypeList_> const &)
{
    return TypeList_t<RhsHeadType_,RhsBodyTypeList_>();
}

template <typename LhsHeadType_, typename LhsBodyTypeList_>
TypeList_t<LhsHeadType_,LhsBodyTypeList_> operator | (TypeList_t<LhsHeadType_,LhsBodyTypeList_> const &, EmptyTypeList const &)
{
    return TypeList_t<LhsHeadType_,LhsBodyTypeList_>();
}

inline EmptyTypeList operator | (EmptyTypeList const &, EmptyTypeList const &)
{
    return EmptyTypeList();
}

template <typename Lhs_, typename RhsHeadType_, typename RhsBodyTypeList_>
TypeList_t<Lhs_,TypeList_t<RhsHeadType_,RhsBodyTypeList_> > operator | (Lhs_ const &, TypeList_t<RhsHeadType_,RhsBodyTypeList_> const &)
{
    return TypeList_t<Lhs_,TypeList_t<RhsHeadType_,RhsBodyTypeList_> >();
}

template <typename LhsHeadType_, typename LhsBodyTypeList_, typename Rhs_>
typename ConcatenationOfTypeLists_t<TypeList_t<LhsHeadType_,LhsBodyTypeList_>,TypeList_t<Rhs_> >::T
    operator | (TypeList_t<LhsHeadType_,LhsBodyTypeList_> const &, Rhs_ const &)
{
    return typename ConcatenationOfTypeLists_t<TypeList_t<LhsHeadType_,LhsBodyTypeList_>,TypeList_t<Rhs_> >::T();
}

template <typename Rhs_>
TypeList_t<Rhs_> operator | (EmptyTypeList const &, Rhs_ const &)
{
    return TypeList_t<Rhs_>();
}

template <typename Lhs_>
TypeList_t<Lhs_> operator | (TypeList_t<Lhs_> const &, EmptyTypeList const &)
{
    return TypeList_t<Lhs_>();
}

} // end of namespace Tenh

#endif // TENH_META_TYPELIST_HPP_
