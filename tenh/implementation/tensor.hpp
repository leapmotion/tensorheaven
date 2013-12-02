// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/tensor.hpp by Victor Dods, created 2013/08/10
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_TENSOR_HPP_
#define TENH_IMPLEMENTATION_TENSOR_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/interface/tensor.hpp"
#include "tenh/meta/tuple.hpp"

namespace Tenh {

template <typename FactorTypeList_, typename Scalar_, typename UseArrayType_>
struct ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_>
    :
    public Tensor_i<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_>,
                    Scalar_,
                    TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,
                    ComponentsAreImmutable_f<UseArrayType_>::V>,
    // privately inherited because it is an implementation detail
    private ArrayStorage_f<Scalar_,
                           DimensionOf_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >::V,
                           UseArrayType_,
                           ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_> >::T
{
    typedef Tensor_i<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_>,
                     Scalar_,
                     TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,
                     ComponentsAreImmutable_f<UseArrayType_>::V> Parent_Tensor_i;
    typedef typename ArrayStorage_f<Scalar_,
                                    DimensionOf_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >::V,
                                    UseArrayType_,
                                    ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_> >::T Parent_Array_i;

    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList_> Concept;
    typedef UseArrayType_ UseArrayType;
    typedef typename Parent_Tensor_i::Derived Derived;
    typedef typename Parent_Tensor_i::Scalar Scalar;
    typedef typename Parent_Tensor_i::BasedVectorSpace BasedVectorSpace;
    using Parent_Tensor_i::DIM;
    typedef typename Parent_Tensor_i::ComponentIndex ComponentIndex;

    typedef typename Parent_Tensor_i::TensorProductOfBasedVectorSpaces TensorProductOfBasedVectorSpaces;
    typedef typename Parent_Tensor_i::FactorTypeList FactorTypeList;
    typedef typename Parent_Tensor_i::MultiIndex MultiIndex;
    using Parent_Tensor_i::ORDER;
    using Parent_Tensor_i::IS_TENSOR_I;

    using Parent_Array_i::COMPONENTS_ARE_IMMUTABLE;
    typedef typename Parent_Array_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_Array_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;

    typedef typename DualOf_f<ImplementationOf_t>::T Dual; // relies on the template specialization below

    typedef ImplementationOf_t<Concept,
                               Scalar_,
                               UseImmutableArray_t<typename ComponentGenerator_ConstantZero_f<Scalar_,DIM>::T> > Zero;
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

    // only use this if UseImmutableArray_t<...> is specified
    ImplementationOf_t ()
        :
        Parent_Array_i(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseImmutableArray_f<UseArrayType_>::V, MUST_BE_USE_IMMUTABLE_ARRAY);
    }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        STATIC_ASSERT(IsComponentIndex_f<BundledIndex>::V, MUST_BE_COMPONENT_INDEX);
        // this constructor breaks the vector index apart into a row-major multi-index
        return MultiIndex_t<BundleIndexTypeList>(b);
    }

    using Parent_Array_i::as_derived;
    using Parent_Array_i::allocation_size_in_bytes;
    using Parent_Array_i::pointer_to_allocation;

    // can't do "using Parent_Array_i::operator[];" due to overloading ambiguity.
    ComponentAccessConstReturnType operator [] (ComponentIndex const &i) const { return Parent_Array_i::operator[](i); }
    ComponentAccessNonConstReturnType operator [] (ComponentIndex const &i) { return Parent_Array_i::operator[](i); }

    using Parent_Tensor_i::operator[];
    // these are what provide indexed expressions -- via expression templates
    using Parent_Tensor_i::operator();

    using Parent_Tensor_i::component_is_immutable_zero;
    using Parent_Tensor_i::scalar_factor_for_component;
    using Parent_Tensor_i::vector_index_of;

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + type_string_of<TensorProductOfBasedVectorSpaces>() + ','
                                     + type_string_of<Scalar>() + ','
                                     + type_string_of<UseArrayType_>() + '>';
    }
};

template <typename FactorTypeList_, typename Scalar_, typename UseArrayType_>
typename ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_>::Zero const ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_>::ZERO;

template <typename FactorTypeList_, typename Scalar_, typename UseArrayType_>
template <Uint32 INDEX_>
typename ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_>::template BasisVector_f<INDEX_>::T const ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_>::BasisVector_f<INDEX_>::V;

template <typename FactorTypeList_, typename Scalar_, typename UseArrayType_>
struct DualOf_f<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_> >
{
    typedef ImplementationOf_t<typename DualOf_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >::T,Scalar_,typename DualOf_f<UseArrayType_>::T> T;
private:
    DualOf_f();
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_TENSOR_HPP_
