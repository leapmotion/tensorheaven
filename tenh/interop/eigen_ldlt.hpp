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
    memcpy(lower_triangular.pointer_to_allocation(), &eigen_lower(0,0), lower_triangular.allocation_size_in_bytes()); // copy the lower triangular matrix
    RealVector eigen_diagonal(LDLTMatrix.vectorD());
    memcpy(diagonal.pointer_to_allocation(), &eigen_diagonal(0,0), diagonal.allocation_size_in_bytes());              // copy the diagonal matrix
}

template <typename Factor>
void LDLT_Tensor2Symmetric (Tensor2Symmetric_t<Factor> const &s,
                            Tensor2_t<Factor,Factor> &lower_triangular)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::LDLT<EigenMatrix> LDLTMatrix(euclideanly_embedded_EigenMatrix_from(s).ldlt());              // do the decomposition
    EigenMatrix eigen_lower(LDLTMatrix.matrixL());
    memcpy(lower_triangular.pointer_to_allocation(), &eigen_lower(0,0), lower_triangular.allocation_size_in_bytes()); // copy the lower triangular matrix
}

template <typename Factor>
void LDLT_Tensor2Symmetric (Tensor2Symmetric_t<Factor> const &s,
                            Tensor2Diagonal_t<Factor,Factor> &diagonal)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    typedef typename Eigen::internal::plain_col_type<EigenMatrix, typename Factor::Scalar>::type RealVector;
    Eigen::LDLT<EigenMatrix> LDLTMatrix(euclideanly_embedded_EigenMatrix_from(s).ldlt());              // do the decomposition
    RealVector eigen_diagonal(LDLTMatrix.vectorD());
    memcpy(diagonal.pointer_to_allocation(), &eigen_diagonal(0,0), diagonal.allocation_size_in_bytes());              // copy the diagonal diagonal matrix
}

template <typename Factor>
void LDLT_Solve (Tensor2Symmetric_t<Factor> const & a,
                 Factor & x,
                 Factor const & b)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,1,Eigen::ColMajor> EigenVector;
    typedef Tensor2_t<typename Factor::WithStandardEuclideanBasis, typename Factor::WithStandardEuclideanBasis> Tensor;

    Tensor blow_up(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);

    EuclideanEmbedding_t<Factor> e;
    EuclideanEmbeddingInverse_t<Factor> e_inv;

    TypedIndex_t<typename Factor::WithStandardEuclideanBasis,'i'> i;
    TypedIndex_t<Factor,'j'> j;
    TypedIndex_t<Factor,'k'> k;
    TypedIndex_t<typename Factor::WithStandardEuclideanBasis,'l'> l;

    typename Factor::WithStandardEuclideanBasis tensor_return_value(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON),
                                                tensor_column(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);

    blow_up(i|l) = e(i|j)*a(j|k)*e(l|k);

    tensor_column(i) = e(i|j)*b(j);

    EigenVector return_value, eigen_column;

    Eigen::LDLT<EigenMatrix> LDLTMatrix(EigenMap_of_Tensor2(blow_up).ldlt());

    memcpy(&eigen_column(0,0), tensor_column.pointer_to_allocation(), tensor_column.allocation_size_in_bytes());

    return_value = LDLTMatrix.solve(eigen_column);

    memcpy(tensor_return_value.pointer_to_allocation(), &return_value(0,0), tensor_return_value.allocation_size_in_bytes());

    x(j) = e_inv(j|i)*tensor_return_value(i);
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_LDLT_HPP_
