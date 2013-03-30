#ifndef EXPRESSION_TEMPLATES_UTILITY_HPP_
#define EXPRESSION_TEMPLATES_UTILITY_HPP_

#include "core.hpp"
#include "compoundindex.hpp"
#include "naturalpairing.hpp"
#include "typelist.hpp"

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
    typedef CompoundIndex_t<BundleIndexTypeList> (*T) (ResultingIndexType const &);
    static T const V;
};

template <typename BundleIndexTypeList, typename ResultingIndexType>
typename BundleIndexMap_t<BundleIndexTypeList,ResultingIndexType>::T const BundleIndexMap_t<BundleIndexTypeList,ResultingIndexType>::V = ResultingIndexType::OwnerVector::Derived::template bundle_index_map<BundleIndexTypeList,ResultingIndexType>;

// not an expression template, but just something that handles the bundled indices
template <typename Operand, typename BundleIndexTypeList, typename ResultingIndexType>
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
    typedef typename BundleIndexMap_t<BundleIndexTypeList,ResultingIndexType>::T BundleIndexMap;

    IndexBundle_t (Operand const &operand) : m_operand(operand) { }

    Scalar operator [] (CompoundIndex const &c) const
    {
        // replace the head of c with the separate indices that it bundles.
        // |= is concatenation of CompoundIndex_t instances
        return m_operand[(BundleIndexMap_t<BundleIndexTypeList,ResultingIndexType>::V)(c.head()) |= c.body()];
    }

    template <typename OtherTensor>
    bool uses_tensor (OtherTensor const &t) const { return m_operand.uses_tensor(t); }
    
private:

    Operand const &m_operand;
};

#endif // EXPRESSION_TEMPLATES_UTILITY_HPP_
