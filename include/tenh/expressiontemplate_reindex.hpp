// ///////////////////////////////////////////////////////////////////////////
// tenh/expressiontemplate_reindex.hpp by Victor Dods, created 2013/10/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_EXPRESSIONTEMPLATE_REINDEX_HPP_
#define TENH_EXPRESSIONTEMPLATE_REINDEX_HPP_

#include "tenh/core.hpp"

#include "tenh/expression_templates.hpp"
#include "tenh/expression_templates_utility.hpp"
#include "tenh/expressiontemplate_eval.hpp"
#include "tenh/reindex.hpp"

namespace Tenh {

// unfortunately GCC 4.2.1 required that there be forward declarations of all these
// reindexed functions.  Clang and later versions of GCC didn't, so they may be
// something we can take out later.

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename Object,
          typename FactorTyple,
          typename DimIndexTyple,
          typename SummedDimIndexTyple_,
          ForceConst FORCE_CONST_,
          CheckForAliasing CHECK_FOR_ALIASING_,
          typename Derived_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                              FactorTyple,
                                                              DimIndexTyple,
                                                              SummedDimIndexTyple_,
                                                              FORCE_CONST_,
                                                              CHECK_FOR_ALIASING_,
                                                              Derived_>>::T
    reindexed (ExpressionTemplate_IndexedObject_t<Object,
                                                  FactorTyple,
                                                  DimIndexTyple,
                                                  SummedDimIndexTyple_,
                                                  FORCE_CONST_,
                                                  CHECK_FOR_ALIASING_,
                                                  Derived_> const &e);

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename Object,
          typename FactorTyple,
          typename DimIndexTyple,
          typename SummedDimIndexTyple_,
          ForceConst FORCE_CONST_,
          CheckForAliasing CHECK_FOR_ALIASING_,
          typename Derived_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                              FactorTyple,
                                                              DimIndexTyple,
                                                              SummedDimIndexTyple_,
                                                              FORCE_CONST_,
                                                              CHECK_FOR_ALIASING_,
                                                              Derived_>>::T
    reindexed (ExpressionTemplate_IndexedObject_t<Object,
                                                  FactorTyple,
                                                  DimIndexTyple,
                                                  SummedDimIndexTyple_,
                                                  FORCE_CONST_,
                                                  CHECK_FOR_ALIASING_,
                                                  Derived_> &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename LeftOperand, typename RightOperand, char OPERATOR>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>>::T
    reindexed (ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> const &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename LeftOperand, typename RightOperand, char OPERATOR>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>>::T
    reindexed (ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename Scalar_, char OPERATOR>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>>::T
    reindexed (ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR> const &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename Scalar_, char OPERATOR>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>>::T
    reindexed (ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR> &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename LeftOperand, typename RightOperand>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>>::T
    reindexed (ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand> const &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename LeftOperand, typename RightOperand>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>>::T
    reindexed (ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand> &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename BundleAbstractIndexTyple, typename ResultingFactorType, typename ResultingAbstractIndexType, CheckFactorTypes CHECK_FACTOR_TYPES_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>>::T
    reindexed (ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_> const &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename BundleAbstractIndexTyple, typename ResultingFactorType, typename ResultingAbstractIndexType, CheckFactorTypes CHECK_FACTOR_TYPES_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>>::T
    reindexed (ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_> &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTyple>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>>::T
    reindexed (ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple> const &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTyple>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>>::T
    reindexed (ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple> &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>>::T
    reindexed (ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType> const &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>>::T
    reindexed (ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType> &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand_,
          typename SourceAbstractIndexType_,
          typename EmbeddingCodomain_,
          typename EmbeddedAbstractIndexType_,
          typename EmbeddingId_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>>::T
    reindexed (ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_> const &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand_,
          typename SourceAbstractIndexType_,
          typename EmbeddingCodomain_,
          typename EmbeddedAbstractIndexType_,
          typename EmbeddingId_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>>::T
    reindexed (ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_> &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Eval_t<Operand>>::T
    reindexed (ExpressionTemplate_Eval_t<Operand> const &e);

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Eval_t<Operand>>::T
    reindexed (ExpressionTemplate_Eval_t<Operand> &e);

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e<...>::Eval_f and reindexed<...>() for all the templates
// in tenh/expression_templates.hpp -- they parallel each expression template
// ///////////////////////////////////////////////////////////////////////////

// ///////////////////////////////////////////////////////////////////////////
// ExpressionTemplate_IndexedObject_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Object,
          typename FactorTyple,
          typename DimIndexTyple,
          typename SummedDimIndexTyple_,
          ForceConst FORCE_CONST_,
          CheckForAliasing CHECK_FOR_ALIASING_,
          typename Derived_>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                FactorTyple,
                                                DimIndexTyple,
                                                SummedDimIndexTyple_,
                                                FORCE_CONST_,
                                                CHECK_FOR_ALIASING_,
                                                Derived_>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef ExpressionTemplate_IndexedObject_t<typename Reindex::template Eval_f<Object>::T,
                                               typename Reindex::template Eval_f<FactorTyple>::T, // will just be FactorTyple, since it has no indices
                                               typename Reindex::template Eval_f<DimIndexTyple>::T,
                                               typename Reindex::template Eval_f<SummedDimIndexTyple_>::T,
                                               FORCE_CONST_,
                                               CHECK_FOR_ALIASING_,
                                               typename Reindex::template Eval_f<Derived_>::T> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename Object,
          typename FactorTyple,
          typename DimIndexTyple,
          typename SummedDimIndexTyple_,
          ForceConst FORCE_CONST_,
          CheckForAliasing CHECK_FOR_ALIASING_,
          typename Derived_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                              FactorTyple,
                                                              DimIndexTyple,
                                                              SummedDimIndexTyple_,
                                                              FORCE_CONST_,
                                                              CHECK_FOR_ALIASING_,
                                                              Derived_>>::T
    reindexed (ExpressionTemplate_IndexedObject_t<Object,
                                                  FactorTyple,
                                                  DimIndexTyple,
                                                  SummedDimIndexTyple_,
                                                  FORCE_CONST_,
                                                  CHECK_FOR_ALIASING_,
                                                  Derived_> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                                          FactorTyple,
                                                                          DimIndexTyple,
                                                                          SummedDimIndexTyple_,
                                                                          FORCE_CONST_,
                                                                          CHECK_FOR_ALIASING_,
                                                                          Derived_>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.object()));
}

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename Object,
          typename FactorTyple,
          typename DimIndexTyple,
          typename SummedDimIndexTyple_,
          ForceConst FORCE_CONST_,
          CheckForAliasing CHECK_FOR_ALIASING_,
          typename Derived_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                              FactorTyple,
                                                              DimIndexTyple,
                                                              SummedDimIndexTyple_,
                                                              FORCE_CONST_,
                                                              CHECK_FOR_ALIASING_,
                                                              Derived_>>::T
    reindexed (ExpressionTemplate_IndexedObject_t<Object,
                                                  FactorTyple,
                                                  DimIndexTyple,
                                                  SummedDimIndexTyple_,
                                                  FORCE_CONST_,
                                                  CHECK_FOR_ALIASING_,
                                                  Derived_> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                                          FactorTyple,
                                                                          DimIndexTyple,
                                                                          SummedDimIndexTyple_,
                                                                          FORCE_CONST_,
                                                                          CHECK_FOR_ALIASING_,
                                                                          Derived_>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.object()));
}

// ///////////////////////////////////////////////////////////////////////////
// ExpressionTemplate_Addition_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename LeftOperand, typename RightOperand, char OPERATOR>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef ExpressionTemplate_Addition_t<typename Reindex::template Eval_f<LeftOperand>::T,
                                          typename Reindex::template Eval_f<RightOperand>::T,
                                          OPERATOR> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename LeftOperand, typename RightOperand, char OPERATOR>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>>::T
    reindexed (ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.left_operand()),
                     reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.right_operand()));
}

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename LeftOperand, typename RightOperand, char OPERATOR>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>>::T
    reindexed (ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.left_operand()),
                     reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.right_operand()));
}

// ///////////////////////////////////////////////////////////////////////////
// ExpressionTemplate_ScalarMultiplication_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Operand, typename Scalar_, char OPERATOR>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef ExpressionTemplate_ScalarMultiplication_t<typename Reindex::template Eval_f<Operand>::T,Scalar_,OPERATOR> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename Scalar_, char OPERATOR>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>>::T
    reindexed (ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()),
                     reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.scalar_operand()));
}

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename Scalar_, char OPERATOR>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>>::T
    reindexed (ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()),
                     reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.scalar_operand()));
}

// ///////////////////////////////////////////////////////////////////////////
// ExpressionTemplate_Multiplication_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename LeftOperand, typename RightOperand>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef ExpressionTemplate_Multiplication_t<typename Reindex::template Eval_f<LeftOperand>::T,
                                                typename Reindex::template Eval_f<RightOperand>::T> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename LeftOperand, typename RightOperand>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>>::T
    reindexed (ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.left_operand()),
                     reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.right_operand()));
}

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename LeftOperand, typename RightOperand>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>>::T
    reindexed (ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.left_operand()),
                     reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.right_operand()));
}

// ///////////////////////////////////////////////////////////////////////////
// ExpressionTemplate_IndexBundle_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Operand, typename BundleAbstractIndexTyple, typename ResultingFactorType, typename ResultingAbstractIndexType, CheckFactorTypes CHECK_FACTOR_TYPES_>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef ExpressionTemplate_IndexBundle_t<typename Reindex::template Eval_f<Operand>::T,
                                             typename Reindex::template Eval_f<BundleAbstractIndexTyple>::T,
                                             ResultingFactorType,
                                             typename Reindex::template Eval_f<ResultingAbstractIndexType>::T,
                                             CHECK_FACTOR_TYPES_> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename BundleAbstractIndexTyple, typename ResultingFactorType, typename ResultingAbstractIndexType, CheckFactorTypes CHECK_FACTOR_TYPES_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>>::T
    reindexed (ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()));
}

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename BundleAbstractIndexTyple, typename ResultingFactorType, typename ResultingAbstractIndexType, CheckFactorTypes CHECK_FACTOR_TYPES_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>>::T
    reindexed (ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()));
}

// ///////////////////////////////////////////////////////////////////////////
// ExpressionTemplate_IndexSplit_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTyple>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef ExpressionTemplate_IndexSplit_t<typename Reindex::template Eval_f<Operand>::T,
                                            typename Reindex::template Eval_f<SourceAbstractIndexType>::T,
                                            typename Reindex::template Eval_f<SplitAbstractIndexTyple>::T> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTyple>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>>::T
    reindexed (ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()));
}

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTyple>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>>::T
    reindexed (ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()));
}

// ///////////////////////////////////////////////////////////////////////////
// ExpressionTemplate_IndexSplitToIndex_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef ExpressionTemplate_IndexSplitToIndex_t<typename Reindex::template Eval_f<Operand>::T,
                                                   typename Reindex::template Eval_f<SourceAbstractIndexType>::T,
                                                   typename Reindex::template Eval_f<SplitAbstractIndexType>::T> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>>::T
    reindexed (ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()));
}

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>>::T
    reindexed (ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()));
}

// ///////////////////////////////////////////////////////////////////////////
// ExpressionTemplate_IndexSplitToIndex_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename EmbeddingCodomain_,
          typename EmbeddedAbstractIndexType_,
          typename EmbeddingId_>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef ExpressionTemplate_IndexEmbed_t<typename Reindex::template Eval_f<Operand_>::T,
                                            typename Reindex::template Eval_f<SourceAbstractIndexType_>::T,
                                            typename Reindex::template Eval_f<EmbeddingCodomain_>::T,
                                            typename Reindex::template Eval_f<EmbeddedAbstractIndexType_>::T,
                                            typename Reindex::template Eval_f<EmbeddingId_>::T> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand_,
          typename SourceAbstractIndexType_,
          typename EmbeddingCodomain_,
          typename EmbeddedAbstractIndexType_,
          typename EmbeddingId_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>>::T
    reindexed (ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()));
}

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand_,
          typename SourceAbstractIndexType_,
          typename EmbeddingCodomain_,
          typename EmbeddedAbstractIndexType_,
          typename EmbeddingId_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>>::T
    reindexed (ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()));
}

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e<...>::Eval_f for all the relevant templates (e.g. IndexBundle_t)
// in tenh/expression_templates_utility.hpp
// ///////////////////////////////////////////////////////////////////////////

// ///////////////////////////////////////////////////////////////////////////
// UnarySummation_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Tensor, typename TensorDimIndexTyple, typename SummedDimIndexTyple>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<UnarySummation_t<Tensor,TensorDimIndexTyple,SummedDimIndexTyple>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef UnarySummation_t<typename Reindex::template Eval_f<Tensor>::T,
                             typename Reindex::template Eval_f<TensorDimIndexTyple>::T,
                             typename Reindex::template Eval_f<SummedDimIndexTyple>::T> T;
};

// no reindex<...> function is necessary because this is made by the expression template

// ///////////////////////////////////////////////////////////////////////////
// BinarySummation_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename LeftOperand, typename RightOperand, typename FreeDimIndexTyple, typename SummedDimIndexTyple>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<BinarySummation_t<LeftOperand,RightOperand,FreeDimIndexTyple,SummedDimIndexTyple>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef BinarySummation_t<typename Reindex::template Eval_f<LeftOperand>::T,
                              typename Reindex::template Eval_f<RightOperand>::T,
                              typename Reindex::template Eval_f<FreeDimIndexTyple>::T,
                              typename Reindex::template Eval_f<SummedDimIndexTyple>::T> T;
};

// no reindex<...> function is necessary because this is made by the expression template

// ///////////////////////////////////////////////////////////////////////////
// IndexBundle_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Operand, typename BundleAbstractIndexTyple, typename ResultingFactorType, typename ResultingAbstractIndexType, CheckFactorTypes CHECK_FACTOR_TYPES_>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef IndexBundle_t<typename Reindex::template Eval_f<Operand>::T,
                          typename Reindex::template Eval_f<BundleAbstractIndexTyple>::T,
                          typename Reindex::template Eval_f<ResultingFactorType>::T,
                          typename Reindex::template Eval_f<ResultingAbstractIndexType>::T,
                          CHECK_FACTOR_TYPES_> T;
};

// no reindex<...> function is necessary because this is made by the expression template

// ///////////////////////////////////////////////////////////////////////////
// IndexSplitter_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTyple>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef IndexSplitter_t<typename Reindex::template Eval_f<Operand>::T,
                            typename Reindex::template Eval_f<SourceAbstractIndexType>::T,
                            typename Reindex::template Eval_f<SplitAbstractIndexTyple>::T> T;
};

// no reindex<...> function is necessary because this is made by the expression template

// ///////////////////////////////////////////////////////////////////////////
// IndexSplitToIndex_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef IndexSplitToIndex_t<typename Reindex::template Eval_f<Operand>::T,
                                typename Reindex::template Eval_f<SourceAbstractIndexType>::T,
                                typename Reindex::template Eval_f<SplitAbstractIndexType>::T> T;
};

// no reindex<...> function is necessary because this is made by the expression template

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e<...>::Eval_f and reindexed<...>() for ExpressionTemplate_Eval_t
// ///////////////////////////////////////////////////////////////////////////

// ///////////////////////////////////////////////////////////////////////////
// ExpressionTemplate_Eval_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename Operand>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<ExpressionTemplate_Eval_t<Operand>>
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef ExpressionTemplate_Eval_t<typename Reindex::template Eval_f<Operand>::T> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Eval_t<Operand>>::T
    reindexed (ExpressionTemplate_Eval_t<Operand> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_Eval_t<Operand>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()));
}

template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_,
          typename Operand>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<ExpressionTemplate_Eval_t<Operand>>::T
    reindexed (ExpressionTemplate_Eval_t<Operand> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<ExpressionTemplate_Eval_t<Operand>>::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.operand()));
}

} // end of namespace Tenh

#endif // TENH_EXPRESSIONTEMPLATE_REINDEX_HPP_
