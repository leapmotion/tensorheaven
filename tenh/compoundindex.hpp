// ///////////////////////////////////////////////////////////////////////////
// tenh/compoundindex.hpp by Victor Dods, created 2013/03/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_COMPOUNDINDEX_HPP_
#define TENH_COMPOUNDINDEX_HPP_

#include "tenh/core.hpp"
#include "tenh/meta/list.hpp"
#include "tenh/meta/typelist_utility.hpp"

namespace Tenh {

// IndexTypeList should be a TypeList_t containing Index types (e.g. Vector_t::Index)
template <typename IndexTypeList_>
struct CompoundIndex_t : List_t<IndexTypeList_>
{
    typedef List_t<IndexTypeList_> Parent;
    typedef IndexTypeList_ IndexTypeList;
    typedef typename IndexTypeList::HeadType HeadIndexType;
    typedef typename IndexTypeList::BodyTypeList BodyIndexTypeList;
    typedef CompoundIndex_t<BodyIndexTypeList> BodyCompoundIndex;
    static Uint32 const COMPONENT_COUNT = HeadIndexType::COMPONENT_COUNT * BodyCompoundIndex::COMPONENT_COUNT;

    CompoundIndex_t () { } // default constructor initializes to "first" component
    CompoundIndex_t (HeadIndexType const &head, BodyCompoundIndex const &body) : Parent(head, body) { }
    // tuple-like initializers
    CompoundIndex_t (Uint32 i0, Uint32 i1) // TODO: replace with IndexTypeList::HeadType i0, IndexTypeList::BodyTypeList::HeadType i1 ?
        :
        Parent(HeadIndexType(i0), BodyCompoundIndex(i1))
    {
        Lvd::Meta::Assert<Parent::LENGTH == 2>();
    }
    CompoundIndex_t (Uint32 i0, Uint32 i1, Uint32 i2)
        :
        Parent(HeadIndexType(i0), BodyCompoundIndex(i1, i2))
    {
        Lvd::Meta::Assert<Parent::LENGTH == 3>();
    }
    CompoundIndex_t (Uint32 i0, Uint32 i1, Uint32 i2, Uint32 i3)
        :
        Parent(HeadIndexType(i0), BodyCompoundIndex(i1, i2, i3))
    {
        Lvd::Meta::Assert<Parent::LENGTH == 4>();
    }

    CompoundIndex_t (CompoundIndex_t<EmptyTypeList> const &) { } // default construction
    CompoundIndex_t (CompoundIndex_t<TypeList_t<HeadIndexType> > const &leading_compound_index)
        :
        Parent(leading_compound_index)
    { }
    // this includes the copy constructor
    template <typename OtherHeadIndexType, typename OtherBodyIndexTypeList>
    CompoundIndex_t (CompoundIndex_t<TypeList_t<OtherHeadIndexType,OtherBodyIndexTypeList> > const &leading_compound_index)
        :
        Parent(leading_compound_index)
    { }

    bool operator == (CompoundIndex_t const &c) const { return this->head() == c.head() && body() == c.body(); }
    bool operator != (CompoundIndex_t const &c) const { return this->head() != c.head() || body() != c.body(); }
    bool operator < (CompoundIndex_t const &c) const { return this->head() < c.head() && body() < c.body(); }

    bool is_at_end () const { return this->head().is_at_end(); } // because the head is the last one incremented
    bool is_not_at_end () const { return this->head().is_not_at_end(); } // because the head is the last one incremented
    Uint32 value () const { return BodyCompoundIndex::COMPONENT_COUNT*this->head().value() + this->body().value(); }
    // TODO: think about adding a redundant single index that just increments and can be returned directly in value()
    void operator ++ ()
    {
        BodyCompoundIndex &b = body();
        ++b; // increment the body index
        if (b.is_at_end()) // if it hit the end, reset it and increment the head
        {
            b.reset();
            ++(this->head());
        }
    }
    void reset () { this->head().reset(); body().reset(); }

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because CompoundIndex_t<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    BodyCompoundIndex const &body () const { return *static_cast<BodyCompoundIndex const *>(&Parent::body()); }
    BodyCompoundIndex &body () { return *static_cast<BodyCompoundIndex *>(&Parent::body()); }

    // TODO: rename to LeadingCompoundIndex_t

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because CompoundIndex_t<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the CompoundIndex_t having the specified range
    template <Uint32 START_INDEX, Uint32 END_INDEX>
    struct RangeType_t
    {
        typedef CompoundIndex_t<typename IndexTypeList::template Range_t<START_INDEX,END_INDEX>::T> T;
    };

    // returns the CompoundIndex_t which is the given range of elements
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
    // works because CompoundIndex_t<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the leading CompoundIndex_t ending at the INDEXth element
    template <Uint32 INDEX>
    struct LeadingListType_t
    {
//         typedef List_t<typename TypeList::template LeadingTypeList_t<INDEX>::T> T;
        typedef CompoundIndex_t<typename IndexTypeList::template LeadingTypeList_t<INDEX>::T> T;
    };

    // TODO: rename to leading_compound_index
    // returns the leading CompoundIndex_t ending at the INDEXth element.
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

    // TODO: rename to TrailingCompoundIndex_t

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because CompoundIndex_t<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingListType_t
    {
        typedef CompoundIndex_t<typename IndexTypeList::template TrailingTypeList_t<INDEX>::T> T;
    };

    // TODO: rename to trailing_compound_index
    // returns the trailing CompoundIndex_t starting at the INDEXth element
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

    static std::string type_as_string () { return "CompoundIndex_t<" + TypeStringOf_t<IndexTypeList>::eval() + '>'; }
};

// template specializations for the IndexTypeList list corner cases
template <typename HeadIndexType>
struct CompoundIndex_t<TypeList_t<HeadIndexType> > : public List_t<TypeList_t<HeadIndexType> >
{
    typedef List_t<TypeList_t<HeadIndexType> > Parent;
    typedef TypeList_t<HeadIndexType> IndexTypeList;
    typedef typename IndexTypeList::BodyTypeList BodyIndexTypeList;
    typedef CompoundIndex_t<BodyIndexTypeList> BodyCompoundIndex;
    static Uint32 const COMPONENT_COUNT = HeadIndexType::COMPONENT_COUNT;

    CompoundIndex_t () { } // default constructor initializes to "first" component
    // explicit because it has a range-check (in the HeadIndexType constructor)
    explicit CompoundIndex_t (Uint32 i) : Parent(HeadIndexType(i)) { }
    CompoundIndex_t (HeadIndexType const &head) : Parent(head) { }

    CompoundIndex_t (CompoundIndex_t<EmptyTypeList> const &) { } // default construction
    // this is the copy constructor
    CompoundIndex_t (CompoundIndex_t const &c)
        :
        Parent(c)
    { }
    template <typename OtherHeadIndexType, typename OtherBodyIndexTypeList>
    CompoundIndex_t (CompoundIndex_t<TypeList_t<OtherHeadIndexType,OtherBodyIndexTypeList> > const &leading_compound_index)
        :
        Parent(leading_compound_index)
    { }

    bool operator == (CompoundIndex_t const &c) const { return this->head() == c.head(); }
    bool operator != (CompoundIndex_t const &c) const { return this->head() != c.head(); }
    bool operator < (CompoundIndex_t const &c) const { return this->head() < c.head(); }

    bool is_at_end () const { return this->head().is_at_end(); }
    bool is_not_at_end () const { return this->head().is_not_at_end(); }
    Uint32 value () const { return this->head().value(); }
    void operator ++ () { ++(this->head()); }
    void reset () { this->head().reset(); }

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because CompoundIndex_t<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    BodyCompoundIndex const &body () const { return *static_cast<BodyCompoundIndex const *>(&Parent::body()); }
    BodyCompoundIndex &body () { return *static_cast<BodyCompoundIndex *>(&Parent::body()); }

    // type conversion operator -- because this CompoundIndex_t only has one component,
    // it can be canonically identified as its component type.
    operator HeadIndexType const & () const { return this->head(); }
    operator HeadIndexType & () { return this->head(); }

    // TODO: rename to LeadingCompoundIndex_t

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because CompoundIndex_t<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the CompoundIndex_t having the specified range
    template <Uint32 START_INDEX, Uint32 END_INDEX>
    struct RangeType_t
    {
        typedef CompoundIndex_t<typename IndexTypeList::template Range_t<START_INDEX,END_INDEX>::T> T;
    };

    // returns the CompoundIndex_t which is the given range of elements
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
    // works because CompoundIndex_t<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the leading CompoundIndex_t ending at the INDEXth element
    template <Uint32 INDEX>
    struct LeadingListType_t
    {
//         typedef List_t<typename TypeList::template LeadingTypeList_t<INDEX>::T> T;
        typedef CompoundIndex_t<typename IndexTypeList::template LeadingTypeList_t<INDEX>::T> T;
    };

    // TODO: rename to leading_compound_index
    // returns the leading CompoundIndex_t ending at the INDEXth element.
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
    // works because CompoundIndex_t<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    // returns the type of the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingListType_t
    {
        typedef CompoundIndex_t<typename IndexTypeList::template TrailingTypeList_t<INDEX>::T> T;
    };

    // returns the trailing CompoundIndex_t starting at the INDEXth element
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

    static std::string type_as_string () { return "CompoundIndex_t<" + TypeStringOf_t<IndexTypeList>::eval() + '>'; }
};

// TODO: check if this is actually made, and if not, prohibit making them
template <>
struct CompoundIndex_t<EmptyTypeList> : public List_t<EmptyTypeList>
{
//    enum { _ = Lvd::Meta::Assert<false>::v }; // don't make one of these

    typedef List_t<EmptyTypeList> Parent;
    typedef EmptyTypeList IndexTypeList;
    typedef EmptyTypeList BodyIndexTypeList;
    typedef CompoundIndex_t<BodyIndexTypeList> BodyCompoundIndex;
    static Uint32 const COMPONENT_COUNT = 0;

    CompoundIndex_t () { }

    bool operator == (CompoundIndex_t const &c) const { return true; }  // there is only one of these, so it must be equal
    bool operator != (CompoundIndex_t const &c) const { return false; } // there is only one of these, so it must not be unequal
    bool operator < (CompoundIndex_t const &c) const { return false; }  // there is only one of these, so it can't be less than

    bool is_at_end () const { return true; }
    bool is_not_at_end () const { return false; }
    void operator ++ () { } // no-op
    void reset () { } // no-op

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because CompoundIndex_t<IndexTypeList> inherits non-virtually from
    // List_t<IndexTypeList> and has no members.
    BodyCompoundIndex const &body () const { return *static_cast<BodyCompoundIndex const *>(&Parent::body()); }
    BodyCompoundIndex &body () { return *static_cast<BodyCompoundIndex *>(&Parent::body()); }

    static std::string type_as_string () { return "CompoundIndex_t<" + TypeStringOf_t<IndexTypeList>::eval() + '>'; }
};





// type-specific analogs of all the operator overloads for List_t

// tack an element onto the beginning of a list (where the list is empty)
template <typename HeadType>
inline CompoundIndex_t<TypeList_t<HeadType> > operator >>= (HeadType const &head, CompoundIndex_t<EmptyTypeList> const &)
{
    return CompoundIndex_t<TypeList_t<HeadType> >(head);
}

// tack an element onto the beginning of a list (catch-all case)
template <typename HeadType, typename BodyTypeList>
inline CompoundIndex_t<TypeList_t<HeadType,BodyTypeList> > operator >>= (HeadType const &head, CompoundIndex_t<BodyTypeList> const &body)
{
    return CompoundIndex_t<TypeList_t<HeadType,BodyTypeList> >(head, body);
}



// concatenate two lists (where both are empty)
inline CompoundIndex_t<EmptyTypeList> operator |= (CompoundIndex_t<EmptyTypeList> const &, CompoundIndex_t<EmptyTypeList> const &)
{
    return CompoundIndex_t<EmptyTypeList>();
}

// concatenate two lists (where the second is empty)
template <typename LeadingHeadType, typename LeadingBodyTypeList>
inline CompoundIndex_t<TypeList_t<LeadingHeadType,LeadingBodyTypeList> > operator |= (
    CompoundIndex_t<TypeList_t<LeadingHeadType,LeadingBodyTypeList> > const &leading_list,
    CompoundIndex_t<EmptyTypeList> const &)
{
    return leading_list;
}

// concatenate two lists (where the first is empty)
template <typename TrailingHeadType, typename TrailingBodyTypeList>
inline CompoundIndex_t<TypeList_t<TrailingHeadType,TrailingBodyTypeList> > operator |= (
    CompoundIndex_t<EmptyTypeList> const &,
    CompoundIndex_t<TypeList_t<TrailingHeadType,TrailingBodyTypeList> > const &trailing_type_list)
{
    return trailing_type_list;
}

// concatenate two lists (where the first has only one element)
template <typename LeadingHeadType, typename TrailingHeadType, typename TrailingBodyTypeList>
inline CompoundIndex_t<TypeList_t<LeadingHeadType,TypeList_t<TrailingHeadType,TrailingBodyTypeList> > > operator |= (
    CompoundIndex_t<TypeList_t<LeadingHeadType> > const &leading_list,
    CompoundIndex_t<TypeList_t<TrailingHeadType,TrailingBodyTypeList> > const &trailing_list)
{
    return CompoundIndex_t<TypeList_t<LeadingHeadType,TypeList_t<TrailingHeadType,TrailingBodyTypeList> > >(
        leading_list.head(),
        trailing_list);
}

// concatenate two lists (catch-all case)
template <typename LeadingTypeList, typename TrailingTypeList>
inline CompoundIndex_t<typename ConcatenationOfTypeLists_t<LeadingTypeList,TrailingTypeList>::T> operator |= (
    CompoundIndex_t<LeadingTypeList> const &leading_list,
    CompoundIndex_t<TrailingTypeList> const &trailing_list)
{
    return CompoundIndex_t<typename ConcatenationOfTypeLists_t<LeadingTypeList,TrailingTypeList>::T>(
        leading_list.head(),
        (leading_list.body() |= trailing_list));
}









// DomainIndexTypeList must be a TypeList_t of unique types
// CodomainIndexTypeList must be a TypeList_t consisting of a subset of DomainIndexTypeList
template <typename DomainIndexTypeList, typename CodomainIndexTypeList>
struct CompoundIndexMap_t
{
    enum { _ = Lvd::Meta::Assert<!ContainsDuplicates_t<DomainIndexTypeList>::V>::v };

    typedef CompoundIndex_t<DomainIndexTypeList> DomainIndex;
    typedef CompoundIndex_t<CodomainIndexTypeList> CodomainIndex;
    typedef CodomainIndex (*EvalMapType) (DomainIndex const &);
    static CodomainIndex eval (DomainIndex const &i)
    {
        return CodomainIndex(i.template el<DomainIndexTypeList::template IndexOf_t<typename CodomainIndexTypeList::HeadType>::V>(),
                             CompoundIndexMap_t<DomainIndexTypeList,typename CodomainIndexTypeList::BodyTypeList>::eval(i));
    }
};

template <typename DomainIndexTypeList, typename CodomainIndexType>
struct CompoundIndexMap_t<DomainIndexTypeList,TypeList_t<CodomainIndexType> >
{
    enum { _ = Lvd::Meta::Assert<!ContainsDuplicates_t<DomainIndexTypeList>::V>::v };

    typedef TypeList_t<CodomainIndexType> CodomainIndexTypeList;
    typedef CompoundIndex_t<DomainIndexTypeList> DomainIndex;
    typedef CompoundIndex_t<CodomainIndexTypeList> CodomainIndex;
    typedef CodomainIndex (*EvalMapType) (DomainIndex const &);
    static CodomainIndex eval (DomainIndex const &i)
    {
        return CodomainIndex(i.template el<DomainIndexTypeList::template IndexOf_t<typename CodomainIndexTypeList::HeadType>::V>());
    }
};

// template <typename DomainIndexTypeList>
// struct CompoundIndexMap_t<DomainIndexTypeList,EmptyTypeList>
// {
//     typedef EmptyTypeList CodomainIndexTypeList;
//     typedef CompoundIndex_t<DomainIndexTypeList> DomainIndex;
//     typedef CompoundIndex_t<CodomainIndexTypeList> CodomainIndex;
//     typedef CodomainIndex (*EvalMapType) (DomainIndex const &);
//     static CodomainIndex eval (DomainIndex const &i)
//     {
//         return CodomainIndex();
//     }
// };

} // end of namespace Tenh

#endif // TENH_COMPOUNDINDEX_HPP_
