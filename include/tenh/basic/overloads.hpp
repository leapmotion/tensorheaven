// ///////////////////////////////////////////////////////////////////////////
// tenh/basic/overloads.hpp by Victor Dods, created 2013/10/11
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_BASIC_OVERLOADS_HPP_
#define TENH_BASIC_OVERLOADS_HPP_

#include "tenh/core.hpp"

#include "tenh/basic/expressionoperand.hpp"
#include "tenh/basic/reindexable.hpp"
#include "tenh/expressiontemplate_reindex.hpp"

namespace Tenh {

// for uniformly indexed types (e.g. for addition, subtraction, scalar multiplication)
// if FREE_INDEX_COUNT_ == 0, then there are no indices
// if FREE_INDEX_COUNT_ == 1, then the index is 'i'
// if FREE_INDEX_COUNT_ == 2, then the indices are 'i','j'

template <Uint32 FREE_INDEX_COUNT_>
struct UniformAbstractIndexTyple_f;

template <>
struct UniformAbstractIndexTyple_f<0>
{
    typedef Typle_t<> T;
private:
    UniformAbstractIndexTyple_f();
};

template <>
struct UniformAbstractIndexTyple_f<1>
{
    typedef Typle_t<AbstractIndex_c<'i'>> T;
private:
    UniformAbstractIndexTyple_f();
};

template <>
struct UniformAbstractIndexTyple_f<2>
{
    typedef Typle_t<AbstractIndex_c<'i'>,AbstractIndex_c<'j'>> T;
private:
    UniformAbstractIndexTyple_f();
};

template <typename Derived_, Uint32 FREE_INDEX_COUNT_>
struct UniformlyIndexedExpressionTemplate_f;

template <typename ExpressionTemplate_, typename FreeDimIndexTyple_, Uint32 FREE_INDEX_COUNT_>
struct UniformlyIndexedExpressionTemplate_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_>,FREE_INDEX_COUNT_>
{
private:
    enum { STATIC_ASSERT_IN_ENUM(Length_f<FreeDimIndexTyple_>::V == FREE_INDEX_COUNT_, LENGTHS_MUST_BE_EQUAL) };
    typedef Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> Reindexable;
    typedef typename AbstractIndicesOfDimIndexTyple_f<FreeDimIndexTyple_>::T DomainAbstractIndexTyple;
    typedef typename UniformAbstractIndexTyple_f<FREE_INDEX_COUNT_>::T CodomainAbstractIndexTyple;
    typedef Reindex_e<DomainAbstractIndexTyple,CodomainAbstractIndexTyple> Reindex;
    typedef typename ExpressionTemplate_::Derived DerivedExpressionTemplate;
    UniformlyIndexedExpressionTemplate_f();
public:
    typedef typename Reindex::template Eval_f<DerivedExpressionTemplate>::T T;
};

template <typename Derived_, Uint32 FREE_INDEX_COUNT_>
typename UniformlyIndexedExpressionTemplate_f<Derived_,FREE_INDEX_COUNT_>::T
    uniformly_indexed (ExpressionOperand_i<Derived_,FREE_INDEX_COUNT_> const &e)
{
    typedef typename UniformAbstractIndexTyple_f<FREE_INDEX_COUNT_>::T Indices;
    return e.as_derived()(Indices());
}

// for lhs-of-contraction indexed types
// if FREE_INDEX_COUNT_ == 0, then there are no indices
// if FREE_INDEX_COUNT_ == 1, then the index is 'j'
// if FREE_INDEX_COUNT_ == 2, then the indices are 'i','j'

template <Uint32 FREE_INDEX_COUNT_>
struct LhsOfContractionAbstractIndexTyple_f;

template <>
struct LhsOfContractionAbstractIndexTyple_f<0>
{
    typedef Typle_t<> T;
private:
    LhsOfContractionAbstractIndexTyple_f();
};

template <>
struct LhsOfContractionAbstractIndexTyple_f<1>
{
    typedef Typle_t<AbstractIndex_c<'j'>> T;
private:
    LhsOfContractionAbstractIndexTyple_f();
};

template <>
struct LhsOfContractionAbstractIndexTyple_f<2>
{
    typedef Typle_t<AbstractIndex_c<'i'>,AbstractIndex_c<'j'>> T;
private:
    LhsOfContractionAbstractIndexTyple_f();
};

template <typename Derived_, Uint32 FREE_INDEX_COUNT_>
struct LhsIndexedContractionExpressionTemplate_f;

template <typename ExpressionTemplate_, typename FreeDimIndexTyple_, Uint32 FREE_INDEX_COUNT_>
struct LhsIndexedContractionExpressionTemplate_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_>,FREE_INDEX_COUNT_>
{
private:
    enum { STATIC_ASSERT_IN_ENUM(Length_f<FreeDimIndexTyple_>::V == FREE_INDEX_COUNT_, LENGTHS_MUST_BE_EQUAL) };
    typedef Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> Reindexable;
    typedef typename AbstractIndicesOfDimIndexTyple_f<FreeDimIndexTyple_>::T DomainAbstractIndexTyple;
    typedef typename LhsOfContractionAbstractIndexTyple_f<FREE_INDEX_COUNT_>::T CodomainAbstractIndexTyple;
    typedef Reindex_e<DomainAbstractIndexTyple,CodomainAbstractIndexTyple> Reindex;
    typedef typename ExpressionTemplate_::Derived DerivedExpressionTemplate;
    LhsIndexedContractionExpressionTemplate_f();
public:
    typedef typename Reindex::template Eval_f<DerivedExpressionTemplate>::T T;
};

template <typename Derived_, Uint32 FREE_INDEX_COUNT_>
typename LhsIndexedContractionExpressionTemplate_f<Derived_,FREE_INDEX_COUNT_>::T
    indexed_as_lhs_of_contraction (ExpressionOperand_i<Derived_,FREE_INDEX_COUNT_> const &e)
{
    typedef typename LhsOfContractionAbstractIndexTyple_f<FREE_INDEX_COUNT_>::T LhsIndices;
    return e.as_derived()(LhsIndices());
}

// for rhs-of-contraction indexed types
// if FREE_INDEX_COUNT_ == 0, then there are no indices
// if FREE_INDEX_COUNT_ == 1, then the index is 'j'
// if FREE_INDEX_COUNT_ == 2, then the indices are 'j','k'

template <Uint32 FREE_INDEX_COUNT_>
struct RhsOfContractionAbstractIndexTyple_f;

template <>
struct RhsOfContractionAbstractIndexTyple_f<0>
{
    typedef Typle_t<> T;
private:
    RhsOfContractionAbstractIndexTyple_f();
};

template <>
struct RhsOfContractionAbstractIndexTyple_f<1>
{
    typedef Typle_t<AbstractIndex_c<'j'>> T;
private:
    RhsOfContractionAbstractIndexTyple_f();
};

template <>
struct RhsOfContractionAbstractIndexTyple_f<2>
{
    typedef Typle_t<AbstractIndex_c<'j'>,AbstractIndex_c<'k'>> T;
private:
    RhsOfContractionAbstractIndexTyple_f();
};

template <typename Derived_, Uint32 FREE_INDEX_COUNT_>
struct RhsIndexedContractionExpressionTemplate_f;

template <typename ExpressionTemplate_, typename FreeDimIndexTyple_, Uint32 FREE_INDEX_COUNT_>
struct RhsIndexedContractionExpressionTemplate_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_>,FREE_INDEX_COUNT_>
{
private:
    enum { STATIC_ASSERT_IN_ENUM(Length_f<FreeDimIndexTyple_>::V == FREE_INDEX_COUNT_, LENGTHS_MUST_BE_EQUAL) };
    typedef Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> Reindexable;
    typedef typename AbstractIndicesOfDimIndexTyple_f<FreeDimIndexTyple_>::T DomainAbstractIndexTyple;
    typedef typename RhsOfContractionAbstractIndexTyple_f<FREE_INDEX_COUNT_>::T CodomainAbstractIndexTyple;
    typedef Reindex_e<DomainAbstractIndexTyple,CodomainAbstractIndexTyple> Reindex;
    typedef typename ExpressionTemplate_::Derived DerivedExpressionTemplate;
    RhsIndexedContractionExpressionTemplate_f();
public:
    typedef typename Reindex::template Eval_f<DerivedExpressionTemplate>::T T;
};

template <typename Derived_, Uint32 FREE_INDEX_COUNT_>
typename RhsIndexedContractionExpressionTemplate_f<Derived_,FREE_INDEX_COUNT_>::T
    indexed_as_rhs_of_contraction (ExpressionOperand_i<Derived_,FREE_INDEX_COUNT_> const &e)
{
    typedef typename RhsOfContractionAbstractIndexTyple_f<FREE_INDEX_COUNT_>::T RhsIndices;
    return e.as_derived()(RhsIndices());
}

// ///////////////////////////////////////////////////////////////////////////
// operator overloads
// ///////////////////////////////////////////////////////////////////////////

// addition

template <typename LhsDerived_,
          Uint32 LHS_FREE_INDEX_COUNT_,
          typename RhsDerived_,
          Uint32 RHS_FREE_INDEX_COUNT_>
struct OperatorAdditionReturnType_f
{
    typedef Reindexable_t<
        ExpressionTemplate_Addition_t<
            typename UniformlyIndexedExpressionTemplate_f<LhsDerived_,LHS_FREE_INDEX_COUNT_>::T,
            typename UniformlyIndexedExpressionTemplate_f<RhsDerived_,RHS_FREE_INDEX_COUNT_>::T,
            '+'>> T;
private:
    OperatorAdditionReturnType_f();
};

template <typename LhsDerived_,
          Uint32 LHS_FREE_INDEX_COUNT_,
          typename RhsDerived_,
          Uint32 RHS_FREE_INDEX_COUNT_>
typename OperatorAdditionReturnType_f<LhsDerived_,LHS_FREE_INDEX_COUNT_,RhsDerived_,RHS_FREE_INDEX_COUNT_>::T
    operator + (ExpressionOperand_i<LhsDerived_,LHS_FREE_INDEX_COUNT_> const &lhs,
                ExpressionOperand_i<RhsDerived_,RHS_FREE_INDEX_COUNT_> const &rhs)
{
    // This sum is an ExpressionTemplate_Addition_t type, but the return will implicitly
    // convert to the Reindexable_t<...> return type.
    return uniformly_indexed(lhs) + uniformly_indexed(rhs);
}

// subtraction

template <typename LhsDerived_,
          Uint32 LHS_FREE_INDEX_COUNT_,
          typename RhsDerived_,
          Uint32 RHS_FREE_INDEX_COUNT_>
struct OperatorSubtractionReturnType_f
{
    typedef Reindexable_t<
        ExpressionTemplate_Addition_t<
            typename UniformlyIndexedExpressionTemplate_f<LhsDerived_,LHS_FREE_INDEX_COUNT_>::T,
            typename UniformlyIndexedExpressionTemplate_f<RhsDerived_,RHS_FREE_INDEX_COUNT_>::T,
            '-'>> T;
private:
    OperatorSubtractionReturnType_f();
};

template <typename LhsDerived_,
          Uint32 LHS_FREE_INDEX_COUNT_,
          typename RhsDerived_,
          Uint32 RHS_FREE_INDEX_COUNT_>
typename OperatorSubtractionReturnType_f<LhsDerived_,LHS_FREE_INDEX_COUNT_,RhsDerived_,RHS_FREE_INDEX_COUNT_>::T
    operator - (ExpressionOperand_i<LhsDerived_,LHS_FREE_INDEX_COUNT_> const &lhs,
                ExpressionOperand_i<RhsDerived_,RHS_FREE_INDEX_COUNT_> const &rhs)
{
    // This difference is an ExpressionTemplate_Addition_t type, but the return will implicitly
    // convert to the Reindexable_t<...> return type.
    return uniformly_indexed(lhs) - uniformly_indexed(rhs);
}

// scalar multiplication

template <typename Derived_,
          Uint32 FREE_INDEX_COUNT_,
          char OPERATOR_>
struct OperatorScalarMultiplicationReturnType_f
{
    typedef Reindexable_t<
        ExpressionTemplate_ScalarMultiplication_t<
            typename UniformlyIndexedExpressionTemplate_f<Derived_,FREE_INDEX_COUNT_>::T,
            typename Derived_::Scalar,
            OPERATOR_>> T;
private:
    OperatorScalarMultiplicationReturnType_f();
};

template <typename Derived_,
          Uint32 FREE_INDEX_COUNT_>
typename OperatorScalarMultiplicationReturnType_f<Derived_,FREE_INDEX_COUNT_,'*'>::T
    operator * (ExpressionOperand_i<Derived_,FREE_INDEX_COUNT_> const &lhs,
                typename Derived_::Scalar const &rhs)
{
    // This product is an ExpressionTemplate_ScalarMultiplication_t type, but the return will implicitly
    // convert to the Reindexable_t<...> return type.
    return uniformly_indexed(lhs) * rhs;
}

template <typename Derived_,
          Uint32 FREE_INDEX_COUNT_>
typename OperatorScalarMultiplicationReturnType_f<Derived_,FREE_INDEX_COUNT_,'*'>::T
    operator * (typename Derived_::Scalar const &lhs,
                ExpressionOperand_i<Derived_,FREE_INDEX_COUNT_> const &rhs)
{
    // This product is an ExpressionTemplate_ScalarMultiplication_t type, but the return will implicitly
    // convert to the Reindexable_t<...> return type.
    return lhs * uniformly_indexed(rhs);
}

template <typename Derived_,
          Uint32 FREE_INDEX_COUNT_,
          typename ScalarOperand_>
typename OperatorScalarMultiplicationReturnType_f<Derived_,FREE_INDEX_COUNT_,'/'>::T
    operator / (ExpressionOperand_i<Derived_,FREE_INDEX_COUNT_> const &lhs,
                ScalarOperand_ const &rhs)
{
    // This product is an ExpressionTemplate_ScalarMultiplication_t type, but the return will implicitly
    // convert to the Reindexable_t<...> return type.
    return uniformly_indexed(lhs) / rhs;
}

// negation

template <typename Derived_,
          Uint32 FREE_INDEX_COUNT_>
typename OperatorScalarMultiplicationReturnType_f<Derived_,FREE_INDEX_COUNT_,'*'>::T
    operator - (ExpressionOperand_i<Derived_,FREE_INDEX_COUNT_> const &op)
{
    // This product is an ExpressionTemplate_ScalarMultiplication_t type, but the return will implicitly
    // convert to the Reindexable_t<...> return type.
    return -uniformly_indexed(op);
}

// composition

template <typename LhsDerived_,
          Uint32 LHS_FREE_INDEX_COUNT_,
          typename RhsDerived_,
          Uint32 RHS_FREE_INDEX_COUNT_>
struct OperatorMultiplicationReturnType_f
{
    typedef Reindexable_t<
        ExpressionTemplate_Multiplication_t<
            typename LhsIndexedContractionExpressionTemplate_f<LhsDerived_,LHS_FREE_INDEX_COUNT_>::T,
            typename RhsIndexedContractionExpressionTemplate_f<RhsDerived_,RHS_FREE_INDEX_COUNT_>::T>> T;
private:
    OperatorMultiplicationReturnType_f();
};

template <typename LhsDerived_,
          Uint32 LHS_FREE_INDEX_COUNT_,
          typename RhsDerived_,
          Uint32 RHS_FREE_INDEX_COUNT_>
typename OperatorMultiplicationReturnType_f<LhsDerived_,LHS_FREE_INDEX_COUNT_,RhsDerived_,RHS_FREE_INDEX_COUNT_>::T
    operator * (ExpressionOperand_i<LhsDerived_,LHS_FREE_INDEX_COUNT_> const &lhs,
                ExpressionOperand_i<RhsDerived_,RHS_FREE_INDEX_COUNT_> const &rhs)
{
    // This sum is an ExpressionTemplate_Multiplication_t type, but the return will implicitly
    // convert to the Reindexable_t<...> return type.
    return indexed_as_lhs_of_contraction(lhs) * indexed_as_rhs_of_contraction(rhs);
}

} // end of namespace Tenh

#endif // TENH_BASIC_OVERLOADS_HPP_
