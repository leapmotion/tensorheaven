// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/typestringof.hpp by Victor Dods, created 2013/02/16
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_META_TYPESTRINGOF_HPP_
#define TENH_META_TYPESTRINGOF_HPP_

#include <complex>
#include <ostream>
#include <string>

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

// used as a tag for "pretty" printing of type strings
template <typename TypeStringOf_, Uint32 SHORTIFY_DEPTH_ = 0>
struct Pretty
{
    typedef TypeStringOf_ TypeStringOf;
    static Uint32 const SHORTIFY_DEPTH = SHORTIFY_DEPTH_;
};

// shortify_depth is the depth at which "template arguments" are replaced with an ellipsis;
// a shortify_depth of 0 means that no ellipsis-collapsing will be done, and e.g. a
// shortify_depth of 1 will produce something like FooBar<...>.
void print_pretty_typestring (std::ostream &out,
                              std::string const &typestring,
                              Uint32 indent_level,
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
                else
                {
                    out << "<\n";
                    ++indent_level;
                    for (Uint32 i = 0; i < indent_level; ++i)
                        out << std::string(indent_size, ' ');
                }
                break;

            case ',':
                // only print stuff if we're not shortified
                if (shortify_depth == 0 || template_argument_depth < shortify_depth)
                {
                    out << ",\n";
                    for (Uint32 i = 0; i < indent_level; ++i)
                        out << std::string(indent_size, ' ');
                }
                break;

            case '>':
                // presumably the < and > brackets will be balanced, but in case they're not...
                if (indent_level > 0)
                    --indent_level;
                // only print stuff if we're not shortified
                if (shortify_depth == 0 || template_argument_depth < shortify_depth)
                {
                    out << '\n';
                    for (Uint32 i = 0; i < indent_level; ++i)
                        out << std::string(indent_size, ' ');
                    out << '>';
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

// does nicely indented formatting
template <typename T_, Uint32 SHORTIFY_DEPTH_>
std::ostream &operator << (std::ostream &out, Pretty<TypeStringOf_t<T_>,SHORTIFY_DEPTH_> const &)
{
    print_pretty_typestring(out, TypeStringOf_t<T_>::eval(), 0, 4, SHORTIFY_DEPTH_);
    return out;
}

} // end of namespace Tenh

#endif // TENH_META_TYPESTRINGOF_HPP_
