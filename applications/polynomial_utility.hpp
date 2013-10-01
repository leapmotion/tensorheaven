// ///////////////////////////////////////////////////////////////////////////
// applications/polynomial_util.hpp by Ted Nitz, created 2013/09/27
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#include "applications/polynomial.hpp"
#include "applications/homogeneouspolynomial.hpp"

#include <iostream>


#ifndef APPLICATIONS_POLYNOMIAL_UTIL_HPP_
#define APPLICATIONS_POLYNOMIAL_UTIL_HPP_


//    operator<< for ostream
template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar>
struct VariableStringComputer
{
    static std::string compute(typename HomogeneousPolynomial<DEG,DIM,Id,Scalar>::Sym::MultiIndex const &n)
    {
        typedef typename HomogeneousPolynomial<DEG,DIM,Id,Scalar>::Sym::MultiIndex MultiIndex;
        MultiIndex m = Tenh::sorted<typename MultiIndex::IndexTypeList,std::less<Uint32> >(n);
        Uint32 last = m.value_of_index(0, Tenh::DONT_CHECK_RANGE);
        Uint32 count = 1;
        std::string result = "x" + AS_STRING(last);
        for (Uint32 i = 1; i < MultiIndex::LENGTH; ++i)
        {
            if (m.value_of_index(i, Tenh::DONT_CHECK_RANGE) == last)
            {
                ++count;
            }
            else
            {
                last = m.value_of_index(i, Tenh::DONT_CHECK_RANGE);
                if (count != 1)
                {
                    result += "^" + AS_STRING(count);
                }
                result += "*x" + AS_STRING(last);
                count = 1;
            }
        }
        if (count != 1)
        {
            result += "^" + AS_STRING(count);
        }

        return result;
    }
};

template<Uint32 DEG, typename Id, typename Scalar>
struct VariableStringComputer<DEG,0,Id,Scalar>
{
    static std::string compute(typename MultivariatePolynomial<DEG,0,Id,Scalar>::Sym::MultiIndex const &n)
    {
        return "";
    }
};


template<Uint32 DEG, typename Id, typename Scalar>
struct VariableStringComputer<DEG,1,Id,Scalar>
{
    static std::string compute(typename MultivariatePolynomial<DEG,1,Id,Scalar>::Sym::MultiIndex const &n)
    {
        typedef typename MultivariatePolynomial<DEG,1,Id,Scalar>::Sym::MultiIndex MultiIndex;
        return "x" + ((MultiIndex::LENGTH > 1) ? "^" + AS_STRING(MultiIndex::LENGTH) : "");
    }
};

template<Uint32 DEG, typename Id, typename Scalar>
struct VariableStringComputer<DEG,2,Id,Scalar>
{
    static std::string compute(typename MultivariatePolynomial<DEG,2,Id,Scalar>::Sym::MultiIndex const &n)
    {
        typedef typename MultivariatePolynomial<DEG,2,Id,Scalar>::Sym::MultiIndex MultiIndex;
        MultiIndex m = Tenh::sorted<typename MultiIndex::IndexTypeList,std::less<Uint32> >(n);
        char vars[3] = "xy";
        Uint32 last = m.value_of_index(0, Tenh::DONT_CHECK_RANGE);
        Uint32 count = 1;
        std::string result;
        result.push_back(vars[last]);
        for (Uint32 i = 1; i < MultiIndex::LENGTH; ++i)
        {
            if (m.value_of_index(i, Tenh::DONT_CHECK_RANGE) == last)
            {
                ++count;
            }
            else
            {
                last = m.value_of_index(i, Tenh::DONT_CHECK_RANGE);
                if (count != 1)
                {
                    result += "^" + AS_STRING(count);
                }
                result += "*";
                result.push_back(vars[last]);
                count = 1;
            }
        }
        if (count != 1)
        {
            result += "^" + AS_STRING(count);
        }

        return result;
    }
};

template<Uint32 DEG, typename Id, typename Scalar>
struct VariableStringComputer<DEG,3,Id,Scalar>
{
    static std::string compute(typename MultivariatePolynomial<DEG,3,Id,Scalar>::Sym::MultiIndex const &n)
    {
        typedef typename MultivariatePolynomial<DEG,3,Id,Scalar>::Sym::MultiIndex MultiIndex;
        MultiIndex m = Tenh::sorted<typename MultiIndex::IndexTypeList,std::less<Uint32> >(n);
        char vars[4] = "xyz";
        Uint32 last = m.value_of_index(0, Tenh::DONT_CHECK_RANGE);
        Uint32 count = 1;
        std::string result;
        result.push_back(vars[last]);
        for (Uint32 i = 1; i < MultiIndex::LENGTH; ++i)
        {
            if (m.value_of_index(i, Tenh::DONT_CHECK_RANGE) == last)
            {
                ++count;
            }
            else
            {
                last = m.value_of_index(i, Tenh::DONT_CHECK_RANGE);
                if (count != 1)
                {
                    result += "^" + AS_STRING(count);
                }
                result += "*";
                result.push_back(vars[last]);
                count = 1;
            }

        }
        if (count != 1)
        {
            result += "^" + AS_STRING(count);
        }

        return result;
    }
};

template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar>
std::ostream &operator << (std::ostream &out, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &poly)
{
    typedef MultivariatePolynomial<DEG,DIM,Id,Scalar> PolyType;
    typedef typename PolyType::Sym::ComponentIndex ComponentIndex;
    typedef typename PolyType::Sym::MultiIndex MultiIndex;
    for (ComponentIndex it; it.is_not_at_end(); ++it)
    {
        MultiIndex m = PolyType::Sym::template bundle_index_map<typename MultiIndex::IndexTypeList, ComponentIndex>(it);
        Scalar coeff = poly.m_term.Coefficients()[it] * Tenh::Factorial_t<DEG>::V / (Tenh::MultiIndexMultiplicity_t<MultiIndex>::eval(m));
        if (coeff == Scalar(1))
        {
            out << VariableStringComputer<DEG,DIM,Id,Scalar>::compute(m)
                << " + ";
        }
        else if (coeff != Scalar(0))
        {
            out << coeff
                << "*"
                <<  VariableStringComputer<DEG,DIM,Id,Scalar>::compute(m)
                << " + ";
        }
    }

    return out << poly.m_body;
}

template<Uint32 DIM, typename Id, typename Scalar>
std::ostream &operator << (std::ostream &out, MultivariatePolynomial<0,DIM,Id,Scalar> const &m)
{
    return out << m.m_term;
}

#endif // APPLICATIONS_POLYNOMIAL_UTIL_HPP_