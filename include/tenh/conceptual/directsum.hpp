// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/directsum.hpp by Ted Nitz, created 2013/10/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_DIRECTSUM_HPP_
#define TENH_CONCEPTUAL_DIRECTSUM_HPP_

#include "tenh/core.hpp"

#include <type_traits>

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/meta/typle.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// DirectSum_c
// ///////////////////////////////////////////////////////////////////////////

// generic direct sum of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <typename SummandTyple_>
struct DirectSum_c
{
    static_assert(IsTyple_f<SummandTyple_>::V, "SummandTyple_ must be a Typle_t");

    typedef Typle_t<> ParentTyple;

    typedef SummandTyple_ SummandTyple;
    static Uint32 const SUMMAND_COUNT = Length_f<SummandTyple>::V;

    static std::string type_as_string ()
    {
        return "DirectSum_c<" + type_string_of<SummandTyple_>() + '>';
    }
};

template <typename SummandTyple_>
struct IsConcept_f<DirectSum_c<SummandTyple_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsDirectSum_f
{
    static bool const V = false;
private:
    IsDirectSum_f();
};
template <typename SummandTyple_> struct IsDirectSum_f<DirectSum_c<SummandTyple_> >
{
    static bool const V = true;
private:
    IsDirectSum_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(DirectSum);
// special convenience macros
#define IS_DIRECT_SUM_UNIQUELY(Concept) HasUniqueDirectSumStructure_f<Concept>::V
#define AS_DIRECT_SUM(Concept) UniqueDirectSumStructureOf_f<Concept>::T

template <typename SummandTyple_>
struct DualOf_f<DirectSum_c<SummandTyple_> >
{
    typedef DirectSum_c<typename DualOf_f<SummandTyple_>::T> T;
private:
    DualOf_f();
};

// property IDs

struct SummandCount;
struct SummandTyple;
struct UniformDirectSumSummand;

// BaseProperty_f accessors

template <typename SummandTyple_>
struct BaseProperty_f<DirectSum_c<SummandTyple_>,SummandCount>
{
    typedef Value_t<Uint32,Length_f<SummandTyple_>::V> T;
private:
    BaseProperty_f();
};

template <typename SummandTyple_>
struct BaseProperty_f<DirectSum_c<SummandTyple_>,SummandTyple>
{
    typedef SummandTyple_ T;
private:
    BaseProperty_f();
};

template <typename SummandTyple_>
struct BaseProperty_f<DirectSum_c<SummandTyple_>,UniformDirectSumSummand>
{
private:
    static bool const THERE_IS_A_UNIQUE_SUMMAND = Length_f<SummandTyple_>::V >= 1 &&
                                                  TypleIsUniform_f<SummandTyple_>::V;
    BaseProperty_f();
public:
    typedef typename If_f<THERE_IS_A_UNIQUE_SUMMAND,
                          typename Head_f<SummandTyple_>::T,
                          NullValue>::T T;
};

// named property accessors

template <typename Concept_> struct SummandCountOf_f
{
    static Uint32 const V = PropertyValue_f<Concept_,SummandCount>::V;
private:
    SummandCountOf_f();
};
template <typename Concept_> struct SummandTypleOf_f
{
    typedef typename Property_f<Concept_,SummandTyple>::T T;
private:
    SummandTypleOf_f();
};
template <typename Concept_> struct UniformDirectSumSummandOf_f
{
    typedef typename Property_f<Concept_,UniformDirectSumSummand>::T T;
private:
    UniformDirectSumSummandOf_f();
};

MAKE_1_ARY_VALUE_EVALUATOR(SummandCountOf);
MAKE_1_ARY_TYPE_EVALUATOR(SummandTypleOf);
MAKE_1_ARY_TYPE_EVALUATOR(UniformDirectSumSummandOf);

// ///////////////////////////////////////////////////////////////////////////
// DirectSumOfVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

template <typename SummandTyple_>
struct DirectSumOfVectorSpaces_c
{
    static_assert(IsTyple_f<SummandTyple_>::V, "SummandTyple_ must be a Typle_t");
    static_assert(AllTypesHaveUniqueVectorSpaceStructures_f<SummandTyple_>::V, "all summands must be vector spaces");
    static_assert(AllTypesHaveSameScalarField_f<SummandTyple_>::V, "all summands must have the same scalar field");
private:
    typedef DirectSum_c<SummandTyple_> As_DirectSum;
    typedef VectorSpace_c<typename ScalarFieldOf_f<typename Head_f<SummandTyple_>::T>::T,
                          SumOfDimensions_f<SummandTyple_>::V,
                          DirectSum_c<typename IdsOfTyple_f<SummandTyple_>::T>> As_VectorSpace;
public:
    typedef Typle_t<As_DirectSum,As_VectorSpace> ParentTyple;

    typedef typename As_VectorSpace::Id Id;

    static std::string type_as_string ()
    {
        return "DirectSumOfVectorSpaces_c<" + type_string_of<SummandTyple_>() + '>';
    }
};

template <typename SummandTyple_>
struct IsConcept_f<DirectSumOfVectorSpaces_c<SummandTyple_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsDirectSumOfVectorSpaces_f
{
    static bool const V = false;
private:
    IsDirectSumOfVectorSpaces_f();
};
template <typename SummandTyple_> struct IsDirectSumOfVectorSpaces_f<DirectSumOfVectorSpaces_c<SummandTyple_> >
{
    static bool const V = true;
private:
    IsDirectSumOfVectorSpaces_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(DirectSumOfVectorSpaces);
// special convenience macros
#define IS_DIRECT_SUM_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueDirectSumOfVectorSpacesStructure_f<Concept>::V
#define AS_DIRECT_SUM_OF_VECTOR_SPACES(Concept) UniqueDirectSumOfVectorSpacesStructureOf_f<Concept>::T

template <typename SummandTyple_>
struct DualOf_f<DirectSumOfVectorSpaces_c<SummandTyple_> >
{
    typedef DirectSumOfVectorSpaces_c<typename DualOf_f<SummandTyple_>::T> T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// DirectSumOfBases_c
// ///////////////////////////////////////////////////////////////////////////

template <typename SummandTyple_>
struct DirectSumOfBases_c
{
    static_assert(IsTyple_f<SummandTyple_>::V, "SummandTyple_ must be a Typle_t");
    static_assert(AllTypesHaveBasisStructures_f<SummandTyple_>::V, "all summands must be bases");
    static_assert(AllTypesHaveUniqueBasisStructures_f<SummandTyple_>::V, "all summands must have unique basis structures");
private:
    typedef DirectSum_c<SummandTyple_> As_DirectSum;
    typedef Basis_c<DirectSum_c<typename IdsOfTyple_f<SummandTyple_>::T>> As_Basis;
public:
    typedef Typle_t<As_DirectSum,As_Basis> ParentTyple;

    typedef typename As_Basis::Id Id;

    static std::string type_as_string ()
    {
        return "DirectSumOfBases_c<" + type_string_of<SummandTyple_>() + '>';
    }
};

template <typename SummandTyple_>
struct IsConcept_f<DirectSumOfBases_c<SummandTyple_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsDirectSumOfBases_f
{
    static bool const V = false;
private:
    IsDirectSumOfBases_f();
};
template <typename SummandTyple_> struct IsDirectSumOfBases_f<DirectSumOfBases_c<SummandTyple_> >
{
    static bool const V = true;
private:
    IsDirectSumOfBases_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(DirectSumOfBases);
// special convenience macros
#define IS_DIRECT_SUM_OF_BASES_UNIQUELY(Concept) HasUniqueDirectSumOfBasesStructure_f<Concept>::V
#define AS_DIRECT_SUM_OF_BASES(Concept) UniqueDirectSumOfBasesStructureOf_f<Concept>::T

template <typename SummandTyple_>
struct DualOf_f<DirectSumOfBases_c<SummandTyple_> >
{
    typedef DirectSumOfBases_c<typename DualOf_f<SummandTyple_>::T> T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// BasedDirectSumOfVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

// TODO: this should be an EmbeddableInDirectSumOfVectorSpaces
template <typename DirectSumOfVectorSpaces_, typename Basis_>
struct BasedDirectSumOfVectorSpaces_c
{
    static_assert(IS_DIRECT_SUM_OF_VECTOR_SPACES_UNIQUELY(DirectSumOfVectorSpaces_), "DirectSumOfVectorSpaces_ must have unique direct sum of vector spaces structure");
    static_assert(IS_BASIS_UNIQUELY(Basis_), "Basis_ must have unique basis structure");
private:
    typedef DirectSumOfVectorSpaces_ As_DirectSumOfVectorSpaces;
    typedef BasedVectorSpace_c<DirectSumOfVectorSpaces_,Basis_> As_BasedVectorSpace;
public:
    typedef Typle_t<As_DirectSumOfVectorSpaces,As_BasedVectorSpace> ParentTyple;

    typedef typename As_BasedVectorSpace::Id Id;

    static std::string type_as_string ()
    {
        return "BasedDirectSumOfVectorSpaces_c<" + type_string_of<DirectSumOfVectorSpaces_>() + ','
                                                 + type_string_of<Basis_>() + '>';
    }
};

template <typename DirectSumOfVectorSpaces_, typename Basis_>
struct IsConcept_f<BasedDirectSumOfVectorSpaces_c<DirectSumOfVectorSpaces_, Basis_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsBasedDirectSumOfVectorSpaces_f
{
    static bool const V = false;
private:
    IsBasedDirectSumOfVectorSpaces_f();
};
template <typename DirectSumOfVectorSpaces_, typename Basis_> struct IsBasedDirectSumOfVectorSpaces_f<BasedDirectSumOfVectorSpaces_c<DirectSumOfVectorSpaces_,Basis_> >
{
    static bool const V = true;
private:
    IsBasedDirectSumOfVectorSpaces_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedDirectSumOfVectorSpaces);
// special convenience macros
#define IS_BASED_DIRECT_SUM_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueBasedDirectSumOfVectorSpacesStructure_f<Concept>::V
#define AS_BASED_DIRECT_SUM_OF_VECTOR_SPACES(Concept) UniqueBasedDirectSumOfVectorSpacesStructureOf_f<Concept>::T

template <typename DirectSumOfVectorSpaces, typename Basis>
struct DualOf_f<BasedDirectSumOfVectorSpaces_c<DirectSumOfVectorSpaces,Basis> >
{
    typedef BasedDirectSumOfVectorSpaces_c<typename DualOf_f<DirectSumOfVectorSpaces>::T,typename DualOf_f<Basis>::T> T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// DirectSumOfBasedVectorSpaces_c
// ///////////////////////////////////////////////////////////////////////////

template <typename SummandTyple_>
struct DirectSumOfBasedVectorSpaces_c
{
    static_assert(IsTyple_f<SummandTyple_>::V, "SummandTyple_ must be a Typle_t");
    static_assert(AllTypesHaveUniqueBasedVectorSpaceStructures_f<SummandTyple_>::V, "all summands must be based vector spaces");
private:
    typedef typename PropertyOfEachInTyple_f<SummandTyple_,Basis>::T BasisTyple;
    typedef BasedDirectSumOfVectorSpaces_c<DirectSumOfVectorSpaces_c<SummandTyple_>,
                                           DirectSumOfBases_c<BasisTyple>> As_BasedDirectSumOfVectorSpaces;
public:
    typedef Typle_t<As_BasedDirectSumOfVectorSpaces> ParentTyple;

    typedef typename As_BasedDirectSumOfVectorSpaces::Id Id;

    static std::string type_as_string ()
    {
        return "DirectSumOfBasedVectorSpaces_c<" + type_string_of<SummandTyple_>() + '>';
    }
};

template <typename SummandTyple_>
struct IsConcept_f<DirectSumOfBasedVectorSpaces_c<SummandTyple_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsDirectSumOfBasedVectorSpaces_f
{
    static bool const V = false;
private:
    IsDirectSumOfBasedVectorSpaces_f();
};
template <typename SummandTyple_> struct IsDirectSumOfBasedVectorSpaces_f<DirectSumOfBasedVectorSpaces_c<SummandTyple_> >
{
    static bool const V = true;
private:
    IsDirectSumOfBasedVectorSpaces_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(DirectSumOfBasedVectorSpaces);
// special convenience macros
#define IS_DIRECT_SUM_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueDirectSumOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_DIRECT_SUM_OF_BASED_VECTOR_SPACES(Concept) UniqueDirectSumOfBasedVectorSpacesStructureOf_f<Concept>::T

// there is a natural identification of the dual of a direct sum of vector spaces with the
// direct sum of the duals of the vector spaces
template <typename SummandTyple_>
struct DualOf_f<DirectSumOfBasedVectorSpaces_c<SummandTyple_> >
{
    typedef DirectSumOfBasedVectorSpaces_c<typename DualOf_f<SummandTyple_>::T> T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// helper functions for easing use of the type system
// ///////////////////////////////////////////////////////////////////////////

// for now, just do direct sum of based vector spaces

template <typename LhsBasedVectorSpace_, typename RhsBasedVectorSpace_>
typename std::enable_if<(IS_BASED_VECTOR_SPACE_UNIQUELY(LhsBasedVectorSpace_) &&
                         IS_BASED_VECTOR_SPACE_UNIQUELY(RhsBasedVectorSpace_)),
                        DirectSumOfBasedVectorSpaces_c<Typle_t<LhsBasedVectorSpace_,
                                                               RhsBasedVectorSpace_>>>::type
    operator + (LhsBasedVectorSpace_ const &, RhsBasedVectorSpace_ const &)
{
    return DirectSumOfBasedVectorSpaces_c<Typle_t<LhsBasedVectorSpace_,
                                                  RhsBasedVectorSpace_>>();
}

template <typename SummandTyple_, typename BasedVectorSpace_>
DirectSumOfBasedVectorSpaces_c<typename Concat2Typles_f<SummandTyple_,
                                                               Typle_t<BasedVectorSpace_>>::T>
    operator + (DirectSumOfBasedVectorSpaces_c<SummandTyple_> const &, BasedVectorSpace_ const &)
{
    return DirectSumOfBasedVectorSpaces_c<typename Concat2Typles_f<SummandTyple_,
                                                                          Typle_t<BasedVectorSpace_>>::T>();
}

template <typename SummandTyple_, typename BasedVectorSpace_>
DirectSumOfBasedVectorSpaces_c<typename HeadBodyTyple_f<BasedVectorSpace_,SummandTyple_>::T>
    operator + (BasedVectorSpace_ const &, DirectSumOfBasedVectorSpaces_c<SummandTyple_> const &)
{
    return DirectSumOfBasedVectorSpaces_c<typename HeadBodyTyple_f<BasedVectorSpace_,SummandTyple_>::T>();
}

template <typename LhsSummandTyple_, typename RhsSummandTyple_>
DirectSumOfBasedVectorSpaces_c<Typle_t<DirectSumOfBasedVectorSpaces_c<LhsSummandTyple_>,
                                       DirectSumOfBasedVectorSpaces_c<RhsSummandTyple_>>>
    operator + (DirectSumOfBasedVectorSpaces_c<LhsSummandTyple_> const &,
                DirectSumOfBasedVectorSpaces_c<RhsSummandTyple_> const &)
{
    return DirectSumOfBasedVectorSpaces_c<Typle_t<DirectSumOfBasedVectorSpaces_c<LhsSummandTyple_>,
                                                  DirectSumOfBasedVectorSpaces_c<RhsSummandTyple_>>>();
}

// ///////////////////////////////////////////////////////////////////////////
// meta-functions for taking direct sum powers of stuff
// ///////////////////////////////////////////////////////////////////////////

template <Uint32 ORDER_, typename Summand_>
struct UniformDirectSum_f
{
    typedef DirectSum_c<typename UniformTypleOfLength_f<ORDER_,Summand_>::T> T;
private:
    UniformDirectSum_f ();
};

template <Uint32 ORDER_, typename Summand_>
struct UniformDirectSumOfVectorSpace_f
{
    static_assert(IS_VECTOR_SPACE_UNIQUELY(Summand_), "Summand_ must have a unique vector space structure");
    typedef DirectSumOfVectorSpaces_c<typename UniformTypleOfLength_f<ORDER_,Summand_>::T> T;
private:
    UniformDirectSumOfVectorSpace_f ();
};

template <Uint32 ORDER_, typename Summand_>
struct UniformDirectSumOfBasis_f
{
    static_assert(IS_BASIS_UNIQUELY(Summand_), "Summand_ must have a unique basis structure");
    typedef DirectSumOfBases_c<typename UniformTypleOfLength_f<ORDER_,Summand_>::T> T;
private:
    UniformDirectSumOfBasis_f ();
};

template <Uint32 ORDER_, typename Summand_>
struct UniformDirectSumOfBasedVectorSpace_f
{
    static_assert(IS_BASED_VECTOR_SPACE_UNIQUELY(Summand_), "Summand_ must have a unique based vector space structure");
    typedef DirectSumOfBasedVectorSpaces_c<typename UniformTypleOfLength_f<ORDER_,Summand_>::T> T;
private:
    UniformDirectSumOfBasedVectorSpace_f ();
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_DIRECTSUM_HPP_
