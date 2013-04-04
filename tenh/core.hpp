// ///////////////////////////////////////////////////////////////////////////
// tenh/core.hpp by Victor Dods, created 2013/03/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CORE_HPP_
#define TENH_CORE_HPP_

#include "tenh/meta/lvd.hpp"

#define FORMAT_VALUE(expr) #expr << " = " << (expr) // TODO: move this out into test code

namespace Tenh {

typedef Lvd::Uint32 Uint32;

// shouldn't ever actually construct one of these
struct NullType
{
private:

    NullType () { }

public:

    static std::string type_as_string () { return "NullType"; }
};

// for use when you want to return a const reference to a zero
template <typename Scalar>
struct StaticScalar
{
    static Scalar const ZERO;
};

template <typename Scalar>
Scalar const StaticScalar<Scalar>::ZERO(0);

// for making the construction of something without initialization explicit and ugly
struct WithoutInitialization { };

// this class is just to contain a preexisting instance of WithoutInitialization that you can use without constructing a new one.
template <typename DummyType = NullType> // the template argument is just so there's no need for a cpp file definition of the static member
struct Static
{
    static WithoutInitialization const WITHOUT_INITIALIZATION;
};

template <typename DummyType>
WithoutInitialization const Static<DummyType>::WITHOUT_INITIALIZATION = WithoutInitialization();

template <typename T>
struct Static_t
{
    static T SINGLETON;
};

template <typename T>
T Static_t<T>::SINGLETON;

// convenient notation for the square of something without evaluating the expression twice.
template <typename T>
T sqr (T const &t)
{
    return t*t;
}

} // end of namespace Tenh

#endif // TENH_CORE_HPP_
