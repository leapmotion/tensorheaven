// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/vector.hpp by Victor Dods, created 2013/08/10
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_VECTOR_HPP_
#define TENH_IMPLEMENTATION_VECTOR_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/interface/vector.hpp"
#include "tenh/meta/tuple.hpp"

namespace Tenh {

template <typename VectorSpace_, typename Basis_, typename Scalar_, typename UseArrayType_, typename Derived_>
struct ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,UseArrayType_,Derived_>
    :
    public Vector_i<typename DerivedType_f<Derived_,ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,UseArrayType_,Derived_> >::T,
                    Scalar_,
                    BasedVectorSpace_c<VectorSpace_,Basis_>,
                    ComponentsAreImmutable_f<UseArrayType_>::V>,
    // privately inherited because it is an implementation detail
    private ArrayStorage_f<Scalar_,
                           DimensionOf_f<VectorSpace_>::V,
                           UseArrayType_,
                           typename DerivedType_f<Derived_,ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,UseArrayType_,Derived_> >::T >::T
{
    typedef Vector_i<typename DerivedType_f<Derived_,ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,UseArrayType_,Derived_> >::T,
                     Scalar_,
                     BasedVectorSpace_c<VectorSpace_,Basis_>,
                     ComponentsAreImmutable_f<UseArrayType_>::V> Parent_Vector_i;
    typedef typename ArrayStorage_f<Scalar_,
                                    DimensionOf_f<VectorSpace_>::V,
                                    UseArrayType_,
                                    typename DerivedType_f<Derived_,ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,UseArrayType_,Derived_> >::T >::T Parent_Array_i;

    typedef BasedVectorSpace_c<VectorSpace_,Basis_> Concept;
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
        typedef typename ComponentGenerator_Characteristic_f<Scalar_,Parent_Vector_i::DIM,INDEX_>::T ComponentGenerator;
    public:
        typedef ImplementationOf_t<Concept,
                                   Scalar_,
                                   UseImmutableArray_t<ComponentGenerator>,
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

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + type_string_of<BasedVectorSpace>() + ','
                                     + type_string_of<Scalar>() + ','
                                     + type_string_of<UseArrayType_>() + '>';
    }
};

template <typename VectorSpace_, typename Basis_, typename Scalar_, typename UseArrayType_, typename Derived_>
typename ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,UseArrayType_,Derived_>::Zero const ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,UseArrayType_,Derived_>::ZERO;

template <typename VectorSpace_, typename Basis_, typename Scalar_, typename UseArrayType_, typename Derived_>
template <Uint32 INDEX_>
typename ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,UseArrayType_,Derived_>::template BasisVector_f<INDEX_>::T const ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,UseArrayType_,Derived_>::BasisVector_f<INDEX_>::V;

template <typename VectorSpace_, typename Basis_, typename Scalar_, typename UseArrayType_, typename Derived_>
struct DualOf_f<ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,UseArrayType_,Derived_> >
{
    typedef ImplementationOf_t<typename DualOf_f<BasedVectorSpace_c<VectorSpace_,Basis_> >::T,Scalar_,typename DualOf_f<UseArrayType_>::T, typename DualOf_f<Derived_>::T> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_VECTOR_HPP_
