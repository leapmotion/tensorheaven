// ///////////////////////////////////////////////////////////////////////////
// tenh/index.hpp by Victor Dods, created 2013/03/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INDEX_HPP_
#define TENH_INDEX_HPP_

#include <ostream>
#include <string>

#include "tenh/core.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// for use in operator [] for actual evaluation of tensor components
template <typename OwnerVector_>
struct Index_t
{
    typedef OwnerVector_ OwnerVector;
    static Uint32 const COMPONENT_COUNT = OwnerVector::DIM;

    // default is to initialize to start of iteration
    Index_t () : m(0) { }
    // this is explicit because it's expensive -- the index value has to pass the range-checking
    // gatekeeper, but once it's inside, it gets to run around freely and safely.
    explicit Index_t (Uint32 i)
        :
        m(i)
    {
        if (m >= COMPONENT_COUNT)
            throw std::out_of_range("raw-integer argument to Vector_t<>::Index constructor was out of range");
    }
    // no range-checking necessary
    Index_t (Index_t const &i) : m(i.m) { }

    // for when you know that 0 <= i < COMPONENT_COUNT -- bypass the range check
    static Index_t range_unchecked (Uint32 i)
    {
        Index_t retval(Static_t<WithoutInitialization>::SINGLETON);
        retval.m = i;
        return retval;
    }

    bool operator == (Index_t const &i) const { return m == i.m; }
    bool operator != (Index_t const &i) const { return m != i.m; }
    bool operator < (Index_t const &i) const { return m < i.m; }

    bool is_at_end () const { return m >= COMPONENT_COUNT; }
    bool is_not_at_end () const { return m < COMPONENT_COUNT; }
    Uint32 value () const { return m; } // for the specific memory addressing scheme that Vector_t uses
    void operator ++ () { ++m; }
    void reset () { m = 0; }

    static std::string type_as_string () { return "Index_t<" + TypeStringOf_t<OwnerVector>::eval() + '>'; }

private:

    Index_t (WithoutInitialization const &) { }

    // TODO: store m as Uint16, and keep "is at end" flag as some separate bool or Uint16.
    // this will allow value() to always return valid index integers, and the system can be safer
    Uint32 m;
};

template <typename OwnerVector>
std::ostream &operator << (std::ostream &out, Index_t<OwnerVector> const &i)
{
    return out << i.value();
}

// for use in operator () for creation of expression templates (indexed tensor expressions)
template <typename OwnerVector_, char SYMBOL_>
struct NamedIndex_t : public Index_t<OwnerVector_>
{
    typedef Index_t<OwnerVector_> Parent;
    typedef OwnerVector_ OwnerVector;
    static char const SYMBOL = SYMBOL_;
    using Parent::COMPONENT_COUNT;

    NamedIndex_t () { }
    explicit NamedIndex_t (Uint32 i) : Parent(i) { }
    NamedIndex_t (Parent const &i) : Parent(i) { }
    NamedIndex_t (NamedIndex_t const &i) : Parent(i) { }

    static std::string type_as_string () { return "NamedIndex_t<" + TypeStringOf_t<OwnerVector>::eval() + ",'" + SYMBOL + "'>"; }
};

} // end of namespace Tenh

#endif // TENH_INDEX_HPP_
