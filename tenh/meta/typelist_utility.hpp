// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typelist_utility.hpp by Victor Dods, created 2013/03/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////
/// @file typelist_utility.hpp
/// @headerfile typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Contains utility functions for working with Typelist_t types.
#ifndef TENH_META_TYPELIST_UTILITY_HPP_
#define TENH_META_TYPELIST_UTILITY_HPP_

#include "tenh/core.hpp"
#include "tenh/meta/typelist.hpp"

namespace Tenh {

/// @brief Check that each type in from can be converted to the corresponding type in to.
/// @details This requires that each type in FromTypeList has a default constructor.
///  If the constructors for each of the elements in each type list have no
///  side effects, then ostensibly this code will be optimized out.
/// @param from the TypeList_t to do type conversions from.
/// @param to the TypeList_t to do conversions to.
template <typename FromHeadType, typename FromBodyTypeList,
          typename ToHeadType, typename ToBodyTypeList>
inline void compile_time_check_that_there_is_a_type_conversion (TypeList_t<FromHeadType,FromBodyTypeList> const &from, TypeList_t<ToHeadType,ToBodyTypeList> const &to)
{
    FromHeadType from_head;
    ToHeadType head_conversion_checker(from_head);
    // tail recursion
    compile_time_check_that_there_is_a_type_conversion(FromBodyTypeList(), ToBodyTypeList());
}

/// @cond false
template <typename FromHeadType,
          typename ToHeadType>
inline void compile_time_check_that_there_is_a_type_conversion (TypeList_t<FromHeadType> const &from, TypeList_t<ToHeadType> const &to)
{
    // base case
    FromHeadType from_head;
    ToHeadType head_conversion_checker(from_head);
}

inline void compile_time_check_that_there_is_a_type_conversion (EmptyTypeList const &, EmptyTypeList const &)
{
    // nothing needs to be done, there are no types to check
}
/// @endcond


// ///////////////////////////////////////////////////////////////////////////
// apply a metafunction to each element in a TypeList_t
// ///////////////////////////////////////////////////////////////////////////

template <typename TypeList_, typename FunctionEvaluator_>
struct OnEach_f
{
    typedef TypeList_t<typename FunctionEvaluator_::template Eval_f<typename TypeList_::HeadType>::T,
                       typename OnEach_f<typename TypeList_::BodyTypeList,FunctionEvaluator_>::T> T;
};

template <typename FunctionEvaluator_>
struct OnEach_f<EmptyTypeList,FunctionEvaluator_>
{
    typedef EmptyTypeList T;
};

// ///////////////////////////////////////////////////////////////////////////
// logic
// ///////////////////////////////////////////////////////////////////////////

/// @struct NegationOfPredicate_e typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Is a predicate, which is the negation of the supplied predicate.
/// @tparam Predicate_ the predicate to negate.
template <typename Predicate_>
struct NegationOfPredicate_e
{
    /// @cond false
    template <typename T_>
    struct Eval_f
    {
        typedef Value_t<bool,!Predicate_::template Eval_f<T_>::T::V> T;
    };
    /// @endcond
};

template <typename TypeList_>
struct And_f
{
private:
    // TODO: assert that each type is a Value_t<bool,...>
public:
    static bool const V = TypeList_::HeadType::V && And_f<typename TypeList_::BodyTypeList>::V;
};

template <>
struct And_f<EmptyTypeList>
{
    static bool const V = true; // vacuously true
};

template <typename TypeList_>
struct Or_f
{
private:
    // TODO: assert that each type is a Value_t<bool,...>
public:
    static bool const V = TypeList_::HeadType::V || Or_f<typename TypeList_::BodyTypeList>::V;
};

template <>
struct Or_f<EmptyTypeList>
{
    static bool const V = false; // base case ("at least one must be true")
};

// ///////////////////////////////////////////////////////////////////////////
// math
// ///////////////////////////////////////////////////////////////////////////

template <typename TypeList_, typename OperandType_>
struct Sum_f
{
private:
    // TODO: assert that each type is a Value_t<OperandType_,...>
public:
    static OperandType_ const V = TypeList_::HeadType::V + Sum_f<typename TypeList_::BodyTypeList,OperandType_>::V;
};

template <typename OperandType_>
struct Sum_f<EmptyTypeList,OperandType_>
{
    static OperandType_ const V = 0;
};

template <typename TypeList_, typename OperandType_>
struct Product_f
{
private:
    // TODO: assert that each type is a Value_t<OperandType_,...>
public:
    static OperandType_ const V = TypeList_::HeadType::V * Product_f<typename TypeList_::BodyTypeList,OperandType_>::V;
};

template <typename OperandType_>
struct Product_f<EmptyTypeList,OperandType_>
{
    static OperandType_ const V = 1;
};

// note that Min_f is not defined on EmptyTypeList
template <typename TypeList_, typename OperandType_> struct Min_f;

template <typename HeadType_, typename NextHeadType_, typename NextBodyTypeList_, typename OperandType_>
struct Min_f<TypeList_t<HeadType_,TypeList_t<NextHeadType_,NextBodyTypeList_> >,OperandType_>
{
private:
    // TODO: assert that each type is a Value_t<OperandType_,...>
    typedef TypeList_t<NextHeadType_,NextBodyTypeList_> BodyTypeList;
    static OperandType_ const HEAD_V = HeadType_::V;
    static OperandType_ const BODY_V = Min_f<BodyTypeList,OperandType_>::V;
public:
    static OperandType_ const V = (HEAD_V < BODY_V) ? HEAD_V : BODY_V;
};

template <typename HeadType_, typename OperandType_>
struct Min_f<TypeList_t<HeadType_>,OperandType_>
{
private:
    // TODO: assert that each type is a Value_t<OperandType_,...>
public:
    static OperandType_ const V = HeadType_::V;
};

// note that Max_f is not defined on EmptyTypeList
template <typename TypeList_, typename OperandType_> struct Max_f;

template <typename HeadType_, typename NextHeadType_, typename NextBodyTypeList_, typename OperandType_>
struct Max_f<TypeList_t<HeadType_,TypeList_t<NextHeadType_,NextBodyTypeList_> >,OperandType_>
{
private:
    // TODO: assert that each type is a Value_t<OperandType_,...>
    typedef TypeList_t<NextHeadType_,NextBodyTypeList_> BodyTypeList;
    static OperandType_ const HEAD_V = HeadType_::V;
    static OperandType_ const BODY_V = Max_f<BodyTypeList,OperandType_>::V;
public:
    static OperandType_ const V = (HEAD_V > BODY_V) ? HEAD_V : BODY_V;
};

template <typename HeadType_, typename OperandType_>
struct Max_f<TypeList_t<HeadType_>,OperandType_>
{
private:
    // TODO: assert that each type is a Value_t<OperandType_,...>
public:
    static OperandType_ const V = HeadType_::V;
};

// ///////////////////////////////////////////////////////////////////////////
// TypeList_t specific stuff
// ///////////////////////////////////////////////////////////////////////////

template <typename TypeList_>
struct Head_f
{
    typedef typename TypeList_::HeadType T;
};

// base case
template <>
struct Head_f<EmptyTypeList>
{
    // undefined on purpose
};

MAKE_1_ARY_TYPE_EVALUATOR(Head);

template <typename TypeList_>
struct Body_f
{
    typedef typename TypeList_::BodyTypeList T;
};

// base case
template <>
struct Body_f<EmptyTypeList>
{
    // undefined on purpose
};

MAKE_1_ARY_TYPE_EVALUATOR(Body);

template <typename TypeList_>
struct Length_f
{
    static Uint32 const V = TypeList_::LENGTH;
};

MAKE_1_ARY_VALUE_EVALUATOR(Length, Uint32);

/// @struct Element_f typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns the INDEX_th element of the given TypeList_.
/// @tparam TypeList_ the TypeList_t to access elements from.
/// @tparam INDEX_ the index of the element to access.
template <typename TypeList_, Uint32 INDEX_>
struct Element_f
{
private:
    static Uint32 const I = (INDEX_ == 0) ? 0 : INDEX_-1;
public:
    /// The INDEX_th element of TypeList_.
    typedef typename If_f<INDEX_ == 0,
                          typename TypeList_::HeadType,
                          typename Element_f<typename TypeList_::BodyTypeList,I>::T>::T T;
};

/// @cond false
template <Uint32 INDEX_>
struct Element_f<EmptyTypeList,INDEX_>
{
    // seems to be necessary
    typedef NullType T;
};
/// @endcond

// TODO: figure out how to Doxygen-comment this, if at all.
MAKE_2_ARY_TYPE_EVALUATOR(Element, Uint32, INDEX_);

// TODO: could be implemented as Or_f<OnEach_f<TypeList_,TypesAreEqual_e>::T>::V
template <typename TypeList_, typename Type_>
struct Contains_f
{
    static bool const V = TypesAreEqual_f<typename TypeList_::HeadType,Type_>::V ||
                          Contains_f<typename TypeList_::BodyTypeList,Type_>::V;
};

// base case
template <typename Type_>
struct Contains_f<EmptyTypeList,Type_>
{
    static bool const V = false;
};

MAKE_2_ARY_VALUE_EVALUATOR(Contains, bool, typename, Type_);

/// @struct IndexOfFirstOccurrence_f typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns the index of the first occurrence of Type_ in the TypeList_, or
/// TypeList_::LENGTH if Type_ does not occur in TypeList_.
/// @tparam TypeList_ the TypeList_t in which to check the presence of Type_.
/// @tparam Type_ the type to search for.
template <typename TypeList_, typename Type_>
struct IndexOfFirstOccurrence_f
{
    /// The return value of the metafunction.
    static Uint32 const V = TypesAreEqual_f<typename TypeList_::HeadType,Type_>::V ?
                            0 :
                            1 + IndexOfFirstOccurrence_f<typename TypeList_::BodyTypeList,Type_>::V;
};

/// @cond false
template <typename Type_>
struct IndexOfFirstOccurrence_f<EmptyTypeList,Type_>
{
    // this makes the index of a type that doesn't occur equal to the length of the list.
    static Uint32 const V = 0;
};
/// @endcond

MAKE_2_ARY_VALUE_EVALUATOR(IndexOfFirstOccurrence, Uint32, typename, Type_);

/// @struct TrailingTypeList_f typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns the TypeList_t which is the range [START_INDEX_, TypeList_::LENGTH) (right
/// endpoint not included) of TypeList_.
/// @tparam TypeList_ the TypeList_t from which to extract the range.
/// @tparam START_INDEX_ the starting index of the range to extract.
template <typename TypeList_, Uint32 START_INDEX_>
struct TrailingTypeList_f
{
    /// The return value of the metafunction.
    typedef typename If_f<START_INDEX_ == 0,
                          TypeList_,
                          typename TrailingTypeList_f<typename TypeList_::BodyTypeList,START_INDEX_-1>::T>::T T;
};

/// @cond false
template <Uint32 START_INDEX_>
struct TrailingTypeList_f<EmptyTypeList,START_INDEX_>
{
    //enum { STATIC_ASSERT_IN_ENUM(START_INDEX_ == 0, INDEX_OUT_OF_RANGE) };
    typedef EmptyTypeList T;
};
/// @endcond

MAKE_2_ARY_TYPE_EVALUATOR(TrailingTypeList, Uint32, START_INDEX_);

/// @struct LeadingTypeList_f typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns the TypeList_t which is the range [0, END_INDEX_) (right
/// endpoint not included) of TypeList_.
/// @tparam TypeList_ the TypeList_t from which to extract the range.
/// @tparam END_INDEX_ the ending index of the range to extract (non-inclusive)
template <typename TypeList_, Uint32 END_INDEX_>
struct LeadingTypeList_f
{
private:
    static Uint32 const E = (END_INDEX_ == 0) ? 0 : END_INDEX_-1;
public:
    /// The return value of the metafunction.
    typedef typename If_f<END_INDEX_ == 0,
                          EmptyTypeList,
                          TypeList_t<typename TypeList_::HeadType,
                                     typename LeadingTypeList_f<typename TypeList_::BodyTypeList,E>::T> >::T T;
};

/// @cond false
template <Uint32 END_INDEX_>
struct LeadingTypeList_f<EmptyTypeList,END_INDEX_>
{
    //enum { STATIC_ASSERT_IN_ENUM(END_INDEX_ == 0, INDEX_OUT_OF_RANGE) };
    typedef EmptyTypeList T;
};
/// @endcond

MAKE_2_ARY_TYPE_EVALUATOR(LeadingTypeList, Uint32, END_INDEX_);

/// @struct TypeListRange_f typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns the TypeList_t which is the range [START_INDEX_, END_INDEX_) (right
/// endpoint not included) of TypeList_.
/// @tparam TypeList_ the TypeList_t from which to extract the range.
/// @tparam START_INDEX_ the starting index of the range to extract.
/// @tparam END_INDEX_ the ending index of the range to extract (non-inclusive)
template <typename TypeList_, Uint32 START_INDEX_, Uint32 END_INDEX_>
struct TypeListRange_f
{
private:
    enum { STATIC_ASSERT_IN_ENUM(START_INDEX_ <= END_INDEX_, INVALID_RANGE_INDICES) };
    static Uint32 const RANGE_LENGTH = END_INDEX_ - START_INDEX_;
public:
    /// The return value of the metafunction.
    typedef typename LeadingTypeList_f<typename TrailingTypeList_f<TypeList_,START_INDEX_>::T,RANGE_LENGTH>::T T;
};

MAKE_3_ARY_TYPE_EVALUATOR(TypeListRange, Uint32, START_INDEX_, Uint32, END_INDEX_);

// ///////////////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////////////


/// @struct TailOfTypeList_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns the last element of a TypeList_t.
/// @tparam TypeList_ the TypeList_t to return the tail of.
template <typename TypeList_>
struct TailOfTypeList_t
{
private:
    enum { STATIC_ASSERT_IN_ENUM((TypeList_::LENGTH > 0), LENGTH_MUST_BE_POSITIVE) };
public:
    /// The return value of the metafunction.
    typedef typename TailOfTypeList_t<typename TypeList_::BodyTypeList>::T T;
};

/// @cond false
template <typename HeadType_>
struct TailOfTypeList_t<TypeList_t<HeadType_> >
{
    typedef HeadType_ T;
};
/// @endcond


/// @struct AllButTailOfTypeList_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns a TypeList_t missing its tail (last element).
/// @tparam TypeList_ the TypeList_t to shorten.
template <typename TypeList_>
struct AllButTailOfTypeList_t
{
private:
    enum { STATIC_ASSERT_IN_ENUM((TypeList_::LENGTH > 0), LENGTH_MUST_BE_POSITIVE) };
public:
    /// The return value of the metafunction.
    typedef TypeList_t<typename TypeList_::HeadType,typename AllButTailOfTypeList_t<typename TypeList_::BodyTypeList>::T> T;
};

/// @cond false
template <typename HeadType_>
struct AllButTailOfTypeList_t<TypeList_t<HeadType_> >
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct ReversedTypeList_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns a reversed version of the given TypeList_t.
/// @tparam TypeList_ the TypeList_t to reverse.
template <typename TypeList_>
struct ReversedTypeList_t
{
    /// The return value of the metafunction.
    typedef TypeList_t<typename TailOfTypeList_t<TypeList_>::T,typename ReversedTypeList_t<typename AllButTailOfTypeList_t<TypeList_>::T>::T> T;
};

/// @cond false
template <>
struct ReversedTypeList_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct UniqueTypesIn_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Generates a TypeList_t containing the types of TypeList_, but without repetitions.
/// @tparam TypeList_ the TypeList_t to condense down to unique types.
/// @tparam UsedTypeList_ a list of the types used in previous steps of the recursion.
template <typename TypeList_, typename UsedTypeList_ = EmptyTypeList>
struct UniqueTypesIn_t
{
private:
    typedef typename TypeList_::HeadType HeadType;
    typedef typename TypeList_::BodyTypeList BodyTypeList;
    typedef TypeList_t<HeadType,UsedTypeList_> NextUsedTypeList;
    typedef typename UniqueTypesIn_t<BodyTypeList,NextUsedTypeList>::T RemainingUniqueTypeList;
public:
    /// The return value of the metafunction.
    typedef typename If_f<(Contains_f<UsedTypeList_,HeadType>::V),
                          RemainingUniqueTypeList,
                          TypeList_t<HeadType,RemainingUniqueTypeList> >::T T;
};

/// @cond false
template <typename UsedTypeList>
struct UniqueTypesIn_t<EmptyTypeList,UsedTypeList>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct Occurrence_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Counts how often a type occurres in a TypeList_t.
/// @tparam TypeList_ the TypeList_t to check.
/// @tparam Type_ the type to check for.
template <typename TypeList_, typename Type_>
struct Occurrence_t
{
    /// The return value of the metafunction.
    static Uint32 const COUNT =
        (TypesAreEqual_f<typename TypeList_::HeadType,Type_>::V ? 1 : 0)
        + Occurrence_t<typename TypeList_::BodyTypeList,Type_>::COUNT;
};

/// @cond false
template <typename Type_>
struct Occurrence_t<EmptyTypeList,Type_>
{
    static Uint32 const COUNT = 0;
};
/// @endcond


/// @struct ElementsOfTypeListHavingMultiplicity_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Finds all of the elements of UniqueTypeList_ which occur in TypeList_ exactly MULTIPLICITY_ times.
/// @tparam TypeList_ the TypeList_t to search within.
/// @tparam UniqueTypeList_ the TypeList_t of types to search for.
/// @tparam MULTIPLICITY_ the number of times the types must occur to be returned.
template <typename TypeList_, typename UniqueTypeList_, Uint32 MULTIPLICITY_>
struct ElementsOfTypeListHavingMultiplicity_t
{
private:
    typedef typename ElementsOfTypeListHavingMultiplicity_t<TypeList_,typename UniqueTypeList_::BodyTypeList,MULTIPLICITY_>::T InBody;
public:
    typedef typename If_f<(Occurrence_t<TypeList_,typename UniqueTypeList_::HeadType>::COUNT == MULTIPLICITY_),
                          TypeList_t<typename UniqueTypeList_::HeadType,InBody>,
                          InBody>::T T;
};

/// @cond false
template <typename TypeList_, Uint32 MULTIPLICITY_>
struct ElementsOfTypeListHavingMultiplicity_t<TypeList_,EmptyTypeList,MULTIPLICITY_>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct ElementsHavingMultiplicity_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns a TypeList_t of the elements of TypeList_ which occur exactly MULTIPLICITY_ times.
/// @tparam TypeList_ the TypeList_t to search within.
/// @tparam MULTIPLICITY_ the number of times the types must occur to be returned.
template <typename TypeList_, Uint32 MULTIPLICITY_>
struct ElementsHavingMultiplicity_t
{
private:
    typedef typename UniqueTypesIn_t<TypeList_>::T UniqueTypeList;
public:
    typedef typename ElementsOfTypeListHavingMultiplicity_t<TypeList_,UniqueTypeList,MULTIPLICITY_>::T T;
};


/// @struct ElementsOfTypeListSatisfyingPredicate_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns a TypeList_t of all of the elements of TypeList_ which satisfy a given predicate.
/// @details Predicate_ must be a struct having a template <> struct Eval_t { static bool const V; };
/// @tparam TypeList_ the TypeList_t to search within.
/// @tparam Predicate_ the predicate the types must satisfy to be returned.
template <typename TypeList_, typename Predicate_>
struct ElementsOfTypeListSatisfyingPredicate_t
{
private:
    typedef typename ElementsOfTypeListSatisfyingPredicate_t<typename TypeList_::BodyTypeList,Predicate_>::T ElementsInBodyTypeListSatisfyingPredicate;
public:
    typedef typename If_f<Predicate_::template Eval_t<typename TypeList_::HeadType>::V,
                          TypeList_t<typename TypeList_::HeadType,ElementsInBodyTypeListSatisfyingPredicate>,
                          ElementsInBodyTypeListSatisfyingPredicate>::T T;
};

/// @cond false
template <typename HeadType_, typename Predicate_>
struct ElementsOfTypeListSatisfyingPredicate_t<TypeList_t<HeadType_>,Predicate_>
{
    typedef typename If_f<Predicate_::template Eval_t<HeadType_>::V,
                          TypeList_t<HeadType_>,
                          EmptyTypeList>::T T;
};

template <typename Predicate_>
struct ElementsOfTypeListSatisfyingPredicate_t<EmptyTypeList,Predicate_>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct ContainsDuplicates_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Determines if a TypeList_t contains any duplicate types.
/// @tparam TypeList_ the TypeList_t to examine for duplicates.
template <typename TypeList_>
struct ContainsDuplicates_t
{
    static bool const V = UniqueTypesIn_t<TypeList_>::T::LENGTH < TypeList_::LENGTH;
};


/// @struct ConcatenationOfTypeLists_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Concatenates two TypeList_t.
/// @tparam FirstTypeList_,SecondTypeList_ the TypeList_t to concatenate.
template <typename FirstTypeList_, typename SecondTypeList_>
struct ConcatenationOfTypeLists_t
{
private:
    typedef typename FirstTypeList_::HeadType FirstTypeListHead;
    typedef typename FirstTypeList_::BodyTypeList FirstTypeListBody;
public:
    typedef TypeList_t<FirstTypeListHead,typename ConcatenationOfTypeLists_t<FirstTypeListBody,SecondTypeList_>::T> T;
};

/// @cond false
template <typename FirstTypeListHead, typename SecondTypeList>
struct ConcatenationOfTypeLists_t<TypeList_t<FirstTypeListHead>,SecondTypeList>
{
    typedef TypeList_t<FirstTypeListHead,SecondTypeList> T;
};

template <typename SecondTypeList>
struct ConcatenationOfTypeLists_t<EmptyTypeList,SecondTypeList>
{
    typedef SecondTypeList T;
};
/// @endcond



/// @struct IsASubsetOf_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Determines if all of the types in the first TypeList_t are contained in the second.
/// @tparam TypeListA_ the potential subset.
/// @tparam TypeListB_ the potential superset.
template <typename TypeListA_, typename TypeListB_>
struct IsASubsetOf_t
{
    static bool const V = Contains_f<TypeListB_,typename TypeListA_::HeadType>::V &&
                          IsASubsetOf_t<typename TypeListA_::BodyTypeList,TypeListB_>::V;
};

/// @cond false
template <typename TypeListAHead_, typename TypeListB_>
struct IsASubsetOf_t<TypeList_t<TypeListAHead_>,TypeListB_>
{
    static bool const V = Contains_f<TypeListB_,TypeListAHead_>::V;
};

template <typename TypeListB_>
struct IsASubsetOf_t<EmptyTypeList,TypeListB_>
{
    static bool const V = true;
};
/// @endcond


/// @struct AreEqualAsSets_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Determines if the type lists contain the same types (set equality, ignoring repeated elements).
/// @tparam TypeListA_,TypeListB_ the TypeLists_t to check for equality.
template <typename TypeListA_, typename TypeListB_>
struct AreEqualAsSets_t
{
    static bool const V = IsASubsetOf_t<TypeListA_,TypeListB_>::V && IsASubsetOf_t<TypeListB_,TypeListA_>::V;
};


/// @struct HasNontrivialIntersectionAsSets_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Checks to sett if there is even a single type that is common to both TypeList_t.
/// @tparam TypeListA_,TypeListB_ the TypeList_t to search for a common element between.
template <typename TypeListA_, typename TypeListB_>
struct HasNontrivialIntersectionAsSets_t
{
    static bool const V = Contains_f<TypeListB_,typename TypeListA_::HeadType>::V ||
                          HasNontrivialIntersectionAsSets_t<typename TypeListA_::BodyTypeList,TypeListB_>::V;
};

/// @cond false
template <typename TypeListB_>
struct HasNontrivialIntersectionAsSets_t<EmptyTypeList,TypeListB_>
{
    static bool const V = false;
};
/// @endcond


/// @struct IntersectionAsSets_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Generates a TypeList_t containing all of the types common to both TypeList_t supplied.
/// @tparam TypeListA_,TypeListB_ the TypeList_t to search for common elements.
template <typename TypeListA_, typename TypeListB_>
struct IntersectionAsSets_t
{
private:
    typedef typename IntersectionAsSets_t<typename TypeListA_::BodyTypeList,TypeListB_>::T RecursionTypeList;
public:
    typedef typename If_f<Contains_f<TypeListB_,typename TypeListA_::HeadType>::V,
                          TypeList_t<typename TypeListA_::HeadType,RecursionTypeList>,
                          RecursionTypeList>::T T;
};

/// @cond false
template <typename TypeListB>
struct IntersectionAsSets_t<EmptyTypeList,TypeListB>
{
    typedef EmptyTypeList T;
};
/// @endcond


// renders the subtraction of TypeListB from TypeListA, as sets
/// @struct SetSubtraction_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Finds all of the elements of one TypeList_t which do not occur in a second TypeList_t
/// @tparam TypeListA_ the elements to potentially return.
/// @tparam TypeListB_ the elements to not return.
template <typename TypeListA_, typename TypeListB_>
struct SetSubtraction_t
{
private:
    typedef typename If_f<Contains_f<TypeListB_,typename TypeListA_::HeadType>::V,
                          EmptyTypeList,
                          TypeList_t<typename TypeListA_::HeadType> >::T HeadTypeList;
public:
    typedef typename ConcatenationOfTypeLists_t<HeadTypeList,typename SetSubtraction_t<typename TypeListA_::BodyTypeList,TypeListB_>::T>::T T;
};

/// @cond false
template <typename TypeListB_>
struct SetSubtraction_t<EmptyTypeList,TypeListB_>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct EachTypeListHasEqualLength_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Checks that all supplied TypeList_t have the same length.
/// @tparam TypeLists_ a TypeList_t of TypeList_t to check for common length.
template <typename TypeLists_>
struct EachTypeListHasEqualLength_t
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsTypeList_f<TypeLists_>::V, MUST_BE_TYPELIST)
    };
    typedef typename TypeLists_::HeadType First;
    typedef typename TypeLists_::BodyTypeList::HeadType Second;
public:
    static bool const V = (First::LENGTH == Second::LENGTH) &&
                          EachTypeListHasEqualLength_t<typename TypeLists_::BodyTypeList>::V;
};

/// @cond false
template <typename HeadType_>
struct EachTypeListHasEqualLength_t<TypeList_t<HeadType_> >
{
    static bool const V = true; // trivially true
};

template <>
struct EachTypeListHasEqualLength_t<EmptyTypeList>
{
    static bool const V = true; // vacuously true
};
/// @endcond


/// @struct HeadTypeOfEach_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Makes a TypeList_t of the HeadType of each TypeList_t in the list.
/// @tparam TypeLists_ a TypeList_t of TypeList_t to pull the head types from.
template <typename TypeLists_>
struct HeadTypeOfEach_t
{
private:
    typedef typename TypeLists_::HeadType FirstTypeList;
    typedef typename TypeLists_::BodyTypeList RestOfTypeLists;
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<TypeLists_>::V, MUST_BE_TYPELIST, TYPELISTS),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<FirstTypeList>::V, MUST_BE_TYPELIST, FIRSTTYPELIST),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<RestOfTypeLists>::V, MUST_BE_TYPELIST, RESTOFTYPELISTS),
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists_>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
public:
    typedef TypeList_t<typename FirstTypeList::HeadType,typename HeadTypeOfEach_t<RestOfTypeLists>::T> T;
};

/// @cond false
template <typename RestOfTypeLists_>
struct HeadTypeOfEach_t<TypeList_t<EmptyTypeList,RestOfTypeLists_> >
{
private:
    typedef TypeList_t<EmptyTypeList,RestOfTypeLists_> TypeLists;
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<TypeLists>::V, MUST_BE_TYPELIST, TYPELISTS),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<RestOfTypeLists_>::V, MUST_BE_TYPELIST, RESTOFTYPELISTS),
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
public:
    typedef EmptyTypeList T;
};

template <>
struct HeadTypeOfEach_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct BodyTypeListOfEach_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Makes a TypeList_t of the BodyTypeList of each TypeList_t in the list
/// @tparam TypeLists_ a TypeList_t of TypeList_t to pull the body TypeList_t from.
template <typename TypeLists_>
struct BodyTypeListOfEach_t
{
private:
    typedef typename TypeLists_::HeadType FirstTypeList;
    typedef typename TypeLists_::BodyTypeList RestOfTypeLists;
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<TypeLists_>::V, MUST_BE_TYPELIST, TYPELISTS),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<FirstTypeList>::V, MUST_BE_TYPELIST, FIRSTTYPELIST),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<RestOfTypeLists>::V, MUST_BE_TYPELIST, RESTOFTYPELISTS),
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists_>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
public:
    typedef TypeList_t<typename FirstTypeList::BodyTypeList,typename BodyTypeListOfEach_t<RestOfTypeLists>::T> T;
};

/// @cond false
template <typename RestOfTypeLists_>
struct BodyTypeListOfEach_t<TypeList_t<EmptyTypeList,RestOfTypeLists_> >
{
private:
    typedef TypeList_t<EmptyTypeList,RestOfTypeLists_> TypeLists;
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<TypeLists>::V, MUST_BE_TYPELIST, TYPELISTS),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsTypeList_f<RestOfTypeLists_>::V, MUST_BE_TYPELIST, RESTOFTYPELISTS),
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
public:
    typedef EmptyTypeList T;
};

template <>
struct BodyTypeListOfEach_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct Zip_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief "Zips" a list of TypeList_t into a TypeList_t of tuples.
/// The heads of all the TypeList_t are put into the head element, etc.
/// e.g. (('a','b','c'),(1,2,3)) turns into (('a',1),('b',2),('c',3)).
/// @tparam TypeLists_ a TypeList_t of the TypeList_t to zip.
template <typename TypeLists_>
struct Zip_t
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists_>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
    typedef typename HeadTypeOfEach_t<TypeLists_>::T HeadTypes;
    typedef typename BodyTypeListOfEach_t<TypeLists_>::T BodyTypeLists;
public:
    typedef TypeList_t<HeadTypes,typename Zip_t<BodyTypeLists>::T> T;
};

/// @cond false
template <typename RestOfTypeLists_>
struct Zip_t<TypeList_t<EmptyTypeList,RestOfTypeLists_> >
{
private:
    typedef TypeList_t<EmptyTypeList,RestOfTypeLists_> TypeLists;
    enum
    {
        STATIC_ASSERT_IN_ENUM(EachTypeListHasEqualLength_t<TypeLists>::V, MUST_HAVE_EQUAL_LENGTHS)
    };
public:
    typedef EmptyTypeList T;
};

template <>
struct Zip_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct Unzip_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief The inverse to Zip_t -- e.g. (('a',1),('b',2),('c',3)) turns into (('a','b','c'),(1,2,3)).
/// @note Nearly trivial, as the zip operation is its own inverse.
/// @tparam ZippedTypeLists_ the TypeList_t to unzip.
template <typename ZippedTypeLists_>
struct Unzip_t
{
    typedef typename Zip_t<ZippedTypeLists_>::T T; // Zip_t is its own inverse
};


/// @struct UniformTypeListOfLength_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Generates a TypeList_t with a specified length, containing only one type.
/// @tparam Type_ the type to contain.
/// @tparam LENGTH_ the length to have.
template <typename Type_, Uint32 LENGTH_>
struct UniformTypeListOfLength_t
{
    typedef TypeList_t<Type_,typename UniformTypeListOfLength_t<Type_,LENGTH_-1>::T> T;
};

/// @cond false
template <typename Type_>
struct UniformTypeListOfLength_t<Type_,0>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct TypeListIsUniform_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns true if (and only if) every element of the given TypeList_t is the same.
/// @tparam TypeList_ the TypeList_t to check for uniformity
template <typename TypeList_> struct TypeListIsUniform_t;

/// @cond false
template <typename HeadType_, typename BodyTypeList_>
struct TypeListIsUniform_t<TypeList_t<HeadType_,BodyTypeList_> >
{
    static bool const V = TypesAreEqual_f<HeadType_,typename BodyTypeList_::HeadType>::V &&
                          TypeListIsUniform_t<BodyTypeList_>::V;
};

template <typename HeadType_>
struct TypeListIsUniform_t<TypeList_t<HeadType_> >
{
    static bool const V = true;
};

template <>
struct TypeListIsUniform_t<EmptyTypeList>
{
    static bool const V = true;
};
/// @endcond


/// @struct EachTypeSatisfies_f typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns true if every type in the given TypeList_t satisfies the given predicate.
/// @tparam TypeList_ the TypeList_t to examine.
/// @tparam Predicate_ the predicate to examine with.
template <typename TypeList_, typename Predicate_>
struct EachTypeSatisfies_f
{
private:
    enum { STATIC_ASSERT_IN_ENUM(IsTypeList_f<TypeList_>::V, MUST_BE_TYPELIST) };
public:
    static bool const V = Predicate_::template Eval_t<typename TypeList_::HeadType>::V && EachTypeSatisfies_f<typename TypeList_::BodyTypeList,Predicate_>::V;
};

/// @cond false
template <typename Predicate_>
struct EachTypeSatisfies_f<EmptyTypeList, Predicate_>
{
    static bool const V = true;
};
/// @endcond



} // end of namespace Tenh

#endif // TENH_META_TYPELIST_UTILITY_HPP_
