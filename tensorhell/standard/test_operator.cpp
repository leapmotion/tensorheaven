// ///////////////////////////////////////////////////////////////////////////
// test_operator.cpp by Victor Dods, created 2013/10/04
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_operator.hpp"

#include "tenh/operator.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Operator {

struct X { static std::string type_as_string () { return "X"; } };
struct Y { static std::string type_as_string () { return "Y"; } };

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
    Operator A(fill);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], fill);
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void assignment (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator const B(3.0f);
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
    A = B(j|k);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], B[i]);

    // expression template assignment
    A = C(j|k);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(A[i], C[i]);
}

/*
template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void addition (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator x(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator y(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        v[i] = i.value();
        w[i] = 3*i.value() + 4;
        x[i] = 8*i.value() - 3;
        y[i] = i.value()*i.value();
    }

    u = v + w;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]+w[i]);

    u = v + (w + x);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]+(w[i]+x[i]));

    u = (v + w) + x;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], (v[i]+w[i])+x[i]);

    u = (v + w) + (x + y);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], (v[i]+w[i])+(x[i]+y[i]));
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void subtraction (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator x(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator y(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        v[i] = i.value();
        w[i] = 3*i.value() + 4;
        x[i] = 8*i.value() - 3;
        y[i] = i.value()*i.value();
    }

    u = v - w;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]-w[i]);

    u = v - (w - x);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]-(w[i]-x[i]));

    u = (v - w) - x;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], (v[i]-w[i])-x[i]);

    u = (v - w) - (x - y);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], (v[i]-w[i])-(x[i]-y[i]));
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void scalar_multiplication (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        v[i] = i.value();

    u = v * 3;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]*Scalar_(3));

    u = 3 * v;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]*Scalar_(3));

    u = v * Scalar_(3.14);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]*Scalar_(3.14));

    u = Scalar_(3.14) * v;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]*Scalar_(3.14));

    u = v / 3;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]/Scalar_(3));

    u = v / Scalar_(3.14);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]/Scalar_(3.14));
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void negation (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;

    Operator u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        v[i] = i.value();

    u = -v;
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], -v[i]);
}

template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void natural_pairing (Context const &context)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;
    typedef typename Tenh::DualOf_f<Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> >::T Covector;

    Covector a(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Covector b(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator A(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Operator w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        a[i] = i.value();
        b[i] = 2*i.value() + 17;
        v[i] = 3*i.value() + 4;
        w[i] = i.value()*i.value();
    }

    // covector on left

    {
        Scalar_ actual_result = a * v;
        Scalar_ expected_result(0);
        for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += a[i] * v[i];
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar_ actual_result = a * (v + w);
        Scalar_ expected_result(0);
        for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += a[i] * (v[i] + w[i]);
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar_ actual_result = (a + b) * v;
        Scalar_ expected_result(0);
        for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += (a[i] + b[i]) * v[i];
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar_ actual_result = (a + b) * (v + w);
        Scalar_ expected_result(0);
        for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += (a[i] + b[i]) * (v[i] + w[i]);
        assert_eq(actual_result, expected_result);
    }

    // vector on left
    {
        Scalar_ actual_result = v * a;
        Scalar_ expected_result(0);
        for (typename Operator::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += v[i] * a[i];
        assert_eq(actual_result, expected_result);
    }

}
*/
template <typename Domain_, typename Codomain_, typename Scalar_, typename UseArrayType_>
void add_particular_tests (Directory *parent)
{
    typedef Tenh::Operator<Domain_,Codomain_,Scalar_,UseArrayType_> Operator;
    Directory *dir = new Directory(Tenh::type_string_of<Operator>(), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "constructor_without_initialization", constructor_without_initialization<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "constructor_fill_with", constructor_fill_with<Domain_,Codomain_,Scalar_,UseArrayType_>, new Context::Data<Scalar_>(42), RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "assignment", assignment<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
//     LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "addition", addition<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
//     LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "subtraction", subtraction<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
//     LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "scalar_multiplication", scalar_multiplication<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
//     LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "negation", negation<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
//     LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "natural_pairing", natural_pairing<Domain_,Codomain_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
}

template <typename Scalar_, typename UseArrayType_>
void add_particular_tests_for_scalar_and_use_array_type (Directory *parent)
{
//     {
//         static Uint32 const DIM = 0;
//         typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,X>,Tenh::Basis_c<X> > BasedVectorSpace;
//         add_particular_tests<BasedVectorSpace,Scalar_,UseArrayType_>(parent);
//     }
//     {
//         static Uint32 const DIM = 1;
//         typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,X>,Tenh::Basis_c<X> > BasedVectorSpace;
//         add_particular_tests<BasedVectorSpace,Scalar_,UseArrayType_>(parent);
//     }
    {
        static Uint32 const DOMAIN_DIM = 3;
        static Uint32 const CODOMAIN_DIM = 4;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DOMAIN_DIM,X>,Tenh::Basis_c<X> > BasedVectorSpaceX;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,CODOMAIN_DIM,Y>,Tenh::Basis_c<Y> > BasedVectorSpaceY;
        add_particular_tests<BasedVectorSpaceX,BasedVectorSpaceY,Scalar_,UseArrayType_>(parent);
    }
//     {
//         static Uint32 const DIM = 20;
//         typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,X>,Tenh::Basis_c<X> > BasedVectorSpace;
//         add_particular_tests<BasedVectorSpace,Scalar_,UseArrayType_>(parent);
//     }

//     LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, FORMAT("check_filled_values<" << Tenh::type_string_of<Scalar>() << ">"), check_filled_values<Scalar>, RESULT_NO_ERROR);
}

template <typename Scalar_>
void add_particular_tests_for_scalar (Directory *parent)
{
    add_particular_tests_for_scalar_and_use_array_type<Scalar_,Tenh::UseMemberArray>(parent);
}

void AddTests (Directory *parent)
{
    Directory *dir = new Directory("Operator", parent);
    add_particular_tests_for_scalar<Sint8>(dir);
    add_particular_tests_for_scalar<Uint32>(dir);
    add_particular_tests_for_scalar<Sint64>(dir);
    add_particular_tests_for_scalar<float>(dir);
    add_particular_tests_for_scalar<double>(dir);
    add_particular_tests_for_scalar<complex<float> >(dir);
    add_particular_tests_for_scalar<complex<double> >(dir);
}

} // end of namespace Operator
} // end of namespace Test
