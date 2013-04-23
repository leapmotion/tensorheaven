// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen_euclideanlyembedded5.cpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_interop_eigen_euclideanlyembedded.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace InteropEigen {
namespace EuclideanlyEmbedded {

void AddTests5 (Directory *parent)
{
    add_embedding_tests_2<double,1,1>(parent);
    add_embedding_tests_2<double,1,3>(parent);
    add_embedding_tests_2<double,1,4>(parent);
}

} // end of namespace EuclideanlyEmbedded
} // end of namespace InteropEigen
} // end of namespace Test