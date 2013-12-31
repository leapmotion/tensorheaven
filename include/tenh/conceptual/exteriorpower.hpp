// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/exteriorpower.hpp by Victor Dods, created 2013/08/15
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_EXTERIORPOWER_HPP_
#define TENH_CONCEPTUAL_EXTERIORPOWER_HPP_

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp" // technically not conceptual code, but close enough.
#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/linearembedding.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/mathutil.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/multiindex.hpp" // technically not conceptual code, but close enough.

namespace Tenh {

// exterior power of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <Uint32 ORDER_, typename Factor_>
struct ExteriorPower_c
{
    typedef EmptyTypeList ParentTypeList;

    static Uint32 const ORDER = ORDER_;
    typedef Factor_ Factor;
    typedef typename UniformTypeListOfLength_t<ORDER_,Factor_>::T FactorTypeList;

    static std::string type_as_string ()
    {
        return "ExteriorPower_c<" + AS_STRING(ORDER_) + ',' + type_string_of<Factor_>() + '>';
    }
};

template <Uint32 ORDER_, typename Factor_>
struct IsConcept_f<ExteriorPower_c<ORDER_,Factor_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsExteriorPower_f
{
    static bool const V = false;
private:
    IsExteriorPower_f();
};
template <Uint32 ORDER, typename Factor> struct IsExteriorPower_f<ExteriorPower_c<ORDER,Factor> >
{
    static bool const V = true;
private:
    IsExteriorPower_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ExteriorPower);
// special convenience macros
#define IS_EXTERIOR_POWER_UNIQUELY(Concept) HasUniqueExteriorPowerStructure_f<Concept>::V
#define AS_EXTERIOR_POWER(Concept) UniqueExteriorPowerStructureOf_f<Concept>::T


template <Uint32 ORDER_, typename Factor_>
struct ExteriorPowerOfVectorSpace_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_VECTOR_SPACE), };

    typedef ExteriorPower_c<ORDER_,Factor_> As_ExteriorPower;
    typedef VectorSpace_c<typename ScalarFieldOf_f<Factor_>::T,BinomialCoefficient_t<DimensionOf_f<Factor_>::V, ORDER_>::V,ExteriorPower_c<ORDER_,typename Factor_::Id> > As_VectorSpace;
    typedef EmbeddableInTensorProductOfVectorSpaces_c<typename TensorPowerOfVectorSpace_f<ORDER_,Factor_>::T> As_EmbeddableInTensorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_ExteriorPower,
            TypeList_t<As_VectorSpace,
            TypeList_t<As_EmbeddableInTensorProductOfVectorSpaces> > > ParentTypeList;

    typedef typename As_VectorSpace::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorPowerOfVectorSpace_c<" + AS_STRING(ORDER_) + ',' + type_string_of<Factor_>() + '>';
    }
};

template <Uint32 ORDER_, typename Factor_>
struct IsConcept_f<ExteriorPowerOfVectorSpace_c<ORDER_,Factor_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsExteriorPowerOfVectorSpace_f
{
    static bool const V = false;
private:
    IsExteriorPowerOfVectorSpace_f();
};
template <Uint32 ORDER, typename Factor> struct IsExteriorPowerOfVectorSpace_f<ExteriorPowerOfVectorSpace_c<ORDER,Factor> >
{
    static bool const V = true;
private:
    IsExteriorPowerOfVectorSpace_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ExteriorPowerOfVectorSpace);
// special convenience macros
#define IS_EXTERIOR_POWER_OF_VECTOR_SPACE_UNIQUELY(Concept) HasUniqueExteriorPowerOfVectorSpaceStructure_f<Concept>::V
#define AS_EXTERIOR_POWER_OF_VECTOR_SPACE(Concept) UniqueExteriorPowerOfVectorSpaceStructureOf_f<Concept>::T


// Factor_ must be a Basis_c type
template <Uint32 ORDER_, typename Factor_>
struct ExteriorPowerOfBasis_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Factor_), MUST_BE_BASIS) };
    typedef ExteriorPower_c<ORDER_,Factor_> As_ExteriorPower;
    typedef Basis_c<ExteriorPower_c<ORDER_,Factor_> > As_Basis;
public:
    typedef TypeList_t<As_ExteriorPower,
            TypeList_t<As_Basis> > ParentTypeList;

    typedef typename As_Basis::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorPowerOfBasis_c<" + AS_STRING(ORDER_) + ',' + type_string_of<Factor_>() + '>';
    }
};

template <Uint32 ORDER_, typename Factor_>
struct IsConcept_f<ExteriorPowerOfBasis_c<ORDER_,Factor_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsExteriorPowerOfBasis_f
{
    static bool const V = false;
private:
    IsExteriorPowerOfBasis_f();
};
template <Uint32 ORDER, typename Factor> struct IsExteriorPowerOfBasis_f<ExteriorPowerOfBasis_c<ORDER,Factor> >
{
    static bool const V = true;
private:
    IsExteriorPowerOfBasis_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ExteriorPowerOfBasis);
// special convenience macros
#define IS_EXTERIOR_POWER_OF_BASES_UNIQUELY(Concept) HasUniqueExteriorPowerOfBasisStructure_f<Concept>::V
#define AS_EXTERIOR_POWER_OF_BASES(Concept) UniqueExteriorPowerOfBasisStructureOf_f<Concept>::T


template <typename ExteriorPowerOfVectorSpace_, typename Basis_>
struct BasedExteriorPowerOfVectorSpace_c
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IS_EXTERIOR_POWER_OF_VECTOR_SPACE_UNIQUELY(ExteriorPowerOfVectorSpace_), MUST_BE_EXTERIOR_POWER_OF_VECTOR_SPACE),
        STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Basis_), MUST_BE_BASIS),
    };
    typedef ExteriorPowerOfVectorSpace_ As_ExteriorPowerOfVectorSpace;
    typedef BasedVectorSpace_c<ExteriorPowerOfVectorSpace_,Basis_> As_BasedVectorSpace;
public:
    typedef TypeList_t<As_ExteriorPowerOfVectorSpace,
            TypeList_t<As_BasedVectorSpace> > ParentTypeList;

    typedef typename As_BasedVectorSpace::Id Id;

    static std::string type_as_string ()
    {
        return "BasedExteriorPowerOfVectorSpace_c<" + type_string_of<ExteriorPowerOfVectorSpace_>() + ','
                                                    + type_string_of<Basis_>() + '>';
    }
};

template <typename ExteriorPowerOfVectorSpace, typename Basis_>
struct IsConcept_f<BasedExteriorPowerOfVectorSpace_c<ExteriorPowerOfVectorSpace, Basis_> >
{ static bool const V = true; };

template <typename T> struct IsBasedExteriorPowerOfVectorSpace_f { static bool const V = false; };
template <typename ExteriorPowerOfVectorSpace, typename Basis> struct IsBasedExteriorPowerOfVectorSpace_f<BasedExteriorPowerOfVectorSpace_c<ExteriorPowerOfVectorSpace,Basis> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedExteriorPowerOfVectorSpace);
// special convenience macros
#define IS_BASED_EXTERIOR_POWER_OF_VECTOR_SPACE_UNIQUELY(Concept) HasUniqueBasedExteriorPowerOfVectorSpaceStructure_f<Concept>::V
#define AS_BASED_EXTERIOR_POWER_OF_VECTOR_SPACE(Concept) UniqueBasedExteriorPowerOfVectorSpaceStructureOf_f<Concept>::T

// TODO: thinka bout this
// template <typename ExteriorPowerOfVectorSpace, typename Basis>
// struct DualOf_f<BasedExteriorPowerOfVectorSpace_c<ExteriorPowerOfVectorSpace,Basis> >
// {
//     typedef BasedExteriorPowerOfVectorSpace_c<typename DualOf_f<ExteriorPowerOfVectorSpace>::T,typename DualOf_f<Basis>::T> T;
// private:
//     DualOf_f();
// };


// Factor_ must be a BasedVectorSpace_c type
template <Uint32 ORDER_, typename Factor_>
struct ExteriorPowerOfBasedVectorSpace_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_BASED_VECTOR_SPACE), };
    typedef typename UniformTypeListOfLength_t<ORDER_,Factor_>::T FactorTypeList;

    typedef BasedExteriorPowerOfVectorSpace_c<ExteriorPowerOfVectorSpace_c<ORDER_,Factor_>,
                                              ExteriorPowerOfBasis_c<ORDER_,typename BasisOf_f<Factor_>::T> > As_BasedExteriorPowerOfVectorSpace;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T,
                                                           typename TensorPowerOfVectorSpace_f<ORDER_,Factor_>::T> As_EmbeddableInTensorProductOfBasedVectorSpaces;
public:
    typedef TypeList_t<As_BasedExteriorPowerOfVectorSpace,
            TypeList_t<As_EmbeddableInTensorProductOfBasedVectorSpaces> > ParentTypeList;

    typedef typename As_BasedExteriorPowerOfVectorSpace::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorPowerOfBasedVectorSpace_c<" + AS_STRING(ORDER_) + ',' + type_string_of<Factor_>() + '>';
    }
};

template <Uint32 ORDER_, typename Factor_>
struct IsConcept_f<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T_> struct IsExteriorPowerOfBasedVectorSpace_f
{
    static bool const V = false;
private:
    IsExteriorPowerOfBasedVectorSpace_f();
};
template <Uint32 ORDER_, typename Factor_> struct IsExteriorPowerOfBasedVectorSpace_f<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_> >
{
    static bool const V = true;
private:
    IsExteriorPowerOfBasedVectorSpace_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(ExteriorPowerOfBasedVectorSpace);
// special convenience macros
#define IS_EXTERIOR_POWER_OF_BASED_VECTOR_SPACE_UNIQUELY(Concept) HasUniqueExteriorPowerOfBasedVectorSpaceStructure_f<Concept>::V
#define AS_EXTERIOR_POWER_OF_BASED_VECTOR_SPACE(Concept) UniqueExteriorPowerOfBasedVectorSpaceStructureOf_f<Concept>::T

// in the category of based vector spaces, the dual space functor commutes
// with the kth exterior power functor.
template <Uint32 ORDER_, typename Factor_>
struct DualOf_f<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_> >
{
    typedef ExteriorPowerOfBasedVectorSpace_c<ORDER_,typename DualOf_f<Factor_>::T> T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// helper functions for easing use of the type system
// ///////////////////////////////////////////////////////////////////////////

// for now, just do exterior power of based vector spaces
template <Uint32 ORDER_, typename Factor_>
ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_> ext (Factor_ const &)
{
    return ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>();
}

// ///////////////////////////////////////////////////////////////////////////
// linear embedding of exterior power into corresponding tensor product
// ///////////////////////////////////////////////////////////////////////////

// TODO: think about how 0th powers should work (since they don't have an index)

// specialization for 1st exterior power -- both the 1st exterior and 1st
// tensor power of a vector space are naturally isomorphic to the vector space
// itself, so the embedding is effectively the identity.
template <typename Factor_, typename Scalar_>
struct LinearEmbedding_c<ExteriorPowerOfBasedVectorSpace_c<1,Factor_>,
                         typename TensorPowerOfBasedVectorSpace_f<1,Factor_>::T,
                         Scalar_,
                         NaturalEmbedding>
{
private:
    typedef ExteriorPowerOfBasedVectorSpace_c<1,Factor_> Ext;
    typedef typename TensorPowerOfBasedVectorSpace_f<1,Factor_>::T TPow;
    typedef ComponentIndex_t<DimensionOf_f<Factor_>::V> FactorComponentIndex;
    typedef MultiIndex_t<typename UniformTypeListOfLength_t<1,FactorComponentIndex>::T> TPowMultiIndex;
public:
    typedef ComponentIndex_t<DimensionOf_f<Ext>::V> ExtComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<TPow>::V> TPowComponentIndex;

    static bool embedded_component_is_procedural_zero (TPowComponentIndex const &) { return false; }
    static Scalar_ scalar_factor_for_embedded_component (TPowComponentIndex const &) { return Scalar_(1); }
    static ExtComponentIndex source_component_index_for_embedded_component (TPowComponentIndex const &i) { return i; }

    static Uint32 term_count_for_coembedded_component (ExtComponentIndex const &) { return 1; }
    static Scalar_ scalar_factor_for_coembedded_component (ExtComponentIndex const &, Uint32) { return Scalar_(1); }
    static TPowComponentIndex source_component_index_for_coembedded_component (ExtComponentIndex const &i, Uint32) { return i; }
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_>
struct LinearEmbedding_c<ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_>,
                         typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T,
                         Scalar_,
                         NaturalEmbedding>
{
private:
    typedef ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_> Ext;
    typedef typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T TPow;
    typedef ComponentIndex_t<DimensionOf_f<Factor_>::V> FactorComponentIndex;
    typedef MultiIndex_t<typename UniformTypeListOfLength_t<ORDER_,FactorComponentIndex>::T> TPowMultiIndex;
public:
    typedef ComponentIndex_t<DimensionOf_f<Ext>::V> ExtComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<TPow>::V> TPowComponentIndex;

    static bool embedded_component_is_procedural_zero (TPowComponentIndex const &i)
    {
        TPowMultiIndex m(i); // this does the row-major conversion
        // sort into non-increasing order -- choosing this instead of non-decreasing
        // makes certain formulas not depend on the dimension of Factor_
        sort<std::greater<Uint32> >(m);
        // if there is any index that occurs at least twice, then this is procedural zero.
        for (Uint32 j = 0; j < ORDER_-1; ++j)
            if (m.index(j, DONT_CHECK_RANGE) == m.index(j+1, DONT_CHECK_RANGE))
                return true;
        return false;
    }
    static Scalar_ scalar_factor_for_embedded_component (TPowComponentIndex const &i)
    {
        TPowMultiIndex m(i); // this does the row-major conversion

        int sign = 1;
        for (Uint32 j = 0; j < ORDER_; ++j)
            if (m.head().value() > m.index(j).value())
                sign *= -1;

        static Uint32 const NEXT_ORDER_DOWN = (ORDER_ <= 1) ? ORDER_ : (ORDER_ - 1);
        typedef LinearEmbedding_c<ExteriorPowerOfBasedVectorSpace_c<NEXT_ORDER_DOWN,Factor_>,
                                  typename TensorPowerOfBasedVectorSpace_f<NEXT_ORDER_DOWN,Factor_>::T,
                                  Scalar_,
                                  NaturalEmbedding> BodyLinearEmbedding;
        // NOTE: this is really inefficient because it converts to and then from a ComponentIndex.
        // it could be better implemented using a private scalar_factor_for_embedded_component(MultiIndex)
        return sign * BodyLinearEmbedding::scalar_factor_for_embedded_component(m.body().as_component_index());
    }
    static ExtComponentIndex source_component_index_for_embedded_component (TPowComponentIndex const &i)
    {
        TPowMultiIndex m(i); // this does the row-major conversion
        // sort into non-increasing order -- choosing this instead of non-decreasing
        // makes certain formulas not depend on the dimension of Factor_
        sort<std::greater<Uint32> >(m);

        static Uint32 const NEXT_ORDER_DOWN = (ORDER_ <= 1) ? ORDER_ : (ORDER_ - 1);
        typedef LinearEmbedding_c<ExteriorPowerOfBasedVectorSpace_c<NEXT_ORDER_DOWN,Factor_>,
                                  typename TensorPowerOfBasedVectorSpace_f<NEXT_ORDER_DOWN,Factor_>::T,
                                  Scalar_,
                                  NaturalEmbedding> BodyLinearEmbedding;
        // NOTE: this is really inefficient because it converts to and then from a ComponentIndex.
        // it could be better implemented using a private scalar_factor_for_embedded_component(MultiIndex)
        return ExtComponentIndex(  binomial_coefficient(m.head().value(), ORDER_)
                                 + BodyLinearEmbedding::source_component_index_for_embedded_component(m.body().as_component_index()).value());
    }

    // static Uint32 term_count_for_coembedded_component (ExtComponentIndex const &)
    // {
    //     return Factorial_t<ORDER_>::V;
    // }
    // // i corresponds to a unique multiindex M that is sorted in
    // // nondecreasing order.  this index has an isotropy subgroup H of the
    // // symmetric group G on ORDER_ letters.  the quotient of G by H is not
    // // necessarily a group (because H is not necessarily normal).  picking
    // // a section of this quotient map gives a set of representative permutations
    // // whose action on M give all TPowMultiIndex values for M.  the signs of the
    // // representative permutations give the scalar factor for the coembedded
    // // components.
    // static Scalar_ scalar_factor_for_coembedded_component (ExtComponentIndex const &i, Uint32 term)
    // {
    //     // if a Gray coding scheme is used to map [0, term) to the non-invariant
    //     // permutations of the multiindex M corresponding to i, such that the
    //     // parity of the index in [0, term) gives the sign of the permutation.
    // }
    // static TPowComponentIndex source_component_index_for_coembedded_component (ExtComponentIndex const &i, Uint32 term)
    // {
    // }
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_EXTERIORPOWER_HPP_
