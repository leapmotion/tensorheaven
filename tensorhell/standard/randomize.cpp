// ///////////////////////////////////////////////////////////////////////////
// randomize.hpp by Ted Nitz, created 2013/04/19
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "randomize.hpp"

// TODO: figure out which ones should be in the header and which ones in the cpp

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

void randomize (Lvd::Sint8 & i, Lvd::Sint8 mean, Lvd::Sint8 sigma)
{
    boost::random::uniform_int_distribution<Lvd::Sint8> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize (Lvd::Sint16 & i, Lvd::Sint16 mean, Lvd::Sint16 sigma)
{
    boost::random::uniform_int_distribution<Lvd::Sint16> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize (Lvd::Sint32 & i, Lvd::Sint32 mean, Lvd::Sint32 sigma)
{
    boost::random::uniform_int_distribution<Lvd::Sint32> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize (Lvd::Sint64 & i, Lvd::Sint64 mean, Lvd::Sint64 sigma)
{
    boost::random::uniform_int_distribution<Lvd::Sint64> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize (float & f, float mean, float sigma)
{
    boost::random::normal_distribution<float> dist(mean, sigma);
    f = dist(gen);
}

void randomize (double & d, double mean, double sigma)
{
    boost::random::normal_distribution<double> dist(mean, sigma);
    d = dist(gen);
}

void randomize (long double & d, long double mean, long double sigma)
{
    boost::random::normal_distribution<long double> dist(mean, sigma);
    d = dist(gen);
}

}
