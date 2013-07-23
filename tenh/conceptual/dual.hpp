// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/id.hpp by Victor Dods, created 2013/07/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_DUAL_HPP_
#define TENH_CONCEPTUAL_DUAL_HPP_

#include "tenh/core.hpp"

#include "tenh/meta/typestringof.hpp"

namespace Tenh {

template <typename Primal_>
struct Dual_c
{
	typedef Primal_ Primal;

    static std::string type_as_string ()
    {
        return "Dual_c<" + TypeStringOf_t<Primal>::eval() + '>';
    }
};

// for taking the dual of a formal symbol (even for things like a float), and it is assumed to be reflexive (self-dual)
template <typename Primal>
struct DualOf_c
{
	typedef Dual_c<Primal> T;
};

// here is where the self-dualness is implemented (as a template specialization)
template <typename Primal>
struct DualOf_c<Dual_c<Primal> >
{
	typedef Primal T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_DUAL_HPP_

