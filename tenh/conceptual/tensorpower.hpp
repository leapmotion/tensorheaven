// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/tensorpower.hpp by Victor Dods, created 2013/07/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_TENSORPOWER_HPP_
#define TENH_CONCEPTUAL_TENSORPOWER_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"
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
private:
    typedef TensorProduct_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T> As_TensorProduct;
public:
    typedef TypeList_t<As_TensorProduct> ParentTypeList;

    typedef typename As_TensorProduct::FactorTypeList FactorTypeList;
    static Uint32 const ORDER = As_TensorProduct::ORDER;
    typedef typename DualOf_c<TensorPower_c>::T Dual; // relies on the template specialization below
    typedef Factor_ Factor;

    static std::string type_as_string ()
    {
        return "TensorPower_c<" + TypeStringOf_t<Factor>::eval() + ',' + AS_STRING(ORDER) + '>';
    }
};

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPower_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPower_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPower_f<TensorPower_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPower);
// special convenience macros
#define IS_TENSOR_POWER_UNIQUELY(Concept) HasUniqueTensorPowerStructure_f<Concept>::V
#define AS_TENSOR_POWER(Concept) UniqueTensorPowerStructureOf_f<Concept>::T

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
private:
    typedef TensorPower_c<Factor_,ORDER_> As_TensorPower;
    typedef TensorProductOfVectorSpaces_c<typename TypeListWithMultiplicity_t<Factor_,ORDER_>::T> As_TensorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_TensorPower, TypeList_t<As_TensorProductOfVectorSpaces> > ParentTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(HasVectorSpaceStructure_f<Factor_>::V, MUST_BE_VECTOR_SPACE),
    };

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

template<typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPowerOfVectorSpaces_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPowerOfVectorSpaces_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPowerOfVectorSpaces_f<TensorPowerOfVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPowerOfVectorSpaces);
// special convenience macros
#define IS_TENSOR_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueTensorPowerOfVectorSpacesStructure_f<Concept>::V
#define AS_TENSOR_POWER_OF_VECTOR_SPACES(Concept) UniqueTensorPowerOfVectorSpacesStructureOf_f<Concept>::T

template <typename Factor, Uint32 ORDER>
struct DualOf_c<TensorPowerOfVectorSpaces_c<Factor,ORDER> >
{
    typedef TensorPowerOfVectorSpaces_c<typename DualOf_c<Factor>::T,ORDER> T;
};


// Factor_ must be a Basis_c type
template <typename Factor_, Uint32 ORDER_>
struct TensorPowerOfBases_c
{
private:
    typedef TensorPower_c<Factor_,ORDER_> As_TensorPower;
    typedef Basis_c<TensorPower_c<Factor_,ORDER_> > As_Basis;
public:
    typedef TypeList_t<As_TensorPower, TypeList_t<As_Basis> > ParentTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(HasBasisStructure_f<Factor_>::V, MUST_BE_BASIS)
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

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPowerOfBases_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPowerOfBases_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPowerOfBases_f<TensorPowerOfBases_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPowerOfBases);
// special convenience macros
#define IS_TENSOR_POWER_OF_BASES_UNIQUELY(Concept) HasUniqueTensorPowerOfBasesStructure_f<Concept>::V
#define AS_TENSOR_POWER_OF_BASES(Concept) UniqueTensorPowerOfBasesStructureOf_f<Concept>::T

template <typename Factor, Uint32 ORDER>
struct DualOf_c<TensorPowerOfBases_c<Factor,ORDER> >
{
    typedef TensorPowerOfBases_c<typename DualOf_c<Factor>::T,ORDER> T;
};


template <typename TensorPowerOfVectorSpaces, typename Basis_>
struct BasedTensorPowerOfVectorSpaces_c
{
private:
    typedef TensorPowerOfVectorSpaces As_TensorPowerOfVectorSpaces;
    typedef BasedTensorProductOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis_> As_BasedTensorProductOfVectorSpaces;
public:
    typedef TypeList_t<As_TensorPowerOfVectorSpaces, TypeList_t<As_BasedTensorProductOfVectorSpaces> > ParentTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(HasTensorPowerOfVectorSpacesStructure_f<TensorPowerOfVectorSpaces>::V, MUST_BE_TENSOR_POWER_OF_VECTOR_SPACES),
        STATIC_ASSERT_IN_ENUM(HasBasisStructure_f<Basis_>::V, MUST_BE_BASIS),
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

template <typename TensorPowerOfVectorSpaces, typename Basis_>
struct IsConcept_f<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces, Basis_> >
{ static bool const V = true; };

template <typename T> struct IsBasedTensorPowerOfVectorSpaces_f { static bool const V = false; };
template <typename TensorPowerOfVectorSpaces, typename Basis> struct IsBasedTensorPowerOfVectorSpaces_f<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedTensorPowerOfVectorSpaces);
// special convenience macros
#define IS_BASED_TENSOR_POWER_OF_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueBasedTensorPowerOfVectorSpacesStructure_f<Concept>::V
#define AS_BASED_TENSOR_POWER_OF_VECTOR_SPACES(Concept) UniqueBasedTensorPowerOfVectorSpacesStructureOf_f<Concept>::T

template <typename TensorPowerOfVectorSpaces, typename Basis>
struct DualOf_c<BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces,Basis> >
{
    typedef BasedTensorPowerOfVectorSpaces_c<typename DualOf_c<TensorPowerOfVectorSpaces>::T,typename DualOf_c<Basis>::T> T;
};


// TODO: Consider what the ParentTypeList should actually be here.
// Factor_ must be a BasedVectorSpace_c type
template <typename Factor_, Uint32 ORDER_>
struct TensorPowerOfBasedVectorSpaces_c
{
private:
    typedef TensorProductOfVectorSpaces_c<typename UniformTypeListOfLength_t<typename AS_VECTOR_SPACE(Factor_),ORDER_>::T> As_TensorProductOfVectorSpaces;
    typedef TensorProductOfBasedVectorSpaces_c<typename UniformTypeListOfLength_t<Factor_,ORDER_>::T> As_TensorProductOfBasedVectorSpaces;
    typedef TensorPowerOfVectorSpaces_c<typename AS_VECTOR_SPACE(Factor_),ORDER_> As_TensorPowerOfVectorSpaces;
    typedef BasedTensorPowerOfVectorSpaces_c<TensorPowerOfVectorSpaces_c<typename AS_VECTOR_SPACE(Factor_),ORDER_>,
                                             TensorPowerOfBases_c<typename Factor_::Basis,ORDER_> > As_BasedTensorPowerOfVectorSpaces;
public:
    typedef TypeList_t<As_BasedTensorPowerOfVectorSpaces, TypeList_t<As_TensorProductOfBasedVectorSpaces> > ParentTypeList;

    enum
    {
        STATIC_ASSERT_IN_ENUM(HasBasedVectorSpaceStructure_f<Factor_>::V, MUST_BE_BASED_VECTOR_SPACE),
    };


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

template <typename Factor_, Uint32 ORDER_>
struct IsConcept_f<TensorPowerOfBasedVectorSpaces_c<Factor_, ORDER_> >
{ static bool const V = true; };

template <typename T> struct IsTensorPowerOfBasedVectorSpaces_f { static bool const V = false; };
template <typename Factor, Uint32 ORDER> struct IsTensorPowerOfBasedVectorSpaces_f<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(TensorPowerOfBasedVectorSpaces);
// special convenience macros
#define IS_TENSOR_POWER_OF_BASED_VECTOR_SPACES_UNIQUELY(Concept) HasUniqueTensorPowerOfBasedVectorSpacesStructure_f<Concept>::V
#define AS_TENSOR_POWER_OF_BASED_VECTOR_SPACES(Concept) UniqueTensorPowerOfBasedVectorSpacesStructureOf_f<Concept>::T

template <typename Factor, Uint32 ORDER>
struct DualOf_c<TensorPowerOfBasedVectorSpaces_c<Factor,ORDER> >
{
    typedef TensorPowerOfBasedVectorSpaces_c<typename DualOf_c<Factor>::T,ORDER> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_TENSORPOWER_HPP_
