#include "adhoc_test.hpp"

#include "tenh/implementation/scalar2tensor.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 2
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_scalar2tensor ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic>> B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Generic>,Basis_c<Generic>> B4;
    typedef Scalar2TensorProductOfBasedVectorSpaces_c<B3,B4> Scalar2;

    typedef double Scalar;
    typedef ImplementationOf_t<Scalar2,Scalar> Sc;

    Sc sc(fill_with(3));
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    // AbstractIndex_c<'k'> k;
    std::cout << "testing scalar 2-tensors\n";
    std::cout << FORMAT_VALUE(sc) << '\n';
    std::cout << FORMAT_VALUE(sc.split(i*j)) << '\n';
    std::cout << '\n';
}

void test_wedge ()
{
    typedef VectorSpace_c<RealField,5,X> VSX;
    typedef Basis_c<X> B;
    typedef BasedVectorSpace_c<VSX,B> BasedX;

    STATIC_ASSERT(HasBasedVectorSpaceStructure_f<BasedX>::V, MUST_BE_BASED_VECTOR_SPACE);

    typedef ExteriorPowerOfBasedVectorSpace_c<3,BasedX> Wedge3_BasedX;
    typedef ExteriorPowerOfBasedVectorSpace_c<2,BasedX> Wedge2_BasedX;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<BasedX,BasedX>> T2_BasedX;

    typedef ImplementationOf_t<Wedge3_BasedX,float> Wedge;

    Wedge w(fill_with(0));
    w[Wedge::ComponentIndex(0, CheckRange::TRUE)] = 6;
    w[Wedge::ComponentIndex(1, CheckRange::TRUE)] = 12;
    w[Wedge::ComponentIndex(2, CheckRange::TRUE)] = 18;
    w[Wedge::ComponentIndex(3, CheckRange::TRUE)] = 24;
    w[Wedge::ComponentIndex(4, CheckRange::TRUE)] = 30;
    w[Wedge::ComponentIndex(5, CheckRange::TRUE)] = 36;
    w[Wedge::ComponentIndex(6, CheckRange::TRUE)] = 42;
    w[Wedge::ComponentIndex(7, CheckRange::TRUE)] = 48;
    w[Wedge::ComponentIndex(8, CheckRange::TRUE)] = 54;
    w[Wedge::ComponentIndex(9, CheckRange::TRUE)] = 60;

    std::cout << FORMAT_VALUE(w) << '\n';

    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'l'> l;
        AbstractIndex_c<'p'> p;
        std::cout << FORMAT_VALUE(w(i).split(i,j*k*l)) << '\n';
        std::cout << FORMAT_VALUE(w.split(j*k*l)) << '\n';
        std::cout << FORMAT_VALUE(w(i).split(i,j*k*l).bundle(j*l,Wedge2_BasedX(),p)) << '\n';
        std::cout << FORMAT_VALUE(w.split(j*k*l).bundle(j*l,Wedge2_BasedX(),p)) << '\n';
        std::cout << FORMAT_VALUE(w(i).split(i,j*k*l).bundle(j*l,T2_BasedX(),p)) << '\n';
        std::cout << FORMAT_VALUE(w.split(j*k*l).bundle(j*l,T2_BasedX(),p)) << '\n';
    }

    std::cout << '\n' << '\n';

    std::cout << "Round-trip convert from ComponentIndex\n";
    Wedge::ComponentIndex i(9);
    std::cout << FORMAT_VALUE(i) << '\n';
    Wedge::MultiIndex m = Wedge::bundle_index_map<Wedge::MultiIndex::IndexTyple, Wedge::ComponentIndex>(i);
    std::cout << FORMAT_VALUE(m) << '\n';
    Wedge::ComponentIndex j = Wedge::vector_index_of(m);
    std::cout << FORMAT_VALUE(j) << "\n\n";

    std::cout << "Round-trip convert from MultiIndex\n";
    Wedge::MultiIndex n(4, 0, 3, CheckRange::TRUE);
    std::cout << FORMAT_VALUE(n) << '\n';
    i = Wedge::vector_index_of(n);
    std::cout << FORMAT_VALUE(i) << '\n';
    n = Wedge::bundle_index_map<Wedge::MultiIndex::IndexTyple, Wedge::ComponentIndex>(i);
    std::cout << FORMAT_VALUE(n) << '\n';
    i = Wedge::vector_index_of(n);
    std::cout << FORMAT_VALUE(i) << "\n\n";
    n = Wedge::bundle_index_map<Wedge::MultiIndex::IndexTyple, Wedge::ComponentIndex>(i);
}

void test_vee ()
{
    typedef VectorSpace_c<RealField,3,X> VSX;
    typedef Basis_c<X> B;
    typedef BasedVectorSpace_c<VSX,B> BasedX;

    STATIC_ASSERT(HasBasedVectorSpaceStructure_f<BasedX>::V, MUST_BE_BASED_VECTOR_SPACE);

    typedef SymmetricPowerOfBasedVectorSpace_c<3,BasedX> Sym3_BasedX;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,BasedX> Sym2_BasedX;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<BasedX,BasedX>> T2_BasedX;

    typedef ImplementationOf_t<Sym3_BasedX,float> Sym;

    Sym w(fill_with(0));
    w[Sym::ComponentIndex(0, CheckRange::TRUE)] = 1;
    w[Sym::ComponentIndex(1, CheckRange::TRUE)] = 6;
    w[Sym::ComponentIndex(2, CheckRange::TRUE)] = 9;
    w[Sym::ComponentIndex(3, CheckRange::TRUE)] = 4;
    w[Sym::ComponentIndex(4, CheckRange::TRUE)] = 15;
    w[Sym::ComponentIndex(5, CheckRange::TRUE)] = 36;
    w[Sym::ComponentIndex(6, CheckRange::TRUE)] = 21;
    w[Sym::ComponentIndex(7, CheckRange::TRUE)] = 24;
    w[Sym::ComponentIndex(8, CheckRange::TRUE)] = 27;
    w[Sym::ComponentIndex(9, CheckRange::TRUE)] = 10;

    std::cout << FORMAT_VALUE(w) << '\n';

    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'l'> l;
        AbstractIndex_c<'p'> p;
        std::cout << FORMAT_VALUE(w(i).split(i,j*k*l)) << '\n';
        std::cout << FORMAT_VALUE(w.split(j*k*l)) << '\n';
        //std::cout << FORMAT_VALUE(w.split(j*k*l)) << '\n'; // this doesn't currently work.
        std::cout << FORMAT_VALUE(w(i).split(i,j*k*l).bundle(j*l,Sym2_BasedX(),p)) << '\n';
        std::cout << FORMAT_VALUE(w.split(j*k*l).bundle(j*l,Sym2_BasedX(),p)) << '\n';
        std::cout << FORMAT_VALUE(w(i).split(i,j*k*l).bundle(j*l,T2_BasedX(),p)) << '\n';
        std::cout << FORMAT_VALUE(w.split(j*k*l).bundle(j*l,T2_BasedX(),p)) << '\n';
    }

    std::cout << '\n' << '\n';

    std::cout << "Round-trip convert from ComponentIndex\n";
    Sym::ComponentIndex i(7, CheckRange::TRUE);
    std::cout << FORMAT_VALUE(i) << '\n';
    Sym::MultiIndex m = Sym::bundle_index_map<Sym::MultiIndex::IndexTyple, Sym::ComponentIndex>(i);
    std::cout << FORMAT_VALUE(m) << '\n';
    Sym::ComponentIndex j = Sym::vector_index_of(m);
    std::cout << FORMAT_VALUE(j) << "\n\n";

    std::cout << "Round-trip convert from MultiIndex\n";
    Sym::MultiIndex n(1, 0, 2, CheckRange::TRUE);
    std::cout << FORMAT_VALUE(n) << '\n';
    i = Sym::vector_index_of(n);
    std::cout << FORMAT_VALUE(i) << '\n';
    n = Sym::bundle_index_map<Sym::MultiIndex::IndexTyple, Sym::ComponentIndex>(i);
    std::cout << FORMAT_VALUE(n) << '\n';
    i = Sym::vector_index_of(n);
    std::cout << FORMAT_VALUE(i) << "\n\n";
    n = Sym::bundle_index_map<Sym::MultiIndex::IndexTyple, Sym::ComponentIndex>(i);
}

