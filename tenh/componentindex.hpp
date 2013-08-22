// ///////////////////////////////////////////////////////////////////////////
// tenh/componentindex.hpp by Victor Dods, created 2013/08/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_COMPONENTINDEX_HPP_
#define TENH_COMPONENTINDEX_HPP_

#include <stdexcept>

#include "tenh/core.hpp"

#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// for use in operator [] for evaluation of array components.
template <Uint32 COMPONENT_COUNT_>
struct ComponentIndex_t
{
    static Uint32 const COMPONENT_COUNT = COMPONENT_COUNT_;

// this to allow 0-component component-indexing to work (necessary for 0-dimensional vectors)
#ifdef __clang_version__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtautological-compare"
#endif // __clang_version__

    // default is to initialize to start of iteration
    ComponentIndex_t () : m(0) { }
    // the default parameter for check_range will cause a range check (and possible
    // exception throw) for safety.  if DONT_RANGE_CHECK is explicitly provided (an
    // intentional and ugly bit of code), the range check can be avoided for efficiency
    // when you know for a fact that your value is within the correct range.
    explicit ComponentIndex_t (Uint32 i, bool check_range = CHECK_RANGE)
        :
        m(i)
    {
        if (check_range && m >= COMPONENT_COUNT)
            throw std::out_of_range("raw-integer argument to ComponentIndex_t<...> constructor was out of range");
    }
    // no range-checking necessary
    ComponentIndex_t (ComponentIndex_t const &i) : m(i.m) { }

    bool operator == (ComponentIndex_t const &i) const { return m == i.m; }
    bool operator != (ComponentIndex_t const &i) const { return m != i.m; }
    bool operator < (ComponentIndex_t const &i) const { return m < i.m; }

    bool is_at_end () const { return m >= COMPONENT_COUNT; }
    bool is_not_at_end () const { return m < COMPONENT_COUNT; }
    Uint32 value () const { return m; } // for the specific memory addressing scheme that Vector_t uses
    void operator ++ () { ++m; }
    void reset () { m = 0; }

#ifdef __clang_version__
#pragma GCC diagnostic pop
#endif // __clang_version__

    static std::string type_as_string () { return "ComponentIndex_t<" + AS_STRING(COMPONENT_COUNT) + '>'; }

private:

    ComponentIndex_t (WithoutInitialization const &) { }

    // TODO: store m as Uint16, and keep "is at end" flag as some separate bool or Uint16 (?)
    // this will allow value() to always return valid index integers, and the system can be safer
    // NOTE: 0-dimensional arrays/vector spaces will prevent this from working.
    Uint32 m;
};

template <typename T> struct IsAComponentIndex_t { static bool const V = false; };
template <Uint32 COMPONENT_COUNT> struct IsAComponentIndex_t<ComponentIndex_t<COMPONENT_COUNT> > { static bool const V = true; };

template <typename TypeList>
struct EachTypeIsAComponentIndex_t
{
    static bool const V = IsAComponentIndex_t<typename TypeList::HeadType>::V &&
                          EachTypeIsAComponentIndex_t<typename TypeList::BodyTypeList>::V;
    operator bool () const { return V; }
};

template <>
struct EachTypeIsAComponentIndex_t<EmptyTypeList>
{
    static bool const V = true;
    operator bool () const { return V; }
};

template <Uint32 COMPONENT_COUNT>
std::ostream &operator << (std::ostream &out, ComponentIndex_t<COMPONENT_COUNT> const &c)
{
    return out << c.value();
}

} // end of namespace Tenh

#endif // TENH_COMPONENTINDEX_HPP_
