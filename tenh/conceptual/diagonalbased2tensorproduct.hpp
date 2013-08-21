// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/diagonalbased2tensorproduct.hpp by Victor Dods, created 2013/07/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_
#define TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/embeddableintensorproduct.hpp"
#include "tenh/conceptual/tensorproduct.hpp"

namespace Tenh {

// formal "diagonal 2-tensor product" of symbols
template <typename Factor1_, typename Factor2_>
struct Diagonal2TensorProduct_c
{
    static Uint32 const ORDER = 2;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;
    typedef TypeList_t<Factor1,TypeList_t<Factor2> > FactorTypeList;
    typedef typename DualOf_c<Diagonal2TensorProduct_c>::T Dual; // the dual is not the diagonal 2-tensor product of the factors

    static std::string type_as_string ()
    {
        return "Diagonal2TensorProduct_c<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>';
    }
};

template <typename Factor1_, typename Factor2_>
struct Diagonal2TensorProductOfBases_c
{
    typedef Basis_c<Diagonal2TensorProduct_c<Factor1_,Factor2_> > As_Basis;

    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsABasis_c<Factor1_>::V, MUST_BE_BASIS, FACTOR1),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsABasis_c<Factor2_>::V, MUST_BE_BASIS, FACTOR2)
    };

    static Uint32 const ORDER = 2;
    typedef typename As_Basis::Id Id;
    typedef typename DualOf_c<Diagonal2TensorProductOfBases_c>::T Dual; // the dual is not the diagonal 2-tensor product the factors
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;

    static std::string type_as_string ()
    {
        return "Diagonal2TensorProduct_c<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>';
    }
};

template <typename Factor1, typename Factor2>
struct IsABasis_c<Diagonal2TensorProductOfBases_c<Factor1,Factor2> > { static bool const V = true; };

// this is an abstract vector space, isomorphic to either Factor1_ or Factor2_ (each of which
// must be a VectorSpace_c).
template <typename Factor1_, typename Factor2_>
struct Diagonal2TensorProductOfVectorSpaces_c
{
    enum
    {
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsAVectorSpace_c<Factor1_>::V, MUST_BE_VECTOR_SPACE, FACTOR1),
        STATIC_ASSERT_IN_ENUM__UNIQUE(IsAVectorSpace_c<Factor2_>::V, MUST_BE_VECTOR_SPACE, FACTOR2),
        STATIC_ASSERT_IN_ENUM((Lvd::Meta::TypesAreEqual<typename Factor1_::Field,typename Factor2_::Field>::v), ALL_FACTORS_MUST_HAVE_SAME_FIELD)
    };

    typedef Diagonal2TensorProduct_c<Factor1_,Factor2_> As_Diagonal2TensorProduct;
    typedef VectorSpace_c<typename Factor1_::Field,
                          (Factor1_::DIM < Factor2_::DIM) ? Factor1_::DIM : Factor2_::DIM,
                          Diagonal2TensorProduct_c<Factor1_,Factor2_> > As_VectorSpace;

    typedef typename As_Diagonal2TensorProduct::Factor1 Factor1;
    typedef typename As_Diagonal2TensorProduct::Factor2 Factor2;
    typedef typename As_Diagonal2TensorProduct::FactorTypeList FactorTypeList;

    typedef typename As_VectorSpace::Field Field;
    static Uint32 const DIM = As_VectorSpace::DIM;
    typedef typename As_VectorSpace::Id Id;
    typedef typename DualOf_c<Diagonal2TensorProductOfVectorSpaces_c>::T Dual; // the dual is not the diagonal 2-tensor product the factors

    static std::string type_as_string ()
    {
        return "Diagonal2TensorProductOfVectorSpaces_c<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>';
    }
};

template <typename Factor1, typename Factor2>
struct IsAVectorSpace_c<Diagonal2TensorProductOfVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };

template <typename Factor1_, typename Factor2_>
struct Diagonal2TensorProductOfBasedVectorSpaces_c
{
    typedef BasedVectorSpace_c<Diagonal2TensorProductOfVectorSpaces_c<Factor1_,Factor2_>,
                               Diagonal2TensorProductOfBases_c<typename Factor1_::Basis,typename Factor2_::Basis> > As_BasedVectorSpace;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,TypeList_t<Factor2_> > > > As_EmbeddableInTensorProductOfBasedVectorSpaces;

    typedef typename As_EmbeddableInTensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;

    enum { STATIC_ASSERT_IN_ENUM(AllFactorsAreBasedVectorSpaces_t<FactorTypeList>::V, ALL_FACTORS_MUST_BE_BASED_VECTOR_SPACES) };

    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProductOfBasedVectorSpaces;
    static Uint32 const ORDER = 2;
    typedef typename As_BasedVectorSpace::Field Field;
    static Uint32 const DIM = As_BasedVectorSpace::DIM;
    typedef typename As_BasedVectorSpace::Id Id;
    typedef typename As_BasedVectorSpace::Basis Basis;
    typedef typename DualOf_c<Diagonal2TensorProductOfBasedVectorSpaces_c>::T Dual; // relies on the template specialization below
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;

    static std::string type_as_string ()
    {
        return "Diagonal2TensorProductOfBasedVectorSpaces_c<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>';
    }
};

template <typename Factor1, typename Factor2> struct IsAVectorSpace_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsATensorProduct_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsATensorProductOfVectorSpaces_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsABasedVectorSpace_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsABasedTensorProductOfVectorSpaces_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsATensorProductOfBasedVectorSpaces_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsEmbeddableInTensorProductOfBasedVectorSpaces_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };

template <typename T> struct IsADiagonal2TensorProductOfBasedVectorSpaces_c { static bool const V = false; };
template <typename Factor1, typename Factor2> struct IsADiagonal2TensorProductOfBasedVectorSpaces_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };

// TODO: verify that this mathematical claim is true
template <typename Factor1, typename Factor2>
struct DualOf_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> >
{
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<typename DualOf_c<Factor1>::T,typename DualOf_c<Factor2>::T> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_
