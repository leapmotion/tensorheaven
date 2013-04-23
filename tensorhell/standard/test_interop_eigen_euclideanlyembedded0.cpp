// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen_euclideanlyembedded0.cpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_interop_eigen_euclideanlyembedded.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace InteropEigen {
namespace EuclideanlyEmbedded {

void AddTests0 (Directory *parent)
{
    add_embedding_tests_1<float,2>(parent);
    add_embedding_tests_1<float,3>(parent);
    add_embedding_tests_1<float,4>(parent);
}

} // end of namespace EuclideanlyEmbedded
} // end of namespace InteropEigen
} // end of namespace Test