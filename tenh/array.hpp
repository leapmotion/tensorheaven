// ///////////////////////////////////////////////////////////////////////////
// tenh/array.hpp by Victor Dods, created 2013/04/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_ARRAY_HPP_
#define TENH_ARRAY_HPP_

#include <stdexcept>

#include "tenh/core.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// fixed-length array of a given component type, which must be a POD type
// (the data_size_in_bytes and data_pointer methods require this)
template <typename Component_, Uint32 DIM_>
struct Array_t
{
    typedef Component_ Component;
    static Uint32 const DIM = DIM_;

    explicit Array_t (WithoutInitialization const &) { }
    explicit Array_t (Component const &fill_with) { for (Uint32 i = 0; i < DIM; ++i) m_component[i] = fill_with; }
    Array_t (Component const &x0, Component const &x1) { Lvd::Meta::Assert<(DIM == 2)>(); m_component[0] = x0; m_component[1] = x1; }
    Array_t (Component const &x0, Component const &x1, Component const &x2) { Lvd::Meta::Assert<(DIM == 3)>(); m_component[0] = x0; m_component[1] = x1; m_component[2] = x2; }
    Array_t (Component const &x0, Component const &x1, Component const &x2, Component const &x3) { Lvd::Meta::Assert<(DIM == 4)>(); m_component[0] = x0; m_component[1] = x1; m_component[2] = x2; m_component[3] = x3; }

    // this has a range check built in -- it will throw if out of range
    Component const &operator [] (Uint32 i) const
    {
        if (i >= DIM)
            throw std::invalid_argument("index out of range");
        else
            return m_component[i];
    }
    Component &operator [] (Uint32 i)
    {
        if (i >= DIM)
            throw std::invalid_argument("index out of range");
        else
            return m_component[i];
    }

    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Component const &component_access_without_range_check (Uint32 i) const { return m_component[i]; }
    // this SHOULD be inconvenient and ugly to call.  it should be used ONLY when you know for certain that 0 <= i < DIM
    Component &component_access_without_range_check (Uint32 i) { return m_component[i]; }

    // access to the raw data
    Uint32 data_size_in_bytes () const { return DIM*sizeof(Component); }
    Component const *data_pointer () const { return &m_component[0]; }
    Component *data_pointer () { return &m_component[0]; }

    static std::string type_as_string ()
    {
        return "Array_t<" + TypeStringOf_t<Component>::eval() + ',' + AS_STRING(DIM) + '>';
    }

protected:

    Component m_component[DIM];
};

} // end of namespace Tenh

#endif // TENH_ARRAY_HPP_
