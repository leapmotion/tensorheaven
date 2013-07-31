// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/symmetricpower.hpp by Victor Dods, created 2013/07/29
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_
#define TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/tensorpower.hpp" // for TypeListWithMultiplicity_t and probably nothing else
#include "tenh/conceptual/vectorspace.hpp"

namespace Tenh {

// fully symmetric tensor power of formal symbols (e.g. identifiers, builtin C++ types, etc)
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
// TODO: real dimension calculation
template <typename Factor_, Uint32 ORDER_>
struct SymmetricPowerOfVectorSpaces_c
    : 
    public SymmetricPower_c<Factor_,ORDER_>,
    public VectorSpace_c<typename Factor_::Field,42,SymmetricPower_c<typename Factor_::Id,ORDER_> >
{
    typedef SymmetricPower_c<Factor_,ORDER_> Parent_SymmetricPower;
    typedef VectorSpace_c<typename Factor_::Field,42,SymmetricPower_c<typename Factor_::Id,ORDER_> > Parent_VectorSpace;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsAVectorSpace_c<Factor_>::V, MUST_BE_VECTOR_SPACE),
    };

    typedef typename Parent_SymmetricPower::FactorTypeList FactorTypeList;
    using Parent_SymmetricPower::ORDER;
    typedef typename Parent_VectorSpace::Field Field;
    using Parent_VectorSpace::DIM;
    typedef typename Parent_VectorSpace::Id Id;
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
    : 
    public SymmetricPower_c<Factor_,ORDER_>,
    public Basis_c<SymmetricPower_c<Factor_,ORDER_> >
{
    typedef SymmetricPower_c<Factor_,ORDER_> Parent_SymmetricPower;
    typedef Basis_c<SymmetricPower_c<Factor_,ORDER_> > Parent_Basis;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsABasis_c<Factor_>::V, MUST_BE_BASIS)
    };

    //typedef typename Parent_SymmetricPower::FactorTypeList FactorTypeList;
    using Parent_SymmetricPower::ORDER;
    typedef typename Parent_Basis::Id Id;
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
    :
    public SymmetricPowerOfVectorSpaces,
    public BasedSymmetricProductOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis_>
{
    typedef SymmetricPowerOfVectorSpaces Parent_SymmetricPowerOfVectorSpaces;
    typedef BasedSymmetricProductOfVectorSpaces_c<SymmetricPowerOfVectorSpaces,Basis_> Parent_BasedSymmetricProductOfVectorSpaces;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsASymmetricPowerOfVectorSpaces_c<SymmetricPowerOfVectorSpaces>::V, MUST_BE_SYMMETRIC_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(IsABasis_c<Basis_>::V, MUST_BE_BASIS),
    };

    //typedef typename Parent_SymmetricPowerOfVectorSpaces::FactorTypeList FactorTypeList;
    using Parent_SymmetricPowerOfVectorSpaces::ORDER;
    typedef typename Parent_BasedSymmetricProductOfVectorSpaces::Field Field;
    using Parent_BasedSymmetricProductOfVectorSpaces::DIM;
    typedef typename Parent_BasedSymmetricProductOfVectorSpaces::Id Id;
    typedef typename Parent_BasedSymmetricProductOfVectorSpaces::Basis Basis;
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
    : 
    public SymmetricPowerOfVectorSpaces_c<typename Factor_::VectorSpace,ORDER_>,
    public BasedVectorSpace_c<SymmetricPowerOfVectorSpaces_c<typename Factor_::VectorSpace,ORDER_>,
                              SymmetricPowerOfBases_c<typename Factor_::Basis,ORDER_> >
{
    typedef SymmetricPowerOfVectorSpaces_c<typename Factor_::VectorSpace,ORDER_> Parent_SymmetricPowerOfVectorSpaces;
    typedef BasedVectorSpace_c<SymmetricPowerOfVectorSpaces_c<typename Factor_::VectorSpace,ORDER_>,
                               SymmetricPowerOfBases_c<typename Factor_::Basis,ORDER_> > Parent_BasedVectorSpace;

    enum
    {
        STATIC_ASSERT_IN_ENUM(IsABasedVectorSpace_c<Factor_>::V, MUST_BE_BASED_VECTOR_SPACE),
    };

    typedef typename Parent_SymmetricPowerOfVectorSpaces::FactorTypeList FactorTypeList;
    using Parent_SymmetricPowerOfVectorSpaces::ORDER;
    typedef typename Parent_BasedVectorSpace::Field Field;
    using Parent_BasedVectorSpace::DIM;
    typedef typename Parent_BasedVectorSpace::Id Id;
    typedef typename Parent_BasedVectorSpace::Basis Basis;
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

template <typename T> struct IsASymmetricPowerOfBasedVectorSpaces_c { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsASymmetricPowerOfBasedVectorSpaces_c<SymmetricPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_SYMMETRICPOWER_HPP_
