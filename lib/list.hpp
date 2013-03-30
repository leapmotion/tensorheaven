#ifndef LIST_HPP_
#define LIST_HPP_

#include <ostream>
#include <string>

#include "core.hpp"
#include "typelist.hpp"
#include "typelist_utility.hpp"

template <typename TypeList, Uint32 INDEX> struct ListHelper_t;

template <typename TypeList_>
struct List_t
{
    typedef TypeList_ TypeList;
    typedef typename TypeList::HeadType HeadType;
    typedef typename TypeList::BodyTypeList BodyTypeList;
    typedef List_t<BodyTypeList> BodyList;
    static Uint32 const LENGTH = TypeList::LENGTH;

    List_t () { }
    List_t (HeadType const &head, BodyList const &body) : m_head(head), m_body(body) { }

    // the following constructors initialize the first elements that are common to both
    // list types, doing default construction for all remaining elements, if any.
    List_t (List_t<EmptyTypeList> const &) { } // default construction
    // construction from single-element leading lists -- default constructor for body
    List_t (List_t<TypeList_t<HeadType> > const &leading_list) : m_head(leading_list.head()) { }
    // construction from leading lists having more than one element
    template <typename BodyTypeList>
    List_t (List_t<TypeList_t<HeadType,BodyTypeList> > const &leading_list)
        :
        m_head(leading_list.head()),
        m_body(leading_list.body())
    { }

    HeadType const &head () const { return m_head; }
    HeadType &head () { return m_head; }
    BodyList const &body () const { return m_body; }
    BodyList &body () { return m_body; }

    // returns the INDEXth element type of this List_t
    template <Uint32 INDEX>
    struct Type_t
    {
        typedef typename TypeList::template El_t<INDEX>::T T;
    };

    // returns the INDEXth element of this List_t
    template <Uint32 INDEX>
    typename Type_t<INDEX>::T const &el () const
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return ListHelper_t<TypeList,INDEX>::el(*this);
    }
    template <Uint32 INDEX>
    typename Type_t<INDEX>::T &el ()
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return ListHelper_t<TypeList,INDEX>::el(*this);
    }

    // returns the type of the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingListType_t
    {
        typedef List_t<typename TypeList::template TrailingTypeList_t<INDEX>::T> T;
    };

    // returns the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    typename TrailingListType_t<INDEX>::T const &trailing_list () const
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return ListHelper_t<TypeList,INDEX>::trailing_list(*this);
    };
    template <Uint32 INDEX>
    typename TrailingListType_t<INDEX>::T &trailing_list ()
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return ListHelper_t<TypeList,INDEX>::trailing_list(*this);
    };

    void print (std::ostream &out, bool print_parens = true) const
    {
        if (print_parens)
            out << '(';
        out << m_head << ", ";
        m_body.print(out, false);
        if (print_parens)
            out << ')';
    }

    static std::string type_as_string () { return "List_t<" + TypeStringOf_t<TypeList>::eval() + '>'; }

private:

    HeadType m_head;
    BodyList m_body;

    template <typename TypeList__, Uint32 INDEX_> friend struct ListHelper_t;
};

// try not to actually construct a List_t<EmptyTypeList>, because it isn't guaranteed to take 0 memory
template <>
struct List_t<EmptyTypeList>
{
    typedef EmptyTypeList TypeList;
    typedef NullType HeadType;
    typedef EmptyTypeList BodyTypeList;
    typedef List_t<EmptyTypeList> BodyList;
    static Uint32 const LENGTH = 0;
    static List_t const SINGLETON; // so you never have to allocate another -- it's not guaranteed to take 0 memory

    List_t () { }
//     template <typename TypeList>
//     List_t (List_t<TypeList> const &) { }

    // there is no head, so there is no accessors for it.
    List_t<EmptyTypeList> const &body () const { return List_t<EmptyTypeList>::SINGLETON; } // TEMP?

    template <Uint32 INDEX>
    struct Type_t
    {
        typedef typename TypeList::template El_t<INDEX>::T T;
    };

    template <Uint32 INDEX>
    typename Type_t<INDEX>::T const &el () const
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return ListHelper_t<TypeList,INDEX>::value(*this);
    }
    template <Uint32 INDEX>
    typename Type_t<INDEX>::T &el ()
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return ListHelper_t<TypeList,INDEX>::value(*this);
    }

    // returns the type of the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingListType_t
    {
        typedef List_t<EmptyTypeList> T;
    };

    // returns the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    typename TrailingListType_t<INDEX>::T const &trailing_list () const
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return ListHelper_t<TypeList,INDEX>::trailing_list(*this);
    };
    template <Uint32 INDEX>
    typename TrailingListType_t<INDEX>::T &trailing_list ()
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return ListHelper_t<TypeList,INDEX>::trailing_list(*this);
    };

    void print (std::ostream &out, bool print_parens = true) const
    {
        if (print_parens)
            out << "()";
    }

    static std::string type_as_string () { return "List_t<" + TypeStringOf_t<TypeList>::eval() + '>'; }
};

// TODO: make into extern variable
List_t<EmptyTypeList> const List_t<EmptyTypeList>::SINGLETON;

typedef List_t<EmptyTypeList> EmptyList;




template <typename HeadType_>
struct List_t<TypeList_t<HeadType_> >
{
    typedef HeadType_ HeadType;
    typedef TypeList_t<HeadType> TypeList;
    typedef EmptyTypeList BodyTypeList;
    typedef List_t<EmptyTypeList> BodyList;
    static Uint32 const LENGTH = 1;

    List_t () { }
    List_t (HeadType const &head) : m_head(head) { }

    // the following constructors initialize the first elements that are common to both
    // list types, doing default construction for all remaining elements, if any.
    List_t (List_t<EmptyTypeList> const &) { } // default construction
    List_t (List_t const &list) : m_head(list.m_head) { }
    template <typename BodyTypeList>
    List_t (List_t<TypeList_t<HeadType, BodyTypeList> > const &leading_list)
        :
        m_head(leading_list.head())
    { }

    HeadType const &head () const { return m_head; }
    HeadType &head () { return m_head; }
    List_t<EmptyTypeList> const &body () const { return List_t<EmptyTypeList>::SINGLETON; } // TEMP?

    // type cast operator for HeadType?

    template <Uint32 INDEX>
    struct Type_t
    {
        typedef typename TypeList::template El_t<INDEX>::T T;
    };

    template <Uint32 INDEX>
    HeadType const &el () const
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return m_head;
    }
    template <Uint32 INDEX>
    HeadType &el ()
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return m_head;
    }

    // returns the type of the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingListType_t
    {
        typedef List_t<typename TypeList::template TrailingTypeList_t<INDEX>::T> T;
    };

    // returns the trailing List_t starting at the INDEXth element
    template <Uint32 INDEX>
    typename TrailingListType_t<INDEX>::T const &trailing_list () const
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return ListHelper_t<TypeList,INDEX>::trailing_list(*this);
    };
    template <Uint32 INDEX>
    typename TrailingListType_t<INDEX>::T &trailing_list ()
    {
        Lvd::Meta::Assert<(INDEX < LENGTH)>();
        return ListHelper_t<TypeList,INDEX>::trailing_list(*this);
    };

    void print (std::ostream &out, bool print_parens = true) const
    {
        if (print_parens)
            out << '(';
        out << head();
        if (print_parens)
            out << ')';
    }

    static std::string type_as_string () { return "List_t<" + TypeStringOf_t<TypeList>::eval() + '>'; }

private:

    HeadType m_head;
};

template <typename TypeList>
std::ostream &operator << (std::ostream &out, List_t<TypeList> const &l)
{
    l.print(out);
    return out;
}







// for use in the el() and trailing_list() methods in List_t
template <typename TypeList, Uint32 INDEX>
struct ListHelper_t
{
    enum { _ = Lvd::Meta::Assert<(INDEX > 0)>::v };
    typedef List_t<TypeList> List;
    typedef typename TypeList::template El_t<INDEX>::T ValueType;
    typedef List_t<typename TypeList::template TrailingTypeList_t<INDEX>::T> TrailingListType;
    static ValueType const &el (List const &list) { return ListHelper_t<typename TypeList::BodyTypeList,INDEX-1>::el(list.body()); }
    static ValueType &el (List &list) { return ListHelper_t<typename TypeList::BodyTypeList,INDEX-1>::el(list.body()); }
    static TrailingListType const &trailing_list (List const &list) { return ListHelper_t<typename TypeList::BodyTypeList,INDEX-1>::trailing_list(list.body()); }
    static TrailingListType &trailing_list (List &list) { return ListHelper_t<typename TypeList::BodyTypeList,INDEX-1>::trailing_list(list.body()); }
};

template <typename TypeList>
struct ListHelper_t<TypeList,0>
{
    typedef List_t<TypeList> List;
    typedef typename List::HeadType ValueType;
    typedef List TrailingListType;
    static ValueType const &el (List const &list) { return list.head(); }
    static ValueType &el (List &list) { return list.head(); }
    static TrailingListType const &trailing_list (List const &list) { return list; }
    static TrailingListType &trailing_list (List &list) { return list; }
};








// tack an element onto the beginning of a list (where the list is empty)
template <typename HeadType>
List_t<TypeList_t<HeadType> > operator >>= (HeadType const &head, List_t<EmptyTypeList> const &) 
{ 
    return List_t<TypeList_t<HeadType> >(head); 
}

// tack an element onto the beginning of a list (catch-all case)
template <typename HeadType, typename BodyTypeList>
List_t<TypeList_t<HeadType,BodyTypeList> > operator >>= (HeadType const &head, List_t<BodyTypeList> const &body) 
{ 
    return List_t<TypeList_t<HeadType,BodyTypeList> >(head, body); 
}



// concatenate two lists (where both are empty)
List_t<EmptyTypeList> operator |= (List_t<EmptyTypeList> const &, List_t<EmptyTypeList> const &)
{
    return List_t<EmptyTypeList>();
}

// concatenate two lists (where the second is empty)
template <typename LeadingHeadType, typename LeadingBodyTypeList>
List_t<TypeList_t<LeadingHeadType,LeadingBodyTypeList> > operator |= (
    List_t<TypeList_t<LeadingHeadType,LeadingBodyTypeList> > const &leading_list, 
    List_t<EmptyTypeList> const &)
{
    return leading_list;
}

// concatenate two lists (where the first is empty)
template <typename TrailingHeadType, typename TrailingBodyTypeList>
List_t<TypeList_t<TrailingHeadType,TrailingBodyTypeList> > operator |= (
    List_t<EmptyTypeList> const &, 
    List_t<TypeList_t<TrailingHeadType,TrailingBodyTypeList> > const &trailing_type_list)
{
    return trailing_type_list;
}

// concatenate two lists (where the first has only one element)
template <typename LeadingHeadType, typename TrailingHeadType, typename TrailingBodyTypeList>
List_t<TypeList_t<LeadingHeadType,TypeList_t<TrailingHeadType,TrailingBodyTypeList> > > operator |= (
    List_t<TypeList_t<LeadingHeadType> > const &leading_list,
    List_t<TypeList_t<TrailingHeadType,TrailingBodyTypeList> > const &trailing_list)
{
    return List_t<TypeList_t<LeadingHeadType,TypeList_t<TrailingHeadType,TrailingBodyTypeList> > >(
        leading_list.head(), 
        trailing_list);
}

// concatenate two lists (catch-all case)
template <typename LeadingTypeList, typename TrailingTypeList>
List_t<typename ConcatenationOfTypeLists_t<LeadingTypeList,TrailingTypeList>::T> operator |= (
    List_t<LeadingTypeList> const &leading_list,
    List_t<TrailingTypeList> const &trailing_list)
{
    return List_t<typename ConcatenationOfTypeLists_t<LeadingTypeList,TrailingTypeList>::T>(
        leading_list.head(), 
        (leading_list.body() |= trailing_list));
}

/*
// the operator associativity for <<= is semantically wrong for this operation

// tack an element onto the end of a list
template <typename TailType>
List_t<TypeList_t<TailType> > operator <<= (List_t<EmptyTypeList> const &, TailType const &tail) 
{ 
    return List_t<TypeList_t<TailType> >(tail); 
}

// tack an element onto the end of a list
template <typename LeadingHeadType, typename TailType>
List_t<TypeList_t<LeadingHeadType,TypeList_t<TailType> > > operator <<= (List_t<LeadingHeadType> const &leading_list, TailType const &tail) 
{ 
    return List_t<TypeList_t<LeadingTypeList,TypeList_t<TailType> > >(leading_list.head(), (leading_list.body() <<= tail)); 
}

// tack an element onto the end of a list
template <typename LeadingTypeList, typename TailType>
List_t<typename ConcatenationOfTypeLists_t<LeadingTypeList,TypeList_t<TailType> >::T> operator <<= (
    List_t<LeadingTypeList> const &leading_list, 
    TailType const &tail) 
{ 
    return List_t<typename ConcatenationOfTypeLists_t<LeadingTypeList,TypeList_t<TailType> >::T>(
        leading_list.head(), 
        (leading_list.body() <<= tail)); 
}
*/



#endif // LIST_HPP_
