// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/array.hpp by Victor Dods, created 2013/08/23
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_ARRAY_HPP_
#define TENH_INTERFACE_ARRAY_HPP_

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp"
#include "tenh/componentqualifier.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// compile-time interface for fixed-length array of a given component type,
// with no presupposition about how the components are provided (see
// MemoryArray_i and ProceduralArray_t).
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_, ComponentQualifier COMPONENT_QUALIFIER_>
struct Array_i
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<Derived_,NullType>::V), DERIVED_MUST_NOT_BE_NULL_TYPE)
    };

    typedef Derived_ Derived;
    typedef Component_ Component;
    static Uint32 const COMPONENT_COUNT = COMPONENT_COUNT_;
    static ComponentQualifier const COMPONENT_QUALIFIER = COMPONENT_QUALIFIER_;
    typedef ComponentIndex_t<COMPONENT_COUNT> ComponentIndex;
    typedef typename ComponentQualifier_m<Component_,COMPONENT_QUALIFIER_>::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename ComponentQualifier_m<Component_,COMPONENT_QUALIFIER_>::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;
    typedef typename ComponentQualifier_m<Component_,COMPONENT_QUALIFIER_>::QualifiedComponent QualifiedComponent;

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

    static std::string type_as_string (bool verbose)
    {
        return "Array_i<" + type_string_of<Derived_>() + ','
                          + type_string_of<Component_>() + ','
                          + FORMAT(COMPONENT_COUNT_) + ','
                          + component_qualifier_as_string(COMPONENT_QUALIFIER_) + '>';
    }
};

template <typename T> struct IsArray_i { static bool const V = false; };
template <typename Derived_, typename Component_, Uint32 COMPONENT_COUNT_, ComponentQualifier COMPONENT_QUALIFIER_> struct IsArray_i<Array_i<Derived_,Component_,COMPONENT_COUNT_,COMPONENT_QUALIFIER_> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_INTERFACE_ARRAY_HPP_
