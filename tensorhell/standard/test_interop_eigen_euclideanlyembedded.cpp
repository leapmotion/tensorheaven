// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen_euclideanlyembedded.cpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_interop_eigen_euclideanlyembedded.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace InteropEigen {
namespace EuclideanlyEmbedded {

void AddTests (Directory *parent)
{
    Directory *euclidean_embedding = new Directory("euclideanly_embedded_EigenMatrix_from_and_to", parent);

    AddTests0(euclidean_embedding);
    AddTests1(euclidean_embedding);
    AddTests2(euclidean_embedding);
    AddTests3(euclidean_embedding);
    AddTests4(euclidean_embedding);
    AddTests5(euclidean_embedding);
    AddTests6(euclidean_embedding);
    AddTests7(euclidean_embedding);
}

} // end of namespace EuclideanlyEmbedded
} // end of namespace InteropEigen
} // end of namespace Test