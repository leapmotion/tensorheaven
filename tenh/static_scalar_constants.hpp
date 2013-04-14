// ///////////////////////////////////////////////////////////////////////////
// tenh/static_scalar_constants.hpp by Victor Dods, created 2013/04/10
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_STATIC_SCALAR_CONSTANTS_HPP_
#define TENH_STATIC_SCALAR_CONSTANTS_HPP_

#include <cmath>

#include "tenh/core.hpp"

namespace Tenh {

// this file has a template specialization of Static<T> to provide static constants
// of the floating point types.  see below.

// "private" helper functions and classes that aren't part of the publicly facing Tenh API
namespace NonPublic {

template <typename Scalar>
struct StaticScalar
{
    static Scalar const ZERO;
    static Scalar const ONE;
    static Scalar const NEGATIVE_ONE;
    static Scalar const SQRT_TWO;
    static Scalar const EPSILON;
};

template <typename Scalar> Scalar const StaticScalar<Scalar>::ZERO(0);
template <typename Scalar> Scalar const StaticScalar<Scalar>::ONE(1);
template <typename Scalar> Scalar const StaticScalar<Scalar>::NEGATIVE_ONE(-1);
template <typename Scalar> Scalar const StaticScalar<Scalar>::SQRT_TWO(M_SQRT2);

// used for comparing floating point values, accounting for roundoff error
template <> float const StaticScalar<float>::EPSILON(1e-06);
template <> double const StaticScalar<double>::EPSILON(1e-14);

} // end of namespace NonPublic

// ////////////////////////////////////////////////////////////////////////////
// specializations for the revelant floating point types
// ////////////////////////////////////////////////////////////////////////////

template <>
struct Static<float> : public NonPublic::StaticScalar<float> { };

template <>
struct Static<double> : public NonPublic::StaticScalar<double> { };

// TODO: add one for long double? (look at the C++ specification of long double -- is it guaranteed
// to be a type that is distinct from double?)

} // end of namespace Tenh

#endif // TENH_STATIC_SCALAR_CONSTANTS_HPP_
