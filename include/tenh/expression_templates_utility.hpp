// ///////////////////////////////////////////////////////////////////////////
// tenh/expression_templates_utility.hpp by Victor Dods, created 2013/03/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_EXPRESSION_TEMPLATES_UTILITY_HPP_
#define TENH_EXPRESSION_TEMPLATES_UTILITY_HPP_

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp"
#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/linearembedding.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/dimindex.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/interface/expressiontemplate.hpp"
#include "tenh/meta/typelist.hpp"
#include "tenh/multiindex.hpp"

namespace Tenh {

// this file contains template metaprograms which assist in the construction and evaluation of expression templates

// for this to work correctly on DimIndex_t types, the dimensions must be correct (i.e. the
// primal/dual vector space checking must already be done).  TODO: redesign-away this caveat
template <typename IndexTypeList>
struct SummedIndexTypeList_t
{
    typedef typename ElementsHavingMultiplicity_t<IndexTypeList,2>::T T;
private:
    SummedIndexTypeList_t();
};

// for this to work correctly on DimIndex_t types, the dimensions must be correct (i.e. the
// primal/dual vector space checking must already be done).  TODO: redesign-away this caveat
template <typename IndexTypeList>
struct FreeIndexTypeList_t
{
    typedef typename ElementsHavingMultiplicity_t<IndexTypeList,1>::T T;
private:
    FreeIndexTypeList_t();
};

template <typename FreeIndexTypeList>
struct IndexIsFree_t
{
    enum { STATIC_ASSERT_IN_ENUM(IsTypeList_f<FreeIndexTypeList>::V, MUST_BE_TYPELIST) };

    template <typename T>
    struct Eval_t
    {
        enum
        {
            STATIC_ASSERT_IN_ENUM(IsTypeList_f<T>::V, MUST_BE_TYPELIST),
            STATIC_ASSERT_IN_ENUM(T::LENGTH == 2, LENGTH_MUST_BE_EXACTLY_2)
        };
        typedef typename T::HeadType Factor;
        typedef typename T::BodyTypeList::HeadType Index;
        static bool const V = Contains_f<FreeIndexTypeList,Index>::V;
    };
private:
    IndexIsFree_t();
};

template <typename Unzipped>
struct FreeFactorTypeListHelper_t
{
    typedef typename Unzipped::HeadType T;
private:
    FreeFactorTypeListHelper_t();
};

template <>
struct FreeFactorTypeListHelper_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
private:
    FreeFactorTypeListHelper_t();
};

// for this to work correctly on DimIndex_t types, the dimensions must be correct (i.e. the
// primal/dual vector space checking must already be done).  TODO: redesign-away this caveat
template <typename FactorTypeList, typename IndexTypeList>
struct FreeFactorTypeList_t
{
private:
    typedef typename Zip_t<TypeList_t<FactorTypeList,TypeList_t<IndexTypeList> > >::T FactorAndIndexPairTypeList;
    typedef typename FreeIndexTypeList_t<IndexTypeList>::T FreeIndexTypeList;
    typedef typename ElementsOfTypeListSatisfyingPredicate_t<FactorAndIndexPairTypeList,IndexIsFree_t<FreeIndexTypeList> >::T FreeFactorAndIndexPairTypeList;
    typedef typename Unzip_t<FreeFactorAndIndexPairTypeList>::T Unzipped;
    enum { STATIC_ASSERT_IN_ENUM((Unzipped::LENGTH == 0 || Unzipped::LENGTH == 2), LENGTH_MUST_BE_EXACTLY_0_OR_2) };
    FreeFactorTypeList_t();
public:
    // each pair in FreeFactorAndIndexPairTypeList is a (factor, index) TypeList_t
    typedef typename FreeFactorTypeListHelper_t<Unzipped>::T T;
};

template <typename AbstractIndexTypeList, typename SummedAbstractIndexTypeList, typename AbstractIndex>
struct SummedAbstractIndexPairElementIndices_t
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsAbstractIndex_f<AbstractIndex>::V, MUST_BE_ABSTRACT_INDEX),
        STATIC_ASSERT_IN_ENUM((Occurrence_t<AbstractIndexTypeList,AbstractIndex>::COUNT == 2), MUST_OCCUR_EXACTLY_TWICE)
    };
    SummedAbstractIndexPairElementIndices_t();
public:
    static Uint32 const FIRST = IndexOfFirstOccurrence_f<AbstractIndexTypeList,AbstractIndex>::V;
private:
    static Uint32 const FIRST_MATCHING_IN_REST = IndexOfFirstOccurrence_f<typename TrailingTypeList_f<AbstractIndexTypeList,FIRST+1>::T,AbstractIndex>::V;
public:
    static Uint32 const SECOND = FIRST + 1 + FIRST_MATCHING_IN_REST;
};

template <typename FirstFactor, typename SecondFactor>
struct SummationIsNaturalPairing_t
{
private:
    SummationIsNaturalPairing_t();
public:
    static bool const V = TypesAreEqual_f<FirstFactor,typename DualOf_f<SecondFactor>::T>::V; //true;
};

template <typename FactorTypeList, typename AbstractIndexTypeList, typename SummedAbstractIndexTypeList, typename AbstractIndex>
struct FactorsOfSummation_t
{
    typedef SummedAbstractIndexPairElementIndices_t<AbstractIndexTypeList,SummedAbstractIndexTypeList,AbstractIndex> Pair;
    typedef typename Element_f<FactorTypeList,Pair::FIRST>::T FirstFactor;
    typedef typename Element_f<FactorTypeList,Pair::SECOND>::T SecondFactor;
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM((FactorTypeList::LENGTH == AbstractIndexTypeList::LENGTH), MUST_HAVE_EQUAL_LENGTHS),
        STATIC_ASSERT_IN_ENUM__UNIQUE(HasBasedVectorSpaceStructure_f<FirstFactor>::V, MUST_BE_BASED_VECTOR_SPACE, FIRSTFACTOR),
        STATIC_ASSERT_IN_ENUM__UNIQUE(HasBasedVectorSpaceStructure_f<SecondFactor>::V, MUST_BE_BASED_VECTOR_SPACE, SECONDFACTOR)
    };
    FactorsOfSummation_t();
};

template <typename FactorTypeList, typename AbstractIndexTypeList, typename SummedAbstractIndexTypeList>
struct AllSummationsAreNaturalPairings_t
{
private:
    typedef FactorsOfSummation_t<FactorTypeList,
                                 AbstractIndexTypeList,
                                 SummedAbstractIndexTypeList,
                                 typename SummedAbstractIndexTypeList::HeadType> FactorsOfSummation;
    static bool const V_HEAD = SummationIsNaturalPairing_t<typename FactorsOfSummation::FirstFactor,
                                                           typename FactorsOfSummation::SecondFactor>::V;
    static bool const V_BODY = AllSummationsAreNaturalPairings_t<FactorTypeList,
                                                                 AbstractIndexTypeList,
                                                                 typename SummedAbstractIndexTypeList::BodyTypeList>::V;
    AllSummationsAreNaturalPairings_t();
public:
    static bool const V = V_HEAD && V_BODY;
};

template <typename FactorTypeList, typename AbstractIndexTypeList>
struct AllSummationsAreNaturalPairings_t<FactorTypeList,AbstractIndexTypeList,EmptyTypeList>
{
    static bool const V = true; // so this can be used in STATIC_ASSERT_IN_ENUM
private:
    AllSummationsAreNaturalPairings_t();
};

// this is designed to handle trace-type expression templates, such as u(i,i) or v(i,j,i)
// technically SummedDimIndexTypeList is a redundant argument (as it is derivable from TensorDimIndexTypeList),
// but it is necessary so that a template specialization can be made for when it is EmptyTypeList.
template <typename Tensor, typename TensorDimIndexTypeList, typename SummedDimIndexTypeList>
struct UnarySummation_t
{
private:
    typedef typename AbstractIndicesOfDimIndexTypeList_t<TensorDimIndexTypeList>::T AbstractIndexTypeList;
    typedef typename AbstractIndicesOfDimIndexTypeList_t<SummedDimIndexTypeList>::T SummedAbstractIndexTypeList;
    enum
    {
        STATIC_ASSERT_IN_ENUM((AllSummationsAreNaturalPairings_t<typename Tensor::FactorTypeList,
                                                                 AbstractIndexTypeList,
                                                                 SummedAbstractIndexTypeList>::V), ALL_SUMMATIONS_MUST_BE_NATURAL_PAIRINGS)
    };
public:

    typedef typename Tensor::Scalar Scalar;
    typedef typename FreeIndexTypeList_t<TensorDimIndexTypeList>::T FreeDimIndexTypeList;
    typedef MultiIndex_t<FreeDimIndexTypeList> MultiIndex;

    static Scalar eval (Tensor const &tensor, MultiIndex const &m)
    {
        typedef typename ConcatenationOfTypeLists_t<FreeDimIndexTypeList,SummedDimIndexTypeList>::T TotalDimIndexTypeList;
        typedef MultiIndex_t<TotalDimIndexTypeList> TotalMultiIndex;
        typedef MultiIndex_t<SummedDimIndexTypeList> SummedMultiIndex;

        // the operands take indices that are a subset of the summed indices and free indices.

        // constructing t with m initializes the first elements which correpond to
        // MultiIndex with the value of m, and initializes the remaining elements to zero.
        TotalMultiIndex t(m);
        Scalar retval(0);
        // get the map which produces the MultiIndex for each tensor from the TotalMultiIndex t
        typedef MultiIndexMap_t<TotalDimIndexTypeList,TensorDimIndexTypeList> TensorIndexMap;
        static typename TensorIndexMap::EvalMapType const tensor_index_map = TensorIndexMap::eval;
        // t = (f,s), which is a concatenation of the free access indices and the summed access indices.
        // s is a reference to the second part, which is what is iterated over in the summation.
        for (SummedMultiIndex &s = t.template trailing_list<FreeDimIndexTypeList::LENGTH>(); s.is_not_at_end(); ++s)
            // TODO: when the dual-vector-space/conceptual refactor is done, this summation_component_factor
            // should go away, since this is a non-natural pairing, and it causes C++ plumbing issues
            // (getting the C++ scalar type from the index, where the index will only be aware of the
            // abstract BasedVectorSpace).
            retval += tensor[tensor_index_map(t)];// * summation_component_factor(s);
        return retval;
    }
};

template <typename Tensor, typename TensorDimIndexTypeList>
struct UnarySummation_t<Tensor,TensorDimIndexTypeList,EmptyTypeList>
{
    // no summations to check the natural pairing for

    typedef typename Tensor::Scalar Scalar;
    typedef typename FreeIndexTypeList_t<TensorDimIndexTypeList>::T FreeDimIndexTypeList;
    typedef MultiIndex_t<FreeDimIndexTypeList> MultiIndex;

    static Scalar eval (Tensor const &tensor, MultiIndex const &m) { return tensor[m]; }
};

template <typename LeftOperand, typename RightOperand, typename FreeDimIndexTypeList, typename SummedDimIndexTypeList>
struct BinarySummation_t
{
private:
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeFactorTypeList,
                                                typename RightOperand::FreeFactorTypeList>::T FactorTypeList;
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeDimIndexTypeList,
                                                typename RightOperand::FreeDimIndexTypeList>::T DimIndexTypeList;
    typedef typename AbstractIndicesOfDimIndexTypeList_t<DimIndexTypeList>::T AbstractIndexTypeList;
    typedef typename AbstractIndicesOfDimIndexTypeList_t<SummedDimIndexTypeList>::T SummedAbstractIndexTypeList;
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<LeftOperand>::V, LEFT_OPERAND_IS_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<RightOperand>::V, RIGHT_OPERAND_IS_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM((TypesAreEqual_f<typename LeftOperand::Scalar,typename RightOperand::Scalar>::V), OPERAND_SCALAR_TYPES_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((SummedDimIndexTypeList::LENGTH > 0), LENGTH_MUST_BE_POSITIVE),
        STATIC_ASSERT_IN_ENUM((AllSummationsAreNaturalPairings_t<FactorTypeList,
                                                                 AbstractIndexTypeList,
                                                                 SummedAbstractIndexTypeList>::V), ALL_SUMMATIONS_MUST_BE_NATURAL_PAIRINGS)
    };
public:
    typedef typename LeftOperand::Scalar Scalar;
    typedef MultiIndex_t<FreeDimIndexTypeList> MultiIndex;

    static Scalar eval (LeftOperand const &left_operand, RightOperand const &right_operand, MultiIndex const &m)
    {
        typedef typename ConcatenationOfTypeLists_t<FreeDimIndexTypeList,SummedDimIndexTypeList>::T TotalDimIndexTypeList;
        typedef MultiIndex_t<TotalDimIndexTypeList> TotalMultiIndex;
        typedef MultiIndex_t<SummedDimIndexTypeList> SummedMultiIndex;

        // the operands take indices that are a subset of the summed indices and free indices.

        // constructing t with m initializes the first elements which correpond to
        // MultiIndex with the value of m, and initializes the remaining elements to zero.
        TotalMultiIndex t(m);
        Scalar retval(0);
        // get the map which produces the MultiIndex for each operand from the TotalMultiIndex t
        typedef MultiIndexMap_t<TotalDimIndexTypeList,typename LeftOperand::FreeDimIndexTypeList> LeftOperandIndexMap;
        typedef MultiIndexMap_t<TotalDimIndexTypeList,typename RightOperand::FreeDimIndexTypeList> RightOperandIndexMap;
        static typename LeftOperandIndexMap::EvalMapType const left_operand_index_map = LeftOperandIndexMap::eval;
        static typename RightOperandIndexMap::EvalMapType const right_operand_index_map = RightOperandIndexMap::eval;
        // t = (f,s), which is a concatenation of the free access indices and the summed access indices.
        // s is a reference to the second part, which is what is iterated over in the summation.
        for (SummedMultiIndex &s = t.template trailing_list<FreeDimIndexTypeList::LENGTH>(); s.is_not_at_end(); ++s)
            retval += left_operand[left_operand_index_map(t)] *
                      right_operand[right_operand_index_map(t)];// *
                      //summation_component_factor(s);
        return retval;
    }
};

// template specialization handles summation over no indices
template <typename LeftOperand, typename RightOperand, typename FreeDimIndexTypeList>
struct BinarySummation_t<LeftOperand,RightOperand,FreeDimIndexTypeList,EmptyTypeList>
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<LeftOperand>::V, LEFT_OPERAND_IS_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<RightOperand>::V, RIGHT_OPERAND_IS_EXPRESSION_TEMPLATE),
        STATIC_ASSERT_IN_ENUM((TypesAreEqual_f<typename LeftOperand::Scalar,typename RightOperand::Scalar>::V), OPERAND_SCALAR_TYPES_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<FreeDimIndexTypeList,IsDimIndex_p>::V), MUST_BE_TYPELIST_OF_DIM_INDEX_TYPES)
        // no summation, so no need to check naturality of pairings
    };
public:

    typedef typename LeftOperand::Scalar Scalar;
    typedef MultiIndex_t<FreeDimIndexTypeList> MultiIndex;

    static Scalar eval (LeftOperand const &left_operand, RightOperand const &right_operand, MultiIndex const &m)
    {
        // get the map which produces the MultiIndex for each operand from the free indices m
        typedef MultiIndexMap_t<FreeDimIndexTypeList,typename LeftOperand::FreeDimIndexTypeList> LeftOperandIndexMap;
        typedef MultiIndexMap_t<FreeDimIndexTypeList,typename RightOperand::FreeDimIndexTypeList> RightOperandIndexMap;
        static typename LeftOperandIndexMap::EvalMapType const left_operand_index_map = LeftOperandIndexMap::eval;
        static typename RightOperandIndexMap::EvalMapType const right_operand_index_map = RightOperandIndexMap::eval;
        return left_operand[left_operand_index_map(m)] * right_operand[right_operand_index_map(m)];
    }
};


template <typename LeftOperand, typename RightOperand>
struct FreeDimIndexTypeListOfMultiplication_t
{
private:
    // the free indices are the single-occurrence indices of the concatenated
    // list of free indices from the left and right operands
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeDimIndexTypeList,
                                                typename RightOperand::FreeDimIndexTypeList>::T CombinedFreeDimIndexTypeList;
    FreeDimIndexTypeListOfMultiplication_t();
public:
    typedef typename ElementsHavingMultiplicity_t<CombinedFreeDimIndexTypeList,1>::T T;
};

template <typename LeftOperand, typename RightOperand>
struct SummedDimIndexTypeListOfMultiplication_t
{
private:
    // the free indices are the single-occurrence indices of the concatenated
    // list of free indices from the left and right operands
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeDimIndexTypeList,
                                                typename RightOperand::FreeDimIndexTypeList>::T CombinedFreeDimIndexTypeList;
    SummedDimIndexTypeListOfMultiplication_t();
public:
    // the summed indices (at this level) are the double-occurrences indices
    // of the concatenated list of free indices from the left and right operands
    typedef typename ElementsHavingMultiplicity_t<CombinedFreeDimIndexTypeList,2>::T T;
};

template <typename LeftOperand, typename RightOperand>
struct UsedDimIndexTypeListOfMultiplication_t
{
private:
    typedef typename SummedDimIndexTypeListOfMultiplication_t<LeftOperand,RightOperand>::T SummedDimIndexTypeList;
    UsedDimIndexTypeListOfMultiplication_t();
public:
    // typelist of used indices which are prohibited from using higher up in the AST
    typedef typename UniqueTypesIn_t<
        typename ConcatenationOfTypeLists_t<
            typename ConcatenationOfTypeLists_t<typename LeftOperand::UsedDimIndexTypeList,
                                                typename RightOperand::UsedDimIndexTypeList>::T,
            SummedDimIndexTypeList>::T>::T T;
};

template <typename LeftOperand, typename RightOperand>
struct FreeFactorTypeListOfMultiplication_t
{
private:
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeFactorTypeList,
                                                typename RightOperand::FreeFactorTypeList>::T CombinedFactorTypeList;
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeDimIndexTypeList,
                                                typename RightOperand::FreeDimIndexTypeList>::T CombinedDimIndexTypeList;
    FreeFactorTypeListOfMultiplication_t();
public:
    typedef typename FreeFactorTypeList_t<CombinedFactorTypeList,CombinedDimIndexTypeList>::T T;
};



template <typename AbstractIndexTypeList, typename FactorTypeList, typename ExtractionAbstractIndexTypeList>
struct ExtractFactorsForAbstractIndices_t
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM((Occurrence_t<AbstractIndexTypeList,typename ExtractionAbstractIndexTypeList::HeadType>::COUNT == 1), MUST_OCCUR_EXACTLY_ONCE),
        STATIC_ASSERT_IN_ENUM((IsASubsetOf_t<ExtractionAbstractIndexTypeList,AbstractIndexTypeList>::V), MUST_BE_SUBSET_OF)
    };
    static Uint32 const INDEX = IndexOfFirstOccurrence_f<AbstractIndexTypeList,typename ExtractionAbstractIndexTypeList::HeadType>::V;
    ExtractFactorsForAbstractIndices_t();
public:
    typedef TypeList_t<typename Element_f<FactorTypeList,INDEX>::T,
                       typename ExtractFactorsForAbstractIndices_t<AbstractIndexTypeList,
                                                                   FactorTypeList,
                                                                   typename ExtractionAbstractIndexTypeList::BodyTypeList>::T> T;
};

template <typename AbstractIndexTypeList, typename FactorTypeList>
struct ExtractFactorsForAbstractIndices_t<AbstractIndexTypeList,FactorTypeList,EmptyTypeList>
{
    typedef EmptyTypeList T;
private:
    ExtractFactorsForAbstractIndices_t();
};

// TODO: Scalar will go away once the bundle map is moved into the conceptual layer.
template <typename Scalar, typename BundleDimIndexTypeList, typename ResultingFactorType, typename ResultingDimIndexType>
struct BundleIndexMap_t
{
    typedef MultiIndex_t<BundleDimIndexTypeList> (*T) (ResultingDimIndexType const &);
    static T const V;
private:
    BundleIndexMap_t();
};

// TODO: the use of UseMemberArray_t<COMPONENTS_ARE_NONCONST> is somewhat arbitrary -- should this be addressed somehow?
template <typename Scalar, typename BundleDimIndexTypeList, typename ResultingFactorType, typename ResultingDimIndexType>
typename BundleIndexMap_t<Scalar,BundleDimIndexTypeList,ResultingFactorType,ResultingDimIndexType>::T const BundleIndexMap_t<Scalar,BundleDimIndexTypeList,ResultingFactorType,ResultingDimIndexType>::V =
    ImplementationOf_t<ResultingFactorType,Scalar,UseMemberArray_t<COMPONENTS_ARE_NONCONST> >::template bundle_index_map<BundleDimIndexTypeList,ResultingDimIndexType>;

// TEMP: until the indexed expressions are flexible enough to safely do
// head/body recursion (e.g. in tensor product of procedural 2-tensors)
static bool const DONT_CHECK_FACTOR_TYPES = true;
static bool const CHECK_FACTOR_TYPES = false;

// not an expression template, but just something that handles the bundled indices
template <typename Operand, typename BundleAbstractIndexTypeList, typename ResultingFactorType, typename ResultingAbstractIndexType, bool DONT_CHECK_FACTOR_TYPES_>
struct IndexBundle_t
{
    typedef typename AbstractIndicesOfDimIndexTypeList_t<typename Operand::FreeDimIndexTypeList>::T OperandFreeAbstractIndexTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_VECTOR_SPACES_UNIQUELY(ResultingFactorType), MUST_BE_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(IsAbstractIndex_f<ResultingAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX),
        STATIC_ASSERT_IN_ENUM((IsASubsetOf_t<BundleAbstractIndexTypeList,OperandFreeAbstractIndexTypeList>::V), BUNDLE_INDICES_MUST_BE_FREE),
        STATIC_ASSERT_IN_ENUM((!Contains_f<BundleAbstractIndexTypeList,ResultingAbstractIndexType>::V), BUNDLE_AND_RESULTING_MUST_BE_DISTINCT),
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<Operand>::V, OPERAND_IS_EXPRESSION_TEMPLATE)
    };

    // if Operand's free indices are i*j*k*l*m*n, the bundle indices are i*k*l, and the resulting
    // index is P, then the transformation is
    //      i*j*k*l*m*n --> j*m*n*i*k*l --> j*m*n*P
    // the first transformation being putting the bundle indices at the back (preserving their order),
    // and the second one transforming them into the bundled index.  the component access goes backward
    // in this diagram (components are accessed via the indices j*m*n*P, the bundle map is applied to
    // P to get i*k*l, and then j*m*n*i*k*l is transformed to i*j*k*l*m*n to get the component from
    // Operand).

    typedef typename DimIndexTypeListOf_t<typename Operand::FreeFactorTypeList,
                                          OperandFreeAbstractIndexTypeList>::T OperandFreeDimIndexTypeList;
    typedef typename ExtractFactorsForAbstractIndices_t<OperandFreeAbstractIndexTypeList,
                                                        typename Operand::FreeFactorTypeList,
                                                        BundleAbstractIndexTypeList>::T BundleFactorTypeList;
    typedef typename DimIndexTypeListOf_t<BundleFactorTypeList,
                                          BundleAbstractIndexTypeList>::T BundleDimIndexTypeList;
    typedef DimIndex_t<ResultingAbstractIndexType::SYMBOL,
                       DimensionOf_f<ResultingFactorType>::V> ResultingDimIndexType;

private:

    typedef typename FactorTypeListOf_f<typename AS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(ResultingFactorType)::TensorProductOfBasedVectorSpaces>::T ResultingFactorTypeFactorTypeList;
    enum
    {
        STATIC_ASSERT_IN_ENUM(DONT_CHECK_FACTOR_TYPES_ ||
                              (TypesAreEqual_f<BundleFactorTypeList,ResultingFactorTypeFactorTypeList>::V),
                              BUNDLE_FACTORS_MUST_MATCH)
    };

public:

    // zip the stuff so that the transformations can act on both the DimIndex_t and factor lists
    typedef typename Zip_t<TypeList_t<OperandFreeDimIndexTypeList,
                           TypeList_t<typename Operand::FreeFactorTypeList> > >::T OperandFreeDimIndexAndFactorTypeList;
    typedef TypeList_t<ResultingDimIndexType,TypeList_t<ResultingFactorType> > ResultingDimIndexAndFactorType;
    typedef typename Zip_t<TypeList_t<BundleDimIndexTypeList,
                           TypeList_t<BundleFactorTypeList> > >::T BundleDimIndexAndFactorTypeList;

    // ResultingDimIndexAndFactorType comes last.  DimIndexAndFactorTypeList may have summed indices
    typedef typename SetSubtraction_t<
        typename ConcatenationOfTypeLists_t<OperandFreeDimIndexAndFactorTypeList,
                                            TypeList_t<ResultingDimIndexAndFactorType> >::T,
        BundleDimIndexAndFactorTypeList>::T DimIndexAndFactorTypeList;
    // this seems like it should be the same as OperandFreeDimIndexAndFactorTypeList, but it has
    // the indices in BundleDimIndexAndFactorTypeList coming last.
    typedef typename ConcatenationOfTypeLists_t<
        typename SetSubtraction_t<OperandFreeDimIndexAndFactorTypeList,BundleDimIndexAndFactorTypeList>::T,
        BundleDimIndexAndFactorTypeList>::T UnpackedDimIndexAndFactorTypeList;

    // unzip the stuff
    typedef typename Unzip_t<DimIndexAndFactorTypeList>::T DimIndexAndFactorTypeList_Unzipped;
    typedef typename Unzip_t<UnpackedDimIndexAndFactorTypeList>::T UnpackedDimIndexAndFactorTypeList_Unzipped;
    typedef typename DimIndexAndFactorTypeList_Unzipped::BodyTypeList::HeadType FactorTypeList;
    typedef typename DimIndexAndFactorTypeList_Unzipped::HeadType DimIndexTypeList;
    typedef typename UnpackedDimIndexAndFactorTypeList_Unzipped::HeadType UnpackedDimIndexTypeList;

    // finally, define the multi-index and bundle index map.
    typedef MultiIndex_t<DimIndexTypeList> MultiIndex;
    typedef typename Operand::Scalar Scalar;
    typedef BundleIndexMap_t<Scalar,
                             BundleDimIndexTypeList,
                             ResultingFactorType,
                             ResultingDimIndexType> BundleIndexMap;

    IndexBundle_t (Operand const &operand) : m_operand(operand) { }

    Scalar operator [] (MultiIndex const &m) const
    {
        // replace the head of m with the separate indices that it bundles.
        // use MultiIndexMap_t to place the indices in the correct order.
        typedef MultiIndexMap_t<UnpackedDimIndexTypeList,typename Operand::FreeDimIndexTypeList> OperandIndexMap;
        static typename OperandIndexMap::EvalMapType const operand_index_map = OperandIndexMap::eval;
        static typename BundleIndexMap::T const bundle_index_map = BundleIndexMap::V;
        // | is concatenation of MultiIndex_t instances
        return m_operand[operand_index_map(m.template leading_list<MultiIndex::LENGTH-1>()
                                           |
                                           bundle_index_map(m.template el<MultiIndex::LENGTH-1>()))];
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_operand.overlaps_memory_range(ptr, range);
    }

    Operand const &operand () const { return m_operand; }

private:

    void operator = (IndexBundle_t const &);

    Operand m_operand;
};

// not an expression template, but just something that handles the split indices
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTypeList>
struct IndexSplitter_t
{
    typedef typename AbstractIndicesOfDimIndexTypeList_t<typename Operand::FreeDimIndexTypeList>::T OperandFreeAbstractIndexTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsAbstractIndex_f<SourceAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX),
        STATIC_ASSERT_IN_ENUM((Contains_f<OperandFreeAbstractIndexTypeList,SourceAbstractIndexType>::V), SOURCE_INDEX_MUST_BE_FREE),
        STATIC_ASSERT_IN_ENUM((!HasNontrivialIntersectionAsSets_t<TypeList_t<SourceAbstractIndexType>,SplitAbstractIndexTypeList>::V), SOURCE_AND_SPLIT_MUST_BE_DISTINCT),
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<Operand>::V, OPERAND_IS_EXPRESSION_TEMPLATE)
    };

    typedef typename Operand::Scalar Scalar;
    // we must replace SourceAbstractIndexType with MultiIndex_t<SplitAbstractIndexTypeList> in the index type list
    static Uint32 const SOURCE_INDEX_TYPE_INDEX = IndexOfFirstOccurrence_f<OperandFreeAbstractIndexTypeList,SourceAbstractIndexType>::V;
    typedef typename Element_f<typename Operand::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX>::T SourceFactor;

    typedef typename AS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(SourceFactor)::TensorProductOfBasedVectorSpaces EmbeddingTensorProduct;

    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE((OrderOf_f<EmbeddingTensorProduct>::V == SplitAbstractIndexTypeList::LENGTH), MUST_HAVE_EQUAL_LENGTHS, FREEFACTORTYPELIST)
    };

    typedef typename ConcatenationOfTypeLists_t<
        typename LeadingTypeList_f<typename Operand::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX>::T,
        typename ConcatenationOfTypeLists_t<
            typename FactorTypeListOf_f<EmbeddingTensorProduct>::T,
            typename TrailingTypeList_f<typename Operand::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX+1>::T
            >::T
        >::T FactorTypeList;
    typedef typename ConcatenationOfTypeLists_t<
        typename LeadingTypeList_f<typename Operand::FreeDimIndexTypeList,SOURCE_INDEX_TYPE_INDEX>::T,
        typename ConcatenationOfTypeLists_t<
            SplitAbstractIndexTypeList,
            typename TrailingTypeList_f<typename Operand::FreeDimIndexTypeList,SOURCE_INDEX_TYPE_INDEX+1>::T
            >::T
        >::T AbstractIndexTypeList;
    typedef typename DimIndexTypeListOf_t<FactorTypeList,AbstractIndexTypeList>::T DimIndexTypeList;
    typedef typename ConcatenationOfTypeLists_t<typename Operand::UsedDimIndexTypeList,SplitAbstractIndexTypeList>::T UsedDimIndexTypeList;
    typedef MultiIndex_t<DimIndexTypeList> MultiIndex;

    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE((FactorTypeList::LENGTH == DimIndexTypeList::LENGTH), MUST_HAVE_EQUAL_LENGTHS, FACTORTYPELIST)
    };

    IndexSplitter_t (Operand const &operand) : m_operand(operand) { }

    Scalar operator [] (MultiIndex const &m) const
    {
        typedef typename DimIndexTypeListOf_t<typename FactorTypeListOf_f<typename AS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_VECTOR_SPACES(SourceFactor)::TensorProductOfVectorSpaces>::T,
                                              SplitAbstractIndexTypeList>::T SourceFactorDimIndexTypeList;
        typedef ComponentIndex_t<DimensionOf_f<SourceFactor>::V> SourceFactorComponentIndex;
        typedef MultiIndex_t<SourceFactorDimIndexTypeList> SourceFactorMultiIndex;
        // TODO: the use of UseMemberArray_t<COMPONENTS_ARE_NONCONST> here is arbitrary because it's just used to access a
        // static method.  figure out if this is a problem
        typedef ImplementationOf_t<SourceFactor,Scalar,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > ImplementationOfSourceFactor;

        SourceFactorMultiIndex s(m.template range<SOURCE_INDEX_TYPE_INDEX,SOURCE_INDEX_TYPE_INDEX+SplitAbstractIndexTypeList::LENGTH>());
        if (ImplementationOfSourceFactor::component_is_procedural_zero(s))
            return Scalar(0);

        SourceFactorComponentIndex i(ImplementationOfSourceFactor::vector_index_of(s));
        // this replaces the SplitAbstractIndexTypeList portion with SourceAbstractIndexType
        typename Operand::MultiIndex c_rebundled(m.template leading_list<SOURCE_INDEX_TYPE_INDEX>()
                                                 |
                                                 (i >>= m.template trailing_list<SOURCE_INDEX_TYPE_INDEX+SplitAbstractIndexTypeList::LENGTH>()));
        return ImplementationOfSourceFactor::scalar_factor_for_component(s) * m_operand[c_rebundled];
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_operand.overlaps_memory_range(ptr, range);
    }

    Operand const &operand () const { return m_operand; }

private:

    void operator = (IndexSplitter_t const &);

    Operand m_operand;
};

// not an expression template, but just something that handles the split indices
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
struct IndexSplitToIndex_t
{
    typedef typename AbstractIndicesOfDimIndexTypeList_t<typename Operand::FreeDimIndexTypeList>::T OperandFreeAbstractIndexTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsAbstractIndex_f<SourceAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX, SOURCE),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsAbstractIndex_f<SplitAbstractIndexType>::V, MUST_BE_ABSTRACT_INDEX, SPLIT),
        STATIC_ASSERT_IN_ENUM((Contains_f<OperandFreeAbstractIndexTypeList,SourceAbstractIndexType>::V), SOURCE_INDEX_MUST_BE_FREE),
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<SourceAbstractIndexType,SplitAbstractIndexType>::V), SOURCE_AND_SPLIT_MUST_BE_DISTINCT),
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<Operand>::V, OPERAND_IS_EXPRESSION_TEMPLATE)
    };

    typedef typename Operand::Scalar Scalar;
    // we must replace SourceAbstractIndexType with SplitAbstractIndexType in the index type list
    static Uint32 const SOURCE_INDEX_TYPE_INDEX = IndexOfFirstOccurrence_f<OperandFreeAbstractIndexTypeList,SourceAbstractIndexType>::V;
    typedef typename Element_f<typename Operand::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX>::T SourceFactor;

    typedef typename AS_EMBEDDABLE_IN_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(SourceFactor)::TensorProductOfBasedVectorSpaces EmbeddingTensorProduct;
    typedef typename ConcatenationOfTypeLists_t<
        typename LeadingTypeList_f<typename Operand::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX>::T,
        TypeList_t<EmbeddingTensorProduct,
                   typename TrailingTypeList_f<typename Operand::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX+1>::T>
        >::T FactorTypeList;
    typedef typename ConcatenationOfTypeLists_t<
        typename LeadingTypeList_f<typename Operand::FreeDimIndexTypeList,SOURCE_INDEX_TYPE_INDEX>::T,
        TypeList_t<SplitAbstractIndexType,
                   typename TrailingTypeList_f<typename Operand::FreeDimIndexTypeList,SOURCE_INDEX_TYPE_INDEX+1>::T>
        >::T AbstractIndexTypeList;
    typedef typename DimIndexTypeListOf_t<FactorTypeList,AbstractIndexTypeList>::T DimIndexTypeList;
    typedef typename ConcatenationOfTypeLists_t<typename Operand::UsedDimIndexTypeList,TypeList_t<SplitAbstractIndexType> >::T UsedDimIndexTypeList;
    typedef MultiIndex_t<DimIndexTypeList> MultiIndex;

    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE((FactorTypeList::LENGTH == DimIndexTypeList::LENGTH), MUST_HAVE_EQUAL_LENGTHS, FACTORTYPELIST)
    };

    IndexSplitToIndex_t (Operand const &operand) : m_operand(operand) { }

    Scalar operator [] (MultiIndex const &m) const
    {
        typedef ComponentIndex_t<DimensionOf_f<SourceFactor>::V> SourceFactorComponentIndex;
        // TODO: the use of UseMemberArray_t<COMPONENTS_ARE_NONCONST> here is arbitrary because it's just used to access a
        // static method.  figure out if this is a problem
        typedef ImplementationOf_t<SourceFactor,Scalar,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > ImplementationOfSourceFactor;
        typedef typename ImplementationOfSourceFactor::MultiIndex SourceFactorMultiIndex;

        // this does the vector-index to multi-index conversion
        SourceFactorMultiIndex s(m.template el<SOURCE_INDEX_TYPE_INDEX>());
        if (ImplementationOfSourceFactor::component_is_procedural_zero(s))
            return Scalar(0);

        SourceFactorComponentIndex i(ImplementationOfSourceFactor::vector_index_of(s));
        // this replaces the SplitAbstractIndexType_ portion with SourceAbstractIndexType
        typename Operand::MultiIndex c_rebundled(m.template leading_list<SOURCE_INDEX_TYPE_INDEX>()
                                                 |
                                                 (i >>= m.template trailing_list<SOURCE_INDEX_TYPE_INDEX+1>()));
        return ImplementationOfSourceFactor::scalar_factor_for_component(s) * m_operand[c_rebundled];
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_operand.overlaps_memory_range(ptr, range);
    }

    Operand const &operand () const { return m_operand; }

private:

    void operator = (IndexSplitToIndex_t const &);

    Operand m_operand;
};

// not an expression template, but just something that handles the embedded indices
template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename EmbeddingCodomain_,
          typename EmbeddedAbstractIndexType_,
          typename EmbeddingId_>
struct IndexEmbedder_t
{
    typedef typename AbstractIndicesOfDimIndexTypeList_t<typename Operand_::FreeDimIndexTypeList>::T OperandFreeAbstractIndexTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsAbstractIndex_f<SourceAbstractIndexType_>::V, MUST_BE_ABSTRACT_INDEX, SOURCE),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsAbstractIndex_f<EmbeddedAbstractIndexType_>::V, MUST_BE_ABSTRACT_INDEX, EMBEDDED),
        STATIC_ASSERT_IN_ENUM((Contains_f<OperandFreeAbstractIndexTypeList,SourceAbstractIndexType_>::V), SOURCE_INDEX_MUST_BE_FREE),
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<SourceAbstractIndexType_,EmbeddedAbstractIndexType_>::V), SOURCE_AND_EMBEDDED_MUST_BE_DISTINCT),
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<Operand_>::V, OPERAND_IS_EXPRESSION_TEMPLATE)
    };

    typedef typename Operand_::Scalar Scalar;
    // we must replace SourceAbstractIndexType_ with EmbeddedAbstractIndexType_ in the index type list
    static Uint32 const SOURCE_INDEX_TYPE_INDEX = IndexOfFirstOccurrence_f<OperandFreeAbstractIndexTypeList,SourceAbstractIndexType_>::V;
    typedef typename Element_f<typename Operand_::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX>::T EmbeddingDomain;

    typedef typename ConcatenationOfTypeLists_t<
        typename LeadingTypeList_f<typename Operand_::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX>::T,
        TypeList_t<EmbeddingCodomain_,
                   typename TrailingTypeList_f<typename Operand_::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX+1>::T>
        >::T FactorTypeList;
    typedef typename ConcatenationOfTypeLists_t<
        typename LeadingTypeList_f<typename Operand_::FreeDimIndexTypeList,SOURCE_INDEX_TYPE_INDEX>::T,
        TypeList_t<EmbeddedAbstractIndexType_,
                   typename TrailingTypeList_f<typename Operand_::FreeDimIndexTypeList,SOURCE_INDEX_TYPE_INDEX+1>::T>
        >::T AbstractIndexTypeList;
    typedef typename DimIndexTypeListOf_t<FactorTypeList,AbstractIndexTypeList>::T DimIndexTypeList;
    typedef typename ConcatenationOfTypeLists_t<typename Operand_::UsedDimIndexTypeList,TypeList_t<EmbeddedAbstractIndexType_> >::T UsedDimIndexTypeList;
    typedef MultiIndex_t<DimIndexTypeList> MultiIndex;

    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE((FactorTypeList::LENGTH == DimIndexTypeList::LENGTH), MUST_HAVE_EQUAL_LENGTHS, FACTORTYPELIST)
    };

    IndexEmbedder_t (Operand_ const &operand) : m_operand(operand) { }

    Scalar operator [] (MultiIndex const &m) const
    {
        typedef ComponentIndex_t<DimensionOf_f<EmbeddingCodomain_>::V> EmbeddingCodomainComponentIndex;
        typedef ComponentIndex_t<DimensionOf_f<EmbeddingDomain>::V> EmbeddingDomainComponentIndex;
        // TODO: the use of UseMemberArray_t<COMPONENTS_ARE_NONCONST> here is arbitrary because it's just used to access a
        // static method.  figure out if this is a problem
        typedef ImplementationOf_t<EmbeddingDomain,Scalar,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > ImplementationOfSourceFactor;
        typedef typename ImplementationOfSourceFactor::MultiIndex EmbeddingDomainMultiIndex;

        EmbeddingCodomainComponentIndex j(m.template el<SOURCE_INDEX_TYPE_INDEX>());

        typedef LinearEmbedding_c<EmbeddingDomain,EmbeddingCodomain_,Scalar,EmbeddingId_> LinearEmbedding;
        if (LinearEmbedding::embedded_component_is_procedural_zero(j))
            return Scalar(0);

        EmbeddingDomainComponentIndex i(LinearEmbedding::source_component_index_for_embedded_component(j));
        // this replaces the EmbeddedAbstractIndexType_ portion with SourceAbstractIndexType_
        typename Operand_::MultiIndex c_rebundled(m.template leading_list<SOURCE_INDEX_TYPE_INDEX>()
                                                 |
                                                 (i >>= m.template trailing_list<SOURCE_INDEX_TYPE_INDEX+1>()));
        return LinearEmbedding::scalar_factor_for_embedded_component(j) * m_operand[c_rebundled];
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_operand.overlaps_memory_range(ptr, range);
    }

    Operand_ const &operand () const { return m_operand; }

private:

    void operator = (IndexEmbedder_t const &);

    Operand_ m_operand;
};

// not an expression template, but just something that handles the embedded indices
template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename CoembeddingCodomain_,
          typename CoembeddedAbstractIndexType_,
          typename EmbeddingId_>
struct IndexCoembedder_t
{
    typedef typename AbstractIndicesOfDimIndexTypeList_t<typename Operand_::FreeDimIndexTypeList>::T OperandFreeAbstractIndexTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsAbstractIndex_f<SourceAbstractIndexType_>::V, MUST_BE_ABSTRACT_INDEX, SOURCE),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsAbstractIndex_f<CoembeddedAbstractIndexType_>::V, MUST_BE_ABSTRACT_INDEX, EMBEDDED),
        STATIC_ASSERT_IN_ENUM((Contains_f<OperandFreeAbstractIndexTypeList,SourceAbstractIndexType_>::V), SOURCE_INDEX_MUST_BE_FREE),
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<SourceAbstractIndexType_,CoembeddedAbstractIndexType_>::V), SOURCE_AND_EMBEDDED_MUST_BE_DISTINCT),
        STATIC_ASSERT_IN_ENUM(IsExpressionTemplate_f<Operand_>::V, OPERAND_IS_EXPRESSION_TEMPLATE)
    };

    typedef typename Operand_::Scalar Scalar;
    // we must replace SourceAbstractIndexType_ with CoembeddedAbstractIndexType_ in the index type list
    static Uint32 const SOURCE_INDEX_TYPE_INDEX = IndexOfFirstOccurrence_f<OperandFreeAbstractIndexTypeList,SourceAbstractIndexType_>::V;
    typedef typename Element_f<typename Operand_::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX>::T CoembeddingDomain;

    typedef typename ConcatenationOfTypeLists_t<
        typename LeadingTypeList_f<typename Operand_::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX>::T,
        TypeList_t<CoembeddingCodomain_,
                   typename TrailingTypeList_f<typename Operand_::FreeFactorTypeList,SOURCE_INDEX_TYPE_INDEX+1>::T>
        >::T FactorTypeList;
    typedef typename ConcatenationOfTypeLists_t<
        typename LeadingTypeList_f<typename Operand_::FreeDimIndexTypeList,SOURCE_INDEX_TYPE_INDEX>::T,
        TypeList_t<CoembeddedAbstractIndexType_,
                   typename TrailingTypeList_f<typename Operand_::FreeDimIndexTypeList,SOURCE_INDEX_TYPE_INDEX+1>::T>
        >::T AbstractIndexTypeList;
    typedef typename DimIndexTypeListOf_t<FactorTypeList,AbstractIndexTypeList>::T DimIndexTypeList;
    typedef typename ConcatenationOfTypeLists_t<typename Operand_::UsedDimIndexTypeList,TypeList_t<CoembeddedAbstractIndexType_> >::T UsedDimIndexTypeList;
    typedef MultiIndex_t<DimIndexTypeList> MultiIndex;

    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE((FactorTypeList::LENGTH == DimIndexTypeList::LENGTH), MUST_HAVE_EQUAL_LENGTHS, FACTORTYPELIST)
    };

    IndexCoembedder_t (Operand_ const &operand) : m_operand(operand) { }

    Scalar operator [] (MultiIndex const &m) const
    {
        typedef ComponentIndex_t<DimensionOf_f<CoembeddingCodomain_>::V> CoembeddingCodomainComponentIndex;
        typedef ComponentIndex_t<DimensionOf_f<CoembeddingDomain>::V> CoembeddingDomainComponentIndex;
        // TODO: the use of UseMemberArray_t<COMPONENTS_ARE_NONCONST> here is arbitrary because it's just used to access a
        // static method.  figure out if this is a problem
        typedef ImplementationOf_t<CoembeddingDomain,Scalar,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > ImplementationOfSourceFactor;
        typedef typename ImplementationOfSourceFactor::MultiIndex CoembeddingDomainMultiIndex;

        CoembeddingCodomainComponentIndex j(m.template el<SOURCE_INDEX_TYPE_INDEX>());

        // the domain and codomain are reversed because of a contravariance property
        typedef LinearEmbedding_c<CoembeddingCodomain_,CoembeddingDomain,Scalar,EmbeddingId_> LinearEmbedding;
        Scalar retval(0);
        Uint32 term_count = LinearEmbedding::term_count_for_coembedded_component(j);
        for (Uint32 term = 0; term < term_count; ++term)
        {
            CoembeddingDomainComponentIndex i(LinearEmbedding::source_component_index_for_coembedded_component(j, term));
            // this replaces the CoembeddedAbstractIndexType_ portion with SourceAbstractIndexType_
            typename Operand_::MultiIndex c_rebundled(m.template leading_list<SOURCE_INDEX_TYPE_INDEX>()
                                                     |
                                                     (i >>= m.template trailing_list<SOURCE_INDEX_TYPE_INDEX+1>()));
            retval += LinearEmbedding::scalar_factor_for_coembedded_component(j, term) * m_operand[c_rebundled];
        }
        return retval;
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_operand.overlaps_memory_range(ptr, range);
    }

    Operand_ const &operand () const { return m_operand; }

private:

    void operator = (IndexCoembedder_t const &);

    Operand_ m_operand;
};

} // end of namespace Tenh

#endif // TENH_EXPRESSION_TEMPLATES_UTILITY_HPP_
