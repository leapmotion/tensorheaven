// ///////////////////////////////////////////////////////////////////////////
// test_linearembedding.cpp by Max Sills, created 2013/12/31
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_linearembedding.hpp"

#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/directsum.hpp"
#include "tenh/conceptual/exteriorpower.hpp"
#include "tenh/conceptual/linearembedding.hpp"
#include "tenh/conceptual/scalarbased2tensorproduct.hpp"
#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/conceptual/utility/linearembedding_checker.hpp"
#include "tenh/implementation/diagonal2tensor.hpp"
#include "tenh/implementation/scalar2tensor.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"
#include "tenh/implementation/wedge.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace LinearEmbedding {

template <typename Domain_, typename Codomain_, typename Scalar_, typename EmbeddingId_>
void check_linear_embedding (Context const &context)
{
    typedef Tenh::LinearEmbedding_c<Domain_,Codomain_,Scalar_,EmbeddingId_,Tenh::ENABLE_EXCEPTIONS> LinearEmbedding;
    std::string error_string;
    bool is_consistent = linear_embedding_is_consistent(LinearEmbedding(), &error_string);
    if (!is_consistent)
        std::cerr << error_string;
    assert(is_consistent);
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename EmbeddingId_>
void check_mutual_adjointness (Context const &context)
{
    Domain_ domain;
    Codomain_ codomain;

    typedef Tenh::ImplementationOf_t<Domain_,Scalar_> D;
    typedef Tenh::ImplementationOf_t<Codomain_,Scalar_> C;

    D d(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    typename C::Dual c(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);

    for (typename D::ComponentIndex i; i.is_not_at_end(); ++i)
        d[i] = i.value() * (i.value() + 3);
    for (typename C::Dual::ComponentIndex i; i.is_not_at_end(); ++i)
        c[i] = i.value() * (i.value() + 1);

    Tenh::AbstractIndex_c<'i'> j;
    // ensure that the embed and coembed operations are mutually adjoint with
    // respect to the appropriate natural pairings.
    assert_eq(d.embed(codomain,j)*c(j) - d(j)*c.coembed(dual(domain),j), Scalar_(0));
}

template <typename BasedVectorSpace_, typename Scalar_>
void add_checks (Directory &parent)
{
    {
        Directory &identity_embedding_dir = parent.GetSubDirectory("identity embedding");

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(identity_embedding_dir, "identity on " + Tenh::type_string_of<BasedVectorSpace_>(), check_linear_embedding<BasedVectorSpace_,BasedVectorSpace_,Scalar_,Tenh::IdentityEmbedding>, RESULT_NO_ERROR);
    }

    typedef Tenh::Scalar2TensorProductOfBasedVectorSpaces_c<BasedVectorSpace_,BasedVectorSpace_> Scalar2;
    typedef Tenh::Diagonal2TensorProductOfBasedVectorSpaces_c<BasedVectorSpace_,BasedVectorSpace_> Diag2;
    typedef Tenh::DirectSum_c<Tenh::TypeList_t<BasedVectorSpace_> > DirectSum;
    typedef Tenh::ExteriorPowerOfBasedVectorSpace_c<1,BasedVectorSpace_> ExteriorPower1;
    typedef Tenh::ExteriorPowerOfBasedVectorSpace_c<2,BasedVectorSpace_> ExteriorPower2;
    typedef Tenh::ExteriorPowerOfBasedVectorSpace_c<3,BasedVectorSpace_> ExteriorPower3;
    typedef Tenh::ExteriorPowerOfBasedVectorSpace_c<4,BasedVectorSpace_> ExteriorPower4;
    typedef Tenh::ExteriorPowerOfBasedVectorSpace_c<5,BasedVectorSpace_> ExteriorPower5;
    typedef Tenh::SymmetricPowerOfBasedVectorSpace_c<1,BasedVectorSpace_> SymmetricPower1;
    typedef Tenh::SymmetricPowerOfBasedVectorSpace_c<2,BasedVectorSpace_> SymmetricPower2;
    typedef Tenh::SymmetricPowerOfBasedVectorSpace_c<3,BasedVectorSpace_> SymmetricPower3;
    typedef Tenh::SymmetricPowerOfBasedVectorSpace_c<4,BasedVectorSpace_> SymmetricPower4;
    typedef Tenh::SymmetricPowerOfBasedVectorSpace_c<5,BasedVectorSpace_> SymmetricPower5;
    typedef typename Tenh::TensorPowerOfBasedVectorSpace_f<1,BasedVectorSpace_>::T TensorPower1;
    typedef typename Tenh::TensorPowerOfBasedVectorSpace_f<2,BasedVectorSpace_>::T TensorPower2;
    typedef typename Tenh::TensorPowerOfBasedVectorSpace_f<3,BasedVectorSpace_>::T TensorPower3;
    typedef typename Tenh::TensorPowerOfBasedVectorSpace_f<4,BasedVectorSpace_>::T TensorPower4;
    typedef typename Tenh::TensorPowerOfBasedVectorSpace_f<5,BasedVectorSpace_>::T TensorPower5;

    {
        Directory &natural_embedding_dir = parent.GetSubDirectory("natural embedding");

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<Scalar2>() + " into " + Tenh::type_string_of<Diag2>(), check_linear_embedding<Scalar2,Diag2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<Scalar2>() + " into " + Tenh::type_string_of<SymmetricPower2>(), check_linear_embedding<Scalar2,SymmetricPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<Scalar2>() + " into " + Tenh::type_string_of<TensorPower2>(), check_linear_embedding<Scalar2,TensorPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<Diag2>() + " into " + Tenh::type_string_of<SymmetricPower2>(), check_linear_embedding<Scalar2,SymmetricPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<Diag2>() + " into " + Tenh::type_string_of<TensorPower2>(), check_linear_embedding<Scalar2,TensorPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);

        static Uint32 const DIM = Tenh::DimensionOf_f<BasedVectorSpace_>::V;
        if (DIM >= 1)
            LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<ExteriorPower1>() + " into " + Tenh::type_string_of<TensorPower1>(), check_linear_embedding<ExteriorPower1,TensorPower1,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        if (DIM >= 2)
            LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<ExteriorPower2>() + " into " + Tenh::type_string_of<TensorPower2>(), check_linear_embedding<ExteriorPower2,TensorPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        if (DIM >= 3)
            LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<ExteriorPower3>() + " into " + Tenh::type_string_of<TensorPower3>(), check_linear_embedding<ExteriorPower3,TensorPower3,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        if (DIM >= 4)
            LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<ExteriorPower4>() + " into " + Tenh::type_string_of<TensorPower4>(), check_linear_embedding<ExteriorPower4,TensorPower4,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        if (DIM >= 5)
            LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<ExteriorPower5>() + " into " + Tenh::type_string_of<TensorPower5>(), check_linear_embedding<ExteriorPower5,TensorPower5,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<SymmetricPower1>() + " into " + Tenh::type_string_of<TensorPower1>(), check_linear_embedding<SymmetricPower1,TensorPower1,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<SymmetricPower2>() + " into " + Tenh::type_string_of<TensorPower2>(), check_linear_embedding<SymmetricPower2,TensorPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<SymmetricPower3>() + " into " + Tenh::type_string_of<TensorPower3>(), check_linear_embedding<SymmetricPower3,TensorPower3,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<SymmetricPower4>() + " into " + Tenh::type_string_of<TensorPower4>(), check_linear_embedding<SymmetricPower4,TensorPower4,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<SymmetricPower5>() + " into " + Tenh::type_string_of<TensorPower5>(), check_linear_embedding<SymmetricPower5,TensorPower5,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
    }

    {
        Directory &mutual_adjointness_dir = parent.GetSubDirectory("mutual adjointness");

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<Scalar2>() + " into " + Tenh::type_string_of<Diag2>(), check_mutual_adjointness<Scalar2,Diag2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<Scalar2>() + " into " + Tenh::type_string_of<SymmetricPower2>(), check_mutual_adjointness<Scalar2,SymmetricPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<Scalar2>() + " into " + Tenh::type_string_of<TensorPower2>(), check_mutual_adjointness<Scalar2,TensorPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<Diag2>() + " into " + Tenh::type_string_of<SymmetricPower2>(), check_mutual_adjointness<Scalar2,SymmetricPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<Diag2>() + " into " + Tenh::type_string_of<TensorPower2>(), check_mutual_adjointness<Scalar2,TensorPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);

        static Uint32 const DIM = Tenh::DimensionOf_f<BasedVectorSpace_>::V;
        if (DIM >= 1)
            LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<ExteriorPower1>() + " into " + Tenh::type_string_of<TensorPower1>(), check_mutual_adjointness<ExteriorPower1,TensorPower1,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        if (DIM >= 2)
            LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<ExteriorPower2>() + " into " + Tenh::type_string_of<TensorPower2>(), check_mutual_adjointness<ExteriorPower2,TensorPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        if (DIM >= 3)
            LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<ExteriorPower3>() + " into " + Tenh::type_string_of<TensorPower3>(), check_mutual_adjointness<ExteriorPower3,TensorPower3,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        if (DIM >= 4)
            LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<ExteriorPower4>() + " into " + Tenh::type_string_of<TensorPower4>(), check_mutual_adjointness<ExteriorPower4,TensorPower4,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        if (DIM >= 5)
            LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<ExteriorPower5>() + " into " + Tenh::type_string_of<TensorPower5>(), check_mutual_adjointness<ExteriorPower5,TensorPower5,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<SymmetricPower1>() + " into " + Tenh::type_string_of<TensorPower1>(), check_mutual_adjointness<SymmetricPower1,TensorPower1,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<SymmetricPower2>() + " into " + Tenh::type_string_of<TensorPower2>(), check_mutual_adjointness<SymmetricPower2,TensorPower2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<SymmetricPower3>() + " into " + Tenh::type_string_of<TensorPower3>(), check_mutual_adjointness<SymmetricPower3,TensorPower3,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<SymmetricPower4>() + " into " + Tenh::type_string_of<TensorPower4>(), check_mutual_adjointness<SymmetricPower4,TensorPower4,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<SymmetricPower5>() + " into " + Tenh::type_string_of<TensorPower5>(), check_mutual_adjointness<SymmetricPower5,TensorPower5,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
    }

}

template <typename BasedVectorSpace0_, typename BasedVectorSpace1_, typename Scalar_>
void add_checks_for_two_spaces (Directory &parent)
{
    typedef Tenh::Scalar2TensorProductOfBasedVectorSpaces_c<BasedVectorSpace0_,BasedVectorSpace1_> Scalar2;
    typedef Tenh::Diagonal2TensorProductOfBasedVectorSpaces_c<BasedVectorSpace0_,BasedVectorSpace1_> Diag2;
    typedef Tenh::TensorProductOfBasedVectorSpaces_c<Tenh::TypeList_t<BasedVectorSpace0_,Tenh::TypeList_t<BasedVectorSpace1_> > > Tensor2;

    {
        Directory &natural_embedding_dir = parent.GetSubDirectory("natural embedding");

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<Scalar2>() + " into " + Tenh::type_string_of<Diag2>(), check_linear_embedding<Scalar2,Diag2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<Scalar2>() + " into " + Tenh::type_string_of<Tensor2>(), check_linear_embedding<Scalar2,Tensor2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(natural_embedding_dir, Tenh::type_string_of<Diag2>() + " into " + Tenh::type_string_of<Tensor2>(), check_linear_embedding<Scalar2,Tensor2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
    }

    {
        Directory &mutual_adjointness_dir = parent.GetSubDirectory("mutual adjointness");

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<Scalar2>() + " into " + Tenh::type_string_of<Diag2>(), check_mutual_adjointness<Scalar2,Diag2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<Scalar2>() + " into " + Tenh::type_string_of<Tensor2>(), check_mutual_adjointness<Scalar2,Tensor2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);

        LVD_ADD_NAMED_TEST_CASE_FUNCTION(mutual_adjointness_dir, Tenh::type_string_of<Diag2>() + " into " + Tenh::type_string_of<Tensor2>(), check_mutual_adjointness<Scalar2,Tensor2,Scalar_,Tenh::NaturalEmbedding>, RESULT_NO_ERROR);
    }
}

void AddTests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory("linearembedding");

    typedef float Scalar;

    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,1,Tenh::Generic>,Tenh::Basis_c<Tenh::Generic> > B1;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,2,Tenh::Generic>,Tenh::Basis_c<Tenh::Generic> > B2;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,3,Tenh::Generic>,Tenh::Basis_c<Tenh::Generic> > B3;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,4,Tenh::Generic>,Tenh::Basis_c<Tenh::Generic> > B4;
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,5,Tenh::Generic>,Tenh::Basis_c<Tenh::Generic> > B5;

    add_checks<B1,double>(dir);
    add_checks<B2,double>(dir);
    add_checks<B3,double>(dir);
    add_checks<B4,double>(dir);
    add_checks<B5,double>(dir);

    add_checks_for_two_spaces<B1,B2,double>(dir);
    add_checks_for_two_spaces<B1,B3,double>(dir);
    add_checks_for_two_spaces<B1,B4,double>(dir);
    add_checks_for_two_spaces<B1,B5,double>(dir);

    add_checks_for_two_spaces<B2,B1,double>(dir);
    add_checks_for_two_spaces<B2,B3,double>(dir);
    add_checks_for_two_spaces<B2,B4,double>(dir);
    add_checks_for_two_spaces<B2,B5,double>(dir);

    add_checks_for_two_spaces<B3,B1,double>(dir);
    add_checks_for_two_spaces<B3,B2,double>(dir);
    add_checks_for_two_spaces<B3,B4,double>(dir);
    add_checks_for_two_spaces<B3,B5,double>(dir);

    add_checks_for_two_spaces<B4,B1,double>(dir);
    add_checks_for_two_spaces<B4,B2,double>(dir);
    add_checks_for_two_spaces<B4,B3,double>(dir);
    add_checks_for_two_spaces<B4,B5,double>(dir);

    add_checks_for_two_spaces<B5,B1,double>(dir);
    add_checks_for_two_spaces<B5,B2,double>(dir);
    add_checks_for_two_spaces<B5,B3,double>(dir);
    add_checks_for_two_spaces<B5,B4,double>(dir);
}

} // end of namespace LinearEmbedding
} // end of namespace Test
