#include <cassert>
#include <iostream>

#include "tenh/core.hpp"

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/conceptualinheritancegraph.hpp"
#include "tenh/conceptual/diagonalbased2tensorproduct.hpp"
#include "tenh/conceptual/exteriorpower.hpp"
#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/expressiontemplate_eval.hpp"
#include "tenh/memberarray.hpp"
#include "tenh/immutablearray.hpp"
#include "tenh/implementation/alt.hpp"
#include "tenh/implementation/diagonal2tensor.hpp"
#include "tenh/implementation/euclideanembedding.hpp"
#include "tenh/implementation/innerproduct.hpp"
#include "tenh/implementation/sym.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/tensorproduct.hpp"
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

using namespace Tenh;

template<Uint32 ORDER, typename Vector, typename Scalar>
ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER,Vector>,Scalar> test_vector_power (const ImplementationOf_t<Vector,Scalar> &input)
{
//    STATIC_ASSERT(IS_BASED_VECTORSPACE_UNIQUELY(Vector), MUST_BE_BASED_VECTOR_SPACE);
    typedef ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<ORDER,Vector>,Scalar> Sym;
    typedef ImplementationOf_t<Vector,Scalar> Vec;

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
    typedef typename TensorPowerOfBasedVectorSpace_f<ORDER,BasedVectorSpace>::T TPow;
    typedef ImplementationOf_t<TPow,float> T;
    T t(Static<WithoutInitialization>::SINGLETON);
    for (typename T::ComponentIndex i; i.is_not_at_end(); ++i)
        t[i] = static_cast<float>(i.value());

    out << FORMAT_VALUE(t) << '\n';
}

template <typename Scalar_, Uint32 DIMENSION_, Uint32 K_>
Scalar_ standard_basis_vector_generator (ComponentIndex_t<DIMENSION_> const &i)
{
    return i.value() == K_ ? Scalar_(1) : Scalar_(0);
}

template <Uint32 DIMENSION_, Uint32 K_>
struct StandardBasisVectorGeneratorId
{
    static std::string type_as_string ()
    {
        return "StandardBasisVectorGenerator<" + AS_STRING(DIMENSION_) + ',' + AS_STRING(K_) + '>';
    }
};

template <typename Scalar_, Uint32 DIMENSION_, Uint32 K_>
struct StandardBasisComponentGenerator_f
{
    typedef ComponentGenerator_t<Scalar_,DIMENSION_,
                                 standard_basis_vector_generator<Scalar_,DIMENSION_,K_>,
                                 StandardBasisVectorGeneratorId<DIMENSION_,K_> > T;
};

template <typename BasedVectorSpace_, typename Scalar_, Uint32 K_>
struct StandardBasisVector_f
{
private:
    typedef typename StandardBasisComponentGenerator_f<Scalar_,DimensionOf_f<BasedVectorSpace_>::V,K_>::T ComponentGenerator;
public:
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_,UseImmutableArray_t<ComponentGenerator> > T;
};

template <typename Scalar_, Uint32 DIM_>
Scalar_ counting_vector_generator (ComponentIndex_t<DIM_> const &i)
{
    return Scalar_(i.value() + 1);
}

template <Uint32 DIM_>
struct CountingVectorGeneratorId { static std::string type_as_string () { return "CountingVectorGenerator<" + AS_STRING(DIM_) + '>'; } };

template <typename Scalar_, Uint32 DIM_, Uint32 K_>
void test_immutable_array_0 ()
{
    std::cout << "test_immutable_array_0<" << TypeStringOf_t<Scalar_>::eval() << ',' + AS_STRING(DIM_) << ',' << AS_STRING(K_) << ">\n";
    typedef ComponentGenerator_t<Scalar_,
                                 DIM_,
                                 standard_basis_vector_generator<Scalar_,DIM_,K_>,
                                 StandardBasisVectorGeneratorId<DIM_,K_> > ComponentGenerator;
    typedef ImmutableArray_t<Scalar_,DIM_,ComponentGenerator> A;
    A a;
    std::cout << FORMAT_VALUE(a.type_as_string()) << '\n';
    for (typename A::ComponentIndex i; i.is_not_at_end(); ++i)
        std::cout << FORMAT_VALUE(i) << ", " << FORMAT_VALUE(a[i]) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 DIM_>
void test_immutable_array_1 ()
{
    std::cout << "test_immutable_array_1<" << TypeStringOf_t<Scalar_>::eval() << ',' << AS_STRING(DIM_) << ">\n";
    typedef ComponentGenerator_t<Scalar_,
                                 DIM_,
                                 counting_vector_generator<Scalar_,DIM_>,
                                 CountingVectorGeneratorId<DIM_> > ComponentGenerator;
    typedef ImmutableArray_t<Scalar_,DIM_,ComponentGenerator> A;
    A a;
    std::cout << FORMAT_VALUE(a.type_as_string()) << '\n';
    for (typename A::ComponentIndex i; i.is_not_at_end(); ++i)
        std::cout << FORMAT_VALUE(i) << ", " << FORMAT_VALUE(a[i]) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 DIM_>
void test_immutable_implementation_of_vector ()
{
    std::cout << "test_immutable_implementation_of_vector<" << TypeStringOf_t<Scalar_>::eval() << ',' << AS_STRING(DIM_) << ">\n";
}

template <typename Scalar_, Uint32 DIM_>
Scalar_ identity_matrix_generator (ComponentIndex_t<DIM_*DIM_> const &i)
{
    Uint32 row = i.value() / DIM_;
    Uint32 col = i.value() % DIM_;
    return row == col ? Scalar_(1) : Scalar_(0);
}

struct IdentityMatrixGeneratorId { static std::string type_as_string () { return "IdentityMatrixGenerator"; } };

template <typename Scalar_, typename BasedVectorSpace_>
void test_immutable_identity_tensor ()
{
    typedef TypeList_t<BasedVectorSpace_,TypeList_t<typename DualOf_f<BasedVectorSpace_>::T> > FactorTypeList;
    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProduct;
    typedef ComponentGenerator_t<Scalar_,
                                 DimensionOf_f<TensorProduct>::V,
                                 identity_matrix_generator<Scalar_,DimensionOf_f<BasedVectorSpace_>::V>,
                                 IdentityMatrixGeneratorId> ComponentGenerator;
    typedef ImplementationOf_t<TensorProduct,Scalar_,UseImmutableArray_t<ComponentGenerator> > IdentityTensor;
    IdentityTensor identity_tensor;
    std::cout << "test_immutable_identity_tensor<" << TypeStringOf_t<Scalar_>::eval() << ',' << TypeStringOf_t<BasedVectorSpace_>::eval() << ">\n";
    std::cout << FORMAT_VALUE(TypeStringOf_t<IdentityTensor>::eval()) << '\n';
    std::cout << FORMAT_VALUE(identity_tensor) << '\n';

    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_> Vector;
    Vector v(Static<WithoutInitialization>::SINGLETON);
    for (typename Vector::ComponentIndex i; i.is_not_at_end(); ++i)
        v[i] = static_cast<float>(i.value());

    AbstractIndex_c<'p'> p;
    AbstractIndex_c<'q'> q;
    AbstractIndex_c<'r'> r;
    std::cout << FORMAT_VALUE(v) << '\n';
    std::cout << FORMAT_VALUE(identity_tensor(p|q)*v(q)) << '\n';
    std::cout << FORMAT_VALUE(identity_tensor(p|q)*identity_tensor(q|r)) << '\n';

    std::cout << '\n';
}

template <typename Scalar_, typename VectorSpace_>
void test_standard_euclidean_inner_product ()
{
    std::cout << "test_standard_euclidean_inner_product<" << TypeStringOf_t<Scalar_>::eval() << ',' << TypeStringOf_t<VectorSpace_>::eval() << ">\n";
    typedef typename InnerProduct_f<BasedVectorSpace_c<VectorSpace_,OrthonormalBasis_c<X> >,StandardInnerProduct,Scalar_>::T InnerProduct;
    InnerProduct g;
    std::cout << FORMAT_VALUE(g) << '\n';
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    std::cout << FORMAT_VALUE(g(P).split(P,i|j)) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 ORDER_, typename BasedVectorSpace_, typename InnerProductId_>
void test_tensor_power_of_inner_product ()
{
    std::cout << "test_tensor_power_of_inner_product<" << TypeStringOf_t<Scalar_>::eval() << ',' << AS_STRING(ORDER_) << ',' << TypeStringOf_t<BasedVectorSpace_>::eval() << ">\n";
    typedef typename InnerProduct_f<typename TensorPowerOfBasedVectorSpace_f<ORDER_,BasedVectorSpace_>::T,
                                    typename TensorPower_f<ORDER_,InnerProductId_>::T,
                                    Scalar_>::T InnerProduct;
    InnerProduct g;
    std::cout << FORMAT_VALUE(g) << '\n';
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    std::cout << FORMAT_VALUE(g(P).split(P,i|j)) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, typename VectorSpace_>
void test_euclidean_embedding_of_standard_euclidean_space ()
{
    std::cout << "test_euclidean_embedding_of_standard_euclidean_space<" << TypeStringOf_t<Scalar_>::eval() << ',' << TypeStringOf_t<VectorSpace_>::eval() << ">\n";
    typedef typename EuclideanEmbedding_f<BasedVectorSpace_c<VectorSpace_,OrthonormalBasis_c<StandardBasisId> >,StandardInnerProduct,Scalar_>::T EuclideanEmbedding;
    EuclideanEmbedding e;
    std::cout << FORMAT_VALUE(e) << '\n';
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    std::cout << FORMAT_VALUE(e(P).split(P,i|j)) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 ORDER_, typename BasedVectorSpace_, typename InnerProductId_>
void test_tensor_power_of_euclidean_embedding ()
{
    std::cout << "test_tensor_power_of_euclidean_embedding<" << TypeStringOf_t<Scalar_>::eval() << ',' << AS_STRING(ORDER_) << ',' << TypeStringOf_t<BasedVectorSpace_>::eval() << ">\n";
    typedef typename EuclideanEmbedding_f<typename TensorPowerOfBasedVectorSpace_f<ORDER_,BasedVectorSpace_>::T,
                                          typename TensorPower_f<ORDER_,InnerProductId_>::T,
                                          Scalar_>::T EuclideanEmbedding;
    EuclideanEmbedding e;
    std::cout << FORMAT_VALUE(e) << '\n';
    AbstractIndex_c<'P'> P;
    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    std::cout << FORMAT_VALUE(e(P).split(P,i|j)) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 ORDER_, Uint32 DIMENSION_>
void test_sym ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,DIMENSION_,X>,Basis_c<X> > BasedVectorSpace;
    std::cout << "testing Sym_f<" << TypeStringOf_t<Scalar_>::eval() << ',' << ORDER_ << ',' << DIMENSION_ << ">\n";
    typedef typename Sym_f<ORDER_,BasedVectorSpace,Scalar_>::T Sym;
    Sym sym;
    std::cout << FORMAT_VALUE(sym) << '\n';
    std::cout << '\n';
}

template <typename Scalar_, Uint32 ORDER_, Uint32 DIMENSION_>
void test_alt ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,DIMENSION_,X>,Basis_c<X> > BasedVectorSpace;
    std::cout << "testing Alt_f<" << TypeStringOf_t<Scalar_>::eval() << ',' << ORDER_ << ',' << DIMENSION_ << ">\n";
    typedef typename Alt_f<ORDER_,BasedVectorSpace,Scalar_>::T Alt;
    Alt alt;
    std::cout << FORMAT_VALUE(alt) << '\n';
    std::cout << '\n';
}

int main (int, char **)
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
        assert((TypesAreEqual<Basis,DualDualBasis>::V));
        // make sure that DualOf_f<Basis>::T and DualOf_f<Basis>::T are the same
        assert((TypesAreEqual<DualBasis,DualOf_f<Basis>::T>::V));
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
        assert((TypesAreEqual<VectorSpace,DualDualVectorSpace>::V));
        // make sure that DualOf_f<VectorSpace>::T and DualOf_f<VectorSpace>::T are the same
        assert((TypesAreEqual<DualVectorSpace,DualOf_f<VectorSpace>::T>::V));
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
        assert((TypesAreEqual<BasedVectorSpace,DualDualBasedVectorSpace>::V));
        // make sure that DualOf_f<BasedVectorSpace>::T and DualOf_f<BasedVectorSpace>::T are the same
        assert((TypesAreEqual<DualBasedVectorSpace,DualOf_f<BasedVectorSpace>::T>::V));
        // make sure that DualOf_f<BasedVectorSpace>::T::Index and DualOf_f<BasedVectorSpace>::T::Index are the same
        // assert((TypesAreEqual<DualOf_f<BasedVectorSpace>::T::Index,DualOf_f<BasedVectorSpace::Index>::T>::V));
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
        assert((TypesAreEqual<TensorProduct,DualDualTensorProduct>::V));
        // make sure that DualOf_f<TensorProduct>::T and DualOf_f<TensorProduct>::T are the same
        assert((TypesAreEqual<DualTensorProduct,DualOf_f<TensorProduct>::T>::V));
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
        assert((TypesAreEqual<T,DualDualT>::V));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((TypesAreEqual<DualT,DualOf_f<T>::T>::V));
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
        assert((TypesAreEqual<T,DualDualT>::V));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((TypesAreEqual<DualT,DualOf_f<T>::T>::V));
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
            assert((TypesAreEqual<T,DualDualT>::V));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((TypesAreEqual<DualT,DualOf_f<T>::T>::V));
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
            assert((TypesAreEqual<T,DualDualT>::V));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((TypesAreEqual<DualT,DualOf_f<T>::T>::V));
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
            assert((TypesAreEqual<T,DualDualT>::V));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((TypesAreEqual<DualT,DualOf_f<T>::T>::V));
        }
    }

    ////////////////////////////////

    {
        typedef SymmetricPower_c<3,X> SymmetricPower;
        typedef DualOf_f<SymmetricPower>::T DualSymmetricPower;
        typedef DualOf_f<DualSymmetricPower>::T DualDualSymmetricPower;
        std::cout << "SymmetricPower = "  << TypeStringOf_t<SymmetricPower>::eval() << '\n'
                  << "DualSymmetricPower = " << TypeStringOf_t<DualSymmetricPower>::eval() << '\n'
                  << "DualOf_f<SymmetricPower>::T = " << TypeStringOf_t<DualOf_f<SymmetricPower>::T>::eval() << '\n'
                  << "DualDualSymmetricPower = " << TypeStringOf_t<DualDualSymmetricPower>::eval() << '\n' << '\n';
        // make sure the tensor power is reflexive (self-double-dual)
        assert((TypesAreEqual<SymmetricPower,DualDualSymmetricPower>::V));
        // make sure that DualOf_f<SymmetricPower>::T and DualOf_f<SymmetricPower>::T are the same
        assert((TypesAreEqual<DualSymmetricPower,DualOf_f<SymmetricPower>::T>::V));
    }

    {
        typedef VectorSpace_c<RealField,5,X> X5;
        typedef SymmetricPowerOfVectorSpace_c<3,X5> T;
        typedef DualOf_f<T>::T DualT;
        typedef DualOf_f<DualT>::T DualDualT;
        std::cout << "SymmetricPowerOfVectorSpace = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualSymmetricPowerOfVectorSpace = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_f<SymmetricPowerOfVectorSpace>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                  << "DualDualSymmetricPowerOfVectorSpace = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of vector spaces is reflexive (self-double-dual)
        assert((TypesAreEqual<T,DualDualT>::V));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((TypesAreEqual<DualT,DualOf_f<T>::T>::V));
    }

    {
        typedef Basis_c<X> BX;
        typedef SymmetricPowerOfBasis_c<3,BX> T;
        typedef DualOf_f<T>::T DualT;
        typedef DualOf_f<DualT>::T DualDualT;
        std::cout << "SymmetricPowerOfBasis = "  << TypeStringOf_t<T>::eval() << '\n'
                  << "DualSymmetricPowerOfBasis = " << TypeStringOf_t<DualT>::eval() << '\n'
                  << "DualOf_f<SymmetricPowerOfBasis>::T = " << TypeStringOf_t<DualOf_f<T>::T>::eval() << '\n'
                  << "DualDualSymmetricPowerOfBasis = " << TypeStringOf_t<DualDualT>::eval() << '\n' << '\n';
        // make sure the tensor power of bases is reflexive (self-double-dual)
        assert((TypesAreEqual<T,DualDualT>::V));
        // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
        assert((TypesAreEqual<DualT,DualOf_f<T>::T>::V));
    }

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
            assert((TypesAreEqual<T,DualDualT>::V));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((TypesAreEqual<DualT,DualOf_f<T>::T>::V));
        }
*/
        {
            typedef SymmetricPowerOfBasedVectorSpace_c<3,BasedX> T;
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
            assert((TypesAreEqual<T,DualDualT>::V));
            // make sure that DualOf_f<T>::T and DualOf_f<T>::T are the same
            assert((TypesAreEqual<DualT,DualOf_f<T>::T>::V));
        }
    }

    std::cout << '\n' << '\n';

    {
        typedef VectorSpace_c<RealField,3,X> VSX;
        typedef Basis_c<X> B;
        typedef BasedVectorSpace_c<VSX,B> BasedX;

        typedef ImplementationOf_t<BasedX,float> V;
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
        typedef ImplementationOf_t<BasedX,float> U;
        typedef ImplementationOf_t<BasedY,float> V;
        typedef ImplementationOf_t<TPBVS,float> T;
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
        typedef ImplementationOf_t<H,float> E;
        E e(0.0f);
        for (E::ComponentIndex it; it.is_not_at_end(); ++it)
            e[it] = static_cast<float>(it.value());
        std::cout << FORMAT_VALUE(e(i)) << '\n'; // this has problems: TODO: fix -- ExpressionTemplate_i may need a vector-indexable operator[]

        V w(1.0f, 3.0f);

        AbstractIndex_c<'k'> k;
        std::cout << FORMAT_VALUE(w) << '\n';
        std::cout << FORMAT_VALUE(e) << '\n';
        std::cout << FORMAT_VALUE(e(i|j|k)*v(j)*w(k)) << '\n';

        // this should cause a compile error due to the non-naturality of the pairing
//        std::cout << FORMAT_VALUE(e(i|j|j)) << '\n';

        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedY,TypeList_t<DualOf_f<BasedY>::T> > > EndomorphismOfY;
        typedef ImplementationOf_t<EndomorphismOfY,float> Endo;
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

//        std::cout << FORMAT_VALUE(e(i).eval()) << '\n';
        std::cout << FORMAT_VALUE(e(i|j|k).eval()) << '\n';

        AbstractIndex_c<'l'> l;
        std::cout << FORMAT_VALUE(e(i).split(i,j|k|l)) << '\n';


        AbstractIndex_c<'P'> P;
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<DualOf_f<BasedY>::T,TypeList_t<DualOf_f<BasedY>::T> > > PartOfH;
        typedef ImplementationOf_t<PartOfH,float> G;
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

        typedef ExteriorPowerOfBasedVectorSpace_c<3,BasedX> Wedge3_BasedX;
        typedef ExteriorPowerOfBasedVectorSpace_c<2,BasedX> Wedge2_BasedX;
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX, TypeList_t<BasedX> > > T2_BasedX;

        typedef ImplementationOf_t<Wedge3_BasedX,float> Wedge;

        Wedge w(0.0f);
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

        typedef SymmetricPowerOfBasedVectorSpace_c<3,BasedX> Sym3_BasedX;
        typedef SymmetricPowerOfBasedVectorSpace_c<2,BasedX> Sym2_BasedX;
        typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedX, TypeList_t<BasedX> > > T2_BasedX;

        typedef ImplementationOf_t<Sym3_BasedX,float> Sym;

        Sym w(0.0f);
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
        typedef ImplementationOf_t<BasedX,float> Vector;
        Vector v(0.0f);
        std::cout << FORMAT_VALUE(v) << '\n';

        std::cout << '\n' << '\n';
    }

    {
        typedef VectorSpace_c<RealField,4,X> VSX;
        typedef Basis_c<X> BasisX;
        typedef BasedVectorSpace_c<VSX,BasisX> BasedX;
        typedef Diagonal2TensorProductOfBasedVectorSpaces_c<DualOf_f<BasedX>::T,DualOf_f<BasedX>::T> Diag;
        typedef ImplementationOf_t<Diag,float> D;
        D d(2.0f, 1.0f, -3.0f, 5.0f);
        std::cout << FORMAT_VALUE(d) << '\n';

        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        std::cout << FORMAT_VALUE(d(i).split(i,j|k)) << '\n';

        typedef ImplementationOf_t<BasedX,float> V;
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
        typedef ImplementationOf_t<SymmetricPowerOfBasedVectorSpace_c<4,BasedX>,float> Sym;
        typedef ImplementationOf_t<BasedX,float> Vec;
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'l'> l;
        AbstractIndex_c<'p'> p;

        Vec v(1,2,3,4);

        std::cout << test_vector_power<4,BasedX,float>(v) << std::endl;
        std::cout << test_vector_power<4,BasedX,float>(v)(i).split(i,j|k|l|p) << std::endl;
        std::cout << '\n';
    }

    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedX;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,2,Y>,Basis_c<Y> > BasedY;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,4,Z>,Basis_c<Z> > BasedZ;

    {
        std::cout << "member array\n";
        typedef ImplementationOf_t<BasedX,float,UseMemberArray> V;
        V v(1.0f, 2.0f, 4.0f);
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << '\n';

        // the following should cause a compile error regarding UsePreallocatedArray
        //float components[3] = {8.0f, 10.0f, 11.0f};
        //V w(&components[0]);
    }

    {
        std::cout << "preallocated array\n";
        typedef ImplementationOf_t<BasedX,float,UsePreallocatedArray> V;
        float components[3] = {8.0f, 10.0f, 11.0f};
        std::cout << FORMAT_VALUE(components[0]) << ", " << FORMAT_VALUE(components[1]) << ", " << FORMAT_VALUE(components[2]) << '\n';
        V v(&components[0], CHECK_POINTER); // v must live no longer than components[]
        std::cout << FORMAT_VALUE(&components[0]) << ", " << FORMAT_VALUE(v.pointer_to_allocation()) << '\n';
        std::cout << FORMAT_VALUE(&v[V::MultiIndex(0)]) << '\n';
        std::cout << FORMAT_VALUE(v[V::MultiIndex(0)]) << '\n';
        std::cout << FORMAT_VALUE(v[V::ComponentIndex(0)]) << ", " << FORMAT_VALUE(v[V::ComponentIndex(1)]) << ", " << FORMAT_VALUE(v[V::ComponentIndex(2)]) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        AbstractIndex_c<'i'> i;
        std::cout << FORMAT_VALUE(v(i)) << '\n';
        for (V::ComponentIndex j; j.is_not_at_end(); ++j)
            std::cout << FORMAT_VALUE(v[j]) << '\n';

        // the following should cause a compile error regarding UseMemberArray
        //V w(1.0f, 2.0f, 4.0f);

        typedef ImplementationOf_t<BasedY,float,UsePreallocatedArray> W;
        W w(&components[1]); // last 2 components, i.e. a linear subspace
        std::cout << FORMAT_VALUE(w) << '\n';
        std::cout << '\n';
    }

    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<BasedY,TypeList_t<DualOf_f<BasedX>::T> > > YTensorXDual;

    {
        typedef ImplementationOf_t<BasedX,float,UsePreallocatedArray> V;
        typedef ImplementationOf_t<YTensorXDual,float,UseMemberArray> T;
        float components[3] = {8.0f, 10.0f, 11.0f};
        V v(&components[0], CHECK_POINTER); // v must live no longer than components[]
        T t(2.0f);
        t[T::MultiIndex(0,1)] = -1.0f;
        t[T::MultiIndex(1,2)] = 3.0f;
        std::cout << FORMAT_VALUE(t) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(t(i|j)*v(j)) << '\n';

        std::cout << '\n';
    }

    {
        // using default template parameter for UseArrayType_
        typedef ImplementationOf_t<BasedX,float> V;
        typedef ImplementationOf_t<YTensorXDual,float> T;
        V v(8.0f, 10.0f, 11.0f);
        T t(2.0f);
        t[T::MultiIndex(0,1)] = -1.0f;
        t[T::MultiIndex(1,2)] = 3.0f;
        std::cout << FORMAT_VALUE(t) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        std::cout << FORMAT_VALUE(t(i|j)*v(j)) << '\n';

        std::cout << '\n';
    }

    {
        typedef ImplementationOf_t<YTensorXDual,float,UseMemberArray> T;
        typedef ImplementationOf_t<YTensorXDual,float,UsePreallocatedArray> U;
        T t(2.0f);
        t[T::MultiIndex(0,1)] = -1.0f;
        t[T::MultiIndex(1,2)] = 3.0f;
        U u(t.pointer_to_allocation()); // u must live no longer than t
        std::cout << FORMAT_VALUE(t) << '\n';
        std::cout << FORMAT_VALUE(u) << '\n';
        std::cout << '\n';
    }

    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<BasedY,DualOf_f<BasedX>::T> Diag_YTensorXDual;

    {
        std::cout << "testing Diagonal2TensorProductOfBasedVectorSpaces_c\n";
        typedef ImplementationOf_t<BasedX,float,UsePreallocatedArray> V;
        typedef ImplementationOf_t<Diag_YTensorXDual,float,UseMemberArray> T;
        float components[3] = {8.0f, 10.0f, 11.0f};
        V v(&components[0], CHECK_POINTER); // v must live no longer than components[]
        T t(2.0f, -3.0f);
        std::cout << FORMAT_VALUE(t) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'P'> P;
        std::cout << FORMAT_VALUE(t(P).split(P,i|j)*v(j)) << '\n';

        std::cout << '\n';
    }

    typedef SymmetricPowerOfBasedVectorSpace_c<3,BasedX> Sym;

    {
        std::cout << "testing SymmetricPowerOfBasedVectorSpace_c\n";
        typedef ImplementationOf_t<BasedX,float,UseMemberArray> V;
        typedef ImplementationOf_t<Sym,float,UseMemberArray> S;
        typedef ImplementationOf_t<Sym,float,UsePreallocatedArray> S_;
        S s(Static<WithoutInitialization>::SINGLETON);
        V v(3.0f, 4.0f, 7.0f);
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'P'> P;
        s(P) = (v(i)*v(j)*v(k)).bundle(i|j|k,Sym(),P);
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(s) << '\n';
        std::cout << FORMAT_VALUE(s(P).split(P,i|j|k)) << '\n';
        S_ s_(s.pointer_to_allocation());
        std::cout << FORMAT_VALUE(s_) << '\n';

        std::cout << '\n';
    }

    typedef ExteriorPowerOfBasedVectorSpace_c<3,BasedZ> Alt;

    {
        std::cout << "testing ExteriorPowerOfBasedVectorSpace_c\n";
        typedef ImplementationOf_t<BasedZ,float,UseMemberArray> V;
        typedef ImplementationOf_t<Alt,float,UseMemberArray> A;
        typedef ImplementationOf_t<Alt,float,UsePreallocatedArray> A_;
        A a(Static<WithoutInitialization>::SINGLETON);
        V u(3.0f, 4.0f, 7.0f, 0.0f);
        V v(1.0f, -2.0f, 4.0f, -1.0f);
        V w(0.0f, 1.0f, 2.0f, 3.0f);
        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'P'> P;
        a(P) = (u(i)*v(j)*w(k) - u(i)*w(j)*v(k) - v(i)*u(j)*w(k) + v(i)*w(j)*u(k) + w(i)*u(j)*v(k) - w(i)*v(j)*u(k)).bundle(i|j|k,Alt(),P);
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(a) << '\n';
        std::cout << FORMAT_VALUE(a(P).split(P,i|j|k)) << '\n';
        A_ a_(a.pointer_to_allocation());
        std::cout << FORMAT_VALUE(a_) << '\n';

        std::cout << '\n';
    }

    test_immutable_array_0<float,3,0>();
    test_immutable_array_0<float,3,1>();
    test_immutable_array_0<float,3,2>();

    test_immutable_array_1<float,7>();

    test_immutable_identity_tensor<float,BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > >();

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
            std::cout << FORMAT_VALUE(TypeStringOf_t<T>::eval()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
        {
            typedef TypeList_t<E0,
                    TypeList_t<E1> > ImmutableVectorImplementationTypeList;
            typedef TensorProductOfImmutableVectors_f<ImmutableVectorImplementationTypeList>::T T;
            std::cout << FORMAT_VALUE(TypeStringOf_t<T>::eval()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
        {
            typedef TypeList_t<E1,
                    TypeList_t<E1> > ImmutableVectorImplementationTypeList;
            typedef TensorProductOfImmutableVectors_f<ImmutableVectorImplementationTypeList>::T T;
            std::cout << FORMAT_VALUE(TypeStringOf_t<T>::eval()) << '\n';
            std::cout << FORMAT_VALUE(T()) << '\n';
            std::cout << '\n';
        }
        {
            typedef TypeList_t<E0,
                    TypeList_t<E1,
                    TypeList_t<E2> > > ImmutableVectorImplementationTypeList;
            typedef TensorProductOfImmutableVectors_f<ImmutableVectorImplementationTypeList>::T T;
            std::cout << FORMAT_VALUE(TypeStringOf_t<T>::eval()) << '\n';
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
            std::cout << FORMAT_VALUE(TProdOfImpl()(P|Q).split(P,i|j).split(Q,k|l)) << '\n';
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
            std::cout << FORMAT_VALUE(TProdOfImpl()(A).split(A,P|Q)) << '\n';
            std::cout << FORMAT_VALUE(TProdOfImpl()(A).split(A,P|Q).split(P,i|j).split(Q,k|l)) << '\n';
        }
        std::cout << '\n';
    }

    // testing pretty typestring printing
    if (false)
    {
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedVectorSpace;
        typedef TensorPowerOfBasedVectorSpace_f<4,BasedVectorSpace>::T TensorPower;
        std::cout << '\n' << Pretty<TypeStringOf_t<TensorPower> >() << '\n';
        std::cout << '\n' << Pretty<TypeStringOf_t<TensorPower>,0>() << '\n';
        std::cout << '\n' << Pretty<TypeStringOf_t<TensorPower>,1>() << '\n';
        std::cout << '\n' << Pretty<TypeStringOf_t<TensorPower>,2>() << '\n';
        std::cout << '\n' << Pretty<TypeStringOf_t<TensorPower>,3>() << '\n';
        std::cout << '\n';
        std::cout << "shortify_depth = 0\n";
        std::cout << "OSTRICH 0 "; print_pretty_typestring(std::cout, "Blah<x", 0, 4, 0);          std::cout << '\n' << '\n';
        std::cout << "OSTRICH 1 "; print_pretty_typestring(std::cout, "Blah<", 0, 4, 0);           std::cout << '\n' << '\n';
        std::cout << "OSTRICH 2 "; print_pretty_typestring(std::cout, "Blah<x>", 0, 4, 0);         std::cout << '\n' << '\n';
        std::cout << "OSTRICH 3 "; print_pretty_typestring(std::cout, "Blah<x>x>", 0, 4, 0);       std::cout << '\n' << '\n';
        std::cout << '\n';
        std::cout << "shortify_depth = 1\n";
        std::cout << "OSTRICH 0 "; print_pretty_typestring(std::cout, "Blah<x", 0, 4, 1);          std::cout << '\n' << '\n';
        std::cout << "OSTRICH 1 "; print_pretty_typestring(std::cout, "Blah<", 0, 4, 1);           std::cout << '\n' << '\n';
        std::cout << "OSTRICH 2 "; print_pretty_typestring(std::cout, "Blah<x>", 0, 4, 1);         std::cout << '\n' << '\n';
        std::cout << "OSTRICH 3 "; print_pretty_typestring(std::cout, "Blah<x>x>", 0, 4, 1);       std::cout << '\n' << '\n';
        std::cout << '\n';
        std::cout << "shortify_depth = 2\n";
        std::cout << "OSTRICH 0 "; print_pretty_typestring(std::cout, "Blah<x", 0, 4, 2);          std::cout << '\n' << '\n';
        std::cout << "OSTRICH 1 "; print_pretty_typestring(std::cout, "Blah<", 0, 4, 2);           std::cout << '\n' << '\n';
        std::cout << "OSTRICH 2 "; print_pretty_typestring(std::cout, "Blah<x>", 0, 4, 2);         std::cout << '\n' << '\n';
        std::cout << "OSTRICH 3 "; print_pretty_typestring(std::cout, "Blah<x>x>", 0, 4, 2);       std::cout << '\n' << '\n';
        std::cout << '\n';
    }

    if (false)
    {
        std::cout << "testing conceptual inheritance graph stuff\n";
        Graph g;
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedVectorSpace;
        typedef TensorPowerOfBasedVectorSpace_f<4,BasedVectorSpace>::T TensorPower;
        add_concept_hierarchy_to_graph<0>(TensorPower(), g);
//         std::cout << g;
        g.print_as_dot_graph(std::cout);
        std::cout << '\n';
    }

    {
        typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedVectorSpace;
        std::cout << FORMAT_VALUE(DimensionOf_f<BasedVectorSpace>::V) << '\n';
        std::cout << FORMAT_VALUE(TypeStringOf_t<IdOf_f<BasedVectorSpace>::T>::eval()) << '\n';
        std::cout << FORMAT_VALUE(TypeStringOf_t<ScalarFieldOf_f<BasedVectorSpace>::T>::eval()) << '\n';
        std::cout << FORMAT_VALUE(TypeStringOf_t<BasisOf_f<BasedVectorSpace>::T>::eval()) << '\n';
    }

    {
        test_sym<float,1,2>();
        test_sym<float,2,2>();
        test_sym<float,3,2>();
        test_sym<float,1,3>();
        test_sym<float,2,3>();
        test_sym<float,3,3>();
    }

    {
        test_alt<float,1,1>();
        test_alt<float,1,2>();
//         test_alt<float,2,2>();
//         test_alt<float,3,2>();
        test_alt<float,1,3>();
//         test_alt<float,2,3>();
//         test_alt<float,3,3>();
//         test_alt<float,4,3>();
    }

    return 0;
}
