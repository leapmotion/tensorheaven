#include <iostream>

#include "tenh/implementation/alt.hpp"
#include "tenh/implementation/diagonal2tensor.hpp"
#include "tenh/implementation/directsum.hpp"
#include "tenh/implementation/euclideanembedding.hpp"
#include "tenh/implementation/innerproduct.hpp"
#include "tenh/implementation/scalar2tensor.hpp"
#include "tenh/implementation/sym.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/tensorproduct.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"
#include "tenh/implementation/wedge.hpp"
#include "tenh/tuple.hpp"

using namespace Tenh;

template <typename Scalar_, Uint32 DIM_>
Scalar_ counting_vector_generator (ComponentIndex_t<DIM_> const &i)
{
    return Scalar_(i.value() + 1);
}

template <Uint32 DIM_>
struct CountingVectorGeneratorId { static std::string type_as_string () { return "CountingVectorGenerator<" + FORMAT(DIM_) + '>'; } };

int main (int argc, char **argv)
{
    Tuple_t<Typle_t<>> empty_tuple;
    Tuple_t<Typle_t<int>> int_tuple(1);
    Tuple_t<Typle_t<int,float>> int_float_tuple(1, 3.0f);
    Tuple_t<Typle_t<int,float,Uint32>> int_float_Uint32_tuple(1, 3.0f, 456);

    assert(empty_tuple.length() == 0);
    assert(int_tuple.length() == 1);
    assert(int_float_tuple.length() == 2);
    assert(int_float_Uint32_tuple.length() == 3);

    std::cout << FORMAT_VALUE(empty_tuple) << '\n';
    std::cout << FORMAT_VALUE(int_tuple) << '\n';
    std::cout << FORMAT_VALUE(int_float_tuple) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple) << '\n';

    std::cout << FORMAT_VALUE(empty_tuple | empty_tuple) << '\n';
    std::cout << FORMAT_VALUE(int_tuple | empty_tuple) << '\n';
    std::cout << FORMAT_VALUE(empty_tuple | int_tuple) << '\n';
    std::cout << FORMAT_VALUE(int_tuple | int_tuple) << '\n';
    std::cout << FORMAT_VALUE(int_tuple | int_float_tuple) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple | int_float_tuple) << '\n';

    std::cout << '\n';

    std::cout << FORMAT_VALUE(empty_tuple.body()) << '\n';
    std::cout << FORMAT_VALUE(int_tuple.head()) << '\n';
    std::cout << FORMAT_VALUE(int_tuple.body()) << '\n';
    std::cout << FORMAT_VALUE(int_float_tuple.head()) << '\n';
    std::cout << FORMAT_VALUE(int_float_tuple.body()) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.head()) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.body()) << '\n';

    std::cout << '\n';

    // std::cout << FORMAT_VALUE(empty_tuple.el<0>()) << '\n';

    std::cout << FORMAT_VALUE(int_tuple.el<0>()) << '\n';
    // std::cout << FORMAT_VALUE(int_tuple.el<1>()) << '\n';

    std::cout << FORMAT_VALUE(int_float_tuple.el<0>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_tuple.el<1>()) << '\n';
    // std::cout << FORMAT_VALUE(int_float_tuple.el<2>()) << '\n';

    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.el<0>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.el<1>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.el<2>()) << '\n';
    // std::cout << FORMAT_VALUE(int_float_Uint32_tuple.el<3>()) << '\n';

    std::cout << '\n';

    std::cout << FORMAT_VALUE(empty_tuple.leading_tuple<0>()) << '\n';
    // std::cout << FORMAT_VALUE(empty_tuple.leading_tuple<1>()) << '\n';

    std::cout << FORMAT_VALUE(int_tuple.leading_tuple<0>()) << '\n';
    std::cout << FORMAT_VALUE(int_tuple.leading_tuple<1>()) << '\n';
    // std::cout << FORMAT_VALUE(int_tuple.leading_tuple<2>()) << '\n';

    std::cout << FORMAT_VALUE(int_float_tuple.leading_tuple<0>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_tuple.leading_tuple<1>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_tuple.leading_tuple<2>()) << '\n';
    // std::cout << FORMAT_VALUE(int_float_tuple.leading_tuple<3>()) << '\n';

    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.leading_tuple<0>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.leading_tuple<1>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.leading_tuple<2>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.leading_tuple<3>()) << '\n';
    // std::cout << FORMAT_VALUE(int_float_Uint32_tuple.leading_tuple<4>()) << '\n';

    std::cout << '\n';

    std::cout << FORMAT_VALUE(empty_tuple.trailing_tuple<0>()) << '\n';
    // std::cout << FORMAT_VALUE(empty_tuple.trailing_tuple<1>()) << '\n';

    std::cout << FORMAT_VALUE(int_tuple.trailing_tuple<0>()) << '\n';
    std::cout << FORMAT_VALUE(int_tuple.trailing_tuple<1>()) << '\n';
    // std::cout << FORMAT_VALUE(int_tuple.trailing_tuple<2>()) << '\n';

    std::cout << FORMAT_VALUE(int_float_tuple.trailing_tuple<0>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_tuple.trailing_tuple<1>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_tuple.trailing_tuple<2>()) << '\n';
    // std::cout << FORMAT_VALUE(int_float_tuple.trailing_tuple<3>()) << '\n';

    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.trailing_tuple<0>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.trailing_tuple<1>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.trailing_tuple<2>()) << '\n';
    std::cout << FORMAT_VALUE(int_float_Uint32_tuple.trailing_tuple<3>()) << '\n';
    // std::cout << FORMAT_VALUE(int_float_Uint32_tuple.trailing_tuple<4>()) << '\n';

    std::cout << '\n';

    std::cout << FORMAT_VALUE((empty_tuple.range<0,0>())) << '\n';
    // std::cout << FORMAT_VALUE((empty_tuple.range<0,1>())) << '\n';
    // std::cout << FORMAT_VALUE((empty_tuple.range<1,1>())) << '\n';

    std::cout << FORMAT_VALUE((int_tuple.range<0,0>())) << '\n';
    std::cout << FORMAT_VALUE((int_tuple.range<0,1>())) << '\n';
    // std::cout << FORMAT_VALUE((int_tuple.range<0,2>())) << '\n';
    std::cout << FORMAT_VALUE((int_tuple.range<1,1>())) << '\n';
    // std::cout << FORMAT_VALUE((int_tuple.range<1,2>())) << '\n';
    // std::cout << FORMAT_VALUE((int_tuple.range<2,2>())) << '\n';

    std::cout << FORMAT_VALUE((int_float_tuple.range<0,0>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_tuple.range<0,1>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_tuple.range<0,2>())) << '\n';
    // std::cout << FORMAT_VALUE((int_float_tuple.range<0,3>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_tuple.range<1,1>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_tuple.range<1,2>())) << '\n';
    // std::cout << FORMAT_VALUE((int_float_tuple.range<1,3>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_tuple.range<2,2>())) << '\n';
    // std::cout << FORMAT_VALUE((int_float_tuple.range<2,3>())) << '\n';
    // std::cout << FORMAT_VALUE((int_float_tuple.range<3,3>())) << '\n';

    std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<0,0>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<0,1>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<0,2>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<0,3>())) << '\n';
    // std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<0,4>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<1,1>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<1,2>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<1,3>())) << '\n';
    // std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<1,4>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<2,2>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<2,3>())) << '\n';
    // std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<2,4>())) << '\n';
    std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<3,3>())) << '\n';
    // std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<3,4>())) << '\n';
    // std::cout << FORMAT_VALUE((int_float_Uint32_tuple.range<4,4>())) << '\n';

    std::cout << '\n';

    {
        std::cout << "testing tuples\n";
        auto t(tuple(3, 'a', "blah", true, 3.01, -123));
        std::cout << FORMAT_VALUE(type_string_of(t)) << '\n';
        std::cout << FORMAT_VALUE(t) << '\n';

        auto u(uniform_tuple<float>(1, 3.0, 4.0f));
        std::cout << FORMAT_VALUE(type_string_of(u)) << '\n';
        std::cout << FORMAT_VALUE(u) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing vectors\n";
        typedef decltype(bvs(generic_real_vs<3>(), generic_basis())) B;
        typedef double Scalar;
        typedef ImplementationOf_t<B,Scalar> V;
        typedef ImplementationOf_t<DualOf_f<B>::T,Scalar> VDual;

        V u(uniform_tuple<Scalar>(3, 5, 8));
        V v(fill_with(3));
        std::cout << FORMAT_VALUE(u) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';

        AbstractIndex_c<'i'> i;
        std::cout << FORMAT_VALUE(u(i)) << '\n';
        std::cout << FORMAT_VALUE(v(i)) << '\n';
        std::cout << FORMAT_VALUE(u(i) + v(i)) << '\n';

        VDual a(uniform_tuple<Scalar>(5, -1, 1));
        std::cout << FORMAT_VALUE(a) << '\n';
        std::cout << FORMAT_VALUE(a(i) * u(i)) << '\n';
        std::cout << FORMAT_VALUE(a(u)) << '\n';
        std::cout << FORMAT_VALUE(u(a)) << '\n';
        std::cout << FORMAT_VALUE(a(tuple(u))) << '\n';
        std::cout << FORMAT_VALUE(u(tuple(a))) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing tensors\n";
        decltype(bvs(generic_real_vs<2>(), generic_basis())) b2;
        decltype(bvs(generic_real_vs<3>(), generic_basis())) b3;
        decltype(bvs(generic_real_vs<4>(), generic_basis())) b4;
        typedef double Scalar;
        typedef ImplementationOf_t<decltype(b2*b3),Scalar> T23;
        typedef ImplementationOf_t<decltype(b2*b3*b4),Scalar> T234;

        T23 t23(uniform_tuple<Scalar>(1, 2, 3,
                                      4, 5, 6));
        std::cout << FORMAT_VALUE(type_string_of<T23::Concept>()) << '\n';
        std::cout << FORMAT_VALUE(t23) << '\n';
        std::cout << '\n';

        T23::Dual a23(uniform_tuple<Scalar>(5, -1, 3,
                                            4,  2, 1));
        std::cout << FORMAT_VALUE(a23) << '\n';
        std::cout << FORMAT_VALUE(a23(t23)) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(a23(i)*t23(i)) << '\n';
        std::cout << FORMAT_VALUE(a23(i*j)*t23(i*j)) << '\n';

        typedef ImplementationOf_t<decltype(b2),Scalar> V2;
        typedef ImplementationOf_t<decltype(b3),Scalar> V3;
        V2 v2(uniform_tuple<Scalar>(4, 3));
        V3 v3(uniform_tuple<Scalar>(1, 0, 5));
        std::cout << FORMAT_VALUE(a23(i*j)*v2(i)*v3(j)) << '\n';
        std::cout << '\n';
    }

    {
        std::cout << "testing direct sums\n";
        decltype(bvs(generic_real_vs<2>(), generic_basis())) b2;
        decltype(bvs(generic_real_vs<3>(), generic_basis())) b3;
        decltype(bvs(generic_real_vs<4>(), generic_basis())) b4;
        typedef double Scalar;
        typedef ImplementationOf_t<decltype(b2+b3),Scalar> S23;
        typedef ImplementationOf_t<decltype(b2+b3+b4),Scalar> S234;

        S23 s23(uniform_tuple<Scalar>(1, 2, 3, 4, 5));
        std::cout << FORMAT_VALUE(type_string_of<S23::Concept>()) << '\n';
        std::cout << FORMAT_VALUE(s23) << '\n';
        std::cout << '\n';

        S23::Dual a23(uniform_tuple<Scalar>(5, -1, 3, 4, 2));
        std::cout << FORMAT_VALUE(a23) << '\n';
        std::cout << FORMAT_VALUE(a23(s23)) << '\n';
        AbstractIndex_c<'i'> i;
        std::cout << FORMAT_VALUE(a23(i)*s23(i)) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing tuple conversions\n";
        typedef DimIndex_t<'i',3> Di3;
        typedef DimIndex_t<'j',4> Dj4;
        typedef ComponentIndex_t<3> C3;
        typedef ComponentIndex_t<4> C4;
        typedef Tuple_t<Typle_t<Di3,Dj4>> Li3j4;
        typedef Tuple_t<Typle_t<C3,C4>> L34;
        Li3j4 li3j4(Di3(1),Dj4(2));

        std::cout << FORMAT_VALUE(type_string_of(li3j4)) << '\n';
        std::cout << FORMAT_VALUE(li3j4) << '\n';

        L34 l34(li3j4);

        std::cout << FORMAT_VALUE(type_string_of(l34)) << '\n';
        std::cout << FORMAT_VALUE(l34) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing multiindex conversions\n";
        typedef DimIndex_t<'i',3> Di3;
        typedef DimIndex_t<'j',4> Dj4;
        typedef ComponentIndex_t<3> C3;
        typedef ComponentIndex_t<4> C4;
        typedef MultiIndex_t<Typle_t<Di3,Dj4>> Mi3j4;
        typedef MultiIndex_t<Typle_t<C3,C4>> M34;
        Mi3j4 mi3j4(Di3(1),Dj4(2));

        std::cout << FORMAT_VALUE(type_string_of(mi3j4)) << '\n';
        std::cout << FORMAT_VALUE(mi3j4) << '\n';

        M34 m34(mi3j4);

        std::cout << FORMAT_VALUE(type_string_of(m34)) << '\n';
        std::cout << FORMAT_VALUE(m34) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing diagonal 2-tensors\n";
        decltype(bvs(generic_real_vs<2>(), generic_basis())) b2;
        decltype(bvs(generic_real_vs<3>(), generic_basis())) b3;
        typedef double Scalar;
        typedef ImplementationOf_t<decltype(diag2(b2,b3)),Scalar> D23;

        D23 d23(uniform_tuple<Scalar>(1, 2));
        std::cout << FORMAT_VALUE(type_string_of<D23::Concept>()) << '\n';
        std::cout << FORMAT_VALUE(d23) << '\n';

        D23::Dual a23(uniform_tuple<Scalar>(5, -1));
        std::cout << FORMAT_VALUE(a23) << '\n';
        std::cout << FORMAT_VALUE(a23(d23)) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(a23(i)*d23(i)) << '\n';

        typedef ImplementationOf_t<decltype(b2),Scalar> V2;
        typedef ImplementationOf_t<decltype(b3),Scalar> V3;
        V2 v2(uniform_tuple<Scalar>(4, 3));
        V3 v3(uniform_tuple<Scalar>(2, 1, -3));
        std::cout << FORMAT_VALUE(v2) << '\n';
        std::cout << FORMAT_VALUE(v3) << '\n';
        std::cout << FORMAT_VALUE(a23.split(i*j)) << '\n';
        std::cout << FORMAT_VALUE(v2(i)*v3(j)) << '\n';
        std::cout << FORMAT_VALUE(a23.split(i*j)*v2(i)*v3(j)) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing scalar 2-tensors\n";
        decltype(bvs(generic_real_vs<2>(), generic_basis())) b2;
        decltype(bvs(generic_real_vs<3>(), generic_basis())) b3;
        typedef double Scalar;
        typedef ImplementationOf_t<decltype(scalar2(b2,b3)),Scalar> S23;

        S23 s23(uniform_tuple<Scalar>(2));
        std::cout << FORMAT_VALUE(type_string_of<S23::Concept>()) << '\n';
        std::cout << FORMAT_VALUE(s23) << '\n';

        S23::Dual a23(uniform_tuple<Scalar>(5));
        std::cout << FORMAT_VALUE(a23) << '\n';
        std::cout << FORMAT_VALUE(a23(s23)) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(a23(i)*s23(i)) << '\n';

        typedef ImplementationOf_t<decltype(b2),Scalar> V2;
        typedef ImplementationOf_t<decltype(b3),Scalar> V3;
        V2 v2(uniform_tuple<Scalar>(4, 3));
        V3 v3(uniform_tuple<Scalar>(2, 1, -3));
        std::cout << FORMAT_VALUE(v2) << '\n';
        std::cout << FORMAT_VALUE(v3) << '\n';
        std::cout << FORMAT_VALUE(a23.split(i*j)) << '\n';
        std::cout << FORMAT_VALUE(v2(i)*v3(j)) << '\n';
        std::cout << FORMAT_VALUE(a23.split(i*j)*v2(i)*v3(j)) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing symmetric powers\n";
        decltype(bvs(generic_real_vs<3>(), generic_basis())) b3;
        typedef double Scalar;
        typedef ImplementationOf_t<decltype(sym<3>(b3)),Scalar> Sym;
        Sym s(Static<WithoutInitialization>::SINGLETON);
        for (Sym::ComponentIndex i; i.is_not_at_end(); ++i)
            s[i] = i.value() + 1;
        std::cout << FORMAT_VALUE(type_string_of(s)) << '\n';
        std::cout << FORMAT_VALUE(s) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'p'> p;
        std::cout << FORMAT_VALUE(s.embed(tensor<3>(b3),p).split(p,i*j*k)) << '\n';
        std::cout << FORMAT_VALUE(s.embed(tensor<3>(b3),p).split(p,i*j*k) - s.embed(tensor<3>(b3),p).split(p,j*i*k)) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing exterior powers\n";
        decltype(bvs(generic_real_vs<3>(), generic_basis())) b3;
        typedef double Scalar;
        typedef ImplementationOf_t<decltype(ext<3>(b3)),Scalar> Ext;
        Ext w(Static<WithoutInitialization>::SINGLETON);
        for (Ext::ComponentIndex i; i.is_not_at_end(); ++i)
            w[i] = i.value() + 1;
        std::cout << FORMAT_VALUE(type_string_of(w)) << '\n';
        std::cout << FORMAT_VALUE(w) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'p'> p;
        std::cout << FORMAT_VALUE(w.embed(tensor<3>(b3),p).split(p,i*j*k)) << '\n';
        std::cout << FORMAT_VALUE(w.embed(tensor<3>(b3),p).split(p,i*j*k) + w.embed(tensor<3>(b3),p).split(p,j*i*k)) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing TensorProductOfProceduralVectors_f\n";
        typedef decltype(bvs(generic_real_vs<3>(), o_n_basis(Generic()))) B3;
        typedef double Scalar;
        typedef ImplementationOf_t<B3,Scalar> V;
        typedef V::BasisVector_f<0>::T E0;
        typedef V::BasisVector_f<1>::T E1;
        typedef V::BasisVector_f<2>::T E2;
        std::cout << FORMAT_VALUE(E0()) << '\n';
        std::cout << FORMAT_VALUE(E1()) << '\n';
        std::cout << FORMAT_VALUE(E2()) << '\n';
        std::cout << '\n';
        {
            typedef Typle_t<E0> ProceduralVectorImplementationTyple;
            typedef TensorProductOfProceduralVectors_f<ProceduralVectorImplementationTyple>::T T;
            std::cout << FORMAT_VALUE(type_string_of<T>()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
        {
            typedef Typle_t<E0,E1> ProceduralVectorImplementationTyple;
            typedef TensorProductOfProceduralVectors_f<ProceduralVectorImplementationTyple>::T T;
            std::cout << FORMAT_VALUE(type_string_of<T>()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
        {
            typedef Typle_t<E1,E1> ProceduralVectorImplementationTyple;
            typedef TensorProductOfProceduralVectors_f<ProceduralVectorImplementationTyple>::T T;
            std::cout << FORMAT_VALUE(type_string_of<T>()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
        {
            typedef Typle_t<E0,E1,E2> ProceduralVectorImplementationTyple;
            typedef TensorProductOfProceduralVectors_f<ProceduralVectorImplementationTyple>::T T;
            std::cout << FORMAT_VALUE(type_string_of<T>()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
    }

    {
        std::cout << "testing TensorProductOfProcedural2Tensors_f (for general 2-tensors)\n";
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,int>,OrthonormalBasis_c<int> > BasedX;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,float>,OrthonormalBasis_c<float> > BasedY;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,bool>,OrthonormalBasis_c<bool> > BasedZ;
        typedef TensorProductOfBasedVectorSpaces_c<Typle_t<BasedX,BasedY>> TensorProductA;
        typedef TensorProductOfBasedVectorSpaces_c<Typle_t<BasedY,BasedZ>> TensorProductB;
        typedef float Scalar;
        typedef ImplementationOf_t<TensorProductA,
                                   Scalar,
                                   UseProceduralArray_t<ComponentGenerator_t<Scalar,
                                                                             DimensionOf_f<TensorProductA>::V,
                                                                             counting_vector_generator<Scalar,DimensionOf_f<TensorProductA>::V>,
                                                                             CountingVectorGeneratorId<DimensionOf_f<TensorProductA>::V>>>> ImplementationA;
        typedef ImplementationOf_t<TensorProductB,
                                   Scalar,
                                   UseProceduralArray_t<ComponentGenerator_t<Scalar,
                                                                             DimensionOf_f<TensorProductB>::V,
                                                                             counting_vector_generator<Scalar,DimensionOf_f<TensorProductB>::V>,
                                                                             CountingVectorGeneratorId<DimensionOf_f<TensorProductB>::V>>>> ImplementationB;
        {
            std::cout << FORMAT_VALUE(ImplementationA()) << '\n';
            typedef TensorProductOfProcedural2Tensors_f<Typle_t<ImplementationA> >::T TProdOfImpl;
            std::cout << FORMAT_VALUE(TProdOfImpl()) << '\n';
        }
        {
            std::cout << FORMAT_VALUE(ImplementationB()) << '\n';
            typedef TensorProductOfProcedural2Tensors_f<Typle_t<ImplementationA,ImplementationB>>::T TProdOfImpl;
            std::cout << FORMAT_VALUE(TProdOfImpl()) << '\n';
            AbstractIndex_c<'P'> P;
            AbstractIndex_c<'Q'> Q;
            AbstractIndex_c<'i'> i;
            AbstractIndex_c<'j'> j;
            AbstractIndex_c<'k'> k;
            AbstractIndex_c<'l'> l;
            std::cout << FORMAT_VALUE(TProdOfImpl()(P*Q).split(P,i*j).split(Q,k*l)) << '\n';
        }
        std::cout << '\n';
    }

    {
        std::cout << "testing TensorProductOfProcedural2Tensors_f (for diagonal 2-tensors)\n";
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,int>,OrthonormalBasis_c<int> > BasedX;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,float>,OrthonormalBasis_c<float> > BasedY;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,bool>,OrthonormalBasis_c<bool> > BasedZ;
        typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedX,BasedY> TensorProductA;
        typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedY,BasedZ> TensorProductB;
        typedef float Scalar;
        typedef ImplementationOf_t<TensorProductA,
                                   Scalar,
                                   UseProceduralArray_t<ComponentGenerator_t<Scalar,
                                                                            DimensionOf_f<TensorProductA>::V,
                                                                            counting_vector_generator<Scalar,DimensionOf_f<TensorProductA>::V>,
                                                                            CountingVectorGeneratorId<DimensionOf_f<TensorProductA>::V> > > > ImplementationA;
        typedef ImplementationOf_t<TensorProductB,
                                   Scalar,
                                   UseProceduralArray_t<ComponentGenerator_t<Scalar,
                                                                            DimensionOf_f<TensorProductB>::V,
                                                                            counting_vector_generator<Scalar,DimensionOf_f<TensorProductB>::V>,
                                                                            CountingVectorGeneratorId<DimensionOf_f<TensorProductB>::V> > > > ImplementationB;
        std::cout << "this is totally broken.\n";
        {
            std::cout << FORMAT_VALUE(ImplementationA()) << '\n';
            typedef TensorProductOfProcedural2Tensors_f<Typle_t<ImplementationA> >::T TProdOfImpl;
            std::cout << FORMAT_VALUE(TProdOfImpl()) << '\n';
        }
        {
            std::cout << FORMAT_VALUE(ImplementationB()) << '\n';
            typedef TensorProductOfProcedural2Tensors_f<Typle_t<ImplementationA,ImplementationB>>::T TProdOfImpl;
            std::cout << FORMAT_VALUE(TProdOfImpl()) << '\n';
            AbstractIndex_c<'A'> A;
            AbstractIndex_c<'P'> P;
            AbstractIndex_c<'Q'> Q;
            AbstractIndex_c<'i'> i;
            AbstractIndex_c<'j'> j;
            AbstractIndex_c<'k'> k;
            AbstractIndex_c<'l'> l;
            std::cout << FORMAT_VALUE(TProdOfImpl()(A).split(A,P*Q)) << '\n';
            std::cout << FORMAT_VALUE(TProdOfImpl()(A).split(A,P*Q).split(P,i*j).split(Q,k*l)) << '\n';
        }
        std::cout << '\n';
    }

    {
        std::cout << "testing TensorProductOfProcedural2Tensors_f (for scalar 2-tensors)\n";
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,int>,OrthonormalBasis_c<int> > BasedX;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,float>,OrthonormalBasis_c<float> > BasedY;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,bool>,OrthonormalBasis_c<bool> > BasedZ;
        typedef Scalar2TensorProductOfBasedVectorSpaces_c<BasedX,BasedY> TensorProductA;
        typedef Scalar2TensorProductOfBasedVectorSpaces_c<BasedY,BasedZ> TensorProductB;
        typedef float Scalar;
        typedef ImplementationOf_t<TensorProductA,
                                   Scalar,
                                   UseProceduralArray_t<ComponentGenerator_Constant_f<Scalar,1,2>::T> > ImplementationA;
                                   // UseProceduralArray_t<ComponentGenerator_t<Scalar,
                                   //                                           DimensionOf_f<TensorProductA>::V,
                                   //                                           counting_vector_generator<Scalar,DimensionOf_f<TensorProductA>::V>,
                                   //                                           CountingVectorGeneratorId<DimensionOf_f<TensorProductA>::V> > > > ImplementationA;
        typedef ImplementationOf_t<TensorProductB,
                                   Scalar,
                                   UseProceduralArray_t<ComponentGenerator_Constant_f<Scalar,1,3>::T> > ImplementationB;
                                   // UseProceduralArray_t<ComponentGenerator_t<Scalar,
                                   //                                           DimensionOf_f<TensorProductB>::V,
                                   //                                           counting_vector_generator<Scalar,DimensionOf_f<TensorProductB>::V>,
                                   //                                           CountingVectorGeneratorId<DimensionOf_f<TensorProductB>::V> > > > ImplementationB;
        {
            std::cout << FORMAT_VALUE(ImplementationA()) << '\n';
            typedef TensorProductOfProcedural2Tensors_f<Typle_t<ImplementationA>>::T TProdOfImpl;
            TProdOfImpl t;
            AbstractIndex_c<'i'> i;
            AbstractIndex_c<'j'> j;
            std::cout << FORMAT_VALUE(t) << '\n';
            std::cout << FORMAT_VALUE(t.split(i*j)) << '\n';
        }
        {
            std::cout << FORMAT_VALUE(ImplementationB()) << '\n';
            typedef TensorProductOfProcedural2Tensors_f<Typle_t<ImplementationB>>::T TProdOfImpl;
            TProdOfImpl t;
            AbstractIndex_c<'i'> i;
            AbstractIndex_c<'j'> j;
            std::cout << FORMAT_VALUE(t) << '\n';
            std::cout << FORMAT_VALUE(t.split(i*j)) << '\n';
        }
        {
            std::cout << FORMAT_VALUE(ImplementationB()) << '\n';
            typedef TensorProductOfProcedural2Tensors_f<Typle_t<ImplementationA,ImplementationB>>::T TProdOfImpl;
            TProdOfImpl t;
            AbstractIndex_c<'A'> A;
            AbstractIndex_c<'P'> P;
            AbstractIndex_c<'Q'> Q;
            AbstractIndex_c<'i'> i;
            AbstractIndex_c<'j'> j;
            AbstractIndex_c<'k'> k;
            AbstractIndex_c<'l'> l;
            std::cout << FORMAT_VALUE(t) << '\n';
            std::cout << FORMAT_VALUE(t.split(i*j)) << '\n';
            std::cout << "this seems right.\n";
            std::cout << FORMAT_VALUE(t(A).split(A,P*Q)) << '\n';
            std::cout << "this doesn't seem quite right, though it probably requires i*j*k*l to be permuted to i*k*j*l.\n";
            std::cout << FORMAT_VALUE(t(A).split(A,P*Q).split(P,i*j).split(Q,k*l)) << '\n';
        }
        std::cout << '\n';
    }

    {
        std::cout << "testing InnerProduct_f\n";
        decltype(bvs(generic_real_vs<3>(), o_n_basis(Generic()))) b;
        typedef double Scalar;
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'p'> p;
        InnerProduct_f<decltype(b),StandardInnerProduct,Scalar>::T g;
        std::cout << FORMAT_VALUE(g) << '\n';
        std::cout << FORMAT_VALUE(g.embed(dual(b*b),p).split(p,i*j)) << '\n';
        decltype(b*b) t2;
        InnerProduct_f<decltype(t2),TensorProduct_c<Typle_t<StandardInnerProduct,StandardInnerProduct>>,Scalar>::T g_t2;
        std::cout << FORMAT_VALUE(g_t2) << '\n';
        std::cout << FORMAT_VALUE(g_t2.embed(dual(t2*t2),p).split(p,i*j)) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing EuclideanEmbedding_f\n";
        decltype(bvs(generic_real_vs<3>(), o_n_basis(Generic()))) b;
        typedef double Scalar;
        // AbstractIndex_c<'i'> i;
        // AbstractIndex_c<'j'> j;
        // AbstractIndex_c<'p'> p;
        EuclideanEmbedding_f<decltype(b),StandardInnerProduct,Scalar>::T e;
        std::cout << FORMAT_VALUE(e) << '\n';
        // std::cout << FORMAT_VALUE(e.embed(dual(b*b),p).split(p,i*j)) << '\n';
        decltype(b*b) t2;
        EuclideanEmbedding_f<decltype(t2),TensorProduct_c<Typle_t<StandardInnerProduct,StandardInnerProduct>>,Scalar>::T e_t2;
        std::cout << FORMAT_VALUE(e_t2) << '\n';
        // std::cout << FORMAT_VALUE(e_t2.embed(dual(t2*t2),p).split(p,i*j)) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing Alt_f\n";
        decltype(bvs(generic_real_vs<4>(), generic_basis())) b;
        typedef double Scalar;
        Alt_f<3,decltype(b),Scalar>::T alt;
        std::cout << FORMAT_VALUE(alt) << '\n';

        std::cout << '\n';
    }

    {
        std::cout << "testing Sym_f\n";
        decltype(bvs(generic_real_vs<4>(), generic_basis())) b;
        typedef double Scalar;
        Sym_f<3,decltype(b),Scalar>::T sym;
        std::cout << FORMAT_VALUE(sym) << '\n';

        std::cout << '\n';
    }

    return 0;
}
