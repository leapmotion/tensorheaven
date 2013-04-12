// ///////////////////////////////////////////////////////////////////////////
// tenh/meta/static_assert.hpp by Ted Nitz, created 2013/04/12
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_META_STATIC_ASSERT_HPP_
#define TENH_META_STATIC_ASSERT_HPP_

#define STATIC_ASSERT(CONDITION,MESSAGE) do { if(Tenh::StaticAssert<bool(CONDITION)>::MESSAGE) {} } while (false)


namespace Tenh {

    template <bool condition>
    struct StaticAssert {};
    
    template <>
    struct StaticAssert<true>
    {
        enum
        {
            TEST_MESSAGE
        };
    };


} // end of namespace Tenh

#endif // TENH_META_STATIC_ASSERT_HPP_
