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
#include "tenh/implementation/diagonal2tensor.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"
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

template<typename Vector, typename Scalar, Uint32 ORDER>
ImplementationOf_t<Scalar,SymmetricPowerOfBasedVectorSpace_c<Vector,ORDER> > test_vector_power (const ImplementationOf_t<Scalar, Vector> &input)
{
//    STATIC_ASSERT(IS_BASED_VECTORSPACE_UNIQUELY(Vector), MUST_BE_BASED_VECTOR_SPACE);
    typedef ImplementationOf_t<Scalar,SymmetricPowerOfBasedVectorSpace_c<Vector,ORDER> > Sym;
    typedef ImplementationOf_t<Scalar, Vector> Vec;

    Sym result(Scalar(1));

    for (typename Sym::ComponentIndex it; it.is_not_at_end(); ++it)
    {
        //result[it] = Scalar(1);
        typename Sym::MultiIndex m = Sym::template bundle_index_map<typename Sym::MultiIndex::IndexTypeList, typename Sym::ComponentIndex>(it);
        for (Uint32 i = 0; i < Sym::MultiIndex::LENGTH; ++i)
        {
            result[it] *= input[typename Vec::ComponentIndex(m.value_of_index(i, DONT_CHECK_RANGE))];
        }
    }
    return result;
}

template <typename BasedVectorSpace, Uint32 ORDER>
void test_tensor_printing (std::ostream &out)
{
    typedef TensorPowerOfBasedVectorSpace_c<BasedVectorSpace,ORDER> TPow;
    typedef typename AS_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES(TPow) TProd;
    typedef ImplementationOf_t<float,TProd> T;
    T t(Static<WithoutInitialization>::SINGLETON);
    for (typename T::ComponentIndex i; i.is_not_at_end(); ++i)
        t[i] = i.value();

    out << FORMAT_VALUE(t) << '\n';
}

int main (int argc, char **argv)
{
    {
        typedef Basis_c<X> Basis;
        typedef DualOf_f<Basis>::T DualBasis;
        typedef DualOf_f<DualBasis>::T DualDualBasis;
        std::cout << "Basis = "  << TypeStringOf_t<Basis>::eval() << '\n'
                  << "DualBasis = " << TypeStringOf_t<DualBasis>::eval()  << '\n'
                  << "DualOf_f<Basis>::T = " << TypeStringOf_t<DualOf_f<Basis>::T>::eval() << '\n'
                  << "DualDualBasis = " << TypeStringOf_t<DualDualBasis>::eval() << '\n' << '\n';
        // make sure the basis is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<Basis,DualDualBasis>::v));
        // make sure that DualOf_f<Basis>::T and DualOf_f<Basis>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualBasis,DualOf_f<Basis>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,3,X> VectorSpace;
        typedef DualOf_f<VectorSpace>::T DualVectorSpace;
        typedef DualOf_f<DualVectorSpace>::T DualDualVectorSpace;
        std::cout << "VectorSpace = "  << TypeStringOf_t<VectorSpace>::eval() << '\n'
                  << "DualVectorSpace = " << TypeStringOf_t<DualVectorSpace>::eval() << '\n'
                  << "DualOf_f<VectorSpace>::T = " << TypeStringOf_t<DualOf_f<VectorSpace>::T>::eval() << '\n'
                  << "DualDualVectorSpace = " << TypeStringOf_t<DualDualVectorSpace>::eval() << '\n' << '\n';
        // make sure the vector space is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<VectorSpace,DualDualVectorSpace>::v));
        // make sure that DualOf_f<VectorSpace>::T and DualOf_f<VectorSpace>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualVectorSpace,DualOf_f<VectorSpace>::T>::v));
        // make sure VectorSpace is actually a VectorSpace
        assert(HasVectorSpaceStructure_f<VectorSpace>::V);
    }

    {
        typedef VectorSpace_c<RealField,3,X> VectorSpace;
        typedef Basis_c<Y> Basis;
        typedef BasedVectorSpace_c<VectorSpace,Basis> BasedVectorSpace;
        // BasedVectorSpace::Index i;
        typedef DualOf_f<BasedVectorSpace>::T DualBasedVectorSpace;
        typedef DualOf_f<DualBasedVectorSpace>::T DualDualBasedVectorSpace;
        std::cout << "BasedVectorSpace = "  << TypeStringOf_t<BasedVectorSpace>::eval() << '\n'
                  // << "BasedVectorSpace::Index = " << TypeStringOf_t<BasedVectorSpace::Index>::eval() << '\n'
                  // << "DualOf_f<BasedVectorSpace::Index>::T = " << TypeStringOf_t<DualOf_f<BasedVectorSpace::Index>::T>::eval() << '\n'
                  // << "DualOf_f<BasedVectorSpace>::T::Index = " << TypeStringOf_t<DualOf_f<BasedVectorSpace>::T::Index>::eval() << '\n'
                  << "DualBasedVectorSpace = " << TypeStringOf_t<DualBasedVectorSpace>::eval() << '\n'
                  << "DualOf_f<BasedVectorSpace>::T = " << TypeStringOf_t<DualOf_f<BasedVectorSpace>::T>::eval() << '\n'
                  << "DualDualBasedVectorSpace = " << TypeStringOf_t<DualDualBasedVectorSpace>::eval() << '\n' << '\n';
        // make sure the vector space is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<BasedVectorSpace,DualDualBasedVectorSpace>::v));
        // make sure that DualOf_f<BasedVectorSpace>::T and DualOf_f<BasedVectorSpace>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualBasedVectorSpace,DualOf_f<BasedVectorSpace>::T>::v));
        // make sure that DualOf_f<BasedVectorSpace>::T::Index and DualOf_f<BasedVectorSpace>::T::Index are the same
        // assert((Lvd::Meta::TypesAreEqual<DualOf_f<BasedVectorSpace>::T::Index,DualOf_f<BasedVectorSpace::Index>::T>::v));
        // make sure BasedVectorSpace is actually a VectorSpace_c and a BasedVectorSpace_c
        assert(HasVectorSpaceStructure_f<BasedVectorSpace>::V);
        assert(HasBasedVectorSpaceStructure_f<BasedVectorSpace>::V);
    }

    {
        typedef TypeList_t<X,TypeList_t<int,TypeList_t<float> > > TypeList;
        typedef TensorProduct_c<TypeList> TensorProduct;
        typedef DualOf_f<TensorProduct>::T DualTensorProduct;
        typedef DualOf_f<DualTensorProduct>::T DualDualTensorProduct;
        std::cout << "TensorProduct = "  << TypeStringOf_t<TensorProduct>::eval() << '\n'
                  << "DualTensorProduct = " << TypeStringOf_t<DualTensorProduct>::eval() << '\n'
                  << "DualOf_f<TensorProduct>::T = " << TypeStringOf_t<DualOf_f<TensorProduct>::T>::eval() << '\n'
                  << "DualDualTensorProduct = " << TypeStringOf_t<DualDualTensorProduct>::eval() << '\n' << '\n';
        // make sure the tensor product is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<TensorProduct,DualDualTensorProduct>::v));
        // make sure that DualOf_f<TensorProduct>::T and DualOf_f<TensorProduct>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualTensorProduct,DualOf_f<TensorProduct>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,3,X> X3;
        typedef VectorSpace_c<RealField,4,Y> Y4;
        typedef VectorSpace_c<RealField,5,Z> Z5;
        typedef TensorProductOfVectorSpaces_c<TypeList_t<X3,TypeList_t<Y4,TypeList_t<Z5> > > > T;
        typedef DualOf_f<T>::T DualT;
        typedef DualOf_f<DualT>::T DualDualT;
        std::cout << "TensorProductOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorProductOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_f<TensorProductOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                  << "DualDualTensorProductOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor product of vector spaces is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
    }

    {
        typedef Basis_c<X> BX;
        typedef Basis_c<Y> BY;
        typedef Basis_c<Z> BZ;
        typedef TensorProductOfBases_c<TypeList_t<BX,TypeList_t<BY,TypeList_t<BZ> > > > T;
        typedef DualOf_f<T>::T DualT;
        typedef DualOf_f<DualT>::T DualDualT;
        std::cout << "TensorProductOfBases = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorProductOfBases = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_f<TensorProductOfBases>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                  << "DualDualTensorProductOfBases = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor product of bases is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
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
        assert(HasBasedVectorSpaceStructure_f<BasedX>::V);
        assert(HasBasedVectorSpaceStructure_f<BasedY>::V);
        assert(HasBasedVectorSpaceStructure_f<BasedZ>::V);

        typedef TensorProductOfVectorSpaces_c<TypeList_t<X3,TypeList_t<Y4,TypeList_t<Z5> > > > TVS;
        assert(HasVectorSpaceStructure_f<TVS>::V);
        assert(HasTensorProductStructure_f<TVS>::V);
        assert(HasTensorProductOfVectorSpacesStructure_f<TVS>::V);

        typedef TensorProductOfBases_c<TypeList_t<BX,TypeList_t<BY,TypeList_t<BZ> > > > TB;
        assert(HasBasisStructure_f<TB>::V);
        assert(HasTensorProductStructure_f<TB>::V);

        {
            typedef BasedTensorProductOfVectorSpaces_c<TVS,TB> T;
            assert(HasTensorProductStructure_f<T>::V);
            assert(HasVectorSpaceStructure_f<T>::V);
            assert(HasBasedVectorSpaceStructure_f<T>::V);
            assert(HasTensorProductOfVectorSpacesStructure_f<T>::V);
            assert(HasBasedTensorProductOfVectorSpacesStructure_f<T>::V);

            typedef DualOf_f<T>::T DualT;
            typedef DualOf_f<DualT>::T DualDualT;
            std::cout << "BasedTensorProductOfVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualBasedTensorProductOfVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_f<BasedTensorProductOfVectorSpaces>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                      << "DualDualBasedTensorProductOfVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the based tensor product of vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
        }

        {
            typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<BasedY,TypeList_t<BasedZ> > > > T;
            assert(HasTensorProductStructure_f<T>::V);
            assert(HasVectorSpaceStructure_f<T>::V);
            assert(HasBasedVectorSpaceStructure_f<T>::V);
            assert(HasTensorProductOfVectorSpacesStructure_f<T>::V);
            assert(HasBasedTensorProductOfVectorSpacesStructure_f<T>::V);
            assert(HasTensorProductOfBasedVectorSpacesStructure_f<T>::V);

            typedef DualOf_f<T>::T DualT;
            typedef DualOf_f<DualT>::T DualDualT;
            std::cout << "TensorProductOfBasedVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualTensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_f<TensorProductOfBasedVectorSpaces>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                      << "DualDualTensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the tensor product of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
        }

        {
            typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedX,BasedY> T;
//            assert(HasTensorProductStructure_f<T>::V); I don't think this is true.
            assert(HasVectorSpaceStructure_f<T>::V);
            assert(HasBasedVectorSpaceStructure_f<T>::V);
//            assert(HasTensorProductOfVectorSpacesStructure_f<T>::V);
//            assert(HasBasedTensorProductOfVectorSpacesStructure_f<T>::V);
//            assert(HasTensorProductOfBasedVectorSpacesStructure_f<T>::V);
            assert(HasDiagonal2TensorProductOfBasedVectorSpacesStructure_f<T>::V);

            typedef DualOf_f<T>::T DualT;
            typedef DualOf_f<DualT>::T DualDualT;
            std::cout << "Diagonal2TensorProductOfBasedVectorSpaces = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualDiagonal2TensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_f<Diagonal2TensorProductOfBasedVectorSpaces>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                      << "DualDualDiagonal2TensorProductOfBasedVectorSpaces = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the diagonal 2 tensor product of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
        }
    }

    ////////////////////////////////

    {
        typedef TensorPower_c<X,3> TensorPower;
        typedef DualOf_f<TensorPower>::T DualTensorPower;
        typedef DualOf_f<DualTensorPower>::T DualDualTensorPower;
        std::cout << "TensorPower = "  << TypeStringOf_t<TensorPower>::eval() << '\n'
                  << "DualTensorPower = " << TypeStringOf_t<DualTensorPower>::eval() << '\n'
                  << "DualOf_f<TensorPower>::T = " << TypeStringOf_t<DualOf_f<TensorPower>::T>::eval() << '\n'
                  << "DualDualTensorPower = " << TypeStringOf_t<DualDualTensorPower>::eval() << '\n' << '\n';
        // make sure the tensor power is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<TensorPower,DualDualTensorPower>::v));
        // make sure that DualOf_f<TensorPower>::T and DualOf_f<TensorPower>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualTensorPower,DualOf_f<TensorPower>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef TensorPowerOfVectorSpace_c<X5,3> T;
        typedef DualOf_f<T>::T DualT;
        typedef DualOf_f<DualT>::T DualDualT;
        std::cout << "TensorPowerOfVectorSpace = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorPowerOfVectorSpace = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_f<TensorPowerOfVectorSpace>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                  << "DualDualTensorPowerOfVectorSpace = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of vector spaces is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
    }

    {
        typedef Basis_c<X> BX;
        typedef TensorPowerOfBasis_c<BX,3> T;
        typedef DualOf_f<T>::T DualT;
        typedef DualOf_f<DualT>::T DualDualT;
        std::cout << "TensorPowerOfBasis = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualTensorPowerOfBasis = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_f<TensorPowerOfBasis>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                  << "DualDualTensorPowerOfBasis = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of bases is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef Basis_c<X> BX;
        typedef BasedVectorSpace_c<X5,BX> BasedX;
        assert(HasBasedVectorSpaceStructure_f<BasedX>::V);

        typedef TensorPowerOfVectorSpace_c<X5,3> TVS;
        assert(HasVectorSpaceStructure_f<TVS>::V);
        assert(HasTensorProductStructure_f<TVS>::V);
        assert(HasTensorProductOfVectorSpacesStructure_f<TVS>::V);
        assert(HasTensorPowerStructure_f<TVS>::V);
        assert(HasTensorPowerOfVectorSpaceStructure_f<TVS>::V);

        typedef TensorPowerOfBasis_c<BX,3> TB;
        assert(HasBasisStructure_f<TB>::V);
        assert(HasTensorProductStructure_f<TB>::V);
        assert(HasTensorPowerStructure_f<TB>::V);

        {
            typedef BasedTensorPowerOfVectorSpace_c<TVS,TB> T;
            assert(HasTensorProductStructure_f<T>::V);
            assert(HasTensorPowerStructure_f<T>::V);
            assert(HasVectorSpaceStructure_f<T>::V);
            assert(HasBasedVectorSpaceStructure_f<T>::V);
            assert(HasTensorProductOfVectorSpacesStructure_f<T>::V);
            assert(HasTensorPowerOfVectorSpaceStructure_f<T>::V);
            assert(HasBasedTensorProductOfVectorSpacesStructure_f<T>::V);
            assert(HasBasedTensorPowerOfVectorSpaceStructure_f<T>::V);

            typedef DualOf_f<T>::T DualT;
            typedef DualOf_f<DualT>::T DualDualT;
            std::cout << "BasedTensorPowerOfVectorSpace = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualBasedTensorPowerOfVectorSpace = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_f<BasedTensorPowerOfVectorSpace>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                      << "DualDualBasedTensorPowerOfVectorSpace = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the based tensor power of vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
        }

        {
            typedef TensorPowerOfBasedVectorSpace_c<BasedX,3> T;
            assert(HasTensorProductStructure_f<T>::V);
            assert(HasTensorPowerStructure_f<T>::V);
            assert(HasVectorSpaceStructure_f<T>::V);
            assert(HasBasedVectorSpaceStructure_f<T>::V);
            assert(HasTensorProductOfVectorSpacesStructure_f<T>::V);
            assert(HasTensorPowerOfVectorSpaceStructure_f<T>::V);
            assert(HasBasedTensorProductOfVectorSpacesStructure_f<T>::V);
            assert(HasBasedTensorPowerOfVectorSpaceStructure_f<T>::V);
            assert(HasTensorProductOfBasedVectorSpacesStructure_f<T>::V);
            assert(HasTensorPowerOfBasedVectorSpaceStructure_f<T>::V);

            typedef DualOf_f<T>::T DualT;
            typedef DualOf_f<DualT>::T DualDualT;
            std::cout << "TensorPowerOfBasedVectorSpace = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualTensorPowerOfBasedVectorSpace = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_f<TensorPowerOfBasedVectorSpace>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                      << "DualDualTensorPowerOfBasedVectorSpace = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the tensor power of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
        }
    }

    ////////////////////////////////

    {
        typedef SymmetricPower_c<X,3> SymmetricPower;
        typedef DualOf_f<SymmetricPower>::T DualSymmetricPower;
        typedef DualOf_f<DualSymmetricPower>::T DualDualSymmetricPower;
        std::cout << "SymmetricPower = "  << TypeStringOf_t<SymmetricPower>::eval() << '\n'
                  << "DualSymmetricPower = " << TypeStringOf_t<DualSymmetricPower>::eval() << '\n'
                  << "DualOf_f<SymmetricPower>::T = " << TypeStringOf_t<DualOf_f<SymmetricPower>::T>::eval() << '\n'
                  << "DualDualSymmetricPower = " << TypeStringOf_t<DualDualSymmetricPower>::eval() << '\n' << '\n';
        // make sure the tensor power is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<SymmetricPower,DualDualSymmetricPower>::v));
        // make sure that DualOf_f<SymmetricPower>::T and DualOf_f<SymmetricPower>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualSymmetricPower,DualOf_f<SymmetricPower>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef SymmetricPowerOfVectorSpace_c<X5,3> T;
        typedef DualOf_f<T>::T DualT;
        typedef DualOf_f<DualT>::T DualDualT;
        std::cout << "SymmetricPowerOfVectorSpace = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualSymmetricPowerOfVectorSpace = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_f<SymmetricPowerOfVectorSpace>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                  << "DualDualSymmetricPowerOfVectorSpace = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of vector spaces is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
    }

    {
        typedef Basis_c<X> BX;
        typedef SymmetricPowerOfBasis_c<BX,3> T;
        typedef DualOf_f<T>::T DualT;
        typedef DualOf_f<DualT>::T DualDualT;
        std::cout << "SymmetricPowerOfBasis = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualSymmetricPowerOfBasis = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_f<SymmetricPowerOfBasis>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                  << "DualDualSymmetricPowerOfBasis = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of bases is reflexive (self-double-dual)
        assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef Basis_c<X> BX;
        typedef BasedVectorSpace_c<X5,BX> BasedX;
        assert(HasBasedVectorSpaceStructure_f<BasedX>::V);

        typedef SymmetricPowerOfVectorSpace_c<X5,3> TVS;
        assert(HasVectorSpaceStructure_f<TVS>::V);
        assert(HasSymmetricPowerStructure_f<TVS>::V);
        assert(HasSymmetricPowerOfVectorSpaceStructure_f<TVS>::V);

        typedef SymmetricPowerOfBasis_c<BX,3> TB;
        assert(HasBasisStructure_f<TB>::V);
        assert(HasSymmetricPowerStructure_f<TB>::V);
/*
        {
            typedef BasedSymmetricPowerOfVectorSpace_c<TVS,TB> T;
            assert(HasSymmetricPowerStructure_f<T>::V);
            assert(HasVectorSpaceStructure_f<T>::V);
            assert(HasBasedVectorSpaceStructure_f<T>::V);
            assert(HasSymmetricPowerOfVectorSpaceStructure_f<T>::V);
            assert(HasBasedSymmetricPowerOfVectorSpaceStructure_f<T>::V);

            typedef DualOf_f<T>::T DualT;
            typedef DualOf_f<DualT>::T DualDualT;
            std::cout << "BasedSymmetricPowerOfVectorSpace = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualBasedSymmetricPowerOfVectorSpace = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_f<BasedSymmetricPowerOfVectorSpace>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                      << "DualDualBasedSymmetricPowerOfVectorSpace = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the based tensor power of vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
        }
*/
        {
            typedef SymmetricPowerOfBasedVectorSpace_c<BasedX,3> T;
            assert(HasSymmetricPowerStructure_f<T>::V);
            assert(HasVectorSpaceStructure_f<T>::V);
            assert(HasBasedVectorSpaceStructure_f<T>::V);
            assert(HasSymmetricPowerOfVectorSpaceStructure_f<T>::V);
            assert(HasSymmetricPowerOfBasedVectorSpaceStructure_f<T>::V);

            typedef DualOf_f<T>::T DualT;
            typedef DualOf_f<DualT>::T DualDualT;
            std::cout << "SymmetricPowerOfBasedVectorSpace = "  << TypeStringOf_t<T>::eval() << '\n'
                      << "DualSymmetricPowerOfBasedVectorSpace = " << TypeStringOf_t<DualT>::eval() << '\n'
                      << "DualOf_f<SymmetricPowerOfBasedVectorSpace>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                      << "DualDualSymmetricPowerOfBasedVectorSpace = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
            // make sure the tensor power of based vector spaces is reflexive (self-double-dual)
            assert((Lvd::Meta::TypesAreEqual<T,DualDualT>::v));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((Lvd::Meta::TypesAreEqual<DualT,DualOf_f<T>::T>::v));
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

        DualOf_f<V>::T a(0.0f, 2.0f, -3.0f);
        std::cout << TypeStringOf_t<DualOf_f<V>::T>::eval() << '\n';
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

        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<DualOf_f<BasedY>::T> > > TPBVS;
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


        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<DualOf_f<BasedY>::T,TypeList_t<DualOf_f<BasedY>::T> > > > H;
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

        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedY,TypeList_t<DualOf_f<BasedY>::T> > > EndomorphismOfY;
        typedef ImplementationOf_t<float,EndomorphismOfY> Endo;
        Endo A(0.0f);
        A[Endo::MultiIndex(0,0)] = 3.0f;
        A[Endo::MultiIndex(1,1)] = 4.0f;
        std::cout << FORMAT_VALUE(A) << '\n';
        std::cout << FORMAT_VALUE(A(i|j)*v(j)) << '\n';
        std::cout << FORMAT_VALUE(A(i|i)) << '\n';


        DualOf_f<E>::T f(3.0f);
        f[DualOf_f<E>::T::MultiIndex(0,0,0,CHECK_RANGE)] = 1.0f;
        f[DualOf_f<E>::T::MultiIndex(1,0,1,CHECK_RANGE)] = 2.0f;
        f[DualOf_f<E>::T::MultiIndex(2,1,1,CHECK_RANGE)] = 4.0f;
        std::cout << FORMAT_VALUE(f) << '\n';
        std::cout << FORMAT_VALUE(f(i|j|k)*e(i|j|k)) << '\n';
        std::cout << FORMAT_VALUE(f(i|j|k)*e(i|k|j)) << '\n';
        std::cout << FORMAT_VALUE(f(i)*e(i)) << '\n';

        std::cout << FORMAT_VALUE(e(i).eval()) << '\n';
        std::cout << FORMAT_VALUE(e(i|j|k).eval()) << '\n';

        AbstractIndex_c<'l'> l;
        std::cout << FORMAT_VALUE(e(i).split(i,j|k|l)) << '\n';


        AbstractIndex_c<'P'> P;
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<DualOf_f<BasedY>::T,TypeList_t<DualOf_f<BasedY>::T> > > PartOfH;
        typedef ImplementationOf_t<float,PartOfH> G;
        std::cout << FORMAT_VALUE(e(i|j|k).bundle(j|k,PartOfH(),P)) << '\n';
        std::cout << FORMAT_VALUE(f(i|j|k).bundle(j|k,DualOf_f<PartOfH>::T(),P)) << '\n';
        std::cout << FORMAT_VALUE(e(i|j|k).bundle(j|k,PartOfH(),P) * f(i|j|k).bundle(j|k,DualOf_f<PartOfH>::T(),P)) << '\n';

        std::cout << '\n' << '\n';
    }

    {
        typedef VectorSpace_c<RealField,5,X> VSX;
        typedef Basis_c<X> B;
        typedef BasedVectorSpace_c<VSX,B> BasedX;

        STATIC_ASSERT(HasBasedVectorSpaceStructure_f<BasedX>::V, MUST_BE_BASED_VECTOR_SPACE);

        typedef ExteriorPowerOfBasedVectorSpace_c<BasedX,3> Wedge3_BasedX;
        typedef ExteriorPowerOfBasedVectorSpace_c<BasedX,2> Wedge2_BasedX;
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX, TypeList_t<BasedX> > > T2_BasedX;

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
            AbstractIndex_c<'p'> p;
            std::cout << FORMAT_VALUE(w(i).split(i,j|k|l)) << '\n';
            std::cout << FORMAT_VALUE(w(i).split(i,j|k|l).bundle(j|l,Wedge2_BasedX(),p)) << '\n';
            std::cout << FORMAT_VALUE(w(i).split(i,j|k|l).bundle(j|l,T2_BasedX(),p)) << '\n';
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
        typedef VectorSpace_c<RealField,3,X> VSX;
        typedef Basis_c<X> B;
        typedef BasedVectorSpace_c<VSX,B> BasedX;

        STATIC_ASSERT(HasBasedVectorSpaceStructure_f<BasedX>::V, MUST_BE_BASED_VECTOR_SPACE);

        typedef SymmetricPowerOfBasedVectorSpace_c<BasedX,3> Sym3_BasedX;
        typedef SymmetricPowerOfBasedVectorSpace_c<BasedX,2> Sym2_BasedX;
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX, TypeList_t<BasedX> > > T2_BasedX;

        typedef ImplementationOf_t<float,Sym3_BasedX> Sym;

        Sym w(0);
        w[Sym::ComponentIndex(0, CHECK_RANGE)] = 1;
        w[Sym::ComponentIndex(1, CHECK_RANGE)] = 6;
        w[Sym::ComponentIndex(2, CHECK_RANGE)] = 9;
        w[Sym::ComponentIndex(3, CHECK_RANGE)] = 4;
        w[Sym::ComponentIndex(4, CHECK_RANGE)] = 15;
        w[Sym::ComponentIndex(5, CHECK_RANGE)] = 36;
        w[Sym::ComponentIndex(6, CHECK_RANGE)] = 21;
        w[Sym::ComponentIndex(7, CHECK_RANGE)] = 24;
        w[Sym::ComponentIndex(8, CHECK_RANGE)] = 27;
        w[Sym::ComponentIndex(9, CHECK_RANGE)] = 10;

        std::cout << FORMAT_VALUE(w) << '\n';

        {
            AbstractIndex_c<'i'> i;
            AbstractIndex_c<'j'> j;
            AbstractIndex_c<'k'> k;
            AbstractIndex_c<'l'> l;
            AbstractIndex_c<'p'> p;
            std::cout << FORMAT_VALUE(w(i).split(i,j|k|l)) << '\n';
            //std::cout << FORMAT_VALUE(w.split(j|k|l)) << '\n'; // this doesn't currently work.
            std::cout << FORMAT_VALUE(w(i).split(i,j|k|l).bundle(j|l,Sym2_BasedX(),p)) << '\n';
            std::cout << FORMAT_VALUE(w(i).split(i,j|k|l).bundle(j|l,T2_BasedX(),p)) << '\n';
        }

        std::cout << '\n' << '\n';

        std::cout << "Round-trip convert from ComponentIndex\n";
        Sym::ComponentIndex i(7, CHECK_RANGE);
        std::cout << FORMAT_VALUE(i) << '\n';
        Sym::MultiIndex m = Sym::bundle_index_map<Sym::MultiIndex::IndexTypeList, Sym::ComponentIndex>(i);
        std::cout << FORMAT_VALUE(m) << '\n';
        Sym::ComponentIndex j = Sym::vector_index_of(m);
        std::cout << FORMAT_VALUE(j) << "\n\n";

        std::cout << "Round-trip convert from MultiIndex\n";
        Sym::MultiIndex n(1, 0, 2, CHECK_RANGE);
        std::cout << FORMAT_VALUE(n) << '\n';
        i = Sym::vector_index_of(n);
        std::cout << FORMAT_VALUE(i) << '\n';
        n = Sym::bundle_index_map<Sym::MultiIndex::IndexTypeList, Sym::ComponentIndex>(i);
        std::cout << FORMAT_VALUE(n) << '\n';
        i = Sym::vector_index_of(n);
        std::cout << FORMAT_VALUE(i) << "\n\n";
        n = Sym::bundle_index_map<Sym::MultiIndex::IndexTypeList, Sym::ComponentIndex>(i);
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

        std::cout << '\n' << '\n';
    }

    {
        // testing 0-dimensional vector spaces
        typedef VectorSpace_c<RealField,0,X> VSX;
        typedef Basis_c<X> BasisX;
        typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
        typedef ImplementationOf_t<float,BasedX> Vector;
        Vector v(0.0f);
        std::cout << FORMAT_VALUE(v) << '\n';

        std::cout << '\n' << '\n';
    }

    {
        typedef VectorSpace_c<RealField,4,X> VSX;
        typedef Basis_c<X> BasisX;
        typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
        typedef Diagonal2TensorProductOfBasedVectorSpaces_c<DualOf_f<BasedX>::T,DualOf_f<BasedX>::T> Diag;
        typedef ImplementationOf_t<float,Diag> D;
        D d(2.0f, 1.0f, -3.0f, 5.0f);
        std::cout << FORMAT_VALUE(d) << '\n';

        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        std::cout << FORMAT_VALUE(d(i).split(i,j|k)) << '\n';

        typedef ImplementationOf_t<float,BasedX> V;
        V u(1.0f, 2.0f, 10.0f, -2.0f);
        std::cout << FORMAT_VALUE(u) << '\n';
        std::cout << FORMAT_VALUE(u(j) * d(i).split(i,j|k) * u(k)) << '\n';

        // do computation by hand to check it
        float total = 0.0f;
        for (ComponentIndex_t<VSX::DIMENSION> it; it.is_not_at_end(); ++it)
            total += u[it] * d(i).split(i,j|k)[D::MultiIndex(it,it)] * u[it];
        std::cout << "computation done by hand: " << total << '\n';

        V v(-1.0f, -1.0f, 2.0f, 2.0f);
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(u(j) * d(i).split(i,j|k) * v(k)) << '\n';

        // do computation by hand to check it
        total = 0.0f;
        for (ComponentIndex_t<VSX::DIMENSION> it; it.is_not_at_end(); ++it)
            total += u[it] * d(i).split(i,j|k)[D::MultiIndex(it,it)] * v[it];
        std::cout << "computation done by hand: " << total << '\n';

        std::cout << '\n' << '\n';
    }

    {
        typedef VectorSpace_c<RealField,0,X> VSX;
        typedef Basis_c<X> BasisX;
        typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
        test_tensor_printing<BasedX,1>(std::cout);
        test_tensor_printing<BasedX,2>(std::cout);
        test_tensor_printing<BasedX,3>(std::cout);
        test_tensor_printing<BasedX,4>(std::cout);
        std::cout << '\n';
    }

    {
        typedef VectorSpace_c<RealField,1,X> VSX;
        typedef Basis_c<X> BasisX;
        typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
        test_tensor_printing<BasedX,1>(std::cout);
        test_tensor_printing<BasedX,2>(std::cout);
        test_tensor_printing<BasedX,3>(std::cout);
        test_tensor_printing<BasedX,4>(std::cout);
        std::cout << '\n';
    }

    {
        typedef VectorSpace_c<RealField,3,X> VSX;
        typedef Basis_c<X> BasisX;
        typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
        test_tensor_printing<BasedX,1>(std::cout);
        test_tensor_printing<BasedX,2>(std::cout);
        test_tensor_printing<BasedX,3>(std::cout);
        test_tensor_printing<BasedX,4>(std::cout);
        std::cout << '\n';
    }

    {
        typedef VectorSpace_c<RealField,4,X> VSX;
        typedef Basis_c<X> BasisX;
        typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
        typedef ImplementationOf_t<float,SymmetricPowerOfBasedVectorSpace_c<BasedX,4> > Sym;
        typedef ImplementationOf_t<float, BasedX> Vec;
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'l'> l;
        AbstractIndex_c<'p'> p;

        Vec v(1,2,3,4);

        std::cout << test_vector_power<BasedX, float, 4>(v) << std::endl;
        std::cout << test_vector_power<BasedX, float, 4>(v)(i).split(i,j|k|l|p) << std::endl;
    }

    return 0;
}
