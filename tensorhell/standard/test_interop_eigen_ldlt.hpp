// ///////////////////////////////////////////////////////////////////////////
// test_eigen_ldlt.hpp by Ted Nitz, created 2013/04/23
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(TEST_EIGENLDLT_HPP_)
#define TEST_EIGENLDLT_HPP_

#include "test.hpp"

namespace Lvd {
namespace TestSystem {

struct Directory;

} // end of namespace TestSystem
} // end of namespace Lvd

namespace Test {
namespace EigenLDLT {

void AddTests (Lvd::TestSystem::Directory *parent);

} // end of namespace EigenLDLT
} // end of namespace Test

#endif // !defined(TEST_EIGENLDLT_HPP_)