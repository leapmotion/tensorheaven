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
        (t.data_pointer());
}

template <typename Factor1, typename Factor2, typename Basis, typename Derived>
Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> >
    EigenMap_of_Tensor2 (Tensor2_t<Factor1,Factor2,Basis,Derived> &t)
{
    return Eigen::Map<Eigen::Matrix<typename Factor1::Scalar,Factor1::DIM,Factor2::DIM,Eigen::RowMajor> >
        (t.data_pointer());
}

// TODO: if there was a Tensor2_t "map" (like Eigen::Map), then this could be made more efficient
template <typename Tensor2Type>
Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor1::DIM,Tensor2Type::Factor2::DIM,Eigen::RowMajor>
    euclideanly_embedded_EigenMatrix_from (Tensor2Type const &t)
{
    STATIC_ASSERT(Tensor2Type::IS_TENSOR_I, TYPE_MUST_BE_A_TENSOR_I);
    STATIC_ASSERT(Tensor2Type::DEGREE == 2, DEGREE_MUST_BE_TWO);
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
    STATIC_ASSERT(Tensor2Type::DEGREE == 2, DEGREE_MUST_BE_TWO);
    // TODO: use a memory map instead of a memcpy
    Tensor2_t<typename Tensor2Type::Factor1::WithStandardEuclideanBasis,
              typename Tensor2Type::Factor2::WithStandardEuclideanBasis> euclideanly_embedded_t(Static<WithoutInitialization>::SINGLETON);
    memcpy(euclideanly_embedded_t.data_pointer(), &m(0,0), euclideanly_embedded_t.data_size_in_bytes());
    TypedIndex_t<typename Tensor2Type::Factor1::WithStandardEuclideanBasis,'i'> i;
    TypedIndex_t<typename Tensor2Type::Factor2::WithStandardEuclideanBasis,'j'> j;
    TypedIndex_t<typename Tensor2Type::WithStandardEuclideanBasis,'p'> p;
    TypedIndex_t<Tensor2Type,'q'> q;
    EuclideanEmbeddingInverse_t<Tensor2Type> e_inv;
    // NOTE: bundle does NOT check if euclideanly_embedded_t has the symmetries of Tensor2Type
    t(q).no_alias() = e_inv(q|p) * euclideanly_embedded_t(i|j).bundle(i|j,p);
}

// NOTE: this assumes that the inverse is the same type as the input
template <typename Tensor2Type, typename InverseTensor2Type>
void invert_tensor2 (Tensor2Type const &t, InverseTensor2Type &t_inverse)
{
    STATIC_ASSERT(Tensor2Type::Factor1::DIM == Tensor2Type::Factor2::DIM, FACTOR_DIMENSIONS_MUST_BE_EQUAL);
    // the factors of InverseTensor2Type must be the reverse of those of Tensor2Type
    STATIC_ASSERT((Lvd::Meta::TypesAreEqual<typename Tensor2Type::Factor1,typename InverseTensor2Type::Factor2>::v),
                  FACTOR_TYPE_ERROR_ON_INVERSE_TENSOR_TYPE);
    STATIC_ASSERT((Lvd::Meta::TypesAreEqual<typename Tensor2Type::Factor2,typename InverseTensor2Type::Factor1>::v),
                  FACTOR_TYPE_ERROR_ON_INVERSE_TENSOR_TYPE);
//     Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor1::DIM,Tensor2Type::Factor2::DIM,Eigen::RowMajor>
//         m(euclideanly_embedded_EigenMatrix_from(t).inverse());
//     euclideanly_embedded_EigenMatrix_to(m, t_inverse);
    typedef typename Tensor2Type::Scalar Scalar;
    typedef typename Tensor2Type::Factor1 Factor1;
    typedef typename Tensor2Type::Factor2 Factor2;
    typedef Tensor2_t<Factor1,Factor2> T;
    typedef Tensor2_t<Factor2,Factor1> T_inv;
    T blown_up_t(Static<WithoutInitialization>::SINGLETON);
    TypedIndex_t<Factor1,'i'> i;
    TypedIndex_t<Factor2,'j'> j;
    TypedIndex_t<Factor2,'k'> k;

    typedef Tenh::Tensor2Diagonal_t<Factor2,Factor2> D2;
    D2 inner_product_on_Factor2(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    D2 inner_product_inverse_squared_on_Factor2(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    // manually populate the result tensor
    Lvd::Meta::Assert<(D2::Index::COMPONENT_COUNT == Factor2::Index::COMPONENT_COUNT)>();
    // NOTE: this is only the inverse if the inner product is diagonal (which it currently is)
    for (typename D2::Index i; i.is_not_at_end(); ++i)
    {
        inner_product_on_Factor2[i] =
            Tenh::InnerProduct_t<Factor2,typename Factor2::Basis>::component(typename Factor2::Index(i.value()));
        inner_product_inverse_squared_on_Factor2[i] =
            Scalar(1) /
            sqr(Tenh::InnerProduct_t<Factor2,typename Factor2::Basis>::component(typename Factor2::Index(i.value())));
    }

    std::cerr << FORMAT_VALUE(t) << '\n';
    blown_up_t(i|j).no_alias() = t(i|j);//*inner_product_inverse_squared_on_Factor2(j|k); // blow up into full tensor on correct basis
    std::cerr << FORMAT_VALUE(blown_up_t) << '\n';
    typedef Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor1::DIM,Tensor2Type::Factor2::DIM,Eigen::RowMajor> EigenMatrix;
    EigenMatrix blown_up_t_as_EigenMatrix(EigenMap_of_Tensor2(blown_up_t));
    std::cerr << FORMAT_VALUE(blown_up_t_as_EigenMatrix.determinant()) << '\n';
    typedef Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor2::DIM,Tensor2Type::Factor1::DIM,Eigen::RowMajor> EigenMatrixInverse;
    EigenMatrixInverse blown_up_t_inv(blown_up_t_as_EigenMatrix.inverse());
    std::cerr << FORMAT_VALUE(blown_up_t_inv) << '\n';
    std::cerr << "Eigen's product:\n" << (blown_up_t_as_EigenMatrix * blown_up_t_inv) << '\n';
//     T_inv &temp = *reinterpret_cast<T_inv *>(&blown_up_t_inv(0,0));
    T_inv temp(Static<WithoutInitialization>::SINGLETON);
    memcpy(temp.data_pointer(), &blown_up_t_inv(0,0), temp.data_size_in_bytes());
    std::cerr << FORMAT_VALUE(temp) << '\n';
    TypedIndex_t<InverseTensor2Type,'p'> p;
    std::cerr << FORMAT_VALUE(temp(i|j)*inner_product_inverse_squared_on_Factor2(j|k)) << '\n';
    t_inverse(p).no_alias() = (temp(i|j)*inner_product_inverse_squared_on_Factor2(j|k)).bundle(i|k,p);

//     typedef Eigen::Matrix<typename Tensor2Type::Scalar,Tensor2Type::Factor1::DIM,Tensor2Type::Factor2::DIM,Eigen::RowMajor> EigenMatrix;
//     EigenMatrix a = EigenMap_of_Tensor2(t);
//     std::cerr << FORMAT_VALUE(a.determinant()) << '\n';
//     EigenMatrix b(a.inverse());
//     // TEMP HIPPO
//     typedef Tensor2_t<Factor1,Factor2> T;
//     T &temp = *reinterpret_cast<T *>(&b(0,0));
//     TypedIndex_t<Factor1,'i'> i;
//     TypedIndex_t<Factor2,'j'> j;
//     TypedIndex_t<InverseTensor2Type,'p'> p;
//     t_inverse(p).no_alias() = temp(i|j).bundle(i|j,p);
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_HPP_
