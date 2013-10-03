// ///////////////////////////////////////////////////////////////////////////
// tenh/expressiontemplate_indexrenamer.hpp by Victor Dods, created 2013/10/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_EXPRESSIONTEMPLATE_INDEXRENAMER_HPP_
#define TENH_EXPRESSIONTEMPLATE_INDEXRENAMER_HPP_

#include "tenh/core.hpp"

#include "tenh/indexrenamer.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/expression_templates_utility.hpp"
#include "tenh/expressiontemplate_eval.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// function which reindexes actual instances -- default definition is a no-op.
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename ThingThatHasIndices_>
ThingThatHasIndices_ const &reindexed (ThingThatHasIndices_ const &x)
{
    return x;
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename ThingThatHasIndices_>
ThingThatHasIndices_ &reindexed (ThingThatHasIndices_ &x)
{
    return x;
}

// ///////////////////////////////////////////////////////////////////////////
// IndexRenamer_e<...>::Eval_f template specializations for all the templates
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
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_IndexedObject_t<Object,
                                                FactorTypeList,
                                                DimIndexTypeList,
                                                SummedDimIndexTypeList_,
                                                FORCE_CONST_,
                                                CHECK_FOR_ALIASING_,
                                                Derived_> >
{
public:
    typedef ExpressionTemplate_IndexedObject_t<typename IndexRenamer_e::Eval_f<Object>::T,
                                               FactorTypeList,
                                               typename IndexRenamer_e::Eval_f<DimIndexTypeList>::T,
                                               typename IndexRenamer_e::Eval_f<SummedDimIndexTypeList_>::T,
                                               FORCE_CONST_,
                                               CHECK_FOR_ALIASING_,
                                               typename IndexRenamer_e::Eval_f<Derived_>::T> T;
};

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename Object,
          typename FactorTypeList,
          typename DimIndexTypeList,
          typename SummedDimIndexTypeList_,
          bool FORCE_CONST_,
          bool CHECK_FOR_ALIASING_,
          typename Derived_>
typename IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
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
    typedef typename IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
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
typename IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
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
    typedef typename IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
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
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR> >
{
public:
    typedef ExpressionTemplate_Addition_t<typename IndexRenamer_e::Eval_f<LeftOperand>::T,
                                          typename IndexRenamer_e::Eval_f<RightOperand>::T,OPERATOR> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename Scalar_, char OPERATOR>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR> >
{
public:
    typedef ExpressionTemplate_ScalarMultiplication_t<typename IndexRenamer_e::Eval_f<Operand>::T,Scalar_,OPERATOR> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename LeftOperand, typename RightOperand>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand> >
{
public:
    typedef ExpressionTemplate_Multiplication_t<typename IndexRenamer_e::Eval_f<LeftOperand>::T,
                                                typename IndexRenamer_e::Eval_f<RightOperand>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename BundleAbstractIndexTypeList, typename ResultingFactorType, typename ResultingAbstractIndexType>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTypeList,ResultingFactorType,ResultingAbstractIndexType> >
{
public:
    typedef ExpressionTemplate_IndexBundle_t<typename IndexRenamer_e::Eval_f<Operand>::T,
                                             typename IndexRenamer_e::Eval_f<BundleAbstractIndexTypeList>::T,
                                             ResultingFactorType,
                                             typename IndexRenamer_e::Eval_f<ResultingAbstractIndexType>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTypeList>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTypeList> >
{
public:
    typedef ExpressionTemplate_IndexSplit_t<typename IndexRenamer_e::Eval_f<Operand>::T,
                                            typename IndexRenamer_e::Eval_f<SourceAbstractIndexType>::T,
                                            typename IndexRenamer_e::Eval_f<SplitAbstractIndexTypeList>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType> >
{
public:
    typedef ExpressionTemplate_IndexSplitToIndex_t<typename IndexRenamer_e::Eval_f<Operand>::T,
                                                   typename IndexRenamer_e::Eval_f<SourceAbstractIndexType>::T,
                                                   typename IndexRenamer_e::Eval_f<SplitAbstractIndexType>::T> T;
};

// ///////////////////////////////////////////////////////////////////////////
// IndexRenamer_e<...>::Eval_f template specializations for all the relevant
// templates (e.g. IndexBundle_t) in tenh/expression_templates_utility.hpp
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Tensor, typename TensorDimIndexTypeList, typename SummedDimIndexTypeList>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<UnarySummation_t<Tensor,TensorDimIndexTypeList,SummedDimIndexTypeList> >
{
public:
    typedef UnarySummation_t<typename IndexRenamer_e::Eval_f<Tensor>::T,
                             typename IndexRenamer_e::Eval_f<TensorDimIndexTypeList>::T,
                             typename IndexRenamer_e::Eval_f<SummedDimIndexTypeList>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename LeftOperand, typename RightOperand, typename FreeDimIndexTypeList, typename SummedDimIndexTypeList>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<BinarySummation_t<LeftOperand,RightOperand,FreeDimIndexTypeList,SummedDimIndexTypeList> >
{
public:
    typedef BinarySummation_t<typename IndexRenamer_e::Eval_f<LeftOperand>::T,
                              typename IndexRenamer_e::Eval_f<RightOperand>::T,
                              typename IndexRenamer_e::Eval_f<FreeDimIndexTypeList>::T,
                              typename IndexRenamer_e::Eval_f<SummedDimIndexTypeList>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename BundleAbstractIndexTypeList, typename ResultingFactorType, typename ResultingAbstractIndexType>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<IndexBundle_t<Operand,BundleAbstractIndexTypeList,ResultingFactorType,ResultingAbstractIndexType> >
{
public:
    typedef IndexBundle_t<typename IndexRenamer_e::Eval_f<Operand>::T,
                          typename IndexRenamer_e::Eval_f<BundleAbstractIndexTypeList>::T,
                          typename IndexRenamer_e::Eval_f<ResultingFactorType>::T,
                          typename IndexRenamer_e::Eval_f<ResultingAbstractIndexType>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTypeList>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTypeList> >
{
public:
    typedef IndexSplitter_t<typename IndexRenamer_e::Eval_f<Operand>::T,
                            typename IndexRenamer_e::Eval_f<SourceAbstractIndexType>::T,
                            typename IndexRenamer_e::Eval_f<SplitAbstractIndexTypeList>::T> T;
};

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType> >
{
public:
    typedef IndexSplitToIndex_t<typename IndexRenamer_e::Eval_f<Operand>::T,
                                typename IndexRenamer_e::Eval_f<SourceAbstractIndexType>::T,
                                typename IndexRenamer_e::Eval_f<SplitAbstractIndexType>::T> T;
};

// ///////////////////////////////////////////////////////////////////////////
// IndexRenamer_e<...>::Eval_f specialization for ExpressionTemplate_Eval_t
// ///////////////////////////////////////////////////////////////////////////

template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename Operand>
struct IndexRenamer_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<ExpressionTemplate_Eval_t<Operand> >
{
public:
    typedef ExpressionTemplate_Eval_t<typename IndexRenamer_e::Eval_f<Operand>::T> T;
};

} // end of namespace Tenh

#endif // TENH_EXPRESSIONTEMPLATE_INDEXRENAMER_HPP_
