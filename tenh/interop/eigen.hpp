// ///////////////////////////////////////////////////////////////////////////
// tenh/interop/eigen.hpp by Victor Dods, created 2013/03/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTEROP_EIGEN_HPP_
#define TENH_INTEROP_EIGEN_HPP_

#include "tenh/core.hpp"
#include "tenh/euclideanembedding.hpp"
#include "tenh/tensor2.hpp"

#include "Eigen/Core"
#include "Eigen/LU"

namespace Tenh {

template <typename Factor1, typename Factor2, typename Basis, typename Derived>
Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> const>
    EigenMap_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> const &t)
{
    return Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> const>
        (t.pointer_to_allocation());
}

template <typename Factor1, typename Factor2, typename Basis, typename Derived>
Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> >
    EigenMap_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> &t)
{
    return Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> >
        (t.pointer_to_allocation());
}

// TODO: if there was a Tensor2_t "map" (like Eigen::Map), then this could be made more efficient
template <typename Tensor2Type>
Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor1::DIM,Tensor2Type::Factor2::DIM,Eigen::RowMajor>
    euclideanly_embedded_EigenMatrix_from (Tensor2Type const &t)
{
    STATIC_ASSERT(Tensor2Type::IS_TENSOR_I, TYPE_MUST_BE_A_TENSOR_I);
    STATIC_ASSERT(Tensor2Type::ORDER == 2, ORDER_MUST_BE_TWO);
    Tensor2_t<typename Tensor2Type::WithStandardEuclideanBasis::Factor1,
              typename Tensor2Type::WithStandardEuclideanBasis::Factor2> euclideanly_embedded_t(Static<WithoutInitialization>::SINGLETON);
    TypedIndex_t<typename Tensor2Type::WithStandardEuclideanBasis::Factor1,'i'> i;
    TypedIndex_t<typename Tensor2Type::WithStandardEuclideanBasis::Factor2,'j'> j;
    TypedIndex_t<typename Tensor2Type::WithStandardEuclideanBasis,'p'> p;
    TypedIndex_t<Tensor2Type,'q'> q;
    EuclideanEmbedding_t<Tensor2Type> e;
    euclideanly_embedded_t(i|j).no_alias() = (e(p|q)*t(q)).split(p,i|j);
    // NOTE: this converts into Eigen::Matrix upon return, so it's not actually returning a reference to a temporary.
    return EigenMap_of_Tensor2(euclideanly_embedded_t);
}

// NOTE: there is no check that m has the symmetries of Tensor2Type -- this must be ensured by the programmer.
template <typename Tensor2Type>
void euclideanly_embedded_EigenMatrix_to (
    Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor1::DIM,Tensor2Type::Factor2::DIM,Eigen::RowMajor> const &m,
    Tensor2Type &t)
{
    STATIC_ASSERT(Tensor2Type::IS_TENSOR_I, TYPE_MUST_BE_A_TENSOR_I);
    STATIC_ASSERT(Tensor2Type::ORDER == 2, ORDER_MUST_BE_TWO);
    // TODO: use a memory map instead of a memcpy
    Tensor2_t<typename Tensor2Type::Factor1::WithStandardEuclideanBasis,
              typename Tensor2Type::Factor2::WithStandardEuclideanBasis> euclideanly_embedded_t(Static<WithoutInitialization>::SINGLETON);
    memcpy(euclideanly_embedded_t.pointer_to_allocation(), &m(0,0), euclideanly_embedded_t.allocation_size_in_bytes());
    TypedIndex_t<typename Tensor2Type::Factor1::WithStandardEuclideanBasis,'i'> i;
    TypedIndex_t<typename Tensor2Type::Factor2::WithStandardEuclideanBasis,'j'> j;
    TypedIndex_t<typename Tensor2Type::WithStandardEuclideanBasis,'p'> p;
    TypedIndex_t<Tensor2Type,'q'> q;
    EuclideanEmbeddingInverse_t<Tensor2Type> e_inv;
    // NOTE: bundle does NOT check if euclideanly_embedded_t has the symmetries of Tensor2Type
    t(q).no_alias() = e_inv(q|p) * euclideanly_embedded_t(i|j).bundle(i|j,p);
}

// NOTE: this assumes that the inverse is the same type as the input
// TODO: function overload for Tensor2_t type, since no blowing-up step is necessary
// this computes the inverse of the given tensor2, notably requiring the squared inverse
// of the inner product on the relevant factor to recover the identity.  for example,
// if t has factors V,W, then t_inverse has factors W,V, and
//
//   t(i|j) * squared_inverse_inner_product_on_W(j|k) * t_inverse(k|l)
//
// will be approximately equal to
//
//   identity_on_V(i|l).
//
// Similarly,
//
//   t_inverse(k|l) * squared_inverse_inner_product_on_V(l|i) * t(i|j)
//
// will be approximately
//
//   identity_on_W(k|j).
template <typename Tensor2Type, typename InverseTensor2Type>
void invert_tensor2 (Tensor2Type const &t, InverseTensor2Type &t_inverse)
{
    STATIC_ASSERT(Tensor2Type::Factor1::DIM == Tensor2Type::Factor2::DIM, FACTOR_DIMENSIONS_MUST_BE_EQUAL);
    // the factors of InverseTensor2Type must be the reverse of those of Tensor2Type
    STATIC_ASSERT((Lvd::Meta::TypesAreEqual<typename Tensor2Type::Factor1,typename InverseTensor2Type::Factor2>::V),
                  FACTOR_TYPE_ERROR_ON_INVERSE_TENSOR_TYPE);
    STATIC_ASSERT((Lvd::Meta::TypesAreEqual<typename Tensor2Type::Factor2,typename InverseTensor2Type::Factor1>::V),
                  FACTOR_TYPE_ERROR_ON_INVERSE_TENSOR_TYPE);
    typedef typename Tensor2Type::Scalar Scalar;
    typedef typename Tensor2Type::Factor1 Factor1;
    typedef typename Tensor2Type::Factor2 Factor2;
    typedef Tensor2_t<Factor1,Factor2> T;
    typedef Tensor2_t<Factor2,Factor1> T_inv;
    T blown_up_t(Static<WithoutInitialization>::SINGLETON);
    TypedIndex_t<Factor1,'i'> i;
    TypedIndex_t<Factor2,'j'> j;
    blown_up_t(i|j).no_alias() = t(i|j);
    typedef Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor2::DIM,Tensor2Type::Factor1::DIM,Eigen::RowMajor> EigenMatrixInverse;
    EigenMatrixInverse temp2(EigenMap_of_Tensor2(blown_up_t).inverse());
    T_inv blown_up_t_inverse(Static<WithoutInitialization>::SINGLETON);
    memcpy(blown_up_t_inverse.pointer_to_allocation(), &temp2(0,0), blown_up_t_inverse.allocation_size_in_bytes());
    TypedIndex_t<InverseTensor2Type,'p'> p;
    t_inverse(p).no_alias() = (blown_up_t_inverse(j|i)).bundle(j|i,p);
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_HPP_
