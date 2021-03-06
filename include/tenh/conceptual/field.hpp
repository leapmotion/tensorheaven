// ///////////////////////////////////////////////////////////////////////////
// tenh/conceptual/field.hpp by Victor Dods, created 2013/07/21
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_CONCEPTUAL_FIELD_HPP_
#define TENH_CONCEPTUAL_FIELD_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/concept.hpp"

namespace Tenh {

// TODO: can Field_c be deprecated, in favor of just using IsAField_c with custom typedefs?
template <typename Id_>
struct Field_c
{
    typedef Typle_t<> ParentTyple;

	typedef Id_ Id;

    static std::string type_as_string (bool verbose)
    {
        if (verbose)
            return "Field_c<" + type_string_of<Id_>() + '>';
        else
            return terse_string_of<Id_>();
    }
};

template <typename Id_>
struct IsConcept_f<Field_c<Id_>>
{
    static bool const V = true;
private:
    IsConcept_f();
};

template <typename T> struct IsField_f
{
    static bool const V = false;
private:
};
template <typename Id> struct IsField_f<Field_c<Id>>
{
    static bool const V = true;
private:
    IsField_f();
};

DEFINE_CONCEPTUAL_STRUCTURE_METAFUNCTIONS(Field);
// special convenience macros
#define IS_FIELD_UNIQUELY(Concept) HasUniqueFieldStructure_f<Concept>::V
#define AS_FIELD(Concept) UniqueFieldStructureOf_f<Concept>::T

// // used for checking if a given C++ type is an acceptable representation for the field
// template <typename ScalarRepresentation, typename Field> struct IsAValidScalarRepresentationForField { static bool const V = false; };

// TODO: some way of specifying associated representation (e.g. double, complex<double>, bool)

struct RealNumbers
{
    static std::string type_as_string (bool verbose)
    {
        return verbose ? "RealNumbers" : "R";
    }
private:
    RealNumbers();
};

typedef Field_c<RealNumbers> RealField;
// template <> struct IsAValidScalarRepresentationForField<float,RealField> { static bool const V = true; };
// template <> struct IsAValidScalarRepresentationForField<double,RealField> { static bool const V = true; };
// // could add one for some arbitrary-precision floating point representation

// TODO: Complex_c, F2_c, etc.
// TODO: maybe quaternion (skew-field)?
// TODO: Should ComplexField have a real VectorSpace structure?

// ///////////////////////////////////////////////////////////////////////////
// helper functions for easing use of the type system
// ///////////////////////////////////////////////////////////////////////////

// for now, just do tensor product of based vector spaces
template <typename Id_>
Field_c<Id_> field (Id_ const &)
{
    return Field_c<Id_>();
}

} // end of namespace Tenh

#endif // TENH_CONCEPTUAL_FIELD_HPP_
