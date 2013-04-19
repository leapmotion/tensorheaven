// ///////////////////////////////////////////////////////////////////////////
// tenh/interop/eigen_svd.hpp by Victor Dods, created 2013/03/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTEROP_EIGEN_SVD_HPP_
#define TENH_INTEROP_EIGEN_SVD_HPP_

#include "tenh/core.hpp"
#include "tenh/interop/eigen.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2diagonal.hpp"

#include "Eigen/Core"
#include "Eigen/SVD"

namespace Tenh {

// SVD with u and v matrices; the singular values are in s; t = u * s * v^T
template <typename Factor1, typename Factor2, typename Basis, typename Derived>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> const &t,
                     Tensor2_t<Factor1,Factor1> &u,
                     Tensor2Diagonal_t<Factor1,Factor2> &s,
                     Tensor2_t<Factor2,Factor2> &v)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(euclideanly_embedded_EigenMatrix_from(t).jacobiSvd(Eigen::ComputeFullU|Eigen::ComputeFullV));
    memcpy(u.data_pointer(), &svd.matrixU()(0,0), u.data_size_in_bytes());
    memcpy(s.data_pointer(), &svd.singularValues()(0,0), s.data_size_in_bytes());
    memcpy(v.data_pointer(), &svd.matrixV()(0,0), v.data_size_in_bytes());
}

// SVD with u matrix but no v matrix; the singular values are in s.
template <typename Factor1, typename Factor2, typename Basis, typename Derived>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> const &t,
                     Tensor2_t<Factor1,Factor1> &u,
                     Tensor2Diagonal_t<Factor1,Factor2> &s)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(euclideanly_embedded_EigenMatrix_from(t).jacobiSvd(Eigen::ComputeFullU));
    memcpy(u.data_pointer(), &svd.matrixU()(0,0), u.data_size_in_bytes());
    memcpy(s.data_pointer(), &svd.singularValues()(0,0), s.data_size_in_bytes());
}

// SVD with v matrix but no u matrix; the singular values are in s.
template <typename Factor1, typename Factor2, typename Basis, typename Derived>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> const &t,
                     Tensor2Diagonal_t<Factor1,Factor2> &s,
                     Tensor2_t<Factor2,Factor2> &v)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(euclideanly_embedded_EigenMatrix_from(t).jacobiSvd(Eigen::ComputeFullU));
    memcpy(s.data_pointer(), &svd.singularValues()(0,0), s.data_size_in_bytes());
    memcpy(v.data_pointer(), &svd.matrixV()(0,0), v.data_size_in_bytes());
}

// SVD with no u or v matrix; the singular values are in s.
template <typename Factor1, typename Factor2, typename Basis, typename Derived>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> const &t,
                     Tensor2Diagonal_t<Factor1,Factor2> &s)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(euclideanly_embedded_EigenMatrix_from(t).jacobiSvd());
    memcpy(s.data_pointer(), &svd.singularValues()(0,0), s.data_size_in_bytes());
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_SVD_HPP_
