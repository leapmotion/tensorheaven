// ///////////////////////////////////////////////////////////////////////////
// tenh/multiindex.hpp by Victor Dods, created 2013/03/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

// TODO: rename to componentmultiindex.hpp

#ifndef TENH_MULTIINDEX_HPP_
#define TENH_MULTIINDEX_HPP_

#include "tenh/core.hpp"

#include <algorithm>

#include "tenh/componentindex.hpp"
#include "tenh/tuple.hpp"

namespace Tenh {

// IndexTyple_ should be a Typle_t containing ComponentIndex_t types
template <typename IndexTyple_>
struct MultiIndex_t : Tuple_t<IndexTyple_>
{
    static_assert(IsTyple_f<IndexTyple_>::V, "IndexTyple_ must be a Typle_t");
    static_assert(EachTypeSatisfies_f<IndexTyple_,IsComponentIndex_e>::V, "each type must be a ComponentIndex_t");

    typedef Tuple_t<IndexTyple_> Parent;
    using Parent::LENGTH;
    typedef IndexTyple_ IndexTyple;
    typedef typename Head_f<IndexTyple_>::T HeadIndexType;
    typedef typename BodyTyple_f<IndexTyple_>::T BodyIndexTyple;
    typedef MultiIndex_t<BodyIndexTyple> BodyMultiIndex;
    static Uint32 const COMPONENT_COUNT = HeadIndexType::COMPONENT_COUNT * BodyMultiIndex::COMPONENT_COUNT;

    MultiIndex_t () { } // default constructor initializes to "first" component
    MultiIndex_t (HeadIndexType const &head, BodyMultiIndex const &body) : Parent(head, body) { }
    // tuple-like initializers for raw Uint32
    MultiIndex_t (Uint32 i0, Uint32 i1, bool check_range = CHECK_RANGE)
        :
        Parent(HeadIndexType(i0, check_range), BodyMultiIndex(i1, check_range))
    {
        STATIC_ASSERT((Parent::LENGTH == 2), LENGTH_DOES_NOT_MATCH_ARGUMENT_COUNT);
    }
    MultiIndex_t (Uint32 i0, Uint32 i1, Uint32 i2, bool check_range = CHECK_RANGE)
        :
        Parent(HeadIndexType(i0, check_range), BodyMultiIndex(i1, i2, check_range))
    {
        STATIC_ASSERT((Parent::LENGTH == 3), LENGTH_DOES_NOT_MATCH_ARGUMENT_COUNT);
    }
    MultiIndex_t (Uint32 i0, Uint32 i1, Uint32 i2, Uint32 i3, bool check_range = CHECK_RANGE)
        :
        Parent(HeadIndexType(i0, check_range), BodyMultiIndex(i1, i2, i3, check_range))
    {
        STATIC_ASSERT((Parent::LENGTH == 4), LENGTH_DOES_NOT_MATCH_ARGUMENT_COUNT);
    }
    // can really only provide tuple-like initializer for 2-multi-indices, due to the lack
    // of a guarantee that BodyMultiIndex has more than a head index type.
    MultiIndex_t (HeadIndexType const &i0, typename BodyMultiIndex::HeadIndexType const &i1)
        :
        Parent(i0, BodyMultiIndex(i1))
    {
        STATIC_ASSERT((Parent::LENGTH == 2), LENGTH_DOES_NOT_MATCH_ARGUMENT_COUNT);
    }

    MultiIndex_t (MultiIndex_t<Typle_t<>> const &) { } // default construction
    // this includes the copy constructor (probably)
    template <typename OtherIndexTyple_>
    MultiIndex_t (MultiIndex_t<OtherIndexTyple_> const &leading_multi_index)
        :
        Parent(leading_multi_index)
    {
        static_assert(IsTyple_f<OtherIndexTyple_>::V, "OtherIndexTyple_ must be a Typle_t");
        static_assert(EachTypeSatisfies_f<OtherIndexTyple_,IsComponentIndex_e>::V, "each type must be a ComponentIndex_t");
    }

private:
    static Uint32 const BODY_COMPONENT_COUNT_OR_ONE = If_f<BodyMultiIndex::COMPONENT_COUNT==0,
                                                           Value_t<Uint32,1>,
                                                           Value_t<Uint32,BodyMultiIndex::COMPONENT_COUNT>>::T::V;

public:
    // for converting a vector index into a multi-index via row-major ordering (most significant is head).
    MultiIndex_t (ComponentIndex_t<COMPONENT_COUNT> const &i)
        :
        Parent(HeadIndexType(i.value() / BODY_COMPONENT_COUNT_OR_ONE),
        BodyMultiIndex(ComponentIndex_t<BodyMultiIndex::COMPONENT_COUNT>(i.value() % BODY_COMPONENT_COUNT_OR_ONE)))
    { }

    bool operator == (MultiIndex_t const &m) const { return this->head() == m.head() && body() == m.body(); }
    bool operator != (MultiIndex_t const &m) const { return this->head() != m.head() || body() != m.body(); }
    bool operator < (MultiIndex_t const &m) const { return this->head() < m.head() && body() < m.body(); }

    bool is_at_end () const { return this->head().is_at_end(); } // because the head is the last one incremented
    bool is_not_at_end () const { return this->head().is_not_at_end(); } // because the head is the last one incremented
    Uint32 value () const { return BodyMultiIndex::COMPONENT_COUNT*this->head().value() + this->body().value(); }
    ComponentIndex_t<COMPONENT_COUNT> as_component_index () const { return ComponentIndex_t<COMPONENT_COUNT>(value(), DONT_CHECK_RANGE); }
    // TODO: think about adding a redundant single index that just increments and can be returned directly in value()
    void operator ++ ()
    {
        BodyMultiIndex &b = body();
        ++b; // increment the body index
        if (b.is_at_end()) // if it hit the end, reset it and increment the head
        {
            b.reset();
            ++(this->head());
        }
    }
    void reset () { this->head().reset(); body().reset(); }

    // slighty hacky way to use Tuple_t's existing functionality -- NOTE: this only
    // works because MultiIndex_t<IndexTyple> inherits non-virtually from
    // Tuple_t<IndexTyple> and has no members.
    BodyMultiIndex const &body () const { return *static_cast<BodyMultiIndex const *>(&Parent::body()); }
    BodyMultiIndex &body () { return *static_cast<BodyMultiIndex *>(&Parent::body()); }

    // if IndexTyple is uniform (meaning all its types are the same), then this provides
    // run-time access to the ith index.
    HeadIndexType const &index (Uint32 i, bool check_range = CHECK_RANGE) const
    {
        static_assert(TypleIsUniform_f<IndexTyple_>::V, "IndexTyple_ must be uniform to use this method");
        assert(this->is_layed_out_contiguously_in_memory());
        if (check_range && i >= LENGTH)
            throw std::out_of_range("index argument was out of range");
        HeadIndexType const *multi_index_as_array = &this->head();
        return multi_index_as_array[i];
    }
    HeadIndexType &index (Uint32 i, bool check_range = CHECK_RANGE)
    {
        static_assert(TypleIsUniform_f<IndexTyple_>::V, "IndexTyple_ must be uniform to use this method");
        assert(this->is_layed_out_contiguously_in_memory());
        if (check_range && i >= LENGTH)
            throw std::out_of_range("index argument was out of range");
        HeadIndexType *multi_index_as_array = &this->head();
        return multi_index_as_array[i];
    }
    // if IndexTyple is non-uniform (meaning not all its types are the same), then this provides
    // run-time access to the ith index in a weakly-typed way (via the Uint32-typed value())
    Uint32 value_of_index (Uint32 i, bool check_range = CHECK_RANGE) const
    {
        assert(this->is_layed_out_contiguously_in_memory());
        if (check_range && i >= LENGTH)
            throw std::out_of_range("index argument was out of range");
        Uint32 const *multi_index_as_array = reinterpret_cast<Uint32 const *>(&this->head());
        return multi_index_as_array[i];
    }
    Uint32 value_of_index (Uint32 i, bool check_range = CHECK_RANGE)
    {
        assert(this->is_layed_out_contiguously_in_memory());
        if (check_range && i >= LENGTH)
            throw std::out_of_range("index argument was out of range");
        Uint32 const *multi_index_as_array = reinterpret_cast<Uint32 const *>(&this->head());
        return multi_index_as_array[i];
    }

    // TODO: rename to RangeMultiIndex_t

    // slighty hacky way to use Tuple_t's existing functionality -- NOTE: this only
    // works because MultiIndex_t<IndexTyple> inherits non-virtually from
    // Tuple_t<IndexTyple> and has no members.
    // returns the type of the MultiIndex_t having the specified range
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    struct RangeType_f
    {
        typedef MultiIndex_t<typename TypleRange_f<IndexTyple,START_INDEX_,END_INDEX_>::T> T;
    };

    // returns the MultiIndex_t which is the given range of elements
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T const &range () const
    {
        return *static_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T const *>(&(this->Parent::template range<START_INDEX_,END_INDEX_>()));
    }
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T &range ()
    {
        return *static_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T *>(&(this->Parent::template range<START_INDEX_,END_INDEX_>()));
    }

    // slighty hacky way to use Tuple_t's existing functionality -- NOTE: this only
    // works because MultiIndex_t<IndexTyple> inherits non-virtually from
    // Tuple_t<IndexTyple> and has no members.
    // returns the type of the leading MultiIndex_t ending at the END_INDEX_th element
    template <Uint32 END_INDEX_>
    struct LeadingTupleType_f
    {
        typedef MultiIndex_t<typename LeadingTyple_f<IndexTyple,END_INDEX_>::T> T;
    };

    // TODO: rename to leading_multi_index
    // returns the leading MultiIndex_t ending at the END_INDEX_th element.
    template <Uint32 END_INDEX_>
    typename LeadingTupleType_f<END_INDEX_>::T const &leading_tuple () const
    {
        return *static_cast<typename LeadingTupleType_f<END_INDEX_>::T const *>(&(this->Parent::template leading_tuple<END_INDEX_>()));
    }
    template <Uint32 END_INDEX_>
    typename LeadingTupleType_f<END_INDEX_>::T &leading_tuple ()
    {
        return *static_cast<typename LeadingTupleType_f<END_INDEX_>::T *>(&(this->Parent::template leading_tuple<END_INDEX_>()));
    }

    // TODO: rename to TrailingMultiIndex_t

    // slighty hacky way to use Tuple_t's existing functionality -- NOTE: this only
    // works because MultiIndex_t<IndexTyple> inherits non-virtually from
    // Tuple_t<IndexTyple> and has no members.
    // returns the type of the trailing Tuple_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    struct TrailingTupleType_f
    {
        typedef MultiIndex_t<typename TrailingTyple_f<IndexTyple,START_INDEX_>::T> T;
    };

    // TODO: rename to trailing_multi_index
    // returns the trailing MultiIndex_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    typename TrailingTupleType_f<START_INDEX_>::T const &trailing_tuple () const
    {
        return *static_cast<typename TrailingTupleType_f<START_INDEX_>::T const *>(&(this->Parent::template trailing_tuple<START_INDEX_>()));
    };
    template <Uint32 START_INDEX_>
    typename TrailingTupleType_f<START_INDEX_>::T &trailing_tuple ()
    {
        return *static_cast<typename TrailingTupleType_f<START_INDEX_>::T *>(&(this->Parent::template trailing_tuple<START_INDEX_>()));
    };

    static std::string type_as_string () { return "MultiIndex_t<" + type_string_of<IndexTyple>() + '>'; }
};

// template specializations for the IndexTyple tuple corner cases
template <typename HeadIndexType_>
struct MultiIndex_t<Typle_t<HeadIndexType_>> : public Tuple_t<Typle_t<HeadIndexType_>>
{
    static_assert(IsComponentIndex_f<HeadIndexType_>::V, "HeadIndexType_ must be a ComponentIndex_t");

    typedef Tuple_t<Typle_t<HeadIndexType_>> Parent;
    typedef HeadIndexType_ HeadIndexType;
    using Parent::LENGTH;
    typedef Typle_t<HeadIndexType> IndexTyple;
    typedef Typle_t<> BodyIndexTyple;
    typedef MultiIndex_t<BodyIndexTyple> BodyMultiIndex;
    static Uint32 const COMPONENT_COUNT = HeadIndexType::COMPONENT_COUNT;

    MultiIndex_t () { } // default constructor initializes to "first" component
    // explicit because it has a range-check (in the HeadIndexType constructor)
    explicit MultiIndex_t (Uint32 i, bool check_range = CHECK_RANGE) : Parent(HeadIndexType(i, check_range)) { }
    explicit MultiIndex_t (HeadIndexType const &head) : Parent(head) { }

    MultiIndex_t (MultiIndex_t<Typle_t<>> const &) { } // default construction
    // this is the copy constructor
    MultiIndex_t (MultiIndex_t const &m)
        :
        Parent(m)
    { }
    template <typename OtherIndexTyple_>
    MultiIndex_t (MultiIndex_t<OtherIndexTyple_> const &leading_multi_index)
        :
        Parent(leading_multi_index)
    {
        static_assert(IsTyple_f<OtherIndexTyple_>::V, "OtherIndexTyple_ must be a Typle_t");
        static_assert(EachTypeSatisfies_f<OtherIndexTyple_,IsComponentIndex_e>::V, "each type must be a ComponentIndex_t");
    }

    bool operator == (MultiIndex_t const &m) const { return this->head() == m.head(); }
    bool operator != (MultiIndex_t const &m) const { return this->head() != m.head(); }
    bool operator < (MultiIndex_t const &m) const { return this->head() < m.head(); }

    bool is_at_end () const { return this->head().is_at_end(); }
    bool is_not_at_end () const { return this->head().is_not_at_end(); }
    Uint32 value () const { return this->head().value(); }
    ComponentIndex_t<COMPONENT_COUNT> as_component_index () const { return ComponentIndex_t<COMPONENT_COUNT>(value(), DONT_CHECK_RANGE); }
    void operator ++ () { ++(this->head()); }
    void reset () { this->head().reset(); }

    // slighty hacky way to use Tuple_t's existing functionality -- NOTE: this only
    // works because MultiIndex_t<IndexTyple> inherits non-virtually from
    // Tuple_t<IndexTyple> and has no members.
    BodyMultiIndex const &body () const { return *static_cast<BodyMultiIndex const *>(&Parent::body()); }
    BodyMultiIndex &body () { return *static_cast<BodyMultiIndex *>(&Parent::body()); }

    // if IndexTyple is uniform (meaning all its types are the same), then this provides
    // run-time access to the ith index.
    HeadIndexType const &index (Uint32 i, bool check_range = CHECK_RANGE) const
    {
        static_assert(TypleIsUniform_f<IndexTyple>::V, "IndexTyple_ must be uniform to use this method");
        assert(this->is_layed_out_contiguously_in_memory());
        if (check_range && i >= LENGTH)
            throw std::out_of_range("index argument was out of range");
        HeadIndexType const *multi_index_as_array = &this->head();
        return multi_index_as_array[i];
    }
    HeadIndexType &index (Uint32 i, bool check_range = CHECK_RANGE)
    {
        static_assert(TypleIsUniform_f<IndexTyple>::V, "IndexTyple_ must be uniform to use this method");
        assert(this->is_layed_out_contiguously_in_memory());
        if (check_range && i >= LENGTH)
            throw std::out_of_range("index argument was out of range");
        HeadIndexType *multi_index_as_array = &this->head();
        return multi_index_as_array[i];
    }
    // if IndexTyple is non-uniform (meaning not all its types are the same), then this provides
    // run-time access to the ith index in a weakly-typed way (via the Uint32-typed value())
    Uint32 value_of_index (Uint32 i, bool check_range = CHECK_RANGE) const
    {
        assert(this->is_layed_out_contiguously_in_memory());
        if (check_range && i >= LENGTH)
            throw std::out_of_range("index argument was out of range");
        Uint32 const *multi_index_as_array = reinterpret_cast<Uint32 const *>(&this->head());
        return multi_index_as_array[i];
    }
    Uint32 value_of_index (Uint32 i, bool check_range = CHECK_RANGE)
    {
        assert(this->is_layed_out_contiguously_in_memory());
        if (check_range && i >= LENGTH)
            throw std::out_of_range("index argument was out of range");
        Uint32 const *multi_index_as_array = reinterpret_cast<Uint32 const *>(&this->head());
        return multi_index_as_array[i];
    }

    // type conversion operator -- because this MultiIndex_t only has one component,
    // it can be canonically identified as its component type.
    operator HeadIndexType_ const & () const { return this->head(); }
    operator HeadIndexType_ & () { return this->head(); }

    // TODO: rename to LeadingMultiIndex_t

    // slighty hacky way to use Tuple_t's existing functionality -- NOTE: this only
    // works because MultiIndex_t<IndexTyple> inherits non-virtually from
    // Tuple_t<IndexTyple> and has no members.
    // returns the type of the MultiIndex_t having the specified range
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    struct RangeType_f
    {
        typedef MultiIndex_t<typename TypleRange_f<IndexTyple,START_INDEX_,END_INDEX_>::T> T;
    };

    // returns the MultiIndex_t which is the given range of elements
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T const &range () const
    {
        return *static_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T const *>(&(this->Parent::template range<START_INDEX_,END_INDEX_>()));
    }
    template <Uint32 START_INDEX_, Uint32 END_INDEX_>
    typename RangeType_f<START_INDEX_,END_INDEX_>::T &range ()
    {
        return *static_cast<typename RangeType_f<START_INDEX_,END_INDEX_>::T *>(&(this->Parent::template range<START_INDEX_,END_INDEX_>()));
    }

    // slighty hacky way to use Tuple_t's existing functionality -- NOTE: this only
    // works because MultiIndex_t<IndexTyple> inherits non-virtually from
    // Tuple_t<IndexTyple> and has no members.
    // returns the type of the leading MultiIndex_t ending at the END_INDEX_th element
    template <Uint32 END_INDEX_>
    struct LeadingTupleType_f
    {
        typedef MultiIndex_t<typename LeadingTyple_f<IndexTyple,END_INDEX_>::T> T;
    };

    // TODO: rename to leading_multi_index
    // returns the leading MultiIndex_t ending at the END_INDEX_th element.
    template <Uint32 END_INDEX_>
    typename LeadingTupleType_f<END_INDEX_>::T const &leading_tuple () const
    {
        return *static_cast<typename LeadingTupleType_f<END_INDEX_>::T const *>(&(this->Parent::template leading_tuple<END_INDEX_>()));
    }
    template <Uint32 END_INDEX_>
    typename LeadingTupleType_f<END_INDEX_>::T &leading_tuple ()
    {
        return *static_cast<typename LeadingTupleType_f<END_INDEX_>::T *>(&(this->Parent::template leading_tuple<END_INDEX_>()));
    }

    // TODO: rename to TrailingMultiIndex_t

    // slighty hacky way to use Tuple_t's existing functionality -- NOTE: this only
    // works because MultiIndex_t<IndexTyple> inherits non-virtually from
    // Tuple_t<IndexTyple> and has no members.
    // returns the type of the trailing Tuple_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    struct TrailingTupleType_f
    {
        typedef MultiIndex_t<typename TrailingTyple_f<IndexTyple,START_INDEX_>::T> T;
    };

    // TODO: rename to trailing_multi_index
    // returns the trailing MultiIndex_t starting at the START_INDEX_th element
    template <Uint32 START_INDEX_>
    typename TrailingTupleType_f<START_INDEX_>::T const &trailing_tuple () const
    {
        return *static_cast<typename TrailingTupleType_f<START_INDEX_>::T const *>(&(this->Parent::template trailing_tuple<START_INDEX_>()));
    };
    template <Uint32 START_INDEX_>
    typename TrailingTupleType_f<START_INDEX_>::T &trailing_tuple ()
    {
        return *static_cast<typename TrailingTupleType_f<START_INDEX_>::T *>(&(this->Parent::template trailing_tuple<START_INDEX_>()));
    };

    static std::string type_as_string () { return "MultiIndex_t<" + type_string_of<IndexTyple>() + '>'; }
};

template <>
struct MultiIndex_t<Typle_t<>> : public Tuple_t<Typle_t<>>
{
    typedef Tuple_t<Typle_t<>> Parent;
    using Parent::LENGTH;
    typedef Typle_t<> IndexTyple;
    typedef Typle_t<> BodyIndexTyple;
    typedef MultiIndex_t<BodyIndexTyple> BodyMultiIndex;
    static Uint32 const COMPONENT_COUNT = 0;

    MultiIndex_t () { }

    bool operator == (MultiIndex_t const &) const { return true; }  // there is only one of these, so it must be equal
    bool operator != (MultiIndex_t const &) const { return false; } // there is only one of these, so it must not be unequal
    bool operator < (MultiIndex_t const &) const { return false; }  // there is only one of these, so it can't be less than

    bool is_at_end () const { return true; }
    bool is_not_at_end () const { return false; }
    Uint32 value () const { return 0; } // vacuous value
    ComponentIndex_t<COMPONENT_COUNT> as_component_index () const { return ComponentIndex_t<COMPONENT_COUNT>(value(), DONT_CHECK_RANGE); }
    void operator ++ () { } // no-op
    void reset () { } // no-op

    // slighty hacky way to use Tuple_t's existing functionality -- NOTE: this only
    // works because MultiIndex_t<IndexTyple> inherits non-virtually from
    // Tuple_t<IndexTyple> and has no members.
    BodyMultiIndex const &body () const { return *static_cast<BodyMultiIndex const *>(&Parent::body()); }
    BodyMultiIndex &body () { return *static_cast<BodyMultiIndex *>(&Parent::body()); }

    static std::string type_as_string () { return "MultiIndex_t<" + type_string_of<IndexTyple>() + '>'; }
};



template <typename T_> struct IsMultiIndex_f
{
    static bool const V = false;
private:
    IsMultiIndex_f();
};
template <typename IndexTyple_> struct IsMultiIndex_f<MultiIndex_t<IndexTyple_>>
{
    static bool const V = true;
private:
    IsMultiIndex_f();
};

MAKE_1_ARY_VALUE_EVALUATOR(IsMultiIndex, bool);



// type-specific analogs of all the operator overloads for Tuple_t

// tack an element onto the beginning of a tuple (where the tuple is empty)
template <typename HeadType_>
inline MultiIndex_t<Typle_t<HeadType_>> operator >>= (HeadType_ const &head, MultiIndex_t<Typle_t<>> const &)
{
    return MultiIndex_t<Typle_t<HeadType_>>(head);
}

// tack an element onto the beginning of a tuple (catch-all case)
template <typename HeadType_, typename... BodyTypes_>
inline MultiIndex_t<Typle_t<HeadType_,BodyTypes_...>> operator >>= (HeadType_ const &head, MultiIndex_t<Typle_t<BodyTypes_...>> const &body)
{
    return MultiIndex_t<Typle_t<HeadType_,BodyTypes_...>>(head, body);
}


// base case
inline MultiIndex_t<Typle_t<>> operator | (MultiIndex_t<Typle_t<>> const &, MultiIndex_t<Typle_t<>> const &)
{
    return MultiIndex_t<Typle_t<>>();
}

// base case
template <typename Typle_>
MultiIndex_t<Typle_> operator | (MultiIndex_t<Typle_> const &lhs, MultiIndex_t<Typle_t<>> const &)
{
    return lhs;
}

// base case
template <typename Typle_>
MultiIndex_t<Typle_> operator | (MultiIndex_t<Typle_t<>> const &, MultiIndex_t<Typle_> const &rhs)
{
    return rhs;
}

// this allows you to do stuff like
// tuple(1, 2, 3) | tuple(4, 5, 6) | tuple(7, 8, 9)
// to get the 9-tuple (1, 2, 3, 4, 5, 6, 7, 8, 9) without having to make a 9-tuple function explicitly.
template <typename Lhs_Typle_,
          typename Rhs_Typle_>
MultiIndex_t<typename Concat2Typles_f<Lhs_Typle_,Rhs_Typle_>::T>
    operator | (MultiIndex_t<Lhs_Typle_> const &lhs, MultiIndex_t<Rhs_Typle_> const &rhs)
{
    return MultiIndex_t<typename Concat2Typles_f<Lhs_Typle_,Rhs_Typle_>::T>(lhs.head(), lhs.body() | rhs);
}





// in-place sort of a uniform MultiIndex_t -- the default type for Compare should be std::less<Uint32>
template <typename Compare_, typename IndexTyple_>
void sort (MultiIndex_t<IndexTyple_> &m)
{
    static_assert(TypleIsUniform_f<IndexTyple_>::V, "IndexTyple_ must be uniform in order to sort");
    // run-time check that the memory is actually layed out contiguously, which is necessary for the reinterpret_cast to work.
    assert(m.is_layed_out_contiguously_in_memory());
    Uint32 *starting_pointer = reinterpret_cast<Uint32*>(&m.head());
    std::sort(starting_pointer, starting_pointer + m.LENGTH, Compare_());
}

// functional sort of a uniform MultiIndex_t -- the default type for Compare should be std::less<Uint32>
template <typename Compare_, typename IndexTyple_>
MultiIndex_t<IndexTyple_> sorted (MultiIndex_t<IndexTyple_> const &m)
{
    MultiIndex_t<IndexTyple_> retval(m);
    sort<Compare_>(retval);
    return retval;
}





// DomainIndexTyple_ must be a Typle_t of unique types
// CodomainIndexTyple_ must be a Typle_t consisting of a subset of DomainIndexTyple_
template <typename DomainIndexTyple_, typename CodomainIndexTyple_>
struct MultiIndexMap_t
{
    static_assert(!ContainsDuplicates_f<DomainIndexTyple_>::V, "DomainIndexTyple_ must not contain duplicates");

    typedef MultiIndex_t<DomainIndexTyple_> DomainIndex;
    typedef MultiIndex_t<CodomainIndexTyple_> CodomainIndex;
    typedef CodomainIndex (*EvalMapType) (DomainIndex const &);
    static CodomainIndex eval (DomainIndex const &i)
    {
        return CodomainIndex(i.template el<IndexOfFirstOccurrence_f<DomainIndexTyple_,typename Head_f<CodomainIndexTyple_>::T>::V>(),
                             MultiIndexMap_t<DomainIndexTyple_,typename BodyTyple_f<CodomainIndexTyple_>::T>::eval(i));
    }
private:
    MultiIndexMap_t();
};

template <typename DomainIndexTyple_, typename CodomainIndexType_>
struct MultiIndexMap_t<DomainIndexTyple_,Typle_t<CodomainIndexType_> >
{
    static_assert(!ContainsDuplicates_f<DomainIndexTyple_>::V, "DomainIndexTyple_ must not contain duplicates");

    typedef Typle_t<CodomainIndexType_> CodomainIndexTyple_;
    typedef MultiIndex_t<DomainIndexTyple_> DomainIndex;
    typedef MultiIndex_t<CodomainIndexTyple_> CodomainIndex;
    typedef CodomainIndex (*EvalMapType) (DomainIndex const &);
    static CodomainIndex eval (DomainIndex const &i)
    {
        return CodomainIndex(i.template el<IndexOfFirstOccurrence_f<DomainIndexTyple_,typename Head_f<CodomainIndexTyple_>::T>::V>());
    }
private:
    MultiIndexMap_t();
};

template <typename MultiIndex_>
struct MultiIndexMultiplicity_t
{
    static_assert(IsMultiIndex_f<MultiIndex_>::V, "MultiIndex_ must be a MultiIndex_t");
    static_assert(TypleIsUniform_f<typename MultiIndex_::IndexTyple>::V, "MultiIndex_ must have uniform component types");
    static Uint32 eval (MultiIndex_ const &m, Uint32 count = 1)
    {
        return count * ((m.head() == m.body().head()) ? 
                        MultiIndexMultiplicity_t<typename MultiIndex_::BodyMultiIndex>::eval(m.body(),count + 1) :
                        MultiIndexMultiplicity_t<typename MultiIndex_::BodyMultiIndex>::eval(m.body(),1));
    }
private:
    MultiIndexMultiplicity_t();
};

template<typename IndexType_>
struct MultiIndexMultiplicity_t<MultiIndex_t<Typle_t<IndexType_>>>
{
    static Uint32 eval (MultiIndex_t<Typle_t<IndexType_>> const &m, Uint32 count = 1) { return count; }
private:
    MultiIndexMultiplicity_t();
};


template <typename IndexTyple_>
Uint32 trailing_zero_count (MultiIndex_t<IndexTyple_> const &m)
{
    if (Length_f<IndexTyple_>::V == 0)
        return 0;

    for (Uint32 i = 0; i < m.length(); ++i)
        if (m.value_of_index(m.length() - i - 1) != 0)
            return i;

    return m.length();
}

} // end of namespace Tenh

#endif // TENH_MULTIINDEX_HPP_
