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

template <typename FactorTypeList_, typename Scalar_, bool USE_MEMBER_ARRAY_>
struct ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,USE_MEMBER_ARRAY_>
    :
    public Tensor_i<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,USE_MEMBER_ARRAY_>,
                    Scalar_,
                    TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >,
    // privately inherited because it is an implementation detail
    private ArrayStorage_f<Scalar_,
                           AS_VECTOR_SPACE(TensorProductOfBasedVectorSpaces_c<FactorTypeList_>)::DIMENSION,
                           USE_MEMBER_ARRAY_,
                           ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,USE_MEMBER_ARRAY_> >::T
{
    typedef Tensor_i<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,USE_MEMBER_ARRAY_>,
                     Scalar_,
                     TensorProductOfBasedVectorSpaces_c<FactorTypeList_> > Parent_Tensor_i;
    typedef typename ArrayStorage_f<Scalar_,
                                    AS_VECTOR_SPACE(TensorProductOfBasedVectorSpaces_c<FactorTypeList_>)::DIMENSION,
                                    USE_MEMBER_ARRAY_,
                                    ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,USE_MEMBER_ARRAY_> >::T Parent_Array_i;

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

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Array_i(w) { }

    // only use these if USE_MEMBER_ARRAY is specified

    // probably only useful for zero element (because this is basis-dependent)
    explicit ImplementationOf_t (Scalar const &fill_with)
        :
        Parent_Array_i(fill_with)
    {
        STATIC_ASSERT(USE_MEMBER_ARRAY_ == USE_MEMBER_ARRAY, MAY_USE_THIS_ONLY_WITH_USE_MEMBER_ARRAY);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1)
        :
        Parent_Array_i(x0, x1)
    {
        STATIC_ASSERT(USE_MEMBER_ARRAY_ == USE_MEMBER_ARRAY, MAY_USE_THIS_ONLY_WITH_USE_MEMBER_ARRAY);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1, Scalar const &x2)
        :
        Parent_Array_i(x0, x1, x2)
    {
        STATIC_ASSERT(USE_MEMBER_ARRAY_ == USE_MEMBER_ARRAY, MAY_USE_THIS_ONLY_WITH_USE_MEMBER_ARRAY);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1, Scalar const &x2, Scalar const &x3)
        :
        Parent_Array_i(x0, x1, x2, x3)
    {
        STATIC_ASSERT(USE_MEMBER_ARRAY_ == USE_MEMBER_ARRAY, MAY_USE_THIS_ONLY_WITH_USE_MEMBER_ARRAY);
    }

    // only use these if USE_PREALLOCATED_ARRAY is specified

    explicit ImplementationOf_t (Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(USE_MEMBER_ARRAY_ == USE_PREALLOCATED_ARRAY, MAY_USE_THIS_ONLY_WITH_USE_PREALLOCATED_ARRAY);
    }
    ImplementationOf_t (Scalar const &fill_with, 
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(fill_with, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(USE_MEMBER_ARRAY_ == USE_PREALLOCATED_ARRAY, MAY_USE_THIS_ONLY_WITH_USE_PREALLOCATED_ARRAY);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(x0, x1, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(USE_MEMBER_ARRAY_ == USE_PREALLOCATED_ARRAY, MAY_USE_THIS_ONLY_WITH_USE_PREALLOCATED_ARRAY);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1, Scalar const &x2,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(x0, x1, x2, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(USE_MEMBER_ARRAY_ == USE_PREALLOCATED_ARRAY, MAY_USE_THIS_ONLY_WITH_USE_PREALLOCATED_ARRAY);
    }
    ImplementationOf_t (Scalar const &x0, Scalar const &x1, Scalar const &x2, Scalar const &x3,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Array_i(x0, x1, x2, x3, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(USE_MEMBER_ARRAY_ == USE_PREALLOCATED_ARRAY, MAY_USE_THIS_ONLY_WITH_USE_PREALLOCATED_ARRAY);
    }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static MultiIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        STATIC_ASSERT(IsDimIndex_f<BundledIndex>::V, MUST_BE_COMPONENT_INDEX);
        // this constructor breaks the vector index apart into a row-major multi-index
        return MultiIndex_t<BundleIndexTypeList>(b);
    }

    using Parent_Array_i::as_derived;
    using Parent_Array_i::operator[];
    using Parent_Array_i::allocation_size_in_bytes;
    using Parent_Array_i::pointer_to_allocation;

    template <typename OtherIndexTypeList>
    Scalar const &operator [] (MultiIndex_t<OtherIndexTypeList> const &m) const
    {
        STATIC_ASSERT(IsTypeList_f<OtherIndexTypeList>::V, MUST_BE_TYPELIST);
        typedef MultiIndex_t<OtherIndexTypeList> OtherMultiIndex;
        STATIC_ASSERT((OtherMultiIndex::LENGTH == MultiIndex::LENGTH), MUST_HAVE_EQUAL_LENGTHS);
        //std::cout << OtherMultiIndex::LENGTH << ", " << MultiIndex::LENGTH << '\n';
        assert(m.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        MultiIndex x(m);
        // m.value() is what does the multi-index-to-vector-index computation
        return operator[](ComponentIndex(m.value(), DONT_CHECK_RANGE));
    }
    template <typename OtherIndexTypeList>
    Scalar &operator [] (MultiIndex_t<OtherIndexTypeList> const &m)
    {
        STATIC_ASSERT(IsTypeList_f<OtherIndexTypeList>::V, MUST_BE_TYPELIST);
        typedef MultiIndex_t<OtherIndexTypeList> OtherMultiIndex;
        STATIC_ASSERT((OtherMultiIndex::LENGTH == MultiIndex::LENGTH), MUST_HAVE_EQUAL_LENGTHS);
        //std::cout << OtherMultiIndex::LENGTH << ", " << MultiIndex::LENGTH << '\n';
        assert(m.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        MultiIndex x(m);
        // m.value() is what does the multi-index-to-vector-index computation
        return operator[](ComponentIndex(m.value(), DONT_CHECK_RANGE));
    }

    // these are what provide indexed expressions -- via expression templates
    using Parent_Tensor_i::operator();

    using Parent_Tensor_i::component_is_immutable_zero;
    using Parent_Tensor_i::scalar_factor_for_component;
    using Parent_Tensor_i::vector_index_of;

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + TypeStringOf_t<TensorProductOfBasedVectorSpaces>::eval() + ',' 
                                     + TypeStringOf_t<Scalar>::eval() + ','
                                     + AS_STRING(USE_MEMBER_ARRAY_) + '>';
    }

private:

    // this has no definition, and is designed to generate a compiler error if used (use the one accepting WithoutInitialization instead).
    ImplementationOf_t ();
};

template <typename FactorTypeList_, typename Scalar_, bool USE_MEMBER_ARRAY_>
struct DualOf_f<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<FactorTypeList_>,Scalar_,USE_MEMBER_ARRAY_> >
{
    typedef ImplementationOf_t<typename DualOf_f<TensorProductOfBasedVectorSpaces_c<FactorTypeList_> >::T,Scalar_,USE_MEMBER_ARRAY_> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_TENSOR_HPP_
