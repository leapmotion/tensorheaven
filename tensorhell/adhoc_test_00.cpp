#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 0
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_Basis_c ()
{
    typedef Basis_c<X> Basis;
    typedef DualOf_f<Basis>::T DualBasis;
    typedef DualOf_f<DualBasis>::T DualDualBasis;
    std::cout << "Basis = "  << type_string_of<Basis>() << '\n'
                << "DualBasis = " << type_string_of<DualBasis>()  << '\n'
                << "DualOf_f<Basis>::T = " << type_string_of<DualOf_f<Basis>::T>() << '\n'
                << "DualDualBasis = " << type_string_of<DualDualBasis>() << '\n' << '\n';
    // make sure the basis is reflexive (self-double-dual)
    assert((TypesAreEqual_f<Basis,DualDualBasis>::V));
    // make sure that DualOf_f<Basis>::T and DualOf_f<Basis>::T are the same
    assert((TypesAreEqual_f<DualBasis,DualOf_f<Basis>::T>::V));
}

void test_VectorSpace_c ()
{
    typedef VectorSpace_c<RealField,3,X> VectorSpace;
    typedef DualOf_f<VectorSpace>::T DualVectorSpace;
    typedef DualOf_f<DualVectorSpace>::T DualDualVectorSpace;
    std::cout << "VectorSpace = "  << type_string_of<VectorSpace>() << '\n'
                << "DualVectorSpace = " << type_string_of<DualVectorSpace>() << '\n'
                << "DualOf_f<VectorSpace>::T = " << type_string_of<DualOf_f<VectorSpace>::T>() << '\n'
                << "DualDualVectorSpace = " << type_string_of<DualDualVectorSpace>() << '\n' << '\n';
    // make sure the vector space is reflexive (self-double-dual)
    assert((TypesAreEqual_f<VectorSpace,DualDualVectorSpace>::V));
    // make sure that DualOf_f<VectorSpace>::T and DualOf_f<VectorSpace>::T are the same
    assert((TypesAreEqual_f<DualVectorSpace,DualOf_f<VectorSpace>::T>::V));
    // make sure VectorSpace is actually a VectorSpace
    assert(HasVectorSpaceStructure_f<VectorSpace>::V);
}

void test_BasedVectorSpace_c ()
{
    typedef VectorSpace_c<RealField,3,X> VectorSpace;
    typedef Basis_c<Y> Basis;
    typedef BasedVectorSpace_c<VectorSpace,Basis> BasedVectorSpace;
    // BasedVectorSpace::Index i;
    typedef DualOf_f<BasedVectorSpace>::T DualBasedVectorSpace;
    typedef DualOf_f<DualBasedVectorSpace>::T DualDualBasedVectorSpace;
    std::cout << "BasedVectorSpace = "  << type_string_of<BasedVectorSpace>() << '\n'
                // << "BasedVectorSpace::Index = " << type_string_of<BasedVectorSpace::Index>() << '\n'
                // << "DualOf_f<BasedVectorSpace::Index>::T = " << type_string_of<DualOf_f<BasedVectorSpace::Index>::T>() << '\n'
                // << "DualOf_f<BasedVectorSpace>::T::Index = " << type_string_of<DualOf_f<BasedVectorSpace>::T::Index>() << '\n'
                << "DualBasedVectorSpace = " << type_string_of<DualBasedVectorSpace>() << '\n'
                << "DualOf_f<BasedVectorSpace>::T = " << type_string_of<DualOf_f<BasedVectorSpace>::T>() << '\n'
                << "DualDualBasedVectorSpace = " << type_string_of<DualDualBasedVectorSpace>() << '\n' << '\n';
    // make sure the vector space is reflexive (self-double-dual)
    assert((TypesAreEqual_f<BasedVectorSpace,DualDualBasedVectorSpace>::V));
    // make sure that DualOf_f<BasedVectorSpace>::T and DualOf_f<BasedVectorSpace>::T are the same
    assert((TypesAreEqual_f<DualBasedVectorSpace,DualOf_f<BasedVectorSpace>::T>::V));
    // make sure that DualOf_f<BasedVectorSpace>::T::Index and DualOf_f<BasedVectorSpace>::T::Index are the same
    // assert((TypesAreEqual_f<DualOf_f<BasedVectorSpace>::T::Index,DualOf_f<BasedVectorSpace::Index>::T>::V));
    // make sure BasedVectorSpace is actually a VectorSpace_c and a BasedVectorSpace_c
    assert(HasVectorSpaceStructure_f<BasedVectorSpace>::V);
    assert(HasBasedVectorSpaceStructure_f<BasedVectorSpace>::V);
}

void test_TensorProduct_c ()
{
    typedef TypeList_t<X,TypeList_t<int,TypeList_t<float> > > TypeList;
    typedef TensorProduct_c<TypeList> TensorProduct;
    typedef DualOf_f<TensorProduct>::T DualTensorProduct;
    typedef DualOf_f<DualTensorProduct>::T DualDualTensorProduct;
    std::cout << "TensorProduct = "  << type_string_of<TensorProduct>() << '\n'
                << "DualTensorProduct = " << type_string_of<DualTensorProduct>() << '\n'
                << "DualOf_f<TensorProduct>::T = " << type_string_of<DualOf_f<TensorProduct>::T>() << '\n'
                << "DualDualTensorProduct = " << type_string_of<DualDualTensorProduct>() << '\n' << '\n';
    // make sure the tensor product is reflexive (self-double-dual)
    assert((TypesAreEqual_f<TensorProduct,DualDualTensorProduct>::V));
    // make sure that DualOf_f<TensorProduct>::T and DualOf_f<TensorProduct>::T are the same
    assert((TypesAreEqual_f<DualTensorProduct,DualOf_f<TensorProduct>::T>::V));
}

void test_TensorProductOfVectorSpaces_c ()
{
    typedef VectorSpace_c<RealField,3,X> X3;
    typedef VectorSpace_c<RealField,4,Y> Y4;
    typedef VectorSpace_c<RealField,5,Z> Z5;
    typedef TensorProductOfVectorSpaces_c<TypeList_t<X3,TypeList_t<Y4,TypeList_t<Z5> > > > T;
    typedef DualOf_f<T>::T DualT;
    typedef DualOf_f<DualT>::T DualDualT;
    std::cout << "TensorProductOfVectorSpaces = "  << type_string_of<T>() << '\n'
                << "DualTensorProductOfVectorSpaces = " << type_string_of<DualT>() << '\n'
                << "DualOf_f<TensorProductOfVectorSpaces>::T = " << type_string_of<DualOf_f<T>::T>() << '\n'
                << "DualDualTensorProductOfVectorSpaces = " << type_string_of<DualDualT>() << '\n' << '\n';
    // make sure the tensor product of vector spaces is reflexive (self-double-dual)
    assert((TypesAreEqual_f<T,DualDualT>::V));
    // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
    assert((TypesAreEqual_f<DualT,DualOf_f<T>::T>::V));
}

void test_TensorProductOfBases_c ()
{
    typedef Basis_c<X> BX;
    typedef Basis_c<Y> BY;
    typedef Basis_c<Z> BZ;
    typedef TensorProductOfBases_c<TypeList_t<BX,TypeList_t<BY,TypeList_t<BZ> > > > T;
    typedef DualOf_f<T>::T DualT;
    typedef DualOf_f<DualT>::T DualDualT;
    std::cout << "TensorProductOfBases = "  << type_string_of<T>() << '\n'
                << "DualTensorProductOfBases = " << type_string_of<DualT>() << '\n'
                << "DualOf_f<TensorProductOfBases>::T = " << type_string_of<DualOf_f<T>::T>() << '\n'
                << "DualDualTensorProductOfBases = " << type_string_of<DualDualT>() << '\n' << '\n';
    // make sure the tensor product of bases is reflexive (self-double-dual)
    assert((TypesAreEqual_f<T,DualDualT>::V));
    // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
    assert((TypesAreEqual_f<DualT,DualOf_f<T>::T>::V));
}

void test_BasedTensorProductStuff ()
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
        std::cout << "BasedTensorProductOfVectorSpaces = "  << type_string_of<T>() << '\n'
                    << "DualBasedTensorProductOfVectorSpaces = " << type_string_of<DualT>() << '\n'
                    << "DualOf_f<BasedTensorProductOfVectorSpaces>::T = " << type_string_of<DualOf_f<T>::T>() << '\n'
                    << "DualDualBasedTensorProductOfVectorSpaces = " << type_string_of<DualDualT>() << '\n' << '\n';
        // make sure the based tensor product of vector spaces is reflexive (self-double-dual)
        assert((TypesAreEqual_f<T,DualDualT>::V));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((TypesAreEqual_f<DualT,DualOf_f<T>::T>::V));
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
        std::cout << "TensorProductOfBasedVectorSpaces = "  << type_string_of<T>() << '\n'
                    << "DualTensorProductOfBasedVectorSpaces = " << type_string_of<DualT>() << '\n'
                    << "DualOf_f<TensorProductOfBasedVectorSpaces>::T = " << type_string_of<DualOf_f<T>::T>() << '\n'
                    << "DualDualTensorProductOfBasedVectorSpaces = " << type_string_of<DualDualT>() << '\n' << '\n';
        // make sure the tensor product of based vector spaces is reflexive (self-double-dual)
        assert((TypesAreEqual_f<T,DualDualT>::V));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((TypesAreEqual_f<DualT,DualOf_f<T>::T>::V));
    }

    {
        typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedX,BasedY> T;
        assert(HasVectorSpaceStructure_f<T>::V);
        assert(HasBasedVectorSpaceStructure_f<T>::V);
        assert(HasDiagonal2TensorProductOfBasedVectorSpacesStructure_f<T>::V);

        typedef DualOf_f<T>::T DualT;
        typedef DualOf_f<DualT>::T DualDualT;
        std::cout << "Diagonal2TensorProductOfBasedVectorSpaces = "  << type_string_of<T>() << '\n'
                    << "DualDiagonal2TensorProductOfBasedVectorSpaces = " << type_string_of<DualT>() << '\n'
                    << "DualOf_f<Diagonal2TensorProductOfBasedVectorSpaces>::T = " << type_string_of<DualOf_f<T>::T>() << '\n'
                    << "DualDualDiagonal2TensorProductOfBasedVectorSpaces = " << type_string_of<DualDualT>() << '\n' << '\n';
        // make sure the diagonal 2 tensor product of based vector spaces is reflexive (self-double-dual)
        assert((TypesAreEqual_f<T,DualDualT>::V));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((TypesAreEqual_f<DualT,DualOf_f<T>::T>::V));
    }
}

void test_SymmetricPower_c ()
{
    typedef SymmetricPower_c<3,X> SymmetricPower;
    typedef DualOf_f<SymmetricPower>::T DualSymmetricPower;
    typedef DualOf_f<DualSymmetricPower>::T DualDualSymmetricPower;
    std::cout << "SymmetricPower = "  << type_string_of<SymmetricPower>() << '\n'
                << "DualSymmetricPower = " << type_string_of<DualSymmetricPower>() << '\n'
                << "DualOf_f<SymmetricPower>::T = " << type_string_of<DualOf_f<SymmetricPower>::T>() << '\n'
                << "DualDualSymmetricPower = " << type_string_of<DualDualSymmetricPower>() << '\n' << '\n';
    // make sure the tensor power is reflexive (self-double-dual)
    assert((TypesAreEqual_f<SymmetricPower,DualDualSymmetricPower>::V));
    // make sure that DualOf_f<SymmetricPower>::T and DualOf_f<SymmetricPower>::T are the same
    assert((TypesAreEqual_f<DualSymmetricPower,DualOf_f<SymmetricPower>::T>::V));
}

void test_SymmetricPowerOfVectorSpace_c ()
{
    typedef VectorSpace_c<RealField,5,X> X5;
    typedef SymmetricPowerOfVectorSpace_c<3,X5> T;
    typedef DualOf_f<T>::T DualT;
    typedef DualOf_f<DualT>::T DualDualT;
    std::cout << "SymmetricPowerOfVectorSpace = "  << type_string_of<T>() << '\n'
                << "DualSymmetricPowerOfVectorSpace = " << type_string_of<DualT>() << '\n'
                << "DualOf_f<SymmetricPowerOfVectorSpace>::T = " << type_string_of<DualOf_f<T>::T>() << '\n'
                << "DualDualSymmetricPowerOfVectorSpace = " << type_string_of<DualDualT>() << '\n' << '\n';
    // make sure the tensor power of vector spaces is reflexive (self-double-dual)
    assert((TypesAreEqual_f<T,DualDualT>::V));
    // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
    assert((TypesAreEqual_f<DualT,DualOf_f<T>::T>::V));
}

void test_SymmetricPowerOfBasis_c ()
{
    typedef Basis_c<X> BX;
    typedef SymmetricPowerOfBasis_c<3,BX> T;
    typedef DualOf_f<T>::T DualT;
    typedef DualOf_f<DualT>::T DualDualT;
    std::cout << "SymmetricPowerOfBasis = "  << type_string_of<T>() << '\n'
                << "DualSymmetricPowerOfBasis = " << type_string_of<DualT>() << '\n'
                << "DualOf_f<SymmetricPowerOfBasis>::T = " << type_string_of<DualOf_f<T>::T>() << '\n'
                << "DualDualSymmetricPowerOfBasis = " << type_string_of<DualDualT>() << '\n' << '\n';
    // make sure the tensor power of bases is reflexive (self-double-dual)
    assert((TypesAreEqual_f<T,DualDualT>::V));
    // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
    assert((TypesAreEqual_f<DualT,DualOf_f<T>::T>::V));
}

void test_BasedSymmetricPowerStuff ()
{
    typedef VectorSpace_c<RealField,5,X> X5;
    typedef Basis_c<X> BX;
    typedef BasedVectorSpace_c<X5,BX> BasedX;
    assert(HasBasedVectorSpaceStructure_f<BasedX>::V);

    typedef SymmetricPowerOfVectorSpace_c<3,X5> TVS;
    assert(HasVectorSpaceStructure_f<TVS>::V);
    assert(HasSymmetricPowerStructure_f<TVS>::V);
    assert(HasSymmetricPowerOfVectorSpaceStructure_f<TVS>::V);

    typedef SymmetricPowerOfBasis_c<3,BX> TB;
    assert(HasBasisStructure_f<TB>::V);
    assert(HasSymmetricPowerStructure_f<TB>::V);

    {
        typedef SymmetricPowerOfBasedVectorSpace_c<3,BasedX> T;
        assert(HasSymmetricPowerStructure_f<T>::V);
        assert(HasVectorSpaceStructure_f<T>::V);
        assert(HasBasedVectorSpaceStructure_f<T>::V);
        assert(HasSymmetricPowerOfVectorSpaceStructure_f<T>::V);
        assert(HasSymmetricPowerOfBasedVectorSpaceStructure_f<T>::V);

        typedef DualOf_f<T>::T DualT;
        typedef DualOf_f<DualT>::T DualDualT;
        std::cout << "SymmetricPowerOfBasedVectorSpace = "  << type_string_of<T>() << '\n'
                    << "DualSymmetricPowerOfBasedVectorSpace = " << type_string_of<DualT>() << '\n'
                    << "DualOf_f<SymmetricPowerOfBasedVectorSpace>::T = " << type_string_of<DualOf_f<T>::T>() << '\n'
                    << "DualDualSymmetricPowerOfBasedVectorSpace = " << type_string_of<DualDualT>() << '\n' << '\n';
        // make sure the tensor power of based vector spaces is reflexive (self-double-dual)
        assert((TypesAreEqual_f<T,DualDualT>::V));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((TypesAreEqual_f<DualT,DualOf_f<T>::T>::V));
    }
}

