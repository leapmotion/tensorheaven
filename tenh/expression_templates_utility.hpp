// ///////////////////////////////////////////////////////////////////////////
// tenh/expression_templates_utility.hpp by Victor Dods, created 2013/03/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_EXPRESSION_TEMPLATES_UTILITY_HPP_
#define TENH_EXPRESSION_TEMPLATES_UTILITY_HPP_

#include "tenh/core.hpp"
#include "tenh/innerproduct.hpp"
#include "tenh/multiindex.hpp"
#include "tenh/meta/typelist.hpp"

namespace Tenh {

// this file contains template metaprograms which assist in the construction and evaluation of expression templates

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
typename HeadType::Owner::Scalar summation_component_factor (MultiIndex_t<TypeList_t<HeadType> > const &s)
{
    return InnerProduct_t<typename HeadType::Owner,typename HeadType::Owner::Basis>::component(s.head());
}

template <typename HeadType, typename BodyTypeList>
typename HeadType::Owner::Scalar summation_component_factor (MultiIndex_t<TypeList_t<HeadType,BodyTypeList> > const &s)
{
    return InnerProduct_t<typename HeadType::Owner,typename HeadType::Owner::Basis>::component(s.head()) * summation_component_factor(s.body());
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
    typedef MultiIndex_t<FreeIndexTypeList> MultiIndex;

    static Scalar eval (Tensor const &tensor, MultiIndex const &m)
    {
        typedef typename ConcatenationOfTypeLists_t<FreeIndexTypeList,SummedIndexTypeList>::T TotalIndexTypeList;
        typedef MultiIndex_t<TotalIndexTypeList> TotalIndex;
        typedef MultiIndex_t<SummedIndexTypeList> SummedIndex;

        // the operands take indices that are a subset of the summed indices and free indices.

        // constructing t with m initializes the first elements which correpond to
        // MultiIndex with the value of m, and initializes the remaining elements to zero.
        TotalIndex t(m);
        Scalar retval(0);
        // get the map which produces the MultiIndex for each tensor from the TotalIndex t
        typedef MultiIndexMap_t<TotalIndexTypeList,TensorIndexTypeList> TensorIndexMap;
        static typename TensorIndexMap::EvalMapType const tensor_index_map = TensorIndexMap::eval;
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
    typedef MultiIndex_t<FreeIndexTypeList> MultiIndex;

    static Scalar eval (Tensor const &tensor, MultiIndex const &m) { return tensor[m]; }
};

template <typename LeftOperand, typename RightOperand, typename FreeIndexTypeList, typename SummedIndexTypeList>
struct BinarySummation_t
{
    enum
    {
        STATIC_ASSERT_IN_ENUM(LeftOperand::IS_EXPRESSION_TEMPLATE_I, LEFT_OPERAND_IS_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM(RightOperand::IS_EXPRESSION_TEMPLATE_I, RIGHT_OPERAND_IS_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM((Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v), OPERAND_SCALAR_TYPES_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((SummedIndexTypeList::LENGTH > 0), LENGTH_MUST_BE_POSITIVE)
    };

    typedef typename LeftOperand::Scalar Scalar;
    typedef MultiIndex_t<FreeIndexTypeList> MultiIndex;

    static Scalar eval (LeftOperand const &left_operand, RightOperand const &right_operand, MultiIndex const &m)
    {
        typedef typename ConcatenationOfTypeLists_t<FreeIndexTypeList,SummedIndexTypeList>::T TotalIndexTypeList;
        typedef MultiIndex_t<TotalIndexTypeList> TotalIndex;
        typedef MultiIndex_t<SummedIndexTypeList> SummedIndex;

        // the operands take indices that are a subset of the summed indices and free indices.

        // constructing t with m initializes the first elements which correpond to
        // MultiIndex with the value of m, and initializes the remaining elements to zero.
        TotalIndex t(m);
        Scalar retval(0);
        // get the map which produces the MultiIndex for each operand from the TotalIndex t
        typedef MultiIndexMap_t<TotalIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef MultiIndexMap_t<TotalIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        static typename LeftOperandIndexMap::EvalMapType const left_operand_index_map = LeftOperandIndexMap::eval;
        static typename RightOperandIndexMap::EvalMapType const right_operand_index_map = RightOperandIndexMap::eval;
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
    enum
    {
        STATIC_ASSERT_IN_ENUM(LeftOperand::IS_EXPRESSION_TEMPLATE_I, LEFT_OPERAND_IS_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM(RightOperand::IS_EXPRESSION_TEMPLATE_I, RIGHT_OPERAND_IS_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM((Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v), OPERAND_SCALAR_TYPES_ARE_EQUAL)
    };

    typedef typename LeftOperand::Scalar Scalar;
    typedef MultiIndex_t<FreeIndexTypeList> MultiIndex;

    static Scalar eval (LeftOperand const &left_operand, RightOperand const &right_operand, MultiIndex const &m)
    {
        // get the map which produces the MultiIndex for each operand from the free indices m
        typedef MultiIndexMap_t<FreeIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef MultiIndexMap_t<FreeIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        static typename LeftOperandIndexMap::EvalMapType const left_operand_index_map = LeftOperandIndexMap::eval;
        static typename RightOperandIndexMap::EvalMapType const right_operand_index_map = RightOperandIndexMap::eval;
        return left_operand[left_operand_index_map(m)] * right_operand[right_operand_index_map(m)];
    }
};

template <typename LeftOperand, typename RightOperand>
struct FreeIndexTypeListOfMultiplication_t
{
private:
    // the free indices are the single-occurrence indices of the concatenated
    // list of free indices from the left and right operands
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeIndexTypeList,
                                                typename RightOperand::FreeIndexTypeList>::T CombinedFreeIndexTypeList;
public:
    typedef typename ElementsHavingMultiplicity_t<CombinedFreeIndexTypeList,1>::T T;
};

template <typename LeftOperand, typename RightOperand>
struct SummedIndexTypeListOfMultiplication_t
{
private:
    // the free indices are the single-occurrence indices of the concatenated
    // list of free indices from the left and right operands
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeIndexTypeList,
                                                typename RightOperand::FreeIndexTypeList>::T CombinedFreeIndexTypeList;
public:
    // the summed indices (at this level) are the double-occurrences indices
    // of the concatenated list of free indices from the left and right operands
    typedef typename ElementsHavingMultiplicity_t<CombinedFreeIndexTypeList,2>::T T;
};

template <typename LeftOperand, typename RightOperand>
struct UsedIndexTypeListOfMultiplication_t
{
private:
    typedef typename SummedIndexTypeListOfMultiplication_t<LeftOperand,RightOperand>::T SummedIndexTypeList;
public:
    // typelist of used indices which are prohibited from using higher up in the AST
    typedef typename UniqueTypesIn_t<
        typename ConcatenationOfTypeLists_t<
            typename ConcatenationOfTypeLists_t<typename LeftOperand::UsedIndexTypeList,
                                                typename RightOperand::UsedIndexTypeList>::T,
            SummedIndexTypeList>::T>::T T;
};

template <typename BundleIndexTypeList, typename ResultingIndexType>
struct BundleIndexMap_t
{
    typedef MultiIndex_t<BundleIndexTypeList> (*T) (ResultingIndexType const &);
    static T const V;
};

template <typename BundleIndexTypeList, typename ResultingIndexType>
typename BundleIndexMap_t<BundleIndexTypeList,ResultingIndexType>::T const BundleIndexMap_t<BundleIndexTypeList,ResultingIndexType>::V = ResultingIndexType::Owner::Derived::template bundle_index_map<BundleIndexTypeList,ResultingIndexType>;

// not an expression template, but just something that handles the bundled indices
template <typename Operand, typename BundleIndexTypeList, typename ResultingIndexType>
struct IndexBundle_t
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((IsASubsetOf_t<BundleIndexTypeList,typename Operand::FreeIndexTypeList>::V), BUNDLE_INDICES_MUST_BE_FREE),
        STATIC_ASSERT_IN_ENUM((!HasNontrivialIntersectionAsSets_t<BundleIndexTypeList,TypeList_t<ResultingIndexType> >::V), BUNDLE_AND_RESULTING_MUST_BE_DISTINCT),
        STATIC_ASSERT_IN_ENUM(Operand::IS_EXPRESSION_TEMPLATE_I, OPERAND_IS_EXPRESSION_TEMPLATE)
    };

    typedef typename Operand::Scalar Scalar;
    // ResultingIndexType comes last in IndexTypeList
    typedef typename SetSubtraction_t<
        typename ConcatenationOfTypeLists_t<typename Operand::FreeIndexTypeList,TypeList_t<ResultingIndexType> >::T,
        BundleIndexTypeList>::T IndexTypeList;
    // this seems like it should be the same as Operand::FreeIndexTypeList, but it has
    // the indices in BundleIndexTypeList coming last.
    typedef typename ConcatenationOfTypeLists_t<
        typename SetSubtraction_t<typename Operand::FreeIndexTypeList,BundleIndexTypeList>::T,
        BundleIndexTypeList>::T UnpackedIndexTypeList;
    typedef typename ConcatenationOfTypeLists_t<typename Operand::UsedIndexTypeList,BundleIndexTypeList>::T UsedIndexTypeList;
    typedef MultiIndex_t<IndexTypeList> MultiIndex;
    typedef typename BundleIndexMap_t<BundleIndexTypeList,ResultingIndexType>::T BundleIndexMap;

    IndexBundle_t (Operand const &operand) : m_operand(operand) { }

    Scalar operator [] (MultiIndex const &m) const
    {
        // replace the head of m with the separate indices that it bundles.
        // use MultiIndexMap_t to place the indices in the correct order.
        typedef MultiIndexMap_t<UnpackedIndexTypeList,typename Operand::FreeIndexTypeList> OperandIndexMap;
        static typename OperandIndexMap::EvalMapType const operand_index_map = OperandIndexMap::eval;
        typedef BundleIndexMap_t<BundleIndexTypeList,ResultingIndexType> BundleIndexMap;
        static typename BundleIndexMap::T const bundle_index_map = BundleIndexMap::V;
        // |= is concatenation of MultiIndex_t instances
        return m_operand[operand_index_map(m.template leading_list<MultiIndex::LENGTH-1>() |=
                                           bundle_index_map(m.template el<MultiIndex::LENGTH-1>()))];
    }

    template <typename OtherTensor>
    bool uses_tensor (OtherTensor const &t) const { return m_operand.uses_tensor(t); }

private:

    Operand const &m_operand;
};

// not an expression template, but just something that handles the split indices
template <typename Operand, typename SourceIndexType, typename SplitIndexTypeList>
struct IndexSplitter_t
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((Operand::FreeIndexTypeList::template Contains_t<SourceIndexType>::V), SOURCE_INDEX_MUST_BE_FREE),
        STATIC_ASSERT_IN_ENUM((!HasNontrivialIntersectionAsSets_t<TypeList_t<SourceIndexType>,SplitIndexTypeList>::V), SOURCE_AND_SPLIT_MUST_BE_DISTINCT),
        STATIC_ASSERT_IN_ENUM(Operand::IS_EXPRESSION_TEMPLATE_I, OPERAND_IS_EXPRESSION_TEMPLATE)
    };

    typedef typename Operand::Scalar Scalar;
    // replace SourceIndexType with MultiIndex_t<SplitIndexTypeList> in the index type list
    static Uint32 const SOURCE_INDEX_TYPE_INDEX = FirstMatchingIn_t<typename Operand::FreeIndexTypeList,SourceIndexType>::INDEX;
    typedef typename ConcatenationOfTypeLists_t<
        typename Operand::FreeIndexTypeList::template LeadingTypeList_t<SOURCE_INDEX_TYPE_INDEX>::T,
        typename ConcatenationOfTypeLists_t<
            SplitIndexTypeList,
            typename Operand::FreeIndexTypeList::template TrailingTypeList_t<SOURCE_INDEX_TYPE_INDEX+1>::T
            >::T
        >::T IndexTypeList;
    typedef typename ConcatenationOfTypeLists_t<typename Operand::UsedIndexTypeList,SplitIndexTypeList>::T UsedIndexTypeList;
    typedef MultiIndex_t<IndexTypeList> MultiIndex;

    IndexSplitter_t (Operand const &operand) : m_operand(operand) { }

    Scalar operator [] (MultiIndex const &m) const
    {
        typedef typename SourceIndexType::Owner::Derived SourceIndexOwnerTensor;
        typename SourceIndexOwnerTensor::MultiIndex s(m.template range<SOURCE_INDEX_TYPE_INDEX,SOURCE_INDEX_TYPE_INDEX+SplitIndexTypeList::LENGTH>());
        if (SourceIndexOwnerTensor::component_is_immutable_zero(s))
            return Scalar(0);

        SourceIndexType i(SourceIndexOwnerTensor::vector_index_of(s));
        // this replaces the SplitIndexTypeList portion with SourceIndexType
        typename Operand::MultiIndex c_rebundled(m.template leading_list<SOURCE_INDEX_TYPE_INDEX>() |=
                                                 (i >>= m.template trailing_list<SOURCE_INDEX_TYPE_INDEX+SplitIndexTypeList::LENGTH>()));
        return SourceIndexOwnerTensor::scalar_factor_for_component(s) * m_operand[c_rebundled];
    }

    template <typename OtherTensor>
    bool uses_tensor (OtherTensor const &t) const { return m_operand.uses_tensor(t); }

private:

    Operand const &m_operand;
};

} // end of namespace Tenh

#endif // TENH_EXPRESSION_TEMPLATES_UTILITY_HPP_
