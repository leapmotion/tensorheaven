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

namespace Tenh {

boost::random::mt19937 gen;

void randomize (Sint8 & i, Sint8 mean, Sint8 sigma)
{
    boost::random::uniform_int_distribution<Sint8> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize (Sint16 & i, Sint16 mean, Sint16 sigma)
{
    boost::random::uniform_int_distribution<Sint16> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize (Sint32 & i, Sint32 mean, Sint32 sigma)
{
    boost::random::uniform_int_distribution<Sint32> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
    i = dist(gen);
}

void randomize (Sint64 & i, Sint64 mean, Sint64 sigma)
{
    boost::random::uniform_int_distribution<Sint64> dist(static_cast<int>(mean - 2.5*sigma), static_cast<int>(mean + 2.5*sigma));
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
