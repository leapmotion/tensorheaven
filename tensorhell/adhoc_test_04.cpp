#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 4
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_vector_power_thingy ()
{
    typedef VectorSpace_c<RealField,4,X> VSX;
    typedef Basis_c<X> BasisX;
    typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
    typedef ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<4,BasedX>,float> Sym;
    typedef ImplementationOf_t<BasedX,float> Vec;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;
    AbstractIndex_c<'p'> p;

    Vec v(tuple(1.0f,2.0f,3.0f,4.0f));

    std::cout << test_vector_power<4,BasedX,float>(v) << '\n';
    std::cout << test_vector_power<4,BasedX,float>(v)(i).split(i,j*k*l*p) << '\n';
    std::cout << '\n';
}

void test_a_bunch_of_stuff ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedX;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,Y>,Basis_c<Y> > BasedY;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Z>,Basis_c<Z> > BasedZ;

    {
        std::cout << "member array\n";
        typedef ImplementationOf_t<BasedX,float,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > V;
        V v(tuple(1.0f, 2.0f, 4.0f));
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << '\n';

        // the following should cause a compile error regarding UsePreallocatedArray_t<COMPONENTS_ARE_NONCONST>
        //float components[3] = {8.0f, 10.0f, 11.0f};
        //V w(&components[0]);
    }

    {
        std::cout << "preallocated array\n";
        typedef ImplementationOf_t<BasedX,float,UsePreallocatedArray_t<COMPONENTS_ARE_NONCONST> > V;
        float components[3] = {8.0f, 10.0f, 11.0f};
        std::cout << FORMAT_VALUE(components[0]) << ", " << FORMAT_VALUE(components[1]) << ", " << FORMAT_VALUE(components[2]) << '\n';
        V v(&components[0], CheckPointer::TRUE); // v must live no longer than components[]
        std::cout << FORMAT_VALUE(&components[0]) << ", " << FORMAT_VALUE(v.pointer_to_allocation()) << '\n';
        std::cout << FORMAT_VALUE(&v[V::MultiIndex(0)]) << '\n';
        std::cout << FORMAT_VALUE(v[V::MultiIndex(0)]) << '\n';
        std::cout << FORMAT_VALUE(v[V::ComponentIndex(0)]) << ", " << FORMAT_VALUE(v[V::ComponentIndex(1)]) << ", " << FORMAT_VALUE(v[V::ComponentIndex(2)]) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        AbstractIndex_c<'i'> i;
        std::cout << FORMAT_VALUE(v(i)) << '\n';
        for (V::ComponentIndex j; j.is_not_at_end(); ++j)
            std::cout << FORMAT_VALUE(v[j]) << '\n';

        // the following should cause a compile error regarding UseMemberArray_t<COMPONENTS_ARE_NONCONST>
        //V w(1.0f, 2.0f, 4.0f);

        typedef ImplementationOf_t<BasedY,float,UsePreallocatedArray_t<COMPONENTS_ARE_NONCONST> > W;
        W w(&components[1]); // last 2 components, i.e. a linear subspace
        std::cout << FORMAT_VALUE(w) << '\n';
        std::cout << '\n';
    }

    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<BasedY,DualOf_f<BasedX>::T>> YTensorXDual;

    {
        typedef ImplementationOf_t<BasedX,float,UsePreallocatedArray_t<COMPONENTS_ARE_NONCONST> > V;
        typedef ImplementationOf_t<YTensorXDual,float,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > T;
        float components[3] = {8.0f, 10.0f, 11.0f};
        V v(&components[0], CheckPointer::TRUE); // v must live no longer than components[]
        T t(fill_with(2));
        t[T::MultiIndex(0,1)] = -1.0f;
        t[T::MultiIndex(1,2)] = 3.0f;
        std::cout << FORMAT_VALUE(t) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(t(i*j)*v(j)) << '\n';

        std::cout << '\n';
    }

    {
        // using default template parameter for UseArrayType_
        typedef ImplementationOf_t<BasedX,float> V;
        typedef ImplementationOf_t<YTensorXDual,float> T;
        V v(tuple(8.0f, 10.0f, 11.0f));
        T t(fill_with(2));
        t[T::MultiIndex(0,1)] = -1.0f;
        t[T::MultiIndex(1,2)] = 3.0f;
        std::cout << FORMAT_VALUE(t) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(t(i*j)*v(j)) << '\n';

        std::cout << '\n';
    }

    {
        typedef ImplementationOf_t<YTensorXDual,float,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > T;
        typedef ImplementationOf_t<YTensorXDual,float,UsePreallocatedArray_t<COMPONENTS_ARE_NONCONST> > U;
        T t(fill_with(2));
        t[T::MultiIndex(0,1)] = -1.0f;
        t[T::MultiIndex(1,2)] = 3.0f;
        U u(t.pointer_to_allocation()); // u must live no longer than t
        std::cout << FORMAT_VALUE(t) << '\n';
        std::cout << FORMAT_VALUE(u) << '\n';
        std::cout << '\n';
    }

    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedY,DualOf_f<BasedX>::T> Diag_YTensorXDual;

    {
        std::cout << "testing Diagonal2TensorProductOfBasedVectorSpaces_c\n";
        typedef ImplementationOf_t<BasedX,float,UsePreallocatedArray_t<COMPONENTS_ARE_NONCONST> > V;
        typedef ImplementationOf_t<Diag_YTensorXDual,float,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > T;
        float components[3] = {8.0f, 10.0f, 11.0f};
        V v(&components[0], CheckPointer::TRUE); // v must live no longer than components[]
        T t(tuple(2.0f, -3.0f));
        std::cout << FORMAT_VALUE(t) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'P'> P;
        std::cout << FORMAT_VALUE(t(P).split(P,i*j)*v(j)) << '\n';
        std::cout << FORMAT_VALUE(t.split(i*j)*v(j)) << '\n';

        std::cout << '\n';
    }

    typedef SymmetricPowerOfBasedVectorSpace_c<3,BasedX> Sym;

    {
        std::cout << "testing SymmetricPowerOfBasedVectorSpace_c\n";
        typedef ImplementationOf_t<BasedX,float,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > V;
        typedef ImplementationOf_t<Sym,float,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > S;
        typedef ImplementationOf_t<Sym,float,UsePreallocatedArray_t<COMPONENTS_ARE_NONCONST> > S_;
        S s(Static<WithoutInitialization>::SINGLETON);
        V v(tuple(3.0f, 4.0f, 7.0f));
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'P'> P;
        s(P) = (v(i)*v(j)*v(k)).bundle(i*j*k,Sym(),P);
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(s) << '\n';
        std::cout << FORMAT_VALUE(s(P).split(P,i*j*k)) << '\n';
        std::cout << FORMAT_VALUE(s.split(i*j*k)) << '\n';
        S_ s_(s.pointer_to_allocation());
        std::cout << FORMAT_VALUE(s_) << '\n';

        std::cout << '\n';
    }

    typedef ExteriorPowerOfBasedVectorSpace_c<3,BasedZ> Alt;

    {
        std::cout << "testing ExteriorPowerOfBasedVectorSpace_c\n";
        typedef ImplementationOf_t<BasedZ,float,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > V;
        typedef ImplementationOf_t<Alt,float,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > A;
        typedef ImplementationOf_t<Alt,float,UsePreallocatedArray_t<COMPONENTS_ARE_NONCONST> > A_;
        A a(Static<WithoutInitialization>::SINGLETON);
        V u(tuple(3.0f, 4.0f, 7.0f, 0.0f));
        V v(tuple(1.0f, -2.0f, 4.0f, -1.0f));
        V w(tuple(0.0f, 1.0f, 2.0f, 3.0f));
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'P'> P;
        a(P) = (u(i)*v(j)*w(k) - u(i)*w(j)*v(k) - v(i)*u(j)*w(k) + v(i)*w(j)*u(k) + w(i)*u(j)*v(k) - w(i)*v(j)*u(k)).bundle(i*j*k,Alt(),P);
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(a) << '\n';
        std::cout << FORMAT_VALUE(a(P).split(P,i*j*k)) << '\n';
        std::cout << FORMAT_VALUE(a.split(i*j*k)) << '\n';
        A_ a_(a.pointer_to_allocation());
        std::cout << FORMAT_VALUE(a_) << '\n';

        std::cout << '\n';
    }
}

