// ///////////////////////////////////////////////////////////////////////////
// tenh/index.hpp by Victor Dods, created 2013/03/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INDEX_HPP_
#define TENH_INDEX_HPP_

#include <ostream>
#include <stdexcept>
#include <string>

#include "tenh/core.hpp"
#include "tenh/meta/typelist.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// Index_t (for accessing particular components of vectors/tensors)
// ///////////////////////////////////////////////////////////////////////////

// for use in operator [] for actual evaluation of tensor components
template <typename Owner_>
struct Index_t
{
    typedef Owner_ Owner;
    static Uint32 const COMPONENT_COUNT = Owner::DIM;

    // default is to initialize to start of iteration
    Index_t () : m(0) { }
    // the default parameter for check_range will cause a range check (and possible
    // exception throw) for safety.  if DONT_RANGE_CHECK is explicitly provided (an
    // intentional and ugly bit of code), the range check can be avoided for efficiency
    // when you know for a fact that your value is within the correct range.
    explicit Index_t (Uint32 i, bool check_range = CHECK_RANGE)
        :
        m(i)
    {
        if (check_range && m >= COMPONENT_COUNT)
            throw std::out_of_range("raw-integer argument to Vector_t<>::Index constructor was out of range");
    }
    // no range-checking necessary
    Index_t (Index_t const &i) : m(i.m) { }

    bool operator == (Index_t const &i) const { return m == i.m; }
    bool operator != (Index_t const &i) const { return m != i.m; }
    bool operator < (Index_t const &i) const { return m < i.m; }

    bool is_at_end () const { return m >= COMPONENT_COUNT; }
    bool is_not_at_end () const { return m < COMPONENT_COUNT; }
    Uint32 value () const { return m; } // for the specific memory addressing scheme that Vector_t uses
    void operator ++ () { ++m; }
    void reset () { m = 0; }

    static std::string type_as_string () { return "Index_t<" + TypeStringOf_t<Owner>::eval() + '>'; }

private:

    Index_t (WithoutInitialization const &) { }

    // TODO: store m as Uint16, and keep "is at end" flag as some separate bool or Uint16.
    // this will allow value() to always return valid index integers, and the system can be safer
    Uint32 m;
};

template <typename Owner>
std::ostream &operator << (std::ostream &out, Index_t<Owner> const &i)
{
    return out << i.value();
}

// ///////////////////////////////////////////////////////////////////////////
// TypedIndex_t (for creating indexed expressions)
// ///////////////////////////////////////////////////////////////////////////

// for use in operator () for creation of expression templates (indexed tensor expressions)
template <typename Owner_, char SYMBOL_>
struct TypedIndex_t : public Index_t<Owner_>
{
    typedef Index_t<Owner_> Parent;
    typedef Owner_ Owner;
    static char const SYMBOL = SYMBOL_;
    using Parent::COMPONENT_COUNT;

    TypedIndex_t () : Parent() { }
    explicit TypedIndex_t (Uint32 i, bool check_range = CHECK_RANGE) : Parent(i, check_range) { }
    TypedIndex_t (Parent const &i) : Parent(i) { }
    TypedIndex_t (TypedIndex_t const &i) : Parent(i) { }

    static std::string type_as_string () { return "TypedIndex_t<" + TypeStringOf_t<Owner>::eval() + ",'" + SYMBOL + "'>"; }
};

template <typename TypeList>
struct AssertThatEachTypeIsATypedIndex_t
{
private:
    typedef typename TypeList::HeadType HeadType;
    typedef typename HeadType::Owner Owner;
    static char const SYMBOL = HeadType::SYMBOL;
public:
    static bool const V = Lvd::Meta::TypesAreEqual<HeadType,TypedIndex_t<Owner,SYMBOL> >::v &&
                          AssertThatEachTypeIsATypedIndex_t<typename TypeList::BodyTypeList>::V;
    operator bool () const { return V; }
};

template <typename HeadType>
struct AssertThatEachTypeIsATypedIndex_t<TypeList_t<HeadType> >
{
private:
    typedef typename HeadType::Owner Owner;
    static char const SYMBOL = HeadType::SYMBOL;
public:
    static bool const V = Lvd::Meta::TypesAreEqual<HeadType,TypedIndex_t<Owner,SYMBOL> >::v;
    operator bool () const { return V; }
};

// vacuously true
template <>
struct AssertThatEachTypeIsATypedIndex_t<EmptyTypeList>
{
    static bool const V = true;
    operator bool () const { return V; }
};

// operator overloads for stringing together type lists of symbolic indices.
template <typename Owner1, char SYMBOL1, typename Owner2, char SYMBOL2>
TypeList_t<TypedIndex_t<Owner1,SYMBOL1>,TypeList_t<TypedIndex_t<Owner2,SYMBOL2> > > operator | (
    TypedIndex_t<Owner1,SYMBOL1> const &,
    TypedIndex_t<Owner2,SYMBOL2> const &)
{
    return TypeList_t<TypedIndex_t<Owner1,SYMBOL1>,TypeList_t<TypedIndex_t<Owner2,SYMBOL2> > >();
}

template <typename HeadType, typename BodyTypeList, typename Owner, char SYMBOL>
typename ConcatenationOfTypeLists_t<TypeList_t<HeadType,BodyTypeList>,TypeList_t<TypedIndex_t<Owner,SYMBOL> > >::T operator | (
    TypeList_t<HeadType,BodyTypeList> const &,
    TypedIndex_t<Owner,SYMBOL> const &)
{
    // make sure the first argument resulted from other operator| calls
    AssertThatEachTypeIsATypedIndex_t<TypeList_t<HeadType,BodyTypeList> >();
    return typename ConcatenationOfTypeLists_t<TypeList_t<HeadType,BodyTypeList>,TypeList_t<TypedIndex_t<Owner,SYMBOL> > >::T();
}

} // end of namespace Tenh

#endif // TENH_INDEX_HPP_
