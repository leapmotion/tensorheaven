// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/basis.hpp by Victor Dods, created 2013/07/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_BASIS_HPP_
#define TENH_CONCEPTUAL_BASIS_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/dual.hpp"
#include "tenh/meta/static_assert.hpp"

namespace Tenh {

template <typename Id_>
struct Basis_c
{
	typedef Id_ Id;
	typedef typename DualOf_c<Basis_c>::T Dual; // relies on the template specialization below
	static bool const IS_BASIS = true;

    static std::string type_as_string ()
    {
        return "Basis_c<" + TypeStringOf_t<Id>::eval() + '>';
    }
};

template <typename Id>
struct DualOf_c<Basis_c<Id> >
{
    typedef Basis_c<typename DualOf_c<Id>::T> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_BASIS_HPP_
