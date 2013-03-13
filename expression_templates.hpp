#ifndef EXPRESSION_TEMPLATES_HPP_
#define EXPRESSION_TEMPLATES_HPP_

#include "core.hpp"
#include "compoundindex.hpp"
#include "typelist.hpp"

template <typename Operand, typename IndexType>
struct ExpressionTemplate_IndexAsVector_t
{
    typedef typename Operand::Scalar Scalar;

    typedef TypeList_t<IndexType> FreeIndexTypeList;
    typedef EmptyTypeList SummedIndexTypeList;
    typedef CompoundIndex_t<FreeIndexTypeList> Index;

    ExpressionTemplate_IndexAsVector_t (Operand const &operand) : m_operand(operand) { }

    // read-only, because it doesn't necessarily make sense to assign to an expression
    // template -- the expression may be a product or some such, where each component
    // is not an L-value.
    Scalar operator [] (IndexType const &i) const { return m_operand[i]; }

private:

    Operand const &m_operand;
};

template <typename Operand, typename F1IndexType, typename F2IndexType>
struct ExpressionTemplate_IndexAsTensor2_t
{
    typedef typename Operand::Scalar Scalar;

    typedef TypeList_t<F1IndexType, TypeList_t<F2IndexType> > FreeIndexTypeList;
    typedef EmptyTypeList SummedIndexTypeList;
    typedef CompoundIndex_t<FreeIndexTypeList> Index;

    ExpressionTemplate_IndexAsTensor2_t (Operand const &operand) : m_operand(operand) { }

    // read-only, because it doesn't necessarily make sense to assign to an expression
    // template -- the expression may be a product or some such, where each component
    // is not an L-value.
    Scalar operator [] (Index const &i) const { return m_operand[i.value()]; }

private:

    Operand const &m_operand;
};

template <typename LeftOperand, typename RightOperand>
struct ExpressionTemplate_Addition_t
{
    // TODO: check that the summed indices from each operand have no indices in common
    // though technically this is unnecessary, because the summed indices are "private"
    // to each contraction, so this is really for the human's benefit, not getting
    // confused by multiple repeated indices that have nothing to do with each other.
    // NOTE: technically this check is already done inside CompoundIndex_t, but it would
    // be good to do the check here so that an error will be more obvious.
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v &&
               Lvd::Meta::Assert<AreEqualAsSets_t<typename LeftOperand::FreeIndexTypeList,typename RightOperand::FreeIndexTypeList>::V>::v &&
               Lvd::Meta::Assert<!ContainsDuplicates_t<typename LeftOperand::FreeIndexTypeList>::V>::v && 
               Lvd::Meta::Assert<!ContainsDuplicates_t<typename RightOperand::FreeIndexTypeList>::V>::v };

    typedef typename LeftOperand::Scalar Scalar;
    typedef typename LeftOperand::FreeIndexTypeList FreeIndexTypeList;
    typedef EmptyTypeList SummedIndexTypeList; // TEMP: see above comment about "private" indices
    typedef typename LeftOperand::Index Index;

    ExpressionTemplate_Addition_t (LeftOperand const &left_operand, RightOperand const &right_operand)
        :
        m_left_operand(left_operand),
        m_right_operand(right_operand)
    { }

    Scalar operator [] (Index const &i) const 
    { 
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
        return m_left_operand[left_operand_index_map(i)] + m_right_operand[right_operand_index_map(i)];
    }

private:

    LeftOperand const &m_left_operand;
    RightOperand const &m_right_operand;
};

template <typename LeftOperand, typename RightOperand, typename FreeIndexTypeList, typename SummedIndexTypeList>
struct Summation_t
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v &&
               Lvd::Meta::Assert<(SummedIndexTypeList::LENGTH > 0)>::v };

    typedef typename LeftOperand::Scalar Scalar;

    static Scalar eval (LeftOperand const &left_operand, RightOperand const &right_operand, CompoundIndex_t<FreeIndexTypeList> const &i)
    {
        typedef typename ConcatenationOfTypeLists_t<FreeIndexTypeList,SummedIndexTypeList>::T TotalIndexTypeList;
        typedef CompoundIndex_t<TotalIndexTypeList> TotalIndex;
        typedef CompoundIndex_t<SummedIndexTypeList> SummedIndex;

        // the operands take indices that are a subset of the summed indices and free indices.

        // constructing t with i initializes the first elements which correpond to
        // Index with the value of i, and initializes the remaining elements to zero.
        TotalIndex t(i);
        Scalar retval(0);
        // get the map which produces the CompoundIndex for each operand from the TotalIndex t
        typedef CompoundIndexMap_t<TotalIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef CompoundIndexMap_t<TotalIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
        // t = (f,s), which is a concatenation of the free access indices and the summed access indices.
        // s is a reference to the second part, which is what is iterated over in the summation.
        for (SummedIndex &s = t.template trailing_list<FreeIndexTypeList::LENGTH>(); s.is_not_at_end(); ++s)
            retval += left_operand[left_operand_index_map(t)] * right_operand[right_operand_index_map(t)];
        return retval;
    }
};

// template specialization handles summation over no indices
template <typename LeftOperand, typename RightOperand, typename FreeIndexTypeList>
struct Summation_t<LeftOperand,RightOperand,FreeIndexTypeList,EmptyTypeList>
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v };

    typedef typename LeftOperand::Scalar Scalar;

    static Scalar eval (LeftOperand const &left_operand, RightOperand const &right_operand, CompoundIndex_t<FreeIndexTypeList> const &i)
    {
        // get the map which produces the CompoundIndex for each operand from the free indices i
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
        return left_operand[left_operand_index_map(i)] * right_operand[right_operand_index_map(i)];
    }
};

// TODO: there is an issue to think about: while it is totally valid to do
// u(i)*v(j)*w(j) (this is an outer product contracted with a vector), the
// expression v(j)*w(j) can be computed first and factored out of the whole
// thing, instead of needing to be multiplied out for each access of the i index.
// this may be somewhat difficult to do, as it would require searching the
// expression template AST for such contractions and restructuring the AST.
template <typename LeftOperand, typename RightOperand>
struct ExpressionTemplate_Multiplication_t
{
    // TODO: check that the summed indices from each operand have no indices in common
    // though technically this is unnecessary, because the summed indices are "private"
    // to each contraction, so this is really for the human's benefit, not getting
    // confused by multiple repeated indices that have nothing to do with each other.
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v };
    // TODO: ensure there are no indices that occur 3+ times (?)

    typedef typename LeftOperand::Scalar Scalar;
    // the free indices are the single-occurrence indices of the concatenated
    // list of free indices from the left and right operands
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeIndexTypeList,
                                                typename RightOperand::FreeIndexTypeList>::T CombinedFreeIndexTypeList;
    typedef typename ElementsHavingMultiplicity_t<CombinedFreeIndexTypeList,1>::T FreeIndexTypeList;
    // the summed indices (at this level) are the double-occurrences indices
    // of the concatenated list of free indices from the left and right operands
    typedef typename ElementsHavingMultiplicity_t<CombinedFreeIndexTypeList,2>::T SummedIndexTypeList;
    // Index is a list (TODO: change to tuple) of type FreeIndices
    typedef CompoundIndex_t<FreeIndexTypeList> Index;

    ExpressionTemplate_Multiplication_t (LeftOperand const &left_operand, RightOperand const &right_operand)
        :
        m_left_operand(left_operand),
        m_right_operand(right_operand)
    { }

    Scalar operator [] (Index const &i) const
    {
        return Summation_t<LeftOperand,RightOperand,FreeIndexTypeList,SummedIndexTypeList>::eval(m_left_operand, m_right_operand, i);
    }

private:

    LeftOperand const &m_left_operand;
    RightOperand const &m_right_operand;
};


#endif // EXPRESSION_TEMPLATES_HPP_
