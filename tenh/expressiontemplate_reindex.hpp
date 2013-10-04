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

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e<...>::Eval_f template specializations for all the templates
// in tenh/expression_templates.hpp -- they parallel each expression template
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Object,
          typename FactorTypeList,
          typename DimIndexTypeList,
          typename SummedDimIndexTypeList_,
          bool FORCE_CONST_,
          bool CHECK_FOR_ALIASING_,
          typename Derived_>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                FactorTypeList,
                                                DimIndexTypeList,
                                                SummedDimIndexTypeList_,
                                                FORCE_CONST_,
                                                CHECK_FOR_ALIASING_,
                                                Derived_> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef ExpressionTemplate_IndexedObject_t<typename Reindex::template Eval_f<Object>::T,
                                               FactorTypeList,
                                               typename Reindex::template Eval_f<DimIndexTypeList>::T,
                                               typename Reindex::template Eval_f<SummedDimIndexTypeList_>::T,
                                               FORCE_CONST_,
                                               CHECK_FOR_ALIASING_,
                                               typename Reindex::template Eval_f<Derived_>::T> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename Object,
          typename FactorTypeList,
          typename DimIndexTypeList,
          typename SummedDimIndexTypeList_,
          bool FORCE_CONST_,
          bool CHECK_FOR_ALIASING_,
          typename Derived_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                              FactorTypeList,
                                                              DimIndexTypeList,
                                                              SummedDimIndexTypeList_,
                                                              FORCE_CONST_,
                                                              CHECK_FOR_ALIASING_,
                                                              Derived_> >::T
    reindexed (ExpressionTemplate_IndexedObject_t<Object,
                                                  FactorTypeList,
                                                  DimIndexTypeList,
                                                  SummedDimIndexTypeList_,
                                                  FORCE_CONST_,
                                                  CHECK_FOR_ALIASING_,
                                                  Derived_> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                     ::template Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                                          FactorTypeList,
                                                                          DimIndexTypeList,
                                                                          SummedDimIndexTypeList_,
                                                                          FORCE_CONST_,
                                                                          CHECK_FOR_ALIASING_,
                                                                          Derived_> >::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(e.object()));
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename Object,
          typename FactorTypeList,
          typename DimIndexTypeList,
          typename SummedDimIndexTypeList_,
          bool FORCE_CONST_,
          bool CHECK_FOR_ALIASING_,
          typename Derived_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                              FactorTypeList,
                                                              DimIndexTypeList,
                                                              SummedDimIndexTypeList_,
                                                              FORCE_CONST_,
                                                              CHECK_FOR_ALIASING_,
                                                              Derived_> >::T
    reindexed (ExpressionTemplate_IndexedObject_t<Object,
                                                  FactorTypeList,
                                                  DimIndexTypeList,
                                                  SummedDimIndexTypeList_,
                                                  FORCE_CONST_,
                                                  CHECK_FOR_ALIASING_,
                                                  Derived_> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                     ::template Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                                          FactorTypeList,
                                                                          DimIndexTypeList,
                                                                          SummedDimIndexTypeList_,
                                                                          FORCE_CONST_,
                                                                          CHECK_FOR_ALIASING_,
                                                                          Derived_> >::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(e.object()));
}

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename LeftOperand, typename RightOperand, char OPERATOR>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef ExpressionTemplate_Addition_t<typename Reindex::template Eval_f<LeftOperand>::T,
                                          typename Reindex::template Eval_f<RightOperand>::T,OPERATOR> T;
};

// unfortunately you have to make a const and a non-const version of each

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_,
          typename LeftOperand, typename RightOperand, char OPERATOR>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> >::T
    reindexed (ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                     ::template Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> >::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(e.left_operand()),
                     reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(e.right_operand()));
}

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_,
          typename LeftOperand, typename RightOperand, char OPERATOR>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> >::T
    reindexed (ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                     ::template Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> >::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(e.left_operand()),
                     reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(e.right_operand()));
}

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename Scalar_, char OPERATOR>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef ExpressionTemplate_ScalarMultiplication_t<typename Reindex::template Eval_f<Operand>::T,Scalar_,OPERATOR> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename LeftOperand, typename RightOperand>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef ExpressionTemplate_Multiplication_t<typename Reindex::template Eval_f<LeftOperand>::T,
                                                typename Reindex::template Eval_f<RightOperand>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename BundleAbstractIndexTypeList, typename ResultingFactorType, typename ResultingAbstractIndexType>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTypeList,ResultingFactorType,ResultingAbstractIndexType> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef ExpressionTemplate_IndexBundle_t<typename Reindex::template Eval_f<Operand>::T,
                                             typename Reindex::template Eval_f<BundleAbstractIndexTypeList>::T,
                                             ResultingFactorType,
                                             typename Reindex::template Eval_f<ResultingAbstractIndexType>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTypeList>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTypeList> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef ExpressionTemplate_IndexSplit_t<typename Reindex::template Eval_f<Operand>::T,
                                            typename Reindex::template Eval_f<SourceAbstractIndexType>::T,
                                            typename Reindex::template Eval_f<SplitAbstractIndexTypeList>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef ExpressionTemplate_IndexSplitToIndex_t<typename Reindex::template Eval_f<Operand>::T,
                                                   typename Reindex::template Eval_f<SourceAbstractIndexType>::T,
                                                   typename Reindex::template Eval_f<SplitAbstractIndexType>::T> T;
};

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e<...>::Eval_f template specializations for all the relevant
// templates (e.g. IndexBundle_t) in tenh/expression_templates_utility.hpp
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Tensor, typename TensorDimIndexTypeList, typename SummedDimIndexTypeList>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<UnarySummation_t<Tensor,TensorDimIndexTypeList,SummedDimIndexTypeList> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef UnarySummation_t<typename Reindex::template Eval_f<Tensor>::T,
                             typename Reindex::template Eval_f<TensorDimIndexTypeList>::T,
                             typename Reindex::template Eval_f<SummedDimIndexTypeList>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename LeftOperand, typename RightOperand, typename FreeDimIndexTypeList, typename SummedDimIndexTypeList>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<BinarySummation_t<LeftOperand,RightOperand,FreeDimIndexTypeList,SummedDimIndexTypeList> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef BinarySummation_t<typename Reindex::template Eval_f<LeftOperand>::T,
                              typename Reindex::template Eval_f<RightOperand>::T,
                              typename Reindex::template Eval_f<FreeDimIndexTypeList>::T,
                              typename Reindex::template Eval_f<SummedDimIndexTypeList>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename BundleAbstractIndexTypeList, typename ResultingFactorType, typename ResultingAbstractIndexType>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<IndexBundle_t<Operand,BundleAbstractIndexTypeList,ResultingFactorType,ResultingAbstractIndexType> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef IndexBundle_t<typename Reindex::template Eval_f<Operand>::T,
                          typename Reindex::template Eval_f<BundleAbstractIndexTypeList>::T,
                          typename Reindex::template Eval_f<ResultingFactorType>::T,
                          typename Reindex::template Eval_f<ResultingAbstractIndexType>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTypeList>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTypeList> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef IndexSplitter_t<typename Reindex::template Eval_f<Operand>::T,
                            typename Reindex::template Eval_f<SourceAbstractIndexType>::T,
                            typename Reindex::template Eval_f<SplitAbstractIndexTypeList>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef IndexSplitToIndex_t<typename Reindex::template Eval_f<Operand>::T,
                                typename Reindex::template Eval_f<SourceAbstractIndexType>::T,
                                typename Reindex::template Eval_f<SplitAbstractIndexType>::T> T;
};

// ///////////////////////////////////////////////////////////////////////////
// Reindex_e<...>::Eval_f specialization for ExpressionTemplate_Eval_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_Eval_t<Operand> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef ExpressionTemplate_Eval_t<typename Reindex::template Eval_f<Operand>::T> T;
};

} // end of namespace Tenh

#endif // TENH_EXPRESSIONTEMPLATE_REINDEX_HPP_
