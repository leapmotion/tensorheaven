#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 5
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_immutable_stuff ()
{
    test_immutable_array_0<float,3,0>();
    test_immutable_array_0<float,3,1>();
    test_immutable_array_0<float,3,2>();

    test_immutable_array_1<float,7>();

    test_immutable_identity_tensor<float,BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > >();
}

void test_inner_product_and_euclidean_stuff ()
{
    test_standard_euclidean_inner_product<float,VectorSpace_c<RealField,4,X> >();

    {
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,OrthonormalBasis_c<X> > BasedVectorSpace;
        test_tensor_power_of_inner_product<float,1,BasedVectorSpace,StandardInnerProduct>();
        test_tensor_power_of_inner_product<float,2,BasedVectorSpace,StandardInnerProduct>();
        test_tensor_power_of_inner_product<float,3,BasedVectorSpace,StandardInnerProduct>();
    }

    test_euclidean_embedding_of_standard_euclidean_space<float,VectorSpace_c<RealField,4,X> >();

    {
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,OrthonormalBasis_c<X> > BasedVectorSpace;
        test_tensor_power_of_euclidean_embedding<float,1,BasedVectorSpace,StandardInnerProduct>();
        test_tensor_power_of_euclidean_embedding<float,2,BasedVectorSpace,StandardInnerProduct>();
        test_tensor_power_of_euclidean_embedding<float,3,BasedVectorSpace,StandardInnerProduct>();
    }
}

void test_products_of_immutable_stuff ()
{
    {
        std::cout << "testing StandardBasisVector_f\n";
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,OrthonormalBasis_c<X> > BasedVectorSpace;
        std::cout << FORMAT_VALUE((StandardBasisVector_f<BasedVectorSpace,float,0>::T())) << '\n';
        std::cout << FORMAT_VALUE((StandardBasisVector_f<BasedVectorSpace,float,1>::T())) << '\n';
        std::cout << FORMAT_VALUE((StandardBasisVector_f<BasedVectorSpace,float,2>::T())) << '\n';
        std::cout << '\n';
    }

    {
        std::cout << "testing TensorProductOfImmutableVectors_f\n";
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,OrthonormalBasis_c<X> > BasedVectorSpace;
        typedef StandardBasisVector_f<BasedVectorSpace,float,0>::T E0;
        typedef StandardBasisVector_f<BasedVectorSpace,float,1>::T E1;
        typedef StandardBasisVector_f<BasedVectorSpace,float,2>::T E2;
        std::cout << FORMAT_VALUE(E0()) << '\n';
        std::cout << FORMAT_VALUE(E1()) << '\n';
        std::cout << FORMAT_VALUE(E2()) << '\n';
        std::cout << '\n';
        {
            typedef TypeList_t<E0> ImmutableVectorImplementationTypeList;
            typedef TensorProductOfImmutableVectors_f<ImmutableVectorImplementationTypeList>::T T;
            std::cout << FORMAT_VALUE(type_string_of<T>()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
        {
            typedef TypeList_t<E0,
                    TypeList_t<E1> > ImmutableVectorImplementationTypeList;
            typedef TensorProductOfImmutableVectors_f<ImmutableVectorImplementationTypeList>::T T;
            std::cout << FORMAT_VALUE(type_string_of<T>()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
        {
            typedef TypeList_t<E1,
                    TypeList_t<E1> > ImmutableVectorImplementationTypeList;
            typedef TensorProductOfImmutableVectors_f<ImmutableVectorImplementationTypeList>::T T;
            std::cout << FORMAT_VALUE(type_string_of<T>()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
        {
            typedef TypeList_t<E0,
                    TypeList_t<E1,
                    TypeList_t<E2> > > ImmutableVectorImplementationTypeList;
            typedef TensorProductOfImmutableVectors_f<ImmutableVectorImplementationTypeList>::T T;
            std::cout << FORMAT_VALUE(type_string_of<T>()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
    }

    {
        std::cout << "testing TensorProductOfImmutable2Tensors_f (for general 2-tensors)\n";
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,X>,OrthonormalBasis_c<X> > BasedX;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Y>,OrthonormalBasis_c<Y> > BasedY;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Z>,OrthonormalBasis_c<Z> > BasedZ;
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX,TypeList_t<BasedY> > > TensorProductA;
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedY,TypeList_t<BasedZ> > > TensorProductB;
        typedef float Scalar;
        typedef ImplementationOf_t<TensorProductA,
                                   Scalar,
                                   UseImmutableArray_t<ComponentGenerator_t<Scalar,
                                                                            DimensionOf_f<TensorProductA>::V,
                                                                            counting_vector_generator<Scalar,DimensionOf_f<TensorProductA>::V>,
                                                                            CountingVectorGeneratorId<DimensionOf_f<TensorProductA>::V> > > > ImplementationA;
        typedef ImplementationOf_t<TensorProductB,
                                   Scalar,
                                   UseImmutableArray_t<ComponentGenerator_t<Scalar,
                                                                            DimensionOf_f<TensorProductB>::V,
                                                                            counting_vector_generator<Scalar,DimensionOf_f<TensorProductB>::V>,
                                                                            CountingVectorGeneratorId<DimensionOf_f<TensorProductB>::V> > > > ImplementationB;
        {
            std::cout << FORMAT_VALUE(ImplementationA()) << '\n';
            typedef TensorProductOfImmutable2Tensors_f<TypeList_t<ImplementationA> >::T TProdOfImpl;
            std::cout << FORMAT_VALUE(TProdOfImpl()) << '\n';
        }
        {
            std::cout << FORMAT_VALUE(ImplementationB()) << '\n';
            typedef TensorProductOfImmutable2Tensors_f<TypeList_t<ImplementationA,TypeList_t<ImplementationB> > >::T TProdOfImpl;
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
        std::cout << "testing TensorProductOfImmutable2Tensors_f (for diagonal 2-tensors)\n";
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,OrthonormalBasis_c<X> > BasedX;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Y>,OrthonormalBasis_c<Y> > BasedY;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Z>,OrthonormalBasis_c<Z> > BasedZ;
        typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedX,BasedY> TensorProductA;
        typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedY,BasedZ> TensorProductB;
        typedef float Scalar;
        typedef ImplementationOf_t<TensorProductA,
                                   Scalar,
                                   UseImmutableArray_t<ComponentGenerator_t<Scalar,
                                                                            DimensionOf_f<TensorProductA>::V,
                                                                            counting_vector_generator<Scalar,DimensionOf_f<TensorProductA>::V>,
                                                                            CountingVectorGeneratorId<DimensionOf_f<TensorProductA>::V> > > > ImplementationA;
        typedef ImplementationOf_t<TensorProductB,
                                   Scalar,
                                   UseImmutableArray_t<ComponentGenerator_t<Scalar,
                                                                            DimensionOf_f<TensorProductB>::V,
                                                                            counting_vector_generator<Scalar,DimensionOf_f<TensorProductB>::V>,
                                                                            CountingVectorGeneratorId<DimensionOf_f<TensorProductB>::V> > > > ImplementationB;
        {
            std::cout << FORMAT_VALUE(ImplementationA()) << '\n';
            typedef TensorProductOfImmutable2Tensors_f<TypeList_t<ImplementationA> >::T TProdOfImpl;
            std::cout << FORMAT_VALUE(TProdOfImpl()) << '\n';
        }
        {
            std::cout << FORMAT_VALUE(ImplementationB()) << '\n';
            typedef TensorProductOfImmutable2Tensors_f<TypeList_t<ImplementationA,TypeList_t<ImplementationB> > >::T TProdOfImpl;
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
}

