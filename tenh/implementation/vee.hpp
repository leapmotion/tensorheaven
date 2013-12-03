// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/vee.hpp by Ted Nitz, created 2013/08/19
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_VEE_HPP_
#define TENH_IMPLEMENTATION_VEE_HPP_

#include "tenh/core.hpp"

#include <functional>

#include "tenh/mathutil.hpp"
#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/interface/embeddableastensor.hpp"
#include "tenh/meta/tuple.hpp"

namespace Tenh {

// Factor_ should be a BasedVectorSpace_c type
template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
struct ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>
    :
    public EmbeddableAsTensor_i<ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>,
                                Scalar_,
                                SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,
                                ComponentsAreImmutable_f<UseArrayType_>::V>,
    // privately inherited because it is an implementation detail
    private ArrayStorage_f<Scalar_,
                           DimensionOf_f<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> >::V,
                           UseArrayType_,
                           ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_> >::T
{
    enum { STATIC_ASSERT_IN_ENUM(HasBasedVectorSpaceStructure_f<Factor_>::V, MUST_BE_BASED_VECTOR_SPACE) };

    typedef EmbeddableAsTensor_i<ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>,
                                 Scalar_,
                                 SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,
                                 ComponentsAreImmutable_f<UseArrayType_>::V> Parent_EmbeddableAsTensor_i;
    typedef typename ArrayStorage_f<Scalar_,
                                    DimensionOf_f<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> >::V,
                                    UseArrayType_,
                                    ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_> >::T Parent_Array_i;

    typedef SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> Concept;
    typedef UseArrayType_ UseArrayType;
    typedef typename Parent_EmbeddableAsTensor_i::Derived Derived;
    typedef typename Parent_EmbeddableAsTensor_i::Scalar Scalar;
    typedef typename Parent_EmbeddableAsTensor_i::BasedVectorSpace BasedVectorSpace;
    using Parent_EmbeddableAsTensor_i::DIM;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentIndex ComponentIndex;

    typedef typename Parent_EmbeddableAsTensor_i::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename Parent_EmbeddableAsTensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_EmbeddableAsTensor_i::MultiIndex MultiIndex;
    static Uint32 const ORDER = ORDER_;
    typedef Factor_ Factor;
    typedef SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor> SymmetricPowerOfBasedVectorSpace;

    using Parent_Array_i::COMPONENTS_ARE_IMMUTABLE;
    typedef typename Parent_Array_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_Array_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;

    typedef typename DualOf_f<ImplementationOf_t>::T Dual; // relies on the template specialization below

    typedef ImplementationOf_t<Concept,
                               Scalar_,
                               UseImmutableArray_t<typename ComponentGenerator_Constant_f<Scalar_,DIM,0>::T> > Zero;
    static Zero const ZERO;

    template <Uint32 INDEX_>
    struct BasisVector_f
    {
    private:
        enum { STATIC_ASSERT_IN_ENUM((INDEX_ < DIM), INDEX_OUT_OF_RANGE) };
        BasisVector_f () { }
    public:
        typedef ImplementationOf_t<Concept,
                                   Scalar_,
                                   UseImmutableArray_t<typename ComponentGenerator_Characteristic_f<Scalar_,DIM,INDEX_>::T> > T;
        static T const V;
    };

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_i(w) { }

    // only use these if UseMemberArray is specified

    // probably only useful for zero element (because this is basis-dependent)
    explicit ImplementationOf_t (Scalar const &fill_with)
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
    ImplementationOf_t (Scalar const &fill_with,
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

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        //STATIC_ASSERT(IsDimIndex_f<BundledIndex>::V, MUST_BE_COMPONENT_INDEX);
        // this constructor breaks the vector index apart into a row-major multi-index
        return BundleIndexComputer_t<BundleIndexTypeList, BundledIndex, ORDER_>::compute(b);
    }

    // only use this if UseImmutableArray_t<...> is specified
    ImplementationOf_t ()
        :
        Parent_Array_i(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseImmutableArray_f<UseArrayType_>::V, MUST_BE_USE_IMMUTABLE_ARRAY);
    }

    using Parent_Array_i::as_derived;
    using Parent_Array_i::operator[];
    using Parent_Array_i::allocation_size_in_bytes;
    using Parent_Array_i::pointer_to_allocation;

    // TODO (maybe): Scalar &operator [] (MultiIndex const &) -- writeable access to components,
    // which would need to throw if the multi-index corresponds to an immutable zero.  there is
    // the complication that components with non-unit scale factors would need to be set with
    // a division by that scale factor.

    // these are what provide indexed expressions -- via expression templates
    using Parent_EmbeddableAsTensor_i::operator();

    static bool component_is_immutable_zero (MultiIndex const &m) { return false; }
    static Scalar scalar_factor_for_component (MultiIndex const &m) { return Scalar(1); }
    static ComponentIndex vector_index_of (MultiIndex const &m)
    {
        MultiIndex n = sorted<typename MultiIndex::IndexTypeList,std::greater<Uint32> >(m);
        return ComponentIndex(VectorIndexComputer_t<MultiIndex>::compute(n), CHECK_RANGE);
    }

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + type_string_of<SymmetricPowerOfBasedVectorSpace>() + ','
                                     + type_string_of<Scalar>() + ','
                                     + type_string_of<UseArrayType_>() + ',';
    }

private:

    template <typename BundleIndexTypeList, typename BundledIndex, Uint32 ORD>
    struct BundleIndexComputer_t;

    template<typename T, typename I = int>
    struct VectorIndexComputer_t;
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
typename ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::Zero const ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::ZERO;

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <Uint32 INDEX_>
typename ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::template BasisVector_f<INDEX_>::T const ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::BasisVector_f<INDEX_>::V;

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <typename BundleIndexTypeList, typename BundledIndex, Uint32 ORD>
struct ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::BundleIndexComputer_t
{
    static MultiIndex_t<BundleIndexTypeList> compute (BundledIndex const &b)
    {
        return MultiIndex_t<BundleIndexTypeList>(typename BundleIndexTypeList::HeadType(index_of_greatest_triangular_number_less_than(b.value(),ORD) - ORD + 1, CHECK_RANGE), BundleIndexComputer_t<typename BundleIndexTypeList::BodyTypeList, BundledIndex, ORD-1>::compute(BundledIndex(b.value() - binomial_coefficient(index_of_greatest_triangular_number_less_than(b.value(),ORD), ORD), CHECK_RANGE)));
    }
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <typename FactorType, typename BundledIndex, Uint32 ORD>
struct ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::BundleIndexComputer_t<TypeList_t<FactorType>, BundledIndex, ORD>
{
    static MultiIndex_t<TypeList_t<FactorType> > compute (BundledIndex const &b)
    {
        return MultiIndex_t<TypeList_t<FactorType> >(FactorType(index_of_greatest_triangular_number_less_than(b.value(),ORD) - ORD + 1, CHECK_RANGE));
    }
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <typename T, typename I>
struct ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::VectorIndexComputer_t
{
    static Uint32 compute (T const &m)
    {
        return VectorIndexComputer_t<typename T::BodyMultiIndex>::compute(m.body()) + binomial_coefficient(m.head().value() + T::LENGTH - 1,T::LENGTH);
    }
};

template <Uint32 ORDER_, typename Factor_, typename Scalar_, typename UseArrayType_>
template <typename I>
struct ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_>,Scalar_,UseArrayType_>::VectorIndexComputer_t<MultiIndex_t<EmptyTypeList>, I>
{
    static Uint32 compute (MultiIndex_t<EmptyTypeList> const &)
    {
        return 0;
    }
};

template <Uint32 ORDER, typename Factor, typename Scalar, typename UseArrayType_>
struct DualOf_f<ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER,Factor>,Scalar,UseArrayType_> >
{
    typedef ImplementationOf_t<typename DualOf_f<SymmetricPowerOfBasedVectorSpace_c<ORDER,Factor> >::T,Scalar,typename DualOf_f<UseArrayType_>::T> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_VEE_HPP_
