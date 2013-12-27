// ///////////////////////////////////////////////////////////////////////////
// tenh/componentqualifier.hpp by Victor Dods, created 2013/12/23
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_COMPONENTQUALIFIER_HPP_
#define TENH_COMPONENTQUALIFIER_HPP_

#include "tenh/core.hpp"

namespace Tenh {

enum ComponentQualifier
{
    COMPONENTS_ARE_PROCEDURAL = 0,
    COMPONENTS_ARE_CONST_MEMORY,
    COMPONENTS_ARE_NONCONST_MEMORY
};

inline std::string component_qualifier_as_string (ComponentQualifier q)
{
    static std::string const STRING_LOOKUP[3] = { "COMPONENTS_ARE_PROCEDURAL", "COMPONENTS_ARE_CONST_MEMORY", "COMPONENTS_ARE_NONCONST_MEMORY" };
    assert(q >= 0 && q < 3);
    return STRING_LOOKUP[q];
}

template <typename Component_, ComponentQualifier COMPONENT_QUALIFIER_>
struct ComponentQualifier_m
{
    // typedef for the return type of operator[] (...) const
    typedef typename If_f<COMPONENT_QUALIFIER_ == COMPONENTS_ARE_PROCEDURAL,
                          Component_,
                          Component_ const &>::T ComponentAccessConstReturnType;
    // typedef for the return type of operator[] (...)
    typedef typename If_f<COMPONENT_QUALIFIER_ == COMPONENTS_ARE_PROCEDURAL,
                          Component_,
            typename If_f<COMPONENT_QUALIFIER_ == COMPONENTS_ARE_CONST_MEMORY,
                          Component_ const &,
                          Component_ &>::T>::T ComponentAccessNonConstReturnType;
//     // this will always be const
//     typedef typename If_f<COMPONENTS_ARE_CONST_,
//                           Component_ const,
//                           Component_ const>::T QualifiedComponentForcedConst;
    // type of the array elements' internal storage.  this will be const or not
    // depending on the value of COMPONENT_QUALIFIER_
    typedef typename If_f<COMPONENT_QUALIFIER_ == COMPONENTS_ARE_NONCONST_MEMORY,
                          Component_,
                          Component_ const>::T QualifiedComponent;
};

} // end of namespace Tenh

#endif // TENH_COMPONENTQUALIFIER_HPP_
