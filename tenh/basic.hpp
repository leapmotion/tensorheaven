// ///////////////////////////////////////////////////////////////////////////
// tenh/basic.hpp by Victor Dods, created 2013/10/11
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_BASIC_HPP_
#define TENH_BASIC_HPP_

#include "tenh/core.hpp"

#include "tenh/basic/operator.hpp"
#include "tenh/basic/vector.hpp"
#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/field.hpp"
#include "tenh/conceptual/vectorspace.hpp"

namespace Tenh {

// Very easy to use templatized vector class -- default template parameters are provided so that
// minimal notation is required for simple use.  For example, Vec<3> provides a 3-dimensional
// real vector (represented using floats) in a generic orthonormal basis.
template <Uint32 DIMENSION_,
          typename Scalar_ = float,
          typename Basis_ = OrthonormalBasis_c<Generic>,
          typename VectorSpaceId_ = Generic,
          typename UseArrayType_ = UseMemberArray_t<COMPONENTS_ARE_NONCONST>,
          typename ScalarField_ = RealField>
struct Vec : public Vector<BasedVectorSpace_c<VectorSpace_c<ScalarField_,DIMENSION_,VectorSpaceId_>,Basis_>,Scalar_,UseArrayType_>
{
private:
    typedef Vector<BasedVectorSpace_c<VectorSpace_c<ScalarField_,DIMENSION_,VectorSpaceId_>,Basis_>,Scalar_,UseArrayType_> Parent_Vector;
public:
    typedef Scalar_ Scalar;
    typedef typename Parent_Vector::ComponentIndex ComponentIndex;
    using Parent_Vector::DIM;
    typedef typename DualOf_f<Vec>::T Dual; // relies on the template specialization below

    explicit Vec (WithoutInitialization const &w) : Parent_Vector(w) { }

    // TODO: copy constructor and operator= (for appropriate UseArrayType_)

    // only use these if UseMemberArray_t<...> is specified

    // probably only useful for zero element (because this is basis-dependent)
    template <typename T_>
    explicit Vec (FillWith_t<T_> const &fill_with)
        :
        Parent_Vector(fill_with)
    {
        STATIC_ASSERT(IsUseMemberArray_f<UseArrayType_>::V, MUST_BE_USE_MEMBER_ARRAY);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Vec (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x)
        :
        Parent_Vector(x.as_member_array())
    {
        STATIC_ASSERT(IsUseMemberArray_f<UseArrayType_>::V, MUST_BE_USE_MEMBER_ARRAY);
    }

    // only use these if UsePreallocatedArray_t<...> is specified

    explicit Vec (Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Vector(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(IsUsePreallocatedArray_f<UseArrayType_>::V, MUST_BE_USE_PREALLOCATED_ARRAY);
    }
    template <typename T_>
    Vec (FillWith_t<T_> const &fill_with,
         Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Vector(fill_with, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(IsUsePreallocatedArray_f<UseArrayType_>::V, MUST_BE_USE_PREALLOCATED_ARRAY);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Vec (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x,
         Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Vector(x, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(IsUsePreallocatedArray_f<UseArrayType_>::V, MUST_BE_USE_PREALLOCATED_ARRAY);
    }

    // only use this if UseProceduralArray_t<...> is specified or if the vector space is 0-dimensional
    Vec ()
        :
        Parent_Vector(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseProceduralArray_f<UseArrayType_>::V || DIMENSION_ == 0,
                      MUST_BE_USE_PROCEDURAL_ARRAY_OR_BE_ZERO_DIMENSIONAL);
    }

    using Parent_Vector::operator =;

    static std::string type_as_string ()
    {
        return "Vec<" + AS_STRING(DIMENSION_) + ','
                      + type_string_of<Scalar_>() + ','
                      + type_string_of<Basis_>() + ','
                      + type_string_of<UseArrayType_>() + ','
                      + type_string_of<ScalarField_>() + '>';
    }
};

template <Uint32 DIMENSION_,
          typename Scalar_,
          typename Basis_,
          typename VectorSpaceId_,
          typename UseArrayType_,
          typename ScalarField_>
struct DualOf_f<Vec<DIMENSION_,Scalar_,Basis_,VectorSpaceId_,UseArrayType_,ScalarField_> >
{
    // The "dual" type of a vector is just a covector.  Note that Generic is self-dual
    typedef Vec<DIMENSION_,
                Scalar_,
                typename DualOf_f<Basis_>::T,
                typename DualOf_f<VectorSpaceId_>::T,
                typename DualOf_f<UseArrayType_>::T,
                ScalarField_> T;
private:
    DualOf_f();
};

// Very easy to use templatized linear operator class -- default template parameters are provided
// so that minimal notation is required for simple use.  For example, Op<3,2> effectively provides
// a 3x2 matrix, whose domain and codomain are effectively Vec<2> and Vec<3> (see Vec<...>)
template <Uint32 CODOMAIN_DIMENSION_,
          Uint32 DOMAIN_DIMENSION_,
          typename Scalar_ = float,
          typename CodomainBasis_ = OrthonormalBasis_c<Generic>,
          typename DomainBasis_ = OrthonormalBasis_c<Generic>,
          typename CodomainId_ = Generic,
          typename DomainId_ = Generic,
          typename UseArrayType_ = UseMemberArray_t<COMPONENTS_ARE_NONCONST>,
          typename ScalarField_ = RealField>
struct Op
    :
    public Operator<BasedVectorSpace_c<VectorSpace_c<ScalarField_,DOMAIN_DIMENSION_,DomainId_>,DomainBasis_>,
                    BasedVectorSpace_c<VectorSpace_c<ScalarField_,CODOMAIN_DIMENSION_,CodomainId_>,CodomainBasis_>,
                    Scalar_,
                    UseArrayType_>
{
private:
    typedef Operator<BasedVectorSpace_c<VectorSpace_c<ScalarField_,DOMAIN_DIMENSION_,DomainId_>,DomainBasis_>,
                     BasedVectorSpace_c<VectorSpace_c<ScalarField_,CODOMAIN_DIMENSION_,CodomainId_>,CodomainBasis_>,
                     Scalar_,
                     UseArrayType_> Parent_Operator;
public:
    typedef Scalar_ Scalar;
    typedef typename Parent_Operator::ComponentIndex ComponentIndex;
    typedef typename Parent_Operator::MultiIndex MultiIndex;
    using Parent_Operator::DIM;
    typedef typename DualOf_f<Op>::T Dual; // relies on the template specialization below

    explicit Op (WithoutInitialization const &w) : Parent_Operator(w) { }
    Op (Op const &op) : Parent_Operator(op) { }

    // TODO: copy constructor and operator= (for appropriate UseArrayType_)

    // only use these if UseMemberArray_t<...> is specified

    // probably only useful for zero element (because this is basis-dependent)
    explicit Op (Scalar_ const &fill_with)
        :
        Parent_Operator(fill_with)
    {
        STATIC_ASSERT(IsUseMemberArray_f<UseArrayType_>::V, MUST_BE_USE_MEMBER_ARRAY);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Op (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x)
        :
        Parent_Operator(x.as_member_array())
    {
        STATIC_ASSERT(IsUseMemberArray_f<UseArrayType_>::V, MUST_BE_USE_MEMBER_ARRAY);
    }

    // only use these if UsePreallocatedArray_t<...> is specified

    explicit Op (Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Operator(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(IsUsePreallocatedArray_f<UseArrayType_>::V, MUST_BE_USE_PREALLOCATED_ARRAY);
    }
    Op (Scalar_ const &fill_with,
        Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Operator(fill_with, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(IsUsePreallocatedArray_f<UseArrayType_>::V, MUST_BE_USE_PREALLOCATED_ARRAY);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    Op (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x,
        Scalar_ *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Operator(x, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT(IsUsePreallocatedArray_f<UseArrayType_>::V, MUST_BE_USE_PREALLOCATED_ARRAY);
    }

    // only use this if UseProceduralArray_t<...> is specified or if the vector space is 0-dimensional
    Op ()
        :
        Parent_Operator(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseProceduralArray_f<UseArrayType_>::V || DIM == 0,
                      MUST_BE_USE_PROCEDURAL_ARRAY_OR_BE_ZERO_DIMENSIONAL);
    }

    using Parent_Operator::operator =;

    static std::string type_as_string ()
    {
        return "Op<" + AS_STRING(CODOMAIN_DIMENSION_) + ','
                     + AS_STRING(DOMAIN_DIMENSION_) + ','
                     + type_string_of<Scalar_>() + ','
                     + type_string_of<CodomainBasis_>() + ','
                     + type_string_of<DomainBasis_>() + ','
                     + type_string_of<CodomainId_>() + ','
                     + type_string_of<DomainId_>() + ','
                     + type_string_of<UseArrayType_>() + ','
                     + type_string_of<ScalarField_>() + '>';
    }
};

template <Uint32 CODOMAIN_DIMENSION_,
          Uint32 DOMAIN_DIMENSION_,
          typename Scalar_,
          typename CodomainBasis_,
          typename DomainBasis_,
          typename CodomainId_,
          typename DomainId_,
          typename UseArrayType_,
          typename ScalarField_>
struct DualOf_f<Op<CODOMAIN_DIMENSION_,
                   DOMAIN_DIMENSION_,
                   Scalar_,
                   CodomainBasis_,
                   DomainBasis_,
                   CodomainId_,
                   DomainId_,
                   UseArrayType_,
                   ScalarField_> >
{
    // The "dual" type of a linear operator is its natural adjoint; if
    // A : V --> W, then A* : W* --> V*, where * denotes the dual functor.
    // Note that Generic is self-dual.
    typedef Op<DOMAIN_DIMENSION_,
               CODOMAIN_DIMENSION_,
               Scalar_,
               typename DualOf_f<DomainBasis_>::T,
               typename DualOf_f<CodomainBasis_>::T,
               typename DualOf_f<DomainId_>::T,
               typename DualOf_f<CodomainId_>::T,
               typename DualOf_f<UseArrayType_>::T,
               ScalarField_> T;
private:
    DualOf_f();
};

} // end of namespace Tenh

#endif // TENH_BASIC_HPP_
