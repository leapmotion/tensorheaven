// ///////////////////////////////////////////////////////////////////////////
// tenh/interop/eigen_selfadjointeigendecomp.hpp by Ted Nitz, created 2013/04/09
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTEROP_EIGEN_LDLT_HPP_
#define TENH_INTEROP_EIGEN_LDLT_HPP_

#include "tenh/core.hpp"
#include "tenh/interop/eigen.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2diagonal.hpp"
#include "tenh/tensor2symmetric.hpp"

#include "Eigen/Cholesky"
#include "Eigen/Core"

namespace Tenh {

// Warning: This code currently doesn't take the permutation matrix into account, so is perhaps not useful.
template <typename Factor>
void LDLT_Tensor2Symmetric (Tensor2Symmetric_t<Factor> const &s,
                            Tensor2Diagonal_t<Factor,Factor> &diagonal,
                            Tensor2_t<Factor,Factor> &lower_triangular)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    typedef typename Eigen::internal::plain_col_type<EigenMatrix, typename Factor::Scalar>::type RealVector;

    Eigen::LDLT<EigenMatrix> LDLTMatrix(euclideanly_embedded_EigenMatrix_from(s).ldlt());              // do the decomposition
    EigenMatrix eigen_lower(LDLTMatrix.matrixL());
    memcpy(lower_triangular.data_pointer(), &eigen_lower(0,0), lower_triangular.data_size_in_bytes()); // copy the lower triangular matrix
    RealVector eigen_diagonal(LDLTMatrix.vectorD());
    memcpy(diagonal.data_pointer(), &eigen_diagonal(0,0), diagonal.data_size_in_bytes());              // copy the diagonal matrix
}

template <typename Factor>
void LDLT_Tensor2Symmetric (Tensor2Symmetric_t<Factor> const &s,
                            Tensor2_t<Factor,Factor> &lower_triangular)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::LDLT<EigenMatrix> LDLTMatrix(euclideanly_embedded_EigenMatrix_from(s).ldlt());              // do the decomposition
    EigenMatrix eigen_lower(LDLTMatrix.matrixL());
    memcpy(lower_triangular.data_pointer(), &eigen_lower(0,0), lower_triangular.data_size_in_bytes()); // copy the lower triangular matrix
}

template <typename Factor>
void LDLT_Tensor2Symmetric (Tensor2Symmetric_t<Factor> const &s,
                            Tensor2Diagonal_t<Factor,Factor> &diagonal)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    typedef typename Eigen::internal::plain_col_type<EigenMatrix, typename Factor::Scalar>::type RealVector;
    Eigen::LDLT<EigenMatrix> LDLTMatrix(euclideanly_embedded_EigenMatrix_from(s).ldlt());              // do the decomposition
    RealVector eigen_diagonal(LDLTMatrix.vectorD());
    memcpy(diagonal.data_pointer(), &eigen_diagonal(0,0), diagonal.data_size_in_bytes());              // copy the diagonal diagonal matrix
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_LDLT_HPP_
