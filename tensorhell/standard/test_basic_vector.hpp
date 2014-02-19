// ///////////////////////////////////////////////////////////////////////////
// test_basic_vector.hpp by Victor Dods, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(TEST_BASIC_VECTOR_HPP_)
#define TEST_BASIC_VECTOR_HPP_

#include "test.hpp"

#include "tenh/basic.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

namespace Lvd {
namespace TestSystem {

struct Directory;

} // end of namespace TestSystem
} // end of namespace Lvd

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Basic {
namespace Vector {

struct X { static std::string type_as_string () { return "X"; } };

template <typename Vector_>
void constructor_without_initialization (Context const &context)
{
    Vector_ v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
}

template <typename Vector_>
void constructor_fill_with (Context const &context)
{
    typedef typename Vector_::Scalar Scalar;
    Scalar fill = context.DataAs<Scalar>();
    Vector_ v(Tenh::fill_with(fill));
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(v[i], fill);
}

template <typename Vector_>
void assignment (Context const &context)
{
//     typedef typename Vector_::Scalar Scalar;
    Vector_ u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ const v(Tenh::fill_with(3));
    Vector_ w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        w[i] = 3*i.value() + 4;

    // plain non-expression template assignment
    u = v;
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]);

    // plain non-expression template assignment
    u = w;
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], w[i]);

    // expression template assignment
    Tenh::AbstractIndex_c<'j'> j;
    u = Tenh::reindexable(v(j));
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]);

    // expression template assignment
    u = Tenh::reindexable(w(j));
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], w[i]);
}

// NOTE: All the bullshit casts to Scalar are necessary because the operands
// *may* be promoted to a larger type (e.g. char -> int).  This is an idiotic
// part of the C standard.

template <typename Vector_>
void addition (Context const &context)
{
    typedef typename Vector_::Scalar Scalar;
    Vector_ u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ x(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ y(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        v[i] = i.value();
        w[i] = 3*i.value() + 4;
        x[i] = 8*i.value() - 3;
        y[i] = i.value()*i.value();
    }

    u = v + w;
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(v[i]+w[i]));

    u = v + (w + x);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(v[i]+Scalar(w[i]+x[i])));

    u = (v + w) + x;
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(Scalar(v[i]+w[i])+x[i]));

    u = (v + w) + (x + y);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(Scalar(v[i]+w[i])+Scalar(x[i]+y[i])));
}

template <typename Vector_>
void subtraction (Context const &context)
{
    typedef typename Vector_::Scalar Scalar;
    Vector_ u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ x(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ y(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        v[i] = i.value();
        w[i] = 3*i.value() + 4;
        x[i] = 8*i.value() - 3;
        y[i] = i.value()*i.value();
    }

    u = v - w;
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(v[i]-w[i]));

    u = v - (w - x);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(v[i]-Scalar(w[i]-x[i])));

    u = (v - w) - x;
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(Scalar(v[i]-w[i])-x[i]));

    u = (v - w) - (x - y);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(Scalar(v[i]-w[i])-Scalar(x[i]-y[i])));
}

template <typename Vector_>
void scalar_multiplication (Context const &context)
{
    typedef typename Vector_::Scalar Scalar;
    Vector_ u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        v[i] = i.value();

    u = v * Scalar(3.14);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(v[i]*Scalar(3.14)));

    u = Scalar(3.14) * v;
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(v[i]*Scalar(3.14)));

    u = v / 3;
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(v[i]/Scalar(3)));

    u = v / Scalar(3.14);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(v[i]/Scalar(3.14)));
}

template <typename Vector_>
void negation (Context const &context)
{
    typedef typename Vector_::Scalar Scalar;
    Vector_ u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        v[i] = i.value();

    u = -v;
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], Scalar(-v[i]));
}

template <typename Vector_>
void natural_pairing (Context const &context)
{
    typedef typename Vector_::Scalar Scalar;
    typedef typename Tenh::DualOf_f<Vector_>::T Covector;

    Covector a(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Covector b(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector_ w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        a[i] = i.value();
        b[i] = 2*i.value() + 17;
        v[i] = 3*i.value() + 4;
        w[i] = i.value()*i.value();
    }

    // covector on left

    {
        Scalar actual_result = a * v;
        Scalar expected_result(0);
        for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar(a[i] * v[i]);
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar actual_result = a * (v + w);
        Scalar expected_result(0);
        for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar(a[i] * Scalar(v[i] + w[i]));
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar actual_result = (a + b) * v;
        Scalar expected_result(0);
        for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar(Scalar(a[i] + b[i]) * v[i]);
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar actual_result = (a + b) * (v + w);
        Scalar expected_result(0);
        for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar(Scalar(a[i] + b[i]) * Scalar(v[i] + w[i]));
        assert_eq(actual_result, expected_result);
    }

    // vector on left

    {
        Scalar actual_result = v * a;
        Scalar expected_result(0);
        for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar(v[i] * a[i]);
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar actual_result = (v + w) * a;
        Scalar expected_result(0);
        for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar(Scalar(v[i] + w[i]) * a[i]);
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar actual_result = v * (a + b);
        Scalar expected_result(0);
        for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar(v[i] * Scalar(a[i] + b[i]));
        assert_eq(actual_result, expected_result);
    }

    {
        Scalar actual_result = (v + w) * (a + b);
        Scalar expected_result(0);
        for (typename Vector_::ComponentIndex i; i.is_not_at_end(); ++i)
            expected_result += Scalar(Scalar(v[i] + w[i]) * Scalar(a[i] + b[i]));
        assert_eq(actual_result, expected_result);
    }
}

template <typename Vector_>
void add_particular_tests (Directory &parent)
{
    typedef typename Vector_::Scalar Scalar;
    Directory &dir = parent.GetSubDirectory(Tenh::type_string_of<Vector_>());
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "constructor_without_initialization", constructor_without_initialization<Vector_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "constructor_fill_with", constructor_fill_with<Vector_>, new Context::Data<Scalar>(42), RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "assignment", assignment<Vector_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "addition", addition<Vector_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "subtraction", subtraction<Vector_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "scalar_multiplication", scalar_multiplication<Vector_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "negation", negation<Vector_>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(dir, "natural_pairing", natural_pairing<Vector_>, RESULT_NO_ERROR);
}

template <typename Scalar_, typename UseArrayType_>
void add_particular_tests_for_scalar_and_use_array_type_Vector (Directory &parent)
{
    {
        static Uint32 const DIM = 0;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,X>,Tenh::Basis_c<X>> BasedVectorSpace;
        typedef Tenh::Vector<BasedVectorSpace,Scalar_,UseArrayType_> V;
        add_particular_tests<V>(parent);
    }
    {
        static Uint32 const DIM = 1;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,X>,Tenh::Basis_c<X>> BasedVectorSpace;
        typedef Tenh::Vector<BasedVectorSpace,Scalar_,UseArrayType_> V;
        add_particular_tests<V>(parent);
    }
    {
        static Uint32 const DIM = 3;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,X>,Tenh::Basis_c<X>> BasedVectorSpace;
        typedef Tenh::Vector<BasedVectorSpace,Scalar_,UseArrayType_> V;
        add_particular_tests<V>(parent);
    }
}

template <typename Scalar_, typename UseArrayType_>
void add_particular_tests_for_scalar_and_use_array_type_Vec (Directory &parent)
{
    {
        static Uint32 const DIM = 0;
        typedef Tenh::Vec<DIM,Scalar_,Tenh::OrthonormalBasis_c<Tenh::Generic>,Tenh::Generic,UseArrayType_> V;
        add_particular_tests<V>(parent);
    }
    {
        static Uint32 const DIM = 1;
        typedef Tenh::Vec<DIM,Scalar_,Tenh::OrthonormalBasis_c<Tenh::Generic>,Tenh::Generic,UseArrayType_> V;
        add_particular_tests<V>(parent);
    }
    {
        static Uint32 const DIM = 3;
        typedef Tenh::Vec<DIM,Scalar_,Tenh::OrthonormalBasis_c<Tenh::Generic>,Tenh::Generic,UseArrayType_> V;
        add_particular_tests<V>(parent);
    }
}

template <typename Scalar_>
void add_particular_tests_for_scalar_Vector (Directory &parent)
{
    add_particular_tests_for_scalar_and_use_array_type_Vector<Scalar_,Tenh::UseMemberArray_t<Tenh::ComponentsAreConst::FALSE>>(parent);
}

template <typename Scalar_>
void add_particular_tests_for_scalar_Vec (Directory &parent)
{
    add_particular_tests_for_scalar_and_use_array_type_Vec<Scalar_,Tenh::UseMemberArray_t<Tenh::ComponentsAreConst::FALSE>>(parent);
}

void AddTests0 (Lvd::TestSystem::Directory &parent);
void AddTests1 (Lvd::TestSystem::Directory &parent);
void AddTests2 (Lvd::TestSystem::Directory &parent);
void AddTests3 (Lvd::TestSystem::Directory &parent);
void AddTests4 (Lvd::TestSystem::Directory &parent);
void AddTests5 (Lvd::TestSystem::Directory &parent);

} // end of namespace Vector
} // end of namespace Basic
} // end of namespace Test

#endif // !defined(TEST_BASIC_VECTOR_HPP_)
