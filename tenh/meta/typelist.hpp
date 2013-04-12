// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typelist.hpp by Victor Dods, created 2013/02/16
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_META_TYPELIST_HPP_
#define TENH_META_TYPELIST_HPP_

#include <ostream>
#include <string>

#include "tenh/core.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

struct EmptyTypeList
{
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
};

template <typename HeadType_, typename BodyTypeList_ = EmptyTypeList>
struct TypeList_t
{
    typedef HeadType_ HeadType;
    typedef BodyTypeList_ BodyTypeList;
    static Uint32 const LENGTH = 1+BodyTypeList::LENGTH;

    static Uint32 length () { return LENGTH; }

    template <typename Type>
    struct Contains_t
    {
        static bool const V = Lvd::Meta::TypesAreEqual<HeadType,Type>::v || BodyTypeList::template Contains_t<Type>::V;
        operator bool () const { return V; }
    };

    template <Uint32 INDEX>
    struct El_t
    {
        enum { _ = STATIC_ASSERT_AS_RVALUE((INDEX < LENGTH), ATTEMPTED_ACCESS_PAST_LIST_END) }; // if this assert failed, you tried to access an element past the end of the list

        static Uint32 const I = (INDEX == 0) ? 0 : INDEX-1;
        typedef typename Lvd::Meta::If<(INDEX == 0), HeadType, typename BodyTypeList::template El_t<I>::T >::T T;
    };

    // returns the TypeList_t which ends at the INDEXth element
    template <Uint32 INDEX>
    struct LeadingTypeList_t
    {
    private:
        static Uint32 const I = (INDEX == 0) ? 0 : INDEX-1;
    public:
        typedef typename Lvd::Meta::If<(INDEX == 0), EmptyTypeList, TypeList_t<HeadType,typename BodyTypeList::template LeadingTypeList_t<I>::T> >::T T;
    };

    // returns the TypeList_t which starts at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingTypeList_t
    {
    private:
        static Uint32 const I = (INDEX == 0) ? 0 : INDEX-1;
    public:
        typedef typename Lvd::Meta::If<(INDEX == 0), TypeList_t, typename BodyTypeList::template TrailingTypeList_t<I>::T >::T T;
    };

    // returns the TypeList_t which is the range [START_INDEX, END_INDEX) (right endpoint not included)
    template <Uint32 START_INDEX, Uint32 END_INDEX>
    struct Range_t
    {
    private:
        enum { START_INDEX_MUST_NOT_EXCEED_END_INDEX = STATIC_ASSERT_AS_RVALUE((START_INDEX <= END_INDEX), START_INDEX_MUST_NOT_EXCEED_END_INDEX) };
        typedef typename TypeList_t::template TrailingTypeList_t<START_INDEX>::T LeadingRange;
    public:
        typedef typename LeadingRange::template LeadingTypeList_t<END_INDEX-START_INDEX>::T T;
    };

    template <typename Type>
    struct IndexOf_t
    {
        static Uint32 const V = Lvd::Meta::TypesAreEqual<Type,HeadType>::v ? 0 : (1 + BodyTypeList::template IndexOf_t<Type>::V);
        operator bool () const { return V; }
    };

    static std::string type_as_string (bool with_angle_brackets = true)
    {
        return std::string(with_angle_brackets ? "TypeList_t<" : "") +
               TypeStringOf_t<HeadType>::eval() +
               std::string(BodyTypeList::LENGTH > 0 ? "," : "") +
               BodyTypeList::type_as_string(false) + // no brackets, so it's not cluttered from nesting
               std::string(with_angle_brackets ? ">" : "");
    }
};

} // end of namespace Tenh

#endif // TENH_META_TYPELIST_HPP_
