#include "conceptual_prototype.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 7
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_eigen_map_of_2_tensor ()
{
    std::cout << "testing EigenMap_of_2tensor\n";
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedVectorSpace;
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedVectorSpace,
                                                TypeList_t<BasedVectorSpace> > > TensorProduct;
    typedef ImplementationOf_t<TensorProduct,float> T;
    T t(fill_with<float>(3));
    std::cout << FORMAT_VALUE(t) << '\n';
    std::cout << FORMAT_VALUE(EigenMap_of_2tensor(t)) << '\n';
    std::cout << FORMAT_VALUE(EigenMap_of_2tensor(static_cast<T const>(t))) << '\n';
    Eigen::Matrix<float,3,1> v1(1.0f, 2.0f, 3.0f); // 3 linearly independent vectors
    Eigen::Matrix<float,3,1> v2(0.0f, 2.0f, 3.0f);
    Eigen::Matrix<float,3,1> v3(1.0f, 2.0f, 4.0f);
    std::cout << FORMAT_VALUE(v1) << '\n';
    std::cout << FORMAT_VALUE(v2) << '\n';
    std::cout << FORMAT_VALUE(v3) << '\n';
    EigenMap_of_2tensor(t) = v1*v1.transpose() + v2*v2.transpose() + v3*v3.transpose(); // make a nonsingular 2-tensor
    std::cout << "after assignment from v1*v1.transpose() + v2*v2.transpose() + v3*v3.transpose() via Eigen Map: ";
    std::cout << FORMAT_VALUE(t) << '\n';

    // TODO: make a metafunction to return the type of the inverse 2-tensor?
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<DualOf_f<BasedVectorSpace>::T,
                                                TypeList_t<DualOf_f<BasedVectorSpace>::T> > > TensorProduct_Inverse;
    typedef ImplementationOf_t<TensorProduct_Inverse,float> TInverse;
    TInverse t_inverse(Static<WithoutInitialization>::SINGLETON);
    invert_2tensor(t, t_inverse);
    std::cout << FORMAT_VALUE(t_inverse) << '\n';
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    // verify the identity tensor of the appropriate type pops out of each composition
    std::cout << FORMAT_VALUE(t(i|j)*t_inverse(j|k)) << '\n';
    std::cout << FORMAT_VALUE(t_inverse(i|j)*t(j|k)) << '\n';
    std::cout << '\n';
}

void test_split_index_to_index_order_1 ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedVectorSpaceX;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Y>,Basis_c<Y> > BasedVectorSpaceY;
    typedef TensorPowerOfBasedVectorSpace_f<1,BasedVectorSpaceX>::T TensorPower;
    typedef SymmetricPowerOfBasedVectorSpace_c<1,BasedVectorSpaceX> SymmetricPower;
    typedef ExteriorPowerOfBasedVectorSpace_c<1,BasedVectorSpaceX> ExteriorPower;
    test_split_index_to_index_order_1<float,TensorPower>();
    test_split_index_to_index_order_1<float,SymmetricPower>();
    test_split_index_to_index_order_1<float,ExteriorPower>();
}

void test_split_index_to_index_order_2 ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedVectorSpaceX;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Y>,Basis_c<Y> > BasedVectorSpaceY;
    typedef TensorPowerOfBasedVectorSpace_f<2,BasedVectorSpaceX>::T TensorPower;
    typedef SymmetricPowerOfBasedVectorSpace_c<2,BasedVectorSpaceX> SymmetricPower;
    typedef ExteriorPowerOfBasedVectorSpace_c<2,BasedVectorSpaceX> ExteriorPower;
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedVectorSpaceX,BasedVectorSpaceY> Diagonal2TensorProduct;
    test_split_index_to_index_order_2<float,TensorPower>();
    test_split_index_to_index_order_2<float,SymmetricPower>();
    test_split_index_to_index_order_2<float,ExteriorPower>();
    test_split_index_to_index_order_2<float,Diagonal2TensorProduct>();
}

void test_split_index_to_index_order_3 ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedVectorSpaceX;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Y>,Basis_c<Y> > BasedVectorSpaceY;
    typedef TensorPowerOfBasedVectorSpace_f<3,BasedVectorSpaceX>::T TensorPower;
    typedef SymmetricPowerOfBasedVectorSpace_c<3,BasedVectorSpaceX> SymmetricPower;
    typedef ExteriorPowerOfBasedVectorSpace_c<3,BasedVectorSpaceX> ExteriorPower;
    test_split_index_to_index_order_3<float,TensorPower>();
    test_split_index_to_index_order_3<float,SymmetricPower>();
    test_split_index_to_index_order_3<float,ExteriorPower>();
}

