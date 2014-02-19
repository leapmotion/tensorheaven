// ///////////////////////////////////////////////////////////////////////////
// tenh/tuple.hpp by Victor Dods, created 2013/02/16
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TUPLE_HPP_
#define TENH_TUPLE_HPP_

#include <ostream>
#include <string>

#include "tenh/core.hpp"
#include "tenh/memberarray.hpp"
#include "tenh/meta/typle.hpp"
#include "tenh/meta/typle_utility.hpp"

namespace Tenh {

template <typename Typle_, Uint32 INDEX_> struct TupleElement_t;
template <typename Typle_, Uint32 INDEX_> struct TupleHelper_t;

// Tuple_t is effectively a tuple, but implemented recursively as a (headelement,bodytuple) pair.
template <typename Typle_>
struct Tuple_t
{
    static_assert(Length_f<Typle_>::V >= 2, "this definition should only be used for 2-typles and above");
    typedef Typle_ Typle;
    typedef typename Head_f<Typle_>::T HeadType;
    typedef typename BodyTyple_f<Typle_>::T BodyTyple;
    typedef Tuple_t<BodyTyple> BodyTuple;
    static Uint32 const LENGTH = Length_f<Typle_>::V;

    Tuple_t () { }
    Tuple_t (HeadType const &head, BodyTuple const &body) : m_head(head), m_body(body) { }
    template <typename... BodyTypes_>
    Tuple_t (HeadType const &head, BodyTypes_... body) : m_head(head), m_body(body...) { }

    // the following constructors initialize the first elements that are common to both
    // tuple types, doing default construction for all remaining elements, if any.
    Tuple_t (Tuple_t<Typle_t<>> const &) { } // default construction
    // construction from single-element leading tuples -- default constructor for body
    Tuple_t (Tuple_t<Typle_t<HeadType>> const &leading_tuple) : m_head(leading_tuple.head()) { }
    // construction from leading tuples having more than one element
    template <typename OtherHeadType_, typename... OtherBodyTypes_>
    Tuple_t (Tuple_t<Typle_t<OtherHeadType_,OtherBodyTypes_...>> const &leading_tuple)
        :
        m_head(leading_tuple.head()),
        m_body(leading_tuple.body())
    { }

    bool operator == (Tuple_t const &l) const { return head() == l.head() && body() == l.body(); }
    bool operator != (Tuple_t const &l) const { return head() != l.head() || body() != l.body(); }

    static Uint32 length () { return LENGTH; }

    HeadType const &head () const { return m_head; }
    HeadType &head () { return m_head; }
    BodyTuple const &body () const { return m_body; }
    BodyTuple &body () { return m_body; }

    MemberArray_t<HeadType,LENGTH> const &as_member_array () const
    {
        static_assert(TypleIsUniform_f<Typle_>::V, "Typle_ template argument must be a uniform Typle_t");
        return *reinterpret_cast<MemberArray_t<HeadType,LENGTH> const *>(this);
    }
    MemberArray_t<HeadType,LENGTH> &as_member_array ()
    {
        static_assert(TypleIsUniform_f<Typle_>::V, "Typle_ template argument must be a uniform Typle_t");
        return *reinterpret_cast<MemberArray_t<HeadType,LENGTH> *>(this);
    }

    // returns the type of the INDEXth element of this Tuple_t
    template <Uint32 INDEX_>
    struct Type_f
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the Tuple_t");
        typedef typename Element_f<Typle_,INDEX_>::T T;
    };

    // returns the INDEXth element of this Tuple_t
    template <Uint32 INDEX_>
    typename Type_f<INDEX_>::T const &el () const
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the Tuple_t");
        return TupleElement_t<Typle_,INDEX_>::el(*this);
    }
    template <Uint32 INDEX_>
    typename Type_f<INDEX_>::T &el ()
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the Tuple_t");
        return TupleElement_t<Typle_,INDEX_>::el(*this);
    }

    // returns the type of the Tuple_t having the specified range
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    struct RangeType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        typedef Tuple_t<typename TypleRange_f<Typle_,START_INDEX_,END_INDEX_>::T> T;
    };

    // returns the Tuple_t which is the given range of elements
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T const &range () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T const *>(&(trailing_tuple<START_INDEX_>()));
    }
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T &range ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T *>(&(trailing_tuple<START_INDEX_>()));
    }

    // returns the type of the leading Tuple_t ending at the END_INDEX_th element.
    // i.e. elements [0,END_INDEX_), where END_INDEX_ is excluded.
    template <Uint32 END_INDEX_>
    struct LeadingTupleType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        typedef Tuple_t<typename LeadingTyple_f<Typle_,END_INDEX_>::T> T;
    };

    // returns the leading Tuple_t ending at the INDEXth element.
    template <Uint32 END_INDEX_>
    typename LeadingTupleType_f<END_INDEX_>::T const &leading_tuple () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingTupleType_f<END_INDEX_>::T const *>(this);
    }
    template <Uint32 END_INDEX_>
    typename LeadingTupleType_f<END_INDEX_>::T &leading_tuple ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingTupleType_f<END_INDEX_>::T *>(this);
    }

    // returns the type of the trailing Tuple_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    struct TrailingTupleType_f
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        typedef Tuple_t<typename TrailingTyple_f<Typle_,START_INDEX_>::T> T;
    };

    // returns the trailing Tuple_t starting at the INDEXth element
    template <Uint32 START_INDEX_>
    typename TrailingTupleType_f<START_INDEX_>::T const &trailing_tuple () const
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        return TupleHelper_t<Typle_,START_INDEX_>::trailing_tuple(*this);
    };
    template <Uint32 START_INDEX_>
    typename TrailingTupleType_f<START_INDEX_>::T &trailing_tuple ()
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        return TupleHelper_t<Typle_,START_INDEX_>::trailing_tuple(*this);
    };

    bool is_layed_out_contiguously_in_memory () const
    {
        return reinterpret_cast<Uint8 const *>(&m_head) + sizeof(HeadType) == reinterpret_cast<Uint8 const *>(&m_body.head());
    }

    void print (std::ostream &out, bool print_parens = true) const
    {
        if (print_parens)
            out << '(';
        out << m_head << ", ";
        m_body.print(out, false);
        if (print_parens)
            out << ')';
    }

    static std::string type_as_string () { return "Tuple_t<" + type_string_of<Typle_>() + '>'; }

private:

    HeadType m_head;
    BodyTuple m_body;
};

// try not to actually construct a Tuple_t<Typle_t<>>, because it isn't guaranteed to take 0 memory
template <>
struct Tuple_t<Typle_t<>>
{
    typedef Typle_t<> Typle;
    typedef Typle_t<> BodyTyple;
    typedef Tuple_t<Typle_t<>> BodyTuple;
    static Uint32 const LENGTH = 0;

    Tuple_t () { }

    bool operator == (Tuple_t const &) const { return true; }  // there is only one of these, so it must be equal
    bool operator != (Tuple_t const &) const { return false; } // there is only one of these, so it can't be unequal

    static Uint32 length () { return LENGTH; }

    // there is no head, so there is no accessors for it.
    Tuple_t<Typle_t<>> const &body () const { return Static<Tuple_t<Typle_t<>>>::SINGLETON; }
    // the const_cast doesn't matter because an empty tuple has no state to modify.
    Tuple_t<Typle_t<>> &body () { return *const_cast<Tuple_t<Typle_t<>> *>(&Static<Tuple_t<Typle_t<>>>::SINGLETON); }

    MemberArray_t<NullType,0> const &as_member_array () const { return *reinterpret_cast<MemberArray_t<NullType,0> const *>(this); }
    MemberArray_t<NullType,0> &as_member_array () { return *reinterpret_cast<MemberArray_t<NullType,0> *>(this); }

    template <Uint32 INDEX_>
    struct Type_f
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the Tuple_t");
        typedef typename Element_f<Typle,INDEX_>::T T;
    };

    template <Uint32 INDEX_>
    typename Type_f<INDEX_>::T const &el () const
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the Tuple_t");
        return TupleElement_t<Typle,INDEX_>::value(*this);
    }
    template <Uint32 INDEX_>
    typename Type_f<INDEX_>::T &el ()
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the Tuple_t");
        return TupleElement_t<Typle,INDEX_>::value(*this);
    }

    // returns the type of the Tuple_t having the specified range
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    struct RangeType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        typedef Tuple_t<typename TypleRange_f<Typle,START_INDEX_,END_INDEX_>::T> T;
    };

    // returns the Tuple_t which is the given range of elements
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T const &range () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T const *>(&(trailing_tuple<START_INDEX_>()));
    }
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T &range ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T *>(&(trailing_tuple<START_INDEX_>()));
    }

    // returns the type of the leading Tuple_t ending at the END_INDEX_th element.
    // i.e. elements [0,END_INDEX_), where END_INDEX_ is excluded.
    template <Uint32 END_INDEX_>
    struct LeadingTupleType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        typedef Tuple_t<typename LeadingTyple_f<Typle,END_INDEX_>::T> T;
    };

    // returns the leading Tuple_t ending at the END_INDEX_th element.
    template <Uint32 END_INDEX_>
    typename LeadingTupleType_f<END_INDEX_>::T const &leading_tuple () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingTupleType_f<END_INDEX_>::T const *>(this);
    }
    template <Uint32 END_INDEX_>
    typename LeadingTupleType_f<END_INDEX_>::T &leading_tuple ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingTupleType_f<END_INDEX_>::T *>(this);
    }

    // returns the type of the trailing Tuple_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    struct TrailingTupleType_f
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        typedef Tuple_t<Typle_t<>> T;
    };

    // returns the trailing Tuple_t starting at the INDEXth element
    template <Uint32 START_INDEX_>
    typename TrailingTupleType_f<START_INDEX_>::T const &trailing_tuple () const
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        return TupleHelper_t<Typle,START_INDEX_>::trailing_tuple(*this);
    };
    template <Uint32 START_INDEX_>
    typename TrailingTupleType_f<START_INDEX_>::T &trailing_tuple ()
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        return TupleHelper_t<Typle,START_INDEX_>::trailing_tuple(*this);
    };

    bool is_layed_out_contiguously_in_memory () const { return true; } // there are no elements; vacuously true

    void print (std::ostream &out, bool print_parens = true) const
    {
        if (print_parens)
            out << "()";
    }

    static std::string type_as_string () { return "Tuple_t<" + type_string_of<Typle>() + '>'; }
};

// you can use this to access the static const singleton as Static<EmptyTuple>::SINGLETON
typedef Tuple_t<Typle_t<>> EmptyTuple;



// template specialization for a single-element tuple
template <typename HeadType_>
struct Tuple_t<Typle_t<HeadType_>>
{
    typedef HeadType_ HeadType;
    typedef Typle_t<HeadType> Typle;
    typedef Typle_t<> BodyTyple;
    typedef Tuple_t<Typle_t<>> BodyTuple;
    static Uint32 const LENGTH = 1;

    Tuple_t () { }
    Tuple_t (HeadType const &head) : m_head(head) { }

    // the following constructors initialize the first elements that are common to both
    // tuple types, doing default construction for all remaining elements, if any.
    Tuple_t (Tuple_t<Typle_t<>> const &) { } // default construction
    Tuple_t (Tuple_t const &tuple) : m_head(tuple.m_head) { }
    template <typename OtherHeadType_, typename... OtherBodyTypes_>
    Tuple_t (Tuple_t<Typle_t<OtherHeadType_,OtherBodyTypes_...>> const &leading_tuple)
        :
        m_head(leading_tuple.head())
    { }

    bool operator == (Tuple_t const &l) const { return head() == l.head(); }
    bool operator != (Tuple_t const &l) const { return head() != l.head(); }

    static Uint32 length () { return LENGTH; }

    HeadType const &head () const { return m_head; }
    HeadType &head () { return m_head; }
    Tuple_t<Typle_t<>> const &body () const { return Static<Tuple_t<Typle_t<>>>::SINGLETON; }
    // the const_cast doesn't matter because an empty tuple has no state to modify.
    Tuple_t<Typle_t<>> &body () { return *const_cast<Tuple_t<Typle_t<>> *>(&Static<Tuple_t<Typle_t<>>>::SINGLETON); }

    MemberArray_t<HeadType,1> const &as_member_array () const { return *reinterpret_cast<MemberArray_t<HeadType,1> const *>(this); }
    MemberArray_t<HeadType,1> &as_member_array () { return *reinterpret_cast<MemberArray_t<HeadType,1> *>(this); }

    // type cast operator for HeadType?

    template <Uint32 INDEX_>
    struct Type_f
    {
        typedef typename Element_f<Typle,INDEX_>::T T;
    };

    template <Uint32 INDEX_>
    HeadType const &el () const
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the Tuple_t");
        return m_head;
    }
    template <Uint32 INDEX_>
    HeadType &el ()
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the Tuple_t");
        return m_head;
    }

    // returns the type of the Tuple_t having the specified range
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    struct RangeType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        typedef Tuple_t<typename TypleRange_f<Typle,START_INDEX_,END_INDEX_>::T> T;
    };

    // returns the Tuple_t which is the given range of elements
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T const &range () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T const *>(&(trailing_tuple<START_INDEX_>()));
    }
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T &range ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T *>(&(trailing_tuple<START_INDEX_>()));
    }

    // returns the type of the leading Tuple_t ending at the END_INDEX_th element.
    // i.e. elements [0,END_INDEX_), where END_INDEX_ is excluded.
    template <Uint32 END_INDEX_>
    struct LeadingTupleType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        typedef Tuple_t<typename LeadingTyple_f<Typle,END_INDEX_>::T> T;
    };

    // returns the leading Tuple_t ending at the END_INDEX_th element.
    template <Uint32 END_INDEX_>
    typename LeadingTupleType_f<END_INDEX_>::T const &leading_tuple () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingTupleType_f<END_INDEX_>::T const *>(this);
    }
    template <Uint32 END_INDEX_>
    typename LeadingTupleType_f<END_INDEX_>::T &leading_tuple ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingTupleType_f<END_INDEX_>::T *>(this);
    }

    // returns the type of the trailing Tuple_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    struct TrailingTupleType_f
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        typedef Tuple_t<typename TrailingTyple_f<Typle,START_INDEX_>::T> T;
    };

    // returns the trailing Tuple_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    typename TrailingTupleType_f<START_INDEX_>::T const &trailing_tuple () const
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        return TupleHelper_t<Typle,START_INDEX_>::trailing_tuple(*this);
    };
    template <Uint32 START_INDEX_>
    typename TrailingTupleType_f<START_INDEX_>::T &trailing_tuple ()
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the Tuple_t");
        return TupleHelper_t<Typle,START_INDEX_>::trailing_tuple(*this);
    };

    bool is_layed_out_contiguously_in_memory () const { return true; } // there is only one element; trivially true

    void print (std::ostream &out, bool print_parens = true) const
    {
        if (print_parens)
            out << '(';
        out << head();
        if (print_parens)
            out << ')';
    }

    static std::string type_as_string () { return "Tuple_t<" + type_string_of<Typle>() + '>'; }

private:

    HeadType m_head;
};

template <typename Typle_>
std::ostream &operator << (std::ostream &out, Tuple_t<Typle_> const &l)
{
    l.print(out);
    return out;
}






template <typename Typle_, Uint32 INDEX_>
struct TupleElement_t
{
    static_assert(INDEX_ < Length_f<Typle_>::V, "attempted element access past the end of the Tuple_t");
    typedef Tuple_t<Typle_> Tuple;
    typedef typename Element_f<Typle_,INDEX_>::T ValueType;
    static ValueType const &el (Tuple const &tuple)
    {
        return TupleElement_t<typename BodyTyple_f<Typle_>::T,INDEX_-1>::el(tuple.body());
    }
    static ValueType &el (Tuple &tuple)
    {
        return TupleElement_t<typename BodyTyple_f<Typle_>::T,INDEX_-1>::el(tuple.body());
    }
};

template <typename Typle_>
struct TupleElement_t<Typle_,0>
{
    static_assert(0 < Length_f<Typle_>::V, "attempted element access past the end of the Tuple_t");
    typedef Tuple_t<Typle_> Tuple;
    typedef typename Element_f<Typle_,0>::T ValueType;
    static ValueType const &el (Tuple const &tuple) { return tuple.head(); }
    static ValueType &el (Tuple &tuple) { return tuple.head(); }
};

// for use in the el() and trailing_tuple() methods in Tuple_t
template <typename Typle_, Uint32 INDEX_>
struct TupleHelper_t
{
    static_assert(INDEX_ > 0, "attempted element access past the end of the Tuple_t");
    typedef Tuple_t<Typle_> Tuple;
    typedef Tuple_t<typename LeadingTyple_f<Typle_,INDEX_>::T> LeadingTupleType;
    typedef Tuple_t<typename TrailingTyple_f<Typle_,INDEX_>::T> TrailingTupleType;
    static TrailingTupleType const &trailing_tuple (Tuple const &tuple)
    {
        return TupleHelper_t<typename BodyTyple_f<Typle_>::T,INDEX_-1>::trailing_tuple(tuple.body());
    }
    static TrailingTupleType &trailing_tuple (Tuple &tuple)
    {
        return TupleHelper_t<typename BodyTyple_f<Typle_>::T,INDEX_-1>::trailing_tuple(tuple.body());
    }
};

template <typename Typle_>
struct TupleHelper_t<Typle_,0>
{
    typedef Tuple_t<Typle_> Tuple;
    typedef Tuple TrailingTupleType;
    static TrailingTupleType const &trailing_tuple (Tuple const &tuple) { return tuple; }
    static TrailingTupleType &trailing_tuple (Tuple &tuple) { return tuple; }
};





template <typename Type_, Uint32 LENGTH_>
struct UniformTuple_f
{
    typedef Tuple_t<typename UniformTyple_f<LENGTH_,Type_>::T> T;
private:
    UniformTuple_f();
};


// tack an element onto the beginning of a tuple (where the tuple is empty)
template <typename HeadType_>
inline Tuple_t<Typle_t<HeadType_>> operator >>= (HeadType_ const &head, Tuple_t<Typle_t<>> const &)
{
    return Tuple_t<Typle_t<HeadType_>>(head);
}

// tack an element onto the beginning of a tuple (catch-all case)
template <typename HeadType_, typename BodyTyple_>
inline Tuple_t<typename HeadBodyTyple_f<HeadType_,BodyTyple_>::T> operator >>= (HeadType_ const &head, Tuple_t<BodyTyple_> const &body)
{
    return Tuple_t<typename HeadBodyTyple_f<HeadType_,BodyTyple_>::T>(head, body);
}


// these can totally go away once Tuple_t is fully implemented.

// base case
inline EmptyTuple operator | (EmptyTuple const &, EmptyTuple const &)
{
    return EmptyTuple();
}

// base case
template <typename Typle_>
Tuple_t<Typle_> operator | (Tuple_t<Typle_> const &lhs, EmptyTuple const &)
{
    return lhs;
}

// base case
template <typename Typle_>
Tuple_t<Typle_> operator | (EmptyTuple const &, Tuple_t<Typle_> const &rhs)
{
    return rhs;
}

// this allows you to do stuff like
// tuple(1, 2, 3) | tuple(4, 5, 6) | tuple(7, 8, 9)
// to get the 9-tuple (1, 2, 3, 4, 5, 6, 7, 8, 9) without having to make a 9-tuple function explicitly.
template <typename Lhs_Typle_, typename Rhs_Typle_>
Tuple_t<typename Concat2Typles_f<Lhs_Typle_,Rhs_Typle_>::T>
    operator | (Tuple_t<Lhs_Typle_> const &lhs, Tuple_t<Rhs_Typle_> const &rhs)
{
    return Tuple_t<typename Concat2Typles_f<Lhs_Typle_,Rhs_Typle_>::T>(lhs.head(), lhs.body() | rhs);
}

// functions for creating tuples in a syntactically-nice way, where the
// tuple types are inferred.

template <typename... Types_>
Tuple_t<Typle_t<Types_...>> tuple (Types_... parameters)
{
    return Tuple_t<Typle_t<Types_...>>(parameters...);
}

// each parameter must have a coercion to UniformType_
template <typename UniformType_, typename... Types_>
typename UniformTuple_f<UniformType_,Length_f<Typle_t<Types_...>>::V>::T
    uniform_tuple (Types_... parameters)
{
    return typename UniformTuple_f<UniformType_,Length_f<Typle_t<Types_...>>::V>::T(parameters...);
}

} // end of namespace Tenh

#endif // TENH_TUPLE_HPP_
