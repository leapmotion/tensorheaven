// ///////////////////////////////////////////////////////////////////////////
// tenh/interop/eigen_linearsolve.hpp by Victor Dods, created 2013/12/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTEROP_EIGEN_LINEARSOLVE_HPP_
#define TENH_INTEROP_EIGEN_LINEARSOLVE_HPP_

#include "tenh/core.hpp"

#include "Eigen/SVD"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/interop/eigen.hpp"

namespace Tenh {

// linear least-squares solve of A*x = b, where x is the vector-value being solved for.
template <typename RhsInnerProductId_,
          typename Factor1_,
          typename Factor2_,
          typename Scalar_,
          typename Derived1_,
          typename Derived2_,
          typename Derived3_,
          ComponentQualifier COMPONENT_QUALIFIER0_,
          ComponentQualifier COMPONENT_QUALIFIER1_>
void linear_solve_using_least_squares (Tensor_i<Derived1_,
                                                Scalar_,
                                                TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,
                                                                                   TypeList_t<typename DualOf_f<Factor2_>::T> > >,
                                                COMPONENT_QUALIFIER0_> const &A,
                                       Vector_i<Derived3_,
                                                Scalar_,
                                                Factor2_,
                                                COMPONENTS_ARE_NONCONST_MEMORY> &x,
                                       Vector_i<Derived2_,
                                                Scalar_,
                                                Factor1_,
                                                COMPONENT_QUALIFIER1_> const &b)
{
    // for now require this special case -- non-standard inner products require
    // the use of a non-identity Euclidean embedding.
    STATIC_ASSERT_TYPES_ARE_EQUAL(RhsInnerProductId_,StandardInnerProduct);

    typename EigenMapOf2Tensor_const_f<Factor1_,typename DualOf_f<Factor2_>::T,Scalar_>::T
        EigenMap_of_A(EigenMap_of_2tensor(A));
    typename EigenMapOfVector_nonconst_f<Factor2_,Scalar_>::T EigenMap_of_x(EigenMap_of_vector(x));
    typename EigenMapOfVector_const_f<Factor1_,Scalar_>::T EigenMap_of_b(EigenMap_of_vector(b));
    EigenMap_of_x = EigenMap_of_A.jacobiSvd(Eigen::ComputeFullU|Eigen::ComputeFullV).solve(EigenMap_of_b);
}

} // end of namespace Tenh

#endif // TENH_INTEROP_EIGEN_LINEARSOLVE_HPP_
