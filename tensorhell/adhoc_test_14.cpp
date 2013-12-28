#include "adhoc_test.hpp"

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
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<DualOfB3,TypeList_t<DualOfB4> > > TensorProduct;
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
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_type_list(i*j)) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_type_list((i*j)*k)) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_type_list(i*(j*k))) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_type_list(i*j*k)) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_type_list((i*j)*(k*l))) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_type_list(i*(j*k*l))) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_type_list((i*j*k)*l)) << '\n';
    std::cout << FORMAT_VALUE(symbol_string_of_abstract_index_type_list(i*j*k*l)) << '\n';
    std::cout << '\n';
}

void test_linear_solve_using_least_squares ()
{
    typedef double Scalar;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,Generic>,OrthonormalBasis_c<Generic> > B2;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,OrthonormalBasis_c<Generic> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Generic>,OrthonormalBasis_c<Generic> > B4;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B2,TypeList_t<DualOf_f<B2>::T> > > T22;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B2,TypeList_t<DualOf_f<B3>::T> > > T23;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B2,TypeList_t<DualOf_f<B4>::T> > > T24;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<DualOf_f<B2>::T> > > T32;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<DualOf_f<B3>::T> > > T33;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<DualOf_f<B4>::T> > > T34;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B4,TypeList_t<DualOf_f<B2>::T> > > T42;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B4,TypeList_t<DualOf_f<B3>::T> > > T43;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B4,TypeList_t<DualOf_f<B4>::T> > > T44;

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
    typedef DirectSumOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<B4> > > DirectSum;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<B4> > > TensorProduct;
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
    std::cout << FORMAT_VALUE(v(i).embed<IdentityEmbedding>(i,b,j)) << '\n';
    std::cout << FORMAT_VALUE(v.embed<IdentityEmbedding>(b,j)) << '\n';

    std::cout << FORMAT_VALUE(v(i).coembed<IdentityEmbedding>(i,b,j)) << '\n';
    std::cout << FORMAT_VALUE(v.coembed<IdentityEmbedding>(b,j)) << '\n';
    std::cout << '\n';
    std::cout << FORMAT_VALUE(a(i)) << '\n';
    std::cout << FORMAT_VALUE(a(i).embed<IdentityEmbedding>(i,dual_of_b,j)) << '\n';
    std::cout << FORMAT_VALUE(a.embed<IdentityEmbedding>(dual_of_b,j)) << '\n';

    std::cout << FORMAT_VALUE(a(i).coembed<IdentityEmbedding>(i,dual_of_b,j)) << '\n';
    std::cout << FORMAT_VALUE(a.coembed<IdentityEmbedding>(dual_of_b,j)) << '\n';
    std::cout << '\n';

    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B,
                                               TypeList_t<DualOfB> > > TProd;
    TProd t;
    typedef ImplementationOf_t<TProd,Scalar> T;

    T m(uniform_tuple<Scalar>(2, 0, 0) |
        uniform_tuple<Scalar>(1, 1, 1) |
        uniform_tuple<Scalar>(0, 3, 1));
    std::cout << FORMAT_VALUE(m) << '\n';
    std::cout << FORMAT_VALUE(m(i)) << '\n';
    std::cout << FORMAT_VALUE(m(i).embed<IdentityEmbedding>(i,t,j)) << '\n';
    std::cout << FORMAT_VALUE(m.embed<IdentityEmbedding>(t,j)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).embed<IdentityEmbedding>(i,b,k)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).embed<IdentityEmbedding>(j,dual_of_b,l)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).embed<IdentityEmbedding>(i,b,k).embed<IdentityEmbedding>(j,dual_of_b,l)) << '\n';

    std::cout << FORMAT_VALUE(m(i).coembed<IdentityEmbedding>(i,t,j)) << '\n';
    std::cout << FORMAT_VALUE(m.coembed<IdentityEmbedding>(t,j)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).coembed<IdentityEmbedding>(i,b,k)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).coembed<IdentityEmbedding>(j,dual_of_b,l)) << '\n';
    std::cout << FORMAT_VALUE(m(i*j).coembed<IdentityEmbedding>(i,b,k).coembed<IdentityEmbedding>(j,dual_of_b,l)) << '\n';
    std::cout << '\n';
}

void test_diag2tensor_embedding ()
{
    std::cout << "testing natural diag2tensor embedding/coembedding\n";

    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Generic>,Basis_c<Generic> > B4;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B4> Diag2;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<B4> > > Tensor2;

    typedef double Scalar;
    typedef ImplementationOf_t<Diag2,Scalar> D;
    typedef ImplementationOf_t<Tensor2,Scalar> T;

    B3 b3;
    B4 b4;
    Diag2 diag2;
    Tensor2 tensor2;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    AbstractIndex_c<'l'> l;

    D d(uniform_tuple<Scalar>(2, 1, 3));
    std::cout << FORMAT_VALUE(d) << '\n';
    std::cout << FORMAT_VALUE(d.split(i*j)) << '\n';
    std::cout << FORMAT_VALUE(d(i).embed<NaturalEmbedding>(i,tensor2,j)) << '\n';
    std::cout << FORMAT_VALUE(d(i).embed<NaturalEmbedding>(i,tensor2,j).split(j,k*l)) << '\n';
    std::cout << FORMAT_VALUE(d.embed<NaturalEmbedding>(tensor2,j)) << '\n';
    std::cout << FORMAT_VALUE(d.embed<NaturalEmbedding>(tensor2,j).split(j,k*l)) << '\n';

    T t(uniform_tuple<Scalar>( 1,  2,  3,  4) |
        uniform_tuple<Scalar>( 5,  6,  7,  8) |
        uniform_tuple<Scalar>( 9, 10, 11, 12));
    std::cout << FORMAT_VALUE(t) << '\n';
    std::cout << FORMAT_VALUE(t(i*j)) << '\n';
    std::cout << FORMAT_VALUE(t(i).coembed<NaturalEmbedding>(i,diag2,j)) << '\n';
    std::cout << FORMAT_VALUE(t(i).coembed<NaturalEmbedding>(i,diag2,j).split(j,k*l)) << '\n';
    std::cout << FORMAT_VALUE(t.coembed<NaturalEmbedding>(diag2,j)) << '\n';
    std::cout << FORMAT_VALUE(t.coembed<NaturalEmbedding>(diag2,j).split(j,k*l)) << '\n';
    std::cout << '\n';
}

