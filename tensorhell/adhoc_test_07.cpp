#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 7
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_eigen_map_of_2_tensor ()
{
    std::cout << "testing EigenMap_of_2tensor\n";
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,Basis_c<IdX>> BasedVectorSpace;
    typedef TensorProductOfBasedVectorSpaces_c<Typle_t<BasedVectorSpace,BasedVectorSpace>> TensorProduct;
    typedef ImplementationOf_t<TensorProduct,float> T;
    T t(fill_with(3));
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
    {
        std::cout << "testing inversion of 2-tensor\n";
        typedef TensorProductOfBasedVectorSpaces_c<Typle_t<DualOf_f<BasedVectorSpace>::T,DualOf_f<BasedVectorSpace>::T>> TensorProduct_Inverse;
        typedef ImplementationOf_t<TensorProduct_Inverse,float> TInverse;
        TInverse t_inverse(Static<WithoutInitialization>::SINGLETON);
        invert_2tensor(t, t_inverse);
        std::cout << FORMAT_VALUE(t_inverse) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        // verify the identity tensor of the appropriate type pops out of each composition
        std::cout << FORMAT_VALUE(t(i*j)*t_inverse(j*k)) << '\n';
        std::cout << FORMAT_VALUE(t_inverse(i*j)*t(j*k)) << '\n';
        std::cout << '\n';

        std::cout << FORMAT_VALUE(determinant_of_2tensor(t)) << '\n';
        std::cout << '\n';
    }
    {
        std::cout << "testing inversion of symmetric 2-tensor\n";
        typedef SymmetricPowerOfBasedVectorSpace_c<2,BasedVectorSpace> SymmetricProduct;
        typedef SymmetricPowerOfBasedVectorSpace_c<2,DualOf_f<BasedVectorSpace>::T> SymmetricProduct_Inverse;
        typedef ImplementationOf_t<SymmetricProduct,float> S;
        typedef ImplementationOf_t<SymmetricProduct_Inverse,float> SInverse;
        S s(uniform_tuple<float>( 3) |
            uniform_tuple<float>(-1, 2) |
            uniform_tuple<float>(-2, 0, 4));
        SInverse s_inverse(Static<WithoutInitialization>::SINGLETON);
        invert_2tensor(s, s_inverse);
        std::cout << FORMAT_VALUE(s) << '\n';
        std::cout << FORMAT_VALUE(s_inverse) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'p'> p;
        AbstractIndex_c<'q'> q;
        std::cout << FORMAT_VALUE(s_inverse(p).split(p,i*j)) << '\n';
        // verify the identity tensor of the appropriate type pops out of each composition
        std::cout << FORMAT_VALUE(s(p).split(p,i*j)*s_inverse(q).split(q,j*k)) << '\n';
        std::cout << FORMAT_VALUE(s_inverse(p).split(p,i*j)*s(q).split(q,j*k)) << '\n';
        std::cout << '\n';
    }
}

void test_split_index_to_index_order_1 ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,Basis_c<IdX>> BasedVectorSpaceX;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,IdY>,Basis_c<IdY>> BasedVectorSpaceY;
    typedef TensorPowerOfBasedVectorSpace_f<1,BasedVectorSpaceX>::T TensorPower;
    typedef SymmetricPowerOfBasedVectorSpace_c<1,BasedVectorSpaceX> SymmetricPower;
    typedef ExteriorPowerOfBasedVectorSpace_c<1,BasedVectorSpaceX> ExteriorPower;
    test_split_index_to_index_order_1<float,TensorPower>();
    test_split_index_to_index_order_1<float,SymmetricPower>();
    test_split_index_to_index_order_1<float,ExteriorPower>();
}

void test_split_index_to_index_order_2 ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,Basis_c<IdX>> BasedVectorSpaceX;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,IdY>,Basis_c<IdY>> BasedVectorSpaceY;
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
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,IdX>,Basis_c<IdX>> BasedVectorSpaceX;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,IdY>,Basis_c<IdY>> BasedVectorSpaceY;
    typedef TensorPowerOfBasedVectorSpace_f<3,BasedVectorSpaceX>::T TensorPower;
    typedef SymmetricPowerOfBasedVectorSpace_c<3,BasedVectorSpaceX> SymmetricPower;
    typedef ExteriorPowerOfBasedVectorSpace_c<3,BasedVectorSpaceX> ExteriorPower;
    test_split_index_to_index_order_3<float,TensorPower>();
    test_split_index_to_index_order_3<float,SymmetricPower>();
    test_split_index_to_index_order_3<float,ExteriorPower>();
}

