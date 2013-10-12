// ///////////////////////////////////////////////////////////////////////////
// test_basic_vector.cpp by Victor Dods, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_basic_vector.hpp"

#include "tenh/basic.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Basic {
namespace Vector {

struct X { static std::string type_as_string () { return "X"; } };

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
void constructor_without_initialization (Context const &context)
{
    typedef Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vector;

    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
void constructor_fill_with (Context const &context)
{
    typedef Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vector;

    Scalar_ fill = context.DataAs<Scalar_>();
    Vector v(fill);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(v[i], fill);
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
void assignment (Context const &context)
{
    typedef Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vector;

    Vector u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector const v(3.0f);
    Vector w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        w[i] = 3*i.value() + 4;

    // plain non-expression template assignment
    u = v;
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]);

    // plain non-expression template assignment
    u = w;
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], w[i]);

    // expression template assignment
    Tenh::AbstractIndex_c<'j'> j;
    u = Tenh::reindexable(v(j));
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]);

    // expression template assignment
    u = Tenh::reindexable(w(j));
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], w[i]);
}

// NOTE: All the bullshit casts to Scalar_ are necessary because the operands
// *may* be promoted to a larger type (e.g. char -> int).  This is an idiotic
// part of the C standard.

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
void addition (Context const &context)
{
    typedef Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vector;

    Vector u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector x(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector y(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        v[i] = i.value();
        w[i] = 3*i.value() + 4;
        x[i] = 8*i.value() - 3;
        y[i] = i.value()*i.value();
    }

    u = v + w;
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(v[i]+w[i]));

    u = v + (w + x);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(v[i]+Scalar_(w[i]+x[i])));

    u = (v + w) + x;
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(Scalar_(v[i]+w[i])+x[i]));

    u = (v + w) + (x + y);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(Scalar_(v[i]+w[i])+Scalar_(x[i]+y[i])));
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
void subtraction (Context const &context)
{
    typedef Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vector;

    Vector u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector x(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector y(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        v[i] = i.value();
        w[i] = 3*i.value() + 4;
        x[i] = 8*i.value() - 3;
        y[i] = i.value()*i.value();
    }

    u = v - w;
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(v[i]-w[i]));

    u = v - (w - x);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(v[i]-Scalar_(w[i]-x[i])));

    u = (v - w) - x;
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(Scalar_(v[i]-w[i])-x[i]));

    u = (v - w) - (x - y);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(Scalar_(v[i]-w[i])-Scalar_(x[i]-y[i])));
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
void scalar_multiplication (Context const &context)
{
    typedef Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vector;

    Vector u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        v[i] = i.value();

    u = v * Scalar_(3.14);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(v[i]*Scalar_(3.14)));

    u = Scalar_(3.14) * v;
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(v[i]*Scalar_(3.14)));

    u = v / 3;
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(v[i]/Scalar_(3)));

    u = v / Scalar_(3.14);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(v[i]/Scalar_(3.14)));
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
void negation (Context const &context)
{
    typedef Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vector;

    Vector u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        v[i] = i.value();

    u = -v;
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar_(-v[i]));
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
void natural_pairing (Context const &context)
{
    typedef Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vector;
    typedef typename Tenh::DualOf_f<Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> >::T Covector;

    Covector a(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Covector b(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
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
        for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar_(a[i] * v[i]);
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar_ actual_result = a * (v + w);
        Scalar_ expected_result(0);
        for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar_(a[i] * Scalar_(v[i] + w[i]));
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar_ actual_result = (a + b) * v;
        Scalar_ expected_result(0);
        for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar_(Scalar_(a[i] + b[i]) * v[i]);
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar_ actual_result = (a + b) * (v + w);
        Scalar_ expected_result(0);
        for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar_(Scalar_(a[i] + b[i]) * Scalar_(v[i] + w[i]));
        assert_eq(actual_result, expected_result);
    }

    // vector on left

    {
        Scalar_ actual_result = v * a;
        Scalar_ expected_result(0);
        for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar_(v[i] * a[i]);
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar_ actual_result = (v + w) * a;
        Scalar_ expected_result(0);
        for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar_(Scalar_(v[i] + w[i]) * a[i]);
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar_ actual_result = v * (a + b);
        Scalar_ expected_result(0);
        for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar_(v[i] * Scalar_(a[i] + b[i]));
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar_ actual_result = (v + w) * (a + b);
        Scalar_ expected_result(0);
        for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar_(Scalar_(v[i] + w[i]) * Scalar_(a[i] + b[i]));
        assert_eq(actual_result, expected_result);
    }
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
void add_particular_tests (Directory *parent)
{
    typedef Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vector;
    Directory *dir = new Directory(Tenh::type_string_of<Vector>(), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "constructor_without_initialization", constructor_without_initialization<BasedVectorSpace_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "constructor_fill_with", constructor_fill_with<BasedVectorSpace_,Scalar_,UseArrayType_>, new Context::Data<Scalar_>(42), RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "assignment", assignment<BasedVectorSpace_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "addition", addition<BasedVectorSpace_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "subtraction", subtraction<BasedVectorSpace_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "scalar_multiplication", scalar_multiplication<BasedVectorSpace_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "negation", negation<BasedVectorSpace_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "natural_pairing", natural_pairing<BasedVectorSpace_,Scalar_,UseArrayType_>, RESULT_NO_ERROR);
}

template <typename Scalar_, typename UseArrayType_>
void add_particular_tests_for_scalar_and_use_array_type (Directory *parent)
{
    {
        static Uint32 const DIM = 0;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,X>,Tenh::Basis_c<X> > BasedVectorSpace;
        add_particular_tests<BasedVectorSpace,Scalar_,UseArrayType_>(parent);
    }
    {
        static Uint32 const DIM = 1;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,X>,Tenh::Basis_c<X> > BasedVectorSpace;
        add_particular_tests<BasedVectorSpace,Scalar_,UseArrayType_>(parent);
    }
    {
        static Uint32 const DIM = 3;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,X>,Tenh::Basis_c<X> > BasedVectorSpace;
        add_particular_tests<BasedVectorSpace,Scalar_,UseArrayType_>(parent);
    }
}

template <typename Scalar_>
void add_particular_tests_for_scalar (Directory *parent)
{
    add_particular_tests_for_scalar_and_use_array_type<Scalar_,Tenh::UseMemberArray>(parent);
}

void AddTests (Directory *parent)
{
    Directory *dir = new Directory("Vector", parent);
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
    add_particular_tests_for_scalar<complex<float> >(dir);
    add_particular_tests_for_scalar<complex<double> >(dir);
}

} // end of namespace Vector
} // end of namespace Basic
} // end of namespace Test
