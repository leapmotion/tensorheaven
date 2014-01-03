// ///////////////////////////////////////////////////////////////////////////
// randomize.hpp by Ted Nitz, created 2013/04/04
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(RANDOMIZE_HPP_)
#define RANDOMIZE_HPP_

// TODO: figure out which ones should be in the header and which ones in the cpp

#include <complex>
#include <algorithm>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "tenh/core.hpp"

namespace Tenh {

extern boost::random::mt19937 gen;

void randomize (Sint8 &i, Sint8 mean = 0, Sint8 sigma = 10);
void randomize (Sint16 &i, Sint16 mean = 0, Sint16 sigma = 10);
void randomize (Sint32 &i, Sint32 mean = 0, Sint32 sigma = 10);
void randomize (Sint64 &i, Sint64 mean = 0, Sint64 sigma = 10);
void randomize (float &f, float mean = 0, float sigma = 10);
void randomize (double &d, double mean = 0, double sigma = 10);
void randomize (long double &d, long double mean = 0, long double sigma = 10);

template <typename T>
void randomize (std::complex<T> &z, std::complex<T> mean = 0, std::complex<T> sigma = 10)
{
	T temp;
    randomize(temp, mean.real(), abs(sigma));
	z.real(temp);
    randomize(temp, mean.imag(), abs(sigma));
	z.imag(temp);

}

}

#endif // !defined(RANDOMIZE_HPP_)