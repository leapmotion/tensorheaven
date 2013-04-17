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
#include "tenh/tensor2.hpp"
#include "tenh/tensor2antisymmetric.hpp"
#include "tenh/tensor2diagonal.hpp"
#include "tenh/tensor2symmetric.hpp"
#include "tenh/vector.hpp"

namespace Tenh {
boost::random::mt19937 gen;

void randomize(int & i, int mean = 0, int sigma = 10)
{
    boost::random::uniform_int_distribution<int> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize(char & i, char mean = 0, char sigma = 10)
{
    boost::random::uniform_int_distribution<char> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize(long & i, long mean = 0, long sigma = 10)
{
    boost::random::uniform_int_distribution<long> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
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

template <typename T> // This might not work
void randomize(std::complex<T> & z, T mean = 0, T sigma = 10)
{
    randomize(z.real(), mean, sigma);
    randomize(z.imag(), mean, sigma);
}

template <typename Scalar, Uint32 DIM>
void randomize(Vector_t<Scalar, DIM> & v, Scalar mean = 0, Scalar sigma = 10)
{
    for(typename Vector_t<Scalar, DIM>::Index i; i.is_not_at_end(); ++i)
    {
        randomize(v[i], mean, sigma);
    }
}

template <typename Factor1, typename Factor2>
void randomize(Tensor2_t<Factor1, Factor2> & t)
{
    for (typename Tensor2_t<Factor1, Factor2>::Index i; i.is_not_at_end(); ++i)
    {
        randomize(t[i]);
    }
}

template <typename Factor1, typename Factor2>
void randomize(Tensor2_t<Factor1, Factor2> & t, Uint32 maximum_rank)
{
    // maximum_rank = std::min(maximum_rank, std::min(Factor1::DIM, Factor2::DIM));
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
void randomize(Tensor2Antisymmetric_t<Factor> & s)
{
    Tensor2_t<Factor, Factor> t(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    randomize(t);
    TypedIndex_t<Factor,'i'> i;
    TypedIndex_t<Factor,'j'> j;
    TypedIndex_t<Tensor2Antisymmetric_t<Factor>, 'q'> q;
    s(q) = ((t(i|j) - t(j|i))/2).bundle(i|j,q);
}

template<typename Factor>
void randomize(Tensor2Antisymmetric_t<Factor> & s, Uint32 maximum_rank)
{
    // maximum_rank = std::min(maximum_rank, std::min(Factor1::DIM, Factor2::DIM));
    typedef typename Factor::Scalar Scalar;
    
    Uint32 looping = maximum_rank / 2;
    s = Tensor2Antisymmetric_t<Factor>(0);
    
    Factor v(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    Factor w(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    
    TypedIndex_t<Factor,'i'> j;
    TypedIndex_t<Factor,'j'> k;
    TypedIndex_t<Tensor2Antisymmetric_t<Factor>,'p'> p;
    
    for (int i = 0; i < looping; ++i)
    {
        randomize(v, static_cast<Scalar>(0), static_cast<Scalar>(sqrt(10)));
        randomize(w, static_cast<Scalar>(0), static_cast<Scalar>(sqrt(10)));
        s(p) = (s(j|k) + v(j)*w(k) - w(j)*v(k)).bundle(j|k,p).eval();
    }
    s(p) = (s(p)/static_cast<Scalar>(maximum_rank)).eval();

}

template<typename Factor>
void randomize(Tensor2Symmetric_t<Factor> & s)
{
    Tensor2_t<Factor, Factor> t(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
    randomize(t);
    TypedIndex_t<Factor,'i'> i;
    TypedIndex_t<Factor,'j'> j;
    TypedIndex_t<Tensor2Symmetric_t<Factor>, 'q'> q;
    s(q) = ((t(i|j) + t(j|i))/2).bundle(i|j,q);
}

template<typename Factor>
void randomize(Tensor2Symmetric_t<Factor> & s, Uint32 maximum_rank)
{
    // maximum_rank = std::min(maximum_rank, std::min(Factor1::DIM, Factor2::DIM));
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