// ///////////////////////////////////////////////////////////////////////////
// tenh/array.hpp by Victor Dods, created 2013/04/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_ARRAY_HPP_
#define TENH_ARRAY_HPP_

#include <stdexcept>

#include "tenh/core.hpp"

#include "tenh/interface/array.hpp"

namespace Tenh {

// fixed-length array of a given component type, which must be a POD type
// (the allocation_size_in_bytes and pointer_to_allocation methods require this).
template <typename Component_, Uint32 COMPONENT_COUNT_>
struct Array_t : public Array_i<Array_t<Component_,COMPONENT_COUNT_>,Component_,COMPONENT_COUNT_>
{
    typedef Array_i<Array_t<Component_,COMPONENT_COUNT_>,Component_,COMPONENT_COUNT_> Parent_Array_i;

    typedef typename Parent_Array_i::Component Component;
    using Parent_Array_i::COMPONENT_COUNT;
    typedef typename Parent_Array_i::ComponentIndex ComponentIndex;

// this is to allow 0-component arrays to work (necessary for 0-dimensional vectors)
#ifdef __clang_version__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtautological-compare"
#endif // __clang_version__

    explicit Array_t (WithoutInitialization const &) { }
    explicit Array_t (Component const &fill_with) { for (Uint32 i = 0; i < COMPONENT_COUNT; ++i) m_component[i] = fill_with; }

#ifdef __clang_version__
#pragma GCC diagnostic pop
#endif // __clang_version__

    Array_t (Component const &x0, Component const &x1) { STATIC_ASSERT((COMPONENT_COUNT == 2),COMPONENT_COUNT_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; }
    Array_t (Component const &x0, Component const &x1, Component const &x2) { STATIC_ASSERT((COMPONENT_COUNT == 3),COMPONENT_COUNT_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; m_component[2] = x2; }
    Array_t (Component const &x0, Component const &x1, Component const &x2, Component const &x3) { STATIC_ASSERT((COMPONENT_COUNT == 4),COMPONENT_COUNT_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; m_component[2] = x2; m_component[3] = x3; }

    Component const &operator [] (ComponentIndex const &i) const
    {
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_component[i.value()];
    }
    Component &operator [] (ComponentIndex const &i)
    {
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_component[i.value()];
    }

    // access to the raw data
    Component const *pointer_to_allocation () const { return &m_component[0]; }
    Component *pointer_to_allocation () { return &m_component[0]; }

    static std::string type_as_string ()
    {
        return "Array_t<" + TypeStringOf_t<Component>::eval() + ',' + AS_STRING(COMPONENT_COUNT) + '>';
    }

protected:

    Component m_component[COMPONENT_COUNT];

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    Array_t ();
};

template <typename T> struct IsArray_t { static bool const V = false; };
template <typename Component, Uint32 COMPONENT_COUNT> struct IsArray_t<Array_t<Component,COMPONENT_COUNT> > { static bool const V = true; };

template <typename Component, Uint32 COMPONENT_COUNT> struct IsArray_i<Array_t<Component,COMPONENT_COUNT> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_ARRAY_HPP_
