// ///////////////////////////////////////////////////////////////////////////
// tenh/interop/eigen_selfadjointeigendecomp.hpp by Victor Dods, created 2013/03/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTEROP_EIGEN_SELFADJOINTEIGENDECOMP_HPP_
#define TENH_INTEROP_EIGEN_SELFADJOINTEIGENDECOMP_HPP_

#include "tenh/core.hpp"
#include "tenh/interop/eigen.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2diagonal.hpp"
#include "tenh/tensor2symmetric.hpp"

#include "Eigen/Core"
#include "Eigen/Eigenvalues"

namespace Tenh {

template <typename Factor>
void diagonalize_Tensor2Symmetric (Tensor2Symmetric_t<Factor> const &s,
                                   Tensor2Diagonal_t<Factor,Factor> &eigenvalues,
                                   Tensor2_t<Factor,Factor> &eigenvectors)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::SelfAdjointEigenSolver<EigenMatrix> solver(EigenMatrix_of(s));                                // do the diagonalization
    memcpy(eigenvectors.data_pointer(), &solver.eigenvectors()(0,0), eigenvectors.data_size_in_bytes()); // copy the eigenvectors
    memcpy(eigenvalues.data_pointer(), &solver.eigenvalues()(0,0), eigenvalues.data_size_in_bytes());    // copy the eigenvalues
}

template <typename Factor>
void eigenvalues_of_Tensor2Symmetric (Tensor2Symmetric_t<Factor> const &s,
                                      Tensor2Diagonal_t<Factor,Factor> &eigenvalues)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::SelfAdjointEigenSolver<EigenMatrix> solver(EigenMatrix_of(s), Eigen::EigenvaluesOnly);     // compute eigenvalues
    memcpy(eigenvalues.data_pointer(), &solver.eigenvalues()(0,0), eigenvalues.data_size_in_bytes()); // copy the eigenvalues
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_SELFADJOINTEIGENDECOMP_HPP_
