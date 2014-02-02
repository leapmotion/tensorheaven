#include <iostream>

#include "tenh/list.hpp"
#include "tenh/implementation/directsum.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/implementation/vector.hpp"

using namespace Tenh;

int main (int argc, char **argv)
{
	List_t<Typle_t<>> empty_list;
	List_t<Typle_t<int>> int_list(1);
	List_t<Typle_t<int,float>> int_float_list(1, 3.0f);
	List_t<Typle_t<int,float,Uint32>> int_float_Uint32_list(1, 3.0f, 456);

	assert(empty_list.length() == 0);
	assert(int_list.length() == 1);
	assert(int_float_list.length() == 2);
	assert(int_float_Uint32_list.length() == 3);

	std::cout << FORMAT_VALUE(empty_list) << '\n';
	std::cout << FORMAT_VALUE(int_list) << '\n';
	std::cout << FORMAT_VALUE(int_float_list) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list) << '\n';

	std::cout << FORMAT_VALUE(empty_list | empty_list) << '\n';
	std::cout << FORMAT_VALUE(int_list | empty_list) << '\n';
	std::cout << FORMAT_VALUE(empty_list | int_list) << '\n';
	std::cout << FORMAT_VALUE(int_list | int_list) << '\n';
	std::cout << FORMAT_VALUE(int_list | int_float_list) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list | int_float_list) << '\n';

	std::cout << '\n';

	std::cout << FORMAT_VALUE(empty_list.body()) << '\n';
	std::cout << FORMAT_VALUE(int_list.head()) << '\n';
	std::cout << FORMAT_VALUE(int_list.body()) << '\n';
	std::cout << FORMAT_VALUE(int_float_list.head()) << '\n';
	std::cout << FORMAT_VALUE(int_float_list.body()) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list.head()) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list.body()) << '\n';

	std::cout << '\n';

	// std::cout << FORMAT_VALUE(empty_list.el<0>()) << '\n';

	std::cout << FORMAT_VALUE(int_list.el<0>()) << '\n';
	// std::cout << FORMAT_VALUE(int_list.el<1>()) << '\n';

	std::cout << FORMAT_VALUE(int_float_list.el<0>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_list.el<1>()) << '\n';
	// std::cout << FORMAT_VALUE(int_float_list.el<2>()) << '\n';

	std::cout << FORMAT_VALUE(int_float_Uint32_list.el<0>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list.el<1>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list.el<2>()) << '\n';
	// std::cout << FORMAT_VALUE(int_float_Uint32_list.el<3>()) << '\n';

	std::cout << '\n';

	std::cout << FORMAT_VALUE(empty_list.leading_list<0>()) << '\n';
	// std::cout << FORMAT_VALUE(empty_list.leading_list<1>()) << '\n';

	std::cout << FORMAT_VALUE(int_list.leading_list<0>()) << '\n';
	std::cout << FORMAT_VALUE(int_list.leading_list<1>()) << '\n';
	// std::cout << FORMAT_VALUE(int_list.leading_list<2>()) << '\n';

	std::cout << FORMAT_VALUE(int_float_list.leading_list<0>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_list.leading_list<1>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_list.leading_list<2>()) << '\n';
	// std::cout << FORMAT_VALUE(int_float_list.leading_list<3>()) << '\n';

	std::cout << FORMAT_VALUE(int_float_Uint32_list.leading_list<0>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list.leading_list<1>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list.leading_list<2>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list.leading_list<3>()) << '\n';
	// std::cout << FORMAT_VALUE(int_float_Uint32_list.leading_list<4>()) << '\n';

	std::cout << '\n';

	std::cout << FORMAT_VALUE(empty_list.trailing_list<0>()) << '\n';
	// std::cout << FORMAT_VALUE(empty_list.trailing_list<1>()) << '\n';

	std::cout << FORMAT_VALUE(int_list.trailing_list<0>()) << '\n';
	std::cout << FORMAT_VALUE(int_list.trailing_list<1>()) << '\n';
	// std::cout << FORMAT_VALUE(int_list.trailing_list<2>()) << '\n';

	std::cout << FORMAT_VALUE(int_float_list.trailing_list<0>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_list.trailing_list<1>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_list.trailing_list<2>()) << '\n';
	// std::cout << FORMAT_VALUE(int_float_list.trailing_list<3>()) << '\n';

	std::cout << FORMAT_VALUE(int_float_Uint32_list.trailing_list<0>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list.trailing_list<1>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list.trailing_list<2>()) << '\n';
	std::cout << FORMAT_VALUE(int_float_Uint32_list.trailing_list<3>()) << '\n';
	// std::cout << FORMAT_VALUE(int_float_Uint32_list.trailing_list<4>()) << '\n';

	std::cout << '\n';

	std::cout << FORMAT_VALUE((empty_list.range<0,0>())) << '\n';
	// std::cout << FORMAT_VALUE((empty_list.range<0,1>())) << '\n';
	// std::cout << FORMAT_VALUE((empty_list.range<1,1>())) << '\n';

	std::cout << FORMAT_VALUE((int_list.range<0,0>())) << '\n';
	std::cout << FORMAT_VALUE((int_list.range<0,1>())) << '\n';
	// std::cout << FORMAT_VALUE((int_list.range<0,2>())) << '\n';
	std::cout << FORMAT_VALUE((int_list.range<1,1>())) << '\n';
	// std::cout << FORMAT_VALUE((int_list.range<1,2>())) << '\n';
	// std::cout << FORMAT_VALUE((int_list.range<2,2>())) << '\n';

	std::cout << FORMAT_VALUE((int_float_list.range<0,0>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_list.range<0,1>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_list.range<0,2>())) << '\n';
	// std::cout << FORMAT_VALUE((int_float_list.range<0,3>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_list.range<1,1>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_list.range<1,2>())) << '\n';
	// std::cout << FORMAT_VALUE((int_float_list.range<1,3>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_list.range<2,2>())) << '\n';
	// std::cout << FORMAT_VALUE((int_float_list.range<2,3>())) << '\n';
	// std::cout << FORMAT_VALUE((int_float_list.range<3,3>())) << '\n';

	std::cout << FORMAT_VALUE((int_float_Uint32_list.range<0,0>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_Uint32_list.range<0,1>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_Uint32_list.range<0,2>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_Uint32_list.range<0,3>())) << '\n';
	// std::cout << FORMAT_VALUE((int_float_Uint32_list.range<0,4>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_Uint32_list.range<1,1>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_Uint32_list.range<1,2>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_Uint32_list.range<1,3>())) << '\n';
	// std::cout << FORMAT_VALUE((int_float_Uint32_list.range<1,4>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_Uint32_list.range<2,2>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_Uint32_list.range<2,3>())) << '\n';
	// std::cout << FORMAT_VALUE((int_float_Uint32_list.range<2,4>())) << '\n';
	std::cout << FORMAT_VALUE((int_float_Uint32_list.range<3,3>())) << '\n';
	// std::cout << FORMAT_VALUE((int_float_Uint32_list.range<3,4>())) << '\n';
	// std::cout << FORMAT_VALUE((int_float_Uint32_list.range<4,4>())) << '\n';

	std::cout << '\n';

	{
		auto t(tuple(3, 'a', "blah", true, 3.01, -123));
		std::cout << FORMAT_VALUE(type_string_of(t)) << '\n';
		std::cout << FORMAT_VALUE(t) << '\n';

		auto u(uniform_tuple<float>(1, 3.0, 4.0f));
		std::cout << FORMAT_VALUE(type_string_of(u)) << '\n';
		std::cout << FORMAT_VALUE(u) << '\n';

		std::cout << '\n';
	}

	{
		typedef decltype(bvs(generic_real_vs<3>(), generic_basis())) B;
		typedef double Scalar;
		typedef ImplementationOf_t<B,Scalar> V;
		typedef ImplementationOf_t<DualOf_f<B>::T,Scalar> VDual;

		V u(uniform_tuple<Scalar>(3, 5, 8));
		V v(fill_with(3));
		std::cout << FORMAT_VALUE(u) << '\n';
		std::cout << FORMAT_VALUE(v) << '\n';

		AbstractIndex_c<'i'> i;
		std::cout << FORMAT_VALUE(u(i)) << '\n';
		std::cout << FORMAT_VALUE(v(i)) << '\n';
		std::cout << FORMAT_VALUE(u(i) + v(i)) << '\n';

		VDual a(uniform_tuple<Scalar>(5, -1, 1));
		std::cout << FORMAT_VALUE(a) << '\n';
		std::cout << FORMAT_VALUE(a(i) * u(i)) << '\n';
		std::cout << FORMAT_VALUE(a(u)) << '\n';
		std::cout << FORMAT_VALUE(u(a)) << '\n';
		std::cout << FORMAT_VALUE(a(tuple(u))) << '\n';
		std::cout << FORMAT_VALUE(u(tuple(a))) << '\n';

		std::cout << '\n';
	}

	{
		decltype(bvs(generic_real_vs<2>(), generic_basis())) b2;
		decltype(bvs(generic_real_vs<3>(), generic_basis())) b3;
		decltype(bvs(generic_real_vs<4>(), generic_basis())) b4;
		typedef double Scalar;
		typedef ImplementationOf_t<decltype(b2*b3),Scalar> T23;
		typedef ImplementationOf_t<decltype(b2*b3*b4),Scalar> T234;

		T23 t23(uniform_tuple<Scalar>(1, 2, 3,
			                          4, 5, 6));
		std::cout << FORMAT_VALUE(type_string_of<T23::Concept>()) << '\n';
		std::cout << FORMAT_VALUE(t23) << '\n';
		std::cout << '\n';

		T23::Dual a23(uniform_tuple<Scalar>(5, -1, 3,
			                                4,  2, 1));
		std::cout << FORMAT_VALUE(a23) << '\n';
		std::cout << FORMAT_VALUE(a23(t23)) << '\n';
		AbstractIndex_c<'i'> i;
		AbstractIndex_c<'j'> j;
		std::cout << FORMAT_VALUE(a23(i)*t23(i)) << '\n';
		std::cout << FORMAT_VALUE(a23(i*j)*t23(i*j)) << '\n';

		typedef ImplementationOf_t<decltype(b2),Scalar> V2;
		typedef ImplementationOf_t<decltype(b3),Scalar> V3;
		V2 v2(uniform_tuple<Scalar>(4, 3));
		V3 v3(uniform_tuple<Scalar>(1, 0, 5));
		std::cout << FORMAT_VALUE(a23(i*j)*v2(i)*v3(j)) << '\n';
		std::cout << '\n';
	}

	{
		decltype(bvs(generic_real_vs<2>(), generic_basis())) b2;
		decltype(bvs(generic_real_vs<3>(), generic_basis())) b3;
		decltype(bvs(generic_real_vs<4>(), generic_basis())) b4;
		typedef double Scalar;
		typedef ImplementationOf_t<decltype(b2+b3),Scalar> S23;
		typedef ImplementationOf_t<decltype(b2+b3+b4),Scalar> S234;

		S23 s23(uniform_tuple<Scalar>(1, 2, 3, 4, 5));
		std::cout << FORMAT_VALUE(type_string_of<S23::Concept>()) << '\n';
		std::cout << FORMAT_VALUE(s23) << '\n';
		std::cout << '\n';

		S23::Dual a23(uniform_tuple<Scalar>(5, -1, 3, 4, 2));
		std::cout << FORMAT_VALUE(a23) << '\n';
		std::cout << FORMAT_VALUE(a23(s23)) << '\n';
		AbstractIndex_c<'i'> i;
		std::cout << FORMAT_VALUE(a23(i)*s23(i)) << '\n';

		std::cout << '\n';
	}

	return 0;
}
