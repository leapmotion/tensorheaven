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
    // TODO: static Uint32 const DIM (this must be recursively defined)
    
    CompoundIndex_t () { } // default constructor initializes to "first" component
    template <typename LeadingIndexTypeList>
    CompoundIndex_t (CompoundIndex_t<LeadingIndexTypeList> const &leading_compound_access_index) 
        : 
        Parent(leading_compound_access_index)
    { }
    
    bool is_at_end () const { return this->head().is_at_end(); } // because the head is the last one incremented
    bool is_not_at_end () const { return this->head().is_not_at_end(); } // because the head is the last one incremented
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
    
    // TEMP
    void print (std::ostream &out) const { out << this->head().value() << ", "; body().print(out); }
    
    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because CompoundIndex_t<IndexTypeList> inherits non-virtually from 
    // List_t<IndexTypeList> and has no members.
    typedef CompoundIndex_t<typename IndexTypeList::Body> BodyList;
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
};

// template specializations for the IndexTypeList list corner cases
template <typename IndexHead>
struct CompoundIndex_t<TypeList_t<IndexHead> > : public List_t<TypeList_t<IndexHead> >
{
    typedef List_t<TypeList_t<IndexHead> > Parent;
    typedef TypeList_t<IndexHead> IndexTypeList;
    // TODO: static Uint32 const DIM
    
    CompoundIndex_t () { } // default constructor initializes to "first" component
    CompoundIndex_t (IndexHead const &head) : Parent(head) { }
    template <typename LeadingIndexTypeList>
    CompoundIndex_t (CompoundIndex_t<LeadingIndexTypeList> const &leading_compound_access_index) 
        : 
        Parent(leading_compound_access_index)
    { }
    
    bool is_at_end () const { return this->head().is_at_end(); }
    bool is_not_at_end () const { return this->head().is_not_at_end(); }
    void operator ++ () { ++(this->head()); }
    void reset () { this->head().reset(); }

    // TEMP
    void print (std::ostream &out) const { out << this->head().value() << ", "; }

    // type conversion operator -- because this CompoundIndex_t only has one component,
    // it can be canonically identified as its component type.
    operator IndexHead const & () const { return this->head(); }
    operator IndexHead & () { return this->head(); }

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
};

template <>    
struct CompoundIndex_t<EmptyTypeList> : public List_t<EmptyTypeList>
{
//    enum { _ = Lvd::Meta::Assert<false>::v }; // don't make one of these

    typedef List_t<EmptyTypeList> Parent;
    typedef EmptyTypeList IndexTypeList;
    // TODO: static Uint32 const DIM
    
//     CompoundIndex_t () { } // default constructor initializes to "first" component
//     
//     bool is_at_end () const { return head().is_at_end(); }
//     bool is_not_at_end () const { return head().is_not_at_end(); }
//     void operator ++ () { ++(head()); }
//     void reset () { head().reset(); }
};












template <typename TypeList, typename UniqueTypes_ = typename UniqueTypesIn_t<TypeList>::T>
struct CompoundIndexMap_t
{
private:

    typedef UniqueTypes_ UniqueTypes;
    static Uint32 const HEAD_INDEX = UniqueTypes::template IndexOf_t<typename TypeList::Head>::V;
    
public:

    typedef CompoundIndex_t<TypeList> (*EvalMapType) (CompoundIndex_t<UniqueTypes> const &index);
    static CompoundIndex_t<TypeList> eval (CompoundIndex_t<UniqueTypes> const &index)
    {
        return CompoundIndex_t<TypeList>(index.template value<HEAD_INDEX>(),
                                               CompoundIndexMap_t<typename TypeList::Body, UniqueTypes>::eval(index));
    }
};

template <typename Head, typename UniqueTypes_>
struct CompoundIndexMap_t<TypeList_t<Head>, UniqueTypes_>
{
private:

    typedef UniqueTypes_ UniqueTypes;
    static Uint32 const HEAD_INDEX = 0;

public:

    typedef CompoundIndex_t<TypeList_t<Head> > (*EvalMapType) (CompoundIndex_t<UniqueTypes> const &index);
    static CompoundIndex_t<TypeList_t<Head> > eval (CompoundIndex_t<UniqueTypes> const &index)
    {
        return CompoundIndex_t<TypeList_t<Head> >(index.template el<HEAD_INDEX>());
    }
};

#endif // COMPOUND_INDEX_HPP_
