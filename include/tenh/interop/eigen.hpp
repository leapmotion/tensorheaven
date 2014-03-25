// ///////////////////////////////////////////////////////////////////////////
// tenh/interop/eigen.hpp by Victor Dods, created 2013/03/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTEROP_EIGEN_HPP_
#define TENH_INTEROP_EIGEN_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/tensor.hpp"

#include "Eigen/Core"
#include "Eigen/LU"

namespace Tenh {

template <typename Factor0_, typename Factor1_, typename Scalar_>
struct EigenMapOf2Tensor_const_f
{
    typedef Eigen::Map<Eigen::Matrix<Scalar_,
                                     DimensionOf_f<Factor0_>::V,
                                     DimensionOf_f<Factor1_>::V,
                                     Eigen::RowMajor> const> T;
};

template <typename Factor0_, typename Factor1_, typename Scalar_>
struct EigenMapOf2Tensor_nonconst_f
{
    typedef Eigen::Map<Eigen::Matrix<Scalar_,
                                     DimensionOf_f<Factor0_>::V,
                                     DimensionOf_f<Factor1_>::V,
                                     Eigen::RowMajor>> T;
};

// const version
template <typename Derived_,
          typename Scalar_,
          typename Factor0_,
          typename Factor1_>
typename EigenMapOf2Tensor_const_f<Factor0_,Factor1_,Scalar_>::T
    EigenMap_of_2tensor (Tensor_i<Derived_,
                                  Scalar_,
                                  TensorProductOfBasedVectorSpaces_c<Typle_t<Factor0_,Factor1_>>,
                                  ComponentQualifier::NONCONST_MEMORY> const &t)
{
    return typename EigenMapOf2Tensor_const_f<Factor0_,Factor1_,Scalar_>::T(t.as_derived().pointer_to_allocation());
}

// non-const version
template <typename Derived_,
          typename Scalar_,
          typename Factor0_,
          typename Factor1_>
typename EigenMapOf2Tensor_nonconst_f<Factor0_,Factor1_,Scalar_>::T
    EigenMap_of_2tensor (Tensor_i<Derived_,
                                  Scalar_,
                                  TensorProductOfBasedVectorSpaces_c<Typle_t<Factor0_,Factor1_>>,
                                  ComponentQualifier::NONCONST_MEMORY> &t)
{
    return typename EigenMapOf2Tensor_nonconst_f<Factor0_,Factor1_,Scalar_>::T(t.as_derived().pointer_to_allocation());
}

template <typename Type_, typename Scalar_>
struct EigenMapOfVector_const_f
{
    typedef Eigen::Map<Eigen::Matrix<Scalar_,
                                     DimensionOf_f<Type_>::V,
                                     1,
                                     Eigen::ColMajor> const> T;
};

template <typename Type_, typename Scalar_>
struct EigenMapOfVector_nonconst_f
{
    typedef Eigen::Map<Eigen::Matrix<Scalar_,
                                     DimensionOf_f<Type_>::V,
                                     1,
                                     Eigen::ColMajor>> T;
};

// const version
template <typename Derived_, typename Scalar_, typename BasedVectorSpace_>
typename EigenMapOfVector_const_f<BasedVectorSpace_,Scalar_>::T
    EigenMap_of_vector (Vector_i<Derived_,Scalar_,BasedVectorSpace_,ComponentQualifier::NONCONST_MEMORY> const &v)
{
    return typename EigenMapOfVector_const_f<BasedVectorSpace_,Scalar_>::T(v.as_derived().pointer_to_allocation());
}

// non-const version
template <typename Derived_, typename Scalar_, typename BasedVectorSpace_>
typename EigenMapOfVector_nonconst_f<BasedVectorSpace_,Scalar_>::T
    EigenMap_of_vector (Vector_i<Derived_,Scalar_,BasedVectorSpace_,ComponentQualifier::NONCONST_MEMORY> &v)
{
    return typename EigenMapOfVector_nonconst_f<BasedVectorSpace_,Scalar_>::T(v.as_derived().pointer_to_allocation());
}

template <typename Derived_,
          typename Scalar_,
          typename Factor0_,
          typename Factor1_>
Scalar_ determinant_of_2tensor (Tensor_i<Derived_,
                                         Scalar_,
                                         TensorProductOfBasedVectorSpaces_c<Typle_t<Factor0_,Factor1_>>,
                                         ComponentQualifier::NONCONST_MEMORY> const &t)
{
    static_assert(DimensionOf_f<Factor0_>::V == DimensionOf_f<Factor1_>::V, "factor dimensions must be equal");
    // create Eigen Maps for the parameter -- this way no copying is necessary;
    // the t tensor's components are read directly by Eigen.
    typedef Eigen::Matrix<Scalar_,
                          DimensionOf_f<Factor0_>::V,
                          DimensionOf_f<Factor1_>::V,
                          Eigen::RowMajor> EigenMatrixType;
    typename EigenMapOf2Tensor_const_f<Factor0_,Factor1_,Scalar_>::T eigen_map_of_t(EigenMap_of_2tensor(t));

    if (DimensionOf_f<Factor0_>::V > 4)
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
