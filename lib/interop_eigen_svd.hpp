// ///////////////////////////////////////////////////////////////////////////
// interop_eigen_svd.hpp by Victor Dods, created 2013/03/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef INTEROP_EIGEN_SVD_HPP_
#define INTEROP_EIGEN_SVD_HPP_

#include "core.hpp"
#include "interop_eigen.hpp"
#include "tensor2.hpp"
#include "tensor2diagonal.hpp"

#include "Eigen/Core"
#include "Eigen/SVD"

// SVD with u and v matrices; the singular values are in s; t = u * s * v^T
template <typename Factor1, typename Factor2>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2> const &t,
                     Tensor2_t<Factor1,Factor1> &u,
                     Tensor2Diagonal_t<Factor1,Factor2> &s,
                     Tensor2_t<Factor2,Factor2> &v)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_Tensor2(t).jacobiSvd(Eigen::ComputeFullU|Eigen::ComputeFullV));
    memcpy(u.data_pointer(), &svd.matrixU()(0,0), u.data_size_in_bytes());
    memcpy(s.data_pointer(), &svd.singularValues()(0,0), s.data_size_in_bytes());
    memcpy(v.data_pointer(), &svd.matrixV()(0,0), v.data_size_in_bytes());
}

// SVD with u matrix but no v matrix; the singular values are in s.
template <typename Factor1, typename Factor2>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2> const &t,
                     Tensor2_t<Factor1,Factor1> &u,
                     Tensor2Diagonal_t<Factor1,Factor2> &s)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_Tensor2(t).jacobiSvd(Eigen::ComputeFullU));
    memcpy(u.data_pointer(), &svd.matrixU()(0,0), u.data_size_in_bytes());
    memcpy(s.data_pointer(), &svd.singularValues()(0,0), s.data_size_in_bytes());
}

// SVD with v matrix but no u matrix; the singular values are in s.
template <typename Factor1, typename Factor2>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2> const &t,
                     Tensor2Diagonal_t<Factor1,Factor2> &s,
                     Tensor2_t<Factor2,Factor2> &v)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_Tensor2(t).jacobiSvd(Eigen::ComputeFullU));
    memcpy(s.data_pointer(), &svd.singularValues()(0,0), s.data_size_in_bytes());
    memcpy(v.data_pointer(), &svd.matrixV()(0,0), v.data_size_in_bytes());
}

// SVD with no u or v matrix; the singular values are in s.
template <typename Factor1, typename Factor2>
void SVD_of_Tensor2 (Tensor2_t<Factor1,Factor2> const &t,
                     Tensor2Diagonal_t<Factor1,Factor2> &s)
{
    typedef Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_Tensor2(t).jacobiSvd());
    memcpy(s.data_pointer(), &svd.singularValues()(0,0), s.data_size_in_bytes());
}

#endif // INTEROP_EIGEN_SVD_HPP_
