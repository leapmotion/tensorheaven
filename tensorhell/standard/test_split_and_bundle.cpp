// ///////////////////////////////////////////////////////////////////////////
// test_split_and_bundle.cpp by Max Sills, created 2013/10/03
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_split_and_bundle.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/tensor.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace SplitAndBundle {

// TODO: templatize this and test a bunch of different splittable types
void test_partial_inverse (Context const &context)
{
    static Uint32 const DIM = 3;
    typedef int DummyId;
    typedef float Scalar;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,DummyId>,Tenh::Basis_c<DummyId> > BasedVectorSpace;
    typedef Tenh::TypeList_t<BasedVectorSpace,
            Tenh::TypeList_t<BasedVectorSpace, 
            Tenh::TypeList_t<BasedVectorSpace> > > FactorTypeList;
    typedef Tenh::TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProduct;
    typedef Tenh::ImplementationOf_t<TensorProduct,Scalar> Tensor;

    Tensor t(Tenh::fill_with(3));
    for (Tensor::ComponentIndex i; i.is_not_at_end(); ++i)
        t[i] = i.value();

    // Tenh::AbstractIndex_c <'I'> I;
    Tenh::AbstractIndex_c <'J'> J;
    Tenh::AbstractIndex_c <'K'> K;
    Tenh::AbstractIndex_c <'L'> L;
    Tenh::AbstractIndex_c <'Z'> Z;
    /*
    std::cerr << FORMAT_VALUE(t) << '\n';
    std::cerr << FORMAT_VALUE(t(Z)) << '\n';
    std::cerr << FORMAT_VALUE(t(I).split(I,J*K*L)) << '\n';
    std::cerr << FORMAT_VALUE(t(I).split(I,J*K*L).bundle(J*K*L,TensorProduct() ,Z ) ) << '\n';
    */
    Tensor eta(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    eta(Z) = t(Z) - t.split(J*K*L).bundle(J*K*L, TensorProduct(), Z);
    //std::cerr << FORMAT_VALUE(eta(Z)) << '\n';
    for (Tensor::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(eta[i], float(0)); 
    
}



void AddTests (Directory &parent)
{
    Directory &split_and_bundle_dir = parent.GetSubDirectory("split_and_bundle");

    LVD_ADD_TEST_CASE_FUNCTION(split_and_bundle_dir, test_partial_inverse, RESULT_NO_ERROR);
}

} // end of namespace SplitAndBundle
} // end of namespace Test
