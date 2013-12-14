// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/directsum.hpp by Ted Nitz, created 2013/10/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_DIRECT_SUM_HPP_
#define TENH_IMPLEMENTATION_DIRECT_SUM_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/directsum.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/interface/vector.hpp"
#include "tenh/meta/tuple.hpp"

namespace Tenh {

template <typename SummandTypeList_, Uint32 N>
struct OffsetForComponent_f
{
private:
    enum { STATIC_ASSERT_IN_ENUM((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END) };
public:
    static const Uint32 V = DimensionOf_f<typename SummandTypeList_::HeadType>::V + OffsetForComponent_f<typename SummandTypeList_::BodyTypeList, N-1>::V;
};

template <typename SummandTypeList_>
struct OffsetForComponent_f<SummandTypeList_,0>
{
    static const Uint32 V = 0;
};

inline Uint32 component_for_offset (EmptyTypeList const &, Uint32 offset)
{
    assert(false && "this should never actually be called");
    return 0;
}

template <typename SummandTypeList_>
Uint32 component_for_offset (SummandTypeList_ const &, Uint32 offset)
{
    STATIC_ASSERT((Length_f<SummandTypeList_>::V > 0), LENGTH_MUST_BE_POSITIVE);
    typedef typename Head_f<SummandTypeList_>::T HeadSummand;
    typedef typename Body_f<SummandTypeList_>::T BodySummand;
    if (offset < DimensionOf_f<HeadSummand>::V)
        return 0;
    else
        return component_for_offset(BodySummand(), offset - DimensionOf_f<HeadSummand>::V);
}

template <typename SummandTypeList_, typename Scalar_, typename UseArrayType_, typename Derived_>
struct ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_,Derived_>
    :
    public Vector_i<typename DerivedType_f<Derived_,ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_,Derived_> >::T,
                    Scalar_,
                    DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,
                    ComponentsAreImmutable_f<UseArrayType_>::V>,
    // privately inherited because it is an implementation detail
    private ArrayStorage_f<Scalar_,
                           DimensionOf_f<DirectSumOfBasedVectorSpaces_c<SummandTypeList_> >::V,
                           UseArrayType_,
                           typename DerivedType_f<Derived_,ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_,Derived_> >::T >::T
{
    typedef Vector_i<typename DerivedType_f<Derived_,ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_,Derived_> >::T,
                     Scalar_,
                     DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,
                     ComponentsAreImmutable_f<UseArrayType_>::V> Parent_Vector_i;
    typedef typename ArrayStorage_f<Scalar_,
                                    DimensionOf_f<DirectSumOfBasedVectorSpaces_c<SummandTypeList_> >::V,
                                    UseArrayType_,
                                    typename DerivedType_f<Derived_,ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_,Derived_> >::T >::T Parent_Array_i;

    typedef DirectSumOfBasedVectorSpaces_c<SummandTypeList_> Concept;
    typedef UseArrayType_ UseArrayType;
    typedef typename Parent_Vector_i::Derived Derived;
    typedef typename Parent_Vector_i::Scalar Scalar;
    typedef typename Parent_Vector_i::BasedVectorSpace BasedVectorSpace;
    using Parent_Vector_i::DIM;
    typedef typename Parent_Vector_i::ComponentIndex ComponentIndex;
    typedef typename Parent_Vector_i::MultiIndex MultiIndex;

    using Parent_Array_i::COMPONENTS_ARE_IMMUTABLE;
    typedef typename Parent_Array_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_Array_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;

    typedef typename DualOf_f<ImplementationOf_t>::T Dual; // relies on the template specialization below

    typedef ImplementationOf_t<Concept,
                               Scalar_,
                               UseImmutableArray_t<typename ComponentGenerator_Constant_f<Scalar_,DIM,0>::T>,
                               Derived_> Zero;
    static Zero const ZERO;

    template <Uint32 INDEX_>
    struct BasisVector_f
    {
    private:
        enum { STATIC_ASSERT_IN_ENUM((INDEX_ < Parent_Vector_i::DIM), INDEX_OUT_OF_RANGE) };
        BasisVector_f () { }
    public:
        typedef ImplementationOf_t<Concept,
                                   Scalar_,
                                   UseImmutableArray_t<typename ComponentGenerator_Characteristic_f<Scalar_,Parent_Vector_i::DIM,INDEX_>::T>,
                                   Derived_> T;
        static T const V;
    };

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_i(w) { }

    // only use these if UseMemberArray is specified

    // probably only useful for zero element (because this is basis-dependent)
    template <typename T_>
    explicit ImplementationOf_t (FillWith_t<T_> const &fill_with)
        :
        Parent_Array_i(fill_with)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    ImplementationOf_t (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x)
        :
        Parent_Array_i(x.as_member_array())
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }

    // only use these if UsePreallocatedArray is specified

    explicit ImplementationOf_t (Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    template <typename T_>
    ImplementationOf_t (FillWith_t<T_> const &fill_with,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(fill_with, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    ImplementationOf_t (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(x, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }

    // only use this if UseImmutableArray_t<...> is specified or if the vector space is 0-dimensional
    ImplementationOf_t ()
        :
        Parent_Array_i(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseImmutableArray_f<UseArrayType_>::V || DIM == 0, MUST_BE_USE_IMMUTABLE_ARRAY_OR_BE_ZERO_DIMENSIONAL);
    }

    using Parent_Array_i::as_derived;
    using Parent_Array_i::operator[];
    using Parent_Array_i::allocation_size_in_bytes;
    using Parent_Array_i::pointer_to_allocation;
    using Parent_Array_i::overlaps_memory_range;

    template <Uint32 N>
    struct ElementType_f
    {
    private:
        enum { STATIC_ASSERT_IN_ENUM((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END) };
    public:
        typedef ImplementationOf_t<typename Element_f<SummandTypeList_, N>::T,
                                   Scalar_,
                                   typename If_f<IsUseImmutableArray_f<UseArrayType_>::V,
                                                 UseArrayType_,
                                                 UsePreallocatedArray>::T > T;
    };

    template <Uint32 N>
    struct ConstElementType_f
    {
    private:
        enum { STATIC_ASSERT_IN_ENUM((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END) };
    public:
        typedef ImplementationOf_t<typename Element_f<SummandTypeList_, N>::T,
                                   Scalar_ const,
                                   typename If_f<IsUseImmutableArray_f<UseArrayType_>::V,
                                                 UseArrayType_,
                                                 UsePreallocatedArray>::T > T;
    };

    template <Uint32 N>
    typename ElementType_f<N>::T el ()
    {
        STATIC_ASSERT((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END);
        return typename ElementType_f<N>::T(pointer_to_allocation() + OffsetForComponent_f<SummandTypeList_,N>::V);
    }

    template <Uint32 N>
    typename ConstElementType_f<N>::T el () const
    {
        STATIC_ASSERT((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END);
        return typename ConstElementType_f<N>::T(pointer_to_allocation() + OffsetForComponent_f<SummandTypeList_,N>::V);
    }

    typename ElementType_f<0>::T el (Uint32 n)
    {
        STATIC_ASSERT(TypeListIsUniform_t<SummandTypeList_>::V, TYPELIST_MUST_BE_UNIFORM);
        return typename ElementType_f<0>::T(pointer_to_allocation() + DimensionOf_f<typename SummandTypeList_::HeadType>::V * n);
    }

    typename ConstElementType_f<0>::T el (Uint32 n) const
    {
        STATIC_ASSERT(TypeListIsUniform_t<SummandTypeList_>::V, TYPELIST_MUST_BE_UNIFORM);
        return typename ConstElementType_f<0>::T(pointer_to_allocation() + DimensionOf_f<typename SummandTypeList_::HeadType>::V * n);
    }

    // These versions of el<...> are intended to allow use like el<n>(i) rather than the more clunky el<n>()(i) to get an indexed expression.
    // Unfortunately the indexed expression type stores a reference to the ImplementationOf_t internally, and in this code that object is
    // a temporary, and so the behavior is undefined.
    // template <Uint32 N, AbstractIndexSymbol SYMBOL_>
    // typename ElementType_f<N>::T::template IndexedExpressionNonConstType_f<SYMBOL_>::T el(AbstractIndex_c<SYMBOL_> const & i)
    // {
    //     STATIC_ASSERT((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END);
    //     return typename ElementType_f<N>::T(pointer_to_allocation() + OffsetForComponent_f<SummandTypeList_,N>::V)(i);
    // }
    //
    // template <Uint32 N, AbstractIndexSymbol SYMBOL_>
    // typename ElementType_f<N>::T::template IndexedExpressionConstType_f<SYMBOL_>::T el(AbstractIndex_c<SYMBOL_> const & i) const
    // {
    //     STATIC_ASSERT((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END);
    //     return typename ElementType_f<N>::T(pointer_to_allocation() + OffsetForComponent_f<SummandTypeList_,N>::V)(i);
    // }

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + type_string_of<DirectSumOfBasedVectorSpaces_c<SummandTypeList_> >() + ','
                                     + type_string_of<Scalar>() + ','
                                     + type_string_of<UseArrayType_>() + '>';
    }
};

template <typename SummandTypeList_, typename Scalar_, typename UseArrayType_, typename Derived_>
typename ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_,Derived_>::Zero const ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_,Derived_>::ZERO;

template <typename SummandTypeList_, typename Scalar_, typename UseArrayType_, typename Derived_>
template <Uint32 INDEX_>
typename ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_,Derived_>::template BasisVector_f<INDEX_>::T const ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_,Derived_>::BasisVector_f<INDEX_>::V;

template <typename SummandTypeList_, typename Scalar_, typename UseArrayType_, typename Derived_>
struct DualOf_f<ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_,Derived_> >
{
    typedef ImplementationOf_t<typename DualOf_f<DirectSumOfBasedVectorSpaces_c<SummandTypeList_> >::T,Scalar_,typename DualOf_f<UseArrayType_>::T, typename DualOf_f<Derived_>::T> T;
};

// ///////////////////////////////////////////////////////////////////////////
// direct sum of immutable 2-tensors (essentially gives a block-diag matrix)
// ///////////////////////////////////////////////////////////////////////////

template <typename Immutable2TensorImplementationTypeList_>
struct ConceptualTypeOfDirectSumOfImmutable2Tensors_f
{
private:
    typedef typename ConceptOfEachTypeIn_f<Immutable2TensorImplementationTypeList_>::T ConceptTypeList;
    enum
    {
        STATIC_ASSERT_IN_ENUM((EachTypeSatisfies_f<ConceptTypeList,IsTensorProductOfBasedVectorSpaces_p>::V
                               ||
                               EachTypeSatisfies_f<ConceptTypeList,IsDiagonal2TensorProductOfBasedVectorSpaces_p>::V), MUST_BE_TYPELIST_OF_DIAGONAL_OR_2_TENSORS)
    };
    typedef typename FactorNOfEachTypeIn_f<0,ConceptTypeList>::T SummandTypeList0;
    typedef typename FactorNOfEachTypeIn_f<1,ConceptTypeList>::T SummandTypeList1;
    typedef DirectSumOfBasedVectorSpaces_c<SummandTypeList0> Factor0DirectSum;
    typedef DirectSumOfBasedVectorSpaces_c<SummandTypeList1> Factor1DirectSum;
    ConceptualTypeOfDirectSumOfImmutable2Tensors_f();
public:
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor0DirectSum,TypeList_t<Factor1DirectSum> > > T;
};

namespace ComponentGeneratorEvaluator {

template <typename Immutable2TensorImplementationTypeList_,
          typename ConceptualTypeOfDirectSum_,
          typename Scalar_>
struct DirectSumOf2TensorsHelper_t
{
    typedef ComponentIndex_t<DimensionOf_f<ConceptualTypeOfDirectSum_>::V> ComponentIndex;
    typedef typename ConceptualTypeOfDirectSumOfImmutable2Tensors_f<Immutable2TensorImplementationTypeList_>::T ConceptualTypeOfDirectSum;
    enum { STATIC_ASSERT_IN_ENUM((TypesAreEqual_f<ConceptualTypeOfDirectSum_,ConceptualTypeOfDirectSum>::V), TYPES_MUST_BE_EQUAL) };
    typedef typename FactorTypeListOf_f<ConceptualTypeOfDirectSum_>::T FactorTypeList;
    typedef typename FactorTypeList::HeadType Factor0;
    typedef typename FactorTypeList::BodyTypeList::HeadType Factor1;
    typedef typename ImplementationOf_t<ConceptualTypeOfDirectSum_,Scalar_,UseMemberArray>::MultiIndex MultiIndex;
    enum { STATIC_ASSERT_IN_ENUM((MultiIndex::LENGTH == 2), LENGTH_MUST_BE_EXACTLY_2) };
    typedef typename Head_f<Immutable2TensorImplementationTypeList_>::T HeadImplementation;
    typedef typename FactorTypeListOf_f<typename HeadImplementation::Concept>::T HeadFactorTypeList;
    typedef typename Element_f<HeadFactorTypeList,0>::T HeadFactor0;
    typedef typename Element_f<HeadFactorTypeList,1>::T HeadFactor1;

    static Scalar_ evaluate (ComponentIndex_t<DimensionOf_f<ConceptualTypeOfDirectSum_>::V> const &i)
    {
        // this breaks up the component index into the corresponding multiindex.
        MultiIndex m(i);
        bool first_block_for_row = m.template el<0>().value() < DimensionOf_f<HeadFactor0>::V;
        bool first_block_for_col = m.template el<1>().value() < DimensionOf_f<HeadFactor1>::V;

        if (first_block_for_row != first_block_for_col) // off block-diagonal
        {
            return Scalar_(0);
        }
        else if (first_block_for_row && first_block_for_col) // on block-diagonal, upper-left block
        {
            AbstractIndex_c<'j'> j;
            AbstractIndex_c<'k'> k;
            AbstractIndex_c<'p'> p;
            HeadImplementation h;
            typedef typename HeadImplementation::MultiIndex M;
            M head_m(m.template el<0>().value(), m.template el<1>().value(), DONT_CHECK_RANGE);
            // this split is unnecessary if HeadImmutable2TensorImplementation_ is a tensor product,
            // but this makes the same code work for diagonal 2 tensors as well, so for now that's fine.
            return h(p).split(p,j|k)[head_m];
        }
        else // body block
        {
            typedef typename Body_f<Immutable2TensorImplementationTypeList_>::T Immutable2TensorImplementationBodyTypeList;
            typedef typename ConceptualTypeOfDirectSumOfImmutable2Tensors_f<Immutable2TensorImplementationBodyTypeList>::T ConceptualTypeOfDirectSumBody;
            typedef DirectSumOf2TensorsHelper_t<Immutable2TensorImplementationBodyTypeList,
                                                ConceptualTypeOfDirectSumBody,
                                                Scalar_> DirectSumOf2TensorsHelper;
            typedef typename DirectSumOf2TensorsHelper::MultiIndex BodyMultiIndex;
            BodyMultiIndex body_m(m.template el<0>().value() - DimensionOf_f<HeadFactor0>::V,
                                  m.template el<1>().value() - DimensionOf_f<HeadFactor1>::V,
                                  DONT_CHECK_RANGE);
            return DirectSumOf2TensorsHelper::evaluate(body_m.as_component_index());
        }
    }
};

template <typename HeadImmutable2TensorImplementation_,
          typename ConceptualTypeOfDirectSum_,
          typename Scalar_>
struct DirectSumOf2TensorsHelper_t<TypeList_t<HeadImmutable2TensorImplementation_>,ConceptualTypeOfDirectSum_,Scalar_>
{
    typedef typename HeadImmutable2TensorImplementation_::MultiIndex MultiIndex;

    static Scalar_ evaluate (ComponentIndex_t<DimensionOf_f<ConceptualTypeOfDirectSum_>::V> const &i)
    {
        // this breaks up the component index into the corresponding multiindex.
        typename HeadImmutable2TensorImplementation_::MultiIndex m(i);
        HeadImmutable2TensorImplementation_ h;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'p'> p;
        // this split is unnecessary if HeadImmutable2TensorImplementation_ is a tensor product,
        // but this makes the same code work for diagonal 2 tensors as well, so for now that's fine.
        return h(p).split(p,j|k)[m];
    }
};

template <typename Immutable2TensorImplementationTypeList_,
          typename ConceptualTypeOfDirectSum_,
          typename Scalar_>
Scalar_ direct_sum_of_2tensors (ComponentIndex_t<DimensionOf_f<ConceptualTypeOfDirectSum_>::V> const &i)
{
    return DirectSumOf2TensorsHelper_t<Immutable2TensorImplementationTypeList_,
                                       ConceptualTypeOfDirectSum_,
                                       Scalar_>::evaluate(i);
}

} // end of namespace ComponentGeneratorEvaluator

template <typename Immutable2TensorImplementationTypeList_>
struct DirectSumOfImmutable2Tensors_f
{
private:
    typedef typename ConceptOfEachTypeIn_f<Immutable2TensorImplementationTypeList_>::T ConceptTypeList;
    typedef typename ScalarOfEachTypeIn_f<Immutable2TensorImplementationTypeList_>::T ScalarTypeList;
    typedef typename ConceptualTypeOfDirectSumOfImmutable2Tensors_f<Immutable2TensorImplementationTypeList_>::T ConceptualTypeOfDirectSum;

    enum
    {
        STATIC_ASSERT_IN_ENUM(TypeListIsUniform_t<ScalarTypeList>::V, ALL_FACTOR_TYPE_SCALARS_ARE_EQUAL),
        STATIC_ASSERT_IN_ENUM((EachTypeUsesImmutableArray_f<Immutable2TensorImplementationTypeList_>::V), MUST_BE_TYPELIST_OF_IMMUTABLE_IMPLEMENTATIONS)
    };

    typedef typename ScalarTypeList::HeadType Scalar;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<ConceptualTypeOfDirectSum>::V,
                                 ComponentGeneratorEvaluator::direct_sum_of_2tensors<Immutable2TensorImplementationTypeList_,
                                                                                     ConceptualTypeOfDirectSum,
                                                                                     Scalar>,
                                 DirectSum_c<Immutable2TensorImplementationTypeList_> > ComponentGenerator;
private:
    DirectSumOfImmutable2Tensors_f();
public:
    typedef ImplementationOf_t<ConceptualTypeOfDirectSum,Scalar,UseImmutableArray_t<ComponentGenerator> > T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_DIRECT_SUM_HPP_
