// ///////////////////////////////////////////////////////////////////////////
// tenh/componentindex.hpp by Victor Dods, created 2013/08/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////
/// @file componentindex.hpp
/// @headerfile componentindex.hpp "tenh/componentindex.hpp"
/// @brief Contains code for dealing with component indices.
#ifndef TENH_COMPONENTINDEX_HPP_
#define TENH_COMPONENTINDEX_HPP_

#include <stdexcept>

#include "tenh/core.hpp"

#include "tenh/meta/typestringof.hpp"

namespace Tenh {

/// @struct ComponentIndex_t componentindex.hpp "tenh/componentindex.hpp"
/// @brief for use in operator [] for evaluation of array components for example Array_i.
/// @tparam COMPONENT_COUNT_ the number of components.
template <Uint32 COMPONENT_COUNT_>
struct ComponentIndex_t
{
    /// Accessor for the template parameter.
    static Uint32 const COMPONENT_COUNT = COMPONENT_COUNT_;

// this to allow 0-component component-indexing to work (necessary for 0-dimensional vectors)
#ifdef __clang_version__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtautological-compare"
#endif // __clang_version__

    /// Default constructor initializes to start of iteration.
    ComponentIndex_t () : m(0) { }

    /// @brief Construct a CompnentIndex_t with initilization.
    /// @details The default parameter for check_range will cause a range check (and possible
    ///  exception throw) for safety. If DONT_RANGE_CHECK is explicitly provided (an
    ///  intentional and ugly bit of code), the range check can be avoided for efficiency
    ///  when you know for a fact that your value is within the correct range.
    explicit ComponentIndex_t (Uint32 i, bool check_range = CHECK_RANGE)
        :
        m(i)
    {
        if (check_range && m >= COMPONENT_COUNT)
            throw std::out_of_range("Raw-integer argument to ComponentIndex_t<...> constructor was out of range.");
    }

    /// Copy constructor, no range-checking necessary.
    ComponentIndex_t (ComponentIndex_t const &i) : m(i.m) { }

    bool operator == (ComponentIndex_t const &i) const { return m == i.m; }
    bool operator != (ComponentIndex_t const &i) const { return m != i.m; }
    bool operator < (ComponentIndex_t const &i) const { return m < i.m; }

    bool is_at_end () const { return m >= COMPONENT_COUNT; }
    bool is_not_at_end () const { return m < COMPONENT_COUNT; }
    Uint32 value () const { return m; }
    void operator ++ () { ++m; }
    void reset () { m = 0; }

#ifdef __clang_version__
#pragma GCC diagnostic pop
#endif // __clang_version__

    /// For use with TypeStringOf_t.
    static std::string type_as_string () { return "ComponentIndex_t<" + FORMAT(COMPONENT_COUNT) + '>'; }

private:

    ComponentIndex_t (WithoutInitialization const &) { }

    // TODO: store m as Uint16, and keep "is at end" flag as some separate bool or Uint16 (?)
    // this will allow value() to always return valid index integers, and the system can be safer
    // NOTE: 0-dimensional arrays/vector spaces will prevent this from working.
    Uint32 m;
};

/// @struct IsComponentIndex_f componentindex.hpp "tenh/componentindex.hpp"
/// @brief Simple function to determine if a type is a ComponentIndex_t.
/// @tparam T the type to test.
template <typename T> struct IsComponentIndex_f { static bool const V = false; };
/// @cond false
template <Uint32 COMPONENT_COUNT> struct IsComponentIndex_f<ComponentIndex_t<COMPONENT_COUNT> >
{
    static bool const V = true;
private:
    IsComponentIndex_f();
};
/// @endcond

/// @struct IsComponentIndex_p componentindex.hpp "tenh/componentindex.hpp"
/// @brief Predicate version of IsComponentIndex_f.
struct IsComponentIndex_p
{
    /// @cond false
    template <typename T>
    struct Eval_t
    {
        static bool const V = IsComponentIndex_f<T>::V;
    };
private:
    IsComponentIndex_p();
    /// @endcond
};

/// Operator overload for outputting a ComponentIndex_t with an ostream.
template <Uint32 COMPONENT_COUNT>
std::ostream &operator << (std::ostream &out, ComponentIndex_t<COMPONENT_COUNT> const &c)
{
    return out << c.value();
}

} // end of namespace Tenh

#endif // TENH_COMPONENTINDEX_HPP_
