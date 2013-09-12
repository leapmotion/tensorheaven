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

// ////////////////////////////////////////////////////////////////////////////
// Standard[Basis]
// ////////////////////////////////////////////////////////////////////////////

struct Standard
{
    static std::string type_as_string () { return "Standard"; }
};

// convenience typedef for the standard [Euclidean] basis (basically is a pre-existing
// name for an orthonormal basis).
typedef Basis_c<Standard> StandardBasis;

// StandardBasis is self-dual (as a Basis_c type), though the space it refers to may not be.
template <>
struct DualOf_f<StandardBasis>
{
    typedef StandardBasis T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_BASIS_HPP_
