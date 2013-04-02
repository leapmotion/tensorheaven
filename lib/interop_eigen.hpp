// ///////////////////////////////////////////////////////////////////////////
// interop_eigen.hpp by Victor Dods, created 2013/03/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef INTEROP_EIGEN_HPP_
#define INTEROP_EIGEN_HPP_

#include "core.hpp"
#include "tensor2.hpp"

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

template <typename Tensor2Type>
Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor1::DIM,Tensor2Type::Factor2::DIM,Eigen::RowMajor>
    EigenMatrix_of (Tensor2Type const &t)
{
    Tensor2_t<typename Tensor2Type::Factor1,typename Tensor2Type::Factor2> temp(Static<>::WITHOUT_INITIALIZATION);
    NamedIndex_t<typename Tensor2Type::Factor1,'i'> i;
    NamedIndex_t<typename Tensor2Type::Factor2,'j'> j;
    temp(i,j) = t(i,j); // "blow up" t into temp
    return EigenMap_of_Tensor2(temp); // NOTE: this converts into Eigen::Matrix upon return
}

#endif // INTEROP_EIGEN_HPP_
