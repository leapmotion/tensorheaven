#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 12
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_induced_inner_product ()
{
    std::cout << "induced inner product\n";

    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,OrthonormalBasis_c<IdX> > B;
    typedef TensorPowerOfBasedVectorSpace_f<2,B>::T T2;
    typedef TensorPowerOfBasedVectorSpace_f<3,B>::T T3;
    typedef DualOf_f<TensorPowerOfBasedVectorSpace_f<2,B>::T>::T T2_star;
    typedef DualOf_f<TensorPowerOfBasedVectorSpace_f<3,B>::T>::T T3_star;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOf_f<B>::T> S2_B_star;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,T2> S2_T2;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,T3> S2_T3;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOf_f<T2>::T> S2_T2_star;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOf_f<T3>::T> S2_T3_star;

    typedef float Scalar;
    typedef ImplementationOf_t<B,Scalar> V;
    typedef ImplementationOf_t<S2_B_star,Scalar> S2;
    S2 g(tuple(2.0f, 3.0f, 5.0f, 7.0f, 11.0f, 13.0f));
    V u(tuple(10.0f, 11.0f, 12.0f));
    V v(tuple(6.0f, 7.0f, 8.0f));
    Sym_f<2,B,Scalar>::T sym;

    AbstractIndex_c<'a'> a;
    AbstractIndex_c<'b'> b;
    AbstractIndex_c<'c'> c;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'p'> p;
    AbstractIndex_c<'q'> q;
    AbstractIndex_c<'r'> r;
    AbstractIndex_c<'x'> x;
    AbstractIndex_c<'y'> y;
    AbstractIndex_c<'z'> z;

    std::cout << FORMAT_VALUE(g) << '\n';
    std::cout << FORMAT_VALUE(g(i).split(i,j*k)) << '\n';
    std::cout << FORMAT_VALUE(u) << '\n';
    std::cout << FORMAT_VALUE(v) << '\n';
    std::cout << FORMAT_VALUE(g(i).split(i,j*k) * u(j) * v(k)) << '\n';
    // this would become a .project(p,SymmetricPower2(),i)
    std::cout << FORMAT_VALUE(g(i) * sym(i*p) * (u(j) * v(k)).bundle(j*k,T2(),p)) << '\n';

    {
        ImplementationOf_t<S2_T2_star,Scalar> g_T2(Static<WithoutInitialization>::SINGLETON);
        g_T2(r) = (g(i).split(i,j*k) *
                   g(a).split(a,b*c)).bundle(j*b,T2_star(),p)
                                     .bundle(k*c,T2_star(),q)
                                     .bundle(p*q,S2_T2_star(),r);
        std::cout << FORMAT_VALUE(g_T2) << '\n';
        std::cout << FORMAT_VALUE(g_T2(i).split(i,j*k)) << '\n';

        typedef ImplementationOf_t<T2,Scalar> T2_impl;
        T2_impl t2_0(Static<WithoutInitialization>::SINGLETON);
        T2_impl t2_1(Static<WithoutInitialization>::SINGLETON);
        for (T2_impl::ComponentIndex idx; idx.is_not_at_end(); ++idx)
        {
            t2_0[idx] = idx.value();
            t2_1[idx] = idx.value() + 7;
        }
        std::cout << FORMAT_VALUE(g_T2(p).split(p,i*j) * t2_0(i) * t2_1(j)) << '\n';
        std::cout << FORMAT_VALUE(t2_0(b*j) * g(a).split(a,b*c)*g(i).split(i,j*k) * t2_1(c*k)) << '\n';
        Sym_f<2,T2,Scalar>::T sym;
        std::cout << FORMAT_VALUE(g_T2(p) * sym(p*q) * (t2_0(i)*t2_1(j)).bundle(i*j,TensorPowerOfBasedVectorSpace_f<2,T2>::T(),q)) << '\n';
        std::cout << FORMAT_VALUE(g_T2(p) * sym(p*q).split(q,i*j) * (t2_0(i)*t2_1(j))) << '\n';
    }

    {
        ImplementationOf_t<S2_T3_star,Scalar> g_T3(Static<WithoutInitialization>::SINGLETON);
        g_T3(r) = (g(i).split(i,j*k) *
                   g(a).split(a,b*c) *
                   g(x).split(x,y*z)).bundle(j*b*y,T3_star(),p)
                                     .bundle(k*c*z,T3_star(),q)
                                     .bundle(p*q,S2_T3_star(),r);
        std::cout << FORMAT_VALUE(g_T3) << '\n';
        std::cout << FORMAT_VALUE(g_T3(i).split(i,j*k)) << '\n';

        typedef ImplementationOf_t<T3,Scalar> T3_impl;
        T3_impl t3_0(Static<WithoutInitialization>::SINGLETON);
        T3_impl t3_1(Static<WithoutInitialization>::SINGLETON);
        for (T3_impl::ComponentIndex idx; idx.is_not_at_end(); ++idx)
        {
            t3_0[idx] = idx.value();
            t3_1[idx] = idx.value() + 7;
        }
        std::cout << FORMAT_VALUE(t3_0(i) * g_T3(p).split(p,i*j) * t3_1(j)) << '\n';
        std::cout << FORMAT_VALUE(t3_0(b*j*y) * g(a).split(a,b*c)*g(i).split(i,j*k)*g(x).split(x,y*z) * t3_1(c*k*z)) << '\n';
        Sym_f<2,T3,Scalar>::T sym;
        std::cout << FORMAT_VALUE(g_T3(p) * sym(p*q) * (t3_0(i)*t3_1(j)).bundle(i*j,TensorPowerOfBasedVectorSpace_f<2,T3>::T(),q)) << '\n';
        std::cout << FORMAT_VALUE(g_T3(p) * sym(p*q).split(q,i*j) * (t3_0(i)*t3_1(j))) << '\n';
    }
}


void test_direct_sums ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,Basis_c<IdX> > BasedVectorSpace;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,1,IdX>,Basis_c<IdX> > RealLine;
    typedef SymmetricPowerOfBasedVectorSpace_c<2, BasedVectorSpace> SymmetricSquare;

    typedef DirectSumOfBasedVectorSpaces_c<UniformTypeListOfLength_t<4,BasedVectorSpace>::T> PowerOfVectorSpace;

    typedef DirectSumOfBasedVectorSpaces_c<TypeList_t<SymmetricSquare, TypeList_t<BasedVectorSpace, TypeList_t<RealLine> > > > DirectSumType;
    typedef ImplementationOf_t<DirectSumType, float> T;
    typedef ImplementationOf_t<BasedVectorSpace, float> S;
    typedef ImplementationOf_t<PowerOfVectorSpace, float> R;

    T t(fill_with(0));

    S s(fill_with(0)), r(fill_with(0));

    R q(fill_with(0));

    for(Uint32 i = 0; i < 4; ++i)
    {
        for(S::ComponentIndex j; j.is_not_at_end(); ++j)
        {
            q.el(i)[j] = (i+1)*(j.value()+1);
        }
    }

    for(T::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        t[i] = i.value();
    }

    for(S::ComponentIndex i; i.is_not_at_end(); ++i)
    {
        s[i] = i.value();
        r[i] = 4-i.value()*i.value();
    }

    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;


    std::cout << FORMAT_VALUE(t) << '\n';
    std::cout << FORMAT_VALUE(t.el<0>()) << '\n';
    std::cout << FORMAT_VALUE(t.el<1>()) << '\n';
    std::cout << FORMAT_VALUE(t.el<2>()) << '\n';
    std::cout << FORMAT_VALUE(t.el<0>()(i).split(i,j*k)) << '\n';
    // std::cout << FORMAT_VALUE(t.el<0>(i).split(i,j*k)) << '\n';
    std::cout << FORMAT_VALUE(t.el<1>()(i)) << '\n';
    // std::cout << FORMAT_VALUE(t.el<1>(i)) << '\n';
    std::cout << FORMAT_VALUE(t.el<2>()(i)) << '\n';
    // std::cout << FORMAT_VALUE(t.el<2>(i)) << '\n';

    t.el<0>()(k) = 0.5f*(s(i)*r(j) + r(i)*s(j)).bundle(i*j,SymmetricSquare(),k);
    std::cout << FORMAT_VALUE(t) << '\n';
    std::cout << FORMAT_VALUE(t.el<0>()) << '\n';
    std::cout << FORMAT_VALUE(t.el<0>()(i).split(i,j*k)) << '\n';
    // std::cout << FORMAT_VALUE(t.el<0>(i).split(i,j*k)) << '\n';

    t.el<1>()(i) = s(i);
    std::cout << FORMAT_VALUE(t) << '\n';
    std::cout << FORMAT_VALUE(t.el<1>()) << '\n';
    std::cout << FORMAT_VALUE(t.el<1>()(i)) << '\n';
    // std::cout << FORMAT_VALUE(t.el<1>(i)) << '\n';

    std::cout << FORMAT_VALUE(q) << '\n';
    for(Uint32 i = 0; i < 4; ++i)
    {
        std::cout << FORMAT_VALUE(q.el(i)) << '\n';
    }

}
