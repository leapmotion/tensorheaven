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

template <typename Tensor2Symmetric, typename DiagonalizingBasis>
struct SelfAdjointEigenDecomposition_t
{
    enum { STATIC_ASSERT_IN_ENUM(Tensor2Symmetric::IS_TENSOR2SYMMETRIC_T, TYPE_MUST_BE_TENSOR2SYMMETRIC_T) };
        
    typedef typename Tensor2Symmetric::Factor Factor;
    typedef Tensor2Diagonal_t<DiagonalizingBasis,DiagonalizingBasis> Eigenvalues;
    typedef Tensor2_t<DiagonalizingBasis,Factor> Eigenvectors;

    // let Basis be Tensor2Symmetric::Factor::Basis
    // the eigenvalues should be in DiagonalizingBasis
    // the eigenvectors should be Hom(Basis, DiagonalizingBasis)
    // Eigen will give eigenvectors as Hom(StandardEuclideanBasis, DiagonalizingBasis)
    // must contract with the inverse of Euclidean embedding, which is Hom(StandardEuclideanBasis, Basis)

    // compute eigenvalues and eigenvectors (this requires a transformation of s to be
    // Euclideanly embedded, and then an inverse transformation of the eigenvectors
    // to be in the original basis.
    void compute (Tensor2Symmetric const &s, Eigenvalues &eigenvalues, Eigenvectors &eigenvectors)
    {
        typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
        Eigen::SelfAdjointEigenSolver<EigenMatrix> solver(euclideanly_embedded_EigenMatrix_of(s));           // do the diagonalization
        memcpy(eigenvalues.data_pointer(), &solver.eigenvalues()(0,0), eigenvalues.data_size_in_bytes());    // copy the eigenvalues
        typedef Tensor2_t<DiagonalizingBasis,typename Factor::WithStandardEuclideanBasis> T;                 // type of computed eigenvectors
        T &euclideanly_embedded_eigenvectors = *reinterpret_cast<T *>(&solver.eigenvalues()(0,0));           // hacky way to avoid a copy
        // transform back to the original basis        
        TypedIndex_t<Factor,'i'> i;
        TypedIndex_t<typename Factor::WithStandardEuclideanBasis,'j'> j;
        TypedIndex_t<typename Factor::WithStandardEuclideanBasis,'k'> k;
        TypedIndex_t<Factor,'l'> l;
        EuclideanEmbeddingInverse_t<Factor> e_inv;
        eigenvectors(i|l).no_alias() = e_inv(i|j)*euclideanly_embedded_eigenvectors(j|k)*e_inv(l|k);
    }

    // compute only eigenvalues
    void compute (Tensor2Symmetric const &s, Eigenvalues &eigenvalues)
    {
        typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
        Eigen::SelfAdjointEigenSolver<EigenMatrix> solver(euclideanly_embedded_EigenMatrix_of(s));           // do the diagonalization
        memcpy(eigenvalues.data_pointer(), &solver.eigenvalues()(0,0), eigenvalues.data_size_in_bytes());    // copy the eigenvalues
    }
};

template <typename Factor, typename Basis, typename Derived>
void diagonalize_Tensor2Symmetric (Tensor2Symmetric_t<Factor,Factor,Basis,Derived> const &s,
                                   Tensor2Diagonal_t<typename Factor::WithStandardEuclideanBasis,
                                                     typename Factor::WithStandardEuclideanBasis> &eigenvalues,
                                   Tensor2_t<typename Factor::WithStandardEuclideanBasis,Factor> &eigenvectors)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::SelfAdjointEigenSolver<EigenMatrix> solver(euclideanly_embedded_EigenMatrix_of(s));           // do the diagonalization
    memcpy(eigenvectors.data_pointer(), &solver.eigenvectors()(0,0), eigenvectors.data_size_in_bytes()); // copy the eigenvectors
    memcpy(eigenvalues.data_pointer(), &solver.eigenvalues()(0,0), eigenvalues.data_size_in_bytes());    // copy the eigenvalues
}

template <typename Factor, typename Basis, typename Derived>
void eigenvalues_of_Tensor2Symmetric (Tensor2Symmetric_t<Factor,Factor,Basis,Derived> const &s,
                                      Tensor2Diagonal_t<typename Factor::WithStandardEuclideanBasis,
                                                        typename Factor::WithStandardEuclideanBasis> &eigenvalues)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::SelfAdjointEigenSolver<EigenMatrix> solver(euclideanly_embedded_EigenMatrix_of(s), Eigen::EigenvaluesOnly); // compute eigenvalues
    memcpy(eigenvalues.data_pointer(), &solver.eigenvalues()(0,0), eigenvalues.data_size_in_bytes());                  // copy the eigenvalues
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_SELFADJOINTEIGENDECOMP_HPP_
