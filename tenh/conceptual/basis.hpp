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

// ////////////////////////////////////////////////////////////////////////////
// Basis_c
// ////////////////////////////////////////////////////////////////////////////

template <typename Id_>
struct Basis_c
{
    typedef EmptyTypeList ParentTypeList;

    typedef Id_ Id;

    static std::string type_as_string ()
    {
        return "Basis_c<" + type_string_of<Id_>() + '>';
    }
};

template <typename Id_>
struct IsConcept_f<Basis_c<Id_> >
{ static bool const V = true; };

template <typename T_> struct IsBasis_f { static bool const V = false; };
template <typename Id_> struct IsBasis_f<Basis_c<Id_> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Basis);
// special convenience macros
#define IS_BASIS_UNIQUELY(Concept) HasUniqueBasisStructure_f<Concept>::V
#define AS_BASIS(Concept) UniqueBasisStructureOf_f<Concept>::T

template <typename Id_>
struct DualOf_f<Basis_c<Id_> >
{
    typedef Basis_c<typename DualOf_f<Id_>::T> T;
};

// ////////////////////////////////////////////////////////////////////////////
// OrthonormalBasis_c
// ////////////////////////////////////////////////////////////////////////////

template <typename Id_>
struct OrthonormalBasis_c
{
    typedef TypeList_t<Basis_c<Id_> > ParentTypeList;

    typedef Id_ Id;

    static std::string type_as_string ()
    {
        return "OrthonormalBasis_c<" + type_string_of<Id_>() + '>';
    }
};

template <typename Id_>
struct IsConcept_f<OrthonormalBasis_c<Id_> >
{ static bool const V = true; };

template <typename T_> struct IsOrthonormalBasis_f { static bool const V = false; };
template <typename Id_> struct IsOrthonormalBasis_f<OrthonormalBasis_c<Id_> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(OrthonormalBasis);
// special convenience macros
#define IS_ORTHONORMAL_BASIS_UNIQUELY(Concept) HasUniqueOrthonormalBasisStructure_f<Concept>::V
#define AS_ORTHONORMAL_BASIS(Concept) UniqueOrthonormalBasisStructureOf_f<Concept>::T

template <typename Id_>
struct DualOf_f<OrthonormalBasis_c<Id_> >
{
    typedef OrthonormalBasis_c<typename DualOf_f<Id_>::T> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_BASIS_HPP_
