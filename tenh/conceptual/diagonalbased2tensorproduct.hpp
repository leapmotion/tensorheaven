// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/diagonalbased2tensorproduct.hpp by Victor Dods, created 2013/07/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_
#define TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/tensorproduct.hpp"

namespace Tenh {

template <typename Factor1_, typename Factor2_>
struct Diagonal2TensorProductOfBasedVectorSpaces_c
    :
    public TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,TypeList_t<Factor2_> > >
{
    typedef TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1_,TypeList_t<Factor2_> > > Parent_TensorProductOfBasedVectorSpaces;

    typedef typename Parent_TensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;
    using Parent_TensorProductOfBasedVectorSpaces::ORDER;
    typedef typename Parent_TensorProductOfBasedVectorSpaces::Field Field;
    using Parent_TensorProductOfBasedVectorSpaces::DIM;
    typedef typename Parent_TensorProductOfBasedVectorSpaces::Id Id;
    typedef typename Parent_TensorProductOfBasedVectorSpaces::Basis Basis;
    typedef typename DualOf_c<Diagonal2TensorProductOfBasedVectorSpaces_c>::T Dual; // relies on the template specialization below
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;

    static std::string type_as_string ()
    {
        return "Diagonal2TensorProductOfBasedVectorSpaces_c<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor1>::eval() + '>';
    }
};

template <typename Factor1, typename Factor2> struct IsAVectorSpace_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsATensorProduct_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsATensorProductOfVectorSpaces_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsABasedVectorSpace_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsABasedTensorProductOfVectorSpaces_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };
template <typename Factor1, typename Factor2> struct IsATensorProductOfBasedVectorSpaces_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };

template <typename T> struct IsADiagonal2TensorProductOfBasedVectorSpaces_c { static bool const V = false; };
template <typename Factor1, typename Factor2> struct IsADiagonal2TensorProductOfBasedVectorSpaces_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> > { static bool const V = true; };

template <typename Factor1, typename Factor2>
struct DualOf_c<Diagonal2TensorProductOfBasedVectorSpaces_c<Factor1,Factor2> >
{
    typedef Diagonal2TensorProductOfBasedVectorSpaces_c<typename DualOf_c<Factor1>::T,typename DualOf_c<Factor2>::T> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_DIAGONALBASED2TENSORPRODUCT_HPP_
