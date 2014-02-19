// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/id.hpp by Victor Dods, created 2013/07/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_DUAL_HPP_
#define TENH_CONCEPTUAL_DUAL_HPP_

#include "tenh/core.hpp"

#include "tenh/meta/typestringof.hpp"
#include "tenh/meta/typle.hpp"

namespace Tenh {

// formal dual of something
template <typename Primal_>
struct Dual_c
{
	typedef Primal_ Primal; // this probably doesn't really need to be here
	typedef Primal Dual;

    static std::string type_as_string ()
    {
        return "Dual_c<" + type_string_of<Primal_>() + '>';
    }
};

// for taking the dual of a formal symbol (even for things like a float), and it is assumed to be reflexive (self-double-dual)
template <typename Primal>
struct DualOf_f
{
	typedef Dual_c<Primal> T;
private:
    DualOf_f();
};

// here is where the self-double-dualness is implemented (as a template specialization)
template <typename Primal>
struct DualOf_f<Dual_c<Primal>>
{
	typedef Primal T;
private:
    DualOf_f();
};

// template specialization to take dual of each element in a Typle_t 
// -- essentially making the DualOf_f and Typle_t functors commute.
template <typename Head_, typename... BodyTypes_>
struct DualOf_f<Typle_t<Head_,BodyTypes_...>>
{
    typedef typename HeadBodyTyple_f<typename DualOf_f<Head_>::T,typename DualOf_f<Typle_t<BodyTypes_...>>::T>::T T;
private:
    DualOf_f();
};

template <>
struct DualOf_f<Typle_t<>>
{
    typedef Typle_t<> T;
private:
    DualOf_f();
};

/// @brief A type for use as a generic identifier, e.g. in a Basis_c.
/// @headerfile core.hpp "tenh/core.hpp"
struct SelfDualGeneric { static std::string type_as_string () { return "SelfDualGeneric"; } };

// template specialization to make the SelfDualGeneric type self-dual.  Note that
// Generic is NOT self-dual.
template <>
struct DualOf_f<SelfDualGeneric>
{
    typedef SelfDualGeneric T;
private:
    DualOf_f();
};

// I guess NullType should be self dual...
template <>
struct DualOf_f<NullType>
{
    typedef NullType T;
private:
    DualOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// helper functions for easing use of the type system
// ///////////////////////////////////////////////////////////////////////////

template <typename Primal_>
typename DualOf_f<Primal_>::T dual (Primal_ const &)
{
    return typename DualOf_f<Primal_>::T();
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_DUAL_HPP_

