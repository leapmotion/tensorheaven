// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/symmetricpower.hpp by Victor Dods, created 2013/07/29
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_
#define TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_

#include "tenh/core.hpp"
#include "tenh/mathutil.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"

namespace Tenh {

// symmetric power of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <Uint32 ORDER_, typename Factor_>
struct SymmetricPower_c
{
    typedef EmptyTypeList ParentTypeList;

    static Uint32 const ORDER = ORDER_;
    typedef Factor_ Factor;
    typedef typename UniformTypeListOfLength_t<ORDER_,Factor_>::T FactorTypeList;

    static std::string type_as_string ()
    {
        return "SymmetricPower_c<" + AS_STRING(ORDER) + ',' + type_string_of<Factor>() + '>';
    }
};

template <Uint32 ORDER_, typename Factor_>
struct IsConcept_f<SymmetricPower_c<ORDER_,Factor_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsSymmetricPower_f
{
    static bool const V = false;
private:
    IsSymmetricPower_f();
};
template <Uint32 ORDER, typename Factor> struct IsSymmetricPower_f<SymmetricPower_c<ORDER,Factor> >
{
    static bool const V = true;
private:
    IsSymmetricPower_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPower);
// special convenience macros
#define IS_SYMMETRIC_POWER_UNIQUELY(Concept) HasUniqueSymmetricPowerStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER(Concept) UniqueSymmetricPowerStructureOf_f<Concept>::T


template <Uint32 ORDER_, typename Factor_>
struct SymmetricPowerOfVectorSpace_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_VECTOR_SPACE), };
    typedef SymmetricPower_c<ORDER_,Factor_> As_SymmetricPower;
    typedef VectorSpace_c<typename ScalarFieldOf_f<Factor_>::T,BinomialCoefficient_t<DimensionOf_f<Factor_>::V + ORDER_ - 1, ORDER_>::V,SymmetricPower_c<ORDER_,typename Factor_::Id> > As_VectorSpace;
    typedef EmbeddableInTensorProductOfVectorSpaces_c<typename TensorPowerOfVectorSpace_f<ORDER_,Factor_>::T> As_EmbeddableInTensorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_SymmetricPower,
            TypeList_t<As_VectorSpace,
            TypeList_t<As_EmbeddableInTensorProductOfVectorSpaces> > > ParentTypeList;

    typedef typename As_VectorSpace::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricPowerOfVectorSpace_c<" + AS_STRING(ORDER_) + ',' + type_string_of<Factor>() + '>';
    }
};

template <Uint32 ORDER_, typename Factor_>
struct IsConcept_f<SymmetricPowerOfVectorSpace_c<ORDER_,Factor_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsSymmetricPowerOfVectorSpace_f
{
    static bool const V = false;
private:
    IsSymmetricPowerOfVectorSpace_f();
};
template <Uint32 ORDER, typename Factor> struct IsSymmetricPowerOfVectorSpace_f<SymmetricPowerOfVectorSpace_c<ORDER,Factor> >
{
    static bool const V = true;
private:
    IsSymmetricPowerOfVectorSpace_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPowerOfVectorSpace);
// special convenience macros
#define IS_SYMMETRIC_POWER_OF_VECTOR_SPACE_UNIQUELY(Concept) HasUniqueSymmetricPowerOfVectorSpaceStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER_OF_VECTOR_SPACE(Concept) UniqueSymmetricPowerOfVectorSpaceStructureOf_f<Concept>::T


// Factor_ must be a Basis_c type
template <Uint32 ORDER_, typename Factor_>
struct SymmetricPowerOfBasis_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Factor_), MUST_BE_BASIS) };
    typedef SymmetricPower_c<ORDER_,Factor_> As_SymmetricPower;
    typedef Basis_c<SymmetricPower_c<ORDER_,Factor_> > As_Basis;
public:
    typedef TypeList_t<As_SymmetricPower,
            TypeList_t<As_Basis> > ParentTypeList;

    static Uint32 const ORDER = As_SymmetricPower::ORDER;
    typedef typename As_Basis::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricPowerOfBasis_c<" + AS_STRING(ORDER) + ',' + type_string_of<Factor>() + '>';
    }
};

template <Uint32 ORDER_, typename Factor_>
struct IsConcept_f<SymmetricPowerOfBasis_c<ORDER_,Factor_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsSymmetricPowerOfBasis_f
{
    static bool const V = false;
private:
    IsSymmetricPowerOfBasis_f();
};
template <Uint32 ORDER, typename Factor> struct IsSymmetricPowerOfBasis_f<SymmetricPowerOfBasis_c<ORDER,Factor> >
{
    static bool const V = true;
private:
    IsSymmetricPowerOfBasis_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPowerOfBasis);
// special convenience macros
#define IS_SYMMETRIC_POWER_OF_BASES_UNIQUELY(Concept) HasUniqueSymmetricPowerOfBasisStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER_OF_BASES(Concept) UniqueSymmetricPowerOfBasisStructureOf_f<Concept>::T


template <typename SymmetricPowerOfVectorSpace_, typename Basis_>
struct BasedSymmetricPowerOfVectorSpace_c
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IS_SYMMETRIC_POWER_OF_VECTOR_SPACE_UNIQUELY(SymmetricPowerOfVectorSpace_), MUST_BE_SYMMETRIC_POWER_OF_VECTOR_SPACE),
        STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Basis_), MUST_BE_BASIS),
    };
    typedef SymmetricPowerOfVectorSpace_ As_SymmetricPowerOfVectorSpace;
    typedef BasedVectorSpace_c<SymmetricPowerOfVectorSpace_,Basis_> As_BasedVectorSpace;
public:
    typedef TypeList_t<As_SymmetricPowerOfVectorSpace,
            TypeList_t<As_BasedVectorSpace> > ParentTypeList;

    typedef typename As_BasedVectorSpace::Id Id;

    static std::string type_as_string ()
    {
        return "BasedSymmetricPowerOfVectorSpace_c<" + type_string_of<SymmetricPowerOfVectorSpace_>() + ','
                                                     + type_string_of<Basis_>() + '>';
    }
};

template <typename SymmetricPowerOfVectorSpace, typename Basis_>
struct IsConcept_f<BasedSymmetricPowerOfVectorSpace_c<SymmetricPowerOfVectorSpace, Basis_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsBasedSymmetricPowerOfVectorSpace_f
{
    static bool const V = false;
private:
    IsBasedSymmetricPowerOfVectorSpace_f();
};
template <typename SymmetricPowerOfVectorSpace, typename Basis> struct IsBasedSymmetricPowerOfVectorSpace_f<BasedSymmetricPowerOfVectorSpace_c<SymmetricPowerOfVectorSpace,Basis> >
{
    static bool const V = true;
private:
    IsBasedSymmetricPowerOfVectorSpace_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedSymmetricPowerOfVectorSpace);
// special convenience macros
#define IS_BASED_SYMMETRIC_POWER_OF_VECTOR_SPACE_UNIQUELY(Concept) HasUniqueBasedSymmetricPowerOfVectorSpaceStructure_f<Concept>::V
#define AS_BASED_SYMMETRIC_POWER_OF_VECTOR_SPACE(Concept) UniqueBasedSymmetricPowerOfVectorSpaceStructureOf_f<Concept>::T

// TODO: thinka bout this
// template <typename SymmetricPowerOfVectorSpace, typename Basis>
// struct DualOf_f<BasedSymmetricPowerOfVectorSpace_c<SymmetricPowerOfVectorSpace,Basis> >
// {
//     typedef BasedSymmetricPowerOfVectorSpace_c<typename DualOf_f<SymmetricPowerOfVectorSpace>::T,typename DualOf_f<Basis>::T> T;
// };


// Factor_ must be a BasedVectorSpace_c type
template <Uint32 ORDER_, typename Factor_>
struct SymmetricPowerOfBasedVectorSpace_c
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor_), MUST_BE_BASED_VECTOR_SPACE) };
    typedef BasedSymmetricPowerOfVectorSpace_c<SymmetricPowerOfVectorSpace_c<ORDER_,Factor_>,
                                               SymmetricPowerOfBasis_c<ORDER_,typename BasisOf_f<Factor_>::T> > As_BasedSymmetricPowerOfVectorSpace;
    typedef typename UniformTypeListOfLength_t<ORDER_,Factor_>::T FactorTypeList;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T,
                                                           typename TensorPowerOfVectorSpace_f<ORDER_,Factor_>::T> As_EmbeddableInTensorProductOfBasedVectorSpaces;
public:
    typedef TypeList_t<As_BasedSymmetricPowerOfVectorSpace,
            TypeList_t<As_EmbeddableInTensorProductOfBasedVectorSpaces> > ParentTypeList;

    typedef typename As_BasedSymmetricPowerOfVectorSpace::Id Id;
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricPowerOfBasedVectorSpace_c<" + AS_STRING(ORDER_) + ',' + type_string_of<Factor_>() + '>';
    }
};

template <Uint32 ORDER_, typename Factor_>
struct IsConcept_f<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsSymmetricPowerOfBasedVectorSpace_f
{
    static bool const V = false;
private:
    IsSymmetricPowerOfBasedVectorSpace_f();
};
template <Uint32 ORDER_, typename Factor_> struct IsSymmetricPowerOfBasedVectorSpace_f<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> >
{
    static bool const V = true;
private:
    IsSymmetricPowerOfBasedVectorSpace_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(SymmetricPowerOfBasedVectorSpace);
// special convenience macros
#define IS_SYMMETRIC_POWER_OF_BASED_VECTOR_SPACE_UNIQUELY(Concept) HasUniqueSymmetricPowerOfBasedVectorSpaceStructure_f<Concept>::V
#define AS_SYMMETRIC_POWER_OF_BASED_VECTOR_SPACE(Concept) UniqueSymmetricPowerOfBasedVectorSpaceStructureOf_f<Concept>::T

// in the category of based vector spaces, the dual space functor commutes
// with the kth symmetric power functor, modulo a particular natural isomorphism.
template <Uint32 ORDER_, typename Factor_>
struct DualOf_f<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> >
{
    typedef SymmetricPowerOfBasedVectorSpace_c<ORDER_,typename DualOf_f<Factor_>::T> T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// helper functions for easing use of the type system
// ///////////////////////////////////////////////////////////////////////////

// for now, just do symmetric power of based vector spaces
template <Uint32 ORDER_, typename Factor_>
SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> sym (Factor_ const &)
{
    return SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>();
}

// ///////////////////////////////////////////////////////////////////////////
// linear embedding of symmetric power into corresponding tensor product
// ///////////////////////////////////////////////////////////////////////////

// TODO: think about how 0th powers should work (since they don't have an index)

// specialization for 1st symmetric power -- both the 1st symmetric and 1st
// tensor power of a vector space are naturally isomorphic to the vector space
// itself, so the embedding is effectively the identity.
template <typename Factor_, typename Scalar_>
struct LinearEmbedding_c<SymmetricPowerOfBasedVectorSpace_c<1,Factor_>,
                         typename TensorPowerOfBasedVectorSpace_f<1,Factor_>::T,
                         Scalar_,
                         NaturalEmbedding>
{
private:
    typedef SymmetricPowerOfBasedVectorSpace_c<1,Factor_> Sym;
    typedef typename TensorPowerOfBasedVectorSpace_f<1,Factor_>::T TPow;
    typedef ComponentIndex_t<DimensionOf_f<Factor_>::V> FactorComponentIndex;
    typedef MultiIndex_t<typename UniformTypeListOfLength_t<1,FactorComponentIndex>::T> TPowMultiIndex;
public:
    typedef ComponentIndex_t<DimensionOf_f<Sym>::V> SymComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<TPow>::V> TPowComponentIndex;

    static bool embedded_component_is_procedural_zero (TPowComponentIndex const &) { return false; }
    static Scalar_ scalar_factor_for_embedded_component (TPowComponentIndex const &) { return Scalar_(1); }
    static SymComponentIndex source_component_index_for_embedded_component (TPowComponentIndex const &i) { return i; }

    static Uint32 term_count_for_coembedded_component (SymComponentIndex const &) { return 1; }
    static Scalar_ scalar_factor_for_coembedded_component (SymComponentIndex const &, Uint32) { return Scalar_(1); }
    static TPowComponentIndex source_component_index_for_coembedded_component (SymComponentIndex const &i, Uint32) { return i; }
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_>
struct LinearEmbedding_c<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,
                         typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T,
                         Scalar_,
                         NaturalEmbedding>
{
private:
    typedef SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> Sym;
    typedef typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T TPow;
    typedef ComponentIndex_t<DimensionOf_f<Factor_>::V> FactorComponentIndex;
    typedef MultiIndex_t<typename UniformTypeListOfLength_t<ORDER_,FactorComponentIndex>::T> TPowMultiIndex;
public:
    typedef ComponentIndex_t<DimensionOf_f<Sym>::V> SymComponentIndex;
    typedef ComponentIndex_t<DimensionOf_f<TPow>::V> TPowComponentIndex;

    static bool embedded_component_is_procedural_zero (TPowComponentIndex const &i) { return false; }
    static Scalar_ scalar_factor_for_embedded_component (TPowComponentIndex const &i) { return Scalar_(1); }
    static SymComponentIndex source_component_index_for_embedded_component (TPowComponentIndex const &i)
    {
        TPowMultiIndex m(i); // this does the row-major conversion
        // sort into non-increasing order -- choosing this instead of non-decreasing
        // makes certain formulas not depend on the dimension of Factor_
        sort<std::greater<Uint32> >(m);

        static Uint32 const NEXT_ORDER_DOWN = (ORDER_ <= 1) ? ORDER_ : (ORDER_ - 1);
        typedef LinearEmbedding_c<SymmetricPowerOfBasedVectorSpace_c<NEXT_ORDER_DOWN,Factor_>,
                                  typename TensorPowerOfBasedVectorSpace_f<NEXT_ORDER_DOWN,Factor_>::T,
                                  Scalar_,
                                  NaturalEmbedding> BodyLinearEmbedding;
        // NOTE: this is really inefficient because it converts to and then from a ComponentIndex.
        // it could be better implemented using a private scalar_factor_for_embedded_component(MultiIndex)
        return SymComponentIndex(  binomial_coefficient(m.head().value() + ORDER_ - 1, ORDER_)
                                 + BodyLinearEmbedding::source_component_index_for_embedded_component(m.body().as_component_index()).value());
    }

    // static Uint32 term_count_for_coembedded_component (SymComponentIndex const &)
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
    // static Scalar_ scalar_factor_for_coembedded_component (SymComponentIndex const &i, Uint32 term)
    // {
    //     // if a Gray coding scheme is used to map [0, term) to the non-invariant
    //     // permutations of the multiindex M corresponding to i, such that the
    //     // parity of the index in [0, term) gives the sign of the permutation.
    // }
    // static TPowComponentIndex source_component_index_for_coembedded_component (SymComponentIndex const &i, Uint32 term)
    // {
    // }
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_
