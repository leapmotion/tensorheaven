#include "conceptual_prototype.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 13
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_zero_vector ()
{
    std::cout << "zero vector\n";

    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,OrthonormalBasis_c<X> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,X>,OrthonormalBasis_c<X> > B4;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<B4> > > TensorProduct;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B4> Diagonal2TensorProduct;
    typedef DirectSumOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<B4> > > DirectSum;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,B3> Sym;
    typedef ExteriorPowerOfBasedVectorSpace_c<2,B3> Alt;

    typedef ImplementationOf_t<B3,float> V3;
    typedef ImplementationOf_t<B4,float> V4;
    typedef ImplementationOf_t<TensorProduct,float> T;
    typedef ImplementationOf_t<Diagonal2TensorProduct,float> D;
    typedef ImplementationOf_t<DirectSum,float> DS;
    typedef ImplementationOf_t<Sym,float> S;
    typedef ImplementationOf_t<Alt,float> A;

    std::cout << FORMAT(V3::ZERO) << '\n';
    std::cout << FORMAT(V4::ZERO) << '\n';
    std::cout << FORMAT(T::ZERO) << '\n';
    std::cout << FORMAT(D::ZERO) << '\n';
    std::cout << FORMAT(DS::ZERO) << '\n';
    std::cout << FORMAT(S::ZERO) << '\n';
    std::cout << FORMAT(A::ZERO) << '\n';
}

void test_basis_vectors ()
{
    std::cout << "basis vectors\n";

    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,OrthonormalBasis_c<X> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,X>,OrthonormalBasis_c<X> > B4;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<B4> > > TensorProduct;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B4> Diagonal2TensorProduct;
    typedef DirectSumOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<B4> > > DirectSum;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,B3> Sym;
    typedef ExteriorPowerOfBasedVectorSpace_c<2,B3> Alt;

    typedef ImplementationOf_t<B3,float> V3;
    typedef ImplementationOf_t<B4,float> V4;
    typedef ImplementationOf_t<TensorProduct,float> T;
    typedef ImplementationOf_t<Diagonal2TensorProduct,float> D;
    typedef ImplementationOf_t<DirectSum,float> DS;
    typedef ImplementationOf_t<Sym,float> S;
    typedef ImplementationOf_t<Alt,float> A;

    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;

    std::cout << "3-vector\n";
    std::cout << FORMAT(V3::BasisVector_f<0>::V) << '\n';
    std::cout << FORMAT(V3::BasisVector_f<1>::V) << '\n';
    std::cout << FORMAT(V3::BasisVector_f<2>::V) << '\n';
    std::cout << '\n';

    std::cout << "4-vector\n";
    std::cout << FORMAT(V4::BasisVector_f<0>::V) << '\n';
    std::cout << FORMAT(V4::BasisVector_f<1>::V) << '\n';
    std::cout << FORMAT(V4::BasisVector_f<2>::V) << '\n';
    std::cout << FORMAT(V4::BasisVector_f<3>::V) << '\n';
    std::cout << '\n';

    std::cout << "tensor product\n";
    std::cout << FORMAT(T::BasisVector_f<0>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<1>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<2>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<3>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<4>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<5>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<6>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<7>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<8>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<9>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<10>::V) << '\n';
    std::cout << FORMAT(T::BasisVector_f<11>::V) << '\n';
    std::cout << '\n';

    std::cout << "diagonal\n";
    std::cout << FORMAT(D::BasisVector_f<0>::V) << '\n';
    std::cout << FORMAT(D::BasisVector_f<1>::V) << '\n';
    std::cout << FORMAT(D::BasisVector_f<2>::V) << '\n';
    std::cout << '\n';
    std::cout << "diagonal -- split\n";
    std::cout << FORMAT(D::BasisVector_f<0>::V(P).split(P,i|j)) << '\n';
    std::cout << FORMAT(D::BasisVector_f<1>::V(P).split(P,i|j)) << '\n';
    std::cout << FORMAT(D::BasisVector_f<2>::V(P).split(P,i|j)) << '\n';
    std::cout << '\n';

    std::cout << "direct sum\n";
    std::cout << FORMAT(DS::BasisVector_f<0>::V) << '\n';
    std::cout << FORMAT(DS::BasisVector_f<1>::V) << '\n';
    std::cout << FORMAT(DS::BasisVector_f<2>::V) << '\n';
    std::cout << FORMAT(DS::BasisVector_f<3>::V) << '\n';
    std::cout << FORMAT(DS::BasisVector_f<4>::V) << '\n';
    std::cout << FORMAT(DS::BasisVector_f<5>::V) << '\n';
    std::cout << FORMAT(DS::BasisVector_f<6>::V) << '\n';
    std::cout << '\n';

    std::cout << "symmetric\n";
    std::cout << FORMAT(S::BasisVector_f<0>::V) << '\n';
    std::cout << FORMAT(S::BasisVector_f<1>::V) << '\n';
    std::cout << FORMAT(S::BasisVector_f<2>::V) << '\n';
    std::cout << FORMAT(S::BasisVector_f<3>::V) << '\n';
    std::cout << FORMAT(S::BasisVector_f<4>::V) << '\n';
    std::cout << FORMAT(S::BasisVector_f<5>::V) << '\n';
    std::cout << "symmetric -- split\n";
    std::cout << FORMAT(S::BasisVector_f<0>::V(P).split(P,i|j)) << '\n';
    std::cout << FORMAT(S::BasisVector_f<1>::V(P).split(P,i|j)) << '\n';
    std::cout << FORMAT(S::BasisVector_f<2>::V(P).split(P,i|j)) << '\n';
    std::cout << FORMAT(S::BasisVector_f<3>::V(P).split(P,i|j)) << '\n';
    std::cout << FORMAT(S::BasisVector_f<4>::V(P).split(P,i|j)) << '\n';
    std::cout << FORMAT(S::BasisVector_f<5>::V(P).split(P,i|j)) << '\n';
    std::cout << '\n';

    std::cout << "alternating\n";
    std::cout << FORMAT(A::BasisVector_f<0>::V) << '\n';
    std::cout << FORMAT(A::BasisVector_f<1>::V) << '\n';
    std::cout << FORMAT(A::BasisVector_f<2>::V) << '\n';
    std::cout << "alternating -- split\n";
    std::cout << FORMAT(A::BasisVector_f<0>::V(P).split(P,i|j)) << '\n';
    std::cout << FORMAT(A::BasisVector_f<1>::V(P).split(P,i|j)) << '\n';
    std::cout << FORMAT(A::BasisVector_f<2>::V(P).split(P,i|j)) << '\n';
    std::cout << '\n';
}
