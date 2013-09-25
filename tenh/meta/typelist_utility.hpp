// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typelist_utility.hpp by Victor Dods, created 2013/03/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////
/// \file typelist_utility.hpp
/// \headerfile typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// \brief Contains utility functions for working with Typelist_t types.
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

inline void compile_time_check_that_there_is_a_type_conversion (EmptyTypeList const &from, EmptyTypeList const &to)
{
    // nothing needs to be done, there are no types to check
}
/// @endcond



/// @struct TailOfTypeList_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Returns the last element of a TypeList_t.
/// @tparam TypeList_ the TypeList_t to return the tail of.
template <typename TypeList_>
struct TailOfTypeList_t
{
private:
    enum { STATIC_ASSERT_IN_ENUM((TypeList_::LENGTH > 0), LENGTH_MUST_BE_POSITIVE) };

public:
    /// The tail type of TypeList_
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
    /// The reversed TypeList_t.
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
    typedef TypeList_t<HeadType_,UsedTypeList_> NextUsedTypeList;
    typedef typename UniqueTypesIn_t<BodyTypeList,NextUsedTypeList>::T RemainingUniqueTypeList;
public:
    /// A typelist containing the unique types in TypeList_.
    typedef typename If<(UsedTypeList::template Contains_t<HeadType>::V),
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
    /// How many occurrences of Type_ are there in TypeList_.
    static Uint32 const COUNT =
        (TypesAreEqual<typename TypeList_::HeadType,Type_>::V ? 1 : 0)
        + Occurrence_t<typename TypeList_::BodyTypeList,Type_>::COUNT;
};

/// @cond false
template <typename Type_>
struct Occurrence_t<EmptyTypeList,Type_>
{
    static Uint32 const COUNT = 0;
};
/// @endcond


/// @struct FirstMatchingIn_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Finds offset of the first occurrence of Type_ in TypeList_. Will static assert if there is no such occurrence.
/// @details INDEX is the smallest Uint32 such that TypeList_::El_t<INDEX>::T is Type_.
/// @tparam TypeList_ the typelist to search.
/// @tparam Type_ the type to search for.
template <typename TypeList_, typename Type_>
struct FirstMatchingIn_t
{
private:
    enum { STATIC_ASSERT_IN_ENUM((Occurrence_t<TypeList,Type>::COUNT > 0), TYPE_MUST_APPEAR_IN_TYPELIST) };
public:
    static Uint32 const INDEX = If<TypesAreEqual<typename TypeList_::HeadType,Type_>::V,
                                   FirstMatchingIn_t<TypeList_t<typename TypeList_::HeadType>,typename TypeList_::HeadType>,
                                   FirstMatchingIn_t<typename TypeList_::BodyTypeList,Type_> >::T::INDEX
                                +
                                (TypesAreEqual<typename TypeList_::HeadType,Type_>::V ? 0 : 1);
                                // this offset is what gets past HeadType if there is no match here
};

/// @cond false
template <typename HeadType_, typename Type_>
struct FirstMatchingIn_t<TypeList_t<HeadType_>,Type_>
{
private:
    enum { STATIC_ASSERT_IN_ENUM((TypesAreEqual<HeadType_,Type_>::V), TYPE_MUST_APPEAR_IN_TYPELIST) };
public:
    static Uint32 const INDEX = 0;
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
    typedef typename If<(Occurrence_t<TypeList_,typename UniqueTypeList_::HeadType>::COUNT == MULTIPLICITY_),
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
    typedef typename UniqueTypesIn_t<TypeList>::T UniqueTypeList;
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
    typedef typename If<Predicate_::template Eval_t<typename TypeList_::HeadType>::V,
                        TypeList_t<typename TypeList_::HeadType,ElementsInBodyTypeListSatisfyingPredicate>,
                        ElementsInBodyTypeListSatisfyingPredicate>::T T;
};

/// @cond false
template <typename HeadType_, typename Predicate_>
struct ElementsOfTypeListSatisfyingPredicate_t<TypeList_t<HeadType_>,Predicate_>
{
    typedef typename If<Predicate_::template Eval_t<HeadType_>::V,
                        TypeList_t<HeadType_>,
                        EmptyTypeList>::T T;
};

template <typename Predicate_>
struct ElementsOfTypeListSatisfyingPredicate_t<EmptyTypeList,Predicate_>
{
    typedef EmptyTypeList T;
};
/// @endcond


/// @struct NegationOfPredicate_t typelist_utility.hpp "tenh/meta/typelist_utility.hpp"
/// @brief Is a predicate, which is the negation of the supplied predicate.
/// @todo Should this be named NegationOfPredicate_p, and should we provide other logical predicate builders?
/// @tparam Predicate_ the predicate to negate.
template <typename Predicate_>
struct NegationOfPredicate_t
{
    /// @cond false
    template <typename T> struct Eval_t { static bool const V = !Predicate_::template Eval_t<T>::V; };
    /// @endcond
};


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
    static bool const V = TypeListB_::template Contains_t<typename TypeListA_::HeadType>::V && IsASubsetOf_t<typename TypeListA_::BodyTypeList,TypeListB_>::V;
};

/// @cond false
template <typename TypeListAHead_, typename TypeListB_>
struct IsASubsetOf_t<TypeList_t<TypeListAHead_>,TypeListB_>
{
    static bool const V = TypeListB_::template Contains_t<TypeListAHead_>::V;
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
    static bool const V = TypeListB_::template Contains_t<typename TypeListA_::HeadType>::V ||
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
    typedef typename If<TypeListB_::template Contains_t<typename TypeListA_::HeadType>::V,
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
    typedef typename If<TypeListB_::template Contains_t<typename TypeListA_::HeadType>::V,
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
    typedef TypeList_t<HeadTypes_,typename Zip_t<BodyTypeLists_>::T> T;
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
    static bool const V = TypesAreEqual<HeadType_,typename BodyTypeList::HeadType_>::V &&
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
