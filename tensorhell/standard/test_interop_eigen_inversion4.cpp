// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen_inversion4.cpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_interop_eigen_inversion.hpp"

namespace Test {
namespace InteropEigen {
namespace Inversion {

void AddTests4 (Directory *parent)
{
    // add tests for factors having matching dimensions
    typedef Tenh::Vector_t<float,1> Float1;          // dim = 1
    typedef Tenh::Vector_t<float,2> Float2;          // dim = 2
    typedef Tenh::Vector_t<float,3> Float3;          // dim = 3
    typedef Tenh::Vector_t<float,4> Float4;          // dim = 4
    typedef Tenh::Vector_t<float,5> Float5;          // dim = 5
    typedef Tenh::Tensor2_t<Float1,Float1> Float1x1; // dim = 1
    typedef Tenh::Tensor2_t<Float1,Float2> Float1x2; // dim = 2
    typedef Tenh::Tensor2_t<Float1,Float3> Float1x3; // dim = 3
    typedef Tenh::Tensor2_t<Float2,Float1> Float2x1; // dim = 2
    typedef Tenh::Tensor2_t<Float2,Float3> Float2x3; // dim = 6
    typedef Tenh::Tensor2_t<Float2,Float5> Float2x5; // dim = 10
    typedef Tenh::Tensor2_t<Float3,Float1> Float3x1; // dim = 3
    typedef Tenh::Tensor2_t<Float3,Float2> Float3x2; // dim = 6
    typedef Tenh::Tensor2Antisymmetric_t<Float2> A2; // dim = 1
    typedef Tenh::Tensor2Antisymmetric_t<Float3> A3; // dim = 3
    typedef Tenh::Tensor2Antisymmetric_t<Float4> A4; // dim = 6
    typedef Tenh::Tensor2Antisymmetric_t<Float5> A5; // dim = 10
    typedef Tenh::Tensor2Symmetric_t<Float1> S1;     // dim = 1
    typedef Tenh::Tensor2Symmetric_t<Float2> S2;     // dim = 3
    typedef Tenh::Tensor2Symmetric_t<Float3> S3;     // dim = 6
    typedef Tenh::Tensor2Symmetric_t<Float4> S4;     // dim = 10

    // first half of dim = 3
    add_inversion_test_2<Float3,Float1x3>(parent);
    add_inversion_test_2<Float3,Float3x1>(parent);
    add_inversion_test_2<Float3,A3>(parent);
    add_inversion_test_2<Float3,S2>(parent);
}

} // end of namespace Inversion
} // end of namespace InteropEigen
} // end of namespace Test