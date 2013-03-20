#ifndef EXPRESSION_TEMPLATES_HPP_
#define EXPRESSION_TEMPLATES_HPP_

#include "core.hpp"
#include "compoundindex.hpp"
#include "naturalpairing.hpp"
#include "typelist.hpp"

// ////////////////////////////////////////////////////////////////////////////
// summation over repeated indices
// ////////////////////////////////////////////////////////////////////////////

template <typename IndexTypeList>
struct SummedIndexTypeList_t
{
    typedef typename ElementsHavingMultiplicity_t<IndexTypeList,2>::T T;
};

template <typename IndexTypeList>
struct FreeIndexTypeList_t
{
    typedef typename ElementsHavingMultiplicity_t<IndexTypeList,1>::T T;
};

template <typename HeadType>
typename HeadType::OwnerVector::Scalar summation_component_factor (CompoundIndex_t<TypeList_t<HeadType> > const &s)
{
    return NaturalPairing_t<typename HeadType::OwnerVector>::component(s.head());
}

template <typename HeadType, typename BodyTypeList>
typename HeadType::OwnerVector::Scalar summation_component_factor (CompoundIndex_t<TypeList_t<HeadType,BodyTypeList> > const &s)
{
    return NaturalPairing_t<typename HeadType::OwnerVector>::component(s.head()) * summation_component_factor(s.body());
}

// TODO: think about how UnarySummation_t and BinarySummation_t could be combined (if it makes sense to do it)

// this is designed to handle trace-type expression templates, such as u(i,i) or v(i,j,i)
// technically SummedIndexTypeList is a redundant argument (as it is derivable from TensorIndexTypeList),
// but it is necessary so that a template specialization can be made for when it is EmptyTypeList.
template <typename Tensor, typename TensorIndexTypeList, typename SummedIndexTypeList>
struct UnarySummation_t
{
    typedef typename Tensor::Scalar Scalar;
    typedef typename FreeIndexTypeList_t<TensorIndexTypeList>::T FreeIndexTypeList;
    typedef CompoundIndex_t<FreeIndexTypeList> CompoundIndex;

    static Scalar eval (Tensor const &tensor, CompoundIndex const &c)
    {
        typedef typename ConcatenationOfTypeLists_t<FreeIndexTypeList,SummedIndexTypeList>::T TotalIndexTypeList;
        typedef CompoundIndex_t<TotalIndexTypeList> TotalIndex;
        typedef CompoundIndex_t<SummedIndexTypeList> SummedIndex;

        // the operands take indices that are a subset of the summed indices and free indices.

        // constructing t with c initializes the first elements which correpond to
        // CompoundIndex with the value of c, and initializes the remaining elements to zero.
        TotalIndex t(c);
        Scalar retval(0);
        // get the map which produces the CompoundIndex for each tensor from the TotalIndex t
        typedef CompoundIndexMap_t<TotalIndexTypeList,TensorIndexTypeList> TensorIndexMap;
        typename TensorIndexMap::EvalMapType tensor_index_map = TensorIndexMap::eval;
        // t = (f,s), which is a concatenation of the free access indices and the summed access indices.
        // s is a reference to the second part, which is what is iterated over in the summation.
        for (SummedIndex &s = t.template trailing_list<FreeIndexTypeList::LENGTH>(); s.is_not_at_end(); ++s)
            retval += tensor[tensor_index_map(t)] * summation_component_factor(s);
        return retval;
    }
};

template <typename Tensor, typename TensorIndexTypeList>
struct UnarySummation_t<Tensor,TensorIndexTypeList,EmptyTypeList>
{
    typedef typename Tensor::Scalar Scalar;
    typedef typename FreeIndexTypeList_t<TensorIndexTypeList>::T FreeIndexTypeList;
    typedef CompoundIndex_t<FreeIndexTypeList> CompoundIndex;

    static Scalar eval (Tensor const &tensor, CompoundIndex const &c) { return tensor[c]; }
};

// ////////////////////////////////////////////////////////////////////////////
// expression-template-generation (making ETs from vectors/tensors)
// ////////////////////////////////////////////////////////////////////////////

// this is the "const" version of an indexed tensor expression (it has summed indices, so it doesn't make sense to assign to it)
template <typename Tensor, typename TensorIndexTypeList, typename SummedIndexTypeList>
struct ExpressionTemplate_IndexedTensor_t
{
    // TODO: some consistency checks on TensorIndexTypeList and SummedIndexTypeList

    typedef typename Tensor::Scalar Scalar;
    typedef typename FreeIndexTypeList_t<TensorIndexTypeList>::T FreeIndexTypeList;
    // typelist of used indices which are prohibited from using higher up in the AST
    typedef SummedIndexTypeList UsedIndexTypeList;
    typedef CompoundIndex_t<FreeIndexTypeList> CompoundIndex;

    static bool const IS_EXPRESSION_TEMPLATE = true;

    ExpressionTemplate_IndexedTensor_t (Tensor const &tensor) : m_tensor(tensor) { }

    // read-only, because it doesn't make sense to assign to an expression which is a summation.
    Scalar operator [] (CompoundIndex const &c) const
    {
        return UnarySummation_t<Tensor,TensorIndexTypeList,SummedIndexTypeList>::eval(m_tensor, c);
    }

    template <typename OtherTensor>
    bool uses_tensor (OtherTensor const &t) const
    {
        // the reinterpret_cast is safe because we're dealing with POD types and there
        // is an explicit type-check at compiletime (TypesAreEqual)
        return Lvd::Meta::TypesAreEqual<OtherTensor,Tensor>::v && reinterpret_cast<Tensor const *>(&t) == &m_tensor;
    }

private:

    Tensor const &m_tensor;
};

// this is the "non-const" version of an indexed tensor expression (it has no summed indices, so it makes sense to assign to it)
template <typename Tensor, typename TensorIndexTypeList>
struct ExpressionTemplate_IndexedTensor_t<Tensor,TensorIndexTypeList,EmptyTypeList>
{
    typedef typename Tensor::Scalar Scalar;
    typedef typename FreeIndexTypeList_t<TensorIndexTypeList>::T FreeIndexTypeList;
    // typelist of used indices which are prohibited from using higher up in the AST
    typedef EmptyTypeList UsedIndexTypeList;
    typedef CompoundIndex_t<FreeIndexTypeList> CompoundIndex;

    static bool const IS_EXPRESSION_TEMPLATE = true;

    ExpressionTemplate_IndexedTensor_t (Tensor &tensor) : m_tensor(tensor) { }

    // read-only, because it doesn't necessarily make sense to assign to an expression
    // template -- the expression may be a product or some such, where each component
    // is not an L-value.
    Scalar const &operator [] (typename Tensor::Index const &i) const { return m_tensor[i]; }
    Scalar const &operator [] (CompoundIndex const &c) const { return m_tensor[c]; }

    // for some dumb reason, the compiler needed a non-templatized assignment operator for the exact matching type
    void operator = (ExpressionTemplate_IndexedTensor_t const &right_operand)
    {
        // if right and left operands' m_tensor references are the same, this is a no-op
        if (&right_operand.m_tensor == &m_tensor)
            return;

        // TODO: replace with memcpy? (this would require that Scalar is a POD type)
        for (typename Tensor::Index i; i.is_not_at_end(); ++i)
            m_tensor[i] = right_operand[i];
    }
    template <typename RightOperand>
    void operator = (RightOperand const &right_operand)
    {
        enum
        {
            RIGHT_OPERAND_IS_EXPRESSION_TEMPLATE        = Lvd::Meta::Assert<RightOperand::IS_EXPRESSION_TEMPLATE>::v,
            OPERAND_SCALAR_TYPES_ARE_EQUAL              = Lvd::Meta::Assert<(Lvd::Meta::TypesAreEqual<Scalar,typename RightOperand::Scalar>::v)>::v,
            OPERANDS_HAVE_SAME_FREE_INDICES             = Lvd::Meta::Assert<AreEqualAsSets_t<FreeIndexTypeList,typename RightOperand::FreeIndexTypeList>::V>::v,
            LEFT_OPERAND_HAS_NO_DUPLICATE_FREE_INDICES  = Lvd::Meta::Assert<!ContainsDuplicates_t<FreeIndexTypeList>::V>::v,
            RIGHT_OPERAND_HAS_NO_DUPLICATE_FREE_INDICES = Lvd::Meta::Assert<!ContainsDuplicates_t<typename RightOperand::FreeIndexTypeList>::V>::v
        };

        // check for aliasing (where source and destination memory overlap)
        if (right_operand.uses_tensor(m_tensor))
            throw std::invalid_argument("invalid aliased tensor assignment (source and destination memory overlap) -- use an intermediate value");

        typedef CompoundIndexMap_t<FreeIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;

        // component-wise assignment via the free index type.
        for (CompoundIndex c; c.is_not_at_end(); ++c)
            m_tensor[c] = right_operand[right_operand_index_map(c)];
    }

    template <typename OtherTensor>
    bool uses_tensor (OtherTensor const &t) const
    {
        // the reinterpret_cast is safe because we're dealing with POD types and there
        // is an explicit type-check at compiletime (TypesAreEqual)
        return Lvd::Meta::TypesAreEqual<OtherTensor,Tensor>::v && reinterpret_cast<Tensor const *>(&t) == &m_tensor;
    }

private:

    Tensor &m_tensor;
};

// ////////////////////////////////////////////////////////////////////////////
// addition of expression templates
// ////////////////////////////////////////////////////////////////////////////

template <typename LeftOperand, typename RightOperand>
struct ExpressionTemplate_Addition_t
{
    // TODO: check that the summed indices from each operand have no indices in common
    // though technically this is unnecessary, because the summed indices are "private"
    // to each contraction, so this is really for the human's benefit, not getting
    // confused by multiple repeated indices that have nothing to do with each other.
    // NOTE: technically this check is already done inside CompoundIndex_t, but it would
    // be good to do the check here so that an error will be more obvious.
    enum
    {
        LEFT_OPERAND_IS_EXPRESSION_TEMPLATE         = Lvd::Meta::Assert<LeftOperand::IS_EXPRESSION_TEMPLATE>::v,
        RIGHT_OPERAND_IS_EXPRESSION_TEMPLATE        = Lvd::Meta::Assert<RightOperand::IS_EXPRESSION_TEMPLATE>::v,
        OPERAND_SCALAR_TYPES_ARE_EQUAL              = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v,
        OPERANDS_HAVE_SAME_FREE_INDICES             = Lvd::Meta::Assert<AreEqualAsSets_t<typename LeftOperand::FreeIndexTypeList,typename RightOperand::FreeIndexTypeList>::V>::v,
        LEFT_OPERAND_HAS_NO_DUPLICATE_FREE_INDICES  = Lvd::Meta::Assert<!ContainsDuplicates_t<typename LeftOperand::FreeIndexTypeList>::V>::v,
        RIGHT_OPERAND_HAS_NO_DUPLICATE_FREE_INDICES = Lvd::Meta::Assert<!ContainsDuplicates_t<typename RightOperand::FreeIndexTypeList>::V>::v
    };

    typedef typename LeftOperand::Scalar Scalar;
    typedef typename LeftOperand::FreeIndexTypeList FreeIndexTypeList;
    typedef EmptyTypeList SummedIndexTypeList; // TEMP: see above comment about "private" indices
    typedef EmptyTypeList UsedIndexTypeList;
    typedef typename LeftOperand::CompoundIndex CompoundIndex;

    static bool const IS_EXPRESSION_TEMPLATE = true;

    ExpressionTemplate_Addition_t (LeftOperand const &left_operand, RightOperand const &right_operand)
        :
        m_left_operand(left_operand),
        m_right_operand(right_operand)
    { }

    Scalar operator [] (CompoundIndex const &c) const
    {
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
        return m_left_operand[left_operand_index_map(c)] + m_right_operand[right_operand_index_map(c)];
    }

    template <typename OtherTensor>
    bool uses_tensor (OtherTensor const &t) const
    {
        return m_left_operand.uses_tensor(t) || m_right_operand.uses_tensor(t);
    }

private:

    LeftOperand const &m_left_operand;
    RightOperand const &m_right_operand;
};

template <typename LeftOperand,
          typename RightOperand>
ExpressionTemplate_Addition_t<LeftOperand,RightOperand>
    operator + (LeftOperand const &left_operand,
                RightOperand const &right_operand)
{
    return ExpressionTemplate_Addition_t<LeftOperand,RightOperand>(left_operand, right_operand);
}

// ////////////////////////////////////////////////////////////////////////////
// multiplication of expression templates (tensor product and contraction)
// ////////////////////////////////////////////////////////////////////////////

template <typename LeftOperand, typename RightOperand, typename FreeIndexTypeList, typename SummedIndexTypeList>
struct BinarySummation_t
{
    enum { _ = Lvd::Meta::Assert<LeftOperand::IS_EXPRESSION_TEMPLATE>::v &&
               Lvd::Meta::Assert<RightOperand::IS_EXPRESSION_TEMPLATE>::v &&
               Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v &&
               Lvd::Meta::Assert<(SummedIndexTypeList::LENGTH > 0)>::v };

    typedef typename LeftOperand::Scalar Scalar;
    typedef CompoundIndex_t<FreeIndexTypeList> CompoundIndex;

    static Scalar eval (LeftOperand const &left_operand, RightOperand const &right_operand, CompoundIndex const &c)
    {
        typedef typename ConcatenationOfTypeLists_t<FreeIndexTypeList,SummedIndexTypeList>::T TotalIndexTypeList;
        typedef CompoundIndex_t<TotalIndexTypeList> TotalIndex;
        typedef CompoundIndex_t<SummedIndexTypeList> SummedIndex;

        // the operands take indices that are a subset of the summed indices and free indices.

        // constructing t with c initializes the first elements which correpond to
        // CompoundIndex with the value of c, and initializes the remaining elements to zero.
        TotalIndex t(c);
        Scalar retval(0);
        // get the map which produces the CompoundIndex for each operand from the TotalIndex t
        typedef CompoundIndexMap_t<TotalIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef CompoundIndexMap_t<TotalIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
        // t = (f,s), which is a concatenation of the free access indices and the summed access indices.
        // s is a reference to the second part, which is what is iterated over in the summation.
        for (SummedIndex &s = t.template trailing_list<FreeIndexTypeList::LENGTH>(); s.is_not_at_end(); ++s)
            retval += left_operand[left_operand_index_map(t)] *
                      right_operand[right_operand_index_map(t)] *
                      summation_component_factor(s);
        return retval;
    }
};

// template specialization handles summation over no indices
template <typename LeftOperand, typename RightOperand, typename FreeIndexTypeList>
struct BinarySummation_t<LeftOperand,RightOperand,FreeIndexTypeList,EmptyTypeList>
{
    enum { _ = Lvd::Meta::Assert<LeftOperand::IS_EXPRESSION_TEMPLATE>::v &&
               Lvd::Meta::Assert<RightOperand::IS_EXPRESSION_TEMPLATE>::v &&
               Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v };

    typedef typename LeftOperand::Scalar Scalar;
    typedef CompoundIndex_t<FreeIndexTypeList> CompoundIndex;

    static Scalar eval (LeftOperand const &left_operand, RightOperand const &right_operand, CompoundIndex const &c)
    {
        // get the map which produces the CompoundIndex for each operand from the free indices c
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
        return left_operand[left_operand_index_map(c)] * right_operand[right_operand_index_map(c)];
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
    typedef typename LeftOperand::Scalar Scalar;
    // the free indices are the single-occurrence indices of the concatenated
    // list of free indices from the left and right operands
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeIndexTypeList,
                                                typename RightOperand::FreeIndexTypeList>::T CombinedFreeIndexTypeList;
    typedef typename ElementsHavingMultiplicity_t<CombinedFreeIndexTypeList,1>::T FreeIndexTypeList;
    // the summed indices (at this level) are the double-occurrences indices
    // of the concatenated list of free indices from the left and right operands
    typedef typename ElementsHavingMultiplicity_t<CombinedFreeIndexTypeList,2>::T SummedIndexTypeList;
    // typelist of used indices which are prohibited from using higher up in the AST
    typedef typename UniqueTypesIn_t<
        typename ConcatenationOfTypeLists_t<
            typename ConcatenationOfTypeLists_t<typename LeftOperand::UsedIndexTypeList,
                                                typename RightOperand::UsedIndexTypeList>::T,
            SummedIndexTypeList>::T>::T UsedIndexTypeList;
    // CompoundIndex is a list (TODO: change to tuple) of type FreeIndices
    typedef CompoundIndex_t<FreeIndexTypeList> CompoundIndex;

    // TODO: check that the summed indices from each operand have no indices in common
    // though technically this is unnecessary, because the summed indices are "private"
    // to each contraction, so this is really for the human's benefit, not getting
    // confused by multiple repeated indices that have nothing to do with each other.
    enum
    {
        LEFT_OPERAND_IS_EXPRESSION_TEMPLATE  = Lvd::Meta::Assert<LeftOperand::IS_EXPRESSION_TEMPLATE>::v,
        RIGHT_OPERAND_IS_EXPRESSION_TEMPLATE = Lvd::Meta::Assert<RightOperand::IS_EXPRESSION_TEMPLATE>::v,
        OPERAND_SCALAR_TYPES_ARE_EQUAL       = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v,
        FREE_INDICES_DONT_COLLIDE_WITH_USED  = Lvd::Meta::Assert<(!HasNontrivialIntersectionAsSets_t<FreeIndexTypeList,UsedIndexTypeList>::V)>::v
    };
    // TODO: ensure there are no indices that occur 3+ times (?)

    static bool const IS_EXPRESSION_TEMPLATE = true;

    ExpressionTemplate_Multiplication_t (LeftOperand const &left_operand, RightOperand const &right_operand)
        :
        m_left_operand(left_operand),
        m_right_operand(right_operand)
    { }

    // available ONLY if FreeIndexTypeList is EmptyTypeList
    operator Scalar () const
    {
        Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<FreeIndexTypeList,EmptyTypeList>::v>();
        return operator[](CompoundIndex());
    }

    Scalar operator [] (CompoundIndex const &c) const
    {
        return BinarySummation_t<LeftOperand,RightOperand,FreeIndexTypeList,SummedIndexTypeList>::eval(m_left_operand, m_right_operand, c);
    }

    template <typename OtherTensor>
    bool uses_tensor (OtherTensor const &t) const
    {
        return m_left_operand.uses_tensor(t) || m_right_operand.uses_tensor(t);
    }

private:

    LeftOperand const &m_left_operand;
    RightOperand const &m_right_operand;
};

template <typename LeftOperand, typename RightOperand>
ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>
    operator * (LeftOperand const &left_operand, RightOperand const &right_operand)
{
    return ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>(left_operand, right_operand);
}

#endif // EXPRESSION_TEMPLATES_HPP_
