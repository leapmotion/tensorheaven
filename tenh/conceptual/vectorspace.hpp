// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/vectorspace.hpp by Victor Dods, created 2013/07/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_VECTORSPACE_HPP_
#define TENH_CONCEPTUAL_VECTORSPACE_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/conceptual/field.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// VectorSpace_c
// ///////////////////////////////////////////////////////////////////////////

template <typename ScalarField_, Uint32 DIMENSION_, typename Id_>
struct VectorSpace_c
{
    typedef EmptyTypeList ParentTypeList;

    enum { STATIC_ASSERT_IN_ENUM(IS_FIELD_UNIQUELY(ScalarField_), MUST_BE_FIELD) };

    typedef ScalarField_ ScalarField;
    static Uint32 const DIMENSION = DIMENSION_;
    typedef Id_ Id;

    static std::string type_as_string ()
    {
        return "VectorSpace_c<" + type_string_of<ScalarField_>() + ','
                                + AS_STRING(DIMENSION_) + ','
                                + type_string_of<Id_>() + '>';
    }
};

template <typename ScalarField_, Uint32 DIMENSION_, typename Id_>
struct IsConcept_f<VectorSpace_c<ScalarField_, DIMENSION_, Id_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T_> struct IsVectorSpace_f
{
    static bool const V = false;
private:
    IsVectorSpace_f();
};
template <typename ScalarField_, Uint32 DIMENSION_, typename Id_> struct IsVectorSpace_f<VectorSpace_c<ScalarField_,DIMENSION_,Id_> >
{
    static bool const V = true;
private:
    IsVectorSpace_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(VectorSpace);
// special convenience macros
#define IS_VECTOR_SPACE_UNIQUELY(Concept) HasUniqueVectorSpaceStructure_f<Concept>::V
#define AS_VECTOR_SPACE(Concept) UniqueVectorSpaceStructureOf_f<Concept>::T

template <typename ScalarField_, Uint32 DIMENSION_, typename Id_>
struct DualOf_f<VectorSpace_c<ScalarField_,DIMENSION_,Id_> >
{
    typedef VectorSpace_c<ScalarField_,DIMENSION_,typename DualOf_f<Id_>::T> T;
private:
    DualOf_f();
};

// property IDs

struct Dimension; // could this be a forward declaration?
struct Id; // could this be a forward declaration?
struct ScalarField; // could this be a forward declaration?

// BaseProperty_f accessors

template <typename ScalarField_, Uint32 DIMENSION_, typename Id_>
struct BaseProperty_f<VectorSpace_c<ScalarField_,DIMENSION_,Id_>,Dimension>
{
    typedef Value_t<Uint32,DIMENSION_> T;
private:
    BaseProperty_f();
};

template <typename ScalarField_, Uint32 DIMENSION_, typename Id_>
struct BaseProperty_f<VectorSpace_c<ScalarField_,DIMENSION_,Id_>,Id>
{
    typedef Id_ T;
private:
    BaseProperty_f();
};

template <typename ScalarField_, Uint32 DIMENSION_, typename Id_>
struct BaseProperty_f<VectorSpace_c<ScalarField_,DIMENSION_,Id_>,ScalarField>
{
    typedef ScalarField_ T;
private:
    BaseProperty_f();
};

// named property accessors

template <typename Concept_> struct DimensionOf_f
{
    static Uint32 const V = PropertyValue_f<Concept_,Dimension>::V;
private:
    DimensionOf_f();
};
template <typename Concept_> struct IdOf_f
{
    typedef typename Property_f<Concept_,Id>::T T;
private:
    IdOf_f();
};
template <typename Concept_> struct ScalarFieldOf_f
{
    typedef typename Property_f<Concept_,ScalarField>::T T;
private:
    ScalarFieldOf_f();
};

// ///////////////////////////////////////////////////////////////////////////
// BasedVectorSpace_c
// ///////////////////////////////////////////////////////////////////////////

// TODO: should these forgetful functors be here?
template <typename VectorSpace_, typename Basis_>
struct BasedVectorSpace_c
{
private:
    enum
    {
        STATIC_ASSERT_IN_ENUM(IS_VECTOR_SPACE_UNIQUELY(VectorSpace_), MUST_BE_VECTOR_SPACE),
        STATIC_ASSERT_IN_ENUM(IS_BASIS_UNIQUELY(Basis_), MUST_BE_BASIS)
    };

    typedef VectorSpace_ As_VectorSpace;
public:
    typedef TypeList_t<As_VectorSpace> ParentTypeList;

    typedef typename As_VectorSpace::Id Id;
    typedef Basis_ Basis;

    static std::string type_as_string ()
    {
        return "BasedVectorSpace_c<" + type_string_of<VectorSpace_>() + ',' + type_string_of<Basis_>() + '>';
    }
};

template <typename VectorSpace_, typename Basis_>
struct IsConcept_f<BasedVectorSpace_c<VectorSpace_, Basis_> >
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T_> struct IsBasedVectorSpace_f
{
    static bool const V = false;
private:
    IsBasedVectorSpace_f();
};
template <typename VectorSpace_, typename Basis_> struct IsBasedVectorSpace_f<BasedVectorSpace_c<VectorSpace_,Basis_> >
{
    static bool const V = true;
private:
    IsBasedVectorSpace_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedVectorSpace);
// special convenience macros
#define IS_BASED_VECTOR_SPACE_UNIQUELY(Concept) HasUniqueBasedVectorSpaceStructure_f<Concept>::V
#define AS_BASED_VECTOR_SPACE(Concept) UniqueBasedVectorSpaceStructureOf_f<Concept>::T

template <typename VectorSpace_, typename Basis_>
struct DualOf_f<BasedVectorSpace_c<VectorSpace_,Basis_> >
{
    typedef BasedVectorSpace_c<typename DualOf_f<VectorSpace_>::T,typename DualOf_f<Basis_>::T> T;
private:
    DualOf_f();
};

// property IDs

struct Basis; // could this be a forward declaration?

// BaseProperty_f accessors

template <typename VectorSpace_, typename Basis_>
struct BaseProperty_f<BasedVectorSpace_c<VectorSpace_,Basis_>,Basis>
{
    typedef Basis_ T;
private:
    BaseProperty_f();
};

// named property accessors

template <typename Concept_> struct BasisOf_f
{
    typedef typename Property_f<Concept_,Basis>::T T;
private:
    BasisOf_f();
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_VECTORSPACE_HPP_
