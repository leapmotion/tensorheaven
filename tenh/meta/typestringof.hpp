// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typestringof.hpp by Victor Dods, created 2013/02/16
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////
/// @file typestringof.hpp
/// @headerfile typestringof.hpp "tenh/meta/typestringof.hpp"
/// @brief Contains runtime and compile time functions for generating strings associated
/// with type names, and for making pretty versions of the same.
#ifndef TENH_META_TYPESTRINGOF_HPP_
#define TENH_META_TYPESTRINGOF_HPP_

#include <complex>
#include <ostream>
#include <string>

#include "tenh/core.hpp"

namespace Tenh {

/// @def AS_STRING(X)
/// @brief Converts built-in types to `std::string` for ease of building type strings.
#define AS_STRING(x) static_cast<std::ostringstream &>(std::ostringstream().flush() << x).str()


/// @struct TypeStringOf_t typestringof.hpp "tenh/meta/typestringof.hpp"
/// @brief Template Meta Function to obtain a string from a typename.
/// @details TypeStringOf_t requires that any user-defined types that it may be passed have a static member
/// function `type_as_string()` which returns a `std::string` containing the name of that type.
/// @tparam T The type to generate a string for.

// by default we call the type's type_as_string static member.
template <typename T> struct TypeStringOf_t { static std::string eval () { return T::type_as_string(); } };

/// @cond false
// template overloads to accomodate built in types and types in the std namespace.
template <> struct TypeStringOf_t<Sint8> { static std::string eval () { return "Sint8"; } };
template <> struct TypeStringOf_t<Uint8> { static std::string eval () { return "Uint8"; } };
template <> struct TypeStringOf_t<Sint16> { static std::string eval () { return "Sint16"; } };
template <> struct TypeStringOf_t<Uint16> { static std::string eval () { return "Uint16"; } };
template <> struct TypeStringOf_t<Sint32> { static std::string eval () { return "Sint32"; } };
template <> struct TypeStringOf_t<Uint32> { static std::string eval () { return "Uint32"; } };
template <> struct TypeStringOf_t<Sint64> { static std::string eval () { return "Sint64"; } };
template <> struct TypeStringOf_t<Uint64> { static std::string eval () { return "Uint64"; } };
template <> struct TypeStringOf_t<bool> { static std::string eval () { return "bool"; } };
template <> struct TypeStringOf_t<float> { static std::string eval () { return "float"; } };
template <> struct TypeStringOf_t<double> { static std::string eval () { return "double"; } };
template <> struct TypeStringOf_t<long double> { static std::string eval () { return "long double"; } };
template <> struct TypeStringOf_t<std::string> { static std::string eval () { return "std::string"; } };
template <typename T>
struct TypeStringOf_t<std::complex<T> >
{
    static std::string eval () { return "complex<" + TypeStringOf_t<T>::eval() + '>'; }
};
template <typename T_, T_ VALUE_>
struct TypeStringOf_t<Value_t<T_,VALUE_> >
{
    static std::string eval () { return "Value_t<" + TypeStringOf_t<T_>::eval() + ',' + AS_STRING(VALUE_) + '>'; }
};
template <> struct TypeStringOf_t<NullValue> { static std::string eval () { return "NullValue"; } };
/// @endcond

/// @brief Overload for operator<< to stream objects of type TypeStringOf_t
template <typename T_>
std::ostream &operator << (std::ostream &out, TypeStringOf_t<T_> const &)
{
    return out << TypeStringOf_t<T_>::eval();
}

/// @brief Returns the type string of the given type instance.
template <typename T_>
std::string type_string_of (T_ const &)
{
    return TypeStringOf_t<T_>::eval();
}

/// @brief Returns the type string of the given explicitly specified type T_.
/// @tparam T_ The type for which to generate the type string.
template <typename T_>
std::string type_string_of ()
{
    return TypeStringOf_t<T_>::eval();
}

/// @struct Pretty typestringof.hpp "tenh/meta/typestringof.hpp"
/// @brief Used as a tag for "pretty" printing of type strings.
/// @details Exists in order to allow an overload of operator<< for TypeStringOf_t which uses print_pretty_typestring
///  with a non-default value for shortify_depth.
/// @tparam TypeStringOf_ An instance of the TypeStringOf_t template which we wish to print "prettified"
/// @tparam SHORTIFY_DEPTH How deep should template parameters be allowed to go
template <typename TypeStringOf_, Uint32 SHORTIFY_DEPTH_ = 0>
struct Pretty
{
    /** @brief Acessor typedef for template parameter */
    typedef TypeStringOf_ TypeStringOf;
    /** @brief Acessor for template parameter */
    static Uint32 const SHORTIFY_DEPTH = SHORTIFY_DEPTH_;
};


/// @brief Output a "prettified" version of a typestring.
/// @details This function goes through a typestring and indents the template parameters as one might
///          indent blocks of code, and does folding of template parameters deeper than the `shortify_depth`
///          parameter.
/// @param out is the ostream to output to
/// @param typestring is the typestring to prettify and output
/// @param indent_level gives the starting indentation level
/// @param indent_size is the number of spaces to use for indenting.
/// @param shortify_depth is the depth at which "template arguments" are replaced with an ellipsis;
///         a shortify_depth of 0 means that no ellipsis-collapsing will be done, and e.g. a
///         shortify_depth of 1 will produce something like FooBar<...>.
void print_pretty_typestring (std::ostream &out,
                              std::string const &typestring,
                              Uint32 indent_level = 0,
                              Uint32 indent_size = 4,
                              Uint32 shortify_depth = 0)
{
    Uint32 template_argument_depth = 0;
    for (std::string::const_iterator it = typestring.begin(), it_end = typestring.end(); it != it_end; ++it)
    {
        char const &c = *it;
        switch (c)
        {
            case '<':
                ++template_argument_depth;
                // we're shortified, so print the shortified "template arguments"
                if (shortify_depth > 0 && template_argument_depth == shortify_depth)
                {
                    out << "<...>";
                }
                else if (shortify_depth == 0 || template_argument_depth < shortify_depth)
                {
                    ++indent_level;
                    out << "<\n" << std::string(indent_level*indent_size, ' ');
                }
                break;

            case ',':
                // only print stuff if we're not shortified
                if (shortify_depth == 0 || template_argument_depth < shortify_depth)
                {
                    out << ",\n" << std::string(indent_level*indent_size, ' ');
                }
                break;

            case '>':
                // presumably the < and > brackets will be balanced, but in case they're not...
                if (indent_level > 0)
                    --indent_level;
                // only print stuff if we're not shortified
                if (shortify_depth == 0 || template_argument_depth < shortify_depth)
                {
                    out << '\n' << std::string(indent_level*indent_size, ' ') << '>';
                }
                if (template_argument_depth)
                    --template_argument_depth;
                break;

            default:
                // only print stuff if we're not shortified
                if (shortify_depth == 0 || template_argument_depth < shortify_depth)
                    out << c;
                break;
        }
    }
}

/// @brief Overload for operator<< to stream objects of type Pretty
template <typename T_, Uint32 SHORTIFY_DEPTH_>
std::ostream &operator << (std::ostream &out, Pretty<TypeStringOf_t<T_>,SHORTIFY_DEPTH_> const &)
{
    print_pretty_typestring(out, TypeStringOf_t<T_>::eval(), 0, 4, SHORTIFY_DEPTH_);
    return out;
}

} // end of namespace Tenh

#endif // TENH_META_TYPESTRINGOF_HPP_
