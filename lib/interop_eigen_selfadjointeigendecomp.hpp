#ifndef INTEROP_EIGEN_SELFADJOINTEIGENDECOMP_HPP_
#define INTEROP_EIGEN_SELFADJOINTEIGENDECOMP_HPP_

#include "core.hpp"
#include "interop_eigen.hpp"
#include "tensor2.hpp"
#include "tensor2diagonal.hpp"
#include "tensor2symmetric.hpp"

#include "Eigen/Core"
#include "Eigen/Eigenvalues"

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

#endif // INTEROP_EIGEN_SELFADJOINTEIGENDECOMP_HPP_
