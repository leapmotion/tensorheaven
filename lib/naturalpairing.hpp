#ifndef NATURALPAIRING_HPP_
#define NATURALPAIRING_HPP_

#include "core.hpp"

// template specializations of this is what allows you to specify custom natural pairings (dot products) for particular
// vector and tensor types.  For example, the space of 3x3 symmetric matrices
//    [a  b  c]
//    [b  d  e]
//    [c  e  f]
// is parameterized by the vector (a,b,c,d,e,f), but the natural pairing (dot product) on this space has
// factors of 2 multiplying the off-diagonal components;
//    (a1,b1,c1,d1,e1,f1)*(a2,b2,c2,d2,e2,f2) = a1*a2 + 2*b1*b2 + 2*c1*c2 + d1*d2 + 2*e1*e2 + f1*f2.
template <typename OwnerVector>
struct NaturalPairing_t
{
    typedef typename OwnerVector::Scalar Scalar;
    typedef typename OwnerVector::Index Index;

    // default natural pairing uses the identity matrix as its Gram matrix
    static Scalar component (Index const &i) { return Scalar(1); }
};

#endif // NATURALPAIRING_HPP_
