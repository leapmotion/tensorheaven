// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/basis.hpp by Victor Dods, created 2013/07/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_BASIS_HPP_
#define TENH_CONCEPTUAL_BASIS_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/dual.hpp"

namespace Tenh {

// ////////////////////////////////////////////////////////////////////////////
// Basis_c
// ////////////////////////////////////////////////////////////////////////////

template <typename Id_>
struct Basis_c
{
    typedef Typle_t<> ParentTyple;

    typedef Id_ Id;

    static std::string type_as_string ()
    {
        return "Basis_c<" + type_string_of<Id_>() + '>';
    }
};

template <typename Id_>
struct IsConcept_f<Basis_c<Id_>>
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T_> struct IsBasis_f
{
    static bool const V = false;
private:
    IsBasis_f();
};
template <typename Id_> struct IsBasis_f<Basis_c<Id_>>
{
    static bool const V = true;
private:
    IsBasis_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Basis);
// special convenience macros
#define IS_BASIS_UNIQUELY(Concept) HasUniqueBasisStructure_f<Concept>::V
#define AS_BASIS(Concept) UniqueBasisStructureOf_f<Concept>::T

template <typename Id_>
struct DualOf_f<Basis_c<Id_>>
{
    typedef Basis_c<typename DualOf_f<Id_>::T> T;
private:
    DualOf_f();
};

// ////////////////////////////////////////////////////////////////////////////
// OrthonormalBasis_c
// ////////////////////////////////////////////////////////////////////////////

template <typename Id_>
struct OrthonormalBasis_c
{
    typedef Typle_t<Basis_c<Id_>> ParentTyple;

    typedef Id_ Id;

    static std::string type_as_string ()
    {
        return "OrthonormalBasis_c<" + type_string_of<Id_>() + '>';
    }
};

template <typename Id_>
struct IsConcept_f<OrthonormalBasis_c<Id_>>
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T_> struct IsOrthonormalBasis_f
{
    static bool const V = false;
private:
    IsOrthonormalBasis_f();
};
template <typename Id_> struct IsOrthonormalBasis_f<OrthonormalBasis_c<Id_>>
{
    static bool const V = true;
private:
    IsOrthonormalBasis_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(OrthonormalBasis);
// special convenience macros
#define IS_ORTHONORMAL_BASIS_UNIQUELY(Concept) HasUniqueOrthonormalBasisStructure_f<Concept>::V
#define AS_ORTHONORMAL_BASIS(Concept) UniqueOrthonormalBasisStructureOf_f<Concept>::T

template <typename Id_>
struct DualOf_f<OrthonormalBasis_c<Id_>>
{
    typedef OrthonormalBasis_c<typename DualOf_f<Id_>::T> T;
private:
    DualOf_f();
};

template <typename Typle_>
struct AllTypesHaveBasisStructures_f // TEMP
{
    static bool const V = And_f<typename OnEach_f<Typle_,HasBasisStructure_e>::T>::V;
};

template <typename Typle_>
struct AllTypesHaveUniqueBasisStructures_f
{
    static bool const V = And_f<typename OnEach_f<Typle_,HasUniqueBasisStructure_e>::T>::V;
};

MAKE_1_ARY_VALUE_EVALUATOR(AllTypesHaveUniqueBasisStructures);

/*
// TODO: change usage of this into EachTypeSatisfiesPredicate_f (or whatever)
template <typename SummandTyple_>
struct AllFactorsAreBases_f
{
    static bool const V = HasBasisStructure_f<typename Head_f<SummandTyple_>::T>::V &&
                          AllFactorsAreBases_f<typename BodyTyple_f<SummandTyple_>::T>::V;
private:
    AllFactorsAreBases_f();
};

template <>
struct AllFactorsAreBases_f<Typle_t<>>
{
    static bool const V = true;
private:
    AllFactorsAreBases_f();
};
*/
// TODO: use OnEach_f
template <typename Typle_>
struct IdsOfTyple_f
{
    typedef typename HeadBodyTyple_f<typename Head_f<Typle_>::T::Id,
                                     typename IdsOfTyple_f<typename BodyTyple_f<Typle_>::T>::T>::T T;
private:
    IdsOfTyple_f();
};

template <>
struct IdsOfTyple_f<Typle_t<>>
{
    typedef Typle_t<> T;
private:
    IdsOfTyple_f();
};

// ///////////////////////////////////////////////////////////////////////////
// helper functions for easing use of the type system
// ///////////////////////////////////////////////////////////////////////////

template <typename Id_>
Basis_c<Id_> basis (Id_ const &)
{
    return Basis_c<Id_>();
}

// convenience function equivalent to basis(Generic())
inline Basis_c<Generic> generic_basis ()
{
    return Basis_c<Generic>();
}

template <typename Id_>
OrthonormalBasis_c<Id_> o_n_basis (Id_ const &)
{
    return OrthonormalBasis_c<Id_>();
}

// convenience function equivalent to orthonormal_basis(Generic())
inline OrthonormalBasis_c<Generic> generic_o_n_basis ()
{
    return OrthonormalBasis_c<Generic>();
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_BASIS_HPP_
