#include "conceptual_prototype.hpp"

int main (int, char **)
{
    test_Basis_c();
    test_VectorSpace_c();
    test_BasedVectorSpace_c();
    test_TensorProduct_c();
    test_TensorProductOfVectorSpaces_c();
    test_TensorProductOfBases_c();
    test_BasedTensorProductStuff();

    test_SymmetricPower_c();
    test_SymmetricPowerOfVectorSpace_c();
    test_SymmetricPowerOfBasis_c();
    test_BasedSymmetricPowerStuff();
    test_vector_contraction();
    test_tensor_contraction();

    test_wedge();
    test_vee();
    test_multiindex_sort();
    test_multiindex_sort_2();
    test_zero_dimensional_vector_space();
    test_Diagonal2Tensor();
    test_tensor_printing_0<0>();
    test_tensor_printing_0<1>();
    test_tensor_printing_0<3>();

    test_vector_power_thingy();
    test_a_bunch_of_stuff();
    test_immutable_stuff();
    test_inner_product_and_euclidean_stuff();
    test_products_of_immutable_stuff();
    test_pretty_typestringof();
    test_conceptual_inheritance_graph();
    test_conceptual_property_accessors();
    test_sym_0();
    test_alt_0();
    test_eigen_map_of_2_tensor();
    test_split_index_to_index_order_1();
    test_split_index_to_index_order_2();
    test_split_index_to_index_order_3();
    test_poly_in_0_dim();
    test_poly_in_1_dim();
    test_poly_in_2_dim();
    test_poly_in_3_dim();
    test_poly_in_4_dim();
    test_polynomial_serialization();
    test_polynomial_multiplication();
    test_tuple_initialization();

    test_induced_inner_product();
    test_direct_sums();

    test_zero_vector();
    test_basis_vectors();
    // test_eval_value();
    test_direct_sum_of_2tensors();

    return 0;
}
