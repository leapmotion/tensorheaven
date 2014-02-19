// ///////////////////////////////////////////////////////////////////////////
// test_basic_operator.cpp by Victor Dods, created 2013/10/04
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_basic_operator.hpp"

#include "tenh/basic.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Basic {
namespace Operator {

struct X { static std::string type_as_string () { return "X"; } };
struct Y { static std::string type_as_string () { return "Y"; } };
struct Z { static std::string type_as_string () { return "Z"; } };

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void constructor_without_initialization (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void constructor_fill_with (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Scalar_ fill = context.DataAs<Scalar_>();
    Operator A(Tenh::fill_with(fill));
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], fill);
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void assignment (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator const B(Tenh::fill_with(3));
    Operator C(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        C[i] = 3*i.value() + 4;

    // plain non-expression template assignment
    A = B;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], B[i]);

    // plain non-expression template assignment
    A = C;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], C[i]);

    // expression template assignment
    Tenh::AbstractIndex_c<'j'> j;
    Tenh::AbstractIndex_c<'k'> k;
    A = Tenh::reindexable(B(j*k));
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], B[i]);

    // expression template assignment
    A = Tenh::reindexable(C(j*k));
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], C[i]);
}

// NOTE: All the bullshit casts to Scalar_ are necessary because the operands
// *may* be promoted to a larger type (e.g. char -> int).  This is an idiotic
// part of the C standard.

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void addition (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator B(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator C(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator M(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator N(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        B[i] = i.value();
        C[i] = 3*i.value() + 4;
        M[i] = 8*i.value() - 3;
        N[i] = i.value()*i.value();
    }

    A = B + C;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(B[i]+C[i]));

    A = C + M;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(C[i]+M[i]));

    A = B + (C + M);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i],Scalar_(B[i]+Scalar_(C[i]+M[i])));

    A = (B + C) + M;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(Scalar_(B[i]+C[i])+M[i]));

    A = (B + C) + (M + N);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(Scalar_(B[i]+C[i])+Scalar_(M[i]+N[i])));
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void subtraction (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator B(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator C(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator M(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator N(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        B[i] = i.value();
        C[i] = 3*i.value() + 4;
        M[i] = 8*i.value() - 3;
        N[i] = i.value()*i.value();
    }

    A = B - C;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(B[i]-C[i]));

    A = B - (C - M);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(B[i]-Scalar_(C[i]-M[i])));

    A = (B - C) - M;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(Scalar_(B[i]-C[i])-M[i]));

    A = (B - C) - (M - N);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(Scalar_(B[i]-C[i])-Scalar_(M[i]-N[i])));
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void scalar_multiplication (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator B(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        B[i] = i.value();

    A = B * Scalar_(3.14);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(B[i]*Scalar_(3.14)));

    A = Scalar_(3.14) * B;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(B[i]*Scalar_(3.14)));

    A = B / 3;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(B[i]/Scalar_(3)));

    A = B / Scalar_(3.14);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(B[i]/Scalar_(3.14)));
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void negation (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator B(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        B[i] = i.value();

    A = -B;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], Scalar_(-B[i]));
}

template <typename BasedVectorSpace1_, typename BasedVectorSpace2_, typename BasedVectorSpace3_, typename Scalar_, typename UseArrayType_>
void composition (Context const &context)
{
    typedef Tenh::Operator<BasedVectorSpace1_,BasedVectorSpace2_,Scalar_,UseArrayType_> Operator12;
    typedef Tenh::Operator<BasedVectorSpace2_,BasedVectorSpace3_,Scalar_,UseArrayType_> Operator23;
    typedef Tenh::Vector<BasedVectorSpace1_,Scalar_,UseArrayType_> Vector1;
    typedef Tenh::Vector<BasedVectorSpace2_,Scalar_,UseArrayType_> Vector2;
    typedef Tenh::Vector<BasedVectorSpace3_,Scalar_,UseArrayType_> Vector3;
    typedef typename Tenh::DualOf_f<Vector3>::T Covector3;

    Covector3 a3(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Covector3 b3(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator12 M12(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator12 N12(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator23 M23(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator23 N23(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector1 v1(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector1 w1(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector2 v2(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector2 w2(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector3 v3(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector3 w3(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Covector3::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        a3[i] = (3*i.value()) % 2;
        b3[i] = i.value() + 1;
    }
    for (typename Operator12::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        M12[i] = i.value();
        N12[i] = 2*i.value() + 17;
    }
    for (typename Operator23::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        M23[i] = i.value() * i.value();
        N23[i] = 8*i.value() - 3;
    }
    for (typename Vector1::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        v1[i] = (5*i.value()) % 3;
        w1[i] = i.value() + 2;
    }
    for (typename Vector2::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        v2[i] = i.value() % 3;
        w2[i] = i.value() + 3;
    }
    for (typename Vector3::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        v3[i] = i.value() % 2;
        w3[i] = i.value() + 5;
    }

    // covector on left of operator
    {
        for (typename Vector2::ComponentIndex i; i.is_not_at_end(); ++i)
        {
            Scalar_ expected_result(0);
            for (typename Covector3::ComponentIndex j; j.is_not_at_end(); ++j)
                expected_result += Scalar_(a3[j] * M23[typename Operator23::MultiIndex(j, i)]);
            assert_eq((a3 * M23)[typename Vector2::MultiIndex(i)], expected_result);
        }

        for (typename Vector2::ComponentIndex i; i.is_not_at_end(); ++i)
        {
            Scalar_ expected_result(0);
            for (typename Covector3::ComponentIndex j; j.is_not_at_end(); ++j)
                expected_result += Scalar_(Scalar_(a3[j] + b3[j]) * M23[typename Operator23::MultiIndex(j, i)]);
            assert_eq(((a3 + b3) * M23)[typename Vector2::MultiIndex(i)], expected_result);
        }
    }

    // vector on right of operator
    {
        for (typename Vector3::ComponentIndex i; i.is_not_at_end(); ++i)
        {
            Scalar_ expected_result(0);
            for (typename Vector2::ComponentIndex j; j.is_not_at_end(); ++j)
                expected_result += Scalar_(M23[typename Operator23::MultiIndex(i, j)] * v2[j]);
            assert_eq((M23 * v2)[typename Vector3::MultiIndex(i)], expected_result);
        }

        for (typename Vector3::ComponentIndex i; i.is_not_at_end(); ++i)
        {
            Scalar_ expected_result(0);
            for (typename Vector2::ComponentIndex j; j.is_not_at_end(); ++j)
                expected_result += Scalar_(M23[typename Operator23::MultiIndex(i, j)] * Scalar_(v2[j] + w2[j]));
            assert_eq((M23 * (v2 + w2))[typename Vector3::MultiIndex(i)], expected_result);
        }
    }

    // operator composed with operator
    {
        for (typename Vector3::ComponentIndex i; i.is_not_at_end(); ++i)
        {
            for (typename Vector1::ComponentIndex j; j.is_not_at_end(); ++j)
            {
                Scalar_ expected_result(0);
                for (typename Vector2::ComponentIndex k; k.is_not_at_end(); ++k)
                    expected_result += Scalar_(M23[typename Operator23::MultiIndex(i, k)] * M12[typename Operator12::MultiIndex(k, j)]);
                typedef Tenh::Typle_t<typename Vector3::ComponentIndex,
                                      typename Vector1::ComponentIndex> ResultIndexTyple;
                assert_eq((M23 * M12)[Tenh::MultiIndex_t<ResultIndexTyple>(i, j)], expected_result);
            }
        }

        for (typename Vector3::ComponentIndex i; i.is_not_at_end(); ++i)
        {
            for (typename Vector1::ComponentIndex j; j.is_not_at_end(); ++j)
            {
                Scalar_ expected_result(0);
                for (typename Vector2::ComponentIndex k; k.is_not_at_end(); ++k)
                    expected_result += Scalar_(M23[typename Operator23::MultiIndex(i, k)]
                                               * Scalar_(M12[typename Operator12::MultiIndex(k, j)] + N12[typename Operator12::MultiIndex(k, j)]));
                typedef Tenh::Typle_t<typename Vector3::ComponentIndex,
                                      typename Vector1::ComponentIndex> ResultIndexTyple;
                assert_eq((M23 * (M12 + N12))[Tenh::MultiIndex_t<ResultIndexTyple>(i, j)], expected_result);
            }
        }

        for (typename Vector3::ComponentIndex i; i.is_not_at_end(); ++i)
        {
            for (typename Vector1::ComponentIndex j; j.is_not_at_end(); ++j)
            {
                Scalar_ expected_result(0);
                for (typename Vector2::ComponentIndex k; k.is_not_at_end(); ++k)
                    expected_result += Scalar_(Scalar_(M23[typename Operator23::MultiIndex(i, k)] + N23[typename Operator23::MultiIndex(i, k)])
                                               * M12[typename Operator12::MultiIndex(k, j)]);
                typedef Tenh::Typle_t<typename Vector3::ComponentIndex,
                                      typename Vector1::ComponentIndex> ResultIndexTyple;
                assert_eq(((M23 + N23) * M12)[Tenh::MultiIndex_t<ResultIndexTyple>(i, j)], expected_result);
            }
        }
    }

    // two compositions
    {
        Scalar_ expected_result(0);
        for (typename Covector3::ComponentIndex i; i.is_not_at_end(); ++i)
            for (typename Vector2::ComponentIndex j; j.is_not_at_end(); ++j)
                expected_result += Scalar_(Scalar_(a3[i] * M23[typename Operator23::MultiIndex(i, j)]) * v2[j]);
        Scalar_ actual_result = a3 * M23 * v2;
        assert_eq(actual_result, expected_result);
    }

    // three compositions
    {
        Scalar_ expected_result(0);
        for (typename Covector3::ComponentIndex i; i.is_not_at_end(); ++i)
            for (typename Vector2::ComponentIndex j; j.is_not_at_end(); ++j)
                for (typename Vector1::ComponentIndex k; k.is_not_at_end(); ++k)
                    expected_result += Scalar_(Scalar_(Scalar_(a3[i] * M23[typename Operator23::MultiIndex(i, j)])
                                                                     * M12[typename Operator12::MultiIndex(j, k)])
                                                                     * v1[k]);
        Scalar_ actual_result = a3 * M23 * M12 * v1;
        assert_eq(actual_result, expected_result);
    }
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void add_particular_tests (Directory &parent)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;
    Directory &dir = parent.GetSubDirectory(Tenh::type_string_of<Operator>());
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "constructor_without_initialization", constructor_without_initialization<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "constructor_fill_with", constructor_fill_with<Domain_,Codomain_,Scalar_,UseArrayType_>, new Context::Data<Scalar_>(42), RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "assignment", assignment<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "addition", addition<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "subtraction", subtraction<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "scalar_multiplication", scalar_multiplication<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "negation", negation<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
}

template <typename BasedVectorSpace1_, typename BasedVectorSpace2_, typename BasedVectorSpace3_, typename Scalar_, typename UseArrayType_>
void add_particular_composition_tests (Directory &dir)
{
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir,
                                     FORMAT("composition<" << Tenh::type_string_of<BasedVectorSpace1_>() << ','
                                                           << Tenh::type_string_of<BasedVectorSpace2_>() << ','
                                                           << Tenh::type_string_of<BasedVectorSpace3_>() << ','
                                                           << Tenh::type_string_of<Scalar_>() << ','
                                                           << Tenh::type_string_of<UseArrayType_>() << '>'),
                                     composition<BasedVectorSpace1_,BasedVectorSpace2_,BasedVectorSpace3_,Scalar_,UseArrayType_>,
                                     RESULT_NO_ERROR);
}


template <typename Scalar_, typename UseArrayType_>
void add_particular_tests_for_scalar_and_use_array_type (Directory &parent)
{
    // TODO: these tests on 0-dimensional vector spaces aren't working -- fix
//     {
//         typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,0,X>,Tenh::Basis_c<X>> BasedVectorSpaceX;
//         typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,1,Y>,Tenh::Basis_c<Y>> BasedVectorSpaceY;
//         typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,2,Z>,Tenh::Basis_c<Z>> BasedVectorSpaceZ;
//         add_particular_tests<BasedVectorSpaceX,BasedVectorSpaceY,Scalar_,UseArrayType_>(parent);
//         add_particular_composition_tests<BasedVectorSpaceX,BasedVectorSpaceY,BasedVectorSpaceZ,Scalar_,UseArrayType_>(parent);
//     }
    {
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,1,X>,Tenh::Basis_c<X>> BasedVectorSpaceX;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,3,Y>,Tenh::Basis_c<Y>> BasedVectorSpaceY;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,2,Z>,Tenh::Basis_c<Z>> BasedVectorSpaceZ;
        add_particular_tests<BasedVectorSpaceX,BasedVectorSpaceY,Scalar_,UseArrayType_>(parent);
        add_particular_composition_tests<BasedVectorSpaceX,BasedVectorSpaceY,BasedVectorSpaceZ,Scalar_,UseArrayType_>(parent);
    }
    {
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,3,X>,Tenh::Basis_c<X>> BasedVectorSpaceX;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,4,Y>,Tenh::Basis_c<Y>> BasedVectorSpaceY;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,5,Z>,Tenh::Basis_c<Z>> BasedVectorSpaceZ;
        add_particular_tests<BasedVectorSpaceX,BasedVectorSpaceY,Scalar_,UseArrayType_>(parent);
        add_particular_composition_tests<BasedVectorSpaceX,BasedVectorSpaceY,BasedVectorSpaceZ,Scalar_,UseArrayType_>(parent);
    }
}

template <typename Scalar_>
void add_particular_tests_for_scalar (Directory &parent)
{
    add_particular_tests_for_scalar_and_use_array_type<Scalar_,Tenh::UseMemberArray_t<Tenh::ComponentsAreConst::FALSE>>(parent);
}

void AddTests (Directory &parent)
{
    Directory &dir = parent.GetSubDirectory("Operator");
    add_particular_tests_for_scalar<Sint8>(dir);
    add_particular_tests_for_scalar<Uint8>(dir);
    add_particular_tests_for_scalar<Sint16>(dir);
    add_particular_tests_for_scalar<Uint16>(dir);
    add_particular_tests_for_scalar<Sint32>(dir);
    add_particular_tests_for_scalar<Uint32>(dir);
    add_particular_tests_for_scalar<Sint64>(dir);
    add_particular_tests_for_scalar<Uint64>(dir);
    add_particular_tests_for_scalar<float>(dir);
    add_particular_tests_for_scalar<double>(dir);
    add_particular_tests_for_scalar<complex<float>>(dir);
    add_particular_tests_for_scalar<complex<double>>(dir);

    // TODO: refactor all this test code so that Op<...> can be tested in a way analogous to Vec.
}

} // end of namespace Operator
} // end of namespace Basic
} // end of namespace Test
