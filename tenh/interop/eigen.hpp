// ///////////////////////////////////////////////////////////////////////////
// tenh/interop/eigen.hpp by Victor Dods, created 2013/03/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTEROP_EIGEN_HPP_
#define TENH_INTEROP_EIGEN_HPP_

#include "tenh/core.hpp"
#include "tenh/implementation/euclideanembedding.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/implementation/tensor.hpp"

#include "Eigen/Core"
#include "Eigen/LU"

namespace Tenh {

template <typename Factor1_, typename Factor2_, typename Scalar_>
struct EigenMapOf2Tensor_const_f
{
    typedef Eigen::Map<Eigen::Matrix<Scalar_,
                                     DimensionOf_f<Factor1_>::V,
                                     DimensionOf_f<Factor2_>::V,
                                     Eigen::RowMajor> const> T;
};

template <typename Factor1_, typename Factor2_, typename Scalar_>
struct EigenMapOf2Tensor_nonconst_f
{
    typedef Eigen::Map<Eigen::Matrix<Scalar_,
                                     DimensionOf_f<Factor1_>::V,
                                     DimensionOf_f<Factor2_>::V,
                                     Eigen::RowMajor> > T;
};

// const version
template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayType_>
typename EigenMapOf2Tensor_const_f<Factor1_,Factor2_,Scalar_>::T
    EigenMap_of_2tensor (ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                               TypeList_t<Factor2_> > >,
                                            Scalar_,
                                            UseArrayType_> const &t)
{
    STATIC_ASSERT(!IsUseImmutableArray_f<UseArrayType_>::V, CANT_MAKE_EIGEN_MAP_OF_IMMUTABLE_ARRAY);
    return typename EigenMapOf2Tensor_const_f<Factor1_,Factor2_,Scalar_>::T(t.pointer_to_allocation());
}

// non-const version
template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayType_>
typename EigenMapOf2Tensor_nonconst_f<Factor1_,Factor2_,Scalar_>::T
    EigenMap_of_2tensor (ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                               TypeList_t<Factor2_> > >,
                                            Scalar_,
                                            UseArrayType_> &t)
{
    STATIC_ASSERT(!IsUseImmutableArray_f<UseArrayType_>::V, CANT_MAKE_EIGEN_MAP_OF_IMMUTABLE_ARRAY);
    return typename EigenMapOf2Tensor_nonconst_f<Factor1_,Factor2_,Scalar_>::T(t.pointer_to_allocation());
}

template <typename Type_, typename Scalar_>
struct EigenMapOfVector_const_f
{
    typedef Eigen::Map<Eigen::Matrix<Scalar_,
                                     DimensionOf_f<Type_>::V,
                                     1,
                                     Eigen::RowMajor> const> T;
};

template <typename Type_, typename Scalar_>
struct EigenMapOfVector_nonconst_f
{
    typedef Eigen::Map<Eigen::Matrix<Scalar_,
                                     DimensionOf_f<Type_>::V,
                                     1,
                                     Eigen::RowMajor> > T;
};

// const version
template <typename Type_, typename Scalar_, typename UseArrayType_>
typename EigenMapOfVector_const_f<Type_,Scalar_>::T
    EigenMap_of_vector (ImplementationOf_t<Type_,
                                           Scalar_,
                                           UseArrayType_> const &t)
{
    STATIC_ASSERT(!IsUseImmutableArray_f<UseArrayType_>::V, CANT_MAKE_EIGEN_MAP_OF_IMMUTABLE_ARRAY);
    return typename EigenMapOfVector_const_f<Type_,Scalar_>::T(t.pointer_to_allocation());
}

// non-const version
template <typename Type_, typename Scalar_, typename UseArrayType_>
typename EigenMapOfVector_nonconst_f<Type_,Scalar_>::T
    EigenMap_of_vector (ImplementationOf_t<Type_,
                                           Scalar_,
                                           UseArrayType_> &t)
{
    STATIC_ASSERT(!IsUseImmutableArray_f<UseArrayType_>::V, CANT_MAKE_EIGEN_MAP_OF_IMMUTABLE_ARRAY);
    return typename EigenMapOfVector_nonconst_f<Type_,Scalar_>::T(t.pointer_to_allocation());
}

// returns true iff the tensor was actually considered to be invertible (by Eigen::MatrixBase::computeInverseWithCheck)
template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayType_, typename InverseUseArrayType_>
bool invert_2tensor (ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                           TypeList_t<Factor2_> > >,
                                        Scalar_,
                                        UseArrayType_> const &t,
                     ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<typename DualOf_f<Factor2_>::T,
                                                                           TypeList_t<typename DualOf_f<Factor1_>::T> > >,
                                        Scalar_,
                                        InverseUseArrayType_> &t_inverse)
{
    STATIC_ASSERT(DimensionOf_f<Factor1_>::V == DimensionOf_f<Factor2_>::V, FACTOR_DIMENSIONS_MUST_BE_EQUAL);
    STATIC_ASSERT(!IsUseImmutableArray_f<UseArrayType_>::V, CANT_MAKE_EIGEN_MAP_OF_IMMUTABLE_ARRAY); // TODO: provide invert for UseImmutableArray
    // create Eigen Maps for each of the parameters -- this way no copying is necessary;
    // the t tensor's components are read directly by Eigen, and t_inverse's components
    // are directly written to by Eigen.
    typedef Eigen::Matrix<Scalar_, DimensionOf_f<Factor1_>::V, DimensionOf_f<Factor2_>::V, Eigen::RowMajor> EigenMatrixType;
    typename EigenMapOf2Tensor_const_f<Factor1_,Factor2_,Scalar_>::T eigen_map_of_t(EigenMap_of_2tensor(t));
    typename EigenMapOf2Tensor_nonconst_f<typename DualOf_f<Factor2_>::T,typename DualOf_f<Factor1_>::T,Scalar_>::T eigen_map_of_t_inverse(EigenMap_of_2tensor(t_inverse));
    Eigen::FullPivLU<EigenMatrixType> lu(eigen_map_of_t);

    if (lu.isInvertible())
    {
       eigen_map_of_t_inverse = lu.inverse();
       return true;
    }
    else
    {
      return false;
    }
}

// returns true iff the tensor was actually considered to be invertible (by Eigen::MatrixBase::computeInverseWithCheck)
template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayType_>
Scalar_ determinant_of_2tensor (ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                           TypeList_t<Factor2_> > >,
                                                  Scalar_,
                                                  UseArrayType_> const &t)
{
    STATIC_ASSERT(DimensionOf_f<Factor1_>::V == DimensionOf_f<Factor2_>::V, FACTOR_DIMENSIONS_MUST_BE_EQUAL);
    STATIC_ASSERT(!IsUseImmutableArray_f<UseArrayType_>::V, CANT_MAKE_EIGEN_MAP_OF_IMMUTABLE_ARRAY); // TODO: provide determinant for UseImmutableArray
    // create Eigen Maps for the parameter -- this way no copying is necessary;
    // the t tensor's components are read directly by Eigen.
    typedef Eigen::Matrix<Scalar_, DimensionOf_f<Factor1_>::V, DimensionOf_f<Factor2_>::V, Eigen::RowMajor> EigenMatrixType;
    typename EigenMapOf2Tensor_const_f<Factor1_,Factor2_,Scalar_>::T eigen_map_of_t(EigenMap_of_2tensor(t));

    if (DimensionOf_f<Factor1_>::V > 4)
    {
      Eigen::FullPivLU<EigenMatrixType> lu(eigen_map_of_t);
      return lu.determinant();
    }
    else
    {
      return eigen_map_of_t.determinant();
    }
}


// TODO: specialization for diagonal 2-tensors

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_HPP_
