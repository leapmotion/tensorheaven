// ///////////////////////////////////////////////////////////////////////////
// test_eigen_ldlt.cpp by Ted Nitz, created 2013/04/23
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_interop_eigen_ldlt.hpp"
#include "randomize.hpp"

#include <utility> // for std::pair
#include <complex>
#include <iostream>

#include "tenh/interop/eigen_ldlt.hpp"
#include "tenh/meta/typelist.hpp"
#include "tenh/expressiontemplate_eval.hpp"
#include "tenh/multiindex.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2antisymmetric.hpp"
#include "tenh/tensor2symmetric.hpp"
#include "tenh/vector.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace EigenLDLT {

template<typename Factor>
void TestLDLTSolve(Context const &context)
{
    typedef typename Factor::Scalar Scalar;
    typedef Tenh::Tensor2Symmetric_t<Factor> Tensor;
    
    Tenh::TypedIndex_t<Factor,'i'> i;
    Tenh::TypedIndex_t<Factor,'j'> j;
    
    Factor x(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON), b(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Tensor a(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    
    randomize(a);
    randomize(b);
    
    LDLT_Solve(a,x,b);
    
    // std::cerr << FORMAT_VALUE(a(i|j)*x(j)) << std::endl << FORMAT_VALUE(b(i)) << std::endl;
    
    assert_about_eq((a(i|j)*x(j) - b(i)).squared_norm(), Scalar(0));
}

template <typename Scalar, Uint32 DIM>
void add_particular_tests (Directory *parent)
{
    typedef Tenh::Vector_t<Scalar,DIM> V;
    typedef Tenh::Tensor2_t<V, V> Tensor2;
    typedef Tenh::Tensor2Antisymmetric_t<V, V> Tensor2Antisymmetric;
    typedef Tenh::Tensor2Symmetric_t<V, V> Tensor2Symmetric;
    typedef Tenh::Tensor2Symmetric_t<Tensor2Symmetric> DoubleSymmetric;
    typedef Tenh::Tensor2Symmetric_t<Tensor2Antisymmetric> SymmetricAntisymmetric;
    typedef Tenh::Tensor2Antisymmetric_t<Tensor2Symmetric> AntisymmetricSymmetric;
    typedef Tenh::Tensor2Antisymmetric_t<Tensor2Antisymmetric> DoubleAntisymmetric;

    Directory *vector
        = new Directory(Tenh::type_string_of<V>(), parent);

    Directory *tensor2
        = new Directory(Tenh::type_string_of<Tensor2>(), parent);

    Directory *tensor2antisymmetric
        = new Directory(Tenh::type_string_of<Tensor2Antisymmetric>(), parent);

    Directory *tensor2symmetric
        = new Directory(Tenh::type_string_of<Tensor2Symmetric>(), parent);

    Directory *doublesymmetric
        = new Directory(Tenh::type_string_of<DoubleSymmetric>(), parent);

    Directory *symmetricantisymmetric
        = new Directory(Tenh::type_string_of<SymmetricAntisymmetric>(), parent);

    Directory *antisymmetricsymmetric
        = new Directory(Tenh::type_string_of<AntisymmetricSymmetric>(), parent);

    Directory *doubleantisymmetric
        = new Directory(Tenh::type_string_of<DoubleAntisymmetric>(), parent);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(vector, "test",
        TestLDLTSolve<V>, RESULT_NO_ERROR);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2, "test",
        TestLDLTSolve<Tensor2>, RESULT_NO_ERROR);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2antisymmetric, "test",
        TestLDLTSolve<Tensor2Antisymmetric>, RESULT_NO_ERROR);

    LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2symmetric, "test",
        TestLDLTSolve<Tensor2Symmetric>, RESULT_NO_ERROR);

    if(DIM < 3)
    {
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(doublesymmetric, "test",
            TestLDLTSolve<DoubleSymmetric>, RESULT_NO_ERROR);
    }
    if(DIM < 4)
    {
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(symmetricantisymmetric, "test",
            TestLDLTSolve<SymmetricAntisymmetric>, RESULT_NO_ERROR);
        
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(antisymmetricsymmetric, "test",
            TestLDLTSolve<AntisymmetricSymmetric>, RESULT_NO_ERROR);
        
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(doubleantisymmetric, "test",
            TestLDLTSolve<DoubleAntisymmetric>, RESULT_NO_ERROR);
        
    }

}

template <typename Scalar>
void add_particular_tests_for_scalar (Directory *parent)
{
    // add_particular_tests<Scalar,2>(parent);
    add_particular_tests<Scalar,3>(parent);
    add_particular_tests<Scalar,6>(parent);
}

void AddTests (Directory *parent)
{
    Directory *eigen_ldlt = new Directory("EigenLDLT", parent);

    add_particular_tests_for_scalar<float>(eigen_ldlt);
    add_particular_tests_for_scalar<double>(eigen_ldlt);

}

} // end of namespace EigenLDLT
} // end of namespace Test