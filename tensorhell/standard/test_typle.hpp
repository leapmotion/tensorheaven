// ///////////////////////////////////////////////////////////////////////////
// test_typle.hpp by Victor Dods, created 2014/01/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(TEST_TYPLE_HPP_)
#define TEST_TYPLE_HPP_

#include "test.hpp"

namespace Lvd {
namespace TestSystem {

struct Directory;

} // end of namespace TestSystem
} // end of namespace Lvd

namespace Test {
namespace Typle {

void AddTests (Lvd::TestSystem::Directory &parent);

} // end of namespace TypeList
} // end of namespace Test

#endif // !defined(TEST_TYPLE_HPP_)
