// ///////////////////////////////////////////////////////////////////////////
// random.hpp by Ted Nitz, created 2013/04/04
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#if !defined(RANDOMIZE_HPP_)
#define RANDOMIZE_HPP_

#include <complex>
#include <algorithm>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "tenh/core.hpp"
#include "tenh/interface/vector.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2antisymmetric.hpp"
#include "tenh/tensor2diagonal.hpp"
#include "tenh/tensor2symmetric.hpp"
#include "tenh/vector.hpp"

namespace Tenh {
boost::random::mt19937 gen;

void randomize(Lvd::Sint8 & i, Lvd::Sint8 mean = 0, Lvd::Sint8 sigma = 10)
{
    boost::random::uniform_int_distribution<Lvd::Sint8> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize(Lvd::Sint16 & i, Lvd::Sint16 mean = 0, Lvd::Sint16 sigma = 10)
{
    boost::random::uniform_int_distribution<Lvd::Sint16> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize(Lvd::Sint32 & i, Lvd::Sint32 mean = 0, Lvd::Sint32 sigma = 10)
{
    boost::random::uniform_int_distribution<Lvd::Sint32> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize(Lvd::Sint64 & i, Lvd::Sint64 mean = 0, Lvd::Sint64 sigma = 10)
{
    boost::random::uniform_int_distribution<Lvd::Sint64> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize(float & f, float mean = 0, float sigma = 10)
{
    boost::random::normal_distribution<float> dist(mean, sigma);
    f = dist(gen);
}

void randomize(double & d, double mean = 0, double sigma = 10)
{
    boost::random::normal_distribution<double> dist(mean, sigma);
    d = dist(gen);
}

void randomize(long double & d, long double mean = 0, long double sigma = 10)
{
    boost::random::normal_distribution<long double> dist(mean, sigma);
    d = dist(gen);
}


template <typename T>
void randomize(std::complex<T> & z, std::complex<T> mean = 0, std::complex<T> sigma = 10)
{
    randomize(z.real(), mean.real(), abs(sigma));
    randomize(z.imag(), mean.imag(), abs(sigma));
}

template <typename Derived, typename Scalar, Uint32 DIM, typename Basis>
void randomize(Vector_i<Derived, Scalar, DIM, Basis> & v, Scalar mean = 0, Scalar sigma = 10)
{
    for(typename Vector_i<Derived, Scalar, DIM, Basis>::Index i; i.is_not_at_end(); ++i)
    {
        randomize((v[i]), mean, sigma);
    }
}


template <typename Factor1, typename Factor2>
void randomize(Tensor2_t<Factor1, Factor2> & t, Uint32 maximum_rank)
{
    maximum_rank = std::min(maximum_rank, std::min(Factor1::DIM, Factor2::DIM));
    typedef typename Factor1::Scalar Scalar;
    
    t = Tensor2_t<Factor1, Factor2>(0);
    
    Factor1 v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Factor2 w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    
    TypedIndex_t<Factor1,'i'> j;
    TypedIndex_t<Factor2,'j'> k;
    
    for (int i = 0; i < maximum_rank; ++i)
    {
        randomize(v, static_cast<Scalar>(0), static_cast<Scalar>(sqrt(10)));
        randomize(w, static_cast<Scalar>(0), static_cast<Scalar>(sqrt(10)));
        t(j|k) = (t(j|k) + v(j)*w(k)).eval();
    }
    t(j|k) = (t(j|k)/static_cast<Scalar>(maximum_rank)).eval();
}

template<typename Factor>
void randomize(Tensor2Antisymmetric_t<Factor> & s, Uint32 maximum_rank)
{
    maximum_rank = std::min(maximum_rank, Factor::DIM);
    typedef typename Factor::Scalar Scalar;
    
    Uint32 number_of_iterations = maximum_rank / 2;
    s = Tensor2Antisymmetric_t<Factor>(0);
    
    Factor v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Factor w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    
    TypedIndex_t<Factor,'i'> j;
    TypedIndex_t<Factor,'j'> k;
    TypedIndex_t<Tensor2Antisymmetric_t<Factor>,'p'> p;
    
    for (int i = 0; i < number_of_iterations; ++i)
    {
        randomize(v, static_cast<Scalar>(0), static_cast<Scalar>(sqrt(10)));
        randomize(w, static_cast<Scalar>(0), static_cast<Scalar>(sqrt(10)));
        s(p) = (s(j|k) + v(j)*w(k) - w(j)*v(k)).bundle(j|k,p).eval();
    }
    s(p) = (s(p)/static_cast<Scalar>(maximum_rank)).eval();

}

template<typename Factor>
void randomize(Tensor2Symmetric_t<Factor> & s, Uint32 maximum_rank)
{
    maximum_rank = std::min(maximum_rank, Factor::DIM);
    typedef typename Factor::Scalar Scalar;
    
    s = Tensor2Symmetric_t<Factor>(0);
    
    Factor v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    int sign;
    boost::random::uniform_int_distribution<int> dist(0, 1);
    
    TypedIndex_t<Factor,'i'> j;
    TypedIndex_t<Factor,'j'> k;
    TypedIndex_t<Tensor2Symmetric_t<Factor>,'p'> p;
    
    for (int i = 0; i < maximum_rank; ++i)
    {
        sign = dist(gen) ? 1 : -1;
        randomize(v, static_cast<Scalar>(0), static_cast<Scalar>(sqrt(10)));
        s(p) = (s(j|k) + sign*v(j)*v(k)).bundle(j|k,p).eval();
    }
    s(p) = (s(p)/static_cast<Scalar>(maximum_rank)).eval();
}

}

#endif // !defined(RANDOMIZE_HPP_)