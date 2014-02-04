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

template <typename ExpressionTemplate_, typename FreeDimIndexTyple_ = typename ExpressionTemplate_::FreeDimIndexTyple>
struct Reindexable_t : public ExpressionOperand_i<Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_>,Length_f<FreeDimIndexTyple_>::V>
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<ExpressionTemplate_>::V, MUST_BE_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM((Hippo::EachTypeSatisfies_f<FreeDimIndexTyple_,IsDimIndex_e>::V), MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES),
        STATIC_ASSERT_IN_ENUM((TypesAreEqual_f<typename ExpressionTemplate_::FreeDimIndexTyple,FreeDimIndexTyple_>::V), TYPES_MUST_BE_EQUAL)
    };

    static_assert(IsExpressionTemplate_f<ExpressionTemplate_>::V, "The type argument to Reindexable must be an ExpressionTemplate.");
    static_assert((EachTypeSatisfies_f<FreeDimIndexTypeList_,IsDimIndex_p>::V), "The FreeDimIndexTypeList in Reindexable must only contain DimIndexes.");
    static_assert((TypesAreEqual_f<typename ExpressionTemplate_::FreeDimIndexTypeList,FreeDimIndexTypeList_>::V), "The FreeDimIndexTypeList in Reindexable must be the FreeDimIndexTypeList of the ExpressionTemplate.");

    typedef typename ExpressionTemplate_::Derived DerivedExpressionTemplate;
    typedef typename ExpressionTemplate_::MultiIndex MultiIndex;
    typedef typename ExpressionTemplate_::Scalar Scalar;

    Reindexable_t (ExpressionTemplate_ const &e) : m_derived_expression_template(e.as_derived()) { }

    DerivedExpressionTemplate const &derived_expression_template () const { return m_derived_expression_template; }

    operator Scalar () const
    {
        static_assert(Hippo::Length_f<typename ExpressionTemplate_::FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return Scalar(m_derived_expression_template);
    }
    Scalar operator [] (MultiIndex const &m) const { return m_derived_expression_template[m]; }

    // for when there are no free indices
    DerivedExpressionTemplate const &operator () (Typle_t<> const &) const
    {
        static_assert(Hippo::Length_f<typename ExpressionTemplate_::FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return m_derived_expression_template;
    }

    template <AbstractIndexSymbol SYMBOL_>
    typename Reindex_e<typename AbstractIndicesOfDimIndexTyple_f<typename ExpressionTemplate_::FreeDimIndexTyple>::T,
                       Typle_t<AbstractIndex_c<SYMBOL_>>>
             ::template Eval_f<DerivedExpressionTemplate>::T
        operator () (AbstractIndex_c<SYMBOL_> const &) const
    {
        typedef typename ExpressionTemplate_::FreeDimIndexTyple FreeDimIndexTyple;
        STATIC_ASSERT(Hippo::Length_f<FreeDimIndexTyple>::V == 1, LENGTH_MUST_BE_EXACTLY_1);
        typedef typename AbstractIndicesOfDimIndexTyple_f<FreeDimIndexTyple>::T DomainAbstractIndexTyple;
        typedef Typle_t<AbstractIndex_c<SYMBOL_>> CodomainAbstractIndexTyple;
        return reindexed<DomainAbstractIndexTyple,CodomainAbstractIndexTyple>(m_derived_expression_template);
    }

    template <typename... AbstractIndexTypes_>
    typename Reindex_e<typename AbstractIndicesOfDimIndexTyple_f<typename ExpressionTemplate_::FreeDimIndexTyple>::T,
                       Typle_t<AbstractIndexTypes_...>>
             ::template Eval_f<DerivedExpressionTemplate>::T
        operator () (Typle_t<AbstractIndexTypes_...> const &) const
    {
        typedef typename ExpressionTemplate_::FreeDimIndexTyple FreeDimIndexTyple;
        typedef Typle_t<AbstractIndexTypes_...> AbstractIndexTyple;
        STATIC_ASSERT(Hippo::Length_f<FreeDimIndexTyple>::V == Hippo::Length_f<AbstractIndexTyple>::V, LENGTHS_MUST_BE_EQUAL);
        typedef typename AbstractIndicesOfDimIndexTyple_f<FreeDimIndexTyple>::T DomainAbstractIndexTyple;
        typedef Typle_t<AbstractIndexTypes_...> CodomainAbstractIndexTyple;
        return reindexed<DomainAbstractIndexTyple,CodomainAbstractIndexTyple>(m_derived_expression_template);
    }

    static std::string type_as_string ()
    {
        return "Reindexable_t<" + type_string_of<ExpressionTemplate_>() + ',' + type_string_of<FreeDimIndexTyple_>() + '>';
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
template <typename DomainAbstractIndexTyple_, typename CodomainAbstractIndexTyple_>
template <typename ExpressionTemplate_, typename FreeDimIndexTyple_>
struct Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
    ::Eval_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> >
{
private:
    typedef Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_> Reindex;
    Eval_f();
public:
    typedef Reindexable_t<typename Reindex::template Eval_f<ExpressionTemplate_>::T,
                          typename Reindex::template Eval_f<FreeDimIndexTyple_>::T> T;
};

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename ExpressionTemplate_,
          typename FreeDimIndexTyple_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> >::T
    reindexed (Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> const &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> >::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.derived_expression_template()));
}

template <typename DomainAbstractIndexTyple_,
          typename CodomainAbstractIndexTyple_,
          typename ExpressionTemplate_,
          typename FreeDimIndexTyple_>
typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
         ::template Eval_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> >::T
    reindexed (Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> &e)
{
    typedef typename Reindex_e<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>
                     ::template Eval_f<Reindexable_t<ExpressionTemplate_,FreeDimIndexTyple_> >::T Reindexed;
    return Reindexed(reindexed<DomainAbstractIndexTyple_,CodomainAbstractIndexTyple_>(e.derived_expression_template()));
}

} // end of namespace Tenh

#endif // TENH_BASIC_REINDEXABLE_HPP_
