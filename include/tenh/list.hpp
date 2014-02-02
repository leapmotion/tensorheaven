// ///////////////////////////////////////////////////////////////////////////
// tenh/list.hpp by Victor Dods, created 2013/02/16
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_LIST_HPP_
#define TENH_LIST_HPP_

#include <ostream>
#include <string>

#include "tenh/core.hpp"
#include "tenh/memberarray.hpp"
#include "tenh/meta/typle.hpp"
#include "tenh/meta/typle_utility.hpp"

namespace Tenh {

template <typename Typle_, Uint32 INDEX_> struct ListElement_t;
template <typename Typle_, Uint32 INDEX_> struct ListHelper_t;

// List_t is effectively a tuple, but implemented recursively as a (headelement,bodylist) pair.
template <typename Typle_>
struct List_t
{
    static_assert(Hippo::Length_f<Typle_>::V >= 2, "this definition should only be used for 2-typles and above");
    typedef Typle_ Typle;
    typedef typename Hippo::Head_f<Typle_>::T HeadType;
    typedef typename Hippo::BodyTyple_f<Typle_>::T BodyTyple;
    typedef List_t<BodyTyple> BodyList;
    static Uint32 const LENGTH = Hippo::Length_f<Typle_>::V;

    List_t () { }
    List_t (HeadType const &head, BodyList const &body) : m_head(head), m_body(body) { }
    template <typename... BodyTypes_>
    List_t (HeadType const &head, BodyTypes_... body) : m_head(head), m_body(body...) { }

    // the following constructors initialize the first elements that are common to both
    // list types, doing default construction for all remaining elements, if any.
    List_t (List_t<Typle_t<>> const &) { } // default construction
    // construction from single-element leading lists -- default constructor for body
    List_t (List_t<Typle_t<HeadType> > const &leading_list) : m_head(leading_list.head()) { }
    // construction from leading lists having more than one element
    template <typename OtherHeadType_, typename... OtherBodyTypes_>
    List_t (List_t<Typle_t<OtherHeadType_,OtherBodyTypes_...>> const &leading_list)
        :
        m_head(leading_list.head()),
        m_body(leading_list.body())
    { }

    bool operator == (List_t const &l) const { return head() == l.head() && body() == l.body(); }
    bool operator != (List_t const &l) const { return head() != l.head() || body() != l.body(); }

    static Uint32 length () { return LENGTH; }

    HeadType const &head () const { return m_head; }
    HeadType &head () { return m_head; }
    BodyList const &body () const { return m_body; }
    BodyList &body () { return m_body; }

    MemberArray_t<HeadType,LENGTH> const &as_member_array () const
    {
        static_assert(Hippo::TypleIsUniform_f<Typle_>::V, "Typle_ template argument must be a uniform Typle_t");
        return *reinterpret_cast<MemberArray_t<HeadType,LENGTH> const *>(this);
    }
    MemberArray_t<HeadType,LENGTH> &as_member_array ()
    {
        static_assert(Hippo::TypleIsUniform_f<Typle_>::V, "Typle_ template argument must be a uniform Typle_t");
        return *reinterpret_cast<MemberArray_t<HeadType,LENGTH> *>(this);
    }

    // returns the type of the INDEXth element of this List_t
    template <Uint32 INDEX_>
    struct Type_f
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the List_t");
        typedef typename Hippo::Element_f<Typle_,INDEX_>::T T;
    };

    // returns the INDEXth element of this List_t
    template <Uint32 INDEX_>
    typename Type_f<INDEX_>::T const &el () const
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the List_t");
        return ListElement_t<Typle_,INDEX_>::el(*this);
    }
    template <Uint32 INDEX_>
    typename Type_f<INDEX_>::T &el ()
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the List_t");
        return ListElement_t<Typle_,INDEX_>::el(*this);
    }

    // returns the type of the List_t having the specified range
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    struct RangeType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        typedef List_t<typename Hippo::TypleRange_f<Typle_,START_INDEX_,END_INDEX_>::T> T;
    };

    // returns the List_t which is the given range of elements
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T const &range () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T const *>(&(trailing_list<START_INDEX_>()));
    }
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T &range ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T *>(&(trailing_list<START_INDEX_>()));
    }

    // returns the type of the leading List_t ending at the END_INDEX_th element.
    // i.e. elements [0,END_INDEX_), where END_INDEX_ is excluded.
    template <Uint32 END_INDEX_>
    struct LeadingListType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        typedef List_t<typename Hippo::LeadingTyple_f<Typle_,END_INDEX_>::T> T;
    };

    // returns the leading List_t ending at the INDEXth element.
    template <Uint32 END_INDEX_>
    typename LeadingListType_f<END_INDEX_>::T const &leading_list () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingListType_f<END_INDEX_>::T const *>(this);
    }
    template <Uint32 END_INDEX_>
    typename LeadingListType_f<END_INDEX_>::T &leading_list ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingListType_f<END_INDEX_>::T *>(this);
    }

    // returns the type of the trailing List_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    struct TrailingListType_f
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        typedef List_t<typename Hippo::TrailingTyple_f<Typle_,START_INDEX_>::T> T;
    };

    // returns the trailing List_t starting at the INDEXth element
    template <Uint32 START_INDEX_>
    typename TrailingListType_f<START_INDEX_>::T const &trailing_list () const
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        return ListHelper_t<Typle_,START_INDEX_>::trailing_list(*this);
    };
    template <Uint32 START_INDEX_>
    typename TrailingListType_f<START_INDEX_>::T &trailing_list ()
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        return ListHelper_t<Typle_,START_INDEX_>::trailing_list(*this);
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

    static std::string type_as_string () { return "List_t<" + type_string_of<Typle_>() + '>'; }

private:

    HeadType m_head;
    BodyList m_body;
};

// try not to actually construct a List_t<Typle_t<>>, because it isn't guaranteed to take 0 memory
template <>
struct List_t<Typle_t<>>
{
    typedef Typle_t<> Typle;
    typedef Typle_t<> BodyTyple;
    typedef List_t<Typle_t<>> BodyList;
    static Uint32 const LENGTH = 0;

    List_t () { }

    bool operator == (List_t const &) const { return true; }  // there is only one of these, so it must be equal
    bool operator != (List_t const &) const { return false; } // there is only one of these, so it can't be unequal

    static Uint32 length () { return LENGTH; }

    // there is no head, so there is no accessors for it.
    List_t<Typle_t<>> const &body () const { return Static<List_t<Typle_t<>>>::SINGLETON; }
    // the const_cast doesn't matter because an empty list has no state to modify.
    List_t<Typle_t<>> &body () { return *const_cast<List_t<Typle_t<>> *>(&Static<List_t<Typle_t<>>>::SINGLETON); }

    MemberArray_t<NullType,0> const &as_member_array () const { return *reinterpret_cast<MemberArray_t<NullType,0> const *>(this); }
    MemberArray_t<NullType,0> &as_member_array () { return *reinterpret_cast<MemberArray_t<NullType,0> *>(this); }

    template <Uint32 INDEX_>
    struct Type_f
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the List_t");
        typedef typename Hippo::Element_f<Typle,INDEX_>::T T;
    };

    template <Uint32 INDEX_>
    typename Type_f<INDEX_>::T const &el () const
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the List_t");
        return ListElement_t<Typle,INDEX_>::value(*this);
    }
    template <Uint32 INDEX_>
    typename Type_f<INDEX_>::T &el ()
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the List_t");
        return ListElement_t<Typle,INDEX_>::value(*this);
    }

    // returns the type of the List_t having the specified range
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    struct RangeType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        typedef List_t<typename Hippo::TypleRange_f<Typle,START_INDEX_,END_INDEX_>::T> T;
    };

    // returns the List_t which is the given range of elements
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T const &range () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T const *>(&(trailing_list<START_INDEX_>()));
    }
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T &range ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T *>(&(trailing_list<START_INDEX_>()));
    }

    // returns the type of the leading List_t ending at the END_INDEX_th element.
    // i.e. elements [0,END_INDEX_), where END_INDEX_ is excluded.
    template <Uint32 END_INDEX_>
    struct LeadingListType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        typedef List_t<typename Hippo::LeadingTyple_f<Typle,END_INDEX_>::T> T;
    };

    // returns the leading List_t ending at the END_INDEX_th element.
    template <Uint32 END_INDEX_>
    typename LeadingListType_f<END_INDEX_>::T const &leading_list () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingListType_f<END_INDEX_>::T const *>(this);
    }
    template <Uint32 END_INDEX_>
    typename LeadingListType_f<END_INDEX_>::T &leading_list ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingListType_f<END_INDEX_>::T *>(this);
    }

    // returns the type of the trailing List_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    struct TrailingListType_f
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        typedef List_t<Typle_t<>> T;
    };

    // returns the trailing List_t starting at the INDEXth element
    template <Uint32 START_INDEX_>
    typename TrailingListType_f<START_INDEX_>::T const &trailing_list () const
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        return ListHelper_t<Typle,START_INDEX_>::trailing_list(*this);
    };
    template <Uint32 START_INDEX_>
    typename TrailingListType_f<START_INDEX_>::T &trailing_list ()
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        return ListHelper_t<Typle,START_INDEX_>::trailing_list(*this);
    };

    bool is_layed_out_contiguously_in_memory () const { return true; } // there are no elements; vacuously true

    void print (std::ostream &out, bool print_parens = true) const
    {
        if (print_parens)
            out << "()";
    }

    static std::string type_as_string () { return "List_t<" + type_string_of<Typle>() + '>'; }
};

// you can use this to access the static const singleton as Static<EmptyList>::SINGLETON
typedef List_t<Typle_t<>> EmptyList;



// template specialization for a single-element list
template <typename HeadType_>
struct List_t<Typle_t<HeadType_>>
{
    typedef HeadType_ HeadType;
    typedef Typle_t<HeadType> Typle;
    typedef Typle_t<> BodyTyple;
    typedef List_t<Typle_t<>> BodyList;
    static Uint32 const LENGTH = 1;

    List_t () { }
    List_t (HeadType const &head) : m_head(head) { }

    // the following constructors initialize the first elements that are common to both
    // list types, doing default construction for all remaining elements, if any.
    List_t (List_t<Typle_t<>> const &) { } // default construction
    List_t (List_t const &list) : m_head(list.m_head) { }
    template <typename OtherHeadType_, typename... OtherBodyTypes_>
    List_t (List_t<Typle_t<OtherHeadType_,OtherBodyTypes_...>> const &leading_list)
        :
        m_head(leading_list.head())
    { }

    bool operator == (List_t const &l) const { return head() == l.head(); }
    bool operator != (List_t const &l) const { return head() != l.head(); }

    static Uint32 length () { return LENGTH; }

    HeadType const &head () const { return m_head; }
    HeadType &head () { return m_head; }
    List_t<Typle_t<>> const &body () const { return Static<List_t<Typle_t<>>>::SINGLETON; }
    // the const_cast doesn't matter because an empty list has no state to modify.
    List_t<Typle_t<>> &body () { return *const_cast<List_t<Typle_t<>> *>(&Static<List_t<Typle_t<>>>::SINGLETON); }

    MemberArray_t<HeadType,1> const &as_member_array () const { return *reinterpret_cast<MemberArray_t<HeadType,1> const *>(this); }
    MemberArray_t<HeadType,1> &as_member_array () { return *reinterpret_cast<MemberArray_t<HeadType,1> *>(this); }

    // type cast operator for HeadType?

    template <Uint32 INDEX_>
    struct Type_f
    {
        typedef typename Hippo::Element_f<Typle,INDEX_>::T T;
    };

    template <Uint32 INDEX_>
    HeadType const &el () const
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the List_t");
        return m_head;
    }
    template <Uint32 INDEX_>
    HeadType &el ()
    {
        static_assert(INDEX_ < LENGTH, "attempted element access past the end of the List_t");
        return m_head;
    }

    // returns the type of the List_t having the specified range
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    struct RangeType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        typedef List_t<typename Hippo::TypleRange_f<Typle,START_INDEX_,END_INDEX_>::T> T;
    };

    // returns the List_t which is the given range of elements
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T const &range () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T const *>(&(trailing_list<START_INDEX_>()));
    }
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T &range ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T *>(&(trailing_list<START_INDEX_>()));
    }

    // returns the type of the leading List_t ending at the END_INDEX_th element.
    // i.e. elements [0,END_INDEX_), where END_INDEX_ is excluded.
    template <Uint32 END_INDEX_>
    struct LeadingListType_f
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        typedef List_t<typename Hippo::LeadingTyple_f<Typle,END_INDEX_>::T> T;
    };

    // returns the leading List_t ending at the END_INDEX_th element.
    template <Uint32 END_INDEX_>
    typename LeadingListType_f<END_INDEX_>::T const &leading_list () const
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingListType_f<END_INDEX_>::T const *>(this);
    }
    template <Uint32 END_INDEX_>
    typename LeadingListType_f<END_INDEX_>::T &leading_list ()
    {
        static_assert(END_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        // slightly yucky, but the elements are laid out in memory in a way that makes this totally valid.
        return *reinterpret_cast<typename LeadingListType_f<END_INDEX_>::T *>(this);
    }

    // returns the type of the trailing List_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    struct TrailingListType_f
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        typedef List_t<typename Hippo::TrailingTyple_f<Typle,START_INDEX_>::T> T;
    };

    // returns the trailing List_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    typename TrailingListType_f<START_INDEX_>::T const &trailing_list () const
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        return ListHelper_t<Typle,START_INDEX_>::trailing_list(*this);
    };
    template <Uint32 START_INDEX_>
    typename TrailingListType_f<START_INDEX_>::T &trailing_list ()
    {
        static_assert(START_INDEX_ <= LENGTH, "attempted element access past the end of the List_t");
        return ListHelper_t<Typle,START_INDEX_>::trailing_list(*this);
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

    static std::string type_as_string () { return "List_t<" + type_string_of<Typle>() + '>'; }

private:

    HeadType m_head;
};

template <typename Typle_>
std::ostream &operator << (std::ostream &out, List_t<Typle_> const &l)
{
    l.print(out);
    return out;
}






template <typename Typle_, Uint32 INDEX_>
struct ListElement_t
{
    static_assert(INDEX_ < Hippo::Length_f<Typle_>::V, "attempted element access past the end of the List_t");
    typedef List_t<Typle_> List;
    typedef typename Hippo::Element_f<Typle_,INDEX_>::T ValueType;
    static ValueType const &el (List const &list)
    {
        return ListElement_t<typename Hippo::BodyTyple_f<Typle_>::T,INDEX_-1>::el(list.body());
    }
    static ValueType &el (List &list)
    {
        return ListElement_t<typename Hippo::BodyTyple_f<Typle_>::T,INDEX_-1>::el(list.body());
    }
};

template <typename Typle_>
struct ListElement_t<Typle_,0>
{
    static_assert(0 < Hippo::Length_f<Typle_>::V, "attempted element access past the end of the List_t");
    typedef List_t<Typle_> List;
    typedef typename Hippo::Element_f<Typle_,0>::T ValueType;
    static ValueType const &el (List const &list) { return list.head(); }
    static ValueType &el (List &list) { return list.head(); }
};

// for use in the el() and trailing_list() methods in List_t
template <typename Typle_, Uint32 INDEX_>
struct ListHelper_t
{
    static_assert(INDEX_ > 0, "attempted element access past the end of the List_t");
    typedef List_t<Typle_> List;
    typedef List_t<typename Hippo::LeadingTyple_f<Typle_,INDEX_>::T> LeadingListType;
    typedef List_t<typename Hippo::TrailingTyple_f<Typle_,INDEX_>::T> TrailingListType;
    static TrailingListType const &trailing_list (List const &list)
    {
        return ListHelper_t<typename Hippo::BodyTyple_f<Typle_>::T,INDEX_-1>::trailing_list(list.body());
    }
    static TrailingListType &trailing_list (List &list)
    {
        return ListHelper_t<typename Hippo::BodyTyple_f<Typle_>::T,INDEX_-1>::trailing_list(list.body());
    }
};

template <typename Typle_>
struct ListHelper_t<Typle_,0>
{
    typedef List_t<Typle_> List;
    typedef List TrailingListType;
    static TrailingListType const &trailing_list (List const &list) { return list; }
    static TrailingListType &trailing_list (List &list) { return list; }
};





template <typename Type_, Uint32 LENGTH_>
struct UniformListOfLength_f
{
    typedef List_t<typename Hippo::UniformTypleOfLength_f<LENGTH_,Type_>::T> T;
private:
    UniformListOfLength_f();
};


// tack an element onto the beginning of a list (where the list is empty)
template <typename HeadType_>
inline List_t<Typle_t<HeadType_> > operator >>= (HeadType_ const &head, List_t<Typle_t<>> const &)
{
    return List_t<Typle_t<HeadType_>>(head);
}

// tack an element onto the beginning of a list (catch-all case)
template <typename HeadType_, typename... BodyTypes_>
inline List_t<Typle_t<HeadType_,BodyTypes_...>> operator >>= (HeadType_ const &head, List_t<Typle_t<BodyTypes_...>> const &body)
{
    return List_t<Typle_t<HeadType_,BodyTypes_...>>(head, body);
}


// these can totally go away once Tuple_t is fully implemented.

// base case
inline EmptyList operator | (EmptyList const &, EmptyList const &)
{
    return EmptyList();
}

// base case
template <typename Typle_>
List_t<Typle_> operator | (List_t<Typle_> const &lhs, EmptyList const &)
{
    return lhs;
}

// base case
template <typename Typle_>
List_t<Typle_> operator | (EmptyList const &, List_t<Typle_> const &rhs)
{
    return rhs;
}

// this allows you to do stuff like
// tuple(1, 2, 3) | tuple(4, 5, 6) | tuple(7, 8, 9)
// to get the 9-tuple (1, 2, 3, 4, 5, 6, 7, 8, 9) without having to make a 9-tuple function explicitly.
template <typename Lhs_Typle_, typename Rhs_Typle_>
List_t<typename Hippo::Concat2Typles_f<Lhs_Typle_,Rhs_Typle_>::T>
    operator | (List_t<Lhs_Typle_> const &lhs, List_t<Rhs_Typle_> const &rhs)
{
    return List_t<typename Hippo::Concat2Typles_f<Lhs_Typle_,Rhs_Typle_>::T>(lhs.head(), lhs.body() | rhs);
}

} // end of namespace Tenh

#endif // TENH_LIST_HPP_
