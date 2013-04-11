// ///////////////////////////////////////////////////////////////////////////
// test_array.cpp by Victor Dods, created 2013/04/07
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_array.hpp"

#include <complex>

#include "tenh/array.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Array {

template <typename Component, Uint32 DIM>
void constructor_without_initialization (Context const &context)
{
    typedef Tenh::Array_t<Component,DIM> Array;

    Array v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
}

template <typename Component, Uint32 DIM>
void constructor_fill_with (Context const &context)
{
    typedef Tenh::Array_t<Component,DIM> Array;

    Component fill = context.DataAs<Component>();
    Array v(fill);
    for (typename Array::Index i; i.is_not_at_end(); ++i)
    {
        assert_eq(v[i], fill);
    }
}

template <typename Component>
void check_filled_values (Context const &context)
{
    typedef Tenh::Array_t<Component,4> Array;

    Array a(0,2,4,6);
    for (typename Array::Index i; i.is_not_at_end(); ++i)
    {
        assert_eq(a[i], Component(2*i.value()));
    }
}

template <typename Component, Uint32 DIM>
void add_particular_tests (Directory *parent)
{
    Directory *array = new Directory(FORMAT("Array_t<" << Tenh::TypeStringOf_t<Component>::eval() << ',' << DIM << '>'), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(array, "constructor_without_initialization", constructor_without_initialization<Component,DIM>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(array, "constructor_fill_with", constructor_fill_with<Component,DIM>, new Context::Data<Component>(42), RESULT_NO_ERROR);
}

template <typename Component>
void add_particular_tests_for_scalar (Directory *parent)
{
    add_particular_tests<Component,1>(parent);
    add_particular_tests<Component,2>(parent);
    add_particular_tests<Component,100>(parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(parent, FORMAT("check_filled_values<" << Tenh::TypeStringOf_t<Component>::eval() << ">"), check_filled_values<Component>, RESULT_NO_ERROR);
}

void AddTests (Directory *parent)
{
    Directory *array = new Directory("Array_t", parent);
    add_particular_tests_for_scalar<char>(array);
    add_particular_tests_for_scalar<unsigned int>(array);
    add_particular_tests_for_scalar<long>(array);
    add_particular_tests_for_scalar<float>(array);
    add_particular_tests_for_scalar<double>(array);
    add_particular_tests_for_scalar<complex<float> >(array);
    add_particular_tests_for_scalar<complex<double> >(array);
}

} // end of namespace Array
} // end of namespace Test
