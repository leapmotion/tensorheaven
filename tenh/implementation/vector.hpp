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

namespace Tenh {

template <typename VectorSpace_, typename Basis_, typename Scalar_, bool USE_MEMBER_ARRAY_>
struct ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,USE_MEMBER_ARRAY_>
    :
    public Vector_i<ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,USE_MEMBER_ARRAY_>,
                    Scalar_,
                    BasedVectorSpace_c<VectorSpace_,Basis_> >,
    // privately inherited because it is an implementation detail
    private ArrayStorage_f<Scalar_,
                           AS_VECTOR_SPACE(VectorSpace_)::DIMENSION,
                           USE_MEMBER_ARRAY_,
                           ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,USE_MEMBER_ARRAY_> >::T
{
    typedef Vector_i<ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,USE_MEMBER_ARRAY_>,
                     Scalar_,
                     BasedVectorSpace_c<VectorSpace_,Basis_> > Parent_Vector_i;
    typedef typename ArrayStorage_f<Scalar_,
                                    AS_VECTOR_SPACE(VectorSpace_)::DIMENSION,
                                    USE_MEMBER_ARRAY_,
                                    ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,USE_MEMBER_ARRAY_> >::T Parent_Array_i;

    typedef typename Parent_Vector_i::Derived Derived;
    typedef typename Parent_Vector_i::Scalar Scalar;
    typedef typename Parent_Vector_i::BasedVectorSpace BasedVectorSpace;
    using Parent_Vector_i::DIM;
    typedef typename Parent_Vector_i::ComponentIndex ComponentIndex;
    typedef typename Parent_Vector_i::MultiIndex MultiIndex;

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

    using Parent_Array_i::as_derived;
    //using Parent_Vector_i::as_derived;
    using Parent_Array_i::operator[];

    using Parent_Array_i::allocation_size_in_bytes;
    using Parent_Array_i::pointer_to_allocation;

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + TypeStringOf_t<BasedVectorSpace>::eval() + ',' 
                                     + TypeStringOf_t<Scalar>::eval() + ','
                                     + AS_STRING(USE_MEMBER_ARRAY_) + '>';
    }

private:

    // this has no definition, and is designed to generate a compiler error if used (use the
    // one accepting WithoutInitialization instead).  TODO: may need to make this public to
    // allow 0-dimensional vectors, adding a static assert to check that it's actually 
    // 0-dimensional and not being used improperly.
    ImplementationOf_t ();
};

template <typename VectorSpace_, typename Basis_, typename Scalar_, bool USE_MEMBER_ARRAY_>
struct DualOf_f<ImplementationOf_t<BasedVectorSpace_c<VectorSpace_,Basis_>,Scalar_,USE_MEMBER_ARRAY_> >
{
    typedef ImplementationOf_t<typename DualOf_f<BasedVectorSpace_c<VectorSpace_,Basis_> >::T,Scalar_,USE_MEMBER_ARRAY_> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_VECTOR_HPP_
