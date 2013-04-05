// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/lvd.hpp by Victor Dods, created 2006/11/29
// Copyright Victor Dods, licensed for use by Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_META_LVD_HPP_
#define TENH_META_LVD_HPP_

//namespace Tenh {
namespace Lvd {
namespace Meta {

// ///////////////////////////////////////////////////////////////////////////
// general template metaprogramming stuff
// ///////////////////////////////////////////////////////////////////////////

template <bool condition_> struct Assert;
template <> struct Assert<true> { static bool const v = true; operator bool () const { return v; } };

template <typename T_> struct Type { typedef T_ T; };

template <typename T_, T_ value_> struct Value { typedef T_ T; static T_ const v = value_; operator T_ () const { return value_; } };

template <typename T0_, typename T1_> struct TypesAreEqual { static bool const v = false; operator bool () const { return v; } };
template <typename T_> struct TypesAreEqual<T_,T_> { static bool const v = true; operator bool () const { return v; } };

template <bool condition_, typename Then_, typename Else_> struct If;
template <typename Then_, typename Else_> struct If<true, Then_, Else_> { typedef Then_ T; };
template <typename Then_, typename Else_> struct If<false, Then_, Else_> { typedef Else_ T; };

} // end of namespace Meta

// ///////////////////////////////////////////////////////////////////////////
// setup for int type templates
// ///////////////////////////////////////////////////////////////////////////

// This series of macro calls creates a linked list of integer types via
// typedefs which will be used by Integer<> to typedef integers of specific
// bit-widths.  If the requested bit-width is not available, it will produce
// a compile error involving "struct NoSuchInteger".

#define LVD_HPP_INTEGER(Name, CurrentType, NextType) \
    struct NextType; \
    struct Name \
    { \
        static int const bit_size = 8*sizeof(CurrentType); \
        struct Current { typedef CurrentType Signed; typedef unsigned CurrentType Unsigned; }; \
        typedef NextType Next; \
    }

LVD_HPP_INTEGER(Char, char, Short);
LVD_HPP_INTEGER(Short, short, Int);
LVD_HPP_INTEGER(Int, int, Long);
LVD_HPP_INTEGER(Long, long, LongLong);
LVD_HPP_INTEGER(LongLong, long long, NoSuchInteger);

#undef LVD_HPP_INTEGER

template <unsigned int bit_size_, typename IntegerType_ = Char>
struct Integer
{
    typedef typename IntegerType_::Current Current;
    typedef typename IntegerType_::Next Next;

    typedef
        typename Meta::If<
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
// compile-time math stuff
// ///////////////////////////////////////////////////////////////////////////

namespace Meta {

// don't use PrivateLog<> -- use Log<> instead
template <unsigned long base_, unsigned long arg_> struct PrivateLog
{
    static unsigned long const v =
        If< (arg_ < base_),
            Value<unsigned long ,0>,
            Value<unsigned long ,1+PrivateLog<base_,(arg_/base_)>::v> >::T::v;
};
template <unsigned long base_> struct PrivateLog<base_,0> { static unsigned long const v = 0; };

template <unsigned long base_, unsigned long arg_> struct Log { static unsigned long const v = PrivateLog<base_,arg_>::v; };
template <unsigned long base_> struct Log<base_,0> { /* v is intentionally undefined because log(0) is undefined */ };
template <unsigned long arg_> struct Log<0,arg_> { /* v is intentionally undefined because the base-0 log is undefined */ };
template <unsigned long arg_> struct Log<1,arg_> { /* v is intentionally undefined because the base-1 log is undefined */ };

template <unsigned long base_, unsigned long exp_> struct Pow { static unsigned long const v = base_ * Pow<base_,exp_-1>::v; };
template <unsigned long base_> struct Pow<base_,0> { static unsigned long const v = 1; };
template <> struct Pow<0,0> { static unsigned long const v = 1; }; // "0^0 is indeed defined for integers" -Dr. Ted Nitz.

template <unsigned long base_, unsigned long arg_> struct LogRemainder { static unsigned long const v = arg_ - Pow<base_,(Log<base_,arg_>::v)>::v; };

template <unsigned long arg_, unsigned long base_> struct IsExactPowerOf
{
    static bool const v = LogRemainder<base_,arg_>::v == 0;
};
template <unsigned long arg_> struct IsExactPowerOf<arg_,0> { static bool const v = arg_ == 0; };
template <unsigned long arg_> struct IsExactPowerOf<arg_,1> { static bool const v = arg_ == 1; };

} // end of namespace Meta

// ///////////////////////////////////////////////////////////////////////////
// setup for binary number template types
// ///////////////////////////////////////////////////////////////////////////

template <Uint32 c_, Uint32 base_> struct IsInBinaryFormInBase
{
    static bool const v =
        Meta::If< (c_==0),
            Meta::Value<bool,true>,
            typename
            Meta::If< ((c_%base_)<=1),
                IsInBinaryFormInBase<(c_/base_),base_>,
                Meta::Value<bool,false>
            >::T
        >::T::v;
};

template <Uint32 c_, Uint32 base_, Uint32 m_ = 1> struct BinaryByteFromBase
{
    enum { no_overflow = Meta::Assert<(m_<=(1<<8))>::v };
    enum { is_in_binary_form_in_base = Meta::Assert<(IsInBinaryFormInBase<c_,base_>::v)>::v };
    static Uint8 const v =
        ((c_%base_)*m_)
        |
        Meta::If< (c_==0),
            Meta::Value<Uint32,0>,
            BinaryByteFromBase<(c_/base_),base_,(m_<<1)>
        >::T::v;
};

template <Uint32 c_, typename T_>
struct BinaryByte
{
    static T_ const v =
        Meta::If< IsInBinaryFormInBase<c_,8>::v,
            BinaryByteFromBase<c_,8>,
            typename
            Meta::If< IsInBinaryFormInBase<c_,16>::v,
                BinaryByteFromBase<c_,16>,
                BinaryByteFromBase<c_,10>
            >::T
        >::T::v;
};

// ///////////////////////////////////////////////////////////////////////////
// binary number templates
// ///////////////////////////////////////////////////////////////////////////

template <Uint32 c0_>
struct Bin8
{
    static Uint8 const u =
        BinaryByte<c0_,Uint8>::v << 0;
    static Sint8 const s = u;
};

template <Uint32 c1_, Uint32 c0_>
struct Bin16
{
    static Uint16 const u =
        BinaryByte<c1_,Uint16>::v << 8 |
        BinaryByte<c0_,Uint16>::v << 0;
    static Sint16 const s = u;
};

template <Uint32 c2_, Uint32 c1_, Uint32 c0_>
struct Bin24
{
    static Uint32 const u =
        BinaryByte<c2_,Uint32>::v << 16 |
        BinaryByte<c1_,Uint32>::v <<  8 |
        BinaryByte<c0_,Uint32>::v <<  0;
    static Sint32 const s = u;
};

template <Uint32 c3_, Uint32 c2_, Uint32 c1_, Uint32 c0_>
struct Bin32
{
    static Uint32 const u =
        BinaryByte<c3_,Uint32>::v << 24 |
        BinaryByte<c2_,Uint32>::v << 16 |
        BinaryByte<c1_,Uint32>::v <<  8 |
        BinaryByte<c0_,Uint32>::v <<  0;
    static Sint32 const s = u;
};

template <Uint32 c4_, Uint32 c3_, Uint32 c2_, Uint32 c1_, Uint32 c0_>
struct Bin40
{
    static Uint64 const u =
        BinaryByte<c4_,Uint64>::v << 32 |
        BinaryByte<c3_,Uint64>::v << 24 |
        BinaryByte<c2_,Uint64>::v << 16 |
        BinaryByte<c1_,Uint64>::v <<  8 |
        BinaryByte<c0_,Uint64>::v <<  0;
    static Sint64 const s = u;
};

template <Uint32 c5_, Uint32 c4_, Uint32 c3_, Uint32 c2_, Uint32 c1_, Uint32 c0_>
struct Bin48
{
    static Uint64 const u =
        BinaryByte<c5_,Uint64>::v << 40 |
        BinaryByte<c4_,Uint64>::v << 32 |
        BinaryByte<c3_,Uint64>::v << 24 |
        BinaryByte<c2_,Uint64>::v << 16 |
        BinaryByte<c1_,Uint64>::v <<  8 |
        BinaryByte<c0_,Uint64>::v <<  0;
    static Sint64 const s = u;
};

template <Uint32 c6_, Uint32 c5_, Uint32 c4_, Uint32 c3_, Uint32 c2_, Uint32 c1_, Uint32 c0_>
struct Bin56
{
    static Uint64 const u =
        BinaryByte<c6_,Uint64>::v << 48 |
        BinaryByte<c5_,Uint64>::v << 40 |
        BinaryByte<c4_,Uint64>::v << 32 |
        BinaryByte<c3_,Uint64>::v << 24 |
        BinaryByte<c2_,Uint64>::v << 16 |
        BinaryByte<c1_,Uint64>::v <<  8 |
        BinaryByte<c0_,Uint64>::v <<  0;
    static Sint64 const s = u;
};

template <Uint32 c7_, Uint32 c6_, Uint32 c5_, Uint32 c4_, Uint32 c3_, Uint32 c2_, Uint32 c1_, Uint32 c0_>
struct Bin64
{
    static Uint64 const u =
        BinaryByte<c7_,Uint64>::v << 56 |
        BinaryByte<c6_,Uint64>::v << 48 |
        BinaryByte<c5_,Uint64>::v << 40 |
        BinaryByte<c4_,Uint64>::v << 32 |
        BinaryByte<c3_,Uint64>::v << 24 |
        BinaryByte<c2_,Uint64>::v << 16 |
        BinaryByte<c1_,Uint64>::v <<  8 |
        BinaryByte<c0_,Uint64>::v <<  0;
    static Sint64 const s = u;
};

// ///////////////////////////////////////////////////////////////////////////
// template types which contain properties of the types defined herein
// ///////////////////////////////////////////////////////////////////////////

template <typename T_>
struct Property;

template <>
struct Property<Sint8>
{
    static Sint8 const min = Bin8<10000000>::s;
    static Sint8 const max = Bin8<01111111>::s;
    static bool const is_signed = true;
};

template <>
struct Property<Uint8>
{
    static Uint8 const min = Bin8<00000000>::u;
    static Uint8 const max = Bin8<11111111>::u;
    static bool const is_signed = false;
};

template <>
struct Property<Sint16>
{
    static Sint16 const min = Bin16<10000000,00000000>::s;
    static Sint16 const max = Bin16<01111111,11111111>::s;
    static bool const is_signed = true;
};

template <>
struct Property<Uint16>
{
    static Uint16 const min = Bin16<00000000,00000000>::u;
    static Uint16 const max = Bin16<11111111,11111111>::u;
    static bool const is_signed = false;
};

template <>
struct Property<Sint32>
{
    static Sint32 const min = Bin32<10000000,00000000,00000000,00000000>::s;
    static Sint32 const max = Bin32<01111111,11111111,11111111,11111111>::s;
    static bool const is_signed = true;
};

template <>
struct Property<Uint32>
{
    static Uint32 const min = Bin32<00000000,00000000,00000000,00000000>::u;
    static Uint32 const max = Bin32<11111111,11111111,11111111,11111111>::u;
    static bool const is_signed = false;
};

template <>
struct Property<Sint64>
{
    static Sint64 const min = Bin64<10000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000>::s;
    static Sint64 const max = Bin64<01111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111>::s;
    static bool const is_signed = true;
};

template <>
struct Property<Uint64>
{
    static Uint64 const min = Bin64<00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000>::u;
    static Uint64 const max = Bin64<11111111,11111111,11111111,11111111,11111111,11111111,11111111,11111111>::u;
    static bool const is_signed = false;
};

// ///////////////////////////////////////////////////////////////////////////
// compile-time asserts for the above code (only basic asserts for BinXX)
// ///////////////////////////////////////////////////////////////////////////

enum
{
    LVD_HPP_COMPILE_TIME_ASSERTS =
        // basic asserts for template metaprogramming
        Meta::Assert<true>::v &&
        Meta::Assert<Meta::Assert<true>::v>::v &&

        Meta::Assert<Meta::Value<int,0>::v == 0>::v &&
        Meta::Assert<Meta::Value<int,1>::v == 1>::v &&

        Meta::Assert<Meta::TypesAreEqual<int,int>::v>::v &&
        Meta::Assert<!Meta::TypesAreEqual<int,float>::v>::v &&

        Meta::Assert<Meta::TypesAreEqual<Meta::Type<int>::T,int>::v>::v &&
        Meta::Assert<!Meta::TypesAreEqual<Meta::Type<int>,int>::v>::v &&

        Meta::Assert<Meta::TypesAreEqual<int,signed>::v>::v &&

        Meta::Assert<Meta::TypesAreEqual<int*,int*>::v>::v &&
        Meta::Assert<!Meta::TypesAreEqual<int*,int>::v>::v &&
        Meta::Assert<!Meta::TypesAreEqual<int*,float*>::v>::v &&

        Meta::Assert<Meta::If<true,Meta::Value<int,0>,Meta::Value<int,1> >::T::v == 0>::v &&
        Meta::Assert<Meta::If<false,Meta::Value<int,0>,Meta::Value<int,1> >::T::v == 1>::v &&

        // math asserts
        Meta::Assert<(Meta::Log<2,0x01>::v == 0)>::v &&
        Meta::Assert<(Meta::Log<2,0x02>::v == 1)>::v &&
        Meta::Assert<(Meta::Log<2,0x03>::v == 1)>::v &&
        Meta::Assert<(Meta::Log<2,0x04>::v == 2)>::v &&
        Meta::Assert<(Meta::Log<2,0x05>::v == 2)>::v &&
        Meta::Assert<(Meta::Log<2,0x06>::v == 2)>::v &&
        Meta::Assert<(Meta::Log<2,0x07>::v == 2)>::v &&
        Meta::Assert<(Meta::Log<2,0x08>::v == 3)>::v &&
        Meta::Assert<(Meta::Log<2,0x09>::v == 3)>::v &&
        Meta::Assert<(Meta::Log<2,0x0A>::v == 3)>::v &&
        Meta::Assert<(Meta::Log<2,0x0B>::v == 3)>::v &&
        Meta::Assert<(Meta::Log<2,0x0C>::v == 3)>::v &&
        Meta::Assert<(Meta::Log<2,0x0D>::v == 3)>::v &&
        Meta::Assert<(Meta::Log<2,0x0E>::v == 3)>::v &&
        Meta::Assert<(Meta::Log<2,0x0F>::v == 3)>::v &&
        Meta::Assert<(Meta::Log<2,0x10>::v == 4)>::v &&
        Meta::Assert<(Meta::Log<2,static_cast<unsigned long>(-1)>::v == 8*sizeof(unsigned long)-1)>::v &&

        Meta::Assert<(Meta::Log<3,0x01>::v == 0)>::v &&
        Meta::Assert<(Meta::Log<3,0x02>::v == 0)>::v &&
        Meta::Assert<(Meta::Log<3,0x03>::v == 1)>::v &&
        Meta::Assert<(Meta::Log<3,0x04>::v == 1)>::v &&
        Meta::Assert<(Meta::Log<3,0x05>::v == 1)>::v &&
        Meta::Assert<(Meta::Log<3,0x06>::v == 1)>::v &&
        Meta::Assert<(Meta::Log<3,0x07>::v == 1)>::v &&
        Meta::Assert<(Meta::Log<3,0x08>::v == 1)>::v &&
        Meta::Assert<(Meta::Log<3,0x09>::v == 2)>::v &&

        Meta::Assert<(Meta::Pow<2,0>::v ==   1)>::v &&
        Meta::Assert<(Meta::Pow<2,1>::v ==   2)>::v &&
        Meta::Assert<(Meta::Pow<2,2>::v ==   4)>::v &&
        Meta::Assert<(Meta::Pow<2,3>::v ==   8)>::v &&
        Meta::Assert<(Meta::Pow<2,4>::v ==  16)>::v &&
        Meta::Assert<(Meta::Pow<2,5>::v ==  32)>::v &&
        Meta::Assert<(Meta::Pow<2,6>::v ==  64)>::v &&
        Meta::Assert<(Meta::Pow<2,7>::v == 128)>::v &&
        Meta::Assert<(Meta::Pow<2,8>::v == 256)>::v &&

        Meta::Assert<(Meta::Pow<3,0>::v ==   1)>::v &&
        Meta::Assert<(Meta::Pow<3,1>::v ==   3)>::v &&
        Meta::Assert<(Meta::Pow<3,2>::v ==   9)>::v &&
        Meta::Assert<(Meta::Pow<3,3>::v ==  27)>::v &&
        Meta::Assert<(Meta::Pow<3,4>::v ==  81)>::v &&

        Meta::Assert<(Meta::LogRemainder<2, 1>::v == 0)>::v &&
        Meta::Assert<(Meta::LogRemainder<2, 2>::v == 0)>::v &&
        Meta::Assert<(Meta::LogRemainder<2, 3>::v == 1)>::v &&
        Meta::Assert<(Meta::LogRemainder<2, 4>::v == 0)>::v &&
        Meta::Assert<(Meta::LogRemainder<2, 5>::v == 1)>::v &&
        Meta::Assert<(Meta::LogRemainder<2, 6>::v == 2)>::v &&
        Meta::Assert<(Meta::LogRemainder<2, 7>::v == 3)>::v &&
        Meta::Assert<(Meta::LogRemainder<2, 8>::v == 0)>::v &&
        Meta::Assert<(Meta::LogRemainder<2, 9>::v == 1)>::v &&
        Meta::Assert<(Meta::LogRemainder<2,10>::v == 2)>::v &&
        Meta::Assert<(Meta::LogRemainder<2,11>::v == 3)>::v &&
        Meta::Assert<(Meta::LogRemainder<2,12>::v == 4)>::v &&
        Meta::Assert<(Meta::LogRemainder<2,13>::v == 5)>::v &&
        Meta::Assert<(Meta::LogRemainder<2,14>::v == 6)>::v &&
        Meta::Assert<(Meta::LogRemainder<2,15>::v == 7)>::v &&
        Meta::Assert<(Meta::LogRemainder<2,16>::v == 0)>::v &&

        Meta::Assert<(Meta::LogRemainder<3, 1>::v == 0)>::v &&
        Meta::Assert<(Meta::LogRemainder<3, 2>::v == 1)>::v &&
        Meta::Assert<(Meta::LogRemainder<3, 3>::v == 0)>::v &&
        Meta::Assert<(Meta::LogRemainder<3, 4>::v == 1)>::v &&
        Meta::Assert<(Meta::LogRemainder<3, 5>::v == 2)>::v &&
        Meta::Assert<(Meta::LogRemainder<3, 6>::v == 3)>::v &&
        Meta::Assert<(Meta::LogRemainder<3, 7>::v == 4)>::v &&
        Meta::Assert<(Meta::LogRemainder<3, 8>::v == 5)>::v &&
        Meta::Assert<(Meta::LogRemainder<3, 9>::v == 0)>::v &&
        Meta::Assert<(Meta::LogRemainder<3,10>::v == 1)>::v &&
        Meta::Assert<(Meta::LogRemainder<3,11>::v == 2)>::v &&
        Meta::Assert<(Meta::LogRemainder<3,12>::v == 3)>::v &&
        Meta::Assert<(Meta::LogRemainder<3,13>::v == 4)>::v &&
        Meta::Assert<(Meta::LogRemainder<3,14>::v == 5)>::v &&
        Meta::Assert<(Meta::LogRemainder<3,15>::v == 6)>::v &&
        Meta::Assert<(Meta::LogRemainder<3,16>::v == 7)>::v &&

        Meta::Assert<( Meta::IsExactPowerOf<0,0>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<1,0>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<2,0>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<3,0>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<4,0>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<5,0>::v)>::v &&

        Meta::Assert<(!Meta::IsExactPowerOf<0,1>::v)>::v &&
        Meta::Assert<( Meta::IsExactPowerOf<1,1>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<2,1>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<3,1>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<4,1>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<5,1>::v)>::v &&

        Meta::Assert<(Meta::IsExactPowerOf<1,1>::v)>::v &&
        Meta::Assert<(Meta::IsExactPowerOf<1,2>::v)>::v &&
        Meta::Assert<(Meta::IsExactPowerOf<1,3>::v)>::v &&
        Meta::Assert<(Meta::IsExactPowerOf<1,4>::v)>::v &&

        Meta::Assert<( Meta::IsExactPowerOf<1,2>::v)>::v &&
        Meta::Assert<( Meta::IsExactPowerOf<2,2>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<3,2>::v)>::v &&
        Meta::Assert<( Meta::IsExactPowerOf<4,2>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<5,2>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<6,2>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<7,2>::v)>::v &&
        Meta::Assert<( Meta::IsExactPowerOf<8,2>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<9,2>::v)>::v &&

        Meta::Assert<( Meta::IsExactPowerOf<1,3>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<2,3>::v)>::v &&
        Meta::Assert<( Meta::IsExactPowerOf<3,3>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<4,3>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<5,3>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<6,3>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<7,3>::v)>::v &&
        Meta::Assert<(!Meta::IsExactPowerOf<8,3>::v)>::v &&
        Meta::Assert<( Meta::IsExactPowerOf<9,3>::v)>::v &&

        // type size checks
        Meta::Assert<sizeof(Diff) == sizeof(void *)>::v &&
        Meta::Assert<sizeof(Size) == sizeof(void *)>::v &&
        Meta::Assert<8*sizeof(Sint8) == 8>::v &&
        Meta::Assert<8*sizeof(Uint8) == 8>::v &&
        Meta::Assert<8*sizeof(Sint16) == 16>::v &&
        Meta::Assert<8*sizeof(Uint16) == 16>::v &&
        Meta::Assert<8*sizeof(Sint32) == 32>::v &&
        Meta::Assert<8*sizeof(Uint32) == 32>::v &&
        Meta::Assert<8*sizeof(Sint64) == 64>::v &&
        Meta::Assert<8*sizeof(Uint64) == 64>::v &&

        // unsigned 8-bit integers
        Meta::Assert<Bin8<00000000>::u == Uint8(0)>::v &&
        Meta::Assert<Bin8<11111111>::u == Uint8(-1)>::v &&
        Meta::Assert<Bin8<00000001>::u == (Uint8(1) << 0)>::v &&
        Meta::Assert<Bin8<00000010>::u == (Uint8(1) << 1)>::v &&
        Meta::Assert<Bin8<00000100>::u == (Uint8(1) << 2)>::v &&
        Meta::Assert<Bin8<00001000>::u == (Uint8(1) << 3)>::v &&
        Meta::Assert<Bin8<00010000>::u == (Uint8(1) << 4)>::v &&
        Meta::Assert<Bin8<00100000>::u == (Uint8(1) << 5)>::v &&
        Meta::Assert<Bin8<01000000>::u == (Uint8(1) << 6)>::v &&
        Meta::Assert<Bin8<10000000>::u == (Uint8(1) << 7)>::v &&

        // signed 8-bit integers
        Meta::Assert<Bin8<00000000>::s == Sint8(0)>::v &&
        Meta::Assert<Bin8<11111111>::s == Sint8(-1)>::v &&
        Meta::Assert<Bin8<00000001>::s == Sint8(Uint8(1) << 0)>::v &&
        Meta::Assert<Bin8<00000010>::s == Sint8(Uint8(1) << 1)>::v &&
        Meta::Assert<Bin8<00000100>::s == Sint8(Uint8(1) << 2)>::v &&
        Meta::Assert<Bin8<00001000>::s == Sint8(Uint8(1) << 3)>::v &&
        Meta::Assert<Bin8<00010000>::s == Sint8(Uint8(1) << 4)>::v &&
        Meta::Assert<Bin8<00100000>::s == Sint8(Uint8(1) << 5)>::v &&
        Meta::Assert<Bin8<01000000>::s == Sint8(Uint8(1) << 6)>::v &&
        Meta::Assert<Bin8<10000000>::s == Sint8(Uint8(1) << 7)>::v &&

        // unsigned 16-bit integers
        Meta::Assert<Bin16<00000000,00000000>::u == Uint16(0)>::v &&
        Meta::Assert<Bin16<11111111,11111111>::u == Uint16(-1)>::v &&
        Meta::Assert<Bin16<00000000,00000001>::u == (Uint16(1) << 0)>::v &&
        Meta::Assert<Bin16<00000000,00000010>::u == (Uint16(1) << 1)>::v &&
        Meta::Assert<Bin16<00000000,00000100>::u == (Uint16(1) << 2)>::v &&
        Meta::Assert<Bin16<00000000,00001000>::u == (Uint16(1) << 3)>::v &&
        Meta::Assert<Bin16<00000000,00010000>::u == (Uint16(1) << 4)>::v &&
        Meta::Assert<Bin16<00000000,00100000>::u == (Uint16(1) << 5)>::v &&
        Meta::Assert<Bin16<00000000,01000000>::u == (Uint16(1) << 6)>::v &&
        Meta::Assert<Bin16<00000000,10000000>::u == (Uint16(1) << 7)>::v &&
        Meta::Assert<Bin16<00000001,00000000>::u == (Uint16(1) << 8)>::v &&
        Meta::Assert<Bin16<00000010,00000000>::u == (Uint16(1) << 9)>::v &&
        Meta::Assert<Bin16<00000100,00000000>::u == (Uint16(1) << 10)>::v &&
        Meta::Assert<Bin16<00001000,00000000>::u == (Uint16(1) << 11)>::v &&
        Meta::Assert<Bin16<00010000,00000000>::u == (Uint16(1) << 12)>::v &&
        Meta::Assert<Bin16<00100000,00000000>::u == (Uint16(1) << 13)>::v &&
        Meta::Assert<Bin16<01000000,00000000>::u == (Uint16(1) << 14)>::v &&
        Meta::Assert<Bin16<10000000,00000000>::u == (Uint16(1) << 15)>::v &&

        // signed 16-bit integers
        Meta::Assert<Bin16<00000000,00000000>::s == Sint16(0)>::v &&
        Meta::Assert<Bin16<11111111,11111111>::s == Sint16(-1)>::v &&
        Meta::Assert<Bin16<00000000,00000001>::s == Sint16(Uint16(1) << 0)>::v &&
        Meta::Assert<Bin16<00000000,00000010>::s == Sint16(Uint16(1) << 1)>::v &&
        Meta::Assert<Bin16<00000000,00000100>::s == Sint16(Uint16(1) << 2)>::v &&
        Meta::Assert<Bin16<00000000,00001000>::s == Sint16(Uint16(1) << 3)>::v &&
        Meta::Assert<Bin16<00000000,00010000>::s == Sint16(Uint16(1) << 4)>::v &&
        Meta::Assert<Bin16<00000000,00100000>::s == Sint16(Uint16(1) << 5)>::v &&
        Meta::Assert<Bin16<00000000,01000000>::s == Sint16(Uint16(1) << 6)>::v &&
        Meta::Assert<Bin16<00000000,10000000>::s == Sint16(Uint16(1) << 7)>::v &&
        Meta::Assert<Bin16<00000001,00000000>::s == Sint16(Uint16(1) << 8)>::v &&
        Meta::Assert<Bin16<00000010,00000000>::s == Sint16(Uint16(1) << 9)>::v &&
        Meta::Assert<Bin16<00000100,00000000>::s == Sint16(Uint16(1) << 10)>::v &&
        Meta::Assert<Bin16<00001000,00000000>::s == Sint16(Uint16(1) << 11)>::v &&
        Meta::Assert<Bin16<00010000,00000000>::s == Sint16(Uint16(1) << 12)>::v &&
        Meta::Assert<Bin16<00100000,00000000>::s == Sint16(Uint16(1) << 13)>::v &&
        Meta::Assert<Bin16<01000000,00000000>::s == Sint16(Uint16(1) << 14)>::v &&
        Meta::Assert<Bin16<10000000,00000000>::s == Sint16(Uint16(1) << 15)>::v &&

        // asserts for Property<Diff> and Property<Size> (because
        // they're specified via Property<XintXX> already)
        Meta::Assert<Property<Diff>::min == Diff(~(Size(-1) >> 1))>::v &&
        Meta::Assert<Property<Diff>::max == Diff(Size(-1) >> 1)>::v &&
        Meta::Assert<Property<Size>::min == 0>::v &&
        Meta::Assert<Property<Size>::max == Size(-1)>::v
};

} // end of namespace Lvd
//} // end of namespace Tenh

#endif // !defined(TENH_META_LVD_HPP_)
