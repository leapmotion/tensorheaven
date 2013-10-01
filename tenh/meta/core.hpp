// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/core.hpp by Victor Dods, created 2006/11/29
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_META_CORE_HPP_
#define TENH_META_CORE_HPP_

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// general template metaprogramming stuff
// ///////////////////////////////////////////////////////////////////////////

template <bool condition_> struct Assert;
template <> struct Assert<true> { static bool const V = true; operator bool () const { return V; } };

// the type string for this is defined in typestringof.hpp
template <typename T_, T_ VALUE_> struct Value_t { typedef T_ T; static T_ const V = VALUE_; operator T_ () const { return VALUE_; } };

// sentinel "value" type
struct NullValue { };

template <typename T0_, typename T1_> struct TypesAreEqual_f { static bool const V = false; operator bool () const { return V; } };
template <typename T_> struct TypesAreEqual_f<T_,T_> { static bool const V = true; operator bool () const { return V; } };

template <bool condition_, typename Then_, typename Else_> struct If;
template <typename Then_, typename Else_> struct If<true, Then_, Else_> { typedef Then_ T; };
template <typename Then_, typename Else_> struct If<false, Then_, Else_> { typedef Else_ T; };

// ///////////////////////////////////////////////////////////////////////////
// setup for int type templates
// ///////////////////////////////////////////////////////////////////////////

// This series of macro calls creates a linked list of integer types via
// typedefs which will be used by Integer<> to typedef integers of specific
// bit-widths.  If the requested bit-width is not available, it will produce
// a compile error involving "struct NoSuchInteger".

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

template <unsigned int bit_size_, typename IntegerType_ = Char>
struct Integer
{
    typedef typename IntegerType_::Current Current;
    typedef typename IntegerType_::Next Next;

    typedef
        typename If<
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
// macros to make evaluator wrappers so that metafunctions can be passed
// in as template parameters like in any real functional language
// ///////////////////////////////////////////////////////////////////////////

#define MAKE_1_ARY_TYPE_EVALUATOR(MetaFunctionName) \
struct MetaFunctionName##_e \
{ \
    template <typename TypeList_> \
    struct Eval_f \
    { \
        typedef typename MetaFunctionName##_f<TypeList_>::T T; \
    }; \
}

#define MAKE_2_ARY_TYPE_EVALUATOR(MetaFunctionName, Type2, Name2) \
template <Type2 Name2> \
struct MetaFunctionName##_e \
{ \
    template <typename TypeList_> \
    struct Eval_f \
    { \
        typedef typename MetaFunctionName##_f<TypeList_,Name2>::T T; \
    }; \
}

#define MAKE_3_ARY_TYPE_EVALUATOR(MetaFunctionName, Type2, Name2, Type3, Name3) \
template <Type2 Name2, Type3 Name3> \
struct MetaFunctionName##_e \
{ \
    template <typename TypeList_> \
    struct Eval_f \
    { \
        typedef typename MetaFunctionName##_f<TypeList_,Name2,Name3>::T T; \
    }; \
}

#define MAKE_1_ARY_VALUE_EVALUATOR(MetaFunctionName, MetaFunctionValueType) \
struct MetaFunctionName##_e \
{ \
    template <typename TypeList_> \
    struct Eval_f \
    { \
        typedef Value_t<MetaFunctionValueType,MetaFunctionName##_f<TypeList_>::V> T; \
    }; \
}

#define MAKE_2_ARY_VALUE_EVALUATOR(MetaFunctionName, MetaFunctionValueType, Type2, Name2) \
template <Type2 Name2> \
struct MetaFunctionName##_e \
{ \
    template <typename TypeList_> \
    struct Eval_f \
    { \
        typedef Value_t<MetaFunctionValueType,MetaFunctionName##_f<TypeList_,Name2>::V> T; \
    }; \
}

#define MAKE_3_ARY_VALUE_EVALUATOR(MetaFunctionName, MetaFunctionValueType, Type2, Name2, Type3, Name3) \
template <Type2 Name2, Type3 Name3> \
struct MetaFunctionName##_e \
{ \
    template <typename TypeList_> \
    struct Eval_f \
    { \
        typedef Value_t<MetaFunctionValueType,MetaFunctionName##_f<TypeList_,Name2,Name3>::V> T; \
    }; \
}


// ///////////////////////////////////////////////////////////////////////////
// compile-time asserts for the above code (only basic asserts for BinXX)
// ///////////////////////////////////////////////////////////////////////////

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

        Assert<If<true,Value_t<int,0>,Value_t<int,1> >::T::V == 0>::V &&
        Assert<If<false,Value_t<int,0>,Value_t<int,1> >::T::V == 1>::V
};

} // end of namespace Tenh

#endif // !defined(TENH_META_CORE_HPP_)
