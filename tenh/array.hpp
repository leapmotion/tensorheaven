// ///////////////////////////////////////////////////////////////////////////
// tenh/array.hpp by Victor Dods, created 2013/04/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_ARRAY_HPP_
#define TENH_ARRAY_HPP_

#include <stdexcept>

#include "tenh/core.hpp"
#include "tenh/index.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// fixed-length array of a given component type, which must be a POD type
// (the data_size_in_bytes and data_pointer methods require this).
// Derived is only used to define the Index type.
template <typename Component_, Uint32 DIM_, typename Derived_ = NullType>
struct Array_t
{
    typedef typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                   Array_t<Component_,DIM_,Derived_>,
                                   Derived_>::T Derived;
    typedef Component_ Component;
    static Uint32 const DIM = DIM_;
    typedef Index_t<Derived> Index;

    explicit Array_t (WithoutInitialization const &) { }
    explicit Array_t (Component const &fill_with) { for (Uint32 i = 0; i < DIM; ++i) m_component[i] = fill_with; }
    Array_t (Component const &x0, Component const &x1) { STATIC_ASSERT((DIM == 2),DIMENSION_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; }
    Array_t (Component const &x0, Component const &x1, Component const &x2) { STATIC_ASSERT((DIM == 3),DIMENSION_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; m_component[2] = x2; }
    Array_t (Component const &x0, Component const &x1, Component const &x2, Component const &x3) { STATIC_ASSERT((DIM == 4),DIMENSION_DOES_NOT_MATCH_ARGUMENT_COUNT); m_component[0] = x0; m_component[1] = x1; m_component[2] = x2; m_component[3] = x3; }

    Component const &operator [] (Index const &i) const
    {
        assert(i.is_not_at_end() && "you used Index_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_component[i.value()];
    }
    Component &operator [] (Index const &i)
    {
        assert(i.is_not_at_end() && "you used Index_t(x, DONT_RANGE_CHECK) inappropriately");
        return m_component[i.value()];
    }

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
