#include <cmath>
#include <complex>
#include <iostream>
#include <string>

#include "tenh/core.hpp"

#include "tenh/meta/typestringof.hpp"

using namespace Tenh;

// evaluate the symbolic expression given the Scalar type
template <typename SymbolicExpression, typename Scalar> struct Evaluate_t;

template <typename Scalar, typename SymbolicExpression>
Scalar evaluate (SymbolicExpression const &)
{
	return Evaluate_t<SymbolicExpression,Scalar>::eval();
}

// terminal expressions which need template specialization to evaluate (e.g. for float)

template <typename Derived_>
struct SymbolicExpression_t
{
	typedef Derived_ Derived;
};

template <Sint64 VALUE_>
struct SInt_t : public SymbolicExpression_t<SInt_t<VALUE_> >
{
	static Sint64 const VALUE = VALUE_;
};

// TODO (?): make SFrac_t take some sort of abstracted integral values?
template <Sint64 NUMERATOR_, Uint64 DENOMINATOR_>
struct SFrac_t : public SymbolicExpression_t<SFrac_t<NUMERATOR_,DENOMINATOR_> >
{
	static Sint64 const NUMERATOR = NUMERATOR_;
	static Uint64 const DENOMINATOR = DENOMINATOR_;
};

template <Uint64 VALUE_>
struct UInt_t : public SymbolicExpression_t<UInt_t<VALUE_> >
{
	static Uint64 const VALUE = VALUE_;
};

// TODO (?): make UFrac_t take some sort of abstracted integral values?
template <Uint64 NUMERATOR_, Uint64 DENOMINATOR_>
struct UFrac_t : public SymbolicExpression_t<UFrac_t<NUMERATOR_,DENOMINATOR_> >
{
	static Uint64 const NUMERATOR = NUMERATOR_;
	static Uint64 const DENOMINATOR = DENOMINATOR_;
};

struct Pi : public SymbolicExpression_t<Pi> { };
struct E : public SymbolicExpression_t<E> { };
struct I : public SymbolicExpression_t<I> { };

template <Uint64 OPERAND_>
struct Factorial_t : public SymbolicExpression_t<Factorial_t<OPERAND_> >
{
	static Uint64 const OPERAND = OPERAND_;
};

// some important functions

// interpret the following OPERATION_ chars as the following operations:
// '+' : positive-sign (really does nothing)
// '-' : negation
template <typename Operand_, char OPERATION_>
struct UnaryOperation_t : public SymbolicExpression_t<UnaryOperation_t<Operand_,OPERATION_> >
{
	typedef Operand_ Operand;
	static char const OPERATION = OPERATION_;
};

// interpret the following OPERATION_ chars as the following operations:
// '+' : addition
// '-' : subtraction
// '*' : multiplication
// '/' : division
// '^' : expenentiation (the base comes first, then the exponent) (this is sort of ill-defined if you consider branches)
// 'L' : logarithm (the base comes first, then the argument) (this is sort of ill-defined if you consider branches)
template <typename LeftOperand_, typename RightOperand_, char OPERATION_>
struct BinaryOperation_t : public SymbolicExpression_t<BinaryOperation_t<LeftOperand_,RightOperand_,OPERATION_> >
{
	typedef LeftOperand_ LeftOperand;
	typedef RightOperand_ RightOperand;
	static char const OPERATION = OPERATION_;
};

// exponential function
template <typename Operand_>
struct Exp_t : public SymbolicExpression_t<Exp_t<Operand_> >
{
	typedef Operand_ Operand;
};

// natural logarithm (i.e. "ln")
template <typename Operand_>
struct Log_t : public SymbolicExpression_t<Log_t<Operand_> >
{
	typedef Operand_ Operand;
};

//
// operator overloads to facilitate construction of the expression types
//

template <typename Derived>
UnaryOperation_t<Derived,'+'> operator + (SymbolicExpression_t<Derived> const &)
{
	return UnaryOperation_t<Derived,'+'>();
}

template <typename Derived>
UnaryOperation_t<Derived,'-'> operator - (SymbolicExpression_t<Derived> const &)
{
	return UnaryOperation_t<Derived,'-'>();
}

template <Uint64 VALUE>
SInt_t<-VALUE> operator - (UInt_t<VALUE> const &)
{
	// can't negate if the value is too big -- alternatively could add "negative unsigned value"
	Lvd::Meta::Assert<(VALUE < (1ull << 63))>();
	return SInt_t<-VALUE>();
}

template <Uint64 NUMERATOR, Uint64 DENOMINATOR>
SFrac_t<-NUMERATOR,DENOMINATOR> operator - (UFrac_t<NUMERATOR,DENOMINATOR> const &)
{
	// can't negate if the value is too big -- alternatively could add "negative unsigned value"
	Lvd::Meta::Assert<(NUMERATOR < (1ull << 63))>();
	return SFrac_t<-NUMERATOR,DENOMINATOR>();
}

template <typename LeftDerived, typename RightDerived>
BinaryOperation_t<LeftDerived,RightDerived,'+'> operator + (SymbolicExpression_t<LeftDerived> const &, SymbolicExpression_t<RightDerived> const &)
{
	return BinaryOperation_t<LeftDerived,RightDerived,'+'>();
}

template <typename LeftDerived, typename RightDerived>
BinaryOperation_t<LeftDerived,RightDerived,'-'> operator - (SymbolicExpression_t<LeftDerived> const &, SymbolicExpression_t<RightDerived> const &)
{
	return BinaryOperation_t<LeftDerived,RightDerived,'-'>();
}

template <typename LeftDerived, typename RightDerived>
BinaryOperation_t<LeftDerived,RightDerived,'*'> operator * (SymbolicExpression_t<LeftDerived> const &, SymbolicExpression_t<RightDerived> const &)
{
	return BinaryOperation_t<LeftDerived,RightDerived,'*'>();
}

template <typename LeftDerived, typename RightDerived>
BinaryOperation_t<LeftDerived,RightDerived,'/'> operator / (SymbolicExpression_t<LeftDerived> const &, SymbolicExpression_t<RightDerived> const &)
{
	return BinaryOperation_t<LeftDerived,RightDerived,'/'>();
}

template <typename LeftDerived, typename RightDerived>
BinaryOperation_t<LeftDerived,RightDerived,'^'> pow (SymbolicExpression_t<LeftDerived> const &, SymbolicExpression_t<RightDerived> const &)
{
	return BinaryOperation_t<LeftDerived,RightDerived,'^'>();
}

template <typename LeftDerived, typename RightDerived>
BinaryOperation_t<LeftDerived,RightDerived,'L'> log (SymbolicExpression_t<LeftDerived> const &, SymbolicExpression_t<RightDerived> const &)
{
	return BinaryOperation_t<LeftDerived,RightDerived,'L'>();
}

template <typename Derived>
Exp_t<Derived> pow (E const &, SymbolicExpression_t<Derived> const &)
{
	return Exp_t<Derived>();
}

template <typename Derived>
Log_t<Derived> log (E const &, SymbolicExpression_t<Derived> const &)
{
	return Log_t<Derived>();
}

template <typename Derived>
Exp_t<Derived> exp (SymbolicExpression_t<Derived> const &)
{
	return Exp_t<Derived>();
}

template <typename Derived>
Log_t<Derived> log (SymbolicExpression_t<Derived> const &)
{
	return Log_t<Derived>();
}

//
// default implementations of the operations
//

template <Sint64 VALUE, typename Scalar>
struct Evaluate_t<SInt_t<VALUE>,Scalar>
{
	static Scalar eval () { return Scalar(VALUE); }
};

template <Sint64 NUMERATOR, Uint64 DENOMINATOR, typename Scalar>
struct Evaluate_t<SFrac_t<NUMERATOR,DENOMINATOR>,Scalar>
{
	static Scalar eval () { return Scalar(NUMERATOR) / Scalar(DENOMINATOR); }
};

template <Uint64 VALUE, typename Scalar>
struct Evaluate_t<UInt_t<VALUE>,Scalar>
{
	static Scalar eval () { return Scalar(VALUE); }
};

template <Uint64 NUMERATOR, Uint64 DENOMINATOR, typename Scalar>
struct Evaluate_t<UFrac_t<NUMERATOR,DENOMINATOR>,Scalar>
{
	static Scalar eval () { return Scalar(NUMERATOR) / Scalar(DENOMINATOR); }
};

// default implementations for Pi, E, and I are only given for certain types

template <>
struct Evaluate_t<Pi,float>
{
	static float eval () { return float(M_PI); }
};

template <>
struct Evaluate_t<Pi,double>
{
	static double eval () { return double(M_PI); }
};

template <>
struct Evaluate_t<Pi,long double>
{
	static long double eval () { return static_cast<long double>(M_PI); }
};

template <typename Scalar>
struct Evaluate_t<Pi,std::complex<Scalar> >
{
	static std::complex<Scalar> eval ()
	{
		return std::complex<Scalar>(Evaluate_t<Pi,Scalar>::eval(),
			                   Evaluate_t<UInt_t<0>,Scalar>::eval());
	}
};

template <>
struct Evaluate_t<E,float>
{
	static float eval () { return float(M_E); }
};

template <>
struct Evaluate_t<E,double>
{
	static double eval () { return double(M_E); }
};

template <>
struct Evaluate_t<E,long double>
{
	static long double eval () { return static_cast<long double>(M_E); }
};

template <typename Scalar>
struct Evaluate_t<E,std::complex<Scalar> >
{
	static std::complex<Scalar> eval ()
	{
		return std::complex<Scalar>(Evaluate_t<E,Scalar>::eval(),
			                        Evaluate_t<UInt_t<0>,Scalar>::eval());
	}
};

template <typename Scalar>
struct Evaluate_t<I,std::complex<Scalar> >
{
	static std::complex<Scalar> eval ()
	{
		return std::complex<Scalar>(Evaluate_t<UInt_t<0>,Scalar>::eval(),
			                        Evaluate_t<UInt_t<1>,Scalar>::eval());
	}
};

template <Uint64 OPERAND, typename Scalar>
struct Evaluate_t<Factorial_t<OPERAND>,Scalar>
{
	static Scalar eval () { return evaluate<Scalar>(UInt_t<OPERAND>() * Factorial_t<OPERAND-1>()); }
};

template <typename Scalar>
struct Evaluate_t<Factorial_t<0>,Scalar>
{
	static Scalar eval () { return evaluate<Scalar>(UInt_t<1>()); }
};

//
// default implementations for operations
//

template <typename Operand, typename Scalar>
struct Evaluate_t<UnaryOperation_t<Operand,'+'>,Scalar>
{
	static Scalar eval ()
	{
		return evaluate<Scalar>(Operand());//Evaluate_t<Operand,Scalar>::eval();
	}
};

template <typename Operand, typename Scalar>
struct Evaluate_t<UnaryOperation_t<Operand,'-'>,Scalar>
{
	static Scalar eval ()
	{
		return -evaluate<Scalar>(Operand());//-Evaluate_t<Operand,Scalar>::eval();
	}
};

template <typename LeftOperand, typename RightOperand, typename Scalar>
struct Evaluate_t<BinaryOperation_t<LeftOperand,RightOperand,'+'>,Scalar>
{
	static Scalar eval ()
	{
		return evaluate<Scalar>(LeftOperand()) + evaluate<Scalar>(RightOperand());
	}
};

template <typename LeftOperand, typename RightOperand, typename Scalar>
struct Evaluate_t<BinaryOperation_t<LeftOperand,RightOperand,'-'>,Scalar>
{
	static Scalar eval ()
	{
		return evaluate<Scalar>(LeftOperand()) - evaluate<Scalar>(RightOperand());
	}
};

template <typename LeftOperand, typename RightOperand, typename Scalar>
struct Evaluate_t<BinaryOperation_t<LeftOperand,RightOperand,'*'>,Scalar>
{
	static Scalar eval ()
	{
		return evaluate<Scalar>(LeftOperand()) * evaluate<Scalar>(RightOperand());
	}
};

template <typename LeftOperand, typename RightOperand, typename Scalar>
struct Evaluate_t<BinaryOperation_t<LeftOperand,RightOperand,'/'>,Scalar>
{
	static Scalar eval ()
	{
		return evaluate<Scalar>(LeftOperand()) / evaluate<Scalar>(RightOperand());
	}
};

template <typename LeftOperand, typename RightOperand>
struct Evaluate_t<BinaryOperation_t<LeftOperand,RightOperand,'^'>,float>
{
	static float eval ()
	{
		return powf(evaluate<float>(LeftOperand()), evaluate<float>(RightOperand()));
	}
};

template <typename LeftOperand, typename RightOperand>
struct Evaluate_t<BinaryOperation_t<LeftOperand,RightOperand,'^'>,double>
{
	static float eval ()
	{
		return pow(evaluate<double>(LeftOperand()), evaluate<double>(RightOperand()));
	}
};

template <typename LeftOperand, typename RightOperand>
struct Evaluate_t<BinaryOperation_t<LeftOperand,RightOperand,'^'>,long double>
{
	static float eval ()
	{
		return powl(evaluate<long double>(LeftOperand()), evaluate<long double>(RightOperand()));
	}
};

// TODO: think about complex exponentiation

template <typename LeftOperand, typename RightOperand>
struct Evaluate_t<BinaryOperation_t<LeftOperand,RightOperand,'L'>,float>
{
	static float eval ()
	{
		return logf(evaluate<float>(RightOperand())) / logf(evaluate<float>(LeftOperand()));
	}
};

template <typename LeftOperand, typename RightOperand>
struct Evaluate_t<BinaryOperation_t<LeftOperand,RightOperand,'L'>,double>
{
	static double eval ()
	{
		return log(evaluate<double>(RightOperand())) / log(evaluate<double>(LeftOperand()));
	}
};

template <typename LeftOperand, typename RightOperand>
struct Evaluate_t<BinaryOperation_t<LeftOperand,RightOperand,'L'>,long double>
{
	static long double eval ()
	{
		return logl(evaluate<long double>(RightOperand())) / logl(evaluate<long double>(LeftOperand()));
	}
};

// TODO: think about complex logarithm

template <typename RightOperand, typename Scalar>
struct Evaluate_t<BinaryOperation_t<E,RightOperand,'^'>,Scalar>
{
	static Scalar eval ()
	{
		return evaluate<Scalar>(Exp_t<RightOperand>());
	}
};

template <typename RightOperand, typename Scalar>
struct Evaluate_t<BinaryOperation_t<E,RightOperand,'L'>,Scalar>
{
	static Scalar eval ()
	{
		return evaluate<Scalar>(Log_t<RightOperand>());
	}
};

template <typename RightOperand>
struct Evaluate_t<Exp_t<RightOperand>,float>
{
	static float eval () { return expf(evaluate<float>(RightOperand())); }
};

template <typename RightOperand>
struct Evaluate_t<Exp_t<RightOperand>,double>
{
	static double eval () { return exp(evaluate<double>(RightOperand())); }
};

template <typename RightOperand>
struct Evaluate_t<Exp_t<RightOperand>,long double>
{
	static long double eval () { return expl(evaluate<long double>(RightOperand())); }
};

template <typename RightOperand>
struct Evaluate_t<Log_t<RightOperand>,float>
{
	static float eval () { return logf(evaluate<float>(RightOperand())); }
};

template <typename RightOperand>
struct Evaluate_t<Log_t<RightOperand>,double>
{
	static double eval () { return log(evaluate<double>(RightOperand())); }
};

template <typename RightOperand>
struct Evaluate_t<Log_t<RightOperand>,long double>
{
	static long double eval () { return logl(evaluate<long double>(RightOperand())); }
};

//
// main progrem code
//

using namespace std;

template <typename Scalar>
void test_constants ()
{
	cout << "running test_constants<" << TypeStringOf_t<Scalar>::eval() << ">() ----------------------------\n";
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(Pi())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(E())) << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(SInt_t<0>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(SInt_t<-1>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(SInt_t<-42>())) << '\n';
	cout << '\n';
	
	cout << FORMAT_VALUE(evaluate<Scalar>(UInt_t<0>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(UInt_t<1>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(UInt_t<42>())) << '\n';
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(SFrac_t<0,1>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(SFrac_t<-1,1>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(SFrac_t<-1,2>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(SFrac_t<-1,3>())) << '\n';
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(UFrac_t<0,1>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(UFrac_t<1,1>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(UFrac_t<1,2>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(UFrac_t<1,3>())) << '\n';
	cout << '\n' << '\n';
}

template <typename Scalar>
void test_complex_specific_constants ()
{
	cout << "running test_complex_specific_constants<" << TypeStringOf_t<Scalar>::eval() << ">() ----------------------------\n";
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(I())) << '\n';
	cout << '\n' << '\n';
}

template <typename Scalar>
void test_factorial ()
{
	cout << "running test_factorial<" << TypeStringOf_t<Scalar>::eval() << ">() ----------------------------\n";
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(Factorial_t<0>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(Factorial_t<1>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(Factorial_t<2>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(Factorial_t<3>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(Factorial_t<4>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(Factorial_t<5>())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(Factorial_t<10>())) << '\n';
	cout << '\n' << '\n';
}

template <typename Scalar>
void test_exp_log ()
{
	cout << "running test_exp_log<" << TypeStringOf_t<Scalar>::eval() << ">() ----------------------------\n";
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(exp(UInt_t<1>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(exp(UInt_t<2>()))) << '\n';
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(log(UInt_t<1>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(log(E()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(log(exp(UInt_t<2>())))) << '\n';
	cout << '\n' << '\n';
}

template <typename Scalar>
void test_pow_log ()
{
	cout << "running test_pow_log<" << TypeStringOf_t<Scalar>::eval() << ">() ----------------------------\n";
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(pow(UInt_t<3>(), UFrac_t<2,1>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(pow(UInt_t<3>(), UFrac_t<1,2>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(pow(UInt_t<3>(), SInt_t<0>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(pow(UInt_t<3>(), SInt_t<-1>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(pow(UInt_t<3>(), SInt_t<-2>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(pow(E(), SInt_t<-1>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(pow(E(), SInt_t<0>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(pow(E(), SInt_t<1>()))) << '\n';
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(log(UInt_t<3>(), UInt_t<9>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(log(UInt_t<3>(), UFrac_t<1,3>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(log(E(), SInt_t<1>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(log(E(), SInt_t<2>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(log(E(), E()))) << '\n';
	cout << '\n';

	cout << '\n' << '\n';
}

template <typename Scalar, typename Operand>
void test_unary_operations ()
{
	cout << "running test_unary_operations<" + TypeStringOf_t<Scalar>::eval() + ", ...>() ----------------------------\n";
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(+Operand())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(-Operand())) << '\n';
	cout << '\n';

	cout << '\n' << '\n';
}

template <typename Scalar, typename LeftOperand, typename RightOperand>
void test_binary_operations ()
{
	cout << "running test_binary_operations<" + TypeStringOf_t<Scalar>::eval() + ", ...>() ----------------------------\n";
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(LeftOperand() + RightOperand())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(LeftOperand() - RightOperand())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(LeftOperand() * RightOperand())) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(LeftOperand() / RightOperand())) << '\n';
	cout << '\n';

	cout << '\n' << '\n';
}

template <typename Scalar>
void test_fancy_expressions ()
{
	cout << "running test_fancy_expressions<" + TypeStringOf_t<Scalar>::eval() + ">() ----------------------------\n";
	cout << '\n';

	cout << FORMAT_VALUE(evaluate<Scalar>(pow(UInt_t<1>() / Factorial_t<2>(), UFrac_t<1,2>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(pow(UInt_t<1>() / Factorial_t<3>(), UFrac_t<1,2>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(pow(UInt_t<1>() / Factorial_t<4>(), UFrac_t<1,2>()))) << '\n';
	cout << FORMAT_VALUE(evaluate<Scalar>(pow(UInt_t<1>() / Factorial_t<5>(), UFrac_t<1,2>()))) << '\n';
	cout << '\n' << '\n';
}

int main (int argc, char **argv)
{
	test_constants<float>();
	test_constants<double>();
	test_constants<long double>();
	test_constants<std::complex<float> >();
	test_constants<std::complex<double> >();
	test_constants<std::complex<long double> >();

	test_complex_specific_constants<std::complex<float> >();
	test_complex_specific_constants<std::complex<double> >();
	test_complex_specific_constants<std::complex<long double> >();

	test_factorial<Uint64>();
	test_factorial<float>();
	test_factorial<double>();
	test_factorial<long double>();
	test_factorial<std::complex<Uint32> >();
	test_factorial<std::complex<float> >();

	test_exp_log<float>();
	test_exp_log<double>();
	test_exp_log<long double>();

	test_pow_log<float>();
	test_pow_log<double>();
	test_pow_log<long double>();

	test_unary_operations<float,E>();
	test_unary_operations<double,E>();
	test_unary_operations<long double,E>();
	test_unary_operations<complex<float>,E>();
	test_unary_operations<complex<double>,E>();
	test_unary_operations<complex<long double>,E>();

	test_binary_operations<float,E,Pi>();
	test_binary_operations<double,E,Pi>();
	test_binary_operations<long double,E,Pi>();
	test_binary_operations<complex<float>,E,Pi>();
	test_binary_operations<complex<double>,E,Pi>();
	test_binary_operations<complex<long double>,E,Pi>();
	test_binary_operations<complex<float>,E,I>();
	test_binary_operations<complex<double>,E,I>();
	test_binary_operations<complex<long double>,E,I>();
	test_binary_operations<complex<float>,I,I>();
	test_binary_operations<complex<double>,I,I>();
	test_binary_operations<complex<long double>,I,I>();

	test_fancy_expressions<float>();
	test_fancy_expressions<double>();
	test_fancy_expressions<long double>();

	return 0;
}
