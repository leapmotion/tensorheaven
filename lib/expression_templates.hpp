#ifndef EXPRESSION_TEMPLATES_HPP_
#define EXPRESSION_TEMPLATES_HPP_

#include "core.hpp"
#include "expressiontemplate.hpp"
#include "expression_templates_utility.hpp"

// ////////////////////////////////////////////////////////////////////////////
// expression-template-generation (making ETs from vectors/tensors)
// ////////////////////////////////////////////////////////////////////////////

// this is the "const" version of an indexed tensor expression (it has summed indices, so it doesn't make sense to assign to it)
template <typename Tensor, typename TensorIndexTypeList, typename SummedIndexTypeList_, typename Derived_ = NullType>
struct ExpressionTemplate_IndexedTensor_t
    :
    public ExpressionTemplate_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                       ExpressionTemplate_IndexedTensor_t<Tensor,TensorIndexTypeList,SummedIndexTypeList_,Derived_>,
                                                       Derived_>::T,
                                typename Tensor::Scalar,
                                typename FreeIndexTypeList_t<TensorIndexTypeList>::T,
                                SummedIndexTypeList_>
{
    typedef ExpressionTemplate_i<typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                       ExpressionTemplate_IndexedTensor_t<Tensor,TensorIndexTypeList,SummedIndexTypeList_,Derived_>,
                                                       Derived_>::T,
                                 typename Tensor::Scalar,
                                 typename FreeIndexTypeList_t<TensorIndexTypeList>::T,
                                 SummedIndexTypeList_> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeIndexTypeList FreeIndexTypeList;
    typedef typename Parent::UsedIndexTypeList UsedIndexTypeList;
    typedef typename Parent::CompoundIndex CompoundIndex;
    using Parent::IS_EXPRESSION_TEMPLATE;
    typedef SummedIndexTypeList_ SummedIndexTypeList;

    ExpressionTemplate_IndexedTensor_t (Tensor const &tensor) : m_tensor(tensor) { }

    operator Scalar () const
    {
        Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<FreeIndexTypeList,EmptyTypeList>::v>();
        return operator[](CompoundIndex());
    }

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
    :
    public ExpressionTemplate_i<ExpressionTemplate_IndexedTensor_t<Tensor,TensorIndexTypeList,EmptyTypeList>,
                                typename Tensor::Scalar,
                                typename FreeIndexTypeList_t<TensorIndexTypeList>::T,
                                EmptyTypeList>
{
    typedef ExpressionTemplate_i<ExpressionTemplate_IndexedTensor_t<Tensor,TensorIndexTypeList,EmptyTypeList>,
                                 typename Tensor::Scalar,
                                 typename FreeIndexTypeList_t<TensorIndexTypeList>::T,
                                 EmptyTypeList> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeIndexTypeList FreeIndexTypeList;
    typedef typename Parent::UsedIndexTypeList UsedIndexTypeList;
    typedef typename Parent::CompoundIndex CompoundIndex;
    using Parent::IS_EXPRESSION_TEMPLATE;

    ExpressionTemplate_IndexedTensor_t (Tensor &tensor) : m_tensor(tensor) { }

    operator Scalar () const
    {
        Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<FreeIndexTypeList,EmptyTypeList>::v>();
        return operator[](CompoundIndex());
    }

    // read-only, because it doesn't necessarily make sense to assign to an expression
    // template -- the expression may be a product or some such, where each component
    // is not an L-value.
    Scalar const &operator [] (typename Tensor::Index const &i) const { return m_tensor[i]; }
    Scalar operator [] (CompoundIndex const &c) const { return m_tensor[c]; }

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

// NOTE: if this is ever subclassed, then it will be necessary to change the inheritance to pass in the Derived type
template <typename LeftOperand, typename RightOperand, char OPERATOR>
struct ExpressionTemplate_Addition_t
    :
    public ExpressionTemplate_i<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>,
                                typename LeftOperand::Scalar,
                                typename LeftOperand::FreeIndexTypeList,
                                EmptyTypeList>
{
    typedef ExpressionTemplate_i<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>,
                                 typename LeftOperand::Scalar,
                                 typename LeftOperand::FreeIndexTypeList,
                                 EmptyTypeList> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeIndexTypeList FreeIndexTypeList;
    typedef typename Parent::UsedIndexTypeList UsedIndexTypeList;
    typedef typename Parent::CompoundIndex CompoundIndex;
    using Parent::IS_EXPRESSION_TEMPLATE;

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
        RIGHT_OPERAND_HAS_NO_DUPLICATE_FREE_INDICES = Lvd::Meta::Assert<!ContainsDuplicates_t<typename RightOperand::FreeIndexTypeList>::V>::v,
        OPERATOR_IS_PLUS_OR_MINUS                   = Lvd::Meta::Assert<(OPERATOR == '+' || OPERATOR == '-')>::v
    };

    ExpressionTemplate_Addition_t (LeftOperand const &left_operand, RightOperand const &right_operand)
        :
        m_left_operand(left_operand),
        m_right_operand(right_operand)
    { }

    operator Scalar () const
    {
        Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<FreeIndexTypeList,EmptyTypeList>::v>();
        if (OPERATOR == '+')
            return m_left_operand.operator Scalar() + m_right_operand.operator Scalar();
        else // OPERATOR == '-'
            return m_left_operand.operator Scalar() - m_right_operand.operator Scalar();
    }

    Scalar operator [] (CompoundIndex const &c) const
    {
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
        if (OPERATOR == '+')
            return m_left_operand[left_operand_index_map(c)] + m_right_operand[right_operand_index_map(c)];
        else // OPERATOR == '-'
            return m_left_operand[left_operand_index_map(c)] - m_right_operand[right_operand_index_map(c)];
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

// ////////////////////////////////////////////////////////////////////////////
// scalar multiplication and division of expression templates
// ////////////////////////////////////////////////////////////////////////////

// it is assumed that scalar multiplication is commutative.
// OPERATOR can be '*' or '/'.
// NOTE: if this is ever subclassed, then it will be necessary to change the inheritance to pass in the Derived type
template <typename Operand, typename Scalar_, char OPERATOR>
struct ExpressionTemplate_ScalarMultiplication_t
    :
    public ExpressionTemplate_i<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>,
                                typename Operand::Scalar,
                                typename Operand::FreeIndexTypeList,
                                typename Operand::UsedIndexTypeList>
{
    typedef ExpressionTemplate_i<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>,
                                 typename Operand::Scalar,
                                 typename Operand::FreeIndexTypeList,
                                 typename Operand::UsedIndexTypeList> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeIndexTypeList FreeIndexTypeList;
    typedef typename Parent::UsedIndexTypeList UsedIndexTypeList;
    typedef typename Parent::CompoundIndex CompoundIndex;
    using Parent::IS_EXPRESSION_TEMPLATE;

    enum
    {
        OPERAND_IS_EXPRESSION_TEMPLATE = Lvd::Meta::Assert<Operand::IS_EXPRESSION_TEMPLATE>::v,
        OPERAND_SCALAR_MATCHES_SCALAR  = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename Operand::Scalar,Scalar_>::v>::v,
        OPERATOR_IS_VALID              = Lvd::Meta::Assert<(OPERATOR == '*' || OPERATOR == '/')>::v
    };

    ExpressionTemplate_ScalarMultiplication_t (Operand const &operand, Scalar scalar_operand)
        :
        m_operand(operand),
        m_scalar_operand(scalar_operand)
    {
        // TODO: should there be a runtime check here if OPERATOR is '/' and scalar_operand is zero (or close to zero)?
    }

    operator Scalar () const
    {
        Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<FreeIndexTypeList,EmptyTypeList>::v>();
        return operator[](CompoundIndex());
    }

    Scalar operator [] (CompoundIndex const &c) const
    {
        if (OPERATOR == '*')
            return m_operand[c] * m_scalar_operand;
        else
            return m_operand[c] / m_scalar_operand;
    }

    template <typename OtherTensor>
    bool uses_tensor (OtherTensor const &t) const
    {
        return m_operand.uses_tensor(t);
    }

private:

    Operand const &m_operand;
    Scalar m_scalar_operand;
};

// ////////////////////////////////////////////////////////////////////////////
// multiplication of expression templates (tensor product and contraction)
// ////////////////////////////////////////////////////////////////////////////

// TODO: there is an issue to think about: while it is totally valid to do
// u(i)*v(j)*w(j) (this is an outer product contracted with a vector), the
// expression v(j)*w(j) can be computed first and factored out of the whole
// thing, instead of needing to be multiplied out for each access of the i index.
// this may be somewhat difficult to do, as it would require searching the
// expression template AST for such contractions and restructuring the AST.
// NOTE: if this is ever subclassed, then it will be necessary to change the inheritance to pass in the Derived type
template <typename LeftOperand, typename RightOperand>
struct ExpressionTemplate_Multiplication_t
    :
    public ExpressionTemplate_i<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>,
                                typename LeftOperand::Scalar,
                                typename FreeIndexTypeListOfMultiplication_t<LeftOperand,RightOperand>::T,
                                typename UsedIndexTypeListOfMultiplication_t<LeftOperand,RightOperand>::T>
{
    typedef ExpressionTemplate_i<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>,
                                 typename LeftOperand::Scalar,
                                 typename FreeIndexTypeListOfMultiplication_t<LeftOperand,RightOperand>::T,
                                 typename UsedIndexTypeListOfMultiplication_t<LeftOperand,RightOperand>::T> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeIndexTypeList FreeIndexTypeList;
    typedef typename Parent::UsedIndexTypeList UsedIndexTypeList;
    typedef typename Parent::CompoundIndex CompoundIndex;
    using Parent::IS_EXPRESSION_TEMPLATE;

    typedef typename SummedIndexTypeListOfMultiplication_t<LeftOperand,RightOperand>::T SummedIndexTypeList;

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

// ////////////////////////////////////////////////////////////////////////////
// bundling multiple indices into a single compound index (tensor downcasting)
// ////////////////////////////////////////////////////////////////////////////

template <typename Operand, typename BundleIndexTypeList, typename ResultingIndexType>
struct IndexTypeListOfIndexBundle_t
{
    
    typedef typename SetSubtraction_t<TypeList_t<ResultingIndexType,typename Operand::FreeIndexTypeList>,BundleIndexTypeList>::T T;
};

template <typename Operand, typename BundleIndexTypeList, typename ResultingIndexType>
struct UsedIndexTypeListOfIndexBundle_t
{
    enum 
    { 
        BUNDLE_INDICES_MUST_BE_FREE           = Lvd::Meta::Assert<!HasNontrivialIntersectionAsSets_t<BundleIndexTypeList,typename Operand::FreeIndexTypeList>::V>::v,
        BUNDLE_AND_RESULTING_MUST_BE_DISTINCT = Lvd::Meta::Assert<!HasNontrivialIntersectionAsSets_t<BundleIndexTypeList,TypeList_t<ResultingIndexType> >::V>::v
    };
    
    typedef typename ConcatenationOfTypeLists_t<typename Operand::UsedIndexTypeList,BundleIndexTypeList>::T T;
};

// not an expression template, but just something that handles the bundled indices
template <typename Operand, typename BundleIndexTypeList, typename ResultingIndexType, CompoundIndex_t<BundleIndexTypeList> (*BUNDLE_INDEX_MAP)(ResultingIndexType const &)>
struct IndexBundle_t
{
    enum 
    { 
        BUNDLE_INDICES_MUST_BE_FREE           = Lvd::Meta::Assert<IsASubsetOf_t<BundleIndexTypeList,typename Operand::FreeIndexTypeList>::V>::v,
        BUNDLE_AND_RESULTING_MUST_BE_DISTINCT = Lvd::Meta::Assert<!HasNontrivialIntersectionAsSets_t<BundleIndexTypeList,TypeList_t<ResultingIndexType> >::V>::v,
        OPERAND_IS_EXPRESSION_TEMPLATE        = Lvd::Meta::Assert<Operand::IS_EXPRESSION_TEMPLATE>::v
    };
    
    typedef typename Operand::Scalar Scalar;
    // ResultingIndexType comes first in IndexTypeList
    typedef typename SetSubtraction_t<TypeList_t<ResultingIndexType,typename Operand::FreeIndexTypeList>,BundleIndexTypeList>::T IndexTypeList;
    typedef typename ConcatenationOfTypeLists_t<typename Operand::UsedIndexTypeList,BundleIndexTypeList>::T UsedIndexTypeList;
    typedef CompoundIndex_t<IndexTypeList> CompoundIndex;

    IndexBundle_t (Operand const &operand) : m_operand(operand) { }

    Scalar operator [] (CompoundIndex const &c) const
    {
        // replace the head of c with the separate indices that it bundles
        return m_operand[BUNDLE_INDEX_MAP(c.head()) |= c.body()]; // |= is concatenation of CompoundIndex_t instances
    }

    template <typename OtherTensor>
    bool uses_tensor (OtherTensor const &t) const { return m_operand.uses_tensor(t); }
    
private:

    Operand const &m_operand;
};

template <typename Operand, typename BundleIndexTypeList, typename ResultingIndexType, CompoundIndex_t<BundleIndexTypeList> (*BUNDLE_INDEX_MAP)(ResultingIndexType const &)>
struct ExpressionTemplate_IndexBundle_t 
    : 
    public ExpressionTemplate_IndexedTensor_t<IndexBundle_t<Operand,BundleIndexTypeList,ResultingIndexType,BUNDLE_INDEX_MAP>,
                                              typename IndexBundle_t<Operand,BundleIndexTypeList,ResultingIndexType,BUNDLE_INDEX_MAP>::IndexTypeList,
                                              typename SummedIndexTypeList_t<typename IndexBundle_t<Operand,BundleIndexTypeList,ResultingIndexType,BUNDLE_INDEX_MAP>::IndexTypeList>::T,
                                              ExpressionTemplate_IndexBundle_t<Operand,BundleIndexTypeList,ResultingIndexType,BUNDLE_INDEX_MAP> >
{
    typedef ExpressionTemplate_IndexedTensor_t<IndexBundle_t<Operand,BundleIndexTypeList,ResultingIndexType,BUNDLE_INDEX_MAP>,
                                               typename IndexBundle_t<Operand,BundleIndexTypeList,ResultingIndexType,BUNDLE_INDEX_MAP>::IndexTypeList,
                                               typename SummedIndexTypeList_t<typename IndexBundle_t<Operand,BundleIndexTypeList,ResultingIndexType,BUNDLE_INDEX_MAP>::IndexTypeList>::T,
                                               ExpressionTemplate_IndexBundle_t<Operand,BundleIndexTypeList,ResultingIndexType,BUNDLE_INDEX_MAP> > Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeIndexTypeList FreeIndexTypeList;
    typedef typename Parent::UsedIndexTypeList UsedIndexTypeList;
    typedef typename Parent::CompoundIndex CompoundIndex;
    using Parent::IS_EXPRESSION_TEMPLATE;
    typedef typename Parent::SummedIndexTypeList SummedIndexTypeList;

    typedef IndexBundle_t<Operand,BundleIndexTypeList,ResultingIndexType,BUNDLE_INDEX_MAP> IndexBundle;

    ExpressionTemplate_IndexBundle_t (Operand const &operand) : Parent(m_index_bundle), m_index_bundle(operand) { }

    operator Scalar () const
    {
        Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<FreeIndexTypeList,EmptyTypeList>::v>();
        return operator[](CompoundIndex());
    }

    // read-only, because it doesn't make sense to assign to an index-bundled expression (which is possibly also a summation).
    Scalar operator [] (CompoundIndex const &c) const
    {
        return UnarySummation_t<IndexBundle,typename IndexBundle::IndexTypeList,SummedIndexTypeList>::eval(m_index_bundle, c);
    }

    template <typename OtherTensor>
    bool uses_tensor (OtherTensor const &t) const { return m_index_bundle.uses_tensor(t); }

private:

    IndexBundle m_index_bundle;
};

// ////////////////////////////////////////////////////////////////////////////
// operator overloads for expression templates
// ////////////////////////////////////////////////////////////////////////////

// expression template addition/subtractions

// addition
template <typename LeftDerived, typename LeftFreeIndexTypeList, typename LeftUsedIndexTypeList,
          typename RightDerived, typename RightFreeIndexTypeList, typename RightUsedIndexTypeList>
ExpressionTemplate_Addition_t<LeftDerived,RightDerived,'+'>
    operator + (ExpressionTemplate_i<LeftDerived,typename LeftDerived::Scalar,LeftFreeIndexTypeList,LeftUsedIndexTypeList> const &left_operand,
                ExpressionTemplate_i<RightDerived,typename RightDerived::Scalar,RightFreeIndexTypeList,RightUsedIndexTypeList> const &right_operand)
{
    return ExpressionTemplate_Addition_t<LeftDerived,RightDerived,'+'>(left_operand.as_derived(), right_operand.as_derived());
}

// subtraction
template <typename LeftDerived, typename LeftFreeIndexTypeList, typename LeftUsedIndexTypeList,
          typename RightDerived, typename RightFreeIndexTypeList, typename RightUsedIndexTypeList>
ExpressionTemplate_Addition_t<LeftDerived,RightDerived,'-'>
    operator - (ExpressionTemplate_i<LeftDerived,typename LeftDerived::Scalar,LeftFreeIndexTypeList,LeftUsedIndexTypeList> const &left_operand,
                ExpressionTemplate_i<RightDerived,typename RightDerived::Scalar,RightFreeIndexTypeList,RightUsedIndexTypeList> const &right_operand)
{
    return ExpressionTemplate_Addition_t<LeftDerived,RightDerived,'-'>(left_operand.as_derived(), right_operand.as_derived());
}

// scalar multiplication/division, including unary negation (multiplication by -1)

// scalar multiplication on the right
template <typename Derived, typename FreeIndexTypeList, typename UsedIndexTypeList>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>
    operator * (ExpressionTemplate_i<Derived,typename Derived::Scalar,FreeIndexTypeList,UsedIndexTypeList> const &operand,
                typename Derived::Scalar scalar_operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>(operand.as_derived(), scalar_operand);
}

// scalar multiplication on the right -- this overload allows integer literals to be used in scalar multiplications
template <typename Derived, typename FreeIndexTypeList, typename UsedIndexTypeList>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>
    operator * (ExpressionTemplate_i<Derived,typename Derived::Scalar,FreeIndexTypeList,UsedIndexTypeList> const &operand,
                int scalar_operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>(operand.as_derived(), scalar_operand);
}

// scalar multiplication on the left
template <typename Derived, typename FreeIndexTypeList, typename UsedIndexTypeList>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>
    operator * (typename Derived::Scalar scalar_operand,
                ExpressionTemplate_i<Derived,typename Derived::Scalar,FreeIndexTypeList,UsedIndexTypeList> const &operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>(operand.as_derived(), scalar_operand);
}

// scalar multiplication on the left -- this overload allows integer literals to be used in scalar multiplications
template <typename Derived, typename FreeIndexTypeList, typename UsedIndexTypeList>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>
    operator * (int scalar_operand,
                ExpressionTemplate_i<Derived,typename Derived::Scalar,FreeIndexTypeList,UsedIndexTypeList> const &operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>(operand.as_derived(), scalar_operand);
}

// scalar division on the right
template <typename Derived, typename FreeIndexTypeList, typename UsedIndexTypeList>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'/'>
    operator / (ExpressionTemplate_i<Derived,typename Derived::Scalar,FreeIndexTypeList,UsedIndexTypeList> const &operand,
                typename Derived::Scalar scalar_operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'/'>(operand.as_derived(), scalar_operand);
}

// scalar division on the right -- this overload allows integer literals to be used in scalar divisions (it's Scalar division, not integer division)
template <typename Derived, typename FreeIndexTypeList, typename UsedIndexTypeList>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'/'>
    operator / (ExpressionTemplate_i<Derived,typename Derived::Scalar,FreeIndexTypeList,UsedIndexTypeList> const &operand,
                int scalar_operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'/'>(operand.as_derived(), scalar_operand);
}

// unary negation
template <typename Derived, typename FreeIndexTypeList, typename UsedIndexTypeList>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>
    operator - (ExpressionTemplate_i<Derived,typename Derived::Scalar,FreeIndexTypeList,UsedIndexTypeList> const &operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>(operand.as_derived(), -1);
}

// expression template multiplication -- tensor contraction and product

template <typename LeftDerived, typename LeftFreeIndexTypeList, typename LeftUsedIndexTypeList,
          typename RightDerived, typename RightFreeIndexTypeList, typename RightUsedIndexTypeList>
ExpressionTemplate_Multiplication_t<LeftDerived,RightDerived>
    operator * (ExpressionTemplate_i<LeftDerived,typename LeftDerived::Scalar,LeftFreeIndexTypeList,LeftUsedIndexTypeList> const &left_operand,
                ExpressionTemplate_i<RightDerived,typename RightDerived::Scalar,RightFreeIndexTypeList,RightUsedIndexTypeList> const &right_operand)
{
    return ExpressionTemplate_Multiplication_t<LeftDerived,RightDerived>(left_operand.as_derived(), right_operand.as_derived());
}

#endif // EXPRESSION_TEMPLATES_HPP_
