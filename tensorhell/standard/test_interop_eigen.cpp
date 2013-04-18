// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen.cpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_interop_eigen.hpp"

#include "tenh/interop/eigen.hpp"
#include "tenh/interop/eigen_selfadjointeigendecomp.hpp"
#include "tenh/interop/eigen_svd.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace InteropEigen {

// do eigen decomposition in the "packed with symmetries" form, then unpack,
// and compare to unpacked decomposition.
template <typename Factor>
void test_self_adjoint_eigen_decomp (Context const &context)
{
    typedef Tenh::Tensor2Symmetric_t<Factor> S;
    S s(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    // TODO: real random S value
    for (typename S::Index i; i.is_not_at_end(); ++i)
        s[i] = i.value() + 1;
    
    typedef Tenh::Tensor2Diagonal_t<typename Factor::WithStandardEuclideanBasis,
                                    typename Factor::WithStandardEuclideanBasis> D;
    typedef Tenh::Tensor2_t<typename Factor::WithStandardEuclideanBasis,Factor> U;
    D eigenvalues(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    U eigenvectors(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    // diagonalize
    diagonalize_Tensor2Symmetric(s, d, u);
    // verify that 
}

template <typename Factor>
void add_particular_self_adjoint_eigen_decomp_test (Directory *parent)
{
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent,
                                     "test_self_adjoint_eigen_decomp<" + Tenh::TypeStringOf_t<Factor>::eval() + '>',
                                     test_self_adjoint_eigen_decomp,
                                     RESULT_NO_ERROR);
}

void AddTests (Directory *parent)
{
    Directory *interop_eigen = new Directory("interop_eigen", parent);

    add_particular_self_adjoint_eigen_decomp_test<
}

} // end of namespace InteropEigen
} // end of namespace Test