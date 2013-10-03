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

template <Uint32 DEGREE_, Uint32 DIMENSION_, typename Id_ = PolynomialBasisId, typename Scalar_ = float>
struct MultivariatePolynomial
{
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIMENSION_,Id_>,
                                     Tenh::Basis_c<Id_> > VectorSpace;
    typedef Tenh::ImplementationOf_t<VectorSpace,Scalar_> Vector;
    typedef Tenh::SymmetricPowerOfBasedVectorSpace_c<DEGREE_,VectorSpace> SymmetricPower;
    typedef Tenh::ImplementationOf_t<SymmetricPower,Scalar_> Sym;
    typedef typename Tenh::DualOf_f<Sym>::T SymDual;
    typedef Scalar_ Scalar;
    typedef Id_ Id;

    typedef HomogeneousPolynomial<DEGREE_,DIMENSION_,Id_,Scalar_> LeadingTermType;

    typedef MultivariatePolynomial<DEGREE_-1,DIMENSION_,Id_,Scalar_> BodyPolynomial;
    static Uint32 const DIMENSION = LeadingTermType::DIMENSION + BodyPolynomial::DIMENSION;

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
    MultivariatePolynomial operator* (Scalar_ const &rhs) const
    {
        return MultivariatePolynomial(m_term*rhs, m_body*rhs);
    }

    template <Uint32 Other_Degree>
    MultivariatePolynomial<DEGREE_+Other_Degree,DIMENSION_,Id_,Scalar_> operator* (const HomogeneousPolynomial<Other_Degree,DIMENSION_,Id_,Scalar_> &rhs) const
    {
        return MultivariatePolynomial<DEGREE_+Other_Degree,DIMENSION_,Id_,Scalar_>((m_term * rhs)) + m_body*rhs;
    }

    MultivariatePolynomial operator+ (Scalar_ const &rhs) const
    {
        return MultivariatePolynomial(m_term, m_body+rhs);
    }

    MultivariatePolynomial operator- () const { return (*this)*Scalar_(-1); }

    template<Uint32 DEG>
    MultivariatePolynomial<(DEG>DEGREE_?DEG:DEGREE_),DIMENSION_,Id_,Scalar_> operator- (
        MultivariatePolynomial<DEG,DIMENSION_,Id_,Scalar_> const &rhs)
    {
        // Use operator+
        return *this + (-rhs);
    }

    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    Tenh::PreallocatedArray_t<Scalar_ const,DIMENSION> as_array () const
    {
        assert(reinterpret_cast<Tenh::Uint8 const *>(&m_body) + sizeof(m_body) == reinterpret_cast<Tenh::Uint8 const *>(&m_term) &&
               "for this to work, the members must be layed out contiguously in memory");
        return Tenh::PreallocatedArray_t<Scalar_ const,DIMENSION>(reinterpret_cast<Scalar_ const *>(&m_body), Tenh::DONT_CHECK_POINTER);
    }
    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    Tenh::PreallocatedArray_t<Scalar_,DIMENSION> as_array ()
    {
        assert(reinterpret_cast<Tenh::Uint8 const *>(&m_body) + sizeof(m_body) == reinterpret_cast<Tenh::Uint8 const *>(&m_term) &&
               "for this to work, the members must be layed out contiguously in memory");
        return Tenh::PreallocatedArray_t<Scalar_,DIMENSION>(reinterpret_cast<Scalar_ *>(&m_body), Tenh::DONT_CHECK_POINTER);
    }

    bool is_exactly_zero() const
    {
        return m_term.is_exactly_zero() && m_body.is_exactly_zero();
    }

private:
    MultivariatePolynomial<DEGREE_-1,DIMENSION_,Id_,Scalar_> m_body;
    LeadingTermType m_term;

    // Helper members for non-member operators.
    //    add is for adding a polynomial of strictly lower degree to this polynomial
    template<Uint32 Other_Degree> MultivariatePolynomial add(MultivariatePolynomial<Other_Degree, DIMENSION_, Id_, Scalar_> const &other) const { return MultivariatePolynomial(m_term, m_body + other); }

    //    add_eq adds a polynomial of the same degree to this polynomial.
    MultivariatePolynomial add_eq(MultivariatePolynomial const &other) const
    {
        return MultivariatePolynomial(m_term + other.m_term, m_body.add_eq(other.m_body));
    }

    template<Uint32,Uint32,typename,typename> friend struct MultivariatePolynomial;
    template<bool> friend struct Adder;
    template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar> friend
    MultivariatePolynomial<DEG,DIM,Id,Scalar> operator+ (MultivariatePolynomial<DEG,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs);
    template<Uint32 DEG1, Uint32 DEG2, Uint32 DIM, typename Id, typename Scalar> friend
    MultivariatePolynomial<DEG1+DEG2,DIM,Id,Scalar> operator* (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs);
    template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar> friend
    std::ostream &operator << (std::ostream &out, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &m);
};

template <Uint32 DIMENSION_, typename Id_, typename Scalar_>
struct MultivariatePolynomial<0,DIMENSION_,Id_,Scalar_>
{
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIMENSION_,Id_>,Tenh::Basis_c<Id_> > VectorSpace;
    typedef Tenh::ImplementationOf_t<VectorSpace,Scalar_> Vector;

    static Uint32 const DIMENSION = 1;

    MultivariatePolynomial (Scalar_ const &leading_term) : m_term(leading_term) { }
    MultivariatePolynomial (Tenh::WithoutInitialization const &w) { }
    MultivariatePolynomial (MultivariatePolynomial const &other) : m_term(other.m_term) { }

    Scalar_ evaluate (Vector const &at) const
    {
        return m_term;
    }

    MultivariatePolynomial operator- () const { return (*this)*Scalar_(-1); }

    template<Uint32 DEG>
    MultivariatePolynomial<DEG,DIMENSION_,Id_,Scalar_> operator- (MultivariatePolynomial<DEG,DIMENSION_,Id_,Scalar_> const &rhs)
    {
        // Use operator+
        return *this + (-rhs);
    }

    MultivariatePolynomial operator* (Scalar_ const &rhs) const { return MultivariatePolynomial(rhs*m_term); }

    template <Uint32 Other_Degree>
    MultivariatePolynomial<Other_Degree,DIMENSION_,Id_,Scalar_> operator* (const HomogeneousPolynomial<Other_Degree,DIMENSION_,Id_,Scalar_> &rhs) const
    {
        return MultivariatePolynomial<Other_Degree,DIMENSION_,Id_,Scalar_>((m_term * rhs));
    }


    MultivariatePolynomial operator+ (Scalar_ const &rhs) const { return MultivariatePolynomial(rhs+m_term); }

    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    Tenh::PreallocatedArray_t<Scalar_ const,DIMENSION> as_array () const
    {
        return Tenh::PreallocatedArray_t<Scalar_ const,DIMENSION>(reinterpret_cast<Scalar_ const *>(&m_term), Tenh::DONT_CHECK_POINTER);
    }
    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    Tenh::PreallocatedArray_t<Scalar_,DIMENSION> as_array ()
    {
        return Tenh::PreallocatedArray_t<Scalar_,DIMENSION>(reinterpret_cast<Scalar_ *>(&m_term), Tenh::DONT_CHECK_POINTER);
    }

    bool is_exactly_zero() const
    {
        return m_term == Scalar_(0);
    }

private:
    Scalar_ m_term;

    //    There is no lower degree possible, so we only need add_eq here.
    MultivariatePolynomial add_eq (MultivariatePolynomial const &other) const { return MultivariatePolynomial(m_term + other.m_term); }


    template<Uint32,Uint32,typename,typename>
    friend struct MultivariatePolynomial;
    template<bool>
    friend struct Adder;
    template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar>
    friend MultivariatePolynomial<DEG,DIM,Id,Scalar> operator+ (MultivariatePolynomial<DEG,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs);
    template<Uint32 DEG1, Uint32 DEG2, Uint32 DIM, typename Id, typename Scalar>
    friend MultivariatePolynomial<DEG1+DEG2,DIM,Id,Scalar> operator* (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs);
    template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar>
    friend MultivariatePolynomial<DEG,DIM,Id,Scalar> operator* (MultivariatePolynomial<0,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs);
    template<Uint32 DIM, typename Id, typename Scalar>
    friend std::ostream &operator << (std::ostream &out, MultivariatePolynomial<0,DIM,Id,Scalar> const &m);
};


// Non-member operator overloads. These exist because they must be partially specalized, or because they cannot be written as member operator overloads.
//    operator+
template<bool DEGREE_OF_LHS_IS_GREATER_THAN_DEGREE_OF_RHS> struct Adder
{
    template<typename T, typename S>
    static T add (T const &lhs, S const &rhs) { return lhs.add(rhs); }
};

template<> struct Adder<false>
{
    template<typename T, typename S>
    static S add (T const &lhs, S const &rhs) { return rhs.add(lhs); }
};

template<Uint32 DEG1, Uint32 DEG2, Uint32 DIM, typename Id, typename Scalar>
MultivariatePolynomial<(DEG1>DEG2?DEG1:DEG2),DIM,Id,Scalar> operator+ (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs)
{
    return Adder<(DEG1>DEG2)>::add(lhs,rhs);
}

template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar>
MultivariatePolynomial<DEG,DIM,Id,Scalar> operator+ (MultivariatePolynomial<DEG,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs)
{
    return lhs.add_eq(rhs);
}


//    scalar operator+
template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar, typename T>
MultivariatePolynomial<DEG,DIM,Id,Scalar> operator+ (T const &lhs, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs)
{
    // Use the member version
    return rhs+Scalar(lhs);
}


//    scalar operator*
template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar>
MultivariatePolynomial<DEG,DIM,Id,Scalar> operator* (Scalar const &lhs, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs)
{
    // Use the member version
    return rhs*lhs;
}


//    operator* for HomogeneousPolynomials
template <Uint32 DEG1, Uint32 DEG2, Uint32 DIM, typename Id, typename Scalar>
MultivariatePolynomial<DEG1+DEG2,DIM,Id,Scalar> operator* (HomogeneousPolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs)
{
    return MultivariatePolynomial<DEG1+DEG2,DIM,Id,Scalar>(rhs*lhs);
}

//    operator*
template<Uint32 DEG1, Uint32 DEG2, Uint32 DIM, typename Id, typename Scalar>
MultivariatePolynomial<DEG1+DEG2,DIM,Id,Scalar> operator* (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs)
{
    return (rhs * lhs.m_term) + (lhs.m_body * rhs);
}

template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar> MultivariatePolynomial<DEG,DIM,Id,Scalar> operator* (MultivariatePolynomial<0,DIM,Id,Scalar> const &lhs,
MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs) { return rhs * lhs.m_term; }


#endif // APPLICATIONS_POLYNOMIAL_HPP_
