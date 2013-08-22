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

// TODO: think about if Basis_c needs to exist, or if it could just be
// a generic identifier.  if it were a generic identifier, this would
// obviate the need for TensorProductOfBases_c.  if this is done, then
// IsABasis_c should still exist, and the user would need to specialize
// IsABasis_c<UserBasisType>, providing "static bool const V = true".

template <typename Id_>
struct Basis_c
{
    typedef EmptyTypeList ParentTypeList;

    typedef Id_ Id;
    typedef typename DualOf_c<Basis_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "Basis_c<" + TypeStringOf_t<Id>::eval() + '>';
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
struct DualOf_c<Basis_c<Id> >
{
    typedef Basis_c<typename DualOf_c<Id>::T> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_BASIS_HPP_
