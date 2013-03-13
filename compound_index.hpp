#ifndef COMPOUND_INDEX_HPP_
#define COMPOUND_INDEX_HPP_

#include "list.hpp"
#include "typelist_utility.hpp"

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

    CompoundIndex_t (CompoundIndex_t<EmptyTypeList> const &) { } // default construction
    CompoundIndex_t (CompoundIndex_t<TypeList_t<HeadIndexType> > const &leading_compound_index)
        :
        Parent(leading_compound_index)
    { }
    // this includes the copy constructor
    template <typename BodyIndexTypeList>
    CompoundIndex_t (CompoundIndex_t<TypeList_t<HeadIndexType,BodyIndexTypeList> > const &leading_compound_index)
        :
        Parent(leading_compound_index)
    { }

    bool is_at_end () const { return this->head().is_at_end(); } // because the head is the last one incremented
    bool is_not_at_end () const { return this->head().is_not_at_end(); } // because the head is the last one incremented
    Uint32 value () const { return BodyCompoundIndex::COMPONENT_COUNT*this->head().value() + this->body().value(); } 
    void operator ++ ()
    {
        BodyList &b = body();
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
    typedef CompoundIndex_t<typename IndexTypeList::BodyTypeList> BodyList;
    BodyList &body ()
    {
        return *static_cast<BodyList *>(&Parent::body());
    }
    BodyList const &body () const
    {
        return *static_cast<BodyList const *>(&Parent::body());
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

// template specializations for the IndexTypeList list corner cases
template <typename HeadIndexType>
struct CompoundIndex_t<TypeList_t<HeadIndexType> > : public List_t<TypeList_t<HeadIndexType> >
{
    typedef List_t<TypeList_t<HeadIndexType> > Parent;
    typedef TypeList_t<HeadIndexType> IndexTypeList;
    static Uint32 const COMPONENT_COUNT = HeadIndexType::COMPONENT_COUNT;

    CompoundIndex_t () { } // default constructor initializes to "first" component
    CompoundIndex_t (HeadIndexType const &head) : Parent(head) { }

    CompoundIndex_t (CompoundIndex_t<EmptyTypeList> const &) { } // default construction
    // this is the copy constructor
    CompoundIndex_t (CompoundIndex_t const &leading_compound_index)
        :
        Parent(leading_compound_index)
    { }
    template <typename BodyIndexTypeList>
    CompoundIndex_t (CompoundIndex_t<TypeList_t<HeadIndexType,BodyIndexTypeList> > const &leading_compound_index)
        :
        Parent(leading_compound_index)
    { }

    bool is_at_end () const { return this->head().is_at_end(); }
    bool is_not_at_end () const { return this->head().is_not_at_end(); }
    Uint32 value () const { return this->head().value(); }
    void operator ++ () { ++(this->head()); }
    void reset () { this->head().reset(); }

    // type conversion operator -- because this CompoundIndex_t only has one component,
    // it can be canonically identified as its component type.
    operator HeadIndexType const & () const { return this->head(); }
    operator HeadIndexType & () { return this->head(); }

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

template <>
struct CompoundIndex_t<EmptyTypeList> : public List_t<EmptyTypeList>
{
//    enum { _ = Lvd::Meta::Assert<false>::v }; // don't make one of these

    typedef List_t<EmptyTypeList> Parent;
    typedef EmptyTypeList IndexTypeList;
//     static Uint32 const COMPONENT_COUNT

    CompoundIndex_t () : m_value(0) { } // default constructor initializes to "first" component (which in this case happens to be at end)

    bool is_at_end () const { return m_value > 0; }
    bool is_not_at_end () const { return m_value == 0; }
    void operator ++ () { ++m_value; } // no-op
    void reset () { m_value = 0; } // no-op

    static std::string type_as_string () { return "CompoundIndex_t<" + TypeStringOf_t<IndexTypeList>::eval() + '>'; }

private:

    Uint32 m_value;
};










// DomainIndexTypeList must be a TypeList_t of unique types
// CodomainIndexTypeList must be a TypeList_t consisting of a subset of DomainIndexTypeList
template <typename DomainIndexTypeList, typename CodomainIndexTypeList>
struct CompoundIndexMap_t
{
    enum { _ = Lvd::Meta::Assert<(DomainIndexTypeList::LENGTH == UniqueTypesIn_t<DomainIndexTypeList>::T::LENGTH)>::v };

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
    enum { _ = Lvd::Meta::Assert<(DomainIndexTypeList::LENGTH == UniqueTypesIn_t<DomainIndexTypeList>::T::LENGTH)>::v };

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







// template <typename TypeList, typename UniqueTypes_ = typename UniqueTypesIn_t<TypeList>::T>
// struct CompoundIndexMap_t
// {
// private:
//
//     typedef UniqueTypes_ UniqueTypes;
//     static Uint32 const HEAD_INDEX = UniqueTypes::template IndexOf_t<typename TypeList::HeadType>::V;
//
// public:
//
//     typedef CompoundIndex_t<TypeList> (*EvalMapType) (CompoundIndex_t<UniqueTypes> const &index);
//     static CompoundIndex_t<TypeList> eval (CompoundIndex_t<UniqueTypes> const &index)
//     {
//         return CompoundIndex_t<TypeList>(index.template value<HEAD_INDEX>(),
//                                                CompoundIndexMap_t<typename TypeList::BodyTypeList, UniqueTypes>::eval(index));
//     }
// };
//
// template <typename HeadType, typename UniqueTypes_>
// struct CompoundIndexMap_t<TypeList_t<HeadType>, UniqueTypes_>
// {
// private:
//
//     typedef UniqueTypes_ UniqueTypes;
//     static Uint32 const HEAD_INDEX = 0;
//
// public:
//
//     typedef CompoundIndex_t<TypeList_t<HeadType> > (*EvalMapType) (CompoundIndex_t<UniqueTypes> const &index);
//     static CompoundIndex_t<TypeList_t<HeadType> > eval (CompoundIndex_t<UniqueTypes> const &index)
//     {
//         return CompoundIndex_t<TypeList_t<HeadType> >(index.template el<HEAD_INDEX>());
//     }
// };

#endif // COMPOUND_INDEX_HPP_
