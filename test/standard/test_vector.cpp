// ///////////////////////////////////////////////////////////////////////////
// test_vector.cpp by Victor Dods, created 2013/04/02
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_vector.hpp"

#include <iomanip>
#include <sstream>
#include <vector>

#include "tenh/vector.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Vector {

template <typename Scalar, Uint32 DIM>
void constructor_without_initialization (Context const &context)
{
    typedef Tenh::Vector_t<Scalar,DIM> Vector;
    
    Vector v(Tenh::Static<>::WITHOUT_INITIALIZATION);
}

template <typename Scalar, Uint32 DIM>
void constructor_fill_with (Context const &context)
{
    typedef Tenh::Vector_t<Scalar,DIM> Vector;
    
    Scalar fill = context.DataAs<Scalar>();
    Vector v(fill);
    for (typename Vector::Index i; i.is_not_at_end(); ++i)
    {
        assert_eq(v[i], fill);
    }
}

#define FORMAT(x) static_cast<ostringstream &>(ostringstream().flush() << x).str()

template <typename Scalar, Uint32 DIM>
void add_particular_tests (Directory *parent)
{
    Directory *vector = new Directory(FORMAT("Vector_t<" << Tenh::TypeStringOf_t<Scalar>::eval() << ',' << DIM << '>'), parent);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(vector, "constructor_without_initialization", constructor_without_initialization<Scalar,DIM>, RESULT_NO_ERROR);
    LVD_ADD_NAMED_TEST_CASE_FUNCTION(vector, "constructor_fill_with", constructor_fill_with<Scalar,DIM>, new Context::Data<Scalar>(42), RESULT_NO_ERROR);
}

template <typename Scalar>
void add_particular_tests_for_scalar (Directory *parent)
{
    add_particular_tests<Scalar,1>(parent);
    add_particular_tests<Scalar,2>(parent);
    add_particular_tests<Scalar,3>(parent);
    add_particular_tests<Scalar,4>(parent);
    add_particular_tests<Scalar,10>(parent);
    add_particular_tests<Scalar,100>(parent);
}

void AddTests (Directory *parent)
{
    add_particular_tests_for_scalar<float>(parent);
    add_particular_tests_for_scalar<double>(parent);
    // TODO: complex scalar (?) -- this would use the standard C++ complex<> type
}

} // end of namespace Vector
} // end of namespace Test
