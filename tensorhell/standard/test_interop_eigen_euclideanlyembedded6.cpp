// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen_euclideanlyembedded6.cpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_interop_eigen_euclideanlyembedded.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace InteropEigen {
namespace EuclideanlyEmbedded {

void AddTests6 (Directory *parent)
{
//     add_embedding_tests_2<double,3,1>(parent); // this causes a static assert in Eigen
    add_embedding_tests_2<double,3,3>(parent);
    add_embedding_tests_2<double,3,4>(parent);
}

} // end of namespace EuclideanlyEmbedded
} // end of namespace InteropEigen
} // end of namespace Test