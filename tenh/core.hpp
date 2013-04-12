// ///////////////////////////////////////////////////////////////////////////
// tenh/core.hpp by Victor Dods, created 2013/03/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CORE_HPP_
#define TENH_CORE_HPP_

#include <cassert>

#include "tenh/meta/lvd.hpp"
#include "tenh/meta/static_assert.hpp"

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

// for making the construction of something without initialization explicit and ugly
struct WithoutInitialization { };

// for use when you want to return a const reference to various constants.
// the default implementation is to provide a default-constructed static
// const SINGLETON of the given value.
//
// you can template-specialize this to provide particular customizations,
// for example, Static<float> and Static<double> provide static constant
// scalar values ZERO, ONE, and so forth.
template <typename T>
struct Static
{
    static T const SINGLETON;
};

template <typename T>
T const Static<T>::SINGLETON = T();

// convenient notation for the square of something without evaluating the expression twice.
template <typename T>
T sqr (T const &t)
{
    return t*t;
}

// these are used in constructors for determining if a range check should be done.
// the default check parameter value should be CHECK_RANGE, which is more expensive,
// but if you know what you're doing, you can pass in DONT_CHECK_RANGE to avoid the
// range check and gain efficiency (e.g. if you know for a fact that the value is
// within the correct range).  this is a compromise between completely correct program
// behavior and program efficiency.
static bool const CHECK_RANGE = true;
static bool const DONT_CHECK_RANGE = false;

} // end of namespace Tenh

#endif // TENH_CORE_HPP_
