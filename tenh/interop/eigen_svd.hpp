// ///////////////////////////////////////////////////////////////////////////
// tenh/interop/eigen_svd.hpp by Victor Dods, created 2013/03/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTEROP_EIGEN_SVD_HPP_
#define TENH_INTEROP_EIGEN_SVD_HPP_

#include "tenh/core.hpp"
#include "tenh/interop/eigen.hpp"
#include "tenh/implementation/diagonal2tensor.hpp"
#include "tenh/implementation/tensor.hpp"

#include "Eigen/Core"
#include "Eigen/SVD"

namespace Tenh {

// We must have t(i|l) == u(i|j) * s(j|k) * v(l|k), i.e. t = u * s * v^T. Also u, and v are square matrices.
template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayTypeT_, typename UseArrayTypeU_, typename UseArrayTypeS_, typename UseArrayTypeV_>
void SVD_of_2tensor (ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                           TypeList_t<Factor2_> > >,
                                        Scalar_,
                                        UseArrayTypeT_> const &t,
                     ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                           TypeList_t<typename DualOf_f<Factor1_>::T > > >,
                                        Scalar_,
                                        UseArrayTypeU_> &u,
                     ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,typename DualOf_f<Factor2_>::T >,
                                        Scalar_,
                                        UseArrayTypeS_> &s,
                     ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor2_,
                                                                           TypeList_t<typename DualOf_f<Factor2_>::T > > >,
                                        Scalar_,
                                        UseArrayTypeV_> &v)
{
    typedef Eigen::Matrix<Scalar_,DimensionOf_f<Factor1_>::V,DimensionOf_f<Factor2_>::V,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_2tensor(t).jacobiSvd(Eigen::ComputeFullU|Eigen::ComputeFullV));
    memcpy(u.pointer_to_allocation(), &svd.matrixU()(0,0), u.allocation_size_in_bytes());
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
    memcpy(v.pointer_to_allocation(), &svd.matrixV()(0,0), v.allocation_size_in_bytes());
}

// SVD with u matrix but no v matrix; the singular values are in s.
template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayTypeT_, typename UseArrayTypeU_, typename UseArrayTypeS_, typename UseArrayTypeV_>
void SVD_of_2tensor (ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                           TypeList_t<Factor2_> > >,
                                        Scalar_,
                                        UseArrayTypeT_> const &t,
                     ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                           TypeList_t<typename DualOf_f<Factor1_>::T > > >,
                                        Scalar_,
                                        UseArrayTypeU_> &u,
                     ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,typename DualOf_f<Factor2_>::T >,
                                        Scalar_,
                                        UseArrayTypeS_> &s)
{
    typedef Eigen::Matrix<Scalar_,DimensionOf_f<Factor1_>::V,DimensionOf_f<Factor2_>::V,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_2tensor(t).jacobiSvd(Eigen::ComputeFullU));
    memcpy(u.pointer_to_allocation(), &svd.matrixU()(0,0), u.allocation_size_in_bytes());
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
}

// SVD with v matrix but no u matrix; the singular values are in s.
template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayTypeT_, typename UseArrayTypeU_, typename UseArrayTypeS_, typename UseArrayTypeV_>
void SVD_of_2tensor (ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                           TypeList_t<Factor2_> > >,
                                        Scalar_,
                                        UseArrayTypeT_> const &t,
                     ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,typename DualOf_f<Factor2_>::T >,
                                        Scalar_,
                                        UseArrayTypeS_> &s,
                     ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor2_,
                                                                           TypeList_t<typename DualOf_f<Factor2_>::T > > >,
                                        Scalar_,
                                        UseArrayTypeV_> &v)
{
    typedef Eigen::Matrix<Scalar_,DimensionOf_f<Factor1_>::V,DimensionOf_f<Factor2_>::V,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_2tensor(t).jacobiSvd(Eigen::ComputeFullV));
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
    memcpy(v.pointer_to_allocation(), &svd.matrixV()(0,0), v.allocation_size_in_bytes());
}

// SVD with no u or v matrix; the singular values are in s.
template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayTypeT_, typename UseArrayTypeU_, typename UseArrayTypeS_, typename UseArrayTypeV_>
void SVD_of_2tensor (ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                           TypeList_t<Factor2_> > >,
                                        Scalar_,
                                        UseArrayTypeT_> const &t,
                     ImplementationOf_t<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,typename DualOf_f<Factor2_>::T >,
                                        Scalar_,
                                        UseArrayTypeS_> &s)
{
    typedef Eigen::Matrix<Scalar_,DimensionOf_f<Factor1_>::V,DimensionOf_f<Factor2_>::V,Eigen::RowMajor> EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_2tensor(t).jacobiSvd());
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_SVD_HPP_
