#include <cassert>
#include <iostream>
#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"

using namespace Tenh;

struct X
{
    static std::string type_as_string () { return "X"; }
};

struct Y
{
    static std::string type_as_string () { return "Y"; }
};

struct Z
{
    static std::string type_as_string () { return "Z"; }
};

int main (int argc, char **argv)
{
    {
        typedef Basis_c<X> Basis;
        typedef Basis::Dual DualBasis;
        typedef DualBasis::Dual DualDualBasis;
        std::cout << "Basis = "  << TypeStringOf_t<Basis>::eval() << '\n'
                  << "DualBasis = " << TypeStringOf_t<DualBasis>::eval()  << '\n'
                  << "DualOf_c<Basis>::T = " << TypeStringOf_t<DualOf_c<Basis>::T>::eval() << '\n'
                  << "DualDualBasis = " << TypeStringOf_t<DualDualBasis>::eval() << '\n' << '\n';
        // make sure the basis is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<Basis,DualDualBasis>::v));
        // make sure that Basis::Dual and DualOf_c<Basis>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualBasis,DualOf_c<Basis>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,3,X> VectorSpace;
        typedef VectorSpace::Dual DualVectorSpace;
        typedef DualVectorSpace::Dual DualDualVectorSpace;
        std::cout << "VectorSpace = "  << TypeStringOf_t<VectorSpace>::eval() << '\n'
                  << "DualVectorSpace = " << TypeStringOf_t<DualVectorSpace>::eval() << '\n' 
                  << "DualOf_c<VectorSpace>::T = " << TypeStringOf_t<DualOf_c<VectorSpace>::T>::eval() << '\n'
                  << "DualDualVectorSpace = " << TypeStringOf_t<DualDualVectorSpace>::eval() << '\n' << '\n';
        // make sure the vector space is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<VectorSpace,DualDualVectorSpace>::v));
        // make sure that VectorSpace::Dual and DualOf_c<VectorSpace>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualVectorSpace,DualOf_c<VectorSpace>::T>::v));
    }

    {
        typedef TypeList_t<X,TypeList_t<int,TypeList_t<float> > > TypeList;
        typedef TensorProduct_c<TypeList> TensorProduct;
        typedef TensorProduct::Dual DualTensorProduct;
        typedef DualTensorProduct::Dual DualDualTensorProduct;
        std::cout << "TensorProduct = "  << TypeStringOf_t<TensorProduct>::eval() << '\n'
                  << "DualTensorProduct = " << TypeStringOf_t<DualTensorProduct>::eval() << '\n' 
                  << "DualOf_c<TensorProduct>::T = " << TypeStringOf_t<DualOf_c<TensorProduct>::T>::eval() << '\n'
                  << "DualDualTensorProduct = " << TypeStringOf_t<DualDualTensorProduct>::eval() << '\n' << '\n';
        // make sure the tensor product is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<TensorProduct,DualDualTensorProduct>::v));
        // make sure that TensorProduct::Dual and DualOf_c<TensorProduct>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualTensorProduct,DualOf_c<TensorProduct>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,3,X> X3;
        typedef VectorSpace_c<RealField,4,Y> Y4;
        typedef VectorSpace_c<RealField,5,Z> Z5;
        typedef TensorProductOfVectorSpaces_c<TypeList_t<X3,TypeList_t<Y4,TypeList_t<Z5> > > > T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "TensorProductOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorProductOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<TensorProductOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualTensorProductOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor product of vector spaces is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    {
        typedef Basis_c<X> BX;
        typedef Basis_c<Y> BY;
        typedef Basis_c<Z> BZ;
        typedef TensorProductOfBases_c<TypeList_t<BX,TypeList_t<BY,TypeList_t<BZ> > > > T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "TensorProductOfBases = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorProductOfBases = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<TensorProductOfBases>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualTensorProductOfBases = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor product of bases is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    return 0;
}
