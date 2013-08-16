// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/tensorpower.hpp by Victor Dods, created 2013/07/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_TENSORPOWER_HPP_
#define TENH_CONCEPTUAL_TENSORPOWER_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/meta/typelist_utility.hpp"

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
{
    typedef TensorProduct_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T> As_TensorProduct;

    typedef typename As_TensorProduct::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_TensorProduct::ORDER;
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
{
    typedef TensorPower_c<Factor_,ORDER_> As_TensorPower;
    typedef TensorProductOfVectorSpaces_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T> As_TensorProductOfVectorSpaces;
    typedef typename As_TensorProductOfVectorSpaces::As_TensorProduct As_TensorProduct;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsAVectorSpace_c<Factor_>::V, MUST_BE_VECTOR_SPACE),
    };

    typedef typename As_TensorProductOfVectorSpaces::As_VectorSpace As_VectorSpace;

    typedef typename As_TensorPower::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_TensorPower::ORDER;
    typedef typename As_TensorProductOfVectorSpaces::Field Field;
    static Uint32 const DIM = As_TensorProductOfVectorSpaces::DIM;
    typedef typename As_TensorProductOfVectorSpaces::Id Id;
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
{
    typedef TensorPower_c<Factor_,ORDER_> As_TensorPower;
    typedef Basis_c<TensorPower_c<Factor_,ORDER_> > As_Basis;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsABasis_c<Factor_>::V, MUST_BE_BASIS)
    };

    typedef typename As_TensorPower::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_TensorPower::ORDER;
    typedef typename As_Basis::Id Id;
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
{
    typedef TensorPowerOfVectorSpaces As_TensorPowerOfVectorSpaces;
    typedef BasedTensorProductOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis_> As_BasedTensorProductOfVectorSpaces;
    typedef typename As_TensorPowerOfVectorSpaces::As_TensorProductOfVectorSpaces As_TensorProductOfVectorSpaces;
    typedef typename As_TensorPowerOfVectorSpaces::As_TensorProduct As_TensorProduct;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsATensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces>::V, MUST_BE_TENSOR_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(IsABasis_c<Basis_>::V, MUST_BE_BASIS),
    };

    typedef typename As_TensorPowerOfVectorSpaces::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_TensorPowerOfVectorSpaces::ORDER;
    typedef typename As_BasedTensorProductOfVectorSpaces::Field Field;
    static Uint32 const DIM = As_BasedTensorProductOfVectorSpaces::DIM;
    typedef typename As_BasedTensorProductOfVectorSpaces::Id Id;
    typedef typename As_BasedTensorProductOfVectorSpaces::Basis Basis;
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
{
    enum
    {
        STATIC_ASSERT_IN_ENUM(IsABasedVectorSpace_c<Factor_>::V, MUST_BE_BASED_VECTOR_SPACE),
    };

    typedef TensorProductOfVectorSpaces_c<typename UniformTypeListOfLength_t<typename Factor_::As_VectorSpace,ORDER_>::T> As_TensorProductOfVectorSpaces;
    typedef TensorProductOfBasedVectorSpaces_c<typename UniformTypeListOfLength_t<Factor_,ORDER_>::T> As_TensorProductOfBasedVectorSpaces;
    typedef TensorPowerOfVectorSpaces_c<typename Factor_::As_VectorSpace,ORDER_> As_TensorPowerOfVectorSpaces;
    typedef BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_c<typename Factor_::As_VectorSpace,ORDER_>,
                                             TensorPowerOfBases_c<typename Factor_::Basis,ORDER_> > As_BasedTensorPowerOfVectorSpaces;

    typedef typename As_TensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_TensorPowerOfVectorSpaces::ORDER;
    typedef typename As_BasedTensorPowerOfVectorSpaces::Field Field;
    static Uint32 const DIM = As_BasedTensorPowerOfVectorSpaces::DIM;
    typedef typename As_BasedTensorPowerOfVectorSpaces::Id Id;
    typedef typename As_BasedTensorPowerOfVectorSpaces::Basis Basis;
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
