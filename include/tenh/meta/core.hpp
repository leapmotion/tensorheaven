// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/core.hpp by Victor Dods, created 2006/11/29
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

/// @file meta/core.hpp
/// @headerfile core.hpp "tenh/meta/core.hpp"
/// @brief Contains essential template metafunction building blocks.
#ifndef TENH_META_CORE_HPP_
#define TENH_META_CORE_HPP_

#include <type_traits>

namespace Tenh {

// a "raw" metafunction has a domain and codomain one of which is not a
// type, and whose evaluation consists of "dereferencing" a type T or a
// value V via the scope operator.  some examples are
//
// template <Uint64 N_> struct Square_f { static Uint64 const V = N_*N_; };
// template <typename Typle_> struct Length_f { static Uint64 const V = ...; };
// template <bool CONDITION_> struct FancyType_f { typedef ... T; };
//
// these raw metafunctions would be used as follows.
//   Square_f<36>::V
//   Length_f<Typle_t<...> >::V
//   FancyType_f<true>::T     or     typename FancyType_f<true>::T
//
// without knowing the domain/codomain, it's difficult to use this type
// of metafunction in a higher-order function.  thus leading to...
//
// a "primitive" metafunction is a metafunction whose domain and codomain
// are both types, and the evaluation of the function is done by dereferencing
// a type T.  This makes the interface regular.  An example is
//
// template <typename T_>
// struct DoubleOf_f
// {
//     typedef Typle_t<T_,T_> T;
// };
//
// the type can be a Value_t<ReturnType,VALUE> so that numeric-valued functions
// can be defined.  In that case, it is helpful to also define a
// "static ReturnType const V = VALUE" member so that extra dereferencing doesn't
// need to be done.  A primitive function can have multiple parameters.
//
// an "evaluator" is a structure that has a primitive metafunction called
// Eval_f inside.  it has the form:
//
// template <...>
// struct FunctionName_e
// {
//     template <typename Arg_> struct Eval_f { typedef ... T; };
// };
//
// then FunctionName_e<...> can be passed into another template as a parameter
// which essentially allows the use of higher-order functions.

// ///////////////////////////////////////////////////////////////////////////
// macros to make evaluator wrappers so that metafunctions can be passed
// in as template parameters like in any real functional language
// ///////////////////////////////////////////////////////////////////////////

// if there is a primitive metafunction, say
// template <typename Arg_> struct FunctionName_f { typedef ... T; };
// then MAKE_1_ARY_TYPE_EVALUATOR(FunctionName) will define an evaluator
// called FunctionName_e for it.

#define MAKE_1_ARY_TYPE_EVALUATOR(MetaFunctionName) \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        typedef typename MetaFunctionName##_f<T_>::T T; \
    }; \
}

// if there is a primitive metafunction, say
// template <typename Arg1_, Type2 Name2> struct FunctionName_f { typedef ... T; };
// then MAKE_2_ARY_TYPE_EVALUATOR(FunctionName, Type2, Name2) will define an
// evaluator called FunctionName_e<Type2 Name2> for it; its Eval_f only takes
// Arg1_ as a parameter -- the second parameter is specialized via the
// parameterization of FunctionName_e.

#define MAKE_2_ARY_TYPE_EVALUATOR(MetaFunctionName, Type2, Name2) \
template <Type2 Name2> \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        typedef typename MetaFunctionName##_f<T_,Name2>::T T; \
    }; \
}

// if there is a "primitive" metafunction, say
// template <typename Arg1_, Type2 Name2, Type3 Name3> struct FunctionName_f { typedef ... T; };
// then MAKE_2_ARY_TYPE_EVALUATOR(FunctionName, Type2, Name2, Type3, Name3) will define an
// evaluator called FunctionName_e<Type2 Name2, Type3 Name3> for it; its Eval_f only takes
// Arg1_ as a parameter -- the second and third parameters are specialized via the
// parameterization of FunctionName_e.

#define MAKE_3_ARY_TYPE_EVALUATOR(MetaFunctionName, Type2, Name2, Type3, Name3) \
template <Type2 Name2, Type3 Name3> \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        typedef typename MetaFunctionName##_f<T_,Name2,Name3>::T T; \
    }; \
}

// if there is a "primitive" metafunction, say
// template <typename Arg_> struct FunctionName_f { static ReturnType const V =  ...; };
// whose T type is of type Value_t<...> (e.g. Value_t<Uint32,123>), then
// MAKE_1_ARY_VALUE_EVALUATOR(FunctionName) will define an evaluator
// called FunctionName_e for it.  In this case, Eval_f also has a static
// const V value whose type is inferred from the metafunction.

#define MAKE_1_ARY_VALUE_EVALUATOR(MetaFunctionName) \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        typedef typename std::remove_const<decltype(MetaFunctionName##_f<T_>::V)>::type ReturnType; \
        static ReturnType const V = MetaFunctionName##_f<T_>::V; \
        typedef Tenh::Value_t<ReturnType,V> T; \
    }; \
}

// if there is a "primitive" metafunction, say
// template <typename Arg1_, Type2 Name2> struct FunctionName_f { static ReturnType const V =  ...; };
// whose T type is of type Value_t<...> (e.g. Value_t<Uint32,123>), then
// MAKE_2_ARY_VALUE_EVALUATOR(FunctionName, Type2, Name2) will define an
// evaluator called FunctionName_e<Type2 Name2> for it; its Eval_f only takes
// Arg1_ as a parameter -- the second parameter is specialized via the
// parameterization of FunctionName_e.  In this case, Eval_f also has a static
// const V value whose type is inferred from the metafunction

#define MAKE_2_ARY_VALUE_EVALUATOR(MetaFunctionName, Type2, Name2) \
template <Type2 Name2> \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        typedef typename std::remove_const<decltype(MetaFunctionName##_f<T_,Name2>::V)>::type ReturnType; \
        static ReturnType const V = MetaFunctionName##_f<T_,Name2>::V; \
        typedef Tenh::Value_t<ReturnType,V> T; \
    }; \
}

// if there is a "global" metafunction, say
// template <typename Arg1_, Type2 Name2, Type3 Name3> struct FunctionName_f { static ReturnType const V =  ...; };
// whose T type is of type Value_t<...> (e.g. Value_t<Uint32,123>), then
// MAKE_2_ARY_TYPE_EVALUATOR(FunctionName, Type2, Name2, Type3, Name3) will define an
// evaluator called FunctionName_e<Type2 Name2, Type3 Name3> for it; its Eval_f only takes
// Arg1_ as a parameter -- the second and third parameters are specialized via the
// parameterization of FunctionName_e.  In this case, Eval_f also has a static
// const V value whose type is inferred from the metafunction

#define MAKE_3_ARY_VALUE_EVALUATOR(MetaFunctionName, Type2, Name2, Type3, Name3) \
template <Type2 Name2, Type3 Name3> \
struct MetaFunctionName##_e \
{ \
    template <typename T_> \
    struct Eval_f \
    { \
        typedef typename std::remove_const<decltype(MetaFunctionName##_f<T_,Name2,Name3>::V)>::type ReturnType; \
        static ReturnType const V = MetaFunctionName##_f<T_,Name2,Name3>::V; \
        typedef Tenh::Value_t<ReturnType,V> T; \
    }; \
}

// the following are for making ordinary C++ functions that can be used in
// combination with the "decltype" keyword of C++11 to do template metaprogramming
// with drastically improved syntax.

#define MAKE_1_ARY_TYPE_FUNCTION_FOR_METAFUNCTION(function_name, MetafunctionName) \
template <typename T0_> \
typename MetafunctionName##_f<T0_>::T function_name (T0_ const &) \
{ \
    return typename MetafunctionName##_f<T0_>::T(); \
}

#define MAKE_2_ARY_TYPE_FUNCTION_FOR_METAFUNCTION(function_name, MetafunctionName) \
template <typename T0_, typename T1_> \
typename MetafunctionName##_f<T0_,T1_>::T function_name (T0_ const &, T1_ const &) \
{ \
    return typename MetafunctionName##_f<T0_,T1_>::T(); \
}

#define MAKE_3_ARY_TYPE_FUNCTION_FOR_METAFUNCTION(function_name, MetafunctionName) \
template <typename T0_, typename T1_, typename T2_> \
typename MetafunctionName##_f<T0_,T1_,T2_>::T function_name (T0_ const &, T1_ const &, T2_ const &) \
{ \
    return typename MetafunctionName##_f<T0_,T1_,T2_>::T(); \
}

#define MAKE_VARIADIC_TYPE_FUNCTION_FOR_METAFUNCTION(function_name, MetafunctionName) \
template <typename... Types_> \
typename MetafunctionName##_f<Types_...>::T function_name (Types_...) \
{ \
    return typename MetafunctionName##_f<Types_...>::T(); \
}

// ///////////////////////////////////////////////////////////////////////////
// general template metaprogramming stuff
// ///////////////////////////////////////////////////////////////////////////

/// @brief Compile time assertion struct, only used within tenh/meta/core.hpp.
/// @headerfile core.hpp "tenh/meta/core.hpp"
template <bool condition_> struct Assert;
/// @cond false
template <> struct Assert<true> { static bool const V = true; operator bool () const { return V; } };
/// @endcond

/// @brief Wrapper type to contain a compile time constant of any template value parameter type.
/// @headerfile core.hpp "tenh/meta/core.hpp"
/// @note The type string for this is defined in typestringof.hpp.
template <typename T_, T_ VALUE_> struct Value_t { typedef T_ T; static T_ const V = VALUE_; operator T_ () const { return VALUE_; } };

template <typename T_> struct IsValue_f { static bool const V = false; };
template <typename T_, T_ VALUE_> struct IsValue_f<Value_t<T_,VALUE_>> { static bool const V = true; };

MAKE_1_ARY_VALUE_EVALUATOR(IsValue);

/// @brief Sentinel "value" type.
/// @headerfile core.hpp "tenh/meta/core.hpp"
struct NullValue { };

/// @brief Wrapper type to contain a type, useful for "deferring" evaluation of metafunctions, e.g. in If_f.
/// @headerfile core.hpp "tenh/meta/core.hpp"
/// @note The type string for this is defined in typestringof.hpp.
template <typename T_> struct Type_t { typedef T_ T; };

template <typename T_> struct IsType_f { static bool const V = false; };
template <typename T_> struct IsType_f<Type_t<T_>> { static bool const V = true; };

MAKE_1_ARY_VALUE_EVALUATOR(IsType);

/// @brief Template metafunction to test whether two types are identical.
/// @headerfile core.hpp "tenh/meta/core.hpp"
template <typename T0_, typename T1_> struct TypesAreEqual_f { static bool const V = false; operator bool () const { return V; } };
/// @cond false
template <typename T_> struct TypesAreEqual_f<T_,T_> { static bool const V = true; operator bool () const { return V; } };
/// @endcond

MAKE_2_ARY_VALUE_EVALUATOR(TypesAreEqual, typename, T1_);

/// @brief Compile-time conditional. Contains one of two types depending upon the boolean passed in.
/// @headerfile core.hpp "tenh/meta/core.hpp"
template <bool condition_, typename Then_, typename Else_> struct If_f;
/// @cond false
template <typename Then_, typename Else_> struct If_f<true,Then_,Else_> { typedef Then_ T; };
template <typename Then_, typename Else_> struct If_f<false,Then_,Else_> { typedef Else_ T; };
/// @endcond

// ///////////////////////////////////////////////////////////////////////////
// setup for int type templates
// ///////////////////////////////////////////////////////////////////////////

/// This series of macro calls creates a linked list of integer types via
/// typedefs which will be used by Integer<> to typedef integers of specific
/// bit-widths.  If the requested bit-width is not available, it will produce
/// a compile error involving "struct NoSuchInteger".
#define TENH_META_CORE_HPP_INTEGER(Name, CurrentType, NextType) \
    struct NextType; \
    struct Name \
    { \
        static int const bit_size = 8*sizeof(CurrentType); \
        struct Current { typedef CurrentType Signed; typedef unsigned CurrentType Unsigned; }; \
        typedef NextType Next; \
    }

TENH_META_CORE_HPP_INTEGER(Char, char, Short);
TENH_META_CORE_HPP_INTEGER(Short, short, Int);
TENH_META_CORE_HPP_INTEGER(Int, int, Long);
TENH_META_CORE_HPP_INTEGER(Long, long, LongLong);
TENH_META_CORE_HPP_INTEGER(LongLong, long long, NoSuchInteger);

#undef TENH_META_CORE_HPP_INTEGER

/// @brief Uses the linked list built with TENH_META_CORE_HPP_INTEGER to find an integer
///  type with a specified bit width.
/// @headerfile core.hpp "tenh/meta/core.hpp"
template <unsigned int bit_size_, typename IntegerType_ = Char>
struct Integer
{
    typedef typename IntegerType_::Current Current;
    typedef typename IntegerType_::Next Next;

    typedef
        typename If_f<
            (bit_size_ == IntegerType_::bit_size),
            Current,
            Integer<bit_size_, Next>
        >::T T;
    typedef typename T::Signed Signed;
    typedef typename T::Unsigned Unsigned;
};

// ///////////////////////////////////////////////////////////////////////////
// pointer-sized integer types
// ///////////////////////////////////////////////////////////////////////////

typedef Integer<8*sizeof(void *)>::Signed Diff;
typedef Integer<8*sizeof(void *)>::Unsigned Size;

// ///////////////////////////////////////////////////////////////////////////
// specific bit-size integer types
// ///////////////////////////////////////////////////////////////////////////

typedef Integer<8>::Signed Sint8;
typedef Integer<8>::Unsigned Uint8;
typedef Integer<16>::Signed Sint16;
typedef Integer<16>::Unsigned Uint16;
typedef Integer<32>::Signed Sint32;
typedef Integer<32>::Unsigned Uint32;
typedef Integer<64>::Signed Sint64;
typedef Integer<64>::Unsigned Uint64;

// ///////////////////////////////////////////////////////////////////////////
// this is what std::numeric_limits should actually provide (constexpr-ish usage)
// ///////////////////////////////////////////////////////////////////////////

template <typename NumericType_> struct NumericBound_t;

template <> struct NumericBound_t<Sint8> { static Sint8 const MIN = Sint8(0x80); static Sint8 const MAX = Sint8(0x7F); };
template <> struct NumericBound_t<Uint8> { static Uint8 const MIN = Uint8(0x00); static Uint8 const MAX = Uint8(0xFF); };

template <> struct NumericBound_t<Sint16> { static Sint16 const MIN = Sint16(0x8000); static Sint16 const MAX = Sint16(0x7FFF); };
template <> struct NumericBound_t<Uint16> { static Uint16 const MIN = Uint16(0x0000); static Uint16 const MAX = Uint16(0xFFFF); };

template <> struct NumericBound_t<Sint32> { static Sint32 const MIN = Sint32(0x80000000); static Sint32 const MAX = Sint32(0x7FFFFFFF); };
template <> struct NumericBound_t<Uint32> { static Uint32 const MIN = Uint32(0x00000000); static Uint32 const MAX = Uint32(0xFFFFFFFF); };

template <> struct NumericBound_t<Sint64> { static Sint64 const MIN = Sint64(0x8000000000000000LL); static Sint64 const MAX = Sint64(0x7FFFFFFFFFFFFFFFLL); };
template <> struct NumericBound_t<Uint64> { static Uint64 const MIN = Uint64(0x0000000000000000ULL); static Uint64 const MAX = Uint64(0xFFFFFFFFFFFFFFFFULL); };

// ///////////////////////////////////////////////////////////////////////////
// compile-time asserts for the above code
// ///////////////////////////////////////////////////////////////////////////

/// Assertions to test the functionality of the code in this file.
enum
{
    TENH_META_CORE_HPP_COMPILE_TIME_ASSERTS =
        // basic asserts for template metaprogramming
        Assert<true>::V &&
        Assert<Assert<true>::V>::V &&

        Assert<Value_t<int,0>::V == 0>::V &&
        Assert<Value_t<int,1>::V == 1>::V &&

        Assert<TypesAreEqual_f<int,int>::V>::V &&
        Assert<!TypesAreEqual_f<int,float>::V>::V &&

        Assert<TypesAreEqual_f<int,signed>::V>::V &&

        Assert<TypesAreEqual_f<int*,int*>::V>::V &&
        Assert<!TypesAreEqual_f<int*,int>::V>::V &&
        Assert<!TypesAreEqual_f<int*,float*>::V>::V &&

        Assert<If_f<true,Value_t<int,0>,Value_t<int,1> >::T::V == 0>::V &&
        Assert<If_f<false,Value_t<int,0>,Value_t<int,1> >::T::V == 1>::V
};

} // end of namespace Tenh

#endif // !defined(TENH_META_CORE_HPP_)
