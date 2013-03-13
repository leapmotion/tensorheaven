#ifndef COMPOUND_ACCESS_INDEX_HPP_
#define COMPOUND_ACCESS_INDEX_HPP_

#include "list.hpp"
#include "typelist_utility.hpp"

// AccessIndexTypeList should be a TypeList_t containing AccessIndex types (e.g. Vector_t::Index)
template <typename AccessIndexTypeList_>
struct CompoundAccessIndex_t : List_t<AccessIndexTypeList_>
{
    typedef List_t<AccessIndexTypeList_> Parent;
    typedef AccessIndexTypeList_ AccessIndexTypeList;
    // TODO: static Uint32 const DIM (this must be recursively defined)
    
    CompoundAccessIndex_t () { } // default constructor initializes to "first" component
    template <typename LeadingAccessIndexTypeList>
    CompoundAccessIndex_t (CompoundAccessIndex_t<LeadingAccessIndexTypeList> const &leading_compound_access_index) 
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
    // works because CompoundAccessIndex_t<AccessIndexTypeList> inherits non-virtually from 
    // List_t<AccessIndexTypeList> and has no members.
    typedef CompoundAccessIndex_t<typename AccessIndexTypeList::Body> BodyList;
    BodyList &body ()
    { 
        return *static_cast<BodyList *>(&Parent::body()); 
    }
    BodyList const &body () const
    { 
        return *static_cast<BodyList const *>(&Parent::body()); 
    }
    
    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because CompoundAccessIndex_t<AccessIndexTypeList> inherits non-virtually from 
    // List_t<AccessIndexTypeList> and has no members.
    // returns the type of the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingListType_t
    {
        typedef CompoundAccessIndex_t<typename AccessIndexTypeList::template TrailingTypeList_t<INDEX>::T> T;
    };

    // returns the trailing CompoundAccessIndex_t starting at the INDEXth element
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

// template specializations for the AccessIndexTypeList list corner cases
template <typename AccessIndexHead>
struct CompoundAccessIndex_t<TypeList_t<AccessIndexHead> > : public List_t<TypeList_t<AccessIndexHead> >
{
    typedef List_t<TypeList_t<AccessIndexHead> > Parent;
    typedef TypeList_t<AccessIndexHead> AccessIndexTypeList;
    // TODO: static Uint32 const DIM
    
    CompoundAccessIndex_t () { } // default constructor initializes to "first" component
    CompoundAccessIndex_t (AccessIndexHead const &head) : Parent(head) { }
    template <typename LeadingAccessIndexTypeList>
    CompoundAccessIndex_t (CompoundAccessIndex_t<LeadingAccessIndexTypeList> const &leading_compound_access_index) 
        : 
        Parent(leading_compound_access_index)
    { }
    
    bool is_at_end () const { return this->head().is_at_end(); }
    bool is_not_at_end () const { return this->head().is_not_at_end(); }
    void operator ++ () { ++(this->head()); }
    void reset () { this->head().reset(); }

    // TEMP
    void print (std::ostream &out) const { out << this->head().value() << ", "; }

    // type conversion operator -- because this CompoundAccessIndex_t only has one component,
    // it can be canonically identified as its component type.
    operator AccessIndexHead const & () const { return this->head(); }
    operator AccessIndexHead & () { return this->head(); }

    // slighty hacky way to use List_t's existing functionality -- NOTE: this only
    // works because CompoundAccessIndex_t<AccessIndexTypeList> inherits non-virtually from 
    // List_t<AccessIndexTypeList> and has no members.
    // returns the type of the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingListType_t
    {
        typedef CompoundAccessIndex_t<typename AccessIndexTypeList::template TrailingTypeList_t<INDEX>::T> T;
    };

    // returns the trailing CompoundAccessIndex_t starting at the INDEXth element
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
struct CompoundAccessIndex_t<EmptyTypeList> : public List_t<EmptyTypeList>
{
//    enum { _ = Lvd::Meta::Assert<false>::v }; // don't make one of these

    typedef List_t<EmptyTypeList> Parent;
    typedef EmptyTypeList AccessIndexTypeList;
    // TODO: static Uint32 const DIM
    
//     CompoundAccessIndex_t () { } // default constructor initializes to "first" component
//     
//     bool is_at_end () const { return head().is_at_end(); }
//     bool is_not_at_end () const { return head().is_not_at_end(); }
//     void operator ++ () { ++(head()); }
//     void reset () { head().reset(); }
};












template <typename TypeList, typename UniqueTypes_ = typename UniqueTypesIn_t<TypeList>::T>
struct CompoundAccessIndexMap_t
{
private:

    typedef UniqueTypes_ UniqueTypes;
    static Uint32 const HEAD_INDEX = UniqueTypes::template IndexOf_t<typename TypeList::Head>::V;
    
public:

    typedef CompoundAccessIndex_t<TypeList> (*EvalMapType) (CompoundAccessIndex_t<UniqueTypes> const &index);
    static CompoundAccessIndex_t<TypeList> eval (CompoundAccessIndex_t<UniqueTypes> const &index)
    {
        return CompoundAccessIndex_t<TypeList>(index.template value<HEAD_INDEX>(),
                                               CompoundAccessIndexMap_t<typename TypeList::Body, UniqueTypes>::eval(index));
    }
};

template <typename Head, typename UniqueTypes_>
struct CompoundAccessIndexMap_t<TypeList_t<Head>, UniqueTypes_>
{
private:

    typedef UniqueTypes_ UniqueTypes;
    static Uint32 const HEAD_INDEX = 0;

public:

    typedef CompoundAccessIndex_t<TypeList_t<Head> > (*EvalMapType) (CompoundAccessIndex_t<UniqueTypes> const &index);
    static CompoundAccessIndex_t<TypeList_t<Head> > eval (CompoundAccessIndex_t<UniqueTypes> const &index)
    {
        return CompoundAccessIndex_t<TypeList_t<Head> >(index.template el<HEAD_INDEX>());
    }
};

#endif // COMPOUND_ACCESS_INDEX_HPP_
