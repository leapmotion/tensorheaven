// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/exteriorpower.hpp by Victor Dods, created 2013/08/15
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_EXTERIORPOWER_HPP_
#define TENH_CONCEPTUAL_EXTERIORPOWER_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/tensorpower.hpp" // for TypeListWithMultiplicity_t and probably nothing else
#include "tenh/conceptual/vectorspace.hpp"

namespace Tenh {

// exterior power of formal symbols (e.g. identifiers, builtin C++ types, etc)
template <typename Factor_, Uint32 ORDER_>
struct ExteriorPower_c
{
    static Uint32 const ORDER = ORDER_;
    typedef Factor_ Factor;
    typedef typename TypeListWithMultiplicity_t<Factor,ORDER>::T FactorTypeList;
    typedef typename DualOf_c<ExteriorPower_c>::T Dual; // the dual is not the exterior power of Factor::Dual

    static std::string type_as_string ()
    {
        return "ExteriorPower_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename T> struct IsAnExteriorPower_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsAnExteriorPower_c<ExteriorPower_c<Factor,ORDER> > { static bool const V = true; };




// FactorTypeList_ must be a TypeList_t of VectorSpace_c types
// TODO: real dimension calculation
template <typename Factor_, Uint32 ORDER_>
struct ExteriorPowerOfVectorSpaces_c
{
    typedef ExteriorPower_c<Factor_,ORDER_> As_ExteriorPower;
    typedef VectorSpace_c<typename Factor_::Field,42,ExteriorPower_c<typename Factor_::Id,ORDER_> > As_VectorSpace;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsAVectorSpace_c<Factor_>::V, MUST_BE_VECTOR_SPACE),
    };

    typedef typename As_ExteriorPower::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_ExteriorPower::ORDER;
    typedef typename As_VectorSpace::Field Field;
    static Uint32 const DIM = As_VectorSpace::DIM;
    typedef typename As_VectorSpace::Id Id;
    typedef typename DualOf_c<ExteriorPowerOfVectorSpaces_c>::T Dual; // the dual is not the exterior power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorPowerOfVectorSpaces_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor, Uint32 ORDER> struct IsAnExteriorPower_c<ExteriorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsAVectorSpace_c<ExteriorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

template <typename T> struct IsAExteriorPowerOfVectorSpaces_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsAExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };


// Factor_ must be a Basis_c type
template <typename Factor_, Uint32 ORDER_>
struct ExteriorPowerOfBases_c
{
    typedef ExteriorPower_c<Factor_,ORDER_> As_ExteriorPower;
    typedef Basis_c<ExteriorPower_c<Factor_,ORDER_> > As_Basis;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsABasis_c<Factor_>::V, MUST_BE_BASIS)
    };

    //typedef typename As_ExteriorPower::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_ExteriorPower::ORDER;
    typedef typename As_Basis::Id Id;
    typedef typename DualOf_c<ExteriorPowerOfBases_c>::T Dual; // the dual is not the exterior power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorPowerOfBases_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor, Uint32 ORDER> struct IsAnExteriorPower_c<ExteriorPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsABasis_c<ExteriorPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };

template <typename T> struct IsAnExteriorPowerOfBases_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsAnExteriorPowerOfBases_c<ExteriorPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };


/*
// TODO: figure out if this struct is necessary
template <typename ExteriorPowerOfVectorSpaces, typename Basis_>
struct BasedExteriorPowerOfVectorSpaces_c
{
    typedef ExteriorPowerOfVectorSpaces As_ExteriorPowerOfVectorSpaces;
    typedef BasedExteriorProductOfVectorSpaces_c<ExteriorPowerOfVectorSpaces,Basis_> As_BasedExteriorProductOfVectorSpaces;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsAExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces>::V, MUST_BE_EXTERIOR_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(IsABasis_c<Basis_>::V, MUST_BE_BASIS),
    };

    //typedef typename As_ExteriorPowerOfVectorSpaces::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_ExteriorPowerOfVectorSpaces::ORDER;
    typedef typename As_BasedExteriorProductOfVectorSpaces::Field Field;
    static Uint32 const DIM = As_BasedExteriorProductOfVectorSpaces::DIM;
    typedef typename As_BasedExteriorProductOfVectorSpaces::Id Id;
    typedef typename As_BasedExteriorProductOfVectorSpaces::Basis Basis;
    typedef typename DualOf_c<BasedExteriorPowerOfVectorSpaces_c>::T Dual; // relies on the template specialization below

    static std::string type_as_string ()
    {
        return "BasedExteriorPowerOfVectorSpaces_c<" + TypeStringOf_t<ExteriorPowerOfVectorSpaces>::eval() + ','
                                                      + TypeStringOf_t<Basis>::eval() + '>';
    }
};

template <typename ExteriorPowerOfVectorSpaces, typename Basis> struct IsAVectorSpace_c<BasedExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces,Basis> > { static bool const V = true; };
template <typename ExteriorPowerOfVectorSpaces, typename Basis> struct IsABasedVectorSpace_c<BasedExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces,Basis> > { static bool const V = true; };
template <typename ExteriorPowerOfVectorSpaces, typename Basis> struct IsABasedExteriorProductOfVectorSpaces_c<BasedExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces,Basis> > { static bool const V = true; };

template <typename T> struct IsABasedExteriorPowerOfVectorSpaces_c { static bool const V = false; };
template <typename ExteriorPowerOfVectorSpaces, typename Basis> struct IsABasedExteriorPowerOfVectorSpaces_c<BasedExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces,Basis> > { static bool const V = true; };

template <typename ExteriorPowerOfVectorSpaces, typename Basis>
struct DualOf_c<BasedExteriorPowerOfVectorSpaces_c<ExteriorPowerOfVectorSpaces,Basis> >
{
    // NOTE: not really sure how to define this, because the dual of a exterior power isn't really
    // the exterior power of the dual (this requires a particular identification)
    typedef BasedTensorPowerOfVectorSpaces_c<typename DualOf_c<TensorPowerOfVectorSpaces>::T,typename DualOf_c<Basis>::T> T;
};
*/

// Factor_ must be a BasedVectorSpace_c type
template <typename Factor_, Uint32 ORDER_>
struct ExteriorPowerOfBasedVectorSpaces_c
{
    typedef ExteriorPowerOfVectorSpaces_c<typename Factor_::As_VectorSpace,ORDER_> As_ExteriorPowerOfVectorSpaces;
    typedef BasedVectorSpace_c<ExteriorPowerOfVectorSpaces_c<typename Factor_::As_VectorSpace,ORDER_>,
                               ExteriorPowerOfBases_c<typename Factor_::Basis,ORDER_> > As_BasedVectorSpace;
    typedef EmbeddableInTensorProductOfBasedVectorSpaces_c<typename TensorPowerOfBasedVectorSpaces_c<Factor_,ORDER_> > As_EmbeddableInTensorProductOfBasedVectorSpaces;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsABasedVectorSpace_c<Factor_>::V, MUST_BE_BASED_VECTOR_SPACE),
    };

    typedef typename As_ExteriorPowerOfVectorSpaces::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_ExteriorPowerOfVectorSpaces::ORDER;
    typedef typename As_BasedVectorSpace::Field Field;
    static Uint32 const DIM = As_BasedVectorSpace::DIM;
    typedef typename As_BasedVectorSpace::Id Id;
    typedef typename As_BasedVectorSpace::Basis Basis;
    typedef typename DualOf_c<ExteriorPowerOfBasedVectorSpaces_c>::T Dual; // the dual is not the exterior power of Factor::Dual
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "ExteriorProductOfBasedVectorSpaces_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor, Uint32 ORDER> struct IsAVectorSpace_c<ExteriorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsAnExteriorPower_c<ExteriorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsAExteriorPowerOfVectorSpaces_c<ExteriorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsABasedVectorSpace_c<ExteriorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };
template <typename Factor, Uint32 ORDER> struct IsEmbeddableInTensorProductOfBasedVectorSpaces_c<ExteriorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

template <typename T> struct IsAExteriorPowerOfBasedVectorSpaces_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsAExteriorPowerOfBasedVectorSpaces_c<ExteriorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_EXTERIORPOWER_HPP_
