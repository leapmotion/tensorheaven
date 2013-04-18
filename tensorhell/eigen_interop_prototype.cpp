#include <iostream>
#include <sstream>
#include <string>

#include "tenh/core.hpp" // everything should include this
#include "tenh/multiindex.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/interop/eigen_ldlt.hpp"
#include "tenh/interop/eigen_selfadjointeigendecomp.hpp"
#include "tenh/interop/eigen_svd.hpp"
#include "tenh/meta/typelist.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/meta/typetuple.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2antisymmetric.hpp"
#include "tenh/tensor2diagonal.hpp"
#include "tenh/tensor2symmetric.hpp"
#include "tenh/vector.hpp"

#include "Eigen/Core"
#include "Eigen/Eigenvalues"
#include "Eigen/SVD"

using namespace Tenh;

template <Uint32 ROWS, Uint32 COLS>
void test_SVD ()
{
    std::cout << "test_SVD<" << ROWS << ',' << COLS << ">\n";
    std::cout << "*************************************************************************\n";

    typedef Vector_t<float,ROWS> V1;
    typedef Vector_t<float,COLS> V2;
    typedef Tensor2_t<V1,V2> Tensor2;
    typedef Tensor2_t<V1,V1> U;
    typedef Tensor2Diagonal_t<V1,V2> S;
    typedef Tensor2_t<V2,V2> V;

    Tensor2 t(Static<WithoutInitialization>::SINGLETON);
    for (typename Tensor2::Index i; i.is_not_at_end(); ++i)
        t[i] = i.value() + 1;

    U u(Static<WithoutInitialization>::SINGLETON);
    S s(Static<WithoutInitialization>::SINGLETON);
    V v(Static<WithoutInitialization>::SINGLETON);
    SVD_of_Tensor2(t, u, s, v);
    std::cout << FORMAT_VALUE(u) << '\n';
    std::cout << FORMAT_VALUE(s) << '\n';
    std::cout << FORMAT_VALUE(v) << '\n';

    Tensor2 x(Static<WithoutInitialization>::SINGLETON);
    TypedIndex_t<V1,'i'> i;
    TypedIndex_t<V1,'j'> j;
    TypedIndex_t<V1,'k'> k;
    TypedIndex_t<V2,'p'> p;
    TypedIndex_t<V2,'q'> q;
    TypedIndex_t<V2,'r'> r;
    std::cout << FORMAT_VALUE(u(i|j)*u(k|j)) << '\n';
    std::cout << FORMAT_VALUE(u(i|j)*u(i|k)) << '\n';
    x(i|q) = u(i|j)*s(j|p)*v(q|p);
    std::cout << FORMAT_VALUE(x) << '\n';

    std::cout << "after clamping singular values to 1:\n";
    s = S(1);
    x(i|q) = u(i|j)*s(j|p)*v(q|p);
    std::cout << FORMAT_VALUE(s) << '\n';
    std::cout << FORMAT_VALUE(x) << '\n';
    std::cout << FORMAT_VALUE(x(i|p)*x(j|p)) << '\n';

    std::cout << '\n';
}

template <Uint32 DIM>
void test_diagonalization ()
{
    std::cout << "test_diagonalization<" << DIM << ">\n";
    std::cout << "*******************************************************\n";

    typedef Vector_t<float,DIM> V;
    typedef Tensor2_t<typename V::WithStandardEuclideanBasis,V> T;
    typedef Tensor2Diagonal_t<typename V::WithStandardEuclideanBasis,typename V::WithStandardEuclideanBasis> D;
    typedef Tensor2Symmetric_t<V> S;

    S s(Static<WithoutInitialization>::SINGLETON);
    for (typename S::Index i; i.is_not_at_end(); ++i)
        s[i] = i.value() + 1;

    D eval(Static<WithoutInitialization>::SINGLETON);
    T evec(Static<WithoutInitialization>::SINGLETON);
    diagonalize_Tensor2Symmetric(s, eval, evec);
    std::cout << FORMAT_VALUE(evec) << '\n';
    std::cout << FORMAT_VALUE(eval) << '\n';

    TypedIndex_t<V,'i'> i;
    TypedIndex_t<V,'j'> j;
    TypedIndex_t<V,'k'> k;
    TypedIndex_t<V,'l'> l;
    std::cout << FORMAT_VALUE(evec(i|j)*eval(j|k)*evec(l|k)) << '\n';

    std::cout << '\n';
}

template <Uint32 DIM>
void test_LDLT ()
{
    std::cout << "test_LDLT<" << DIM << ">\n";
    std::cout << "*******************************************************\n";

    typedef Vector_t<float,DIM> V;
    typedef Tensor2_t<V,V> T;
    typedef Tensor2Diagonal_t<V,V> D;
    typedef Tensor2Symmetric_t<V> S;

    S s(Static<WithoutInitialization>::SINGLETON);
    for (typename S::Index i; i.is_not_at_end(); ++i)
        s[i] = i.value() + 1;

    D d(Static<WithoutInitialization>::SINGLETON);
    T l(Static<WithoutInitialization>::SINGLETON);

    LDLT_Tensor2Symmetric(s, d, l);

    std::cout << FORMAT_VALUE(s) << '\n';
    std::cout << FORMAT_VALUE(d) << '\n';
    std::cout << FORMAT_VALUE(l) << '\n';
    TypedIndex_t<V,'h'> h;
    TypedIndex_t<V,'i'> i;
    TypedIndex_t<V,'j'> j;
    TypedIndex_t<V,'k'> k;
    std::cout << FORMAT_VALUE(l(h|i)*d(i|j)*l(k|j)) << '\n';
}

int main (int argc, char **argv)
{
    test_SVD<1,1>();
//     test_SVD<1,3>();
    test_SVD<2,2>();
    test_SVD<3,4>();

    test_diagonalization<1>();
    test_diagonalization<3>();
    test_diagonalization<10>();

    test_LDLT<2>();
    test_LDLT<3>();
    test_LDLT<8>();

    return 0;
}
