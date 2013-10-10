// ///////////////////////////////////////////////////////////////////////////
// test.cpp by Victor Dods, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test.hpp"

#include "test_abstractindex.hpp"
#include "test_array.hpp"
#include "test_basic_operator.hpp"
#include "test_basic_vector.hpp"
#include "test_dimindex.hpp"
#include "test_expressiontemplate_reindex.hpp"
// #include "test_euclideanembedding.hpp"
// #include "test_euclideanembeddinginverse.hpp"
// #include "test_interop_eigen_euclideanlyembedded.hpp"
// #include "test_interop_eigen_inversion.hpp"
// #include "test_interop_eigen_ldlt.hpp"
#include "test_list.hpp"
#include "test_tuple.hpp"
#include "test_typelist.hpp"
// #include "test_tensor2.hpp"
// #include "test_tensor2diagonal.hpp"
// #include "test_expressiontemplates.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

int main (int argc, char **argv, char **envp)
{
    Directory root;

    Test::AbstractIndex::AddTests(&root);
    Test::Array::AddTests(&root);

    {
        Directory *basic_dir = new Directory("basic", &root);
        Test::Basic::Operator::AddTests(basic_dir);
        Test::Basic::Vector::AddTests(basic_dir);
    }

    Test::DimIndex::AddTests(&root);
    Test::ExpressionTemplate_Reindex::AddTests(&root);
//     Test::EigenLDLT::AddTests(&root);
//     Test::EuclideanEmbedding::AddTests(&root);
//     Test::EuclideanEmbeddingInverse::AddTests(&root);
//     Test::ExpressionTemplates::AddTests(&root);
//     {
//         Directory *interop_eigen = new Directory("interop_eigen", &root);
//         Test::InteropEigen::EuclideanlyEmbedded::AddTests(interop_eigen);
//         Test::InteropEigen::Inversion::AddTests(interop_eigen);
//     }
    Test::List::AddTests(&root);
    Test::Tuple::AddTests(&root);
    Test::TypeList::AddTests(&root);
//     Test::Tensor2::AddTests(&root);
//     Test::Tensor2Diagonal::AddTests(&root);

    int failure_count = RunScheduled(argc, argv, envp, root);

    return (failure_count > 0) ? 1 : 0;
}
