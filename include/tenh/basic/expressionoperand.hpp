// ///////////////////////////////////////////////////////////////////////////
// tenh/basic/expressionoperand.hpp by Victor Dods, created 2013/10/11
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_BASIC_EXPRESSIONOPERAND_HPP_
#define TENH_BASIC_EXPRESSIONOPERAND_HPP_

#include "tenh/core.hpp"

namespace Tenh {

// interface for everything in basic mode that has operator overloads
template <typename Derived_, Uint32 FREE_INDEX_COUNT_>
struct ExpressionOperand_i
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM((!TypesAreEqual_f<Derived_,NullType>::V), DERIVED_MUST_NOT_BE_NULL_TYPE),
        STATIC_ASSERT_IN_ENUM(FREE_INDEX_COUNT_ <= 2, FREE_INDEX_COUNT_MUST_BE_AT_MOST_2)
    };
public:

    typedef Derived_ Derived;
    static Uint32 const FREE_INDEX_COUNT = FREE_INDEX_COUNT_;

    // for accessing this as the Derived type
    Derived const &as_derived () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived () { return *static_cast<Derived *>(this); }
};

} // end of namespace Tenh

#endif // TENH_BASIC_EXPRESSIONOPERAND_HPP_
