#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 15
// ///////////////////////////////////////////////////////////////////////////////////////////

struct IdB { static std::string type_as_string (bool verbose) { return "B"; } };
struct IdC { static std::string type_as_string (bool verbose) { return "C"; } };

void test_terse_strings ()
{
    std::cout << "testing terse strings\n";

    std::cout << FORMAT_VALUE(terse_string_of(AbstractIndex_c<'i'>())) << '\n';

    std::cout << FORMAT_VALUE(terse_string_of(basis(IdX()))) << '\n';
    std::cout << FORMAT_VALUE(terse_string_of(generic_basis())) << '\n';
    std::cout << FORMAT_VALUE(terse_string_of(o_n_basis(IdX()))) << '\n';
    std::cout << FORMAT_VALUE(terse_string_of(generic_o_n_basis())) << '\n';

    {
        VectorSpace_c<RealField,3,IdX> X;
        VectorSpace_c<RealField,4,IdY> Y;

        std::cout << FORMAT_VALUE(terse_string_of(X)) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(Y)) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(dual(Y))) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(dual(dual(Y)))) << '\n';
    }
    std::cout << '\n';

    {
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,Basis_c<IdB> > BX;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdY>,Basis_c<IdC> > BY;
        BX X;
        BY Y;
        std::cout << FORMAT_VALUE(terse_string_of(X)) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(dual(X))) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(X*Y)) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(diag2(X,Y))) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(diag2(X*Y))) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(scalar2(X,Y))) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(scalar2(X*Y))) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(X*Y*Y*X*X)) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(tensor<4>(X))) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(ext<4>(X))) << '\n';
        std::cout << FORMAT_VALUE(terse_string_of(sym<4>(X))) << '\n';

        std::cout << FORMAT_VALUE(terse_string_of(X+Y+Y+X+X)) << '\n';

        {
            LinearEmbedding_f<ExteriorPowerOfBasedVectorSpace_c<3,BX>,
                              TensorPowerOfBasedVectorSpace_f<3,BX>::T,
                              double,
                              NaturalEmbedding,
                              DISABLE_EXCEPTIONS>::T linear_embedding;
            std::cout << FORMAT_VALUE(terse_string_of(linear_embedding)) << '\n';
        }
    }
    std::cout << '\n';
}
