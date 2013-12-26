// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/static_assert.hpp by Ted Nitz, created 2013/04/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////
/// @file static_assert.hpp
/// @headerfile static_assert.hpp "tenh/meta/static_assert.hpp"
/// @brief Contains macros and a template class for performing static asserts without C++11.
/// @details All error messages used in a static assert this way must be added to the anonymous
///  enum contained in `StaticAssert<true>`.
/// @todo TODO: Replace with C++11 static_assert.
#ifndef TENH_META_STATIC_ASSERT_HPP_
#define TENH_META_STATIC_ASSERT_HPP_

#include "tenh/meta/core.hpp"

namespace Tenh {

struct EmptyTypeList;

}

/// @brief Macro to perform a static assert within functions.
#define STATIC_ASSERT(CONDITION,MESSAGE) do { if(Tenh::StaticAssert<bool(CONDITION)>::MESSAGE) {} } while (false)
//#define STATIC_ASSERT_AS_RVALUE(CONDITION,MESSAGE) Tenh::StaticAssert<bool(CONDITION)>::MESSAGE
/// @brief Macro to perform a static assert within an enum. Typically an anonymous enum in the private section of a class.
#define STATIC_ASSERT_IN_ENUM(CONDITION,MESSAGE) MESSAGE = Tenh::StaticAssert<bool(CONDITION)>::MESSAGE
/// @brief The purpose of STATIC_ASSERT_IN_ENUM__UNIQUE is so that the same assert message can be used
///  multiple times in the same enum without a compile error -- however the AUX_MSG must be unique.
#define STATIC_ASSERT_IN_ENUM__UNIQUE(CONDITION,MESSAGE,AUX_MSG) MESSAGE_##AUX_MSG = Tenh::StaticAssert<bool(CONDITION)>::MESSAGE
/// @brief Asserts that a TypeList_t is the EmptyTypeList using STATIC_ASSERT.
#define STATIC_ASSERT_TYPELIST_IS_EMPTY(TYPELIST) STATIC_ASSERT((TypesAreEqual_f<TYPELIST,Tenh::EmptyTypeList>::V), TYPELIST_MUST_BE_EMPTY)
/// @brief Asserts that two types are equal using STATIC_ASSERT.
#define STATIC_ASSERT_TYPES_ARE_EQUAL(TYPE1,TYPE2) STATIC_ASSERT((TypesAreEqual_f<TYPE1,TYPE2>::V), TYPES_MUST_BE_EQUAL)

namespace Tenh {

/// @cond false
template <bool condition>
struct StaticAssert {};
/// @endcond

/// @brief Implementation detail for the static assert macros.
/// @headerfile static_assert.hpp "tenh/meta/static_assert.hpp"
template <>
struct StaticAssert<true>
{
    /// @brief Every error message issued in a static assert macro must exist within this enum.
    enum
    {
        /// @cond false
        ABSTRACT_INDEX_SYMBOL_MUST_BE_POSITIVE,
        ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL,
        ALL_FACTORS_MUST_BE_BASED_VECTOR_SPACES,
        ALL_FACTORS_MUST_BE_BASES,
        ALL_FACTORS_MUST_BE_VECTOR_SPACES,
        ALL_FACTORS_MUST_HAVE_SAME_FIELD,
        ALL_SUMMATIONS_MUST_BE_NATURAL_PAIRINGS,
        ALL_TYPES_IN_TYPELIST_MUST_BE_THE_SAME,
        ARGUMENT_LENGTH_MUST_EQUAL_ORDER,
        ATTEMPTED_ACCESS_PAST_LIST_END,
        BASIS_IS_STANDARD_EUCLIDEAN,
        BUNDLE_AND_RESULTING_MUST_BE_DISTINCT,
        BUNDLE_INDICES_MUST_BE_FREE,
        BUNDLE_FACTORS_MUST_MATCH,
        CAN_ONLY_BUNDLE_TWO_INDICES,
        CANT_CONTRACT_WITH_EUCLIDEANLY_EMBEDDED_TENSOR,
        CANT_MAKE_EIGEN_MAP_OF_PROCEDURAL_ARRAY,
        COMPONENT_COUNT_DOES_NOT_MATCH_ARGUMENT_COUNT,
        ORDER_MUST_BE_TWO,
        DERIVED_MUST_NOT_BE_NULL_TYPE,
        DOMAIN_INDICES_MUST_NOT_CONTAIN_DUPLICATES,
        DIMENSION_MUST_BE_POSITIVE,
        DIMENSION_MUST_BE_EXACTLY_3,
        DIMENSIONS_MUST_MATCH,
        EACH_TYPE_MUST_BE_ABSTRACT_INDEX,
        EACH_TYPE_MUST_BE_INDEX,
        FACTOR_DIMENSIONS_MUST_BE_EQUAL,
        FACTOR_SCALAR_TYPES_ARE_EQUAL,
        FACTOR_TYPE_ERROR_ON_INVERSE_TENSOR_TYPE,
        FACTOR1_AND_FACTOR2_MUST_BE_IDENTICAL,
        FREE_INDICES_DONT_COLLIDE_WITH_USED,
        FREE_INDICES_HAVE_NO_DUPLICATES,
        FREE_INDEX_COUNT_MUST_BE_AT_MOST_2,
        INDEX_OUT_OF_RANGE,
        INVALID_RANGE_INDICES,
        LEFT_OPERAND_HAS_NO_DUPLICATE_FREE_INDICES,
        LEFT_OPERAND_IS_EXPRESSION_TEMPLATE,
        LENGTH_DOES_NOT_MATCH_ARGUMENT_COUNT,
        LENGTH_MUST_BE_AT_LEAST_1,
        LENGTH_MUST_BE_EXACTLY_0_OR_2,
        LENGTH_MUST_BE_EXACTLY_1_OR_2,
        LENGTH_MUST_BE_EXACTLY_1,
        LENGTH_MUST_BE_EXACTLY_2,
        LENGTH_MUST_BE_POSITIVE,
        LENGTHS_MUST_BE_EQUAL,
        MUST_BE_ABSTRACT_INDEX,
        MUST_BE_BASED_VECTOR_SPACE,
        MUST_BE_BASIS,
        MUST_BE_COMPONENT_GENERATOR,
        MUST_BE_COMPONENT_INDEX,
        MUST_BE_CONCEPT,
        MUST_BE_DIM_INDEX,
        MUST_BE_DIRECT_SUM_OF_VECTOR_SPACES,
        MUST_BE_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_VECTOR_SPACES,
        MUST_BE_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES,
        MUST_BE_EXPRESSION_TEMPLATE,
        MUST_BE_EXTERIOR_POWER_OF_VECTOR_SPACE,
        MUST_BE_FIELD,
        MUST_BE_INDEX,
        MUST_BE_MULTI_INDEX,
        MUST_BE_NONEMPTY,
        MUST_BE_SUBSET_OF,
        MUST_BE_SYMMETRIC_POWER_OF_VECTOR_SPACE,
        MUST_BE_TENSOR_POWER,
        MUST_BE_TENSOR_POWER_OF_BASED_VECTOR_SPACE,
        MUST_BE_TENSOR_POWER_OF_VECTOR_SPACE,
        MUST_BE_TENSOR_PRODUCT,
        MUST_BE_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES,
        MUST_BE_TENSOR_PRODUCT_OF_VECTOR_SPACES,
        MUST_BE_TYPELIST,
        MUST_BE_TYPELIST_OF_2_TENSORS,
        MUST_BE_TYPELIST_OF_DIAGONAL_OR_2_TENSORS,
        MUST_BE_TYPELIST_OF_ABSTRACT_INDEX_TYPES,
        MUST_BE_TYPELIST_OF_BASED_VECTOR_SPACES,
        MUST_BE_TYPELIST_OF_DIAGONAL_2_TENSORS,
        MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES,
        MUST_BE_TYPELIST_OF_PROCEDURAL_IMPLEMENTATIONS,
        MUST_BE_USE_MEMBER_ARRAY,
        MUST_BE_USE_MEMORY_ARRAY,
        MUST_BE_USE_PREALLOCATED_ARRAY,
        MUST_BE_USE_PROCEDURAL_ARRAY,
        MUST_BE_USE_PROCEDURAL_ARRAY_OR_BE_ZERO_DIMENSIONAL,
        MUST_BE_VECTOR_SPACE,
        MUST_HAVE_EQUAL_LENGTHS,
        MUST_HAVE_UNIQUE_CONCEPTUAL_STRUCTURE,
        MUST_NOT_BE_COMPONENTS_ARE_PROCEDURAL,
        MUST_NOT_BE_USE_PROCEDURAL_ARRAY,
        MUST_OCCUR_EXACTLY_ONCE,
        MUST_OCCUR_EXACTLY_TWICE,
        ONLY_ONE_DIMENSIONAL_VECTORS_CAN_BE_CONVERTED_TO_SCALARS,
        OPERAND_HAS_THE_SAME_FREE_INDICES,
        OPERAND_IS_EXPRESSION_TEMPLATE,
        OPERAND_SCALAR_MATCHES_SCALAR,
        OPERAND_SCALAR_TYPES_ARE_EQUAL,
        OPERANDS_HAVE_SAME_FREE_INDICES,
        OPERANDS_HAVE_SAME_FACTORS,
        OPERATOR_IS_PLUS_OR_MINUS,
        OPERATOR_IS_VALID,
        ORDER_MUST_BE_EXACTLY_2,
        ORDER_MUST_BE_POSITIVE,
        PROPERTY_IS_NOT_WELL_DEFINED,
        RIGHT_OPERAND_HAS_NO_DUPLICATE_FREE_INDICES,
        RIGHT_OPERAND_IS_EXPRESSION_TEMPLATE,
        SOURCE_AND_SPLIT_MUST_BE_DISTINCT,
        SOURCE_INDEX_MUST_BE_FREE,
        START_INDEX_MUST_NOT_EXCEED_END_INDEX,
        SUMMATION_MUST_BE_NATURAL_PAIRING,
        TYPE_MUST_APPEAR_IN_TYPELIST,
        TYPE_MUST_BE_A_TENSOR_I,
        TYPE_MUST_BE_TENSOR2SYMMETRIC_T,
        TYPES_MUST_BE_EQUAL,
        TYPELIST_MUST_BE_EMPTY,
        TYPELIST_MUST_BE_UNIFORM,
        UNSPECIFIED_MESSAGE // for when it doesn't fucking matter.
        /// @endcond
    };
};

} // end of namespace Tenh

#endif // TENH_META_STATIC_ASSERT_HPP_
