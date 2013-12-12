#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 1
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_vector_contraction ()
{
    std::cout << "testing vector contraction\n";

    typedef VectorSpace_c<RealField,3,X> VSX;
    typedef Basis_c<X> B;
    typedef BasedVectorSpace_c<VSX,B> BasedX;

    typedef ImplementationOf_t<BasedX,float> V;
    V v(tuple(1.0f, 2.0f, 3.0f));
    V w(tuple(8.0f, -2.0f, 6.0f));
    std::cout << type_string_of<V>() << '\n';
    std::cout << FORMAT_VALUE(v) << '\n';
    std::cout << FORMAT_VALUE(w) << '\n';

    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    std::cout << FORMAT_VALUE(v(i)) << '\n';
    std::cout << FORMAT_VALUE(w(i)) << '\n';
    std::cout << FORMAT_VALUE(v(i) + w(i)) << '\n';
    std::cout << FORMAT_VALUE(v(i) - w(i)) << '\n';

    DualOf_f<V>::T a(tuple(0.0f, 2.0f, -3.0f));
    std::cout << type_string_of<DualOf_f<V>::T>() << '\n';
    std::cout << FORMAT_VALUE(a) << '\n';
    std::cout << FORMAT_VALUE(a(i)*v(i)) << '\n';
    std::cout << FORMAT_VALUE(v(i)*a(i)) << '\n';
    std::cout << FORMAT_VALUE(v(i)*a(j)) << '\n';

    // this should cause a compile error due to the non-naturality of the pairing
    // std::cout << FORMAT_VALUE(v(i)*w(i)) << '\n';

    // this should cause a compile error due to a mismatch of factor types
    // std::cout << FORMAT_VALUE(a(i) + v(i)) << '\n';

    // testing 1-multiindex indexing of vectors
    std::cout << FORMAT_VALUE(a(EmptyTypeList()|i)) << '\n';
    std::cout << FORMAT_VALUE(v(EmptyTypeList()|i)) << '\n';

    std::cout << '\n' << '\n';
}

void test_tensor_contraction ()
{
    std::cout << "testing tensor contraction\n";

    typedef VectorSpace_c<RealField,3,X> VSX;
    typedef Basis_c<X> BX;
    typedef BasedVectorSpace_c<VSX,BX> BasedX;

    typedef VectorSpace_c<RealField,2,Y> VSY;
    typedef Basis_c<Y> BY;
    typedef BasedVectorSpace_c<VSY,BY> BasedY;

    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<DualOf_f<BasedY>::T> > > TPBVS;
    typedef ImplementationOf_t<BasedX,float> U;
    typedef ImplementationOf_t<BasedY,float> V;
    typedef ImplementationOf_t<TPBVS,float> T;
    std::cout << type_string_of<T>() << '\n';

    T t(fill_with(3));
    std::cout << FORMAT_VALUE(t) << '\n';
    t[T::MultiIndex(0,0)] = 0.0f;
    t[T::MultiIndex(1,0)] = 4.0f;
    t[T::MultiIndex(2,0)] = -2.0f;
    t[T::MultiIndex(0,1)] = 3.0f;
    t[T::MultiIndex(1,1)] = 4.0f;
    t[T::MultiIndex(2,1)] = 5.0f;
    std::cout << "after assignment: " << FORMAT_VALUE(t) << '\n';

    V v(tuple(2.0f, 7.0f));
    std::cout << FORMAT_VALUE(v) << '\n';

    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    std::cout << FORMAT_VALUE(t(i|j)*v(j)) << '\n';

    U u(tuple(1.0f, 0.0f, -1.0f));
    std::cout << FORMAT_VALUE(u) << '\n';
    // this should cause a compile error due to the non-naturality of the pairing
    // std::cout << FORMAT_VALUE(u(i)*t(i|j)) << '\n';


    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<DualOf_f<BasedY>::T,TypeList_t<DualOf_f<BasedY>::T> > > > H;
    typedef ImplementationOf_t<H,float> E;
    E e(fill_with(0));
    for (E::ComponentIndex it; it.is_not_at_end(); ++it)
        e[it] = static_cast<float>(it.value());
    std::cout << FORMAT_VALUE(e(i)) << '\n'; // this has problems: TODO: fix -- ExpressionTemplate_i may need a vector-indexable operator[]

    V w(tuple(1.0f, 3.0f));

    AbstractIndex_c<'k'> k;
    std::cout << FORMAT_VALUE(w) << '\n';
    std::cout << FORMAT_VALUE(e) << '\n';
    std::cout << FORMAT_VALUE(e(i|j|k)*v(j)*w(k)) << '\n';

    // this should cause a compile error due to the non-naturality of the pairing
    // std::cout << FORMAT_VALUE(e(i|j|j)) << '\n';

    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedY,TypeList_t<DualOf_f<BasedY>::T> > > EndomorphismOfY;
    typedef ImplementationOf_t<EndomorphismOfY,float> Endo;
    Endo A(fill_with(0));
    A[Endo::MultiIndex(0,0)] = 3.0f;
    A[Endo::MultiIndex(1,1)] = 4.0f;
    std::cout << FORMAT_VALUE(A) << '\n';
    std::cout << FORMAT_VALUE(A(i|j)*v(j)) << '\n';
    std::cout << FORMAT_VALUE(A(i|i)) << '\n';


    DualOf_f<E>::T f(fill_with(3));
    f[DualOf_f<E>::T::MultiIndex(0,0,0,CHECK_RANGE)] = 1.0f;
    f[DualOf_f<E>::T::MultiIndex(1,0,1,CHECK_RANGE)] = 2.0f;
    f[DualOf_f<E>::T::MultiIndex(2,1,1,CHECK_RANGE)] = 4.0f;
    std::cout << FORMAT_VALUE(f) << '\n';
    std::cout << FORMAT_VALUE(f(i|j|k)*e(i|j|k)) << '\n';
    std::cout << FORMAT_VALUE(f(i|j|k)*e(i|k|j)) << '\n';
    std::cout << FORMAT_VALUE(f(i)*e(i)) << '\n';

    // std::cout << FORMAT_VALUE(e(i).eval()) << '\n';
    std::cout << FORMAT_VALUE(e(i|j|k).eval()) << '\n';

    AbstractIndex_c<'l'> l;
    std::cout << FORMAT_VALUE(e(i).split(i,j|k|l)) << '\n';


    AbstractIndex_c<'P'> P;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<DualOf_f<BasedY>::T,TypeList_t<DualOf_f<BasedY>::T> > > PartOfH;
    typedef ImplementationOf_t<PartOfH,float> G;
    std::cout << FORMAT_VALUE(e(i|j|k).bundle(j|k,PartOfH(),P)) << '\n';
    std::cout << FORMAT_VALUE(f(i|j|k).bundle(j|k,DualOf_f<PartOfH>::T(),P)) << '\n';
    std::cout << FORMAT_VALUE(e(i|j|k).bundle(j|k,PartOfH(),P) * f(i|j|k).bundle(j|k,DualOf_f<PartOfH>::T(),P)) << '\n';

    std::cout << FORMAT_VALUE(e(EmptyTypeList()|i)) << '\n';

    std::cout << '\n' << '\n';
}

