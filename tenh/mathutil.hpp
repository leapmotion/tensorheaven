// ///////////////////////////////////////////////////////////////////////////
// tenh/mathutil.hpp by Ted Nitz, created 2013/08/15
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_MATHUTIL_HPP_
#define TENH_MATHUTIL_HPP_

#include "tenh/core.hpp"
#include "tenh/meta/lvd.hpp"

// TODO: Decide if/how this code should be split amongst files.
namespace Tenh {

Uint32 factorial(Uint32 x)
{
    return (x > 0) ? factorial(x-1) * x : 1;
}

template<Uint32 X>
struct Factorial_t
{
    static const Uint32 V = Factorial_t<X-1>::V * X;
};

template<>
struct Factorial_t<0>
{
    static const Uint32 V = 1;
};

Uint32 binomial_coefficient(Uint32 n, Uint32 k)
{
  return (k == 0) ? 1 : binomial_coefficient(n,k-1) * (n - k + 1) / k;
}

template<Uint32 N, Uint32 K>
struct BinomialCoefficient_t
{
    static const Uint32 V = BinomialCoefficient_t<N,K-1>::V * (N - K + 1) / K;
};

template<Uint32 N>
struct BinomialCoefficient_t<N,0>
{
    static const Uint32 V = 1;
};

Uint32 index_of_greatest_triangular_number_less_than(Uint32 x, Uint32 d, Uint32 iteration = 0)
{
    return (binomial_coefficient(iteration,d) > x) ? 
           iteration - 1 : 
           index_of_greatest_triangular_number_less_than(x,d,iteration+1);
}

template<Uint32 X, Uint32 D, Uint32 iteration=0>
struct IndexOfGreatestTriangularNumberLessThan_t
{
    static const Uint32 V = If<(BinomialCoefficient_t<iteration,D>::V > X),
                               Value<Uint32,iteration-1>,
                               IndexOfGreatestTriangularNumberLessThan_t<X,D,iteration+1> >::T::V;
};

} // end of namespace Tenh

#endif // TENH_MATHUTIL_HPP_
