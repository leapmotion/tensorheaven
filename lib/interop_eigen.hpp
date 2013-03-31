#ifndef INTEROP_EIGEN_HPP_
#define INTEROP_EIGEN_HPP_

#include "core.hpp"
#include "tensor2.hpp"
#include "tensor2antisymmetric.hpp"
#include "tensor2symmetric.hpp"

#include "Eigen/Core"

template <typename Factor1, typename Factor2>
Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> const>
    EigenMap_of_Tensor2 (Tensor2_t<Factor1,Factor2> const &t)
{
    return Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> const>(t.data_pointer());
}

template <typename Factor1, typename Factor2>
Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> >
    EigenMap_of_Tensor2 (Tensor2_t<Factor1,Factor2> &t)
{
    return Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> >(t.data_pointer());
}

template <typename Factor>
Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor>
    EigenMatrix_of_Tensor2Antisymmetric (Tensor2Antisymmetric_t<Factor> const &a)
{
    Tensor2_t<Factor,Factor> t(Static<>::WITHOUT_INITIALIZATION);
    NamedIndex_t<Factor,'i'> i;
    NamedIndex_t<Factor,'j'> j;
    t(i,j) = a(i,j); // "blow up" a into t
    return EigenMap_of_Tensor2(t);
}

template <typename Factor>
Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor>
    EigenMatrix_of_Tensor2Antisymmetric (Tensor2Antisymmetric_t<Factor> &a)
{
    Tensor2_t<Factor,Factor> t(Static<>::WITHOUT_INITIALIZATION);
    NamedIndex_t<Factor,'i'> i;
    NamedIndex_t<Factor,'j'> j;
    t(i,j) = a(i,j); // "blow up" a into t
    return EigenMap_of_Tensor2(t);
}

template <typename Factor>
Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor>
    EigenMatrix_of_Tensor2Symmetric (Tensor2Symmetric_t<Factor> const &s)
{
    Tensor2_t<Factor,Factor> t(Static<>::WITHOUT_INITIALIZATION);
    NamedIndex_t<Factor,'i'> i;
    NamedIndex_t<Factor,'j'> j;
    t(i,j) = s(i,j); // "blow up" s into t
    return EigenMap_of_Tensor2(t);
}

template <typename Factor>
Eigen::Matrix<typename Factor::Scalar,Factor::DIM,Factor::DIM,Eigen::RowMajor>
    EigenMatrix_of_Tensor2Symmetric (Tensor2Symmetric_t<Factor> &s)
{
    Tensor2_t<Factor,Factor> t(Static<>::WITHOUT_INITIALIZATION);
    NamedIndex_t<Factor,'i'> i;
    NamedIndex_t<Factor,'j'> j;
    t(i,j) = s(i,j); // "blow up" s into t
    return EigenMap_of_Tensor2(t);
}

#endif // INTEROP_EIGEN_HPP_
