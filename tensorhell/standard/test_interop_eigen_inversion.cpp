// ///////////////////////////////////////////////////////////////////////////
// test_interop_eigen_inversion.cpp by Victor Dods, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_interop_eigen_inversion.hpp"

namespace Test {
namespace InteropEigen {
namespace Inversion {

/*
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
*/

void AddTests (Directory *parent)
{
    Directory *inversion = new Directory("inversion", parent);

    AddTests0(inversion);
    AddTests1(inversion);
    AddTests2(inversion);
    AddTests3(inversion);
    AddTests4(inversion);
    AddTests5(inversion);
    AddTests6(inversion);
    AddTests7(inversion);
}

} // end of namespace Inversion
} // end of namespace InteropEigen
} // end of namespace Test