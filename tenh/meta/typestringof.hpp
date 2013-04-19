// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typestringof.hpp by Victor Dods, created 2013/02/16
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_META_TYPESTRINGOF_HPP_
#define TENH_META_TYPESTRINGOF_HPP_

#include <string>
// #include <sstream> // include this only if AS_STRING is used
#include <complex>

#include "tenh/core.hpp"

namespace Tenh {

#define AS_STRING(x) static_cast<std::ostringstream &>(std::ostringstream().flush() << x).str()

template <typename T> struct TypeStringOf_t { static std::string eval () { return T::type_as_string(); } }; // default

template <typename T> struct TypeStringOf_t<std::complex<T> > { static std::string eval ()
    { return "complex<" + TypeStringOf_t<T>::eval() + ">"; } };

template <> struct TypeStringOf_t<Lvd::Sint8> { static std::string eval () { return "Sint8"; } };
template <> struct TypeStringOf_t<Lvd::Uint8> { static std::string eval () { return "Uint8"; } };
template <> struct TypeStringOf_t<Lvd::Sint16> { static std::string eval () { return "Sint16"; } };
template <> struct TypeStringOf_t<Lvd::Uint16> { static std::string eval () { return "Uint16"; } };
template <> struct TypeStringOf_t<Lvd::Sint32> { static std::string eval () { return "Sint32"; } };
template <> struct TypeStringOf_t<Lvd::Uint32> { static std::string eval () { return "Uint32"; } };
template <> struct TypeStringOf_t<Lvd::Sint64> { static std::string eval () { return "Sint64"; } };
template <> struct TypeStringOf_t<Lvd::Uint64> { static std::string eval () { return "Uint64"; } };
template <> struct TypeStringOf_t<bool> { static std::string eval () { return "bool"; } };
template <> struct TypeStringOf_t<float> { static std::string eval () { return "float"; } };
template <> struct TypeStringOf_t<double> { static std::string eval () { return "double"; } };
template <> struct TypeStringOf_t<long double> { static std::string eval () { return "long double"; } };
template <> struct TypeStringOf_t<std::string> { static std::string eval () { return "std::string"; } };

} // end of namespace Tenh

#endif // TENH_META_TYPESTRINGOF_HPP_
