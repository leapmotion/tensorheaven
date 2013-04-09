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

#include "Eigen/Core"
#include "Eigen/Cholesky"

namespace Tenh {

template <typename Factor>
void LDLT_Tensor2Symmetric (Tensor2Symmetric_t<Factor> const &s,
                            Tensor2Diagonal_t<Factor,Factor> &d,
                            Tensor2_t<Factor,Factor> &l)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::LDLT<EigenMatrix> LDLTMatrix(EigenMatrix_of(s).ldlt());                // do the diagonalization
    memcpy(d.data_pointer(), &LDLTMatrix.MatrixL()(0,0), d.data_size_in_bytes()); // copy the eigenvectors
    memcpy(l.data_pointer(), &LDLTMatrix.VectorD()(0,0), l.data_size_in_bytes()); // copy the eigenvalues
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_LDLT_HPP_
