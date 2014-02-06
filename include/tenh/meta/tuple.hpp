// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/tuple.hpp by Victor Dods, created 2013/02/16
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_META_TUPLE_HPP_
#define TENH_META_TUPLE_HPP_

#include "tenh/core.hpp"

#include "tenh/list.hpp"

namespace Tenh {

// this will all go away once List_t is renamed to Tuple_t

template <typename... Types_>
struct Tuple_f
{
    typedef Tuple_t<Typle_t<Types_...>> T;
};

inline Tuple_f<>::T tuple ()
{
    return Tuple_f<>::T();
}

template <typename Head_, typename... BodyTypes_>
inline typename Tuple_f<Head_,BodyTypes_...>::T tuple (Head_ const &head, BodyTypes_... body)
{
    return typename Tuple_f<Head_,BodyTypes_...>::T(head, body...);
}

// sort of hacky way to get limited variadic template behavior
template <typename T_, Uint32 LENGTH_>
struct UniformTuple_f
{
    typedef typename UniformTupleOfLength_f<T_,LENGTH_>::T T;
};

// identical to tuple()
template <typename UniformType_>
typename UniformTuple_f<UniformType_,0>::T uniform_tuple ()
{
    return typename UniformTuple_f<UniformType_,0>::T();
}

// each parameter must have a coercion to UniformType_
template <typename UniformType_, typename Head_, typename... BodyTypes_>
typename UniformTuple_f<UniformType_,Length_f<Typle_t<Head_,BodyTypes_...>>::V>::T
    uniform_tuple (Head_ const &head, BodyTypes_... body)
{
    return typename UniformTuple_f<UniformType_,Length_f<Typle_t<Head_,BodyTypes_...>>::V>::T(head, body...);
}

} // end of namespace Tenh

#endif // TENH_META_TUPLE_HPP_
