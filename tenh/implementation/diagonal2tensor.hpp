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
#include "tenh/meta/tuple.hpp"

namespace Tenh {

// Factor_ should be a BasedVectorSpace_c type
template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayType_, typename Derived_>
struct ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_>
    :
    public EmbeddableAsTensor_i<typename DerivedType_f<Derived_, ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_> >::T,
                                Scalar_,
                                Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,
                                ComponentsAreProcedural_f<UseArrayType_>::V>,
    // privately inherited because it is an implementation detail
    private ArrayStorage_f<Scalar_,
                           DimensionOf_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> >::V,
                           UseArrayType_,
                           typename DerivedType_f<Derived_, ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_> >::T >::T
{
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor1_), MUST_BE_BASED_VECTOR_SPACE, FACTOR1),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IS_BASED_VECTOR_SPACE_UNIQUELY(Factor2_), MUST_BE_BASED_VECTOR_SPACE, FACTOR2)
    };

    typedef EmbeddableAsTensor_i<typename DerivedType_f<Derived_, ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_> >::T,
                                 Scalar_,
                                 Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,
                                 ComponentsAreProcedural_f<UseArrayType_>::V> Parent_EmbeddableAsTensor_i;
    typedef typename ArrayStorage_f<Scalar_,
                                    DimensionOf_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> >::V,
                                    UseArrayType_,
                                    typename DerivedType_f<Derived_, ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_> >::T >::T Parent_Array_i;

    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> Concept;
    typedef UseArrayType_ UseArrayType;
    typedef typename Parent_EmbeddableAsTensor_i::Derived Derived;
    typedef typename Parent_EmbeddableAsTensor_i::Scalar Scalar;
    using Parent_EmbeddableAsTensor_i::DIM;
    typedef typename Parent_EmbeddableAsTensor_i::ComponentIndex ComponentIndex;

    typedef typename Parent_EmbeddableAsTensor_i::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename Parent_EmbeddableAsTensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_EmbeddableAsTensor_i::MultiIndex MultiIndex;
    static Uint32 const ORDER = 2;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;

    using Parent_Array_i::COMPONENTS_ARE_PROCEDURAL;
    typedef typename Parent_Array_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_Array_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;

    typedef typename DualOf_f<ImplementationOf_t>::T Dual; // relies on the template specialization below

    typedef ImplementationOf_t<Concept,
                               Scalar_,
                               UseProceduralArray_t<typename ComponentGenerator_Constant_f<Scalar_,DIM,0>::T>,
                               Derived_> Zero;
    static Zero const ZERO;

    template <Uint32 INDEX_>
    struct BasisVector_f
    {
    private:
        enum { STATIC_ASSERT_IN_ENUM((INDEX_ < Parent_EmbeddableAsTensor_i::DIM), INDEX_OUT_OF_RANGE) };
        BasisVector_f () { }
    public:
        typedef ImplementationOf_t<Concept,
                                   Scalar_,
                                   UseProceduralArray_t<typename ComponentGenerator_Characteristic_f<Scalar_,Parent_EmbeddableAsTensor_i::DIM,INDEX_>::T>,
                                   Derived_> T;
        static T const V;
    };

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_i(w) { }

    // only use these if UseMemberArray is specified

    // similar to a copy constructor, except initializes from a Vector_i.
    // this was chosen to be explicit to avoid unnecessary copies.
    template <typename OtherDerived_, bool COMPONENTS_ARE_PROCEDURAL_>
    explicit ImplementationOf_t (Vector_i<OtherDerived_,Scalar_,Concept,COMPONENTS_ARE_PROCEDURAL_> const &x)
        :
        Parent_Array_i(Static<WithoutInitialization>::SINGLETON)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
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
    // similar to a copy constructor, except initializes from a Vector_i.
    // this was chosen to be explicit to avoid unnecessary copies.
    template <typename OtherDerived_, bool COMPONENTS_ARE_PROCEDURAL_>
    ImplementationOf_t (Vector_i<OtherDerived_,Scalar_,Concept,COMPONENTS_ARE_PROCEDURAL_> const &x,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
        // TODO: could make this use MemoryArray_i::copy_from (?)
        for (ComponentIndex i; i.is_not_at_end(); ++i)
            (*this)[i] = x[i];
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

    // only use this if UseProceduralArray_t<...> is specified
    ImplementationOf_t ()
        :
        Parent_Array_i(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseProceduralArray_f<UseArrayType_>::V, MUST_BE_USE_PROCEDURAL_ARRAY);
    }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        STATIC_ASSERT(IsDimIndex_f<BundledIndex>::V, MUST_BE_DIM_INDEX);
        STATIC_ASSERT(BundleIndexTypeList::LENGTH == 2, LENGTH_MUST_BE_EXACTLY_2);
        typedef MultiIndex_t<BundleIndexTypeList> MultiIndex;
        typedef typename BundleIndexTypeList::HeadType Index1;
        typedef typename BundleIndexTypeList::BodyTypeList::HeadType Index2;
        STATIC_ASSERT(IsDimIndex_f<Index1>::V, MUST_BE_DIM_INDEX);
        STATIC_ASSERT(IsDimIndex_f<Index2>::V, MUST_BE_DIM_INDEX);
        STATIC_ASSERT(Index1::COMPONENT_COUNT == DimensionOf_f<Factor1_>::V, DIMENSIONS_MUST_MATCH);
        STATIC_ASSERT(Index2::COMPONENT_COUNT == DimensionOf_f<Factor2_>::V, DIMENSIONS_MUST_MATCH);
        Uint32 b_value = b.value();
        return MultiIndex(b_value, b_value, DONT_CHECK_RANGE);
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
    static ComponentIndex vector_index_of (MultiIndex const &m) { return ComponentIndex(m.template el<0>().value(), DONT_CHECK_RANGE); }

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + type_string_of<Concept>() + ','
                                     + type_string_of<Scalar>() + ','
                                     + type_string_of<UseArrayType_>() + '>';
    }
};

template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayType_, typename Derived_>
typename ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_>::Zero const ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_>::ZERO;

template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayType_, typename Derived_>
template <Uint32 INDEX_>
typename ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_>::template BasisVector_f<INDEX_>::T const ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_>::BasisVector_f<INDEX_>::V;

template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayType_, typename Derived_>
struct DualOf_f<ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_>,Scalar_,UseArrayType_,Derived_> >
{
    typedef ImplementationOf_t<typename DualOf_f<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> >::T,Scalar_,typename DualOf_f<UseArrayType_>::T,typename DualOf_f<Derived_>::T> T;
private:
    DualOf_f();
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_DIAGONAL2TENSOR_HPP_
