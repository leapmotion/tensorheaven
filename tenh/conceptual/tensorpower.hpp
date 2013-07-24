// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/tensorpower.hpp by Victor Dods, created 2013/07/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_TENSORPOWER_HPP_
#define TENH_CONCEPTUAL_TENSORPOWER_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/tensorproduct.hpp"

namespace Tenh {

template <typename Type, Uint32 MULTIPLICITY>
struct TypeListWithMultiplicity_t
{
    typedef TypeList_t<Type,typename TypeListWithMultiplicity_t<Type,MULTIPLICITY-1>::T> T;
};

template <typename Type>
struct TypeListWithMultiplicity_t<Type,0>
{
    typedef EmptyTypeList T;
};

// generic tensor power of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <typename Factor_, Uint32 ORDER_>
struct TensorPower_c
    :
    TensorProduct_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T>
{
    typedef TensorProduct_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T> Parent_TensorProduct;

    typedef typename Parent_TensorProduct::FactorTypeList FactorTypeList;
    using Parent_TensorProduct::ORDER;
    typedef typename DualOf_c<TensorPower_c>::T Dual; // relies on the template specialization below
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPower_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor, Uint32 ORDER> struct IsATensorProduct_c<TensorPower_c<Factor,ORDER> > { static bool const V = true; };

template <typename T> struct IsATensorPower_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsATensorPower_c<TensorPower_c<Factor,ORDER> > { static bool const V = true; };

// the dual of a tensor power is naturally identified as the tensor power of the dual.
template <typename Factor, Uint32 ORDER>
struct DualOf_c<TensorPower_c<Factor,ORDER> >
{
    typedef TensorPower_c<typename DualOf_c<Factor>::T,ORDER> T;
};


// FactorTypeList_ must be a TypeList_t of VectorSpace_c types
template <typename Factor_, Uint32 ORDER_>
struct TensorPowerOfVectorSpaces_c
    : 
    public TensorPower_c<Factor_,ORDER_>,
    public TensorProductOfVectorSpaces_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T>
{
    typedef TensorPower_c<Factor_,ORDER_> Parent_TensorPower;
    typedef TensorProductOfVectorSpaces_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T> Parent_TensorProductOfVectorSpaces;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsAVectorSpace_c<Factor_>::V, MUST_BE_VECTOR_SPACE),
    };

    typedef typename Parent_TensorPower::FactorTypeList FactorTypeList;
    using Parent_TensorPower::ORDER;
    typedef typename Parent_TensorProductOfVectorSpaces::Field Field;
    using Parent_TensorProductOfVectorSpaces::DIM;
    typedef typename Parent_TensorProductOfVectorSpaces::Id Id;
    typedef typename DualOf_c<TensorPowerOfVectorSpaces_c>::T Dual; // relies on the template specialization below
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPowerOfVectorSpaces_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor, Uint32 ORDER> struct IsATensorProduct_c<TensorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsATensorPower_c<TensorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsAVectorSpace_c<TensorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsATensorProductOfVectorSpaces_c<TensorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

template <typename T> struct IsATensorPowerOfVectorSpaces_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsATensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

template <typename Factor, Uint32 ORDER>
struct DualOf_c<TensorPowerOfVectorSpaces_c<Factor,ORDER> >
{
    typedef TensorPowerOfVectorSpaces_c<typename DualOf_c<Factor>::T,ORDER> T;
};


// Factor_ must be a Basis_c type
template <typename Factor_, Uint32 ORDER_>
struct TensorPowerOfBases_c
    : 
    public TensorPower_c<Factor_,ORDER_>,
    public Basis_c<TensorPower_c<Factor_,ORDER_> >
{
    typedef TensorPower_c<Factor_,ORDER_> Parent_TensorPower;
    typedef Basis_c<TensorPower_c<Factor_,ORDER_> > Parent_Basis;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsABasis_c<Factor_>::V, MUST_BE_BASIS)
    };

    typedef typename Parent_TensorPower::FactorTypeList FactorTypeList;
    using Parent_TensorPower::ORDER;
    typedef typename Parent_Basis::Id Id;
    typedef typename DualOf_c<TensorPowerOfBases_c>::T Dual; // relies on the template specialization below
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPowerOfBases_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor, Uint32 ORDER> struct IsATensorProduct_c<TensorPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsATensorPower_c<TensorPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsABasis_c<TensorPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };

template <typename T> struct IsATensorPowerOfBases_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsATensorPowerOfBases_c<TensorPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };

template <typename Factor, Uint32 ORDER>
struct DualOf_c<TensorPowerOfBases_c<Factor,ORDER> >
{
    typedef TensorPowerOfBases_c<typename DualOf_c<Factor>::T,ORDER> T;
};



template <typename TensorPowerOfVectorSpaces, typename Basis_>
struct BasedTensorPowerOfVectorSpaces_c
    :
    public TensorPowerOfVectorSpaces,
    public BasedTensorProductOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis_>
{
    typedef TensorPowerOfVectorSpaces Parent_TensorPowerOfVectorSpaces;
    typedef BasedTensorProductOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis_> Parent_BasedTensorProductOfVectorSpaces;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsATensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces>::V, MUST_BE_TENSOR_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(IsABasis_c<Basis_>::V, MUST_BE_BASIS),
    };

    typedef typename Parent_TensorPowerOfVectorSpaces::FactorTypeList FactorTypeList;
    using Parent_TensorPowerOfVectorSpaces::ORDER;
    typedef typename Parent_BasedTensorProductOfVectorSpaces::Field Field;
    using Parent_BasedTensorProductOfVectorSpaces::DIM;
    typedef typename Parent_BasedTensorProductOfVectorSpaces::Id Id;
    typedef typename Parent_BasedTensorProductOfVectorSpaces::Basis Basis;
    typedef typename DualOf_c<BasedTensorPowerOfVectorSpaces_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "BasedTensorPowerOfVectorSpaces_c<" + TypeStringOf_t<TensorPowerOfVectorSpaces>::eval() + ','
                                                   + TypeStringOf_t<Basis>::eval() + '>';
    }
};

template <typename TensorPowerOfVectorSpaces, typename Basis> struct IsAVectorSpace_c<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> > { static bool const V = true; };
template <typename TensorPowerOfVectorSpaces, typename Basis> struct IsATensorProduct_c<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> > { static bool const V = true; };
template <typename TensorPowerOfVectorSpaces, typename Basis> struct IsATensorPower_c<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> > { static bool const V = true; };
template <typename TensorPowerOfVectorSpaces, typename Basis> struct IsATensorProductOfVectorSpaces_c<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> > { static bool const V = true; };
template <typename TensorPowerOfVectorSpaces, typename Basis> struct IsATensorPowerOfVectorSpaces_c<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> > { static bool const V = true; };
template <typename TensorPowerOfVectorSpaces, typename Basis> struct IsABasedVectorSpace_c<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> > { static bool const V = true; };
template <typename TensorPowerOfVectorSpaces, typename Basis> struct IsABasedTensorProductOfVectorSpaces_c<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> > { static bool const V = true; };

template <typename T> struct IsABasedTensorPowerOfVectorSpaces_c { static bool const V = false; };
template <typename TensorPowerOfVectorSpaces, typename Basis> struct IsABasedTensorPowerOfVectorSpaces_c<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> > { static bool const V = true; };

template <typename TensorPowerOfVectorSpaces, typename Basis>
struct DualOf_c<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> >
{
    typedef BasedTensorPowerOfVectorSpaces_c<typename DualOf_c<TensorPowerOfVectorSpaces>::T,typename DualOf_c<Basis>::T> T;
};


// Factor_ must be a BasedVectorSpace_c type
template <typename Factor_, Uint32 ORDER_>
struct TensorPowerOfBasedVectorSpaces_c
    : 
    public TensorPowerOfVectorSpaces_c<typename Factor_::VectorSpace,ORDER_>,
    public BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_c<typename Factor_::VectorSpace,ORDER_>,
                                            TensorPowerOfBases_c<typename Factor_::Basis,ORDER_> >
{
    typedef TensorPowerOfVectorSpaces_c<typename Factor_::VectorSpace,ORDER_> Parent_TensorPowerOfVectorSpaces;
    typedef BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_c<typename Factor_::VectorSpace,ORDER_>,
                                             TensorPowerOfBases_c<typename Factor_::Basis,ORDER_> > Parent_BasedTensorPowerOfVectorSpaces;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsABasedVectorSpace_c<Factor_>::V, MUST_BE_BASED_VECTOR_SPACE),
    };

    typedef typename Parent_TensorPowerOfVectorSpaces::FactorTypeList FactorTypeList;
    using Parent_TensorPowerOfVectorSpaces::ORDER;
    typedef typename Parent_BasedTensorPowerOfVectorSpaces::Field Field;
    using Parent_BasedTensorPowerOfVectorSpaces::DIM;
    typedef typename Parent_BasedTensorPowerOfVectorSpaces::Id Id;
    typedef typename Parent_BasedTensorPowerOfVectorSpaces::Basis Basis;
    typedef typename DualOf_c<TensorPowerOfBasedVectorSpaces_c>::T Dual; // relies on the template specialization below
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorProductOfBasedVectorSpaces_c<" + TypeStringOf_t<FactorTypeList>::eval() + '>';
    }
};

template <typename Factor, Uint32 ORDER> struct IsAVectorSpace_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsATensorProduct_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsATensorPower_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsATensorProductOfVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsATensorPowerOfVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsABasedVectorSpace_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsABasedTensorProductOfVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsABasedTensorPowerOfVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsATensorProductOfBasedVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

template <typename T> struct IsATensorPowerOfBasedVectorSpaces_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsATensorPowerOfBasedVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

template <typename Factor, Uint32 ORDER>
struct DualOf_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> >
{
    typedef TensorPowerOfBasedVectorSpaces_c<typename DualOf_c<Factor>::T,ORDER> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_TENSORPOWER_HPP_
