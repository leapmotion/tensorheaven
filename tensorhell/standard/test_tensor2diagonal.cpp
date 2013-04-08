// ///////////////////////////////////////////////////////////////////////////
// test_tensor2diagonal.cpp by Ted Nitz, created 2013/04/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "test_tensor2diagonal.hpp"

#include <utility> // for std::pair
#include <complex>

#include "tenh/meta/typelist.hpp"
#include "tenh/multiindex.hpp"
#include "tenh/tensor2diagonal.hpp"
#include "tenh/vector.hpp"

// this is included last because it redefines the `assert` macro,
// which would be bad for the above includes.
#include "lvd_testsystem.hpp"

using namespace Lvd;
using namespace std;
using namespace TestSystem;

namespace Test {
namespace Tensor2Diagonal {

    template <typename Type>
    void constructor_without_initialization (Context const &context)
    {
        Type d(Tenh::Static<>::WITHOUT_INITIALIZATION);
    }

    template <typename Type>
    void constructor_fill_with (Context const &context)
    {
        typedef typename Type::Scalar Scalar;
        
        Scalar fill = context.DataAs<Scalar>();
        Type d(fill);
        
        for (typename Type::Index i; i.is_not_at_end(); ++i)
        {
            assert_eq(d[i], fill);
        }
    }
    
    template <typename Type>
    void test_read_access (Context const &context)
    {
        typedef typename Type::Scalar Scalar;
        typedef typename Type::Factor1 V1;
        typedef typename Type::Factor2 V2;
        
        std::pair<Type,Scalar> p = context.DataAs<std::pair<Type,Scalar> >();
        Type t = p.first;
        Scalar s = p.second;
        
        for (typename V1::Index i; i.is_not_at_end(); ++i)
        {
            for (typename V2::Index j; j.is_not_at_end(); ++j)
            {
                Tenh::MultiIndex_t<Tenh::TypeList_t<typename V1::Index,
                    Tenh::TypeList_t<typename V2::Index> > > ci(i,j);
                assert_eq(t[ci], t.component(i,j));
                if(i.value() == j.value())
                {
                    assert_eq(t[ci], s);
                    assert_eq(t.scalar_factor_for_component(ci), Scalar(1));
                    assert(t.component_corresponds_to_memory_location(ci));
                }
                else
                {
                    assert_eq(t[ci], Scalar(0));
                        assert_eq(t.scalar_factor_for_component(ci), Scalar(0));
                    assert(!t.component_corresponds_to_memory_location(ci));
                }
            }
        }
    }
    
    template <typename Type>
    void test_write_on_diagonal (Context const &context)
    {
        typedef typename Type::Scalar Scalar;
        typedef typename Type::Factor1 V1;
        typedef typename Type::Factor2 V2;
        
        std::pair<Type,Scalar> p = context.DataAs<std::pair<Type,Scalar> >();
        Type t = p.first;
        
        typename V1::Index i;
        typename V2::Index j;
        
        t.set_component(i, j, Scalar(1));
        assert_eq(t.component(i,j), Scalar(1));

        t.set_component(i, j, Scalar(2));
        assert_eq(t.component(i,j), Scalar(2));
    }

    template <typename Type>
    void test_write_off_diagonal (Context const &context)
    {
        typedef typename Type::Scalar Scalar;
        typedef typename Type::Factor1 V1;
        typedef typename Type::Factor2 V2;
        
        std::pair<Type,Scalar> p = context.DataAs<std::pair<Type,Scalar> >();
        Type t = p.first;
        
        typename V1::Index i;
        typename V2::Index j;

        if (V1::DIM > 1)
        {
            ++i;
        }
        else if (V2::DIM > 1)
        {
            ++j;
        }
        else
        {
            throw std::invalid_argument("No off diagonal components.");
        }
        
        t.set_component(i, j, Scalar(1));
        assert_eq(t.component(i,j), Scalar(1));
    }

    template <typename Scalar, Uint32 ROWS, Uint32 COLS>
    void add_particular_tests (Directory *parent)
    {
        typedef Tenh::Vector_t<Scalar,ROWS> V1;
        typedef Tenh::Vector_t<Scalar,COLS> V2;
        typedef Tenh::Tensor2Diagonal_t<V1, V2> Tensor2Diagonal;
        
        Directory *tensor2diagonal 
            = new Directory(Tenh::TypeStringOf_t<Tenh::Tensor2Diagonal_t<V1,V2> >::eval(), parent);
        
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2diagonal, "constructor_without_initialization", 
            constructor_without_initialization<Tensor2Diagonal>, RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2diagonal, "constructor_fill_with",
            constructor_fill_with<Tensor2Diagonal>, new Context::Data<Scalar>(42), RESULT_NO_ERROR);
        
        Scalar s(1);
        Tensor2Diagonal t(s);
        std::pair<Tensor2Diagonal,Scalar> p(t,s);
        
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2diagonal, "test_read_access", test_read_access<Tensor2Diagonal>,
            new Context::Data<std::pair<Tensor2Diagonal,Scalar> >(p), RESULT_NO_ERROR);
        LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2diagonal, "test_write_on_diagonal",
            test_write_on_diagonal<Tensor2Diagonal>, new Context::Data<std::pair<Tensor2Diagonal,Scalar> >(p),
            RESULT_NO_ERROR);
        // if (ROWS > 1 || COLS > 1)
        // {
        //     LVD_ADD_NAMED_TEST_CASE_FUNCTION(tensor2diagonal, "test_write_off_diagonal",
        //         test_write_off_diagonal<Tensor2Diagonal>, new Context::Data<std::pair<Tensor2Diagonal,Scalar> >(p),
        //         RESULT_UNCAUGHT_EXCEPTION, STAGE_TEST_BODY);        
        // }        
    }
    
    template <typename Scalar>
    void add_particular_tests_for_scalar (Directory *parent)
    {
        add_particular_tests<Scalar,1,1>(parent);
        add_particular_tests<Scalar,1,5>(parent);
        add_particular_tests<Scalar,8,1>(parent);
        add_particular_tests<Scalar,100,100>(parent);
    }

    void AddTests (Directory *parent)
    {
        Directory *tensor2diagonal = new Directory("Tensor2Diagonal_t", parent);
        
        add_particular_tests_for_scalar<int>(tensor2diagonal);
        add_particular_tests_for_scalar<float>(tensor2diagonal);
        add_particular_tests_for_scalar<double>(tensor2diagonal);
        add_particular_tests_for_scalar<complex<float> >(tensor2diagonal);
        add_particular_tests_for_scalar<complex<double> >(tensor2diagonal);
    }

} // end of namespace Tensor2Diagonal
} // end of namespace Test