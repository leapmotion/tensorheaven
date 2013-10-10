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

    operator Scalar () const
    {
        STATIC_ASSERT(ExpressionTemplate_::FreeDimIndexTypeList::LENGTH == 0, LENGTH_MUST_BE_EXACTLY_0);
        return Scalar(m_derived_expression_template);
    }

    // for when there are no free indices
    Scalar operator () (EmptyTypeList const &) const
    {
        typedef typename ExpressionTemplate_::FreeDimIndexTypeList FreeDimIndexTypeList;
        STATIC_ASSERT(FreeDimIndexTypeList::LENGTH == 0, LENGTH_MUST_BE_EXACTLY_0);
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

// ///////////////////////////////////////////////////////////////////////////
// operator overloads involving only Reindexable_t types
// ///////////////////////////////////////////////////////////////////////////

// ///////////////////////////////////////////////////////////////////////////
// addition
// ///////////////////////////////////////////////////////////////////////////

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

// ///////////////////////////////////////////////////////////////////////////
// subtraction
// ///////////////////////////////////////////////////////////////////////////

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

// ///////////////////////////////////////////////////////////////////////////
// composition
// ///////////////////////////////////////////////////////////////////////////

// metafunctions for creating the correct abstract multiindices for contraction

template <Uint32 FREE_INDEX_COUNT> struct LhsAbstractIndicesForContraction_f;

template <>
struct LhsAbstractIndicesForContraction_f<0>
{
    typedef EmptyTypeList T;
};

template <>
struct LhsAbstractIndicesForContraction_f<1>
{
    typedef AbstractIndex_c<'j'> T;
};

template <>
struct LhsAbstractIndicesForContraction_f<2>
{
    typedef TypeList_t<AbstractIndex_c<'i'>,
            TypeList_t<AbstractIndex_c<'j'> > > T;
};

template <Uint32 FREE_INDEX_COUNT> struct RhsAbstractIndicesForContraction_f;

template <>
struct RhsAbstractIndicesForContraction_f<0>
{
    typedef EmptyTypeList T;
};

template <>
struct RhsAbstractIndicesForContraction_f<1>
{
    typedef AbstractIndex_c<'j'> T;
};

template <>
struct RhsAbstractIndicesForContraction_f<2>
{
    typedef TypeList_t<AbstractIndex_c<'j'>,
            TypeList_t<AbstractIndex_c<'k'> > > T;
};

// metafunction for determining the return type of a composition (could be
// Reindexable_t<ExpressionTemplate_Multiplication_t<...> > or
// Reindexable_t<ExpressionTemplate_ScalarMultiplication_t<...> >)

template <typename Lhs_ExpressionTemplate_,
          Uint32 LHS_FREE_INDEX_COUNT,
          typename Rhs_ExpressionTemplate_,
          Uint32 RHS_FREE_INDEX_COUNT>
struct ReindexableContractionReturnType_f
{
    typedef Reindexable_t<ExpressionTemplate_Multiplication_t<Lhs_ExpressionTemplate_,Rhs_ExpressionTemplate_> > T;
};

template <typename Lhs_ExpressionTemplate_,
          typename Rhs_ExpressionTemplate_,
          Uint32 RHS_FREE_INDEX_COUNT>
struct ReindexableContractionReturnType_f<Lhs_ExpressionTemplate_,
                                          0,
                                          Rhs_ExpressionTemplate_,
                                          RHS_FREE_INDEX_COUNT>
{
    typedef Reindexable_t<ExpressionTemplate_ScalarMultiplication_t<Rhs_ExpressionTemplate_,
                                                                    typename Rhs_ExpressionTemplate_::Scalar,
                                                                    '*'> > T;
};

template <typename Lhs_ExpressionTemplate_,
          Uint32 LHS_FREE_INDEX_COUNT,
          typename Rhs_ExpressionTemplate_>
struct ReindexableContractionReturnType_f<Lhs_ExpressionTemplate_,
                                          LHS_FREE_INDEX_COUNT,
                                          Rhs_ExpressionTemplate_,
                                          0>
{
    typedef Reindexable_t<ExpressionTemplate_ScalarMultiplication_t<Lhs_ExpressionTemplate_,
                                                                    typename Lhs_ExpressionTemplate_::Scalar,
                                                                    '*'> > T;
};

template <typename Lhs_ExpressionTemplate_,
          typename Rhs_ExpressionTemplate_>
struct ReindexableContractionReturnType_f<Lhs_ExpressionTemplate_,
                                          0,
                                          Rhs_ExpressionTemplate_,
                                          0>
{
    typedef typename Lhs_ExpressionTemplate_::Scalar T;
};

template <typename Lhs_ExpressionTemplate_,
          typename Rhs_ExpressionTemplate_>
struct ReindexableContractionReturnType_f<Lhs_ExpressionTemplate_,
                                          1,
                                          Rhs_ExpressionTemplate_,
                                          1>
{
    typedef typename Lhs_ExpressionTemplate_::Scalar T;
};

// this handles scalar multiplication and non-scalar composition

template <typename Lhs_ExpressionTemplate_,
          typename Lhs_FreeDimIndexTypeList_,
          typename Rhs_ExpressionTemplate_,
          typename Rhs_FreeDimIndexTypeList_>
typename ReindexableContractionReturnType_f<Lhs_ExpressionTemplate_,
                                            Lhs_FreeDimIndexTypeList_::LENGTH,
                                            Rhs_ExpressionTemplate_,
                                            Rhs_FreeDimIndexTypeList_::LENGTH>::T
    operator * (Reindexable_t<Lhs_ExpressionTemplate_,Lhs_FreeDimIndexTypeList_> const &lhs,
                Reindexable_t<Rhs_ExpressionTemplate_,Rhs_FreeDimIndexTypeList_> const &rhs)
{
    typedef typename LhsAbstractIndicesForContraction_f<Lhs_FreeDimIndexTypeList_::LENGTH>::T LhsIndices;
    typedef typename RhsAbstractIndicesForContraction_f<Rhs_FreeDimIndexTypeList_::LENGTH>::T RhsIndices;
    return lhs(LhsIndices()) * rhs(RhsIndices());
}

// ///////////////////////////////////////////////////////////////////////////
// negation
// ///////////////////////////////////////////////////////////////////////////

template <typename ExpressionTemplate_,
          typename FreeDimIndexTypeList_>
Reindexable_t<ExpressionTemplate_ScalarMultiplication_t<ExpressionTemplate_,typename ExpressionTemplate_::Scalar,'*'> >
    operator - (Reindexable_t<ExpressionTemplate_,FreeDimIndexTypeList_> const &rhs)
{
    return -rhs.as_derived_expression_template();
}

} // end of namespace Tenh

#endif // TENH_BASIC_REINDEXABLE_HPP_
