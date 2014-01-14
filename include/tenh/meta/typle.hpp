// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typle.hpp by Victor Dods, created 2014/01/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

/// @file typle.hpp
/// @headerfile typle.hpp "tenh/meta/typle.hpp"
/// @brief Contains basic classes for creating tuples of types.

#ifndef TENH_META_TYPLE_HPP_
#define TENH_META_TYPLE_HPP_

#include <ostream>
#include <string>

#include "tenh/core.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

/// @cond false
namespace Nonpublic {

template <typename... Body_>
struct CommaSeparatedStringOfVariadicTemplate_t;

template <>
struct CommaSeparatedStringOfVariadicTemplate_t<>
{
    static std::string eval () { return ""; }
};

template <typename Head_>
struct CommaSeparatedStringOfVariadicTemplate_t<Head_>
{
    static std::string eval () { return TypeStringOf_t<Head_>::eval(); }
};

template <typename Head_, typename... Body_>
struct CommaSeparatedStringOfVariadicTemplate_t<Head_,Body_...>
{
    static std::string eval () { return TypeStringOf_t<Head_>::eval() + ',' + CommaSeparatedStringOfVariadicTemplate_t<Body_...>::eval(); }
};

} // end of namespace Nonpublic
/// @endcond

/// @struct Typle_t typle.hpp "tenh/meta/typle.hpp"
/// @brief A compile time tuple of types, variadically specified, but "stored" using head/body-recursion.
/// @tparam Ts_... the comma-separated, ordered list of types.
template <typename... Ts_> struct Typle_t;

template <typename T_> struct Head_f;
template <typename T_> struct BodyTyple_f;
template <typename T_> struct Length_f;

MAKE_1_ARY_TYPE_EVALUATOR(Head);
MAKE_1_ARY_TYPE_EVALUATOR(BodyTyple);
MAKE_1_ARY_VALUE_EVALUATOR(Length, Uint32);

/// @cond false
template <>
struct Typle_t<>
{
    static Uint32 length () { return 0; }
};

template <typename Head_, typename... Body_>
struct Typle_t<Head_,Body_...>
{
    typedef Head_ Head;
    typedef Typle_t<Body_...> BodyTyple;

    static Uint32 length () { return Length_f<Typle_t>::V; }
};

template <typename... Ts_>
struct TypeStringOf_t<Typle_t<Ts_...>>
{
    static std::string eval () { return "Typle_t<" + Nonpublic::CommaSeparatedStringOfVariadicTemplate_t<Ts_...>::eval() + '>'; }
};
/// @endcond

/// @struct IsTyple_f typle.hpp "tenh/meta/typle.hpp"
/// @brief Simple function to determine if a type is a Typle_t<...>.
/// @tparam T the type to test.
template <typename T_> struct IsTyple_f { static bool const V = false; };
/// @cond false
template <typename... Ts_> struct IsTyple_f<Typle_t<Ts_...>> { static bool const V = true; };
/// @endcond



// NOTE: this stuff should move into a newly-created typle_utility.hpp

template <typename Head_, typename... Body_>
struct Head_f<Typle_t<Head_,Body_...>> { typedef Head_ T; };

template <typename Head_, typename... Body_>
struct BodyTyple_f<Typle_t<Head_,Body_...>> { typedef Typle_t<Body_...> T; };

template <>
struct Length_f<Typle_t<>> { static Uint32 const V = 0; };

template <typename Head_, typename... Body_>
struct Length_f<Typle_t<Head_,Body_...>> { static Uint32 const V = 1 + Length_f<Typle_t<Body_...>>::V; };

template <typename T0_, typename T1_> struct Concat2Typles_f;

// for recursively constructing Typle_t, as is done throughout Tenh.
template <typename Head_, typename BodyTyple_> struct HeadBodyTyple_f;

template <typename Head_, typename... BodyTypes_>
struct HeadBodyTyple_f<Head_,Typle_t<BodyTypes_...>>
{
    typedef Typle_t<Head_,BodyTypes_...> T;
};

// template <typename Head0_, typename... Body0_, typename Head1_, typename... Body1_>
template <typename... LhsTypes_, typename... RhsTypes_>
struct Concat2Typles_f<Typle_t<LhsTypes_...>,Typle_t<RhsTypes_...>>
{
    typedef Typle_t<LhsTypes_...,RhsTypes_...> T;
};

template <typename... Ts_> struct ConcatTyples_f;

template <> struct ConcatTyples_f<> { typedef Typle_t<> T; };

template <typename HeadTyple_, typename... BodyTyples_>
struct ConcatTyples_f<HeadTyple_,BodyTyples_...>
{
    static_assert(IsTyple_f<HeadTyple_>::V, "each argument must be a Typle_t");
    typedef typename Concat2Typles_f<HeadTyple_,typename ConcatTyples_f<BodyTyples_...>::T>::T T;
};

// for concatenating typles

template <typename... LhsTypes_, typename... RhsTypes_>
typename Concat2Typles_f<Typle_t<LhsTypes_...>,Typle_t<RhsTypes_...>>::T
    operator | (Typle_t<LhsTypes_...> const &, Typle_t<RhsTypes_...> const &)
{
    return typename Concat2Typles_f<Typle_t<LhsTypes_...>,Typle_t<RhsTypes_...>>::T();
}

template <typename Lhs_, typename... RhsTypes_>
Typle_t<Lhs_,RhsTypes_...> operator | (Lhs_ const &, Typle_t<RhsTypes_...> const &)
{
    return Typle_t<Lhs_,RhsTypes_...>();
}

template <typename Rhs_, typename... LhsTypes_>
typename Concat2Typles_f<Typle_t<LhsTypes_...>,Typle_t<Rhs_>>::T
    operator | (Typle_t<LhsTypes_...> const &, Rhs_ const &)
{
    return typename Concat2Typles_f<Typle_t<LhsTypes_...>,Typle_t<Rhs_>>::T();
}

// NOTE: this stuff should move into a newly-created (or changed) tuple.hpp
/*
template <typename Typle_> struct Tuple_t;

template <>
struct Tuple_t<Typle_t<>>
{
    Tuple_t () { }

    void print_guts (std::ostream &out) const { }
};

template <typename Head_>
struct Tuple_t<Typle_t<Head_>>
{
    Tuple_t (Head_ const &h) : m_head(h) { }

    void print_guts (std::ostream &out) const { out << m_head; }



private:

    Head_ m_head;
};

template <typename Head_, typename... Body_>
struct Tuple_t<Typle_t<Head_,Body_...>>
{
    typedef Tuple_t<Typle_t<Body_...>> BodyTuple;

    Tuple_t (Head_ const &h, BodyTuple const &b) : m_head(h), m_body_tuple(b) { }
    Tuple_t (Head_ const &h, Body_... body) : m_head(h), m_body_tuple(body...) { }

    void print_guts (std::ostream &out) const
    {
        out << m_head << ", ";
        m_body_tuple.print_guts(out);
    }

    Head_ m_head;
    BodyTuple m_body_tuple;
};

template <typename... Ts_>
Tuple_t<Typle_t<Ts_...>> tuple (Ts_... ts)
{
    return Tuple_t<Typle_t<Ts_...>>(ts...);
}

template <typename Typle_>
std::ostream &operator << (std::ostream &out, Tuple_t<Typle_> const &t)
{
//     out << "Tuple_t<" << type_string_of<Typle_>() << ">(";
    out << '(';
    t.print_guts(out);
    return out << ')';
}
*/

} // end of namespace Tenh

#endif // TENH_META_TYPLE_HPP_
