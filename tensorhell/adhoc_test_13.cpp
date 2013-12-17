#include "adhoc_test.hpp"

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
    std::cout << FORMAT(D::BasisVector_f<0>::V(P).split(P,i*j)) << '\n';
    std::cout << FORMAT(D::BasisVector_f<1>::V(P).split(P,i*j)) << '\n';
    std::cout << FORMAT(D::BasisVector_f<2>::V(P).split(P,i*j)) << '\n';
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
    std::cout << FORMAT(S::BasisVector_f<0>::V(P).split(P,i*j)) << '\n';
    std::cout << FORMAT(S::BasisVector_f<1>::V(P).split(P,i*j)) << '\n';
    std::cout << FORMAT(S::BasisVector_f<2>::V(P).split(P,i*j)) << '\n';
    std::cout << FORMAT(S::BasisVector_f<3>::V(P).split(P,i*j)) << '\n';
    std::cout << FORMAT(S::BasisVector_f<4>::V(P).split(P,i*j)) << '\n';
    std::cout << FORMAT(S::BasisVector_f<5>::V(P).split(P,i*j)) << '\n';
    std::cout << '\n';

    std::cout << "alternating\n";
    std::cout << FORMAT(A::BasisVector_f<0>::V) << '\n';
    std::cout << FORMAT(A::BasisVector_f<1>::V) << '\n';
    std::cout << FORMAT(A::BasisVector_f<2>::V) << '\n';
    std::cout << "alternating -- split\n";
    std::cout << FORMAT(A::BasisVector_f<0>::V(P).split(P,i*j)) << '\n';
    std::cout << FORMAT(A::BasisVector_f<1>::V(P).split(P,i*j)) << '\n';
    std::cout << FORMAT(A::BasisVector_f<2>::V(P).split(P,i*j)) << '\n';
    std::cout << '\n';
}

void test_eval_value ()
{
    std::cout << "testing eval().value()\n";
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > BasedVectorSpace;
    typedef float Scalar;
    typedef ImplementationOf_t<BasedVectorSpace,Scalar,UseMemberArray> V;
    V x(uniform_tuple<Scalar>(1, 2, 3));
    V y(uniform_tuple<Scalar>(4, 8, 9));
    V z(uniform_tuple<Scalar>(0, 8, 0));
    std::cout << FORMAT_VALUE(x(i)*y(j)*z(k)) << '\n';
    std::cout << FORMAT_VALUE((x(i)*y(j)*z(k)).eval()) << '\n';
    std::cout << FORMAT_VALUE((x(i)*y(j)*z(k)).eval().value()) << '\n';

    // V v((x(i)+y(i)).eval().value());
    // V w((x(i)+y(i)).eval());
    std::cout << FORMAT_VALUE(x(i)+y(i)) << '\n';
    std::cout << FORMAT_VALUE((x(i)*y(j)).eval()) << '\n';
    std::cout << FORMAT_VALUE((x(i)*y(j)).eval().value()) << '\n';

    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedVectorSpace,
                                               TypeList_t<BasedVectorSpace> > > TensorProduct;
    typedef ImplementationOf_t<TensorProduct,Scalar,UseMemberArray> T;
    T t((x(i)*y(j)).eval().value());
    std::cout << FORMAT_VALUE(t) << '\n';
    // this should use "operator EvaluatedTensor const &" in ExpressionTemplate_Eval_t
    T s((x(i)*y(j)).eval());
    std::cout << FORMAT_VALUE(s) << '\n';
    // std::cout << FORMAT_VALUE((x(i)+y(i)).eval()) << '\n';
    // std::cout << FORMAT_VALUE((x(i)+y(i)).eval().value()) << '\n';
}

void test_direct_sum_of_2tensors ()
{
    typedef double Scalar;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,1,Generic>,Basis_c<Generic> > B1;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,Generic>,Basis_c<Generic> > B2;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > B3;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B1,TypeList_t<B1> > > T11;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B1,TypeList_t<B2> > > T12;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B1,TypeList_t<B3> > > T13;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B2,TypeList_t<B1> > > T21;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B2,TypeList_t<B2> > > T22;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B2,TypeList_t<B3> > > T23;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<B1> > > T31;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<B2> > > T32;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<B3,TypeList_t<B3> > > T33;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T11>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T11>::V,11>,
                                 Generic> ComponentGenerator11;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T12>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T12>::V,12>,
                                 Generic> ComponentGenerator12;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T13>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T13>::V,13>,
                                 Generic> ComponentGenerator13;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T21>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T21>::V,21>,
                                 Generic> ComponentGenerator21;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T22>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T22>::V,22>,
                                 Generic> ComponentGenerator22;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T23>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T23>::V,23>,
                                 Generic> ComponentGenerator23;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T31>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T31>::V,31>,
                                 Generic> ComponentGenerator31;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T32>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T32>::V,32>,
                                 Generic> ComponentGenerator32;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T33>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T33>::V,33>,
                                 Generic> ComponentGenerator33;
    typedef ImplementationOf_t<T11,Scalar,UseImmutableArray_t<ComponentGenerator11> > T_11;
    typedef ImplementationOf_t<T12,Scalar,UseImmutableArray_t<ComponentGenerator12> > T_12;
    typedef ImplementationOf_t<T13,Scalar,UseImmutableArray_t<ComponentGenerator13> > T_13;
    typedef ImplementationOf_t<T21,Scalar,UseImmutableArray_t<ComponentGenerator21> > T_21;
    typedef ImplementationOf_t<T22,Scalar,UseImmutableArray_t<ComponentGenerator22> > T_22;
    typedef ImplementationOf_t<T23,Scalar,UseImmutableArray_t<ComponentGenerator23> > T_23;
    typedef ImplementationOf_t<T31,Scalar,UseImmutableArray_t<ComponentGenerator31> > T_31;
    typedef ImplementationOf_t<T32,Scalar,UseImmutableArray_t<ComponentGenerator32> > T_32;
    typedef ImplementationOf_t<T33,Scalar,UseImmutableArray_t<ComponentGenerator33> > T_33;

    typedef TypeList_t<T_11,
            TypeList_t<T_12,
            TypeList_t<T_13,
            TypeList_t<T_21,
            TypeList_t<T_22,
            TypeList_t<T_23,
            TypeList_t<T_31,
            TypeList_t<T_32,
            TypeList_t<T_33> > > > > > > > > TL;
    typedef DirectSumOfImmutable2Tensors_f<TL>::T D;
    D d;
    std::cout << "direct sum of immutable 2-tensors\n";
    std::cout << FORMAT_VALUE(d) << '\n';
}

void test_direct_sum_of_diagonal2tensors ()
{
    typedef double Scalar;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,1,Generic>,Basis_c<Generic> > B1;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,Generic>,Basis_c<Generic> > B2;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,Basis_c<Generic> > B3;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B1,B1> T11;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B1,B2> T12;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B1,B3> T13;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B2,B1> T21;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B2,B2> T22;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B2,B3> T23;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B1> T31;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B2> T32;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<B3,B3> T33;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T11>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T11>::V,11>,
                                 Generic> ComponentGenerator11;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T12>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T12>::V,12>,
                                 Generic> ComponentGenerator12;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T13>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T13>::V,13>,
                                 Generic> ComponentGenerator13;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T21>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T21>::V,21>,
                                 Generic> ComponentGenerator21;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T22>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T22>::V,22>,
                                 Generic> ComponentGenerator22;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T23>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T23>::V,23>,
                                 Generic> ComponentGenerator23;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T31>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T31>::V,31>,
                                 Generic> ComponentGenerator31;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T32>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T32>::V,32>,
                                 Generic> ComponentGenerator32;
    typedef ComponentGenerator_t<Scalar,
                                 DimensionOf_f<T33>::V,
                                 constant_component_generator_evaluator<Scalar,DimensionOf_f<T33>::V,33>,
                                 Generic> ComponentGenerator33;
    typedef ImplementationOf_t<T11,Scalar,UseImmutableArray_t<ComponentGenerator11> > T_11;
    typedef ImplementationOf_t<T12,Scalar,UseImmutableArray_t<ComponentGenerator12> > T_12;
    typedef ImplementationOf_t<T13,Scalar,UseImmutableArray_t<ComponentGenerator13> > T_13;
    typedef ImplementationOf_t<T21,Scalar,UseImmutableArray_t<ComponentGenerator21> > T_21;
    typedef ImplementationOf_t<T22,Scalar,UseImmutableArray_t<ComponentGenerator22> > T_22;
    typedef ImplementationOf_t<T23,Scalar,UseImmutableArray_t<ComponentGenerator23> > T_23;
    typedef ImplementationOf_t<T31,Scalar,UseImmutableArray_t<ComponentGenerator31> > T_31;
    typedef ImplementationOf_t<T32,Scalar,UseImmutableArray_t<ComponentGenerator32> > T_32;
    typedef ImplementationOf_t<T33,Scalar,UseImmutableArray_t<ComponentGenerator33> > T_33;

    typedef TypeList_t<T_11,
            TypeList_t<T_12,
            TypeList_t<T_13,
            TypeList_t<T_21,
            TypeList_t<T_22,
            TypeList_t<T_23,
            TypeList_t<T_31,
            TypeList_t<T_32,
            TypeList_t<T_33> > > > > > > > > TL;
    typedef DirectSumOfImmutable2Tensors_f<TL>::T D;
    D d;
    std::cout << "direct sum of immutable diagonal-2-tensors\n";
    std::cout << FORMAT_VALUE(d) << '\n';
}

void test_direct_sum_of_inner_products ()
{
    typedef double Scalar;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,Generic>,OrthonormalBasis_c<Generic> > B3;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Generic>,OrthonormalBasis_c<Generic> > B4;
    typedef DirectSumOfBasedVectorSpaces_c<TypeList_t<B3,
                                           TypeList_t<B4> > > DirectSum;
    typedef DirectSum_c<TypeList_t<StandardInnerProduct,
                        TypeList_t<StandardInnerProduct> > > DirectSumInnerProductId;
    InnerProduct_f<DirectSum,DirectSumInnerProductId,Scalar>::T inner_product;
    std::cout << "direct sum of inner products\n";
    std::cout << FORMAT_VALUE(inner_product) << '\n';
    std::cout << '\n';
}
