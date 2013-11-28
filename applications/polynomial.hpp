// ///////////////////////////////////////////////////////////////////////////
// applications/polynomial.hpp by Ted Nitz, created 2013/08/27
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef APPLICATIONS_POLYNOMIAL_HPP_
#define APPLICATIONS_POLYNOMIAL_HPP_

#include "applications/homogeneouspolynomial.hpp"

#include "tenh/core.hpp"

#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/sym.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/multiindex.hpp"
#include "tenh/preallocatedarray.hpp"

using Tenh::Uint32;

template <Uint32 DEGREE_, typename BasedVectorSpace_, typename Scalar_ = float>
struct MultivariatePolynomial
{
    typedef Tenh::ImplementationOf_t<BasedVectorSpace_,Scalar_> Vector;
    typedef Tenh::SymmetricPowerOfBasedVectorSpace_c<DEGREE_,BasedVectorSpace_> SymmetricPower;
    typedef Tenh::ImplementationOf_t<SymmetricPower,Scalar_> Sym;
    typedef typename Tenh::DualOf_f<Sym>::T SymDual;
    typedef Scalar_ Scalar;

    typedef HomogeneousPolynomial<DEGREE_,BasedVectorSpace_,Scalar_> LeadingTermType;

    typedef MultivariatePolynomial<DEGREE_-1,BasedVectorSpace_,Scalar_> BodyPolynomial;
    static Uint32 const DIMENSION = LeadingTermType::DIMENSION + BodyPolynomial::DIMENSION;

    typedef Tenh::PreallocatedArray_t<Scalar_,DIMENSION> CoefficientArray;
    typedef Tenh::PreallocatedArray_t<Scalar_ const,DIMENSION> ConstCoefficientArray;

    MultivariatePolynomial (Scalar_ const &fill_with) : m_body(fill_with), m_term(fill_with) { }
    MultivariatePolynomial (Tenh::WithoutInitialization const &w) : m_body(w), m_term(w) { }
    MultivariatePolynomial (LeadingTermType const &leading_term, BodyPolynomial const &body)
        :
        m_body(body),
        m_term(leading_term)
    { }
    MultivariatePolynomial (LeadingTermType const &leading_term)
        :
        m_body(Scalar(0)),
        m_term(leading_term)
    { }
    MultivariatePolynomial (SymDual const &leading_term, BodyPolynomial const &body)
        :
        m_body(body),
        m_term(leading_term)
    { }
    MultivariatePolynomial (SymDual const &leading_term)
        :
        m_body(Scalar(0)),
        m_term(leading_term)
    { }
    MultivariatePolynomial (MultivariatePolynomial const &other)
        :
        m_body(other.m_body),
        m_term(other.m_term)
    { }

    Scalar_ evaluate (Vector const &at) const
    {
        return m_term.evaluate(at) + m_body.evaluate(at);
    }

    // Member operators
    MultivariatePolynomial operator * (Scalar_ const &rhs) const
    {
        return MultivariatePolynomial(m_term*rhs, m_body*rhs);
    }

    template <Uint32 OTHER_DEGREE_>
    MultivariatePolynomial<DEGREE_+OTHER_DEGREE_,BasedVectorSpace_,Scalar_>
        operator * (const HomogeneousPolynomial<OTHER_DEGREE_,BasedVectorSpace_,Scalar_> &rhs) const
    {
        return MultivariatePolynomial<DEGREE_+OTHER_DEGREE_,BasedVectorSpace_,Scalar_>((m_term * rhs)) + m_body*rhs;
    }

    MultivariatePolynomial operator + (Scalar_ const &rhs) const
    {
        return MultivariatePolynomial(m_term, m_body+rhs);
    }

    MultivariatePolynomial operator - () const { return (*this)*Scalar_(-1); }

    template <Uint32 OTHER_DEGREE_>
    MultivariatePolynomial<(OTHER_DEGREE_>DEGREE_?OTHER_DEGREE_:DEGREE_),BasedVectorSpace_,Scalar_>
        operator - (MultivariatePolynomial<OTHER_DEGREE_,BasedVectorSpace_,Scalar_> const &rhs)
    {
        // Use operator+
        return *this + (-rhs);
    }

    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    ConstCoefficientArray as_array () const
    {
        assert(reinterpret_cast<Tenh::Uint8 const *>(&m_body) + sizeof(m_body) == reinterpret_cast<Tenh::Uint8 const *>(&m_term) &&
               "for this to work, the members must be layed out contiguously in memory");
        return ConstCoefficientArray(reinterpret_cast<Scalar_ const *>(&m_body), Tenh::DONT_CHECK_POINTER);
    }
    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    CoefficientArray as_array ()
    {
        assert(reinterpret_cast<Tenh::Uint8 const *>(&m_body) + sizeof(m_body) == reinterpret_cast<Tenh::Uint8 const *>(&m_term) &&
               "for this to work, the members must be layed out contiguously in memory");
        return CoefficientArray(reinterpret_cast<Scalar_ *>(&m_body), Tenh::DONT_CHECK_POINTER);
    }

    bool is_exactly_zero() const
    {
        return m_term.is_exactly_zero() && m_body.is_exactly_zero();
    }

private:
    MultivariatePolynomial<DEGREE_-1,BasedVectorSpace_,Scalar_> m_body;
    LeadingTermType m_term;

    // Helper members for non-member operators.
    //    add is for adding a polynomial of strictly lower degree to this polynomial
    template<Uint32 OTHER_DEGREE_> MultivariatePolynomial
        add (MultivariatePolynomial<OTHER_DEGREE_,BasedVectorSpace_,Scalar_> const &other) const
    {
        return MultivariatePolynomial(m_term, m_body + other);
    }

    //    add_eq adds a polynomial of the same degree to this polynomial.
    MultivariatePolynomial add_eq (MultivariatePolynomial const &other) const
    {
        return MultivariatePolynomial(m_term + other.m_term, m_body.add_eq(other.m_body));
    }

    template <Uint32,typename,typename> friend struct MultivariatePolynomial;
    template <bool> friend struct Adder;
    template <Uint32 DEG, typename Scalar> friend
    MultivariatePolynomial<DEG,BasedVectorSpace_,Scalar>
        operator + (MultivariatePolynomial<DEG,BasedVectorSpace_,Scalar> const &lhs,
                    MultivariatePolynomial<DEG,BasedVectorSpace_,Scalar> const &rhs);
    template <Uint32 DEG1, Uint32 DEG2, typename Scalar> friend
    MultivariatePolynomial<DEG1+DEG2,BasedVectorSpace_,Scalar>
        operator * (MultivariatePolynomial<DEG1,BasedVectorSpace_,Scalar> const &lhs,
                    MultivariatePolynomial<DEG2,BasedVectorSpace_,Scalar> const &rhs);
    template <Uint32 DEG, typename Scalar> friend
    std::ostream &operator << (std::ostream &out, MultivariatePolynomial<DEG,BasedVectorSpace_,Scalar> const &m);
};

template <typename BasedVectorSpace_, typename Scalar_>
struct MultivariatePolynomial<0,BasedVectorSpace_,Scalar_>
{
    typedef Tenh::ImplementationOf_t<BasedVectorSpace_,Scalar_> Vector;

    static Uint32 const DIMENSION = 1;

    typedef Tenh::PreallocatedArray_t<Scalar_,DIMENSION> CoefficientArray;
    typedef Tenh::PreallocatedArray_t<Scalar_ const,DIMENSION> ConstCoefficientArray;

    MultivariatePolynomial (Scalar_ const &leading_term) : m_term(leading_term) { }
    MultivariatePolynomial (Tenh::WithoutInitialization const &w) { }
    MultivariatePolynomial (MultivariatePolynomial const &other) : m_term(other.m_term) { }

    Scalar_ evaluate (Vector const &at) const
    {
        return m_term;
    }

    MultivariatePolynomial operator - () const { return (*this)*Scalar_(-1); }

    template <Uint32 DEG>
    MultivariatePolynomial<DEG,BasedVectorSpace_,Scalar_>
        operator - (MultivariatePolynomial<DEG,BasedVectorSpace_,Scalar_> const &rhs)
    {
        // Use operator+
        return *this + (-rhs);
    }

    MultivariatePolynomial operator * (Scalar_ const &rhs) const { return MultivariatePolynomial(rhs*m_term); }

    template <Uint32 OTHER_DEGREE_>
    MultivariatePolynomial<OTHER_DEGREE_,BasedVectorSpace_,Scalar_>
        operator * (const HomogeneousPolynomial<OTHER_DEGREE_,BasedVectorSpace_,Scalar_> &rhs) const
    {
        return MultivariatePolynomial<OTHER_DEGREE_,BasedVectorSpace_,Scalar_>((m_term * rhs));
    }

    MultivariatePolynomial operator + (Scalar_ const &rhs) const { return MultivariatePolynomial(rhs+m_term); }

    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    ConstCoefficientArray as_array () const
    {
        return ConstCoefficientArray(reinterpret_cast<Scalar_ const *>(&m_term), Tenh::DONT_CHECK_POINTER);
    }
    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    CoefficientArray as_array ()
    {
        return CoefficientArray(reinterpret_cast<Scalar_ *>(&m_term), Tenh::DONT_CHECK_POINTER);
    }

    bool is_exactly_zero() const
    {
        return m_term == Scalar_(0);
    }

private:
    Scalar_ m_term;

    //    There is no lower degree possible, so we only need add_eq here.
    MultivariatePolynomial add_eq (MultivariatePolynomial const &other) const { return MultivariatePolynomial(m_term + other.m_term); }


    template <Uint32,typename,typename>
    friend struct MultivariatePolynomial;
    template <bool>
    friend struct Adder;
    template <Uint32 DEG_>
    friend MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_>
        operator + (MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_> const &lhs,
                    MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_> const &rhs);
    template <Uint32 DEG1_, Uint32 DEG2_>
    friend MultivariatePolynomial<DEG1_+DEG2_,BasedVectorSpace_,Scalar_>
        operator * (MultivariatePolynomial<DEG1_,BasedVectorSpace_,Scalar_> const &lhs,
                   MultivariatePolynomial<DEG2_,BasedVectorSpace_,Scalar_> const &rhs);
    template <Uint32 DEG_>
    friend MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_>
        operator * (MultivariatePolynomial<0,BasedVectorSpace_,Scalar_> const &lhs,
                    MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_> const &rhs);
    friend std::ostream &operator << (std::ostream &out, MultivariatePolynomial<0,BasedVectorSpace_,Scalar_> const &m);
};


// Non-member operator overloads. These exist because they must be partially specalized, or because they cannot be written as member operator overloads.
//    operator+
template <bool DEGREE_OF_LHS_IS_GREATER_THAN_DEGREE_OF_RHS> struct Adder
{
    template<typename T, typename S>
    static T add (T const &lhs, S const &rhs) { return lhs.add(rhs); }
private:
    Adder();
};

template <> struct Adder<false>
{
    template<typename T, typename S>
    static S add (T const &lhs, S const &rhs) { return rhs.add(lhs); }
private:
    Adder();
};

template <Uint32 DEG1_, Uint32 DEG2_, typename BasedVectorSpace_, typename Scalar_>
MultivariatePolynomial<(DEG1_>DEG2_?DEG1_:DEG2_),BasedVectorSpace_,Scalar_>
    operator + (MultivariatePolynomial<DEG1_,BasedVectorSpace_,Scalar_> const &lhs,
                MultivariatePolynomial<DEG2_,BasedVectorSpace_,Scalar_> const &rhs)
{
    return Adder<(DEG1_>DEG2_)>::add(lhs,rhs);
}

template <Uint32 DEG_, typename BasedVectorSpace_, typename Scalar_>
MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_>
    operator + (MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_> const &lhs,
                MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_> const &rhs)
{
    return lhs.add_eq(rhs);
}

//    scalar operator +
template <Uint32 DEG_, typename BasedVectorSpace_, typename Scalar_, typename T>
MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_>
    operator + (T const &lhs, MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_> const &rhs)
{
    // Use the member version
    return rhs+Scalar_(lhs);
}

//    scalar operator *
template <Uint32 DEG_, typename BasedVectorSpace_, typename Scalar_>
MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_>
    operator * (Scalar_ const &lhs, MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_> const &rhs)
{
    // Use the member version
    return rhs*lhs;
}

//    operator * for HomogeneousPolynomials
template <Uint32 DEG1_, Uint32 DEG2_, typename BasedVectorSpace_, typename Scalar_>
MultivariatePolynomial<DEG1_+DEG2_,BasedVectorSpace_,Scalar_>
    operator * (HomogeneousPolynomial<DEG1_,BasedVectorSpace_,Scalar_> const &lhs,
                MultivariatePolynomial<DEG2_,BasedVectorSpace_,Scalar_> const &rhs)
{
    return MultivariatePolynomial<DEG1_+DEG2_,BasedVectorSpace_,Scalar_>(rhs*lhs);
}

//    operator*
template <Uint32 DEG1_, Uint32 DEG2_, typename BasedVectorSpace_, typename Scalar_>
MultivariatePolynomial<DEG1_+DEG2_,BasedVectorSpace_,Scalar_>
    operator * (MultivariatePolynomial<DEG1_,BasedVectorSpace_,Scalar_> const &lhs,
                MultivariatePolynomial<DEG2_,BasedVectorSpace_,Scalar_> const &rhs)
{
    return (rhs * lhs.m_term) + (lhs.m_body * rhs);
}

template <Uint32 DEG_, typename BasedVectorSpace_, typename Scalar_>
MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_>
    operator * (MultivariatePolynomial<0,BasedVectorSpace_,Scalar_> const &lhs,
                MultivariatePolynomial<DEG_,BasedVectorSpace_,Scalar_> const &rhs)
{
    return rhs * lhs.m_term;
}

#endif // APPLICATIONS_POLYNOMIAL_HPP_
