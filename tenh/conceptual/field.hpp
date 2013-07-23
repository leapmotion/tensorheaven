// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/field.hpp by Victor Dods, created 2013/07/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_FIELD_HPP_
#define TENH_CONCEPTUAL_FIELD_HPP_

#include "tenh/core.hpp"

namespace Tenh {

template <typename Id_>
struct Field_c
{
	typedef Id_ Id;
	static bool const IS_FIELD = true;

    static std::string type_as_string ()
    {
        return "Field_c<" + TypeStringOf_t<Id>::eval() + '>';
    }
};

// TODO: some way of specifying associated representation (e.g. double, complex<double>, bool)

struct Real_c
{
    static std::string type_as_string ()
    {
        return "Real_c";
    }
};

typedef Field_c<Real_c> RealField;

// TODO: Complex_c, F2_c, etc.
// TODO: maybe quaternion (skew-field)?

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_FIELD_HPP_
