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

    template <typename Type>
    struct Contains_t
    {
        static bool const V = false;
        operator bool () const { return V; }
    };

    template <Uint32 INDEX>
    struct El_t
    {
        typedef NullType T;
    };

    // returns the TypeList_t which is the range [START_INDEX, END_INDEX) (right endpoint not included)
    template <Uint32 START_INDEX, Uint32 END_INDEX>
    struct Range_t
    {
        typedef EmptyTypeList T;
    };

    template <Uint32 INDEX>
    struct LeadingTypeList_t
    {
        typedef EmptyTypeList T;
    };

    template <Uint32 INDEX>
    struct TrailingTypeList_t
    {
        typedef EmptyTypeList T;
    };

    template <typename Type>
    struct IndexOf_t
    {
        static Uint32 const V = 0; // this is past the end of the indexing
        operator bool () const { return V; }
    };

    static std::string type_as_string (bool with_angle_brackets = true) { return with_angle_brackets ? "TypeList_t<>" : ""; }
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

    /// @brief Determine if this TypeList_t contains a given type.
    /// @headerfile typelist.hpp "tenh/meta/typelist.hpp"
    template <typename Type_>
    struct Contains_t
    {
        static bool const V = TypesAreEqual<HeadType_,Type_>::V || BodyTypeList::template Contains_t<Type_>::V;
        operator bool () const { return V; }
    };

    /// @brief Find the type at a given index into this TypeList_t.
    /// @headerfile typelist.hpp "tenh/meta/typelist.hpp"
    template <Uint32 INDEX_>
    struct El_t
    {
    private:
        enum { STATIC_ASSERT_IN_ENUM((INDEX_ < LENGTH), ATTEMPTED_ACCESS_PAST_LIST_END) };
        static Uint32 const I = (INDEX_ == 0) ? 0 : INDEX_-1;
    public:
        typedef typename If<(INDEX_ == 0), HeadType, typename BodyTypeList::template El_t<I>::T >::T T;
    };

    /// @brief Returns the TypeList_t which ends at the INDEXth element.
    /// @headerfile typelist.hpp "tenh/meta/typelist.hpp"
    template <Uint32 INDEX_>
    struct LeadingTypeList_t
    {
    private:
        static Uint32 const I = (INDEX_ == 0) ? 0 : INDEX_-1;
    public:
        typedef typename If<(INDEX_ == 0), EmptyTypeList, TypeList_t<HeadType,typename BodyTypeList::template LeadingTypeList_t<I>::T> >::T T;
    };

    /// @brief Returns the TypeList_t which starts at the INDEXth element.
    /// @headerfile typelist.hpp "tenh/meta/typelist.hpp"
    template <Uint32 INDEX_>
    struct TrailingTypeList_t
    {
    private:
        static Uint32 const I = (INDEX_ == 0) ? 0 : INDEX_-1;
    public:
        typedef typename If<(INDEX_ == 0), TypeList_t, typename BodyTypeList::template TrailingTypeList_t<I>::T >::T T;
    };

    /// @brief Returns the TypeList_t which is the range [START_INDEX_, END_INDEX_) (right endpoint not included).
    /// @headerfile typelist.hpp "tenh/meta/typelist.hpp"
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    struct Range_t
    {
    private:
        enum { STATIC_ASSERT_IN_ENUM((START_INDEX_ <= END_INDEX_), START_INDEX_MUST_NOT_EXCEED_END_INDEX) };
        typedef typename TypeList_t::template TrailingTypeList_t<START_INDEX_>::T LeadingRange;
    public:
        typedef typename LeadingRange::template LeadingTypeList_t<END_INDEX_-START_INDEX_>::T T;
    };

    /// @brief Returns the first index at which a type occurs.
    /// @headerfile typelist.hpp "tenh/meta/typelist.hpp"
    template <typename Type_>
    struct IndexOf_t
    {
        static Uint32 const V = TypesAreEqual<Type_,HeadType>::V ? 0 : (1 + BodyTypeList::template IndexOf_t<Type_>::V);
        operator Uint32 () const { return V; }
    };

    /// For use with TypeStringOf_t.
    static std::string type_as_string (bool with_angle_brackets = true)
    {
        return std::string(with_angle_brackets ? "TypeList_t<" : "") +
               type_string_of<HeadType>() +
               std::string(BodyTypeList::LENGTH > 0 ? "," : "") +
               BodyTypeList::type_as_string(false) + // no brackets, so it's not cluttered from nesting
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

} // end of namespace Tenh

#endif // TENH_META_TYPELIST_HPP_
