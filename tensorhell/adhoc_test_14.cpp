#include "adhoc_test.hpp"

#include "tenh/implementation/scalar2tensor.hpp"
#include "tenh/interop/eigen_linearsolve.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 14
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_embeddable_as_tensor_as_multilinear_form ()
{
    std::cout << "EmbeddableAsTensor_i as multilinear form\n";

    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,OrthonormalBasis_c<X> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,X>,OrthonormalBasis_c<X> > B4;
    typedef DualOf_f<B3>::T DualOfB3;
    typedef DualOf_f<B4>::T DualOfB4;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<DualOfB3,DualOfB4>> TensorProduct;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<DualOfB3,DualOfB4> Diagonal2TensorProduct;
    typedef SymmetricPowerOfBasedVectorSpace_c<3,DualOfB3> Sym;
    typedef ExteriorPowerOfBasedVectorSpace_c<2,DualOfB3> Alt;

    typedef float Scalar;
    typedef ImplementationOf_t<B3,Scalar> V3;
    typedef ImplementationOf_t<B4,Scalar> V4;
    typedef ImplementationOf_t<DualOfB3,Scalar> DualOfV3;
    typedef ImplementationOf_t<DualOfB4,Scalar> DualOfV4;
    typedef ImplementationOf_t<TensorProduct,Scalar> T;
    typedef ImplementationOf_t<Diagonal2TensorProduct,Scalar> D;
    typedef ImplementationOf_t<Sym,Scalar> S;
    typedef ImplementationOf_t<Alt,Scalar> A;

    T t(Static<WithoutInitialization>::SINGLETON);
    D d(Static<WithoutInitialization>::SINGLETON);
    S s(Static<WithoutInitialization>::SINGLETON);
    A a(Static<WithoutInitialization>::SINGLETON);

    for (T::ComponentIndex i; i.is_not_at_end(); ++i)
        t[i] = i.value();
    for (D::ComponentIndex i; i.is_not_at_end(); ++i)
        d[i] = i.value();
    for (S::ComponentIndex i; i.is_not_at_end(); ++i)
        s[i] = i.value();
    for (A::ComponentIndex i; i.is_not_at_end(); ++i)
        a[i] = i.value();

    V3 v1(uniform_tuple<Scalar>(8, 4, 1));
    V3 v2(uniform_tuple<Scalar>(0, 1, 3));
    V3 v3(uniform_tuple<Scalar>(-4, 2, 2));
    V4 w(uniform_tuple<Scalar>(8, -2, 5, -1));

    DualOfV3 x(uniform_tuple<Scalar>(-1, 3, 2));
    DualOfV4 y(uniform_tuple<Scalar>(1, -2, 3, 4));

    std::cout << FORMAT_VALUE(v1) << '\n';
    std::cout << FORMAT_VALUE(v2) << '\n';
    std::cout << FORMAT_VALUE(v3) << '\n';
    std::cout << FORMAT_VALUE(w) << '\n';
    std::cout << '\n';

    std::cout << FORMAT_VALUE(x) << '\n';
    std::cout << FORMAT_VALUE(y) << '\n';
    std::cout << '\n';

    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    std::cout << FORMAT_VALUE(t(i*j)) << '\n';
    std::cout << FORMAT_VALUE(d.split(i*j)) << '\n';
    std::cout << FORMAT_VALUE(s.split(i*j*k)) << '\n';
    std::cout << FORMAT_VALUE(a.split(i*j)) << '\n';
    std::cout << '\n';

    std::cout << FORMAT_VALUE(t(tuple(v1, w))) << '\n';
    std::cout << FORMAT_VALUE(d(tuple(v1, w))) << '\n';
    std::cout << FORMAT_VALUE(s(tuple(v1, v2, v3))) << '\n';
    std::cout << FORMAT_VALUE(a(tuple(v1, v2))) << '\n';
    std::cout << '\n';

    std::cout << FORMAT_VALUE(t(v1, w)) << '\n';
    std::cout << FORMAT_VALUE(d(v1, w)) << '\n';
    std::cout << FORMAT_VALUE(a(v1, v2)) << '\n';
    std::cout << '\n';

    std::cout << "the following numbers should be exactly zero:\n";
    std::cout << FORMAT_VALUE(x(tuple(v1)) - x(i)*v1(i)) << '\n';
    std::cout << FORMAT_VALUE(y(tuple(w)) - y(i)*w(i)) << '\n';
    std::cout << FORMAT_VALUE(t(tuple(v1, w)) - t(i*j)*v1(i)*w(j)) << '\n';
    std::cout << FORMAT_VALUE(d(tuple(v1, w)) - d.split(i*j)*v1(i)*w(j)) << '\n';
    std::cout << FORMAT_VALUE(s(tuple(v1, v2, v3)) - s.split(i*j*k)*v1(i)*v2(j)*v3(k)) << '\n';
    std::cout << FORMAT_VALUE(a(tuple(v1, v2)) - a.split(i*j)*v1(i)*v2(j)) << '\n';
    std::cout << FORMAT_VALUE(x(v1) - x(i)*v1(i)) << '\n';
    std::cout << FORMAT_VALUE(y(w) - y(i)*w(i)) << '\n';
    std::cout << FORMAT_VALUE(t(v1, w) - t(i*j)*v1(i)*w(j)) << '\n';
    std::cout << FORMAT_VALUE(d(v1, w) - d.split(i*j)*v1(i)*w(j)) << '\n';
    std::cout << FORMAT_VALUE(a(v1, v2) - a.split(i*j)*v1(i)*v2(j)) << '\n';
    std::cout << '\n';

    std::cout << '\n';
}

void test_product_of_abstract_indices ()
{
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;

    std::cout << "testing product of abstract indices\n";
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_typle(i*j)) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_typle((i*j)*k)) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_typle(i*(j*k))) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_typle(i*j*k)) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_typle((i*j)*(k*l))) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_typle(i*(j*k*l))) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_typle((i*j*k)*l)) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_typle(i*j*k*l)) << '\n';
    std::cout << '\n';
}

void test_linear_solve_using_least_squares ()
{
    typedef double Scalar;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,Generic>,OrthonormalBasis_c<Generic> > B2;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,OrthonormalBasis_c<Generic> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Generic>,OrthonormalBasis_c<Generic> > B4;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B2,DualOf_f<B2>::T>> T22;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B2,DualOf_f<B3>::T>> T23;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B2,DualOf_f<B4>::T>> T24;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B3,DualOf_f<B2>::T>> T32;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B3,DualOf_f<B3>::T>> T33;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B3,DualOf_f<B4>::T>> T34;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B4,DualOf_f<B2>::T>> T42;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B4,DualOf_f<B3>::T>> T43;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B4,DualOf_f<B4>::T>> T44;

    typedef ImplementationOf_t<B2,Scalar> V2;
    typedef ImplementationOf_t<B3,Scalar> V3;
    typedef ImplementationOf_t<B4,Scalar> V4;
    typedef ImplementationOf_t<T22,Scalar> L22;
    typedef ImplementationOf_t<T23,Scalar> L23;
    typedef ImplementationOf_t<T24,Scalar> L24;
    typedef ImplementationOf_t<T32,Scalar> L32;
    typedef ImplementationOf_t<T33,Scalar> L33;
    typedef ImplementationOf_t<T34,Scalar> L34;
    typedef ImplementationOf_t<T42,Scalar> L42;
    typedef ImplementationOf_t<T43,Scalar> L43;
    typedef ImplementationOf_t<T44,Scalar> L44;

    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        L22 l22(uniform_tuple<Scalar>(1, 2) |
                uniform_tuple<Scalar>(3, 4));
        V2 v2(uniform_tuple<Scalar>(0, 1));
        V2 x(Static<WithoutInitialization>::SINGLETON);
        std::cout << FORMAT_VALUE(l22) << '\n';
        std::cout << FORMAT_VALUE(v2) << '\n';
        linear_solve_using_least_squares<StandardInnerProduct>(l22, x, v2);
        std::cout << "solution: " << FORMAT_VALUE(x) << '\n';
        std::cout << "image of solution: " << FORMAT_VALUE(l22(i*j)*x(j)) << '\n';
        std::cout << "discrepancy: " << FORMAT_VALUE(l22(i*j)*x(j) - v2(i)) << '\n';
    }
    {
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        L22 l22(uniform_tuple<Scalar>(1, 1) |
                uniform_tuple<Scalar>(1, 1));
        V2 v2(uniform_tuple<Scalar>(0, 1));
        V2 x(Static<WithoutInitialization>::SINGLETON);
        std::cout << FORMAT_VALUE(l22) << '\n';
        std::cout << FORMAT_VALUE(v2) << '\n';
        linear_solve_using_least_squares<StandardInnerProduct>(l22, x, v2);
        std::cout << "solution: " << FORMAT_VALUE(x) << '\n';
        std::cout << "image of solution: " << FORMAT_VALUE(l22(i*j)*x(j)) << '\n';
        std::cout << "discrepancy: " << FORMAT_VALUE(l22(i*j)*x(j) - v2(i)) << '\n';
    }
}

template <typename Concept_, typename Scalar_>
void test_particular_implementation_of_vector_construction_via_vector_i ()
{
    typedef ImplementationOf_t<Concept_,Scalar_,UseMemberArray_t<COMPONENTS_ARE_NONCONST> > U;
    typedef ImplementationOf_t<Concept_,Scalar_,UsePreallocatedArray_t<COMPONENTS_ARE_NONCONST> > V;
    static Uint32 const DIM = U::DIM;

    U u(Static<WithoutInitialization>::SINGLETON);
    Scalar_ array[DIM];
    V v(&array[0]);

    for (typename U::ComponentIndex i; i.is_not_at_end(); ++i)
        u[i] = i.value() + 1;
    for (typename V::ComponentIndex i; i.is_not_at_end(); ++i)
        v[i] = i.value() + 1 + DIM;

    std::cout << "test_implementation_of_vector_construction_via_vector_i<" + type_string_of<Concept_>() + ',' + type_string_of<Scalar_>() + ">\n";
    std::cout << FORMAT_VALUE(u) << '\n';
    std::cout << FORMAT_VALUE(v) << '\n';

    typename U::Parent_Vector_i const &u_as_vector_i = u;
    std::cout << FORMAT_VALUE(u_as_vector_i) << '\n';
    typename V::Parent_Vector_i const &v_as_vector_i = v;
    std::cout << FORMAT_VALUE(v_as_vector_i) << '\n';
    {
        std::cout << "testing UseMemberArray_t<COMPONENTS_ARE_NONCONST> construction from Vector_i types\n";
        U constructed_from_vector_i_0(u_as_vector_i);
        std::cout << FORMAT_VALUE(constructed_from_vector_i_0) << '\n';
        U constructed_from_vector_i_1(v_as_vector_i);
        std::cout << FORMAT_VALUE(constructed_from_vector_i_1) << '\n';
        U constructed_from_vector_i_2(U::template BasisVector_f<0>::V);
        std::cout << FORMAT_VALUE(constructed_from_vector_i_2) << '\n';
    }
    {
        std::cout << "testing UsePreallocatedArray_t<COMPONENTS_ARE_NONCONST> construction from Vector_i types\n";
        Scalar_ other_array[3];
        V constructed_from_vector_i_0(u_as_vector_i, &other_array[0]);
        std::cout << FORMAT_VALUE(constructed_from_vector_i_0) << '\n';
        V constructed_from_vector_i_1(v_as_vector_i, &other_array[0]);
        std::cout << FORMAT_VALUE(constructed_from_vector_i_1) << '\n';
        V constructed_from_vector_i_2(V::template BasisVector_f<0>::V, &other_array[0]);
        std::cout << FORMAT_VALUE(constructed_from_vector_i_2) << '\n';
    }
    std::cout << '\n';
}

void test_implementation_of_vector_construction_via_vector_i ()
{
    typedef double Scalar;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Generic>,Basis_c<Generic> > B4;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B4> Diagonal2Tensor;
    typedef DirectSumOfBasedVectorSpaces_c<Typle_t<B3,B4>> DirectSum;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B3,B4>> TensorProduct;
    typedef ExteriorPowerOfBasedVectorSpace_c<3,B4> ExteriorPower;
    typedef SymmetricPowerOfBasedVectorSpace_c<3,B3> SymmetricPower;
    test_particular_implementation_of_vector_construction_via_vector_i<B3,Scalar>();
    test_particular_implementation_of_vector_construction_via_vector_i<Diagonal2Tensor,Scalar>();
    test_particular_implementation_of_vector_construction_via_vector_i<DirectSum,Scalar>();
    test_particular_implementation_of_vector_construction_via_vector_i<TensorProduct,Scalar>();
    test_particular_implementation_of_vector_construction_via_vector_i<ExteriorPower,Scalar>();
    test_particular_implementation_of_vector_construction_via_vector_i<SymmetricPower,Scalar>();
}

void test_const_component_arrays ()
{
    {
        std::cout << "testing const-component MemberArray_t\n";
        typedef MemberArray_t<Uint32,8,COMPONENTS_ARE_NONCONST> MemberArrayNonConst;
        typedef MemberArray_t<Uint32,8,COMPONENTS_ARE_CONST> MemberArrayConst;

        MemberArrayNonConst nonconst_array(Static<WithoutInitialization>::SINGLETON);
        for (MemberArrayNonConst::ComponentIndex i; i.is_not_at_end(); ++i)
            nonconst_array[i] = i.value() + 1;
        std::cout << "nonconst_array = ";
        nonconst_array.print(std::cout);
        std::cout << '\n';

        MemberArrayConst const_array(nonconst_array);
        std::cout << "const_array = ";
        const_array.print(std::cout);
        std::cout << "\n\n";

        // this should produce a compile error
        //const_array[MemberArrayConst::ComponentIndex(0)] = 82;
    }

    {
        std::cout << "testing const-component PreallocatedArray_t\n";
        typedef PreallocatedArray_t<Uint32,8,COMPONENTS_ARE_CONST> PreallocatedArrayConst;

        Uint32 const data[8] = {2, 4, 8, 16, 32, 64, 128, 256};

        std::cout << "data = {";
        for (Uint32 i = 0; i < 8; ++i)
            std::cout << data[i] << ", ";
        std::cout << "}\n";

        PreallocatedArrayConst const_array(&data[0]);
        std::cout << "const_array = ";
        const_array.print(std::cout);
        std::cout << "\n\n";

        // this should produce a compile error
        //const_array[PreallocatedArrayConst::ComponentIndex(0)] = 82;
    }
}

void test_identity_embedding ()
{
    std::cout << "testing identity embedding/coembedding\n";

    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > B;
    typedef DualOf_f<B>::T DualOfB;
    B b;
    DualOfB dual_of_b;
    typedef double Scalar;
    typedef ImplementationOf_t<B,Scalar> V;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;

    V v(uniform_tuple<Scalar>(1, 2, 4));
    V::Dual a(uniform_tuple<Scalar>(3, -1, 2));

    std::cout << FORMAT_VALUE(v(i)) << '\n';
    std::cout << FORMAT_VALUE(v(i).embed_using<IdentityEmbedding>(i,b,j)) << '\n';
    std::cout << FORMAT_VALUE(v.embed_using<IdentityEmbedding>(b,j)) << '\n';

    std::cout << FORMAT_VALUE(v(i).coembed_using<IdentityEmbedding>(i,b,j)) << '\n';
    std::cout << FORMAT_VALUE(v.coembed_using<IdentityEmbedding>(b,j)) << '\n';
    std::cout << '\n';
    std::cout << FORMAT_VALUE(a(i)) << '\n';
    std::cout << FORMAT_VALUE(a(i).embed_using<IdentityEmbedding>(i,dual_of_b,j)) << '\n';
    std::cout << FORMAT_VALUE(a.embed_using<IdentityEmbedding>(dual_of_b,j)) << '\n';

    std::cout << FORMAT_VALUE(a(i).coembed_using<IdentityEmbedding>(i,dual_of_b,j)) << '\n';
    std::cout << FORMAT_VALUE(a.coembed_using<IdentityEmbedding>(dual_of_b,j)) << '\n';
    std::cout << '\n';

    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B,DualOfB>> TProd;
    TProd t;
    typedef ImplementationOf_t<TProd,Scalar> T;

    T m(uniform_tuple<Scalar>(2, 0, 0) |
        uniform_tuple<Scalar>(1, 1, 1) |
        uniform_tuple<Scalar>(0, 3, 1));
    std::cout << FORMAT_VALUE(m) << '\n';
    std::cout << FORMAT_VALUE(m(i)) << '\n';
    std::cout << FORMAT_VALUE(m(i).embed_using<IdentityEmbedding>(i,t,j)) << '\n';
    std::cout << FORMAT_VALUE(m.embed_using<IdentityEmbedding>(t,j)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).embed_using<IdentityEmbedding>(i,b,k)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).embed_using<IdentityEmbedding>(j,dual_of_b,l)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).embed_using<IdentityEmbedding>(i,b,k).embed_using<IdentityEmbedding>(j,dual_of_b,l)) << '\n';

    std::cout << FORMAT_VALUE(m(i).coembed_using<IdentityEmbedding>(i,t,j)) << '\n';
    std::cout << FORMAT_VALUE(m.coembed_using<IdentityEmbedding>(t,j)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).coembed_using<IdentityEmbedding>(i,b,k)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).coembed_using<IdentityEmbedding>(j,dual_of_b,l)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).coembed_using<IdentityEmbedding>(i,b,k).coembed_using<IdentityEmbedding>(j,dual_of_b,l)) << '\n';
    std::cout << '\n';
}

void test_diag2tensor_embedding ()
{
    std::cout << "testing natural diag2tensor embedding/coembedding\n";

    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Generic>,Basis_c<Generic> > B4;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B4> Diag2;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B3,B4>> Tensor2;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,B3> Sym2;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B3> Diag2OfB3;

    typedef double Scalar;
    typedef ImplementationOf_t<B3,Scalar> V3;
    typedef ImplementationOf_t<B4,Scalar> V4;
    typedef ImplementationOf_t<Diag2,Scalar> D;
    typedef ImplementationOf_t<Diag2OfB3,Scalar> DOfB3;
    typedef ImplementationOf_t<Tensor2,Scalar> T;
    typedef ImplementationOf_t<Sym2,Scalar> S;

    // B3 b3;
    // B4 b4;
    Diag2 diag2;
    Diag2OfB3 diag2ofb3;
    DualOf_f<Diag2>::T dual_of_diag2;
    Tensor2 tensor2;
    Sym2 sym2;
    // DualOf_f<Tensor2>::T dual_of_tensor2;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;

    {
        D d(uniform_tuple<Scalar>(2, 1, 3));
        std::cout << FORMAT_VALUE(d) << '\n';
        std::cout << FORMAT_VALUE(d.split(i*j)) << '\n';
        std::cout << FORMAT_VALUE(d(i).embed(i,tensor2,j)) << '\n';
        std::cout << FORMAT_VALUE(d(i).embed(i,tensor2,j).split(j,k*l)) << '\n';
        std::cout << FORMAT_VALUE(d.embed(tensor2,j)) << '\n';
        std::cout << FORMAT_VALUE(d.embed(tensor2,j).split(j,k*l)) << '\n';

        T t(uniform_tuple<Scalar>( 1,  2,  3,  4) |
            uniform_tuple<Scalar>( 5,  6,  7,  8) |
            uniform_tuple<Scalar>( 9, 10, 11, 12));
        std::cout << FORMAT_VALUE(t) << '\n';
        std::cout << FORMAT_VALUE(t(i*j)) << '\n';
        std::cout << FORMAT_VALUE(t(i).coembed(i,diag2,j)) << '\n';
        std::cout << FORMAT_VALUE(t(i).coembed(i,diag2,j).split(j,k*l)) << '\n';
        std::cout << FORMAT_VALUE(t.coembed(diag2,j)) << '\n';
        std::cout << FORMAT_VALUE(t.coembed(diag2,j).split(j,k*l)) << '\n';
        std::cout << '\n';

        T::Dual t_dual(uniform_tuple<Scalar>( 1,  2,  3,  4) |
                       uniform_tuple<Scalar>( 5,  6,  7,  8) |
                       uniform_tuple<Scalar>( 9, 10, 11, 12));
        std::cout << "this should be exactly zero: " << FORMAT_VALUE(d.embed(tensor2,j)*t_dual(j) - d(i)*t_dual.coembed(dual_of_diag2,i)) << '\n';
    }

    // this should produce a type error (nonexistence of LinearEmbedding_c from diag2 to dual_of_tensor2)
    // std::cout << FORMAT_VALUE(d.embed(dual_of_tensor2,j)) << '\n';
    // this should produce a type error (nonexistence of LinearEmbedding_c from dual_of_diag2 to tensor2)
    // std::cout << FORMAT_VALUE(t.coembed(dual_of_diag2,j)) << '\n';

    {
        V3 u(uniform_tuple<Scalar>(1, 2, 3));
        V4 v(uniform_tuple<Scalar>(4, 5, 6, 7));
        std::cout << FORMAT_VALUE(u) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(u(i)*v(j)) << '\n';
        std::cout << FORMAT_VALUE((u(i)*v(j)).bundle(i*j,tensor2,k).coembed(k,diag2,l)) << '\n';
        std::cout << '\n';
    }

    {
        DOfB3 d(uniform_tuple<Scalar>(2, 1, 3));
        std::cout << FORMAT_VALUE(d) << '\n';
        std::cout << FORMAT_VALUE(d.split(i*j)) << '\n';
        std::cout << FORMAT_VALUE(d(i).embed(i,sym2,j)) << '\n';
        std::cout << FORMAT_VALUE(d(i).embed(i,sym2,j).split(j,k*l)) << '\n';
        std::cout << FORMAT_VALUE(d.embed(sym2,j)) << '\n';
        std::cout << FORMAT_VALUE(d.embed(sym2,j).split(j,k*l)) << '\n';

        S s(uniform_tuple<Scalar>( 1) |
            uniform_tuple<Scalar>( 2, 3) |
            uniform_tuple<Scalar>( 4, 5, 6));
        std::cout << FORMAT_VALUE(s) << '\n';
        std::cout << FORMAT_VALUE(s.split(i*j)) << '\n';
        std::cout << FORMAT_VALUE(s(i).coembed(i,diag2ofb3,j)) << '\n';
        std::cout << FORMAT_VALUE(s(i).coembed(i,diag2ofb3,j).split(j,k*l)) << '\n';
        std::cout << FORMAT_VALUE(s.coembed(diag2ofb3,j)) << '\n';
        std::cout << FORMAT_VALUE(s.coembed(diag2ofb3,j).split(j,k*l)) << '\n';
        std::cout << '\n';
    }
}

void test_scalar2tensor_embedding ()
{
    std::cout << "testing natural scalar2tensor embedding/coembedding\n";

    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Generic>,Basis_c<Generic> > B4;
    typedef Scalar2TensorProductOfBasedVectorSpaces_c<B3,B4> Scalar2;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B4> Diag2;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B3,B4>> Tensor2;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,B3> Sym2;
    typedef Scalar2TensorProductOfBasedVectorSpaces_c<B3,B3> Scalar2OfB3;

    typedef double Scalar;
    typedef ImplementationOf_t<B3,Scalar> V3;
    typedef ImplementationOf_t<B4,Scalar> V4;
    typedef ImplementationOf_t<Diag2,Scalar> D;
    typedef ImplementationOf_t<Scalar2,Scalar> Sc;
    typedef ImplementationOf_t<Scalar2OfB3,Scalar> ScOfB3;
    typedef ImplementationOf_t<Tensor2,Scalar> T;
    typedef ImplementationOf_t<Sym2,Scalar> S;

    // B3 b3;
    // B4 b4;
    Scalar2 scalar2;
    Diag2 diag2;
    Scalar2OfB3 scalar2ofb3;
    DualOf_f<Scalar2>::T dual_of_scalar2;
    Tensor2 tensor2;
    Sym2 sym2;
    // DualOf_f<Tensor2>::T dual_of_tensor2;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;

    {
        Sc d(uniform_tuple<Scalar>(2));
        std::cout << FORMAT_VALUE(d) << '\n';
        std::cout << FORMAT_VALUE(d.split(i*j)) << '\n';
        std::cout << FORMAT_VALUE(d(i).embed(i,tensor2,j)) << '\n';
        std::cout << FORMAT_VALUE(d(i).embed(i,tensor2,j).split(j,k*l)) << '\n';
        std::cout << FORMAT_VALUE(d.embed(tensor2,j)) << '\n';
        std::cout << FORMAT_VALUE(d.embed(tensor2,j).split(j,k*l)) << '\n';

        T t(uniform_tuple<Scalar>( 1,  2,  3,  4) |
            uniform_tuple<Scalar>( 5,  6,  7,  8) |
            uniform_tuple<Scalar>( 9, 10, 11, 12));
        std::cout << FORMAT_VALUE(t) << '\n';
        std::cout << FORMAT_VALUE(t(i*j)) << '\n';
        std::cout << FORMAT_VALUE(t(i).coembed(i,scalar2,j)) << '\n';
        std::cout << FORMAT_VALUE(t(i).coembed(i,scalar2,j).split(j,k*l)) << '\n';
        std::cout << FORMAT_VALUE(t.coembed(scalar2,j)) << '\n';
        std::cout << FORMAT_VALUE(t.coembed(scalar2,j).split(j,k*l)) << '\n';
        std::cout << '\n';

        T::Dual t_dual(uniform_tuple<Scalar>( 1,  2,  3,  4) |
                       uniform_tuple<Scalar>( 5,  6,  7,  8) |
                       uniform_tuple<Scalar>( 9, 10, 11, 12));
        std::cout << "this should be exactly zero: " << FORMAT_VALUE(d.embed(tensor2,j)*t_dual(j) - d(i)*t_dual.coembed(dual_of_scalar2,i)) << '\n';
    }

    // this should produce a type error (nonexistence of LinearEmbedding_c from scalar2 to dual_of_tensor2)
    // std::cout << FORMAT_VALUE(d.embed(dual_of_tensor2,j)) << '\n';
    // this should produce a type error (nonexistence of LinearEmbedding_c from dual_of_scalar2 to tensor2)
    // std::cout << FORMAT_VALUE(t.coembed(dual_of_scalar2,j)) << '\n';

    {
        V3 u(uniform_tuple<Scalar>(1, 2, 3));
        V4 v(uniform_tuple<Scalar>(4, 5, 6, 7));
        std::cout << FORMAT_VALUE(u) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(u(i)*v(j)) << '\n';
        std::cout << FORMAT_VALUE((u(i)*v(j)).bundle(i*j,tensor2,k).coembed(k,scalar2,l)) << '\n';
        std::cout << '\n';
    }

    {
        ScOfB3 d(uniform_tuple<Scalar>(2));
        std::cout << FORMAT_VALUE(d) << '\n';
        std::cout << FORMAT_VALUE(d.split(i*j)) << '\n';
        std::cout << FORMAT_VALUE(d(i).embed(i,sym2,j)) << '\n';
        std::cout << FORMAT_VALUE(d(i).embed(i,sym2,j).split(j,k*l)) << '\n';
        std::cout << FORMAT_VALUE(d.embed(sym2,j)) << '\n';
        std::cout << FORMAT_VALUE(d.embed(sym2,j).split(j,k*l)) << '\n';

        S s(uniform_tuple<Scalar>( 1) |
            uniform_tuple<Scalar>( 2, 3) |
            uniform_tuple<Scalar>( 4, 5, 6));
        std::cout << FORMAT_VALUE(s) << '\n';
        std::cout << FORMAT_VALUE(s.split(i*j)) << '\n';
        std::cout << FORMAT_VALUE(s(i).coembed(i,scalar2ofb3,j)) << '\n';
        std::cout << FORMAT_VALUE(s(i).coembed(i,scalar2ofb3,j).split(j,k*l)) << '\n';
        std::cout << FORMAT_VALUE(s.coembed(scalar2ofb3,j)) << '\n';
        std::cout << FORMAT_VALUE(s.coembed(scalar2ofb3,j).split(j,k*l)) << '\n';
        std::cout << '\n';
    }

    {
        Sc s(uniform_tuple<Scalar>(2));
        std::cout << FORMAT_VALUE(s) << '\n';
        std::cout << FORMAT_VALUE(s.split(i*j)) << '\n';
        std::cout << FORMAT_VALUE(s(i).embed(i,diag2,j)) << '\n';
        std::cout << FORMAT_VALUE(s(i).embed(i,diag2,j).split(j,k*l)) << '\n';
        std::cout << FORMAT_VALUE(s.embed(diag2,j)) << '\n';
        std::cout << FORMAT_VALUE(s.embed(diag2,j).split(j,k*l)) << '\n';

        D d(uniform_tuple<Scalar>(1, 2, 3));
        std::cout << FORMAT_VALUE(d) << '\n';
        std::cout << FORMAT_VALUE(d.split(i*j)) << '\n';
        std::cout << FORMAT_VALUE(d(i).coembed(i,scalar2,j)) << '\n';
        std::cout << FORMAT_VALUE(d(i).coembed(i,scalar2,j).split(j,k*l)) << '\n';
        std::cout << FORMAT_VALUE(d.coembed(scalar2,j)) << '\n';
        std::cout << FORMAT_VALUE(d.coembed(scalar2,j).split(j,k*l)) << '\n';
        std::cout << '\n';
    }
}


template <Uint32 ORDER_, typename Factor_>
void test_exterior_power_embedding_templatized ()
{
    std::cout << "testing natural " << ORDER_ << "-exterior power embedding/coembedding\n";

    typedef ExteriorPowerOfBasedVectorSpace_c<ORDER_,Factor_> Ext;
    typedef typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T TPow;

    typedef double Scalar;
    typedef ImplementationOf_t<Ext,Scalar> E;
    typedef ImplementationOf_t<TPow,Scalar> T;

    // Factor_ v;
    // Ext ext;
    TPow tpow;

    // AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    // AbstractIndex_c<'k'> k;
    // AbstractIndex_c<'l'> l;

    E e(Static<WithoutInitialization>::SINGLETON);
    for (typename E::ComponentIndex it; it.is_not_at_end(); ++it)
        e[it] = it.value() + 1;

    std::cout << FORMAT_VALUE(e) << '\n';
    std::cout << FORMAT_VALUE(e.embed(tpow,j)) << '\n';
    typename AbstractIndexRangeTyple_f<ORDER_,'k'>::T abstract_index_range;
    std::cout << FORMAT_VALUE(e.embed(tpow,j).split(j,abstract_index_range)) << '\n';
    // this causes a compile error because single indices and 1-multiindices are distinct types.
    // std::cout << FORMAT_VALUE(e.embed(tpow,j).eval().value()) << '\n';
    std::cout << '\n';
}

void test_exterior_power_embedding ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > B;
    test_exterior_power_embedding_templatized<1,B>();
    test_exterior_power_embedding_templatized<2,B>();
    test_exterior_power_embedding_templatized<3,B>();
}

template <Uint32 ORDER_, typename Factor_>
void test_symmetric_power_embedding_templatized ()
{
    std::cout << "testing natural " << ORDER_ << "-symmetric power embedding/coembedding\n";

    typedef SymmetricPowerOfBasedVectorSpace_c<ORDER_,Factor_> Sym;
    typedef typename TensorPowerOfBasedVectorSpace_f<ORDER_,Factor_>::T TPow;

    typedef double Scalar;
    typedef ImplementationOf_t<Sym,Scalar> S;
    typedef ImplementationOf_t<TPow,Scalar> T;

    // Factor_ v;
    // Sym sym;
    TPow tpow;

    // AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    // AbstractIndex_c<'k'> k;
    // AbstractIndex_c<'l'> l;

    S s(Static<WithoutInitialization>::SINGLETON);
    for (typename S::ComponentIndex it; it.is_not_at_end(); ++it)
        s[it] = it.value() + 1;

    std::cout << FORMAT_VALUE(s) << '\n';
    std::cout << FORMAT_VALUE(s.embed(tpow,j)) << '\n';
    typename AbstractIndexRangeTyple_f<ORDER_,'k'>::T abstract_index_range;
    std::cout << FORMAT_VALUE(s.embed(tpow,j).split(j,abstract_index_range)) << '\n';
    // this causes a compile error because single indices and 1-multiindices are distinct types.
    // std::cout << FORMAT_VALUE(s.embed(tpow,j).eval().value()) << '\n';
    std::cout << '\n';
}

void test_symmetric_power_embedding ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > B;
    test_symmetric_power_embedding_templatized<1,B>();
    test_symmetric_power_embedding_templatized<2,B>();
    test_symmetric_power_embedding_templatized<3,B>();
}

void test_embed_coembed_adjointness ()
{
    std::cout << "testing embed/coembed adjointness\n";

    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Generic>,Basis_c<Generic> > B4;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B4> Diag2;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<B3,B4>> Tensor2;

    typedef double Scalar;
    typedef ImplementationOf_t<B3,Scalar> V3;
    typedef ImplementationOf_t<B4,Scalar> V4;
    typedef ImplementationOf_t<Diag2,Scalar> D;
    typedef ImplementationOf_t<Tensor2,Scalar> T;

    // B3 b3;
    // B4 b4;
    // Diag2 diag2;
    DualOf_f<Diag2>::T dual_of_diag2;
    Tensor2 tensor2;
    // DualOf_f<Tensor2>::T dual_of_tensor2;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    // AbstractIndex_c<'k'> k;
    // AbstractIndex_c<'l'> l;

    D d(uniform_tuple<Scalar>(2, 1, 3));
    T::Dual t(uniform_tuple<Scalar>( 1,  2,  3,  4) |
              uniform_tuple<Scalar>( 5,  6,  7,  8) |
              uniform_tuple<Scalar>( 9, 10, 11, 12));
    std::cout << FORMAT_VALUE(d.split(i*j)) << '\n';
    std::cout << FORMAT_VALUE(t.split(i*j)) << '\n';
    std::cout << FORMAT_VALUE(d.embed(tensor2,j)*t(j)) << '\n';
    std::cout << FORMAT_VALUE(d(i)*t.coembed(dual_of_diag2,i)) << '\n';
    std::cout << "this should be exactly zero: ";
    std::cout << FORMAT_VALUE(d.embed(tensor2,j)*t(j) - d(i)*t.coembed(dual_of_diag2,i)) << '\n';
    std::cout << '\n';
}

void test_type_system_helper_functions ()
{
    std::cout << "testing type system helper functions\n";
    RealField real_field;
    Generic generic;
    std::cout << FORMAT_VALUE(type_string_of(real_field)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(generic)) << '\n';
    std::cout << '\n';
    std::cout << FORMAT_VALUE(type_string_of(vs<3>(real_field, generic))) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(generic_vs<3>(real_field))) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(real_vs<3>(generic))) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(generic_real_vs<3>())) << '\n';
    std::cout << '\n';
    std::cout << FORMAT_VALUE(type_string_of(basis(Generic()))) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(generic_basis())) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(o_n_basis(Generic()))) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(generic_o_n_basis())) << '\n';
    std::cout << '\n';
    Basis_c<Generic> b;
    VectorSpace_c<RealField,3,Generic> v3;
    // VectorSpace_c<RealField,4,Generic> v4;
    std::cout << FORMAT_VALUE(type_string_of(bvs(v3, b))) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(bvs<3>(real_field, generic))) << '\n';
    std::cout << '\n';
    BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > bvs3;
    BasedVectorSpace_c<VectorSpace_c<RealField,4,Generic>,Basis_c<Generic> > bvs4;
    std::cout << FORMAT_VALUE(type_string_of(bvs3 + bvs4)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(bvs3 + bvs4 + bvs3)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of((bvs3 + bvs4) + bvs3)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(bvs3 + (bvs4 + bvs3))) << '\n';
    std::cout << FORMAT_VALUE(type_string_of((bvs3 + bvs4) + (bvs4 + bvs3))) << '\n';
    std::cout << '\n';
    std::cout << FORMAT_VALUE(type_string_of(bvs3 * bvs4)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(bvs3 * bvs4 * bvs3)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of((bvs3 * bvs4) * bvs3)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(bvs3 * (bvs4 * bvs3))) << '\n';
    std::cout << FORMAT_VALUE(type_string_of((bvs3 * bvs4) * (bvs4 * bvs3))) << '\n';
    std::cout << '\n';
    std::cout << FORMAT_VALUE(type_string_of(diag2(bvs3*bvs4))) << '\n';
    std::cout << '\n';
    std::cout << FORMAT_VALUE(type_string_of(ext<3>(bvs3))) << '\n';
    std::cout << '\n';
    std::cout << FORMAT_VALUE(type_string_of(sym<3>(bvs3))) << '\n';
    std::cout << '\n';
}
