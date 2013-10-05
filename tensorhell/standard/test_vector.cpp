// ///////////////////////////////////////////////////////////////////////////
// test_vector.cpp by Victor Dods, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_vector.hpp"

#include "tenh/vector.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
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
    u = v(j);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]);

    // expression template assignment
    u = w(j);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], w[i]);
}

template <typename BasedVectorSpace_, typename Scalar_, typename UseArrayType_>
void addition (Context const &context)
{
    typedef Tenh::Vector<BasedVectorSpace_,Scalar_,UseArrayType_> Vector;

    Vector u(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Vector w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        v[i] = i.value();
        w[i] = 3*i.value() + 4;
    }

    u = v + w;
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        assert_eq(u[i], v[i]+w[i]);
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
        static Uint32 const DIM = 3;
        typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIM,X>,Tenh::Basis_c<X> > BasedVectorSpace;
        add_particular_tests<BasedVectorSpace,Scalar_,UseArrayType_>(parent);
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
    Directory *dir = new Directory("Vector", parent);
    add_particular_tests_for_scalar<Sint8>(dir);
    add_particular_tests_for_scalar<Uint32>(dir);
    add_particular_tests_for_scalar<Sint64>(dir);
    add_particular_tests_for_scalar<float>(dir);
    add_particular_tests_for_scalar<double>(dir);
    add_particular_tests_for_scalar<complex<float> >(dir);
    add_particular_tests_for_scalar<complex<double> >(dir);
}

} // end of namespace Vector
} // end of namespace Test
