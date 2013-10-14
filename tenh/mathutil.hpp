// ///////////////////////////////////////////////////////////////////////////
// tenh/mathutil.hpp by Ted Nitz, created 2013/08/15
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_MATHUTIL_HPP_
#define TENH_MATHUTIL_HPP_

#include "tenh/core.hpp"
#include "tenh/meta/typelist.hpp"

// TODO: Decide if/how this code should be split amongst files.
namespace Tenh {

inline Uint32 factorial(Uint32 x)
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

inline Uint32 binomial_coefficient(Uint32 n, Uint32 k)
{
    return (k > n) ? 0 : (k == 0) ? 1 : binomial_coefficient(n,k-1) * (n + 1 - k) / k;
}

template<Uint32 N, Uint32 K>
struct BinomialCoefficientImpl_t
{
    static const Uint32 V = BinomialCoefficientImpl_t<N,K-1>::V * (N + 1 - K) / K;
};

template<Uint32 N>
struct BinomialCoefficientImpl_t<N,0>
{
    static const Uint32 V = 1;
};

template<Uint32 N, Uint32 K>
struct BinomialCoefficient_t
{
	static const Uint32 V = If_f<(K > N), Value_t<Uint32,0>, BinomialCoefficientImpl_t<N,K> >::T::V;
};

inline Uint32 index_of_greatest_triangular_number_less_than(Uint32 x, Uint32 d, Uint32 iteration = 0)
{
    return (binomial_coefficient(iteration,d) > x) ?
           iteration - 1 :
           index_of_greatest_triangular_number_less_than(x,d,iteration+1);
}

template<Uint32 X, Uint32 D, Uint32 iteration=0>
struct IndexOfGreatestTriangularNumberLessThan_t
{
    static const Uint32 V = If_f<(BinomialCoefficient_t<iteration,D>::V > X),
                                 Value_t<Uint32,iteration-1>,
                                 IndexOfGreatestTriangularNumberLessThan_t<X,D,iteration+1> >::T::V;
};

// ValueType_ must be an integral type
template <typename ValueType_, ValueType_ STARTING_VALUE_, ValueType_ ENDING_VALUE_>
struct EnumeratedRange_f
{
    enum { STATIC_ASSERT_IN_ENUM(STARTING_VALUE_ <= ENDING_VALUE_, UNSPECIFIED_MESSAGE) }; // TODO: real message
    typedef TypeList_t<Value_t<ValueType_,STARTING_VALUE_>,
                       typename EnumeratedRange_f<ValueType_,STARTING_VALUE_+1,ENDING_VALUE_>::T> T;
};

template <typename ValueType_, ValueType_ STARTING_AND_ENDING_VALUE_>
struct EnumeratedRange_f<ValueType_,STARTING_AND_ENDING_VALUE_,STARTING_AND_ENDING_VALUE_>
{
    typedef TypeList_t<Value_t<ValueType_,STARTING_AND_ENDING_VALUE_> > T;
};

} // end of namespace Tenh

#endif // TENH_MATHUTIL_HPP_
