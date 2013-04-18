// ///////////////////////////////////////////////////////////////////////////
// tenh/innerproduct.hpp by Victor Dods, created 2013/03/19
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INNERPRODUCT_HPP_
#define TENH_INNERPRODUCT_HPP_

#include "tenh/core.hpp"

namespace Tenh {

// TODO: somehow make this use the Tensor_i interface, so that arbitrary
// 2-tensors can be used for inner products.
// this particular implementation detail will realistically require that
// particular operations are optimized (e.g. if the 2-tensor is diagonal, then
// contracting it on both sides with a vector doesn't require the full matrix
// multiply, just a weighted dot product).

// template specializations of this is what allows you to specify custom inner products for particular
// vector and tensor types.  For example, the space of 3x3 symmetric matrices
//    [a  b  c]
//    [b  d  e]
//    [c  e  f]
// is parameterized by the vector (a,b,c,d,e,f), but the inner product on this space has
// factors of 2 multiplying the off-diagonal components;
//    (a1,b1,c1,d1,e1,f1)*(a2,b2,c2,d2,e2,f2) = a1*a2 + 2*b1*b2 + 2*c1*c2 + d1*d2 + 2*e1*e2 + f1*f2.

template <typename Vector, typename Basis>
struct InnerProduct_t;

struct StandardEuclideanBasis
{
    static std::string type_as_string () { return "StandardEuclideanBasis"; }
};

// template specialization for the standard inner product on a Euclidean space.
// this can be used on any vector type, including tensor types.
template <typename Vector>
struct InnerProduct_t<Vector,StandardEuclideanBasis>
{
    enum { STATIC_ASSERT_IN_ENUM((Lvd::Meta::TypesAreEqual<typename Vector::Basis,StandardEuclideanBasis>::v), BASIS_IS_STANDARD_EUCLIDEAN) };

    typedef typename Vector::Scalar Scalar;
    typedef typename Vector::Index Index;

    // default inner product is the standard Euclidean one -- having identity Gram matrix
    static Scalar component (Index const &i) { return Scalar(1); }
};

} // end of namespace Tenh

#endif // TENH_INNERPRODUCT_HPP_
