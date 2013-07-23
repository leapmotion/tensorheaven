#include <iostream>
#include "tenh/conceptual/basis.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"

using namespace Tenh;

struct X
{
	static std::string type_as_string () { return "X"; }
};

int main (int argc, char **argv)
{
	typedef Basis_c<X> Basis;
	typedef Basis::Dual DualBasis;
	typedef DualBasis::Dual DualDualBasis;
	std::cout << "Basis = "  << TypeStringOf_t<Basis>::eval() 
	          << ", DualBasis = " << TypeStringOf_t<DualBasis>::eval() 
	          << ", DualOf_c<Basis>::T = " << TypeStringOf_t<DualOf_c<Basis>::T>::eval()
	          << ", DualDualBasis = " << TypeStringOf_t<DualDualBasis>::eval() << '\n' << '\n';

	typedef VectorSpace_c<RealField,3,X> VectorSpace;
	typedef VectorSpace::Dual DualVectorSpace;
	typedef DualVectorSpace::Dual DualDualVectorSpace;
	std::cout << "VectorSpace = "  << TypeStringOf_t<VectorSpace>::eval() 
	          << ", DualVectorSpace = " << TypeStringOf_t<DualVectorSpace>::eval() 
	          << ", DualOf_c<VectorSpace>::T = " << TypeStringOf_t<DualOf_c<VectorSpace>::T>::eval()
	          << ", DualDualVectorSpace = " << TypeStringOf_t<DualDualVectorSpace>::eval() << '\n' << '\n';

	typedef TypeList_t<X,TypeList_t<int,TypeList_t<float> > > TypeList;
	typedef TensorProduct_c<TypeList> TensorProduct;
	typedef TensorProduct::Dual DualTensorProduct;
	typedef DualTensorProduct::Dual DualDualTensorProduct;
	std::cout << "TensorProduct = "  << TypeStringOf_t<TensorProduct>::eval() 
	          << ", DualTensorProduct = " << TypeStringOf_t<DualTensorProduct>::eval() 
	          << ", DualOf_c<TensorProduct>::T = " << TypeStringOf_t<DualOf_c<TensorProduct>::T>::eval()
	          << ", DualDualTensorProduct = " << TypeStringOf_t<DualDualTensorProduct>::eval() << '\n' << '\n';

	return 0;
}
