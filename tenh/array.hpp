// ///////////////////////////////////////////////////////////////////////////
// tenh/array.hpp by Victor Dods, created 2013/04/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_ARRAY_HPP_
#define TENH_ARRAY_HPP_

#include <stdexcept>

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// fixed-length array of a given component type, which must be a POD type
// (the data_size_in_bytes and data_pointer methods require this).
template <typename Component_, Uint32 COMPONENT_COUNT_>
struct Array_t
{
    typedef Component_ Component;
    static Uint32 const COMPONENT_COUNT = COMPONENT_COUNT_;
    typedef ComponentIndex_t<COMPONENT_COUNT> ComponentIndex;

    explicit Array_t (WithoutInitialization const &) { }
    explicit Array_t (Component const &fill_with) { for (Uint32 i = 0; i < COMPONENT_COUNT; ++i) m_component[i] = fill_with; }
    Array_t (Component const &x0, Component const &x1) { STATIC_ASSERT((COMPONENT_COUNT == 2),COMPONENT_COUNT_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; }
    Array_t (Component const &x0, Component const &x1, Component const &x2) { STATIC_ASSERT((COMPONENT_COUNT == 3),COMPONENT_COUNT_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; m_component[2] = x2; }
    Array_t (Component const &x0, Component const &x1, Component const &x2, Component const &x3) { STATIC_ASSERT((COMPONENT_COUNT == 4),COMPONENT_COUNT_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; m_component[2] = x2; m_component[3] = x3; }

    Component const &operator [] (ComponentIndex const &i) const
    {
        assert(i.is_not_at_end() && "you used Index_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_component[i.value()];
    }
    Component &operator [] (ComponentIndex const &i)
    {
        assert(i.is_not_at_end() && "you used Index_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_component[i.value()];
    }

    // access to the raw data
    Uint32 data_size_in_bytes () const { return COMPONENT_COUNT*sizeof(Component); }
    Component const *data_pointer () const { return &m_component[0]; }
    Component *data_pointer () { return &m_component[0]; }

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

template <typename T> struct IsAnArray_t { static bool const V = false; };
template <typename Component, Uint32 COMPONENT_COUNT> struct IsAnArray_t<Array_t<Component,COMPONENT_COUNT> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_ARRAY_HPP_
