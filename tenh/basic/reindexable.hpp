// ///////////////////////////////////////////////////////////////////////////
// tenh/basic/reindexable.hpp by Victor Dods, created 2013/10/09
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_BASIC_REINDEXABLE_HPP_
#define TENH_BASIC_REINDEXABLE_HPP_

#include "tenh/core.hpp"

#include "tenh/dimindex.hpp"
#include "tenh/expressiontemplate_reindex.hpp"
#include "tenh/interface/expressiontemplate.hpp"
#include "tenh/reindex.hpp"

namespace Tenh {

template <typename ExpressionTemplate_, typename FreeDimIndexTypeList_ = typename ExpressionTemplate_::FreeDimIndexTypeList>
struct Reindexable_t
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<ExpressionTemplate_>::V, MUST_BE_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<FreeDimIndexTypeList_,IsDimIndex_p>::V), MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES),
        STATIC_ASSERT_IN_ENUM((TypesAreEqual_f<typename ExpressionTemplate_::FreeDimIndexTypeList,FreeDimIndexTypeList_>::V), TYPES_MUST_BE_EQUAL)
    };
    typedef typename ExpressionTemplate_::Scalar Scalar;

public:

    typedef typename ExpressionTemplate_::Derived DerivedExpressionTemplate;

    Reindexable_t (ExpressionTemplate_ const &e) : m_derived_expression_template(e.as_derived()) { }

    DerivedExpressionTemplate const &as_derived_expression_template () const { return m_derived_expression_template; }

    Scalar operator () (EmptyTypeList const &) const
    {
        STATIC_ASSERT(ExpressionTemplate_::FreeDimIndexTypeList::LENGTH == 0, LENGTH_MUST_BE_EXACTLY_0);
        return Scalar(m_derived_expression_template);
    }

    template <AbstractIndexSymbol SYMBOL_>
    typename Reindex_e<typename AbstractIndicesOfDimIndexTypeList_t<typename ExpressionTemplate_::FreeDimIndexTypeList>::T,
                       TypeList_t<AbstractIndex_c<SYMBOL_> > >
             ::template Eval_f<DerivedExpressionTemplate>::T
        operator () (AbstractIndex_c<SYMBOL_> const &) const
    {
        typedef typename ExpressionTemplate_::FreeDimIndexTypeList FreeDimIndexTypeList;
        STATIC_ASSERT(FreeDimIndexTypeList::LENGTH == 1, LENGTH_MUST_BE_EXACTLY_1);
        typedef typename AbstractIndicesOfDimIndexTypeList_t<FreeDimIndexTypeList>::T DomainAbstractIndexTypeList;
        typedef TypeList_t<AbstractIndex_c<SYMBOL_> > CodomainAbstractIndexTypeList;
        return reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(m_derived_expression_template);
    }

    template <typename AbstractIndexHeadType_, typename AbstractIndexBodyTypeList_>
    typename Reindex_e<typename AbstractIndicesOfDimIndexTypeList_t<typename ExpressionTemplate_::FreeDimIndexTypeList>::T,
                       TypeList_t<AbstractIndexHeadType_,AbstractIndexBodyTypeList_> >
             ::template Eval_f<DerivedExpressionTemplate>::T
        operator () (TypeList_t<AbstractIndexHeadType_,AbstractIndexBodyTypeList_> const &) const
    {
        typedef typename ExpressionTemplate_::FreeDimIndexTypeList FreeDimIndexTypeList;
        STATIC_ASSERT(FreeDimIndexTypeList::LENGTH == 2, LENGTH_MUST_BE_EXACTLY_2);
        typedef typename AbstractIndicesOfDimIndexTypeList_t<FreeDimIndexTypeList>::T DomainAbstractIndexTypeList;
        typedef TypeList_t<AbstractIndexHeadType_,AbstractIndexBodyTypeList_> CodomainAbstractIndexTypeList;
        return reindexed<DomainAbstractIndexTypeList,CodomainAbstractIndexTypeList>(m_derived_expression_template);
    }

    static std::string type_as_string ()
    {
        return "Reindexable_t<" + type_string_of<ExpressionTemplate_>() + ',' + type_string_of<FreeDimIndexTypeList_>() + '>';
    }

private:

    DerivedExpressionTemplate m_derived_expression_template;
};

template <typename Lhs_ExpressionTemplate_,
          typename Lhs_FreeDimIndexTypeList_,
          typename Rhs_ExpressionTemplate_,
          typename Rhs_FreeDimIndexTypeList_>
Reindexable_t<ExpressionTemplate_Addition_t<Lhs_ExpressionTemplate_,Rhs_ExpressionTemplate_,'+'> >
    operator + (Reindexable_t<Lhs_ExpressionTemplate_,Lhs_FreeDimIndexTypeList_> const &lhs,
                Reindexable_t<Rhs_ExpressionTemplate_,Rhs_FreeDimIndexTypeList_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs(i) + rhs(i);
}

template <typename Lhs_ExpressionTemplate_,
          typename Lhs_FreeDimIndexTypeList_,
          typename Rhs_ExpressionTemplate_,
          typename Rhs_FreeDimIndexTypeList_>
Reindexable_t<ExpressionTemplate_Addition_t<Lhs_ExpressionTemplate_,Rhs_ExpressionTemplate_,'-'> >
    operator - (Reindexable_t<Lhs_ExpressionTemplate_,Lhs_FreeDimIndexTypeList_> const &lhs,
                Reindexable_t<Rhs_ExpressionTemplate_,Rhs_FreeDimIndexTypeList_> const &rhs)
{
    AbstractIndex_c<'i'> i;
    return lhs(i) - rhs(i);
}

// template <typename LeftOperand_, typename RightOperand_>
// Reindexable_t<ExpressionTemplate_Addition_t<LeftOperand_,RightOperand_,'-'> >
//     operator - (Reindexable_t<LeftOperand_> const &lhs, Reindexable_t<RightOperand_> const &rhs)
// {
//     AbstractIndex_c<'i'> i;
//     return lhs(i) - rhs(i);
// }

} // end of namespace Tenh

#endif // TENH_BASIC_REINDEXABLE_HPP_
