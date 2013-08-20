// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/symmetricpower.hpp by Victor Dods, created 2013/07/29
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_
#define TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_

#include "tenh/core.hpp"
#include "tenh/mathutil.hpp"

#include "tenh/conceptual/tensorpower.hpp" // for TypeListWithMultiplicity_t and probably nothing else
#include "tenh/conceptual/vectorspace.hpp"

namespace Tenh {

// symmetric power of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPower_c
{
    static Uint32 const ORDER = ORDER_;
    typedef Factor_ Factor;
    typedef typename TypeListWithMultiplicity_t<Factor,ORDER>::T FactorTypeList;
    typedef typename DualOf_c<SymmetricPower_c>::T Dual; // the dual is not the symmetric power of Factor::Dual

    static std::string type_as_string ()
    {
        return "SymmetricPower_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename T> struct IsASymmetricPower_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsASymmetricPower_c<SymmetricPower_c<Factor,ORDER> > { static bool const V = true; };




// FactorTypeList_ must be a TypeList_t of VectorSpace_c types
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPowerOfVectorSpaces_c
{
    typedef SymmetricPower_c<Factor_,ORDER_> As_SymmetricPower;
    typedef VectorSpace_c<typename Factor_::Field,BinomialCoefficient_t<Factor_::DIM + ORDER_ - 1, ORDER_>::V,SymmetricPower_c<typename Factor_::Id,ORDER_> > As_VectorSpace;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsAVectorSpace_c<Factor_>::V, MUST_BE_VECTOR_SPACE),
    };

    typedef typename As_SymmetricPower::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_SymmetricPower::ORDER;
    typedef typename As_VectorSpace::Field Field;
    static Uint32 const DIM = As_VectorSpace::DIM;
    typedef typename As_VectorSpace::Id Id;
    typedef typename DualOf_c<SymmetricPowerOfVectorSpaces_c>::T Dual; // the dual is not the symmetric power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricPowerOfVectorSpaces_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor, Uint32 ORDER> struct IsASymmetricPower_c<SymmetricPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsAVectorSpace_c<SymmetricPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

template <typename T> struct IsASymmetricPowerOfVectorSpaces_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsASymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };


// Factor_ must be a Basis_c type
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPowerOfBases_c
{
    typedef SymmetricPower_c<Factor_,ORDER_> As_SymmetricPower;
    typedef Basis_c<SymmetricPower_c<Factor_,ORDER_> > As_Basis;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsABasis_c<Factor_>::V, MUST_BE_BASIS)
    };

    //typedef typename As_SymmetricPower::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_SymmetricPower::ORDER;
    typedef typename As_Basis::Id Id;
    typedef typename DualOf_c<SymmetricPowerOfBases_c>::T Dual; // the dual is not the symmetric power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricPowerOfBases_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor, Uint32 ORDER> struct IsASymmetricPower_c<SymmetricPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsABasis_c<SymmetricPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };

template <typename T> struct IsASymmetricPowerOfBases_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsASymmetricPowerOfBases_c<SymmetricPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };


/*
// TODO: figure out if this struct is necessary
template <typename SymmetricPowerOfVectorSpaces, typename Basis_>
struct BasedSymmetricPowerOfVectorSpaces_c
{
    typedef SymmetricPowerOfVectorSpaces As_SymmetricPowerOfVectorSpaces;
    typedef BasedSymmetricProductOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis_> As_BasedSymmetricProductOfVectorSpaces;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsASymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces>::V, MUST_BE_SYMMETRIC_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(IsABasis_c<Basis_>::V, MUST_BE_BASIS),
    };

    //typedef typename As_SymmetricPowerOfVectorSpaces::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_SymmetricPowerOfVectorSpaces::ORDER;
    typedef typename As_BasedSymmetricProductOfVectorSpaces::Field Field;
    static Uint32 const DIM = As_BasedSymmetricProductOfVectorSpaces::DIM;
    typedef typename As_BasedSymmetricProductOfVectorSpaces::Id Id;
    typedef typename As_BasedSymmetricProductOfVectorSpaces::Basis Basis;
    typedef typename DualOf_c<BasedSymmetricPowerOfVectorSpaces_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "BasedSymmetricPowerOfVectorSpaces_c<" + TypeStringOf_t<SymmetricPowerOfVectorSpaces>::eval() + ','
                                                      + TypeStringOf_t<Basis>::eval() + '>';
    }
};

template <typename SymmetricPowerOfVectorSpaces, typename Basis> struct IsAVectorSpace_c<BasedSymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis> > { static bool const V = true; };
template <typename SymmetricPowerOfVectorSpaces, typename Basis> struct IsABasedVectorSpace_c<BasedSymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis> > { static bool const V = true; };
template <typename SymmetricPowerOfVectorSpaces, typename Basis> struct IsABasedSymmetricProductOfVectorSpaces_c<BasedSymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis> > { static bool const V = true; };

template <typename T> struct IsABasedSymmetricPowerOfVectorSpaces_c { static bool const V = false; };
template <typename SymmetricPowerOfVectorSpaces, typename Basis> struct IsABasedSymmetricPowerOfVectorSpaces_c<BasedSymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis> > { static bool const V = true; };

template <typename SymmetricPowerOfVectorSpaces, typename Basis>
struct DualOf_c<BasedSymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis> >
{
    // NOTE: not really sure how to define this, because the dual of a symmetric power isn't really
    // the symmetric power of the dual (this requires a particular identification)
    typedef BasedTensorPowerOfVectorSpaces_c<typename DualOf_c<TensorPowerOfVectorSpaces>::T,typename DualOf_c<Basis>::T> T;
};
*/

// Factor_ must be a BasedVectorSpace_c type
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPowerOfBasedVectorSpaces_c
{
    typedef SymmetricPowerOfVectorSpaces_c<typename Factor_::As_VectorSpace,ORDER_> As_SymmetricPowerOfVectorSpaces;
    typedef BasedVectorSpace_c<SymmetricPowerOfVectorSpaces_c<typename Factor_::As_VectorSpace,ORDER_>,
                               SymmetricPowerOfBases_c<typename Factor_::Basis,ORDER_> > As_BasedVectorSpace;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<TensorPowerOfBasedVectorSpaces_c<Factor_, ORDER_> > As_EmbeddableInTensorProductOfBasedVectorSpaces;

    typedef typename As_EmbeddableInTensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;

    enum { STATIC_ASSERT_IN_ENUM(AllFactorsAreBasedVectorSpaces_t<FactorTypeList>::V, ALL_FACTORS_MUST_BE_BASED_VECTOR_SPACES) };

    typedef TensorProductOfBasedVectorSpaces_c<FactorTypeList> TensorProductOfBasedVectorSpaces;
    static Uint32 const ORDER = As_SymmetricPowerOfVectorSpaces::ORDER;
    typedef typename As_BasedVectorSpace::Field Field;
    static Uint32 const DIM = As_BasedVectorSpace::DIM;
    typedef typename As_BasedVectorSpace::Id Id;
    typedef typename As_BasedVectorSpace::Basis Basis;
    typedef typename DualOf_c<SymmetricPowerOfBasedVectorSpaces_c>::T Dual; // the dual is not the symmetric power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "SymmetricProductOfBasedVectorSpaces_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor, Uint32 ORDER> struct IsAVectorSpace_c<SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsASymmetricPower_c<SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsASymmetricPowerOfVectorSpaces_c<SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsABasedVectorSpace_c<SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsEmbeddableInTensorProductOfBasedVectorSpaces_c<SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

template <typename T> struct IsASymmetricPowerOfBasedVectorSpaces_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsASymmetricPowerOfBasedVectorSpaces_c<SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_
