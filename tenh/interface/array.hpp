// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/array.hpp by Victor Dods, created 2013/08/23
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_ARRAY_HPP_
#define TENH_INTERFACE_ARRAY_HPP_

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// compile-time interface for fixed-length array of a given component type,
// layed out contiguously in memory.
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_>
struct Array_i
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v), DERIVED_MUST_NOT_BE_NULL_TYPE)
    };

    typedef Derived_ Derived;
    typedef Component_ Component;
    static Uint32 const COMPONENT_COUNT = COMPONENT_COUNT_;
    typedef ComponentIndex_t<COMPONENT_COUNT> ComponentIndex;

// protected:
    
//     explicit Array_i (WithoutInitialization const &) { }
//     explicit Array_i () { }

// public:

    Component const &operator [] (ComponentIndex const &i) const { return as_derived().Derived::operator[](i); }
    Component &operator [] (ComponentIndex const &i) { return as_derived().Derived::operator[](i); }

    // access to the raw data (WARNING: if Component isn't a POD type, then this isn't "everything")
    Uint32 data_size_in_bytes () const { return COMPONENT_COUNT*sizeof(Component); }
    Component const *data_pointer () const { return as_derived().Derived::data_pointer(); }
    Component *data_pointer () { return as_derived().Derived::data_pointer(); }

    // accessor as Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }

    static std::string type_as_string ()
    {
        return "Array_i<" + TypeStringOf_t<Derived>::eval() + ','
                          + TypeStringOf_t<Component>::eval() + ',' 
                          + AS_STRING(COMPONENT_COUNT) + '>';
    }
};

template <typename T> struct IsArray_i { static bool const V = false; };
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_> struct IsArray_i<Array_i<Derived_,Component_,COMPONENT_COUNT_> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_INTERFACE_ARRAY_HPP_
