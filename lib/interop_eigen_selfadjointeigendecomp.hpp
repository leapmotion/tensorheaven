#ifndef INTEROP_EIGEN_SELFADJOINTEIGENDECOMP_HPP_
#define INTEROP_EIGEN_SELFADJOINTEIGENDECOMP_HPP_

#include "core.hpp"
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
    Tensor2_t<Factor,Factor> t(Static<>::WITHOUT_INITIALIZATION);
    NamedIndex_t<Factor,'i'> i;
    NamedIndex_t<Factor,'j'> j;
    t(i,j) = s(i,j); // "blow up" s into t (redundant -- Eigen really only needs lower-triangular part)
    Eigen::Map<EigenMatrix> m(t.data_pointer()); // m functions as an EigenMatrix, using the memory from t
    Eigen::SelfAdjointEigenSolver<EigenMatrix> solver(m); // do the diagonalization
    memcpy(eigenvectors.data_pointer(), &solver.eigenvectors(), eigenvectors.data_size_in_bytes()); // copy the eigenvectors
    memcpy(eigenvalues.data_pointer(), &solver.eigenvalues(), eigenvalues.data_size_in_bytes());    // copy the eigenvalues
}

template <typename Factor>
void eigenvalues_of_Tensor2Symmetric (Tensor2Symmetric_t<Factor> const &s,
                                      Tensor2Diagonal_t<Factor,Factor> &eigenvalues)
{
    typedef Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor> EigenMatrix;
    Tensor2_t<Factor,Factor> t(Static<>::WITHOUT_INITIALIZATION);
    NamedIndex_t<Factor,'i'> i;
    NamedIndex_t<Factor,'j'> j;
    t(i,j) = s(i,j); // "blow up" s into t (redundant -- Eigen really only needs lower-triangular part)
    Eigen::Map<EigenMatrix> m(t.data_pointer()); // m functions as an EigenMatrix, using the memory from t
    Eigen::SelfAdjointEigenSolver<EigenMatrix> solver(m, Eigen::EigenvaluesOnly); // compute eigenvalues
    memcpy(eigenvalues.data_pointer(), &solver.eigenvalues(), eigenvalues.data_size_in_bytes());    // copy the eigenvalues
}

/*
    std::cout << "test_diagonalization<" << DIM << ">\n";
    std::cout << "*******************************************************\n";

    typedef Vector_t<float,DIM> V;
    typedef Tensor2_t<V,V> T;
    typedef Tensor2Diagonal_t<V,V> D;
    typedef Tensor2Symmetric_t<V> S;
    
    S s(Static<>::WITHOUT_INITIALIZATION);
    for (typename S::Index i; i.is_not_at_end(); ++i)
        s[i] = i.value() + 1;
        
    // "blow up" s into t (redundant -- Eigen really only needs lower-triangular part)
    T t(Static<>::WITHOUT_INITIALIZATION);
    NamedIndex_t<V,'i'> i;
    NamedIndex_t<V,'j'> j;
    NamedIndex_t<V,'k'> k;
    NamedIndex_t<V,'l'> l;
    t(i,j) = s(i,j);
    std::cout << FORMAT_VALUE(t) << '\n';

    typedef Eigen::Matrix<float,DIM,DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::Map<EigenMatrix> map(t.data_pointer());
    
    Eigen::SelfAdjointEigenSolver<EigenMatrix> solver(map);
    std::cout << FORMAT_VALUE(solver.eigenvectors()) << '\n';
    std::cout << FORMAT_VALUE(solver.eigenvalues()) << '\n';
    
    T evec(Static<>::WITHOUT_INITIALIZATION);
    D eval(Static<>::WITHOUT_INITIALIZATION);
    memcpy(evec.data_pointer(), &solver.eigenvectors(), evec.data_size_in_bytes());
    memcpy(eval.data_pointer(), &solver.eigenvalues(), eval.data_size_in_bytes());
    
    std::cout << FORMAT_VALUE(evec) << '\n';
    std::cout << FORMAT_VALUE(eval) << '\n';
    
    std::cout << FORMAT_VALUE(evec(i,j)*eval(j,k)*evec(l,k)) << '\n';
    
    std::cout << '\n';
*/

#endif // INTEROP_EIGEN_SELFADJOINTEIGENDECOMP_HPP_
