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
#include "test_linearembedding.hpp"
#include "test_list.hpp"
#include "test_split_and_bundle.hpp"
#include "test_typle.hpp"
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

    Test::AbstractIndex::AddTests(root);
    Test::Array::AddTests(root);

    {
        Directory &basic_dir = root.GetSubDirectory("basic");
        Test::Basic::Operator::AddTests(basic_dir);
        Test::Basic::Vector::AddTests0(basic_dir);
        Test::Basic::Vector::AddTests1(basic_dir);
        Test::Basic::Vector::AddTests2(basic_dir);
        Test::Basic::Vector::AddTests3(basic_dir);
        Test::Basic::Vector::AddTests4(basic_dir);
        Test::Basic::Vector::AddTests5(basic_dir);
    }

    Test::DimIndex::AddTests(root);
    Test::ExpressionTemplate_Reindex::AddTests(root);
//     Test::EigenLDLT::AddTests(root);
//     Test::EuclideanEmbedding::AddTests(root);
//     Test::EuclideanEmbeddingInverse::AddTests(root);
//     Test::ExpressionTemplates::AddTests(root);
//     {
//         Directory &interop_eigen_dir = root.GetSubDirectory("interop_eigen");
//         Test::InteropEigen::EuclideanlyEmbedded::AddTests(interop_eigen_dir);
//         Test::InteropEigen::Inversion::AddTests(interop_eigen_dir);
//     }

    Test::LinearEmbedding::AddTests(root);
    Test::SplitAndBundle::AddTests(root);
    Test::Tuple::AddTests(root);
    Test::Typle::AddTests(root);
//     Test::Tensor2::AddTests(root);
//     Test::Tensor2Diagonal::AddTests(root);

    int failure_count = RunScheduled(argc, argv, envp, root);

    return (failure_count > 0) ? 1 : 0;
}
