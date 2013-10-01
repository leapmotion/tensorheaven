// ///////////////////////////////////////////////////////////////////////////
// tenh/core.hpp by Victor Dods, created 2013/03/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CORE_HPP_
#define TENH_CORE_HPP_

#if _WIN32
#pragma warning( disable : 4503 )
#endif

#include <cassert>
#include <complex>

#include "tenh/meta/core.hpp"
#include "tenh/meta/static_assert.hpp"

#define FORMAT(expr) static_cast<std::ostringstream &>(std::ostringstream().flush() << expr).str()
#define FORMAT_VALUE(expr) #expr << " = " << (expr) // TODO: move this out into test code

namespace Tenh {

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

// these are used in constructors for determining if a pointer check should be done.
// the default check parameter value should be CHECK_POINTER, which is more expensive,
// but if you know what you're doing, you can pass in DONT_CHECK_POINTER to avoid the
// pointer check and gain efficiency (e.g. if you know for a fact that the pointer is
// non-NULL).  this is a compromise between completely correct program behavior and
// program efficiency.
// TODO: should these be moved to preallocatedarray.hpp? (if they're only used there)
static bool const CHECK_POINTER = true;
static bool const DONT_CHECK_POINTER = false;

// used in the curiously recurring template pattern, where the derived type is passed
// to parent classes as a template parameter, so that the baseclass can access the
// derived type's methods.  will "return" (as a typedef for T) Derived if Derived
// is not NullType, otherwise DefaultType.
template <typename Derived, typename DefaultType>
struct DerivedType_t
{
    typedef typename If<TypesAreEqual_f<Derived,NullType>::V,
                        DefaultType,
                        Derived>::T T;
};

// provide an implementation of this for any custom scalar type (e.g. an arbitrary-precision
// floating point type).
template <typename Scalar>
struct AssociatedFloatingPointType_t;

template <>
struct AssociatedFloatingPointType_t<float> { typedef float T; };

template <>
struct AssociatedFloatingPointType_t<double> { typedef double T; };

template <>
struct AssociatedFloatingPointType_t<long double> { typedef long double T; };

template <typename RealScalar>
struct AssociatedFloatingPointType_t<std::complex<RealScalar> > { typedef RealScalar T; };

template <>
struct AssociatedFloatingPointType_t<Sint8> { typedef float T; }; // smallest lossless floating point conversion

template <>
struct AssociatedFloatingPointType_t<Uint8> { typedef float T; }; // smallest lossless floating point conversion

template <>
struct AssociatedFloatingPointType_t<Sint16> { typedef float T; }; // smallest lossless floating point conversion

template <>
struct AssociatedFloatingPointType_t<Uint16> { typedef float T; }; // smallest lossless floating point conversion

template <>
struct AssociatedFloatingPointType_t<Sint32> { typedef double T; }; // smallest lossless floating point conversion

template <>
struct AssociatedFloatingPointType_t<Uint32> { typedef double T; }; // smallest lossless floating point conversion

template <>
struct AssociatedFloatingPointType_t<Sint64> { typedef long double T; }; // smallest lossless floating point conversion

template <>
struct AssociatedFloatingPointType_t<Uint64> { typedef long double T; }; // smallest lossless floating point conversion

} // end of namespace Tenh

#endif // TENH_CORE_HPP_
