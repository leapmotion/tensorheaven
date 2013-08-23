// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/vectorspace.hpp by Victor Dods, created 2013/07/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_VECTORSPACE_HPP_
#define TENH_CONCEPTUAL_VECTORSPACE_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"
#include "tenh/conceptual/dual.hpp"
#include "tenh/conceptual/field.hpp"

namespace Tenh {

template <typename Field_, Uint32 DIM_, typename Id_>
struct VectorSpace_c
{
    typedef EmptyTypeList ParentTypeList;

    enum { STATIC_ASSERT_IN_ENUM(IS_FIELD_UNIQUELY(Field_), MUST_BE_FIELD) };

    typedef typename AS_FIELD(Field_) Field;
    static Uint32 const DIM = DIM_;
    typedef Id_ Id;

    static std::string type_as_string ()
    {
        return "VectorSpace_c<" + TypeStringOf_t<Field_>::eval() + ','
                                + AS_STRING(DIM_) + ',' + TypeStringOf_t<Id_>::eval() + '>';
    }
};

template <typename Field_, Uint32 DIM_, typename Id_>
struct IsConcept_f<VectorSpace_c<Field_, DIM_, Id_> >
{ static bool const V = true; };

template <typename T> struct IsVectorSpace_f { static bool const V = false; };
template <typename Field, Uint32 DIM, typename Id> struct IsVectorSpace_f<VectorSpace_c<Field,DIM,Id> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(VectorSpace);
// special convenience macros
#define IS_VECTOR_SPACE_UNIQUELY(Concept) HasUniqueVectorSpaceStructure_f<Concept>::V
#define AS_VECTOR_SPACE(Concept) UniqueVectorSpaceStructureOf_f<Concept>::T

template <typename Field, Uint32 DIM, typename Id>
struct DualOf_f<VectorSpace_c<Field,DIM,Id> >
{
    typedef VectorSpace_c<Field,DIM,typename DualOf_f<Id>::T> T;
};


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

    typedef typename AS_VECTOR_SPACE(VectorSpace_) As_VectorSpace;
public:
    typedef TypeList_t<As_VectorSpace> ParentTypeList;


    typedef typename As_VectorSpace::Field Field;
    static Uint32 const DIM = As_VectorSpace::DIM;
    typedef typename As_VectorSpace::Id Id;
    typedef typename AS_BASIS(Basis_) Basis;

    static std::string type_as_string ()
    {
        return "BasedVectorSpace_c<" + TypeStringOf_t<VectorSpace_>::eval() + ',' + TypeStringOf_t<Basis_>::eval() + '>';
    }
};

template <typename VectorSpace_, typename Basis_>
struct IsConcept_f<BasedVectorSpace_c<VectorSpace_, Basis_> >
{ static bool const V = true; };

template <typename T> struct IsBasedVectorSpace_f { static bool const V = false; };
template <typename VectorSpace, typename Basis> struct IsBasedVectorSpace_f<BasedVectorSpace_c<VectorSpace,Basis> > { static bool const V = true; };

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(BasedVectorSpace);
// special convenience macros
#define IS_BASED_VECTOR_SPACE_UNIQUELY(Concept) HasUniqueBasedVectorSpaceStructure_f<Concept>::V
#define AS_BASED_VECTOR_SPACE(Concept) UniqueBasedVectorSpaceStructureOf_f<Concept>::T

template <typename VectorSpace, typename Basis>
struct DualOf_f<BasedVectorSpace_c<VectorSpace,Basis> >
{
    typedef BasedVectorSpace_c<typename DualOf_f<VectorSpace>::T,typename DualOf_f<Basis>::T> T;
};

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_VECTORSPACE_HPP_
