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

template <typename T, typename ResultUseArrayType_ = UsePreallocatedArray> struct RowsOfTwoTensor_f;

template <typename Factor1_, typename Factor2_, typename Scalar_, typename UseArrayType_, typename Derived_, typename ResultUseArrayType_>
struct RowsOfTwoTensor_f<ImplementationOf_t<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,TypeList_t<Factor2_> > >,Scalar_, UseArrayType_, Derived_>, ResultUseArrayType_>
{
private:
    enum { STATIC_ASSERT_IN_ENUM((!IsUseProceduralArray_f<UseArrayType_>::V), MUST_NOT_BE_USE_PROCEDURAL_ARRAY) };
public:
    typedef ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<typename UniformTypeListOfLength_t<Factor2_, DimensionOf_f<Factor1_>::V>::T>, Scalar_, ResultUseArrayType_> T;
    typedef ImplementationOf_t<Factor2_, Scalar_, UseArrayType_> RowType;
};

template <typename T> struct SVDReturnTypesOf_m;

// We must have t(i*l) == u(i*j) * s(j*k) * v(l*k), i.e. t = u * s * v^T. Also u, and v are square matrices.
template <typename Factor1_, typename Factor2_>
struct SVDReturnTypesOf_m<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,TypeList_t<Factor2_> > > >
{
    struct OfU_f
    {
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                   TypeList_t<typename DualOf_f<Factor1_>::T > > > T;
    };

    struct OfS_f
    {
        typedef Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1_,Factor2_> T;
    };

    struct OfV_f
    {
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor2_,
                                                   TypeList_t<typename DualOf_f<Factor2_>::T > > > T;
    };

    struct SolutionVector_f
    {
      typedef typename DualOf_f<Factor2_>::T T;
    };

    struct ConstantTerm_f
    {
      typedef Factor1_ T;
    };
};

template <typename T> struct EigenMatrixFor_f;

template <typename Factor1_, typename Factor2_, typename Scalar_, typename Derived_, bool COMPONENTS_ARE_PROCEDURAL_>
struct EigenMatrixFor_f<Tensor_i<Derived_, Scalar_, TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,TypeList_t<Factor2_> > >, COMPONENTS_ARE_PROCEDURAL_> >
{
    typedef Eigen::Matrix<Scalar_,DimensionOf_f<Factor1_>::V,DimensionOf_f<Factor2_>::V,Eigen::RowMajor> T;
};

template <typename T> struct EigenVectorFor_f;

template <typename Type_, typename Scalar_, typename Derived_, bool COMPONENTS_ARE_PROCEDURAL_>
struct EigenVectorFor_f<Vector_i<Derived_, Scalar_, Type_, COMPONENTS_ARE_PROCEDURAL_> >
{
    typedef Eigen::Matrix<Scalar_,DimensionOf_f<Type_>::V,1,Eigen::RowMajor> T;
};


template <typename DerivedT_, typename DerivedU_, typename DerivedS_, typename DerivedV_, typename Scalar_, typename Concept_, bool COMPONENTS_ARE_PROCEDURAL_>
void SVD_of_2tensor (Tensor_i<DerivedT_, Scalar_, Concept_, COMPONENTS_ARE_PROCEDURAL_> const &t,
                     Tensor_i<DerivedU_, Scalar_, typename SVDReturnTypesOf_m<Concept_>::OfU_f::T, MUTABLE_COMPONENTS> &u,
                     Tensor_i<DerivedS_, Scalar_, typename SVDReturnTypesOf_m<Concept_>::OfS_f::T, MUTABLE_COMPONENTS> &s,
                     Tensor_i<DerivedV_, Scalar_, typename SVDReturnTypesOf_m<Concept_>::OfV_f::T, MUTABLE_COMPONENTS> &v)
{
    typedef typename EigenMatrixFor_f<Tensor_i<DerivedT_, Scalar_, Concept_, COMPONENTS_ARE_PROCEDURAL_> >::T EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_2tensor(t).jacobiSvd(Eigen::ComputeFullU|Eigen::ComputeFullV));
    memcpy(u.pointer_to_allocation(), &svd.matrixU()(0,0), u.allocation_size_in_bytes());
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
    memcpy(v.pointer_to_allocation(), &svd.matrixV()(0,0), v.allocation_size_in_bytes());
}

// SVD with u matrix but no v matrix; the singular values are in s.
template <typename DerivedT_, typename DerivedU_, typename DerivedS_, typename Scalar_, typename Concept_, bool COMPONENTS_ARE_PROCEDURAL_>
void SVD_of_2tensor (Tensor_i<DerivedT_, Scalar_, Concept_, COMPONENTS_ARE_PROCEDURAL_> const &t,
                     Tensor_i<DerivedU_, Scalar_, typename SVDReturnTypesOf_m<Concept_>::OfU_f::T, MUTABLE_COMPONENTS> &u,
                     Tensor_i<DerivedS_, Scalar_, typename SVDReturnTypesOf_m<Concept_>::OfS_f::T, MUTABLE_COMPONENTS> &s)
{
    typedef typename EigenMatrixFor_f<Tensor_i<DerivedT_, Scalar_, Concept_, COMPONENTS_ARE_PROCEDURAL_> >::T EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_2tensor(t).jacobiSvd(Eigen::ComputeFullU));
    memcpy(u.pointer_to_allocation(), &svd.matrixU()(0,0), u.allocation_size_in_bytes());
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
}

// SVD with v matrix but no u matrix; the singular values are in s.
template <typename DerivedT_, typename DerivedS_, typename DerivedV_, typename Scalar_, typename Concept_, bool COMPONENTS_ARE_PROCEDURAL_>
void SVD_of_2tensor (Tensor_i<DerivedT_, Scalar_, Concept_, COMPONENTS_ARE_PROCEDURAL_> const &t,
                     Tensor_i<DerivedS_, Scalar_, typename SVDReturnTypesOf_m<Concept_>::OfS_f::T, MUTABLE_COMPONENTS> &s,
                     Tensor_i<DerivedV_, Scalar_, typename SVDReturnTypesOf_m<Concept_>::OfV_f::T, MUTABLE_COMPONENTS> &v)
{
    typedef typename EigenMatrixFor_f<Tensor_i<DerivedT_, Scalar_, Concept_, COMPONENTS_ARE_PROCEDURAL_> >::T EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_2tensor(t).jacobiSvd(Eigen::ComputeFullV));
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
    memcpy(v.pointer_to_allocation(), &svd.matrixV()(0,0), v.allocation_size_in_bytes());
}

// SVD with no u or v matrix; the singular values are in s.
template <typename DerivedT_, typename DerivedU_, typename DerivedS_, typename DerivedV_, typename Scalar_, typename Concept_, bool COMPONENTS_ARE_PROCEDURAL_>
void SVD_of_2tensor (Tensor_i<DerivedT_, Scalar_, Concept_, COMPONENTS_ARE_PROCEDURAL_> const &t,
                     Tensor_i<DerivedS_, Scalar_, typename SVDReturnTypesOf_m<Concept_>::OfS_f::T, MUTABLE_COMPONENTS> &s)
{
    typedef typename EigenMatrixFor_f<Tensor_i<DerivedT_, Scalar_, Concept_, COMPONENTS_ARE_PROCEDURAL_> >::T EigenMatrix;
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_2tensor(t).jacobiSvd());
    memcpy(s.pointer_to_allocation(), &svd.singularValues()(0,0), s.allocation_size_in_bytes());
}

template <typename DerivedT_, typename DerivedX_, typename DerivedB_, typename Scalar_, typename Concept_, bool COMPONENTS_ARE_PROCEDURAL_, bool COMPONENTS_ARE_PROCEDURAL_b_>
void SVD_solve (Tensor_i<DerivedT_, Scalar_, Concept_, COMPONENTS_ARE_PROCEDURAL_> const &t,
                Vector_i<DerivedX_, Scalar_, typename SVDReturnTypesOf_m<Concept_>::SolutionVector_f::T, MUTABLE_COMPONENTS> &x,
                Vector_i<DerivedB_, Scalar_, typename SVDReturnTypesOf_m<Concept_>::ConstantTerm_f::T, COMPONENTS_ARE_PROCEDURAL_b_> const &b)
{
    typedef typename EigenMatrixFor_f<Tensor_i<DerivedT_, Scalar_, Concept_, COMPONENTS_ARE_PROCEDURAL_> >::T EigenMatrix;

    // Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_2tensor(t).jacobiSvd(Eigen::ComputeThinU|Eigen::ComputeThinV));
    Eigen::JacobiSVD<EigenMatrix> svd(EigenMap_of_2tensor(t).jacobiSvd(Eigen::ComputeFullU|Eigen::ComputeFullV));
    EigenMap_of_vector(x) = svd.solve(EigenMap_of_vector(b));
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_SVD_HPP_
