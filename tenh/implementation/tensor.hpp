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

namespace Tenh {

template <typename FactorTypeList_, typename Scalar_, typename UseArrayType_>
struct ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_>
    :
    public Tensor_i<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_>,
                    Scalar_,
                    TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >,
    // privately inherited because it is an implementation detail
    private ArrayStorage_f<Scalar_,
                           AS_VECTOR_SPACE(TensorProductOfBasedVectorSpaces_c<FactorTypeList_>)::DIMENSION,
                           UseArrayType_,
                           ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_> >::T
{
    typedef Tensor_i<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_>,
                     Scalar_,
                     TensorProductOfBasedVectorSpaces_c<FactorTypeList_> > Parent_Tensor_i;
    typedef typename ArrayStorage_f<Scalar_,
                                    AS_VECTOR_SPACE(TensorProductOfBasedVectorSpaces_c<FactorTypeList_>)::DIMENSION,
                                    UseArrayType_,
                                    ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_> >::T Parent_Array_i;

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

    typedef typename DualOf_f<ImplementationOf_t>::T Dual; // relies on the template specialization below

    // TODO: default constructor which is only allowed for UseImmutableArray

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_i(w) { }

    // only use these if UseMemoryArray is specified

    // probably only useful for zero element (because this is basis-dependent)
    explicit ImplementationOf_t (Scalar const &fill_with)
        :
        Parent_Array_i(fill_with)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemoryArray);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1)
        :
        Parent_Array_i(x0, x1)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemoryArray);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1, Scalar const &x2)
        :
        Parent_Array_i(x0, x1, x2)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemoryArray);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1, Scalar const &x2, Scalar const &x3)
        :
        Parent_Array_i(x0, x1, x2, x3)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemoryArray);
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
    ImplementationOf_t (Scalar const &x0, Scalar const &x1,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(x0, x1, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1, Scalar const &x2,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(x0, x1, x2, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1, Scalar const &x2, Scalar const &x3,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(x0, x1, x2, x3, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        STATIC_ASSERT(IsDimIndex_f<BundledIndex>::V, MUST_BE_COMPONENT_INDEX);
        // this constructor breaks the vector index apart into a row-major multi-index
        return MultiIndex_t<BundleIndexTypeList>(b);
    }

    using Parent_Array_i::as_derived;
    using Parent_Array_i::allocation_size_in_bytes;
    using Parent_Array_i::pointer_to_allocation;

    // can't do "using Parent_Array_i::operator[];" due to overloading ambiguity.
    Scalar const &operator [] (ComponentIndex const &i) const { return Parent_Array_i::operator[](i); }
    Scalar &operator [] (ComponentIndex const &i) { return Parent_Array_i::operator[](i); }

    using Parent_Tensor_i::operator[];
    // these are what provide indexed expressions -- via expression templates
    using Parent_Tensor_i::operator();

    using Parent_Tensor_i::component_is_immutable_zero;
    using Parent_Tensor_i::scalar_factor_for_component;
    using Parent_Tensor_i::vector_index_of;

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + TypeStringOf_t<TensorProductOfBasedVectorSpaces>::eval() + ',' 
                                     + TypeStringOf_t<Scalar>::eval() + ','
                                     + TypeStringOf_t<UseArrayType_>::eval() + '>';
    }

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    ImplementationOf_t ();
};

template <typename FactorTypeList_, typename Scalar_, typename UseArrayType_>
struct DualOf_f<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,UseArrayType_> >
{
    typedef ImplementationOf_t<typename DualOf_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >::T,Scalar_,typename DualOf_f<UseArrayType_>::T> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_TENSOR_HPP_
