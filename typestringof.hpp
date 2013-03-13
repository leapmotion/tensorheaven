#ifndef TYPESTRINGOF_HPP_
#define TYPESTRINGOF_HPP_

#include <string>

#include "lvd.hpp"

template <typename T> struct TypeStringOf_t { static std::string eval () { return T::type_as_string(); } }; // default

template <> struct TypeStringOf_t<Lvd::Sint8> { static std::string eval () { return "Sint8"; } };
template <> struct TypeStringOf_t<Lvd::Uint8> { static std::string eval () { return "Uint8"; } };
template <> struct TypeStringOf_t<Lvd::Sint16> { static std::string eval () { return "Sint16"; } };
template <> struct TypeStringOf_t<Lvd::Uint16> { static std::string eval () { return "Uint16"; } };
template <> struct TypeStringOf_t<Lvd::Sint32> { static std::string eval () { return "Sint32"; } };
template <> struct TypeStringOf_t<Lvd::Uint32> { static std::string eval () { return "Uint32"; } };
template <> struct TypeStringOf_t<bool> { static std::string eval () { return "bool"; } };
template <> struct TypeStringOf_t<float> { static std::string eval () { return "float"; } };
template <> struct TypeStringOf_t<double> { static std::string eval () { return "double"; } };
template <> struct TypeStringOf_t<std::string> { static std::string eval () { return "std::string"; } };

#endif // TYPESTRINGOF_HPP_
