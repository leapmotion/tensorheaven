// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/basis.hpp by Victor Dods, created 2013/07/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_BASIS_HPP_
#define TENH_CONCEPTUAL_BASIS_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/meta/static_assert.hpp"

namespace Tenh {

template <typename Id_>
struct Basis_c
{
    typedef EmptyTypeList ParentTypeList;

    typedef Id_ Id;
    typedef typename DualOf_f<Basis_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "Basis_c<" + TypeStringOf_t<Id_>::eval() + '>';
    }
};

template <typename Id_>
struct IsConcept_f<Basis_c<Id_> >
{ static bool const V = true; };

template <typename T> struct IsBasis_f { static bool const V = false; };
template <typename Id> struct IsBasis_f<Basis_c<Id> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Basis);
// special convenience macros
#define IS_BASIS_UNIQUELY(Concept) HasUniqueBasisStructure_f<Concept>::V
#define AS_BASIS(Concept) UniqueBasisStructureOf_f<Concept>::T

template <typename Id>
struct DualOf_f<Basis_c<Id> >
{
    typedef Basis_c<typename DualOf_f<Id>::T> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_BASIS_HPP_
