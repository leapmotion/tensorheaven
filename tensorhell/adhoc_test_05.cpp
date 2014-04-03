#include "adhoc_test.hpp"

#include "tenh/implementation/scalar2tensor.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 5
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_procedural_stuff ()
{
    test_procedural_array_0<float,3,0>();
    test_procedural_array_0<float,3,1>();
    test_procedural_array_0<float,3,2>();

    test_procedural_array_1<float,7>();

    test_procedural_identity_tensor<float,BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,Basis_c<IdX>>>();
}

void test_inner_product_and_euclidean_stuff ()
{
    test_standard_euclidean_inner_product<float,VectorSpace_c<RealField,4,IdX>>();

    {
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,OrthonormalBasis_c<IdX>> BasedVectorSpace;
        test_tensor_power_of_inner_product<float,1,BasedVectorSpace,StandardInnerProduct>();
        test_tensor_power_of_inner_product<float,2,BasedVectorSpace,StandardInnerProduct>();
        test_tensor_power_of_inner_product<float,3,BasedVectorSpace,StandardInnerProduct>();
    }

    test_euclidean_embedding_of_standard_euclidean_space<float,VectorSpace_c<RealField,4,IdX>>();

    {
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,OrthonormalBasis_c<IdX>> BasedVectorSpace;
        test_tensor_power_of_euclidean_embedding<float,1,BasedVectorSpace,StandardInnerProduct>();
        test_tensor_power_of_euclidean_embedding<float,2,BasedVectorSpace,StandardInnerProduct>();
        test_tensor_power_of_euclidean_embedding<float,3,BasedVectorSpace,StandardInnerProduct>();
    }
}

void test_products_of_procedural_stuff ()
{
    {
        std::cout << "testing StandardBasisVector_f\n";
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,OrthonormalBasis_c<IdX>> BasedVectorSpace;
        std::cout << FORMAT_VALUE((StandardBasisVector_f<BasedVectorSpace,float,0>::T())) << '\n';
        std::cout << FORMAT_VALUE((StandardBasisVector_f<BasedVectorSpace,float,1>::T())) << '\n';
        std::cout << FORMAT_VALUE((StandardBasisVector_f<BasedVectorSpace,float,2>::T())) << '\n';
        std::cout << '\n';
    }

    {
        std::cout << "testing TensorProductOfProceduralVectors_f\n";
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,OrthonormalBasis_c<IdX>> BasedVectorSpace;
        typedef StandardBasisVector_f<BasedVectorSpace,float,0>::T E0;
        typedef StandardBasisVector_f<BasedVectorSpace,float,1>::T E1;
        typedef StandardBasisVector_f<BasedVectorSpace,float,2>::T E2;
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
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,IdX>,OrthonormalBasis_c<IdX>> BasedX;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdY>,OrthonormalBasis_c<IdY>> BasedY;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,IdZ>,OrthonormalBasis_c<IdZ>> BasedZ;
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
            typedef TensorProductOfProcedural2Tensors_f<Typle_t<ImplementationA>>::T TProdOfImpl;
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
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,OrthonormalBasis_c<IdX>> BasedX;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,IdY>,OrthonormalBasis_c<IdY>> BasedY;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,5,IdZ>,OrthonormalBasis_c<IdZ>> BasedZ;
        typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedX,BasedY> TensorProductA;
        typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedY,BasedZ> TensorProductB;
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
            typedef TensorProductOfProcedural2Tensors_f<Typle_t<ImplementationA>>::T TProdOfImpl;
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
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,OrthonormalBasis_c<IdX>> BasedX;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdY>,OrthonormalBasis_c<IdY>> BasedY;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdZ>,OrthonormalBasis_c<IdZ>> BasedZ;
        typedef Scalar2TensorProductOfBasedVectorSpaces_c<BasedX,BasedY> TensorProductA;
        typedef Scalar2TensorProductOfBasedVectorSpaces_c<BasedY,BasedZ> TensorProductB;
        typedef float Scalar;
        typedef ImplementationOf_t<TensorProductA,
                                   Scalar,
                                   UseProceduralArray_t<ComponentGenerator_Constant_f<Scalar,1,2>::T>> ImplementationA;
                                   // UseProceduralArray_t<ComponentGenerator_t<Scalar,
                                   //                                           DimensionOf_f<TensorProductA>::V,
                                   //                                           counting_vector_generator<Scalar,DimensionOf_f<TensorProductA>::V>,
                                   //                                           CountingVectorGeneratorId<DimensionOf_f<TensorProductA>::V>>>> ImplementationA;
        typedef ImplementationOf_t<TensorProductB,
                                   Scalar,
                                   UseProceduralArray_t<ComponentGenerator_Constant_f<Scalar,1,3>::T>> ImplementationB;
                                   // UseProceduralArray_t<ComponentGenerator_t<Scalar,
                                   //                                           DimensionOf_f<TensorProductB>::V,
                                   //                                           counting_vector_generator<Scalar,DimensionOf_f<TensorProductB>::V>,
                                   //                                           CountingVectorGeneratorId<DimensionOf_f<TensorProductB>::V>>>> ImplementationB;
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
            std::cout << FORMAT_VALUE(t(A).split(A,P*Q)) << '\n';
            std::cout << FORMAT_VALUE(t(A).split(A,P*Q).split(P,i*j).split(Q,k*l)) << '\n';
        }
        std::cout << '\n';
    }
}

