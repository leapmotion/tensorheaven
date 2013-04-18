// ///////////////////////////////////////////////////////////////////////////
// tenh/interop/eigen.hpp by Victor Dods, created 2013/03/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTEROP_EIGEN_HPP_
#define TENH_INTEROP_EIGEN_HPP_

#include "tenh/core.hpp"
#include "tenh/tensor2.hpp"

#include "Eigen/Core"

namespace Tenh {

template <typename Factor1, typename Factor2, typename Basis, typename Derived>
Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> const>
    EigenMap_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> const &t)
{
    return Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> const>
        (t.data_pointer());
}

template <typename Factor1, typename Factor2, typename Basis, typename Derived>
Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> >
    EigenMap_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> &t)
{
    return Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> >
        (t.data_pointer());
}

template <typename Tensor2Type>
Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor1::DIM,Tensor2Type::Factor2::DIM,Eigen::RowMajor>
    euclideanly_embedded_EigenMatrix_of (Tensor2Type const &t)
{
    Tensor2_t<typename Tensor2Type::WithStandardEuclideanBasis::Factor1,
              typename Tensor2Type::WithStandardEuclideanBasis::Factor2> euclideanly_embedded_t(Static<WithoutInitialization>::SINGLETON);
    TypedIndex_t<typename Tensor2Type::Factor1::WithStandardEuclideanBasis,'i'> i;
    TypedIndex_t<typename Tensor2Type::Factor1,'j'> j;
    TypedIndex_t<typename Tensor2Type::Factor2,'k'> k;
    TypedIndex_t<typename Tensor2Type::Factor2::WithStandardEuclideanBasis,'l'> l;
    EuclideanEmbedding_t<typename Tensor2Type::Factor1> e1;
    EuclideanEmbedding_t<typename Tensor2Type::Factor2> e2;
    euclideanly_embedded_t(i|l).no_alias() = e1(i|j)*t(j|k)*e2(l|k);
    // NOTE: this converts into Eigen::Matrix upon return, so it's not actually returning
    // a reference to a temporary, as it would seem.
    return EigenMap_of_Tensor2(euclideanly_embedded_t);
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_HPP_
