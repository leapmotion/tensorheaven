// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/index.hpp by Victor Dods, created 2013/03/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_INDEX_HPP_
#define TENH_CONCEPTUAL_INDEX_HPP_

#include <ostream>
#include <stdexcept>
#include <string>

#include "tenh/core.hpp"

#include "tenh/meta/typelist.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

/*
possible ideas for redesign of indexing
---------------------------------------
Because abstract vector spaces and tensor products have indices and abstract
indexed expressions can be written down and checked with no reference to any
bases, perhaps it's a good idea to have two levels of indices:
- AbstractIndex_c<VectorSpace>
- BasedIndex_c<BasedVectorSpace>

And similarly for tensors/multi-indices:
- AbstractMultiIndex_c<TensorProductOfVectorSpaces>   : is a list of AbstractIndex_c types
- BasedMultiIndex_c<TensorProductOfBasedVectorSpaces> : is a list of BasedIndex_c types

HOWEVER, ideally there would only be abstract indices, and the basis checking
in natural pairings/additions/etc would be done via explicit static asserts
*/

template <typename T> struct IsABasedVectorSpace_c;

// ///////////////////////////////////////////////////////////////////////////
// Index_c (for accessing particular components of vectors/tensors)
// ///////////////////////////////////////////////////////////////////////////

// for use in operator [] for actual evaluation of tensor components.
// Owner_ should be a BasedVectorSpace_c type.  Technically Owner_ should
// be a VectorSpace_c type (since the index is an abstract object), but
// because Tensor Heaven does natural pairings of based vector spaces, it is
// necessary to distinguish vector spaces and bases.  Perhaps this could be
// changed to use VectorSpace_c abstractly, but for now this is the choice.
// TODO: rename Index_c to BasedIndex_c
template <typename Owner_>
struct Index_c
{
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsABasedVectorSpace_c<Owner_>::V, MUST_BE_BASED_VECTOR_SPACE)
    };

    typedef Owner_ Owner; // TODO: rename to OfBasedVectorSpace (?)
    static Uint32 const COMPONENT_COUNT = Owner::DIM;
    typedef typename DualOf_c<Index_c>::T Dual; // relies on the template specialization below

    // default is to initialize to start of iteration
    Index_c () : m(0) { }
    // the default parameter for check_range will cause a range check (and possible
    // exception throw) for safety.  if DONT_RANGE_CHECK is explicitly provided (an
    // intentional and ugly bit of code), the range check can be avoided for efficiency
    // when you know for a fact that your value is within the correct range.
    explicit Index_c (Uint32 i, bool check_range = CHECK_RANGE)
        :
        m(i)
    {
        if (check_range && m >= COMPONENT_COUNT)
            throw std::out_of_range("raw-integer argument to Vector_t<>::Index constructor was out of range");
    }
    // no range-checking necessary
    Index_c (Index_c const &i) : m(i.m) { }

    bool operator == (Index_c const &i) const { return m == i.m; }
    bool operator != (Index_c const &i) const { return m != i.m; }
    bool operator < (Index_c const &i) const { return m < i.m; }

    bool is_at_end () const { return m >= COMPONENT_COUNT; }
    bool is_not_at_end () const { return m < COMPONENT_COUNT; }
    Uint32 value () const { return m; } // for the specific memory addressing scheme that Vector_t uses
    void operator ++ () { ++m; }
    void reset () { m = 0; }

    static std::string type_as_string () { return "Index_c<" + TypeStringOf_t<Owner>::eval() + '>'; }

private:

    Index_c (WithoutInitialization const &) { }

    // TODO: store m as Uint16, and keep "is at end" flag as some separate bool or Uint16.
    // this will allow value() to always return valid index integers, and the system can be safer
    // NOTE: 0-dimensional vector spaces will prevent this from working.
    Uint32 m;
};

template <typename T> struct IsAnIndex_c { static bool const V = false; };
template <typename Owner> struct IsAnIndex_c<Index_c<Owner> > { static bool const V = true; };

template <typename Owner>
struct DualOf_c<Index_c<Owner> >
{
    typedef Index_c<typename Owner::Dual> T;
};

template <typename Owner>
std::ostream &operator << (std::ostream &out, Index_c<Owner> const &i)
{
    return out << i.value();
}

// ///////////////////////////////////////////////////////////////////////////
// TypedIndex_c (for creating indexed expressions)
// ///////////////////////////////////////////////////////////////////////////

// for use in operator () for creation of expression templates (indexed tensor expressions)
// Owner_ should be a BasedVectorSpace_c type.
template <typename Owner_, char SYMBOL_>
struct TypedIndex_c : public Index_c<Owner_>
{
    typedef Index_c<Owner_> Parent;
    typedef Owner_ Owner;
    static char const SYMBOL = SYMBOL_;
    using Parent::COMPONENT_COUNT;

    TypedIndex_c () : Parent() { }
    explicit TypedIndex_c (Uint32 i, bool check_range = CHECK_RANGE) : Parent(i, check_range) { }
    TypedIndex_c (Parent const &i) : Parent(i) { }
    TypedIndex_c (TypedIndex_c const &i) : Parent(i) { }

    static std::string type_as_string () { return "TypedIndex_c<" + TypeStringOf_t<Owner>::eval() + ",'" + SYMBOL + "'>"; }
};

template <typename T> struct IsATypedIndex_c { static bool const V = false; };
template <typename Owner, char SYMBOL> struct IsATypedIndex_c<TypedIndex_c<Owner,SYMBOL> > { static bool const V = true; };

template <typename TypeList>
struct AssertThatEachTypeIsATypedIndex_c
{
    static bool const V = IsATypedIndex_c<typename TypeList::HeadType>::V &&
                          AssertThatEachTypeIsATypedIndex_c<typename TypeList::BodyTypeList>::V;
    operator bool () const { return V; }
};

template <typename HeadType>
struct AssertThatEachTypeIsATypedIndex_c<TypeList_t<HeadType> >
{
    static bool const V = IsATypedIndex_c<HeadType>::V;
    operator bool () const { return V; }
};

// vacuously true
template <>
struct AssertThatEachTypeIsATypedIndex_c<EmptyTypeList>
{
    static bool const V = true;
    operator bool () const { return V; }
};

// operator overloads for stringing together type lists of symbolic indices.
template <typename Owner1, char SYMBOL1, typename Owner2, char SYMBOL2>
TypeList_t<TypedIndex_c<Owner1,SYMBOL1>,TypeList_t<TypedIndex_c<Owner2,SYMBOL2> > > operator | (
    TypedIndex_c<Owner1,SYMBOL1> const &,
    TypedIndex_c<Owner2,SYMBOL2> const &)
{
    return TypeList_t<TypedIndex_c<Owner1,SYMBOL1>,TypeList_t<TypedIndex_c<Owner2,SYMBOL2> > >();
}

template <typename HeadType, typename BodyTypeList, typename Owner, char SYMBOL>
typename ConcatenationOfTypeLists_t<TypeList_t<HeadType,BodyTypeList>,TypeList_t<TypedIndex_c<Owner,SYMBOL> > >::T operator | (
    TypeList_t<HeadType,BodyTypeList> const &,
    TypedIndex_c<Owner,SYMBOL> const &)
{
    // make sure the first argument resulted from other operator| calls
    AssertThatEachTypeIsATypedIndex_c<TypeList_t<HeadType,BodyTypeList> >();
    return typename ConcatenationOfTypeLists_t<TypeList_t<HeadType,BodyTypeList>,TypeList_t<TypedIndex_c<Owner,SYMBOL> > >::T();
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_INDEX_HPP_
