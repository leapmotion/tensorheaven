// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/tuple.hpp by Victor Dods, created 2013/02/16
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_META_TUPLE_HPP_
#define TENH_META_TUPLE_HPP_

#include "tenh/core.hpp"

#include "tenh/list.hpp"
#include "tenh/meta/typetuple.hpp"

namespace Tenh {

// sort of hacky way to get limited variadic template behavior
template <typename T0_ = NullType,
          typename T1_ = NullType,
          typename T2_ = NullType,
          typename T3_ = NullType,
          typename T4_ = NullType,
          typename T5_ = NullType,
          typename T6_ = NullType,
          typename T7_ = NullType>
struct Tuple_f
{
    typedef List_t<typename TypeTuple_f<T0_,T1_,T2_,T3_,T4_,T5_,T6_,T7_>::T> T;
};

inline Tuple_f<>::T tuple ()
{
    return Tuple_f<>::T();
}

template <typename T0_>
typename Tuple_f<T0_>::T tuple (T0_ const &x0)
{
    return typename Tuple_f<T0_>::T(x0);
}

template <typename T0_,
          typename T1_>
typename Tuple_f<T0_,T1_>::T
    tuple (T0_ const &x0,
           T1_ const &x1)
{
    return typename Tuple_f<T0_,T1_>::T(x0, tuple(x1));
}

template <typename T0_,
          typename T1_,
          typename T2_>
typename Tuple_f<T0_,T1_,T2_>::T
    tuple (T0_ const &x0,
           T1_ const &x1,
           T2_ const &x2)
{
    return typename Tuple_f<T0_,T1_,T2_>::T(x0, tuple(x1, x2));
}

template <typename T0_,
          typename T1_,
          typename T2_,
          typename T3_>
typename Tuple_f<T0_,T1_,T2_,T3_>::T
    tuple (T0_ const &x0,
           T1_ const &x1,
           T2_ const &x2,
           T3_ const &x3)
{
    return typename Tuple_f<T0_,T1_,T2_,T3_>::T(x0, tuple(x1, x2, x3));
}

template <typename T0_,
          typename T1_,
          typename T2_,
          typename T3_,
          typename T4_>
typename Tuple_f<T0_,T1_,T2_,T3_,T4_>::T
    tuple (T0_ const &x0,
           T1_ const &x1,
           T2_ const &x2,
           T3_ const &x3,
           T4_ const &x4)
{
    return typename Tuple_f<T0_,T1_,T2_,T3_,T4_>::T(x0, tuple(x1, x2, x3, x4));
}

template <typename T0_,
          typename T1_,
          typename T2_,
          typename T3_,
          typename T4_,
          typename T5_>
typename Tuple_f<T0_,T1_,T2_,T3_,T4_,T5_>::T
    tuple (T0_ const &x0,
           T1_ const &x1,
           T2_ const &x2,
           T3_ const &x3,
           T4_ const &x4,
           T5_ const &x5)
{
    return typename Tuple_f<T0_,T1_,T2_,T3_,T4_,T5_>::T(x0, tuple(x1, x2, x3, x4, x5));
}

template <typename T0_,
          typename T1_,
          typename T2_,
          typename T3_,
          typename T4_,
          typename T5_,
          typename T6_>
typename Tuple_f<T0_,T1_,T2_,T3_,T4_,T5_,T6_>::T
    tuple (T0_ const &x0,
           T1_ const &x1,
           T2_ const &x2,
           T3_ const &x3,
           T4_ const &x4,
           T5_ const &x5,
           T6_ const &x6)
{
    return typename Tuple_f<T0_,T1_,T2_,T3_,T4_,T5_,T6_>::T(x0, tuple(x1, x2, x3, x4, x5, x6));
}

template <typename T0_,
          typename T1_,
          typename T2_,
          typename T3_,
          typename T4_,
          typename T5_,
          typename T6_,
          typename T7_>
typename Tuple_f<T0_,T1_,T2_,T3_,T4_,T5_,T6_,T7_>::T
    tuple (T0_ const &x0,
           T1_ const &x1,
           T2_ const &x2,
           T3_ const &x3,
           T4_ const &x4,
           T5_ const &x5,
           T6_ const &x6,
           T7_ const &x7)
{
    return typename Tuple_f<T0_,T1_,T2_,T3_,T4_,T5_,T6_,T7_>::T(x0, tuple(x1, x2, x3, x4, x5, x6, x7));
}

// sort of hacky way to get limited variadic template behavior
template <typename T_, Uint32 LENGTH_>
struct UniformTuple_f
{
    typedef typename UniformListOfLength_f<T_,LENGTH_>::T T;
};

// identical to tuple()
inline UniformTuple_f<NullType,0>::T uniform_tuple ()
{
    return UniformTuple_f<NullType,0>::T();
}

template <typename T_>
typename UniformTuple_f<T_,1>::T uniform_tuple (T_ const &x0)
{
    return typename UniformTuple_f<T_,1>::T(x0);
}

template <typename T_>
typename UniformTuple_f<T_,2>::T uniform_tuple (T_ const &x0,
                                                T_ const &x1)
{
    return typename UniformTuple_f<T_,2>::T(x0, uniform_tuple<T_>(x1));
}

template <typename T_>
typename UniformTuple_f<T_,3>::T uniform_tuple (T_ const &x0,
                                                T_ const &x1,
                                                T_ const &x2)
{
    return typename UniformTuple_f<T_,3>::T(x0, uniform_tuple<T_>(x1, x2));
}

template <typename T_>
typename UniformTuple_f<T_,4>::T uniform_tuple (T_ const &x0,
                                                T_ const &x1,
                                                T_ const &x2,
                                                T_ const &x3)
{
    return typename UniformTuple_f<T_,4>::T(x0, uniform_tuple<T_>(x1, x2, x3));
}

template <typename T_>
typename UniformTuple_f<T_,5>::T uniform_tuple (T_ const &x0,
                                                T_ const &x1,
                                                T_ const &x2,
                                                T_ const &x3,
                                                T_ const &x4)
{
    return typename UniformTuple_f<T_,5>::T(x0, uniform_tuple<T_>(x1, x2, x3, x4));
}

template <typename T_>
typename UniformTuple_f<T_,6>::T uniform_tuple (T_ const &x0,
                                                T_ const &x1,
                                                T_ const &x2,
                                                T_ const &x3,
                                                T_ const &x4,
                                                T_ const &x5)
{
    return typename UniformTuple_f<T_,6>::T(x0, uniform_tuple<T_>(x1, x2, x3, x4, x5));
}

template <typename T_>
typename UniformTuple_f<T_,7>::T uniform_tuple (T_ const &x0,
                                                T_ const &x1,
                                                T_ const &x2,
                                                T_ const &x3,
                                                T_ const &x4,
                                                T_ const &x5,
                                                T_ const &x6)
{
    return typename UniformTuple_f<T_,7>::T(x0, uniform_tuple<T_>(x1, x2, x3, x4, x5, x6));
}

template <typename T_>
typename UniformTuple_f<T_,8>::T uniform_tuple (T_ const &x0,
                                                T_ const &x1,
                                                T_ const &x2,
                                                T_ const &x3,
                                                T_ const &x4,
                                                T_ const &x5,
                                                T_ const &x6,
                                                T_ const &x7)
{
    return typename UniformTuple_f<T_,8>::T(x0, uniform_tuple<T_>(x1, x2, x3, x4, x5, x6, x7));
}

} // end of namespace Tenh

#endif // TENH_META_TUPLE_HPP_
