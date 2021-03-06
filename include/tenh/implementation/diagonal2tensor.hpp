// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/diagonal2tensor.hpp by Victor Dods, created 2013/08/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_DIAGONAL2TENSOR_HPP_
#define TENH_IMPLEMENTATION_DIAGONAL2TENSOR_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/dimindex.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/interface/embeddableastensor.hpp"
#include "tenh/tuple.hpp"

namespace Tenh {

// Factor_ should be a BasedVectorSpace_c type
template <typename Factor0_, typename Factor1_, typename Scalar_, typename UseArrayType_, typename Derived_>
struct ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>
    :
    public EmbeddableAsTensor_i<typename DerivedType_f<Derived_, ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>>::T,
                                Scalar_,
                                Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,
                                ComponentQualifierOfArrayType_f<UseArrayType_>::V>,
    // privately inherited because it is an implementation detail
    private ArrayStorage_f<Scalar_,
                           DimensionOf_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>>::V,
                           UseArrayType_,
                           typename DerivedType_f<Derived_, ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>>::T >::T
{
    static_assert(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor0_), "Factor0_ must have unique based vector space structure");
    static_assert(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor1_), "Factor1_ must have unique based vector space structure");

    typedef EmbeddableAsTensor_i<typename DerivedType_f<Derived_, ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>>::T,
                                 Scalar_,
                                 Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,
                                 ComponentQualifierOfArrayType_f<UseArrayType_>::V> Parent_EmbeddableAsTensor_i;
    typedef typename ArrayStorage_f<Scalar_,
                                    DimensionOf_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>>::V,
                                    UseArrayType_,
                                    typename DerivedType_f<Derived_, ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>>::T >::T Parent_Array_i;

    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_> Concept;
    typedef UseArrayType_ UseArrayType;
    typedef typename Parent_EmbeddableAsTensor_i::Derived Derived;
    typedef typename Parent_EmbeddableAsTensor_i::Scalar Scalar;
    using Parent_EmbeddableAsTensor_i::DIM;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentIndex ComponentIndex;
    using Parent_EmbeddableAsTensor_i::COMPONENT_QUALIFIER;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;
    typedef typename Parent_EmbeddableAsTensor_i::QualifiedComponent QualifiedComponent;

    typedef typename Parent_EmbeddableAsTensor_i::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename Parent_EmbeddableAsTensor_i::FactorTyple FactorTyple;
    typedef typename Parent_EmbeddableAsTensor_i::MultiIndex MultiIndex;
    static Uint32 const ORDER = 2;
    typedef Factor0_ Factor0;
    typedef Factor1_ Factor1;

    typedef typename DualOf_f<ImplementationOf_t>::T Dual; // relies on the template specialization below

    typedef ImplementationOf_t<Concept,
                               Scalar_,
                               UseProceduralArray_t<typename ComponentGenerator_Constant_f<Scalar_,DIM,0>::T>,
                               Derived_> Zero;
    static Zero const ZERO;

    template <Uint32 INDEX_>
    struct BasisVector_f
    {
        static_assert(INDEX_ < Parent_EmbeddableAsTensor_i::DIM, "index must be in range [0,DIM)");
        typedef ImplementationOf_t<Concept,
                                   Scalar_,
                                   UseProceduralArray_t<typename ComponentGenerator_Characteristic_f<Scalar_,Parent_EmbeddableAsTensor_i::DIM,INDEX_>::T>,
                                   Derived_> T;
        static T const V;
    private:
        BasisVector_f () { }
    };

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_i(w) { }

    // only use these if UseMemberArray_t<...> is specified

    // similar to a copy constructor, except initializes from a Vector_i.
    // this was chosen to be explicit to avoid unnecessary copies.
    template <typename OtherDerived_, ComponentQualifier OTHER_COMPONENT_QUALIFIER_>
    explicit ImplementationOf_t (Vector_i<OtherDerived_,Scalar_,Concept,OTHER_COMPONENT_QUALIFIER_> const &x)
        :
        Parent_Array_i(Static<WithoutInitialization>::SINGLETON)
    {
        static_assert(IsUseMemberArray_f<UseArrayType_>::V, "UseArrayType_ must be a UseMemberArray_t type");
        // TODO: could make this use MemoryArray_i::copy_from (?)
        for (ComponentIndex i; i.is_not_at_end(); ++i)
            (*this)[i] = x[i];
    }
    // probably only useful for zero element (because this is basis-dependent), though
    // this would also give any scalar matrix, including the identity matrix (though you
    // would really want to TODO: implement Scalar2TensorProductOfBasedVectorSpaces_c)
    template <typename T_>
    explicit ImplementationOf_t (FillWith_t<T_> const &fill_with)
        :
        Parent_Array_i(fill_with)
    {
        static_assert(IsUseMemberArray_f<UseArrayType_>::V, "UseArrayType_ must be a UseMemberArray_t type");
    }
    // this is the tuple-based constructor
    template <typename... Types_>
    ImplementationOf_t (Tuple_t<Typle_t<Types_...>> const &x)
        :
        Parent_Array_i(x.as_member_array())
    {
        static_assert(IsUseMemberArray_f<UseArrayType_>::V, "UseArrayType_ must be a UseMemberArray_t type");
    }

    // only use these if UsePreallocatedArray_t<...> is specified

    explicit ImplementationOf_t (QualifiedComponent *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        Parent_Array_i(pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "UseArrayType_ must be a UsePreallocatedArray_t type");
    }
    // similar to a copy constructor, except initializes from a Vector_i.
    // this was chosen to be explicit to avoid unnecessary copies.
    template <typename OtherDerived_, ComponentQualifier OTHER_COMPONENT_QUALIFIER_>
    ImplementationOf_t (Vector_i<OtherDerived_,Scalar_,Concept,OTHER_COMPONENT_QUALIFIER_> const &x,
                        QualifiedComponent *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        Parent_Array_i(pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "UseArrayType_ must be a UsePreallocatedArray_t type");
        // TODO: could make this use MemoryArray_i::copy_from (?)
        for (ComponentIndex i; i.is_not_at_end(); ++i)
            (*this)[i] = x[i];
    }
    template <typename T_>
    ImplementationOf_t (FillWith_t<T_> const &fill_with,
                        QualifiedComponent *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        Parent_Array_i(fill_with, pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "UseArrayType_ must be a UsePreallocatedArray_t type");
    }
    // this is the tuple-based constructor
    template <typename... Types_>
    ImplementationOf_t (Tuple_t<Typle_t<Types_...>> const &x,
                        QualifiedComponent *pointer_to_allocation, CheckPointer check_pointer = CheckPointer::TRUE)
        :
        Parent_Array_i(x, pointer_to_allocation, check_pointer)
    {
        static_assert(IsUsePreallocatedArray_f<UseArrayType_>::V, "UseArrayType_ must be a UsePreallocatedArray_t type");
    }

    // only use this if UseProceduralArray_t<...> is specified
    ImplementationOf_t ()
        :
        Parent_Array_i(WithoutInitialization()) // sort of meaningless constructor
    {
        static_assert(IsUseProceduralArray_f<UseArrayType_>::V || DIM == 0, "UseArrayType_ must be UseProceduralArray_t or space must be 0-dimensional");
    }

    template <typename BundleIndexTyple, typename BundledIndex>
    static MultiIndex_t<BundleIndexTyple> bundle_index_map (BundledIndex const &b)
    {
        static_assert(IsDimIndex_f<BundledIndex>::V, "BundledIndex must be a DimIndex_t");
        static_assert(Length_f<BundleIndexTyple>::V == 2, "must bundle exactly 2 indices");
        typedef MultiIndex_t<BundleIndexTyple> MultiIndex;
        typedef typename Element_f<BundleIndexTyple,0>::T Index0;
        typedef typename Element_f<BundleIndexTyple,1>::T Index1;
        static_assert(IsDimIndex_f<Index0>::V, "Index0 must be a DimIndex_t");
        static_assert(IsDimIndex_f<Index1>::V, "Index1 must be a DimIndex_t");
        static_assert(Index0::COMPONENT_COUNT == DimensionOf_f<Factor0_>::V, "dimensions must match");
        static_assert(Index1::COMPONENT_COUNT == DimensionOf_f<Factor1_>::V, "dimensions must match");
        Uint32 b_value = b.value();
        return MultiIndex(b_value, b_value, CheckRange::FALSE);
    }

    using Parent_Array_i::as_derived;
    using Parent_Array_i::operator[];
    using Parent_Array_i::allocation_size_in_bytes;
    using Parent_Array_i::pointer_to_allocation;
    using Parent_Array_i::overlaps_memory_range;

    // TODO (maybe): Scalar &operator [] (MultiIndex const &) -- writeable access to components,
    // which would need to throw if the multi-index corresponds to an procedural zero.  there is
    // the complication that components with non-unit scale factors would need to be set with
    // a division by that scale factor.

    // these are what provide indexed expressions -- via expression templates
    using Parent_EmbeddableAsTensor_i::operator();

    static bool component_is_procedural_zero (MultiIndex const &m) { return m.template el<0>().value() != m.template el<1>().value(); }
    static Scalar scalar_factor_for_component (MultiIndex const &) { return Scalar(1); }
    static ComponentIndex vector_index_of (MultiIndex const &m) { return ComponentIndex(m.template el<0>().value(), CheckRange::FALSE); }
};

template <typename Factor0_, typename Factor1_, typename Scalar_, typename UseArrayType_, typename Derived_>
typename ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>::Zero const ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>::ZERO;

template <typename Factor0_, typename Factor1_, typename Scalar_, typename UseArrayType_, typename Derived_>
template <Uint32 INDEX_>
typename ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>::template BasisVector_f<INDEX_>::T const ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>::BasisVector_f<INDEX_>::V;

template <typename Factor0_, typename Factor1_, typename Scalar_, typename UseArrayType_, typename Derived_>
struct DualOf_f<ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>,Scalar_,UseArrayType_,Derived_>>
{
    typedef ImplementationOf_t<typename DualOf_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor0_,Factor1_>>::T,Scalar_,typename DualOf_f<UseArrayType_>::T,typename DualOf_f<Derived_>::T> T;
private:
    DualOf_f();
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_DIAGONAL2TENSOR_HPP_
