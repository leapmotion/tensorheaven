#ifndef CORE_HPP_
#define CORE_HPP_

#include "lvd.hpp"

#define FORMAT_VALUE(expr) #expr << " = " << (expr)

typedef Lvd::Uint32 Uint32;

// for use when you want to return a const reference to a zero
template <typename Scalar>
struct Static
{
    static Scalar const ZERO;
};

template <typename Scalar>
Scalar const Static<Scalar>::ZERO(0);

struct WithoutInitialization { };

extern WithoutInitialization const WITHOUT_INITIALIZATION; // you must define it in your program -- use default constructor

#endif // CORE_HPP_
