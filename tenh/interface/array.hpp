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

static bool const IMMUTABLE_COMPONENTS = true;
static bool const MUTABLE_COMPONENTS = false;

// compile-time interface for fixed-length array of a given component type,
// with no presupposition about how the components are provided (see
// MemoryArray_i and ImmutableArray_t).
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_, bool COMPONENTS_ARE_IMMUTABLE_>
struct Array_i
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual<Derived_,NullType>::V), DERIVED_MUST_NOT_BE_NULL_TYPE)
    };

    typedef Derived_ Derived;
    typedef Component_ Component;
    static Uint32 const COMPONENT_COUNT = COMPONENT_COUNT_;
    static bool const COMPONENTS_ARE_IMMUTABLE = COMPONENTS_ARE_IMMUTABLE_;
    typedef ComponentIndex_t<COMPONENT_COUNT> ComponentIndex;
    typedef typename If<COMPONENTS_ARE_IMMUTABLE_,Component_,Component_ const &>::T ComponentAccessConstReturnType;
    typedef typename If<COMPONENTS_ARE_IMMUTABLE_,Component_,Component_ &>::T ComponentAccessNonConstReturnType;

    ComponentAccessConstReturnType operator [] (ComponentIndex const &i) const { return as_derived().Derived::operator[](i); }
    ComponentAccessNonConstReturnType operator [] (ComponentIndex const &i) { return as_derived().Derived::operator[](i); }

    // accessor as Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }

    void print (std::ostream &out) const
    {
        if (COMPONENT_COUNT_ == 0)
        {
            out << "{ }";
            return;
        }

        ComponentIndex i; // initialized to the beginning automatically
        out << '{' << operator[](i);
        ++i;
        for ( ; i.is_not_at_end(); ++i)
            out << ", " << operator[](i);
        out << '}';
    }

    static std::string type_as_string ()
    {
        return "Array_i<" + type_string_of<Derived>() + ','
                          + type_string_of<Component>() + ','
                          + AS_STRING(COMPONENT_COUNT) + ','
                          + AS_STRING((COMPONENTS_ARE_IMMUTABLE_ ? "IMMUTABLE_COMPONENTS" : "MUTABLE_COMPONENTS")) + '>';
    }
};

template <typename T> struct IsArray_i { static bool const V = false; };
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_, bool COMPONENTS_ARE_IMMUTABLE_> struct IsArray_i<Array_i<Derived_,Component_,COMPONENT_COUNT_,COMPONENTS_ARE_IMMUTABLE_> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_INTERFACE_ARRAY_HPP_
