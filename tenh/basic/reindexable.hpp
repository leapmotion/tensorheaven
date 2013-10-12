// ///////////////////////////////////////////////////////////////////////////
// tenh/basic/reindexable.hpp by Victor Dods, created 2013/10/09
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_BASIC_REINDEXABLE_HPP_
#define TENH_BASIC_REINDEXABLE_HPP_

#include "tenh/core.hpp"

#include "tenh/basic/expressionoperand.hpp"
#include "tenh/dimindex.hpp"
#include "tenh/expressiontemplate_reindex.hpp"
#include "tenh/interface/expressiontemplate.hpp"
#include "tenh/reindex.hpp"

namespace Tenh {

template <typename ExpressionTemplate_, typename FreeDimIndexTypeList_ = typename ExpressionTemplate_::FreeDimIndexTypeList>
struct Reindexable_t : public ExpressionOperand_i<Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_>,Length_f<FreeDimIndexTypeList_>::V>
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<ExpressionTemplate_>::V, MUST_BE_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<FreeDimIndexTypeList_,IsDimIndex_p>::V), MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES),
        STATIC_ASSERT_IN_ENUM((TypesAreEqual_f<typename ExpressionTemplate_::FreeDimIndexTypeList,FreeDimIndexTypeList_>::V), TYPES_MUST_BE_EQUAL)
    };

public:

    typedef typename ExpressionTemplate_::Derived DerivedExpressionTemplate;
    typedef typename ExpressionTemplate_::MultiIndex MultiIndex;
    typedef typename ExpressionTemplate_::Scalar Scalar;

    Reindexable_t (ExpressionTemplate_ const &e) : m_derived_expression_template(e.as_derived()) { }

    DerivedExpressionTemplate const &derived_expression_template () const { return m_derived_expression_template; }

    operator Scalar () const
    {
        STATIC_ASSERT_TYPELIST_IS_EMPTY(typename ExpressionTemplate_::FreeDimIndexTypeList);
        return Scalar(m_derived_expression_template);
    }
    Scalar operator [] (MultiIndex const &m) const { return m_derived_expression_template[m]; }

    // for when there are no free indices
    DerivedExpressionTemplate const &operator () (EmptyTypeList const &) const
    {
        STATIC_ASSERT_TYPELIST_IS_EMPTY(typename ExpressionTemplate_::FreeDimIndexTypeList);
        return m_derived_expression_template;
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
        typedef TypeList_t<AbstractIndexHeadType_,AbstractIndexBodyTypeList_> AbstractIndexTypeList;
        STATIC_ASSERT(Length_f<FreeDimIndexTypeList>::V == Length_f<AbstractIndexTypeList>::V, LENGTHS_MUST_BE_EQUAL);
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

// an easy way to construct a Reindexable_t from an expression template without having to type a shitty typename
template <typename ExpressionTemplate_>
Reindexable_t<ExpressionTemplate_> reindexable (ExpressionTemplate_ const &e)
{
    return Reindexable_t<ExpressionTemplate_>(e);
}

// specialization for Reindex_e for Reindexable_t types
template <typename DomainAbstractIndexTypeList_, typename CodomainAbstractIndexTypeList_>
template <typename ExpressionTemplate_, typename FreeDimIndexTypeList_>
struct Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
    ::Eval_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_> Reindex;
public:
    typedef Reindexable_t<typename Reindex::template Eval_f<ExpressionTemplate_>::T,
                          typename Reindex::template Eval_f<FreeDimIndexTypeList_>::T> T;
};

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename ExpressionTemplate_,
          typename FreeDimIndexTypeList_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> >::T
    reindexed (Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                     ::template Eval_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> >::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(e.derived_expression_template()));
}

template <typename DomainAbstractIndexTypeList_,
          typename CodomainAbstractIndexTypeList_,
          typename ExpressionTemplate_,
          typename FreeDimIndexTypeList_>
typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
         ::template Eval_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> >::T
    reindexed (Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>
                     ::template Eval_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> >::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTypeList_,CodomainAbstractIndexTypeList_>(e.derived_expression_template()));
}

} // end of namespace Tenh

#endif // TENH_BASIC_REINDEXABLE_HPP_
