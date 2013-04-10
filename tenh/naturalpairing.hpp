// ///////////////////////////////////////////////////////////////////////////
// tenh/naturalpairing.hpp by Victor Dods, created 2013/03/19
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_NATURALPAIRING_HPP_
#define TENH_NATURALPAIRING_HPP_

#include "tenh/core.hpp"

namespace Tenh {

// TODO: somehow make this use the Tensor_i interface, so that arbitrary
// 2-tensors can be used for natural pairings (and by extension, inner products).
// this particular implementation detail will realistically require that
// particular operations are optimized (e.g. if the 2-tensor is diagonal, then
// contracting it on both sides with a vector doesn't require the full matrix
// multiply, just a weighted dot product).

// template specializations of this is what allows you to specify custom natural pairings (dot products) for particular
// vector and tensor types.  For example, the space of 3x3 symmetric matrices
//    [a  b  c]
//    [b  d  e]
//    [c  e  f]
// is parameterized by the vector (a,b,c,d,e,f), but the natural pairing (dot product) on this space has
// factors of 2 multiplying the off-diagonal components;
//    (a1,b1,c1,d1,e1,f1)*(a2,b2,c2,d2,e2,f2) = a1*a2 + 2*b1*b2 + 2*c1*c2 + d1*d2 + 2*e1*e2 + f1*f2.

// NOTE: technically this is the inner product, not the natural pairing (we don't have dual spaces yet)
template <typename OwnerVector>
struct NaturalPairing_t
{
    typedef typename OwnerVector::Scalar Scalar;
    typedef typename OwnerVector::Index Index;

    // default natural pairing uses the identity matrix as its Gram matrix
    static Scalar component (Index const &i) { return Scalar(1); }
};

} // end of namespace Tenh

#endif // TENH_NATURALPAIRING_HPP_
