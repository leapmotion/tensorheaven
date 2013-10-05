// ///////////////////////////////////////////////////////////////////////////
// test_operator.hpp by Victor Dods, created 2013/10/04
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(TEST_OPERATOR_HPP_)
#define TEST_OPERATOR_HPP_

#include "test.hpp"

namespace Lvd {
namespace TestSystem {

struct Directory;

} // end of namespace TestSystem
} // end of namespace Lvd

namespace Test {
namespace Operator {

void AddTests (Lvd::TestSystem::Directory *parent);

} // end of namespace Operator
} // end of namespace Test

#endif // !defined(TEST_OPERATOR_HPP_)
