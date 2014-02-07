// ///////////////////////////////////////////////////////////////////////////
// test_list.hpp by Victor Dods, created 2013/04/03
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(TEST_LIST_HPP_)
#define TEST_LIST_HPP_

#include "test.hpp"

namespace Lvd {
namespace TestSystem {

struct Directory;

} // end of namespace TestSystem
} // end of namespace Lvd

namespace Test {
namespace Tuple {

void AddTests (Lvd::TestSystem::Directory &parent);

} // end of namespace Tuple
} // end of namespace Test

#endif // !defined(TEST_LIST_HPP_)
