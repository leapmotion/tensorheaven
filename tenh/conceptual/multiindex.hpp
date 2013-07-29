// ///////////////////////////////////////////////////////////////////////////
// tenh/multiindex.hpp by Victor Dods, created 2013/03/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_MULTIINDEX_HPP_
#define TENH_MULTIINDEX_HPP_

#include "tenh/core.hpp"
#include "tenh/list.hpp"
#include "tenh/meta/typelist_utility.hpp"

namespace Tenh {

// IndexTypeList should be a TypeList_t containing Index types (e.g. Vector_t::Index)
// TODO: change IndexTypeList_ to be BasedTensorProduct_ (?).  NOTE: this may complicate
// the definition of this class, since it uses the recursive list structure of TypeList_t
// TODO: rename MultiIndex_c to BasedMultiIndex_c
template <typename IndexTypeList_>
struct MultiIndex_c : List_t<IndexTypeList_>
{
    typedef List_t<IndexTypeList_> Parent;
    using Parent::LENGTH;
    typedef IndexTypeList_ IndexTypeList;
    typedef typename IndexTypeList::HeadType HeadIndexType;
    typedef typename IndexTypeList::BodyTypeList BodyIndexTypeList;
    typedef MultiIndex_c<BodyIndexTypeList> BodyMultiIndex;
    static Uint32 const COMPONENT_COUNT = HeadIndexType::COMPONENT_COUNT * BodyMultiIndex::COMPONENT_COUNT;

    MultiIndex_c () { } // default constructor initializes to "first" component
    MultiIndex_c (HeadIndexType const &head, BodyMultiIndex const &body) : Parent(head, body) { }
    // tuple-like initializers
    MultiIndex_c (Uint32 i0, Uint32 i1, bool check_range = CHECK_RANGE) // TODO: replace with IndexTypeList::HeadType i0, IndexTypeList::BodyTypeList::HeadType i1 ?
        :
        Parent(HeadIndexType(i0, check_range), BodyMultiIndex(i1, check_range))
    {
        STATIC_ASSERT((Parent::LENGTH == 2),LENGTH_DOES_NOT_MATCH_ARGUMENT_COUNT);
    }
    MultiIndex_c (Uint32 i0, Uint32 i1, Uint32 i2, bool check_range = CHECK_RANGE)
        :
        Parent(HeadIndexType(i0, check_range), BodyMultiIndex(i1, i2, check_range))
    {
        STATIC_ASSERT((Parent::LENGTH == 3),LENGTH_DOES_NOT_MATCH_ARGUMENT_COUNT);
    }
    MultiIndex_c (Uint32 i0, Uint32 i1, Uint32 i2, Uint32 i3, bool check_range = CHECK_RANGE)
        :
        Parent(HeadIndexType(i0, check_range), BodyMultiIndex(i1, i2, i3, check_range))
    {
        STATIC_ASSERT((Parent::LENGTH == 4),LENGTH_DOES_NOT_MATCH_ARGUMENT_COUNT);
    }

    MultiIndex_c (MultiIndex_c<EmptyTypeList> const &) { } // default construction
    MultiIndex_c (MultiIndex_c<TypeList_t<HeadIndexType> > const &leading_multi_index)
        :
        Parent(leading_multi_index)
    { }
    // this includes the copy constructor
    template <typename OtherHeadIndexType, typename OtherBodyIndexTypeList>
    MultiIndex_c (MultiIndex_c<TypeList_t<OtherHeadIndexType,OtherBodyIndexTypeList> > const &leading_multi_index)
        :
        Parent(leading_multi_index)
    { }

    bool operator == (MultiIndex_c const &m) const { return this->head() == m.head() && body() == m.body(); }
    bool operator != (MultiIndex_c const &m) const { return this->head() != m.head() || body() != m.body(); }
    bool operator < (MultiIndex_c const &m) const { return this->head() < m.head() && body() < m.body(); }

    bool is_at_end () const { return this->head().is_at_end(); } // because the head is the last one incremented
    bool is_not_at_end () const { return this->head().is_not_at_end(); } // because the head is the last one incremented
    Uint32 value () const { return BodyMultiIndex::COMPONENT_COUNT*this->head().value() + this->body().value(); }
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

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because MultiIndex_c<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    BodyMultiIndex const &body () const { return *static_cast<BodyMultiIndex const *>(&Parent::body()); }
    BodyMultiIndex &body () { return *static_cast<BodyMultiIndex *>(&Parent::body()); }

    // TODO: rename to RangeMultiIndex_c

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because MultiIndex_c<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the MultiIndex_c having the specified range
    template <Uint32 START_INDEX, Uint32 END_INDEX>
    struct RangeType_t
    {
        typedef MultiIndex_c<typename IndexTypeList::template Range_t<START_INDEX,END_INDEX>::T> T;
    };

    // returns the MultiIndex_c which is the given range of elements
    template <Uint32 START_INDEX, Uint32 END_INDEX>
    typename RangeType_t<START_INDEX,END_INDEX>::T const &range () const
    {
        return *static_cast<typename RangeType_t<START_INDEX,END_INDEX>::T const *>(&(this->Parent::template range<START_INDEX,END_INDEX>()));
    }
    template <Uint32 START_INDEX, Uint32 END_INDEX>
    typename RangeType_t<START_INDEX,END_INDEX>::T &range ()
    {
        return *static_cast<typename RangeType_t<START_INDEX,END_INDEX>::T *>(&(this->Parent::template range<START_INDEX,END_INDEX>()));
    }

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because MultiIndex_c<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the leading MultiIndex_c ending at the INDEXth element
    template <Uint32 INDEX>
    struct LeadingListType_t
    {
//         typedef List_t<typename TypeList::template LeadingTypeList_t<INDEX>::T> T;
        typedef MultiIndex_c<typename IndexTypeList::template LeadingTypeList_t<INDEX>::T> T;
    };

    // TODO: rename to leading_multi_index
    // returns the leading MultiIndex_c ending at the INDEXth element.
    template <Uint32 INDEX>
    typename LeadingListType_t<INDEX>::T const &leading_list () const
    {
        return *static_cast<typename LeadingListType_t<INDEX>::T const *>(&(this->Parent::template leading_list<INDEX>()));
    }
    template <Uint32 INDEX>
    typename LeadingListType_t<INDEX>::T &leading_list ()
    {
        return *static_cast<typename LeadingListType_t<INDEX>::T *>(&(this->Parent::template leading_list<INDEX>()));
    }

    // TODO: rename to TrailingMultiIndex_c

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because MultiIndex_c<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingListType_t
    {
        typedef MultiIndex_c<typename IndexTypeList::template TrailingTypeList_t<INDEX>::T> T;
    };

    // TODO: rename to trailing_multi_index
    // returns the trailing MultiIndex_c starting at the INDEXth element
    template <Uint32 INDEX>
    typename TrailingListType_t<INDEX>::T const &trailing_list () const
    {
        return *static_cast<typename TrailingListType_t<INDEX>::T const *>(&(this->Parent::template trailing_list<INDEX>()));
    };
    template <Uint32 INDEX>
    typename TrailingListType_t<INDEX>::T &trailing_list ()
    {
        return *static_cast<typename TrailingListType_t<INDEX>::T *>(&(this->Parent::template trailing_list<INDEX>()));
    };

    static std::string type_as_string () { return "MultiIndex_c<" + TypeStringOf_t<IndexTypeList>::eval() + '>'; }
};

// template specializations for the IndexTypeList list corner cases
template <typename HeadIndexType>
struct MultiIndex_c<TypeList_t<HeadIndexType> > : public List_t<TypeList_t<HeadIndexType> >
{
    typedef List_t<TypeList_t<HeadIndexType> > Parent;
    using Parent::LENGTH;
    typedef TypeList_t<HeadIndexType> IndexTypeList;
    typedef typename IndexTypeList::BodyTypeList BodyIndexTypeList;
    typedef MultiIndex_c<BodyIndexTypeList> BodyMultiIndex;
    static Uint32 const COMPONENT_COUNT = HeadIndexType::COMPONENT_COUNT;

    MultiIndex_c () { } // default constructor initializes to "first" component
    // explicit because it has a range-check (in the HeadIndexType constructor)
    explicit MultiIndex_c (Uint32 i, bool check_range = CHECK_RANGE) : Parent(HeadIndexType(i, check_range)) { }
    MultiIndex_c (HeadIndexType const &head) : Parent(head) { }

    MultiIndex_c (MultiIndex_c<EmptyTypeList> const &) { } // default construction
    // this is the copy constructor
    MultiIndex_c (MultiIndex_c const &m)
        :
        Parent(m)
    { }
    template <typename OtherHeadIndexType, typename OtherBodyIndexTypeList>
    MultiIndex_c (MultiIndex_c<TypeList_t<OtherHeadIndexType,OtherBodyIndexTypeList> > const &leading_multi_index)
        :
        Parent(leading_multi_index)
    { }

    bool operator == (MultiIndex_c const &m) const { return this->head() == m.head(); }
    bool operator != (MultiIndex_c const &m) const { return this->head() != m.head(); }
    bool operator < (MultiIndex_c const &m) const { return this->head() < m.head(); }

    bool is_at_end () const { return this->head().is_at_end(); }
    bool is_not_at_end () const { return this->head().is_not_at_end(); }
    Uint32 value () const { return this->head().value(); }
    void operator ++ () { ++(this->head()); }
    void reset () { this->head().reset(); }

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because MultiIndex_c<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    BodyMultiIndex const &body () const { return *static_cast<BodyMultiIndex const *>(&Parent::body()); }
    BodyMultiIndex &body () { return *static_cast<BodyMultiIndex *>(&Parent::body()); }

    // type conversion operator -- because this MultiIndex_c only has one component,
    // it can be canonically identified as its component type.
    operator HeadIndexType const & () const { return this->head(); }
    operator HeadIndexType & () { return this->head(); }

    // TODO: rename to LeadingMultiIndex_c

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because MultiIndex_c<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the MultiIndex_c having the specified range
    template <Uint32 START_INDEX, Uint32 END_INDEX>
    struct RangeType_t
    {
        typedef MultiIndex_c<typename IndexTypeList::template Range_t<START_INDEX,END_INDEX>::T> T;
    };

    // returns the MultiIndex_c which is the given range of elements
    template <Uint32 START_INDEX, Uint32 END_INDEX>
    typename RangeType_t<START_INDEX,END_INDEX>::T const &range () const
    {
        return *static_cast<typename RangeType_t<START_INDEX,END_INDEX>::T const *>(&(this->Parent::template range<START_INDEX,END_INDEX>()));
    }
    template <Uint32 START_INDEX, Uint32 END_INDEX>
    typename RangeType_t<START_INDEX,END_INDEX>::T &range ()
    {
        return *static_cast<typename RangeType_t<START_INDEX,END_INDEX>::T *>(&(this->Parent::template range<START_INDEX,END_INDEX>()));
    }

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because MultiIndex_c<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the leading MultiIndex_c ending at the INDEXth element
    template <Uint32 INDEX>
    struct LeadingListType_t
    {
//         typedef List_t<typename TypeList::template LeadingTypeList_t<INDEX>::T> T;
        typedef MultiIndex_c<typename IndexTypeList::template LeadingTypeList_t<INDEX>::T> T;
    };

    // TODO: rename to leading_multi_index
    // returns the leading MultiIndex_c ending at the INDEXth element.
    template <Uint32 INDEX>
    typename LeadingListType_t<INDEX>::T const &leading_list () const
    {
        return *static_cast<typename LeadingListType_t<INDEX>::T const *>(&(this->Parent::template leading_list<INDEX>()));
    }
    template <Uint32 INDEX>
    typename LeadingListType_t<INDEX>::T &leading_list ()
    {
        return *static_cast<typename LeadingListType_t<INDEX>::T *>(&(this->Parent::template leading_list<INDEX>()));
    }

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because MultiIndex_c<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingListType_t
    {
        typedef MultiIndex_c<typename IndexTypeList::template TrailingTypeList_t<INDEX>::T> T;
    };

    // returns the trailing MultiIndex_c starting at the INDEXth element
    template <Uint32 INDEX>
    typename TrailingListType_t<INDEX>::T const &trailing_list () const
    {
        return *static_cast<typename TrailingListType_t<INDEX>::T const *>(&(this->Parent::template trailing_list<INDEX>()));
    };
    template <Uint32 INDEX>
    typename TrailingListType_t<INDEX>::T &trailing_list ()
    {
        return *static_cast<typename TrailingListType_t<INDEX>::T *>(&(this->Parent::template trailing_list<INDEX>()));
    };

    static std::string type_as_string () { return "MultiIndex_c<" + TypeStringOf_t<IndexTypeList>::eval() + '>'; }
};

// TODO: check if this is actually made, and if not, prohibit making them
template <>
struct MultiIndex_c<EmptyTypeList> : public List_t<EmptyTypeList>
{
//    enum { _ = Lvd::Meta::Assert<false>::v }; // don't make one of these

    typedef List_t<EmptyTypeList> Parent;
    using Parent::LENGTH;
    typedef EmptyTypeList IndexTypeList;
    typedef EmptyTypeList BodyIndexTypeList;
    typedef MultiIndex_c<BodyIndexTypeList> BodyMultiIndex;
    static Uint32 const COMPONENT_COUNT = 0;

    MultiIndex_c () { }

    bool operator == (MultiIndex_c const &m) const { return true; }  // there is only one of these, so it must be equal
    bool operator != (MultiIndex_c const &m) const { return false; } // there is only one of these, so it must not be unequal
    bool operator < (MultiIndex_c const &m) const { return false; }  // there is only one of these, so it can't be less than

    bool is_at_end () const { return true; }
    bool is_not_at_end () const { return false; }
    void operator ++ () { } // no-op
    void reset () { } // no-op

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because MultiIndex_c<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    BodyMultiIndex const &body () const { return *static_cast<BodyMultiIndex const *>(&Parent::body()); }
    BodyMultiIndex &body () { return *static_cast<BodyMultiIndex *>(&Parent::body()); }

    static std::string type_as_string () { return "MultiIndex_c<" + TypeStringOf_t<IndexTypeList>::eval() + '>'; }
};





// type-specific analogs of all the operator overloads for List_t

// tack an element onto the beginning of a list (where the list is empty)
template <typename HeadType>
inline MultiIndex_c<TypeList_t<HeadType> > operator >>= (HeadType const &head, MultiIndex_c<EmptyTypeList> const &)
{
    return MultiIndex_c<TypeList_t<HeadType> >(head);
}

// tack an element onto the beginning of a list (catch-all case)
template <typename HeadType, typename BodyTypeList>
inline MultiIndex_c<TypeList_t<HeadType,BodyTypeList> > operator >>= (HeadType const &head, MultiIndex_c<BodyTypeList> const &body)
{
    return MultiIndex_c<TypeList_t<HeadType,BodyTypeList> >(head, body);
}



// concatenate two lists (where both are empty)
inline MultiIndex_c<EmptyTypeList> operator |= (MultiIndex_c<EmptyTypeList> const &, MultiIndex_c<EmptyTypeList> const &)
{
    return MultiIndex_c<EmptyTypeList>();
}

// concatenate two lists (where the second is empty)
template <typename LeadingHeadType, typename LeadingBodyTypeList>
inline MultiIndex_c<TypeList_t<LeadingHeadType,LeadingBodyTypeList> > operator |= (
    MultiIndex_c<TypeList_t<LeadingHeadType,LeadingBodyTypeList> > const &leading_list,
    MultiIndex_c<EmptyTypeList> const &)
{
    return leading_list;
}

// concatenate two lists (where the first is empty)
template <typename TrailingHeadType, typename TrailingBodyTypeList>
inline MultiIndex_c<TypeList_t<TrailingHeadType,TrailingBodyTypeList> > operator |= (
    MultiIndex_c<EmptyTypeList> const &,
    MultiIndex_c<TypeList_t<TrailingHeadType,TrailingBodyTypeList> > const &trailing_type_list)
{
    return trailing_type_list;
}

// concatenate two lists (where the first has only one element)
template <typename LeadingHeadType, typename TrailingHeadType, typename TrailingBodyTypeList>
inline MultiIndex_c<TypeList_t<LeadingHeadType,TypeList_t<TrailingHeadType,TrailingBodyTypeList> > > operator |= (
    MultiIndex_c<TypeList_t<LeadingHeadType> > const &leading_list,
    MultiIndex_c<TypeList_t<TrailingHeadType,TrailingBodyTypeList> > const &trailing_list)
{
    return MultiIndex_c<TypeList_t<LeadingHeadType,TypeList_t<TrailingHeadType,TrailingBodyTypeList> > >(
        leading_list.head(),
        trailing_list);
}

// concatenate two lists (catch-all case)
template <typename LeadingTypeList, typename TrailingTypeList>
inline MultiIndex_c<typename ConcatenationOfTypeLists_t<LeadingTypeList,TrailingTypeList>::T> operator |= (
    MultiIndex_c<LeadingTypeList> const &leading_list,
    MultiIndex_c<TrailingTypeList> const &trailing_list)
{
    return MultiIndex_c<typename ConcatenationOfTypeLists_t<LeadingTypeList,TrailingTypeList>::T>(
        leading_list.head(),
        (leading_list.body() |= trailing_list));
}









// DomainIndexTypeList must be a TypeList_t of unique types
// CodomainIndexTypeList must be a TypeList_t consisting of a subset of DomainIndexTypeList
template <typename DomainIndexTypeList, typename CodomainIndexTypeList>
struct MultiIndexMap_t
{
    enum { STATIC_ASSERT_IN_ENUM((!ContainsDuplicates_t<DomainIndexTypeList>::V), DOMAIN_INDICES_MUST_NOT_CONTAIN_DUPLICATES) };

    typedef MultiIndex_c<DomainIndexTypeList> DomainIndex;
    typedef MultiIndex_c<CodomainIndexTypeList> CodomainIndex;
    typedef CodomainIndex (*EvalMapType) (DomainIndex const &);
    static CodomainIndex eval (DomainIndex const &i)
    {
        return CodomainIndex(i.template el<DomainIndexTypeList::template IndexOf_t<typename CodomainIndexTypeList::HeadType>::V>(),
                             MultiIndexMap_t<DomainIndexTypeList,typename CodomainIndexTypeList::BodyTypeList>::eval(i));
    }
};

template <typename DomainIndexTypeList, typename CodomainIndexType>
struct MultiIndexMap_t<DomainIndexTypeList,TypeList_t<CodomainIndexType> >
{
    enum { STATIC_ASSERT_IN_ENUM((!ContainsDuplicates_t<DomainIndexTypeList>::V), DOMAIN_INDICES_MUST_NOT_CONTAIN_DUPLICATES) };

    typedef TypeList_t<CodomainIndexType> CodomainIndexTypeList;
    typedef MultiIndex_c<DomainIndexTypeList> DomainIndex;
    typedef MultiIndex_c<CodomainIndexTypeList> CodomainIndex;
    typedef CodomainIndex (*EvalMapType) (DomainIndex const &);
    static CodomainIndex eval (DomainIndex const &i)
    {
        return CodomainIndex(i.template el<DomainIndexTypeList::template IndexOf_t<typename CodomainIndexTypeList::HeadType>::V>());
    }
};

} // end of namespace Tenh

#endif // TENH_MULTIINDEX_HPP_
