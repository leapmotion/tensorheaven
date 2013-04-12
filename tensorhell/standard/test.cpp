// ///////////////////////////////////////////////////////////////////////////
// test.cpp by Victor Dods, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "tenh/meta/static_assert.hpp"
#include "test.hpp"
#include "test_array.hpp"
#include "test_list.hpp"
#include "test_typelist.hpp"
#include "test_vector.hpp"
#include "test_tensor2.hpp"
#include "test_tensor2diagonal.hpp"
#include "test_expressiontemplates.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

int main (int argc, char **argv, char **envp)
{
    
    STATIC_ASSERT(false, TEST_MESSAGE);
    
    Directory root;

    Test::Array::AddTests(&root);
    Test::List::AddTests(&root);
    Test::TypeList::AddTests(&root);
    Test::Vector::AddTests(&root);
    Test::Tensor2::AddTests(&root);
    Test::Tensor2Diagonal::AddTests(&root);
    Test::ExpressionTemplates::AddTests(&root);

    int failure_count = RunScheduled(argc, argv, envp, root);

    return (failure_count > 0) ? 1 : 0;
}
