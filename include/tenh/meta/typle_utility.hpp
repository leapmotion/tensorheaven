// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typle_utility.hpp by Victor Dods, created 2014/01/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

/// @file typle_utility.hpp
/// @headerfile typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Contains utility functions for working with Typle_t types.

#ifndef TENH_META_TYPLE_UTILITY_HPP_
#define TENH_META_TYPLE_UTILITY_HPP_

#include "tenh/core.hpp"

#include <limits>

#include "tenh/meta/function.hpp"
#include "tenh/meta/typle.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// higher-order metafunctions
// ///////////////////////////////////////////////////////////////////////////

namespace Hippo { // temp namespace until TypeList_t is gone

// composition of metafunctions -- each type in the typelist must be an evaluator.

// forward declaration -- this should go in function.hpp
template <typename... Evaluators_> struct CompositionOf_e;

template <typename HeadEvaluator_, typename... EvaluatorBodies_>
struct CompositionOf_e<HeadEvaluator_,EvaluatorBodies_...>
{
private:
    typedef CompositionOf_e<EvaluatorBodies_...> BodyComposition;
public:
    template <typename T_>
    struct Eval_f
    {
    private:
        typedef typename BodyComposition::template Eval_f<T_>::T BodyEvaluation;
    public:
        typedef typename HeadEvaluator_::template Eval_f<BodyEvaluation>::T T;
    };
};

// composition of no maps produces the identity
template <>
struct CompositionOf_e<Typle_t<>>
{
    template <typename T_>
    struct Eval_f
    {
        typedef T_ T; // identity map
    };
};


// apply a metafunction to each element in a Typle_t

template <typename Typle_, typename FunctionEvaluator_>
struct OnEach_f
{
    static_assert(IsTyple_f<Typle_>::V, "template parameter Typle_ must be a Typle_t");
private:
    typedef typename FunctionEvaluator_::template Eval_f<typename Head_f<Typle_>::T>::T HeadEvaluation;
    typedef typename OnEach_f<typename BodyTyple_f<Typle_>::T,FunctionEvaluator_>::T BodyOnEach;
public:
    typedef typename HeadBodyTyple_f<HeadEvaluation,BodyOnEach>::T T;
};

template <typename FunctionEvaluator_>
struct OnEach_f<Typle_t<>,FunctionEvaluator_>
{
    typedef Typle_t<> T;
};

MAKE_2_ARY_TYPE_EVALUATOR(OnEach, typename, FunctionEvaluator_);

// ///////////////////////////////////////////////////////////////////////////
// logic
// ///////////////////////////////////////////////////////////////////////////

/// @struct NegationOfPredicate_e typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Is a predicate, which is the negation of the supplied predicate.
/// @tparam Predicate_e_ the predicate to negate -- must be an evaluator.
template <typename Predicate_e_>
struct NegationOfPredicate_e
{
    /// @cond false
    template <typename T_>
    struct Eval_f
    {
        typedef Value_t<bool,!Predicate_e_::template Eval_f<T_>::V> T;
    };
    /// @endcond
};

template <typename Typle_>
struct And_f
{
    static_assert(IsTyple_f<Typle_>::V, "template parameter Typle_ must be a Typle_t");
    static_assert(IsValue_f<typename Head_f<Typle_>::T>::V, "each type must be a Value_t");
    static bool const V = Head_f<Typle_>::T::V && And_f<typename BodyTyple_f<Typle_>::T>::V;
};

template <>
struct And_f<Typle_t<>>
{
    static bool const V = true; // vacuously true
};

MAKE_1_ARY_VALUE_EVALUATOR(And, bool);

template <typename Typle_>
struct Or_f
{
    static_assert(IsTyple_f<Typle_>::V, "template parameter Typle_ must be a Typle_t");
    static_assert(IsValue_f<typename Head_f<Typle_>::T>::V, "each type must be a Value_t");
    static bool const V = Head_f<Typle_>::T::V || Or_f<typename BodyTyple_f<Typle_>::T>::V;
};

template <>
struct Or_f<Typle_t<>>
{
    static bool const V = false; // base case ("at least one must be true")
};

MAKE_1_ARY_VALUE_EVALUATOR(Or, bool);

// ///////////////////////////////////////////////////////////////////////////
// convenience metafunction -- the composition of And_f and OnEach_f
// ///////////////////////////////////////////////////////////////////////////

/// @struct EachTypeSatisfies_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Returns true if every type in the given Typle_t satisfies the given predicate.
/// @tparam Typle_ the Typle_t to examine.
/// @tparam Predicate_e_ the predicate to examine with -- must be an evaluator.
template <typename Typle_, typename Predicate_e_>
struct EachTypeSatisfies_f
{
    static_assert(IsTyple_f<Typle_>::V, "template parameter Typle_ must be a Typle_t");
    static bool const V = And_f<typename OnEach_f<Typle_,Predicate_e_>::T>::V;
};

MAKE_2_ARY_VALUE_EVALUATOR(EachTypeSatisfies, bool, typename, Predicate_e_);

// ///////////////////////////////////////////////////////////////////////////
// math
// ///////////////////////////////////////////////////////////////////////////

template <typename Typle_, typename OperandType_>
struct Sum_f
{
    static_assert(EachTypeSatisfies_f<Typle_,IsValue_e>::V, "each type must be a Value_t type");
    static OperandType_ const V = Head_f<Typle_>::T::V + Sum_f<typename BodyTyple_f<Typle_>::T,OperandType_>::V;
};

template <typename OperandType_>
struct Sum_f<Typle_t<>,OperandType_>
{
    static OperandType_ const V = 0;
};

MAKE_2_ARY_VALUE_EVALUATOR(Sum, OperandType_, typename, OperandType_);

template <typename Typle_, typename OperandType_>
struct Product_f
{
    static_assert(EachTypeSatisfies_f<Typle_,IsValue_e>::V, "each type must be a Value_t type");
    static OperandType_ const V = Head_f<Typle_>::T::V * Product_f<typename BodyTyple_f<Typle_>::T,OperandType_>::V;
};

template <typename OperandType_>
struct Product_f<Typle_t<>,OperandType_>
{
    static OperandType_ const V = 1;
};

MAKE_2_ARY_VALUE_EVALUATOR(Product, OperandType_, typename, OperandType_);

template <typename Typle_, typename OperandType_>
struct Min_f
{
    static_assert(EachTypeSatisfies_f<Typle_,IsValue_e>::V, "each type must be a Value_t type");
private:
    static OperandType_ const HEAD_V = Head_f<Typle_>::T::V;
    static OperandType_ const BODY_V = Min_f<typename BodyTyple_f<Typle_>::T,OperandType_>::V;
public:
    static OperandType_ const V = (HEAD_V < BODY_V) ? HEAD_V : BODY_V;
};

template <typename OperandType_>
struct Min_f<Typle_t<>,OperandType_>
{
    static OperandType_ const V = NumericBound_t<OperandType_>::MAX;
};

MAKE_2_ARY_VALUE_EVALUATOR(Min, OperandType_, typename, OperandType_);

template <typename Typle_, typename OperandType_>
struct Max_f
{
    static_assert(EachTypeSatisfies_f<Typle_,IsValue_e>::V, "each type must be a Value_t type");
private:
    static OperandType_ const HEAD_V = Head_f<Typle_>::T::V;
    static OperandType_ const BODY_V = Max_f<typename BodyTyple_f<Typle_>::T,OperandType_>::V;
public:
    static OperandType_ const V = (HEAD_V > BODY_V) ? HEAD_V : BODY_V;
};

template <typename OperandType_>
struct Max_f<Typle_t<>,OperandType_>
{
    static OperandType_ const V = NumericBound_t<OperandType_>::MIN;
};

MAKE_2_ARY_VALUE_EVALUATOR(Max, OperandType_, typename, OperandType_);

/// @struct Element_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Returns the INDEX_th element of the given Typle_.
/// @tparam Typle_ the Typle_t to access elements from.
/// @tparam INDEX_ the index of the element to access.
template <typename Typle_, Uint32 INDEX_>
struct Element_f
{
    static_assert(!TypesAreEqual_f<Typle_,Typle_t<>>::V, "Typle_t<> has no elements to access");
};

/// @cond false
template <Uint32 INDEX_, typename Head_, typename... BodyTypes_>
struct Element_f<Typle_t<Head_,BodyTypes_...>,INDEX_>
{
    static_assert(INDEX_ < Length_f<Typle_t<Head_,BodyTypes_...>>::V, "INDEX_ must be less than the length of the Typle_t");
    typedef typename Element_f<Typle_t<BodyTypes_...>,INDEX_-1>::T T;
};

template <typename Head_, typename... BodyTypes_>
struct Element_f<Typle_t<Head_,BodyTypes_...>,0>
{
    typedef Head_ T;
};

// TODO: figure out how to Doxygen-comment this, if at all.
MAKE_2_ARY_TYPE_EVALUATOR(Element, Uint32, INDEX_);

template <typename Typle_, typename Element_>
struct Contains_f
{
    static bool const V = Or_f<typename OnEach_f<Typle_,TypesAreEqual_e<Element_>>::T>::V;
};

MAKE_2_ARY_VALUE_EVALUATOR(Contains, bool, typename, Element_);

/// @struct IndexOfFirstOccurrence_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Returns the index of the first occurrence of Type_ in Typle_, or
/// the length of the typle if Type_ does not occur in Typle_.
/// @tparam Typle_ the Typle_t in which to check the presence of Type_.
/// @tparam Type_ the type to search for.
template <typename Typle_, typename Type_>
struct IndexOfFirstOccurrence_f
{
    /// The return value of the metafunction.
    static Uint32 const V = TypesAreEqual_f<typename Head_f<Typle_>::T,Type_>::V ?
                            0 :
                            1 + IndexOfFirstOccurrence_f<typename BodyTyple_f<Typle_>::T,Type_>::V;
};

/// @cond false
template <typename Type_>
struct IndexOfFirstOccurrence_f<Typle_t<>,Type_>
{
    // this makes the index of a type that doesn't occur equal to the length of the typle.
    static Uint32 const V = 0;
};
/// @endcond

MAKE_2_ARY_VALUE_EVALUATOR(IndexOfFirstOccurrence, Uint32, typename, Type_);

/// @struct TrailingTyple_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Returns the Typle_t which is the range [START_INDEX_, Typle_::LENGTH) (right
/// endpoint not included) of Typle_.
/// @tparam Typle_ the Typle_t from which to extract the range.
/// @tparam START_INDEX_ the starting index of the range to extract.
template <typename Typle_, Uint32 START_INDEX_>
struct TrailingTyple_f
{
private:
    static Uint32 const I = (START_INDEX_ > 0) ? START_INDEX_-1 : START_INDEX_;
public:
    typedef typename If_f<START_INDEX_ == 0,
                          Typle_,
                          typename TrailingTyple_f<typename BodyTyple_f<Typle_>::T,I>::T>::T T;
};

/// @cond false
template <Uint32 START_INDEX_>
struct TrailingTyple_f<Typle_t<>,START_INDEX_>
{
    static_assert(START_INDEX_ == 0, "the only allowable trailing typle on an empty typle is the 0th");
    typedef Typle_t<> T;
};
/// @endcond

MAKE_2_ARY_TYPE_EVALUATOR(TrailingTyple, Uint32, START_INDEX_);

/// @struct LeadingTyple_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Returns the Typle_t which is the range [0, END_INDEX_) (right
/// endpoint not included) of Typle_.
/// @tparam Typle_ the Typle_t from which to extract the range.
/// @tparam END_INDEX_ the ending index of the range to extract (non-inclusive)
template <typename Typle_, Uint32 END_INDEX_>
struct LeadingTyple_f
{
private:
    static Uint32 const E = (END_INDEX_ == 0) ? 0 : END_INDEX_-1;
public:
    typedef typename If_f<END_INDEX_ == 0,
                          Typle_t<>,
                          typename HeadBodyTyple_f<typename Head_f<Typle_>::T,
                                                   typename LeadingTyple_f<typename BodyTyple_f<Typle_>::T,E>::T>::T>::T T;
};

/// @cond false
template <Uint32 END_INDEX_>
struct LeadingTyple_f<Typle_t<>,END_INDEX_>
{
    static_assert(END_INDEX_ == 0, "the only allowable leading typle on an empty typle is the 0th");
    typedef Typle_t<> T;
};
/// @endcond

MAKE_2_ARY_TYPE_EVALUATOR(LeadingTyple, Uint32, END_INDEX_);

/// @struct TypleRange_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Returns the Typle_t which is the range [START_INDEX_, END_INDEX_) (right
/// endpoint not included) of Typle_.
/// @tparam Typle_ the Typle_t from which to extract the range.
/// @tparam START_INDEX_ the starting index of the range to extract.
/// @tparam END_INDEX_ the ending index of the range to extract (non-inclusive)
template <typename Typle_, Uint32 START_INDEX_, Uint32 END_INDEX_>
struct TypleRange_f
{
    static_assert(START_INDEX_ <= END_INDEX_, "invalid range indices");
private:
    static Uint32 const RANGE_LENGTH = END_INDEX_ - START_INDEX_;
public:
    typedef typename LeadingTyple_f<typename TrailingTyple_f<Typle_,START_INDEX_>::T,RANGE_LENGTH>::T T;
};

MAKE_3_ARY_TYPE_EVALUATOR(TypleRange, Uint32, START_INDEX_, Uint32, END_INDEX_);

/// @struct UniqueTypesIn_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Generates a Typle_t containing the types of Typle_, but without repetitions.
/// @tparam Typle_ the Typle_t to condense down to unique types.
/// @tparam UsedTyple_ a typle of the types used in previous steps of the recursion.
template <typename Typle_, typename UsedTyple_ = Typle_t<>>
struct UniqueTypesIn_f
{
    static_assert(IsTyple_f<Typle_>::V, "Typle_ must be a Typle_t");
private:
    typedef typename Head_f<Typle_>::T Head;
    typedef typename BodyTyple_f<Typle_>::T BodyTyple;
    typedef typename HeadBodyTyple_f<Head,UsedTyple_>::T NextUsedTyple;
    typedef typename UniqueTypesIn_f<BodyTyple,NextUsedTyple>::T RemainingUniqueTyple;
public:
    typedef typename If_f<(Contains_f<UsedTyple_,Head>::V),
                          RemainingUniqueTyple,
                          typename HeadBodyTyple_f<Head,RemainingUniqueTyple>::T>::T T;
};

/// @cond false
template <typename UsedTyple_>
struct UniqueTypesIn_f<Typle_t<>,UsedTyple_>
{
    typedef Typle_t<> T;
};
/// @endcond

MAKE_1_ARY_TYPE_EVALUATOR(UniqueTypesIn);

/// @struct OccurrenceCount_t typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Counts how often a type occurres in a Typle_t.
/// @tparam Typle_ the Typle_t to check.
/// @tparam Type_ the type to check for.
template <typename Typle_, typename Type_>
struct OccurrenceCount_f
{
    static Uint32 const V =
        (TypesAreEqual_f<typename Head_f<Typle_>::T,Type_>::V ? 1 : 0)
        + OccurrenceCount_f<typename BodyTyple_f<Typle_>::T,Type_>::V;
};

/// @cond false
template <typename Type_>
struct OccurrenceCount_f<Typle_t<>,Type_>
{
    static Uint32 const V = 0;
};
/// @endcond

MAKE_2_ARY_VALUE_EVALUATOR(OccurrenceCount, Uint32, typename, Type_);

namespace NonPublic {

/// @cond false

/// @struct ElementsOfTypleHavingMultiplicity_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Finds all of the elements of UniqueTyple_ which occur in Typle_ exactly MULTIPLICITY_ times.
/// @tparam Typle_ the Typle_t to search within.
/// @tparam UniqueTyple_ the Typle_t of types to search for.
/// @tparam MULTIPLICITY_ the number of times the types must occur to be returned.
template <typename Typle_, typename UniqueTyple_, Uint32 MULTIPLICITY_>
struct ElementsOfTypleHavingMultiplicity_f
{
    static_assert(MULTIPLICITY_ > 0, "it doesn't make sense to test for multiplicity 0");
private:
    typedef typename ElementsOfTypleHavingMultiplicity_f<Typle_,typename BodyTyple_f<UniqueTyple_>::T,MULTIPLICITY_>::T InBody;
public:
    typedef typename If_f<(OccurrenceCount_f<Typle_,typename Head_f<UniqueTyple_>::T>::V == MULTIPLICITY_),
                          typename HeadBodyTyple_f<typename Head_f<UniqueTyple_>::T,InBody>::T,
                          InBody>::T T;
};

template <typename Typle_, Uint32 MULTIPLICITY_>
struct ElementsOfTypleHavingMultiplicity_f<Typle_,Typle_t<>,MULTIPLICITY_>
{
    static_assert(MULTIPLICITY_ > 0, "it doesn't make sense to test for multiplicity 0");
    typedef Typle_t<> T;
};
/// @endcond

} // end of namespace NonPublic

/// @struct ElementsHavingMultiplicity_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Returns a Typle_t of the elements of Typle_ which occur exactly MULTIPLICITY_ times.
/// @tparam Typle_ the Typle_t to search within.
/// @tparam MULTIPLICITY_ the number of times the types must occur to be returned.
template <typename Typle_, Uint32 MULTIPLICITY_>
struct ElementsHavingMultiplicity_f
{
private:
    typedef typename UniqueTypesIn_f<Typle_>::T UniqueTyple;
public:
    typedef typename NonPublic::ElementsOfTypleHavingMultiplicity_f<Typle_,UniqueTyple,MULTIPLICITY_>::T T;
};

MAKE_2_ARY_TYPE_EVALUATOR(ElementsHavingMultiplicity, Uint32, MULTIPLICITY_);

/// @struct ElementsOfTypleSatisfying_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Returns a Typle_t of all of the elements of Typle_ which satisfy a given predicate.
/// @details Predicate_e_ must be an evaluator.
/// @tparam Typle_ the Typle_t to search within.
/// @tparam Predicate_e_ the predicate the types must satisfy to be returned.
template <typename Typle_, typename Predicate_e_>
struct ElementsOfTypleSatisfying_f
{
private:
    typedef typename ElementsOfTypleSatisfying_f<typename BodyTyple_f<Typle_>::T,Predicate_e_>::T ElementsInBodyTypleSatisfying;
public:
    typedef typename If_f<Predicate_e_::template Eval_f<typename Head_f<Typle_>::T>::V,
                          typename HeadBodyTyple_f<typename Head_f<Typle_>::T,ElementsInBodyTypleSatisfying>::T,
                          ElementsInBodyTypleSatisfying>::T T;
};

/// @cond false
template <typename Predicate_e_>
struct ElementsOfTypleSatisfying_f<Typle_t<>,Predicate_e_>
{
    typedef Typle_t<> T;
};
/// @endcond

MAKE_2_ARY_TYPE_EVALUATOR(ElementsOfTypleSatisfying, typename, Predicate_e_);

/// @struct ContainsDuplicates_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Determines if a Typle_t contains any duplicate types.
/// @tparam Typle_ the Typle_t to examine for duplicates.
template <typename Typle_>
struct ContainsDuplicates_f
{
    static bool const V = Length_f<typename UniqueTypesIn_f<Typle_>::T>::V < Length_f<Typle_>::V;
};

MAKE_1_ARY_VALUE_EVALUATOR(ContainsDuplicates, bool);

/// @struct IsASubsetOf_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Determines if all of the types in the first Typle_t are contained in the second,
/// with no regard given to order.
/// @tparam TypleA_ the potential subset.
/// @tparam TypleB_ the potential superset.
template <typename TypleA_, typename TypleB_>
struct IsASubsetOf_f
{
    static bool const V = Contains_f<TypleB_,typename Head_f<TypleA_>::T>::V &&
                          IsASubsetOf_f<typename BodyTyple_f<TypleA_>::T,TypleB_>::V;
};

/// @cond false
// template <typename TypleAHead_, typename TypleB_>
// struct IsASubsetOf_f<Typle_t<TypleAHead_>,TypleB_>
// {
//     static bool const V = Contains_f<TypleB_,TypleAHead_>::V;
// };

template <typename TypleB_>
struct IsASubsetOf_f<Typle_t<>,TypleB_>
{
    static bool const V = true;
};
/// @endcond

MAKE_2_ARY_VALUE_EVALUATOR(IsASubsetOf, bool, typename, TypleB_);

/// @struct AreEqualAsSets_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Determines if the typles contain the same types (set equality, ignoring repeated elements).
/// @tparam TypleA_,TypleB_ the Typle_t to check for equality.
template <typename TypleA_, typename TypleB_>
struct AreEqualAsSets_f
{
    static bool const V = IsASubsetOf_f<TypleA_,TypleB_>::V && IsASubsetOf_f<TypleB_,TypleA_>::V;
};

MAKE_2_ARY_VALUE_EVALUATOR(AreEqualAsSets, bool, typename, TypleB_);

/// @struct HasNontrivialIntersectionAsSets_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Checks to sett if there is even a single type that is common to both Typle_t.
/// @tparam TypleA_,TypleB_ the Typle_t to search for a common element between.
template <typename TypleA_, typename TypleB_>
struct HasNontrivialIntersectionAsSets_f
{
    static bool const V = Contains_f<TypleB_,typename Head_f<TypleA_>::T>::V ||
                          HasNontrivialIntersectionAsSets_f<typename BodyTyple_f<TypleA_>::T,TypleB_>::V;
};

/// @cond false
template <typename TypleB_>
struct HasNontrivialIntersectionAsSets_f<Typle_t<>,TypleB_>
{
    static bool const V = false;
};
/// @endcond

MAKE_2_ARY_VALUE_EVALUATOR(HasNontrivialIntersectionAsSets, bool, typename, TypleB_);

/// @struct IntersectionAsSets_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Generates a Typle_t containing all of the types common to both Typle_t supplied.
/// @tparam TypleA_,TypleB_ the Typle_t to search for common elements.
template <typename TypleA_, typename TypleB_>
struct IntersectionAsSets_f
{
private:
    typedef typename IntersectionAsSets_f<typename BodyTyple_f<TypleA_>::T,TypleB_>::T RecursionTyple;
public:
    typedef typename If_f<Contains_f<TypleB_,typename Head_f<TypleA_>::T>::V,
                          typename HeadBodyTyple_f<typename Head_f<TypleA_>::T,RecursionTyple>::T,
                          RecursionTyple>::T T;
};

/// @cond false
template <typename TypleB_>
struct IntersectionAsSets_f<Typle_t<>,TypleB_>
{
    typedef Typle_t<> T;
};
/// @endcond

MAKE_2_ARY_TYPE_EVALUATOR(IntersectionAsSets, typename, TypleB_);

// renders the subtraction of TypleB_ from TypleA_, as sets
/// @struct SetSubtraction_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Finds all of the elements of one Typle_t which do not occur in a second Typle_t
/// @tparam TypleA_ the elements to potentially return.
/// @tparam TypleB_ the elements to not return.
template <typename TypleA_, typename TypleB_>
struct SetSubtraction_f
{
private:
    typedef typename If_f<Contains_f<TypleB_,typename Head_f<TypleA_>::T>::V,
                          Typle_t<>,
                          Typle_t<typename Head_f<TypleA_>::T> >::T HeadTyple;
public:
    typedef typename Concat2Typles_f<HeadTyple,typename SetSubtraction_f<typename BodyTyple_f<TypleA_>::T,TypleB_>::T>::T T;
};

/// @cond false
template <typename TypleB_>
struct SetSubtraction_f<Typle_t<>,TypleB_>
{
    typedef Typle_t<> T;
};
/// @endcond

MAKE_2_ARY_TYPE_EVALUATOR(SetSubtraction, typename, TypleB_);

/// @struct EachTypleHasEqualLength_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Checks that all supplied Typle_t have the same length.
/// @tparam Typles_ a Typle_t of Typle_t to check for common length.
template <typename Typles_>
struct EachTypleHasEqualLength_f
{
    static_assert(IsTyple_f<Typles_>::V, "template parameter Typles_ must be a Typle_t");
    static_assert(And_f<typename OnEach_f<Typles_,IsTyple_e>::T>::V, "each element in Typles_ must be a Typle_t");
private:
    typedef typename BodyTyple_f<Typles_>::T BodyTyple;
    typedef typename Head_f<Typles_>::T First;
    typedef typename Head_f<BodyTyple>::T Second;
public:
    static bool const V = (Length_f<First>::V == Length_f<Second>::V) && EachTypleHasEqualLength_f<BodyTyple>::V;
};

/// @cond false
template <typename Head_>
struct EachTypleHasEqualLength_f<Typle_t<Head_> >
{
    static bool const V = true; // trivially true
};

template <>
struct EachTypleHasEqualLength_f<Typle_t<>>
{
    static bool const V = true; // vacuously true
};
/// @endcond

MAKE_1_ARY_VALUE_EVALUATOR(EachTypleHasEqualLength, bool);

/// @struct Zip_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief "Zips" a list of Typle_t into a Typle_t of tuples.
/// The heads of all the Typle_t are put into the head element, etc.
/// e.g. (('a','b','c'),(1,2,3)) turns into (('a',1),('b',2),('c',3)).
/// @details Essentially this is the matrix transpose of the matrix
/// whose rows are the typles in Typles_.
/// @tparam Typles_ a Typle_t of the Typle_t to zip.
template <typename Typles_>
struct Zip_f
{
    static_assert(EachTypleHasEqualLength_f<Typles_>::V, "each Typle_t element must have the same length");
private:
    typedef typename OnEach_f<Typles_,Head_e>::T HeadTypes;
    typedef typename OnEach_f<Typles_,BodyTyple_e>::T BodyTyples;
public:
    typedef typename HeadBodyTyple_f<HeadTypes,typename Zip_f<BodyTyples>::T>::T T;
};

/// @cond false
template <typename... RestOfTyples_>
struct Zip_f<Typle_t<Typle_t<>,RestOfTyples_...> >
{
private:
    typedef Typle_t<Typle_t<>,RestOfTyples_...> Typles;
    static_assert(EachTypleHasEqualLength_f<Typles>::V, "each Typle_t element must have the same length");
public:
    typedef Typle_t<> T;
};

template <>
struct Zip_f<Typle_t<>>
{
    typedef Typle_t<> T;
};
/// @endcond

MAKE_1_ARY_TYPE_EVALUATOR(Zip);

/// @struct Unzip_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief The inverse to Zip_f -- e.g. (('a',1),('b',2),('c',3)) turns into (('a','b','c'),(1,2,3)).
/// @note Nearly trivial, as the zip operation is its own inverse.
/// @tparam ZippedTyples_ the Typle_t to unzip.
template <typename ZippedTyples_>
struct Unzip_f
{
    typedef typename Zip_f<ZippedTyples_>::T T; // Zip_f is its own inverse
};

MAKE_1_ARY_TYPE_EVALUATOR(Unzip);

/// @struct UniformTypleOfLength_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Generates a Typle_t with a specified length, containing only one type.
/// @tparam LENGTH_ the length to have.
/// @tparam Type_ the type to contain.
template <Uint32 LENGTH_, typename Type_>
struct UniformTypleOfLength_f
{
    typedef typename HeadBodyTyple_f<Type_,typename UniformTypleOfLength_f<LENGTH_-1,Type_>::T>::T T;
private:
    UniformTypleOfLength_f ();
};

/// @cond false
template <typename Type_>
struct UniformTypleOfLength_f<0,Type_>
{
    typedef Typle_t<> T;
private:
    UniformTypleOfLength_f ();
};
/// @endcond

// NOTE: technically can't make an evaluator for UniformTypleOfLength_f, because its first
// parameter is not a type, but a Uint32.  to add this capability would require changing
// how evaluators are declared somewhat.

/// @struct TypleIsUniform_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Returns true if (and only if) every element of the given Typle_t is the same.
/// @tparam Typle_ the Typle_t to check for uniformity
template <typename Typle_> struct TypleIsUniform_f;

/// @cond false
template <typename Head_, typename... BodyTypes_>
struct TypleIsUniform_f<Typle_t<Head_,BodyTypes_...> >
{
private:
    typedef Typle_t<BodyTypes_...> BodyTyple;
    TypleIsUniform_f ();
public:
    static bool const V = TypesAreEqual_f<Head_,typename Head_f<BodyTyple>::T>::V &&
                          TypleIsUniform_f<BodyTyple>::V;
};

template <typename Head_>
struct TypleIsUniform_f<Typle_t<Head_> >
{
    static bool const V = true;
};

template <>
struct TypleIsUniform_f<Typle_t<>>
{
    static bool const V = true;
};
/// @endcond

MAKE_1_ARY_VALUE_EVALUATOR(TypleIsUniform, bool);

/// @struct TypeOfUniformTyple_f typle_utility.hpp "tenh/meta/typle_utility.hpp"
/// @brief Returns the unique type of a uniform typle, assuming it is uniform.  This is
/// not defined for Typle_t<> (the empty typle).
/// @tparam Typle_ the Typle_t whose uniform type to return.
template <typename Typle_> struct TypeOfUniformTyple_f;

/// @cond false
template <typename Head_, typename... BodyTypes_>
struct TypeOfUniformTyple_f<Typle_t<Head_,BodyTypes_...> >
{
private:
    typedef Typle_t<Head_,BodyTypes_...> Typle;
    static_assert(TypleIsUniform_f<Typle>::V, "parameter typle must be uniform");
    TypeOfUniformTyple_f ();
public:
    typedef Head_ T;
};
/// @endcond

} // end of namespace Hippo

} // end of namespace Tenh

#endif // TENH_META_TYPLE_UTILITY_HPP_
