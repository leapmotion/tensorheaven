#include "adhoc_test.hpp"

// ///////////////////////////////////////////////////////////////////////////////////////////
// 6
// ///////////////////////////////////////////////////////////////////////////////////////////

void test_pretty_typestringof ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedVectorSpace;
    typedef TensorPowerOfBasedVectorSpace_f<4,BasedVectorSpace>::T TensorPower;
    std::cout << '\n' << Pretty<TypeStringOf_t<TensorPower> >() << '\n';
    std::cout << '\n' << Pretty<TypeStringOf_t<TensorPower>,0>() << '\n';
    std::cout << '\n' << Pretty<TypeStringOf_t<TensorPower>,1>() << '\n';
    std::cout << '\n' << Pretty<TypeStringOf_t<TensorPower>,2>() << '\n';
    std::cout << '\n' << Pretty<TypeStringOf_t<TensorPower>,3>() << '\n';
    std::cout << '\n';
    std::cout << "shortify_depth = 0\n";
    std::cout << "OSTRICH 0 "; print_pretty_typestring(std::cout, "Blah<x", 0);          std::cout << '\n' << '\n';
    std::cout << "OSTRICH 1 "; print_pretty_typestring(std::cout, "Blah<", 0);           std::cout << '\n' << '\n';
    std::cout << "OSTRICH 2 "; print_pretty_typestring(std::cout, "Blah<x>", 0);         std::cout << '\n' << '\n';
    std::cout << "OSTRICH 3 "; print_pretty_typestring(std::cout, "Blah<x>x>", 0);       std::cout << '\n' << '\n';
    std::cout << '\n';
    std::cout << "shortify_depth = 1\n";
    std::cout << "OSTRICH 0 "; print_pretty_typestring(std::cout, "Blah<x", 1);          std::cout << '\n' << '\n';
    std::cout << "OSTRICH 1 "; print_pretty_typestring(std::cout, "Blah<", 1);           std::cout << '\n' << '\n';
    std::cout << "OSTRICH 2 "; print_pretty_typestring(std::cout, "Blah<x>", 1);         std::cout << '\n' << '\n';
    std::cout << "OSTRICH 3 "; print_pretty_typestring(std::cout, "Blah<x>x>", 1);       std::cout << '\n' << '\n';
    std::cout << '\n';
    std::cout << "shortify_depth = 2\n";
    std::cout << "OSTRICH 0 "; print_pretty_typestring(std::cout, "Blah<x", 2);          std::cout << '\n' << '\n';
    std::cout << "OSTRICH 1 "; print_pretty_typestring(std::cout, "Blah<", 2);           std::cout << '\n' << '\n';
    std::cout << "OSTRICH 2 "; print_pretty_typestring(std::cout, "Blah<x>", 2);         std::cout << '\n' << '\n';
    std::cout << "OSTRICH 3 "; print_pretty_typestring(std::cout, "Blah<x>x>", 2);       std::cout << '\n' << '\n';
    std::cout << '\n';
}

void test_conceptual_inheritance_graph ()
{
    std::cout << "testing conceptual inheritance graph stuff\n";
    Graph g;
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedVectorSpace;
    typedef TensorPowerOfBasedVectorSpace_f<4,BasedVectorSpace>::T TensorPower;
    add_concept_hierarchy_to_graph<0>(TensorPower(), g);
    g.print_as_dot_graph(std::cout);
    std::cout << '\n';
}

void test_conceptual_property_accessors ()
{
    typedef BasedVectorSpace_c<VectorSpace_c<RealField,3,X>,Basis_c<X> > BasedVectorSpace;
    std::cout << FORMAT_VALUE(DimensionOf_f<BasedVectorSpace>::V) << '\n';
    std::cout << FORMAT_VALUE(type_string_of<IdOf_f<BasedVectorSpace>::T>()) << '\n';
    std::cout << FORMAT_VALUE(type_string_of<ScalarFieldOf_f<BasedVectorSpace>::T>()) << '\n';
    std::cout << FORMAT_VALUE(type_string_of<BasisOf_f<BasedVectorSpace>::T>()) << '\n';
}

void test_sym_0 ()
{
    test_sym<float,1,2>();
    test_sym<float,2,2>();
    test_sym<float,3,2>();
    test_sym<float,1,3>();
    test_sym<float,2,3>();
    test_sym<float,3,3>();
}

void test_alt_0 ()
{
    test_alt<float,1,1>();
    test_alt<float,1,2>();
    test_alt<float,2,2>();
    test_alt<float,3,2>();
    test_alt<float,1,3>();
    test_alt<float,2,3>();
    test_alt<float,3,3>();
    test_alt<float,4,3>();
}

