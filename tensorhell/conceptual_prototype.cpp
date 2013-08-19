#include <cassert>
#include <iostream>

#include "tenh/core.hpp"

#include "tenh/array.hpp"
#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/exteriorpower.hpp"
#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/conceptual/tensorpower.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/expressiontemplate_eval.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/wedge.hpp"

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

namespace Tenh {


} // end of namespace Tenh



using namespace Tenh;

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
        // BasedVectorSpace::Index i;
        typedef BasedVectorSpace::Dual DualBasedVectorSpace;
        typedef DualBasedVectorSpace::Dual DualDualBasedVectorSpace;
        std::cout << "BasedVectorSpace = "  << TypeStringOf_t<BasedVectorSpace>::eval() << '\n'
                  // << "BasedVectorSpace::Index = " << TypeStringOf_t<BasedVectorSpace::Index>::eval() << '\n'
                  // << "BasedVectorSpace::Index::Dual = " << TypeStringOf_t<BasedVectorSpace::Index::Dual>::eval() << '\n'
                  // << "BasedVectorSpace::Dual::Index = " << TypeStringOf_t<BasedVectorSpace::Dual::Index>::eval() << '\n'
                  << "DualBasedVectorSpace = " << TypeStringOf_t<DualBasedVectorSpace>::eval() << '\n'
                  << "DualOf_c<BasedVectorSpace>::T = " << TypeStringOf_t<DualOf_c<BasedVectorSpace>::T>::eval() << '\n'
                  << "DualDualBasedVectorSpace = " << TypeStringOf_t<DualDualBasedVectorSpace>::eval() << '\n' << '\n';
        // make sure the vector space is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<BasedVectorSpace,DualDualBasedVectorSpace>::v));
        // make sure that BasedVectorSpace::Dual and DualOf_c<BasedVectorSpace>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualBasedVectorSpace,DualOf_c<BasedVectorSpace>::T>::v));
        // make sure that BasedVectorSpace::Dual::Index and BasedVectorSpace::Dual::Index are the same
        // assert((Lvd::Meta::TypesAreEqual<BasedVectorSpace::Dual::Index,BasedVectorSpace::Index::Dual>::v));
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

    ////////////////////////////////

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

    ////////////////////////////////

    {
        typedef SymmetricPower_c<X,3> SymmetricPower;
        typedef SymmetricPower::Dual DualSymmetricPower;
        typedef DualSymmetricPower::Dual DualDualSymmetricPower;
        std::cout << "SymmetricPower = "  << TypeStringOf_t<SymmetricPower>::eval() << '\n'
                  << "DualSymmetricPower = " << TypeStringOf_t<DualSymmetricPower>::eval() << '\n'
                  << "DualOf_c<SymmetricPower>::T = " << TypeStringOf_t<DualOf_c<SymmetricPower>::T>::eval() << '\n'
                  << "DualDualSymmetricPower = " << TypeStringOf_t<DualDualSymmetricPower>::eval() << '\n' << '\n';
        // make sure the tensor power is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<SymmetricPower,DualDualSymmetricPower>::v));
        // make sure that SymmetricPower::Dual and DualOf_c<SymmetricPower>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualSymmetricPower,DualOf_c<SymmetricPower>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef SymmetricPowerOfVectorSpaces_c<X5,3> T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "SymmetricPowerOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualSymmetricPowerOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<SymmetricPowerOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualSymmetricPowerOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of vector spaces is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that T::Dual and DualOf_c<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
    }

    {
        typedef Basis_c<X> BX;
        typedef SymmetricPowerOfBases_c<BX,3> T;
        typedef T::Dual DualT;
        typedef DualT::Dual DualDualT;
        std::cout << "SymmetricPowerOfBases = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualSymmetricPowerOfBases = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_c<SymmetricPowerOfBases>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                  << "DualDualSymmetricPowerOfBases = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
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

        typedef SymmetricPowerOfVectorSpaces_c<X5,3> TVS;
        assert(IsAVectorSpace_c<TVS>::V);
        assert(IsASymmetricPower_c<TVS>::V);
        assert(IsASymmetricPowerOfVectorSpaces_c<TVS>::V);

        typedef SymmetricPowerOfBases_c<BX,3> TB;
        assert(IsABasis_c<TB>::V);
        assert(IsASymmetricPower_c<TB>::V);
/*
        {
            typedef BasedSymmetricPowerOfVectorSpaces_c<TVS,TB> T;
            assert(IsASymmetricPower_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsASymmetricPowerOfVectorSpaces_c<T>::V);
            assert(IsABasedSymmetricPowerOfVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "BasedSymmetricPowerOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualBasedSymmetricPowerOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<BasedSymmetricPowerOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualBasedSymmetricPowerOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the based tensor power of vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }
*/
        {
            typedef SymmetricPowerOfBasedVectorSpaces_c<BasedX,3> T;
            assert(IsASymmetricPower_c<T>::V);
            assert(IsAVectorSpace_c<T>::V);
            assert(IsABasedVectorSpace_c<T>::V);
            assert(IsASymmetricPowerOfVectorSpaces_c<T>::V);
            assert(IsASymmetricPowerOfBasedVectorSpaces_c<T>::V);

            typedef T::Dual DualT;
            typedef DualT::Dual DualDualT;
            std::cout << "SymmetricPowerOfBasedVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualSymmetricPowerOfBasedVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_c<SymmetricPowerOfBasedVectorSpaces>::T = " << TypeStringOf_t<DualOf_c<T>::T>::eval() << '\n'
                      << "DualDualSymmetricPowerOfBasedVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the tensor power of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that T::Dual and DualOf_c<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_c<T>::T>::v));
        }
    }

    std::cout << '\n' << '\n';

    {
        typedef VectorSpace_c<RealField,3,X> VSX;
        typedef Basis_c<X> B;
        typedef BasedVectorSpace_c<VSX,B> BasedX;

        typedef ImplementationOf_t<float,BasedX> V;
        V v(1.0f, 2.0f, 3.0f);
        V w(8.0f, -2.0f, 6.0f);
        std::cout << TypeStringOf_t<V>::eval() << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(w) << '\n';

        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(v(i)) << '\n';
        std::cout << FORMAT_VALUE(w(i)) << '\n';
        std::cout << FORMAT_VALUE(v(i) + w(i)) << '\n';
        std::cout << FORMAT_VALUE(v(i) - w(i)) << '\n';

        V::Dual a(0.0f, 2.0f, -3.0f);
        std::cout << TypeStringOf_t<V::Dual>::eval() << '\n';
        std::cout << FORMAT_VALUE(a) << '\n';
        std::cout << FORMAT_VALUE(a(i)*v(i)) << '\n';
        std::cout << FORMAT_VALUE(v(i)*a(i)) << '\n';
        std::cout << FORMAT_VALUE(v(i)*a(j)) << '\n';

        // this should cause a compile error due to the non-naturality of the pairing
//        std::cout << FORMAT_VALUE(v(i)*w(i)) << '\n';

        // this should cause a compile error due to a mismatch of factor types
//        std::cout << FORMAT_VALUE(a(i) + v(i)) << '\n';

        std::cout << '\n' << '\n';
    }

    {
        typedef VectorSpace_c<RealField,3,X> VSX;
        typedef Basis_c<X> BX;
        typedef BasedVectorSpace_c<VSX,BX> BasedX;

        typedef VectorSpace_c<RealField,2,Y> VSY;
        typedef Basis_c<Y> BY;
        typedef BasedVectorSpace_c<VSY,BY> BasedY;

        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<BasedY::Dual> > > TPBVS;
        typedef ImplementationOf_t<float,BasedX> U;
        typedef ImplementationOf_t<float,BasedY> V;
        typedef ImplementationOf_t<float,TPBVS> T;
        std::cout << TypeStringOf_t<T>::eval() << '\n';

        T t(3.0f);
        std::cout << FORMAT_VALUE(t) << '\n';
        t[T::MultiIndex(0,0)] = 0.0f;
        t[T::MultiIndex(1,0)] = 4.0f;
        t[T::MultiIndex(2,0)] = -2.0f;
        t[T::MultiIndex(0,1)] = 3.0f;
        t[T::MultiIndex(1,1)] = 4.0f;
        t[T::MultiIndex(2,1)] = 5.0f;
        std::cout << "after assignment: " << FORMAT_VALUE(t) << '\n';

        V v(2.0f, 7.0f);
        std::cout << FORMAT_VALUE(v) << '\n';

        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(t(i|j)*v(j)) << '\n';

        U u(1.0f, 0.0f, -1.0f);
        std::cout << FORMAT_VALUE(u) << '\n';
        // this should cause a compile error due to the non-naturality of the pairing
//        std::cout << FORMAT_VALUE(u(i)*t(i|j)) << '\n';


        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<BasedY::Dual,TypeList_t<BasedY::Dual> > > > H;
        typedef ImplementationOf_t<float,H> E;
        E e(0.0f);
        for (E::ComponentIndex it; it.is_not_at_end(); ++it)
            e[it] = it.value();
        std::cout << FORMAT_VALUE(e(i)) << '\n'; // this has problems: TODO: fix -- ExpressionTemplate_i may need a vector-indexable operator[]

        V w(1.0f, 3.0f);

        AbstractIndex_c<'k'> k;
        std::cout << FORMAT_VALUE(w) << '\n';
        std::cout << FORMAT_VALUE(e) << '\n';
        std::cout << FORMAT_VALUE(e(i|j|k)*v(j)*w(k)) << '\n';

        // this should cause a compile error due to the non-naturality of the pairing
//        std::cout << FORMAT_VALUE(e(i|j|j)) << '\n';

        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedY,TypeList_t<BasedY::Dual> > > EndomorphismOfY;
        typedef ImplementationOf_t<float,EndomorphismOfY> Endo;
        Endo A(0.0f);
        A[Endo::MultiIndex(0,0)] = 3.0f;
        A[Endo::MultiIndex(1,1)] = 4.0f;
        std::cout << FORMAT_VALUE(A) << '\n';
        std::cout << FORMAT_VALUE(A(i|j)*v(j)) << '\n';
        std::cout << FORMAT_VALUE(A(i|i)) << '\n';


        E::Dual f(3.0f);
        f[E::Dual::MultiIndex(0,0,0,CHECK_RANGE)] = 1.0f;
        f[E::Dual::MultiIndex(1,0,1,CHECK_RANGE)] = 2.0f;
        f[E::Dual::MultiIndex(2,1,1,CHECK_RANGE)] = 4.0f;
        std::cout << FORMAT_VALUE(f) << '\n';
        std::cout << FORMAT_VALUE(f(i|j|k)*e(i|j|k)) << '\n';
        std::cout << FORMAT_VALUE(f(i|j|k)*e(i|k|j)) << '\n';
        std::cout << FORMAT_VALUE(f(i)*e(i)) << '\n';

        std::cout << FORMAT_VALUE(e(i).eval()) << '\n';
        std::cout << FORMAT_VALUE(e(i|j|k).eval()) << '\n';

        AbstractIndex_c<'l'> l;
        std::cout << FORMAT_VALUE(e(i).split(i,j|k|l)) << '\n';


        AbstractIndex_c<'P'> P;
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedY::Dual,TypeList_t<BasedY::Dual> > > PartOfH;
        typedef ImplementationOf_t<float,PartOfH> G;
        std::cout << FORMAT_VALUE(e(i|j|k).bundle(j|k,PartOfH(),P)) << '\n';
        std::cout << FORMAT_VALUE(f(i|j|k).bundle(j|k,PartOfH::Dual(),P)) << '\n';
        std::cout << FORMAT_VALUE(e(i|j|k).bundle(j|k,PartOfH(),P) * f(i|j|k).bundle(j|k,PartOfH::Dual(),P)) << '\n';

        std::cout << '\n' << '\n';
    }

    {
        typedef VectorSpace_c<RealField,5,X> VSX;
        typedef Basis_c<X> B;
        typedef BasedVectorSpace_c<VSX,B> BasedX;

        STATIC_ASSERT(IsABasedVectorSpace_c<BasedX>::V, MUST_BE_BASED_VECTOR_SPACE);

        typedef ExteriorPowerOfBasedVectorSpaces_c<BasedX,3> Wedge3_BasedX;
        typedef ImplementationOf_t<float,Wedge3_BasedX> Wedge;

        Wedge w(0);
        w[Wedge::ComponentIndex(0, CHECK_RANGE)] = 6;
        w[Wedge::ComponentIndex(1, CHECK_RANGE)] = 12;
        w[Wedge::ComponentIndex(2, CHECK_RANGE)] = 18;
        w[Wedge::ComponentIndex(3, CHECK_RANGE)] = 24;
        w[Wedge::ComponentIndex(4, CHECK_RANGE)] = 30;
        w[Wedge::ComponentIndex(5, CHECK_RANGE)] = 36;
        w[Wedge::ComponentIndex(6, CHECK_RANGE)] = 42;
        w[Wedge::ComponentIndex(7, CHECK_RANGE)] = 48;
        w[Wedge::ComponentIndex(8, CHECK_RANGE)] = 54;
        w[Wedge::ComponentIndex(9, CHECK_RANGE)] = 60;

        std::cout << FORMAT_VALUE(w) << '\n';

        {
            AbstractIndex_c<'i'> i;
            AbstractIndex_c<'j'> j;
            AbstractIndex_c<'k'> k;
            AbstractIndex_c<'l'> l;
            std::cout << FORMAT_VALUE(w(i).split(i,j|k|l)) << '\n';
        }

        std::cout << '\n' << '\n';

        std::cout << "Round-trip convert from ComponentIndex\n";
        Wedge::ComponentIndex i(9);
        std::cout << FORMAT_VALUE(i) << '\n';
        Wedge::MultiIndex m = Wedge::bundle_index_map<Wedge::MultiIndex::IndexTypeList, Wedge::ComponentIndex>(i);
        std::cout << FORMAT_VALUE(m) << '\n';
        Wedge::ComponentIndex j = Wedge::vector_index_of(m);
        std::cout << FORMAT_VALUE(j) << "\n\n";

        std::cout << "Round-trip convert from MultiIndex\n";
        Wedge::MultiIndex n(4, 0, 3, CHECK_RANGE);
        std::cout << FORMAT_VALUE(n) << '\n';
        i = Wedge::vector_index_of(n);
        std::cout << FORMAT_VALUE(i) << '\n';
        n = Wedge::bundle_index_map<Wedge::MultiIndex::IndexTypeList, Wedge::ComponentIndex>(i);
        std::cout << FORMAT_VALUE(n) << '\n';
        i = Wedge::vector_index_of(n);
        std::cout << FORMAT_VALUE(i) << "\n\n";
        n = Wedge::bundle_index_map<Wedge::MultiIndex::IndexTypeList, Wedge::ComponentIndex>(i);
    }

    {
        typedef ComponentIndex_t<5> C;
        typedef UniformTypeListOfLength_t<C,3>::T IndexTypeList;
        typedef MultiIndex_t<IndexTypeList> MultiIndex;

        MultiIndex m; // default constructor uses default constructor of each component
        m.el<0>() = C(3);
        m.el<1>() = C(1);
        //m.el<2>() = C(4);
        m.index(2) = C(4);
        std::cout << FORMAT_VALUE(m) << '\n';
        sort<IndexTypeList,std::less<Uint32> >(m);
        std::cout << "after sort: " << FORMAT_VALUE(m) << '\n';

        std::cout << '\n' << '\n';
    }

    {
        typedef ComponentIndex_t<5> C;
        typedef UniformTypeListOfLength_t<C,1>::T IndexTypeList;
        typedef MultiIndex_t<IndexTypeList> MultiIndex;

        MultiIndex m; // default constructor uses default constructor of each component
        m.index(0) = C(4);
        std::cout << FORMAT_VALUE(m) << '\n';
        sort<IndexTypeList,std::less<Uint32> >(m);
        std::cout << "after sort: " << FORMAT_VALUE(m) << '\n';
    }

    std::cout << '\n' << '\n';

    return 0;
}
