#include <cassert>
#include <iostream>
#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/tensorpower.hpp"
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
        // make sure VectorSpace is actually a VectorSpace
        assert(IsAVectorSpace_c<VectorSpace>::V);
    }

    {
        typedef VectorSpace_c<RealField,3,X> VectorSpace;
        typedef Basis_c<Y> Basis;
        typedef BasedVectorSpace_c<VectorSpace,Basis> BasedVectorSpace;
        typedef BasedVectorSpace::Dual DualBasedVectorSpace;
        typedef DualBasedVectorSpace::Dual DualDualBasedVectorSpace;
        std::cout << "BasedVectorSpace = "  << TypeStringOf_t<BasedVectorSpace>::eval() << '\n'
                  << "DualBasedVectorSpace = " << TypeStringOf_t<DualBasedVectorSpace>::eval() << '\n' 
                  << "DualOf_c<BasedVectorSpace>::T = " << TypeStringOf_t<DualOf_c<BasedVectorSpace>::T>::eval() << '\n'
                  << "DualDualBasedVectorSpace = " << TypeStringOf_t<DualDualBasedVectorSpace>::eval() << '\n' << '\n';
        // make sure the vector space is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<BasedVectorSpace,DualDualBasedVectorSpace>::v));
        // make sure that BasedVectorSpace::Dual and DualOf_c<BasedVectorSpace>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualBasedVectorSpace,DualOf_c<BasedVectorSpace>::T>::v));
        // make sure BasedVectorSpace is actually a VectorSpace_c and a BasedVectorSpace_c
        assert(IsAVectorSpace_c<BasedVectorSpace>::V);
        assert(IsABasedVectorSpace_c<BasedVectorSpace>::V);
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

    {
        typedef VectorSpace_c<RealField,3,X> X3;
        typedef VectorSpace_c<RealField,4,Y> Y4;
        typedef VectorSpace_c<RealField,5,Z> Z5;
        typedef Basis_c<X> BX;
        typedef Basis_c<Y> BY;
        typedef Basis_c<Z> BZ;
        typedef BasedVectorSpace_c<X3,BX> BasedX;
        typedef BasedVectorSpace_c<Y4,BY> BasedY;
        typedef BasedVectorSpace_c<Z5,BZ> BasedZ;
        assert(IsABasedVectorSpace_c<BasedX>::V);
        assert(IsABasedVectorSpace_c<BasedY>::V);
        assert(IsABasedVectorSpace_c<BasedZ>::V);

        typedef TensorProductOfVectorSpaces_c<TypeList_t<X3,TypeList_t<Y4,TypeList_t<Z5> > > > TVS;
        assert(IsAVectorSpace_c<TVS>::V);
        assert(IsATensorProduct_c<TVS>::V);
        assert(IsATensorProductOfVectorSpaces_c<TVS>::V);

        typedef TensorProductOfBases_c<TypeList_t<BX,TypeList_t<BY,TypeList_t<BZ> > > > TB;
        assert(IsABasis_c<TB>::V);
        assert(IsATensorProduct_c<TB>::V);

        {
            typedef BasedTensorProductOfVectorSpaces_c<TVS,TB> T;
            assert(IsATensorProduct_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsATensorProductOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorProductOfVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "BasedTensorProductOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualBasedTensorProductOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<BasedTensorProductOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualBasedTensorProductOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the based tensor product of vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }

        {
            typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<BasedY,TypeList_t<BasedZ> > > > T;
            assert(IsATensorProduct_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsATensorProductOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorProductOfVectorSpaces_c<T>::V);
            assert(IsATensorProductOfBasedVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "TensorProductOfBasedVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualTensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<TensorProductOfBasedVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualTensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the tensor product of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }

        {
            typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedX,BasedY> T;
            assert(IsATensorProduct_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsATensorProductOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorProductOfVectorSpaces_c<T>::V);
            assert(IsATensorProductOfBasedVectorSpaces_c<T>::V);
            assert(IsADiagonal2TensorProductOfBasedVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "Diagonal2TensorProductOfBasedVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualDiagonal2TensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<Diagonal2TensorProductOfBasedVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualDiagonal2TensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the diagonal 2 tensor product of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }
    }

    {
        typedef TensorPower_c<X,3> TensorPower;
        typedef TensorPower::Dual DualTensorPower;
        typedef DualTensorPower::Dual DualDualTensorPower;
        std::cout << "TensorPower = "  << TypeStringOf_t<TensorPower>::eval() << '\n'
                  << "DualTensorPower = " << TypeStringOf_t<DualTensorPower>::eval() << '\n' 
                  << "DualOf_c<TensorPower>::T = " << TypeStringOf_t<DualOf_c<TensorPower>::T>::eval() << '\n'
                  << "DualDualTensorPower = " << TypeStringOf_t<DualDualTensorPower>::eval() << '\n' << '\n';
        // make sure the tensor power is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<TensorPower,DualDualTensorPower>::v));
        // make sure that TensorPower::Dual and DualOf_c<TensorPower>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualTensorPower,DualOf_c<TensorPower>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef TensorPowerOfVectorSpaces_c<X5,3> T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "TensorPowerOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorPowerOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<TensorPowerOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualTensorPowerOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of vector spaces is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    {
        typedef Basis_c<X> BX;
        typedef TensorPowerOfBases_c<BX,3> T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "TensorPowerOfBases = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorPowerOfBases = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<TensorPowerOfBases>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualTensorPowerOfBases = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of bases is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef Basis_c<X> BX;
        typedef BasedVectorSpace_c<X5,BX> BasedX;
        assert(IsABasedVectorSpace_c<BasedX>::V);

        typedef TensorPowerOfVectorSpaces_c<X5,3> TVS;
        assert(IsAVectorSpace_c<TVS>::V);
        assert(IsATensorProduct_c<TVS>::V);
        assert(IsATensorProductOfVectorSpaces_c<TVS>::V);
        assert(IsATensorPower_c<TVS>::V);
        assert(IsATensorPowerOfVectorSpaces_c<TVS>::V);

        typedef TensorPowerOfBases_c<BX,3> TB;
        assert(IsABasis_c<TB>::V);
        assert(IsATensorProduct_c<TB>::V);
        assert(IsATensorPower_c<TB>::V);

        {
            typedef BasedTensorPowerOfVectorSpaces_c<TVS,TB> T;
            assert(IsATensorProduct_c<T>::V);
            assert(IsATensorPower_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsATensorProductOfVectorSpaces_c<T>::V);
            assert(IsATensorPowerOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorProductOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorPowerOfVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "BasedTensorPowerOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualBasedTensorPowerOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<BasedTensorPowerOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualBasedTensorPowerOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the based tensor power of vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }

        {
            typedef TensorPowerOfBasedVectorSpaces_c<BasedX,3> T;
            assert(IsATensorProduct_c<T>::V);
            assert(IsATensorPower_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsATensorProductOfVectorSpaces_c<T>::V);
            assert(IsATensorPowerOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorProductOfVectorSpaces_c<T>::V);
            assert(IsABasedTensorPowerOfVectorSpaces_c<T>::V);
            assert(IsATensorProductOfBasedVectorSpaces_c<T>::V);
            assert(IsATensorPowerOfBasedVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "TensorPowerOfBasedVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualTensorPowerOfBasedVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<TensorPowerOfBasedVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualTensorPowerOfBasedVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the tensor power of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }
    }

    return 0;
}
