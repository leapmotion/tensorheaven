// ///////////////////////////////////////////////////////////////////////////
// tenh/applications/polynomial.hpp by Ted Nitz, created 2013/08/27
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_APPLICATIONS_POLYNOMIAL_HPP_
#define TENH_APPLICATIONS_POLYNOMIAL_HPP_

#include "tenh/core.hpp"

#include <iostream>

#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"

using Tenh::Uint32;

template <Uint32 DEGREE_, Uint32 DIMENSION_, typename Id_, typename Scalar_ = float>
struct MultivariatePolynomial
{
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIMENSION_,Id_>,Tenh::Basis_c<Id_> > VectorSpace;
    typedef Tenh::ImplementationOf_t<Scalar_, VectorSpace> Vector;
    typedef Tenh::ImplementationOf_t<Scalar_,Tenh::SymmetricPowerOfBasedVectorSpace_c<VectorSpace,DEGREE_> > Sym;
    // typedef typename Tenh::DualOf_f<Sym>::T SymDual;
   typedef Tenh::ImplementationOf_t<Scalar_,Tenh::SymmetricPowerOfBasedVectorSpace_c<typename Tenh::DualOf_f<VectorSpace>::T,DEGREE_> > SymDual;

    MultivariatePolynomial() : m_term(0) {};
    MultivariatePolynomial(SymDual const &leading_term,  MultivariatePolynomial<DEGREE_-1,DIMENSION_,Id_,Scalar_> const &rest)
        : m_term(leading_term), m_body(rest) {};
    MultivariatePolynomial(MultivariatePolynomial const & other) : m_term(other.m_term), m_body(other.m_body) {};

    Scalar_ evaluate (Vector const &at)
    {
        Tenh::AbstractIndex_c<'i'> i;
        std::cout << m_term << std::endl;
        return m_term(i)*OuterPowerOf(at)(i) + m_body.evaluate(at);
    }

private:
    SymDual m_term;
    MultivariatePolynomial<DEGREE_-1,DIMENSION_,Id_,Scalar_> m_body;

    Sym OuterPowerOf (Vector const &input)
    {
        Sym result(Scalar_(1));

        for (typename Sym::ComponentIndex it; it.is_not_at_end(); ++it)
        {
            typename Sym::MultiIndex m = Sym::template bundle_index_map<typename Sym::MultiIndex::IndexTypeList, typename Sym::ComponentIndex>(it);
            for (Uint32 i = 0; i < Sym::MultiIndex::LENGTH; ++i)
            {
                result[it] *= input[typename Vector::ComponentIndex(m.value_of_index(i, Tenh::DONT_CHECK_RANGE))];
            }
        }
        std::cout << result << std::endl;
        return result;
    }

    template<Uint32,Uint32,typename,typename> friend struct MultivariatePolynomial;
    template<Uint32 DEG1,Uint32 DEG2,Uint32 DIM,typename Id,typename Scalar> friend MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar> operator+ (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs);
    template<Uint32 DEG1,Uint32 DEG2,Uint32 DIM,typename Id,typename Scalar> friend MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar> operator- (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs);
};

template <Uint32 DIMENSION_, typename Id_, typename Scalar_>
struct MultivariatePolynomial<0,DIMENSION_,Id_,Scalar_>
{
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIMENSION_,Id_>,Tenh::Basis_c<Id_> > VectorSpace;
    typedef Tenh::ImplementationOf_t<Scalar_, VectorSpace> Vector;

    MultivariatePolynomial() : m_term(0) {};
    MultivariatePolynomial(Scalar_ const &leading_term) : m_term(leading_term) {};
    MultivariatePolynomial(MultivariatePolynomial const & other) : m_term(other.m_term) {};

    Scalar_ evaluate (Vector const &at)
    {
        std::cout << m_term << std::endl;
        return m_term;
    }

private:
    Scalar_ m_term;

    template<Uint32,Uint32,typename,typename> friend struct MultivariatePolynomial;
    template<Uint32 DEG1,Uint32 DEG2,Uint32 DIM,typename Id,typename Scalar> friend MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar> operator+ (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs);
    template<Uint32 DEG1,Uint32 DEG2,Uint32 DIM,typename Id,typename Scalar> friend MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar> operator- (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs);
};

template<Uint32 DEG1, Uint32 DEG2,  Uint32 DIM, typename Id, typename Scalar>
MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar> operator+ (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs)
{
    if (DEG1 == DEG2)
    {
        if (DEG1 == 0)
        {
            return MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar>(lhs.m_term + rhs.m_term);
        }
        else
        {
            return MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar>(lhs.m_term + rhs.m_term, lhs.m_body + rhs.m_body);
        }
    }
    else if (DEG1 > DEG2)
    {
        return MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar>(rhs.m_term, lhs + rhs.m_body);
    }
    else
    {
        return MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar>(lhs.m_term, lhs.m_body + rhs);
    }
}

template<Uint32 DEG1, Uint32 DEG2,  Uint32 DIM, typename Id, typename Scalar>
MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar> operator- (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs)
{
    if (DEG1 == DEG2)
    {
        if (DEG1 == 0)
        {
            return MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar>(lhs.m_term - rhs.m_term);
        }
        else
        {
            return MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar>(lhs.m_term - rhs.m_term, lhs.m_body - rhs.m_body);
        }
    }
    else if (DEG1 > DEG2)
    {
        return MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar>(rhs.m_term, lhs - rhs.m_body);
    }
    else
    {
        return MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar>(lhs.m_term, lhs.m_body - rhs);
    }
}

#endif // TENH_APPLICATIONS_POLYNOMIAL_HPP_
