#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 3
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_multiindex_sort ()
{
    typedef ComponentIndex_t<5> C;
    typedef UniformTypleOfLength_f<3,C>::T IndexTyple;
    typedef MultiIndex_t<IndexTyple> MultiIndex;

    MultiIndex m; // default constructor uses default constructor of each component
    m.el<0>() = C(3);
    m.el<1>() = C(1);
    //m.el<2>() = C(4);
    m.index(2) = C(4);
    std::cout << FORMAT_VALUE(m) << '\n';
    sort<std::less<Uint32> >(m);
    std::cout << "after sort: " << FORMAT_VALUE(m) << '\n';

    std::cout << '\n' << '\n';
}

void test_multiindex_sort_2 ()
{
    typedef ComponentIndex_t<5> C;
    typedef UniformTypleOfLength_f<1,C>::T IndexTyple;
    typedef MultiIndex_t<IndexTyple> MultiIndex;

    MultiIndex m; // default constructor uses default constructor of each component
    m.index(0) = C(4);
    std::cout << FORMAT_VALUE(m) << '\n';
    sort<std::less<Uint32> >(m);
    std::cout << "after sort: " << FORMAT_VALUE(m) << '\n';

    std::cout << '\n' << '\n';
}

void test_zero_dimensional_vector_space ()
{
    // testing 0-dimensional vector spaces
    typedef VectorSpace_c<RealField,0,X> VSX;
    typedef Basis_c<X> BasisX;
    typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
    typedef ImplementationOf_t<BasedX,float> Vector;
    Vector v(fill_with(0));
    std::cout << FORMAT_VALUE(v) << '\n';

    std::cout << '\n' << '\n';
}

void test_Diagonal2Tensor ()
{
    typedef VectorSpace_c<RealField,4,X> VSX;
    typedef Basis_c<X> BasisX;
    typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<DualOf_f<BasedX>::T,DualOf_f<BasedX>::T> Diag;
    typedef ImplementationOf_t<Diag,float> D;
    D d(tuple(2.0f, 1.0f, -3.0f, 5.0f));
    std::cout << FORMAT_VALUE(d) << '\n';

    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    std::cout << FORMAT_VALUE(d(i).split(i,j*k)) << '\n';
    std::cout << FORMAT_VALUE(d.split(j*k)) << '\n';

    typedef ImplementationOf_t<BasedX,float> V;
    V u(tuple(1.0f, 2.0f, 10.0f, -2.0f));
    std::cout << FORMAT_VALUE(u) << '\n';
    std::cout << FORMAT_VALUE(u(j) * d(i).split(i,j*k) * u(k)) << '\n';
    std::cout << FORMAT_VALUE(u(j) * d.split(j*k) * u(k)) << '\n';

    // do computation by hand to check it
    float total = 0.0f;
    for (ComponentIndex_t<VSX::DIMENSION> it; it.is_not_at_end(); ++it)
        total += u[it] * d(i).split(i,j*k)[D::MultiIndex(it,it)] * u[it];
    std::cout << "computation done by hand: " << total << '\n';

    V v(tuple(-1.0f, -1.0f, 2.0f, 2.0f));
    std::cout << FORMAT_VALUE(v) << '\n';
    std::cout << FORMAT_VALUE(u(j) * d(i).split(i,j*k) * v(k)) << '\n';
    std::cout << FORMAT_VALUE(u(j) * d.split(j*k) * v(k)) << '\n';

    // do computation by hand to check it
    total = 0.0f;
    for (ComponentIndex_t<VSX::DIMENSION> it; it.is_not_at_end(); ++it)
        total += u[it] * d(i).split(i,j*k)[D::MultiIndex(it,it)] * v[it];
    std::cout << "computation done by hand: " << total << '\n';

    std::cout << '\n' << '\n';
}

