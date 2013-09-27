// ///////////////////////////////////////////////////////////////////////////
// tenh/applications/polynomial.hpp by Ted Nitz, created 2013/08/27
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_APPLICATIONS_POLYNOMIAL_HPP_
#define TENH_APPLICATIONS_POLYNOMIAL_HPP_

#include "tenh/core.hpp"

#include <iostream>

#include "tenh/meta/typelist_utility.hpp"
#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"

using Tenh::Uint32;

template <Uint32 DEGREE_, Uint32 DIMENSION_, typename Id_, typename Scalar_ = float>
struct MultivariatePolynomial
{
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIMENSION_,Id_>,Tenh::Basis_c<Id_> > VectorSpace;
    typedef Tenh::ImplementationOf_t<VectorSpace,Scalar_> Vector;
    typedef Tenh::ImplementationOf_t<Tenh::SymmetricPowerOfBasedVectorSpace_c<DEGREE_,VectorSpace>,Scalar_> Sym;
    typedef typename Tenh::DualOf_f<Sym>::T SymDual;
    //typedef Tenh::ImplementationOf_t<Tenh::SymmetricPowerOfBasedVectorSpace_c<DEGREE_,typename Tenh::DualOf_f<VectorSpace>::T>,Scalar_> SymDual;
    typedef Scalar_ Scalar;

    MultivariatePolynomial() : m_term(Scalar_(0)) {};
    MultivariatePolynomial(SymDual const &leading_term,  MultivariatePolynomial<DEGREE_-1,DIMENSION_,Id_,Scalar_> const &rest)
        : m_term(leading_term), m_body(rest) {};
    MultivariatePolynomial(SymDual const &leading_term)
        : m_term(leading_term), m_body() {};

    MultivariatePolynomial(MultivariatePolynomial const & other) : m_term(other.m_term), m_body(other.m_body) {};

    Scalar_ evaluate (Vector const &at) const
    {
        Tenh::AbstractIndex_c<'i'> i;
        return m_term(i)*OuterPowerOf(at)(i) + m_body.evaluate(at);
    }

    // Member operators
    MultivariatePolynomial operator* (Scalar_ const &rhs) const
    {
        Tenh::AbstractIndex_c<'i'> i;
        SymDual term(Scalar_(0));
        term(i) = rhs*m_term(i);
        return MultivariatePolynomial(term, rhs*m_body);
    }

    MultivariatePolynomial operator- () const { return (*this)*Scalar_(-1); }

    template<Uint32 DEG>
    MultivariatePolynomial<(DEG>DEGREE_?DEG:DEGREE_),DIMENSION_,Id_,Scalar_> operator- (MultivariatePolynomial<DEG,DIMENSION_,Id_,Scalar_> const &rhs)
    {
        // Use operator+
        return *this + (-rhs);
    }

private:
    SymDual m_term;
    MultivariatePolynomial<DEGREE_-1,DIMENSION_,Id_,Scalar_> m_body;

    // Helper members for non-member operators.
    //    add is for adding a polynomial of strictly lower degree to this polynomial
    template<Uint32 Other_Degree> MultivariatePolynomial add(MultivariatePolynomial<Other_Degree, DIMENSION_, Id_, Scalar_> const &other) const { return MultivariatePolynomial(m_term, m_body + other); }

    //    add_eq adds a polynomial of the same degree to this polynomial.
    MultivariatePolynomial add_eq(MultivariatePolynomial const &other) const
    {
        Tenh::AbstractIndex_c<'i'> i;
        SymDual term(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
        term(i) = m_term(i) + other.m_term(i);
        return MultivariatePolynomial(term, m_body.add_eq(other.m_body));
    }

    template<Uint32 Term_Degree>
    MultivariatePolynomial<Term_Degree + DEGREE_,DIMENSION_,Id_,Scalar_> monomial_multiply (Tenh::ImplementationOf_t<Tenh::SymmetricPowerOfBasedVectorSpace_c<Term_Degree,typename Tenh::DualOf_f<VectorSpace>::T>,Scalar_> const &monomial) const
    {
        typedef Tenh::SymmetricPowerOfBasedVectorSpace_c<Term_Degree + DEGREE_,typename Tenh::DualOf_f<VectorSpace>::T> ResultingTermType;
        Tenh::AbstractIndex_c<'i'> i;
        Tenh::AbstractIndex_c<'j'> j;
        Tenh::AbstractIndex_c<'k'> k;
        Tenh::ImplementationOf_t<ResultingTermType,Scalar_> result(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
        // I know this doesn't work, we need to symmatrize, not bundle.
        result(i) = (m_term(j)*monomial(k)).bundle(j|k,ResultingTermType(),i);
        return MultivariatePolynomial<Term_Degree + DEGREE_, DIMENSION_, Id_, Scalar_>(result, m_body.template monomial_multiply<Term_Degree>(monomial));
    }

    // This should probably be a function in tensor heaven somewhere.
    Sym OuterPowerOf (Vector const &input) const
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
        return result;
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

    MultivariatePolynomial() : m_term(Scalar_(0)) {};
    MultivariatePolynomial(Scalar_ const &leading_term) : m_term(leading_term) {};
    MultivariatePolynomial(MultivariatePolynomial const & other) : m_term(other.m_term) {};

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

private:
    Scalar_ m_term;

    //    There is no lower degree possible, so we only need add_eq here.
    MultivariatePolynomial add_eq (MultivariatePolynomial const &other) const { return MultivariatePolynomial(m_term + other.m_term); }

    template<Uint32 Term_Degree>
    MultivariatePolynomial<Term_Degree, DIMENSION_, Id_, Scalar_> monomial_multiply (Tenh::ImplementationOf_t<Tenh::SymmetricPowerOfBasedVectorSpace_c<Term_Degree,typename Tenh::DualOf_f<VectorSpace>::T>,Scalar_> const &monomial) const
    {
        Tenh::AbstractIndex_c<'i'> i;
        Tenh::ImplementationOf_t<Tenh::SymmetricPowerOfBasedVectorSpace_c<Term_Degree,typename Tenh::DualOf_f<VectorSpace>::T>,Scalar_> term(Scalar_(0));
        term(i) = m_term*monomial(i);
        return MultivariatePolynomial<Term_Degree, DIMENSION_, Id_, Scalar_>(term);
    }

    template<Uint32,Uint32,typename,typename> friend struct MultivariatePolynomial;
    template<bool> friend struct Adder;
    template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar> friend
    MultivariatePolynomial<DEG,DIM,Id,Scalar> operator+ (MultivariatePolynomial<DEG,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs);
    template<Uint32 DEG1, Uint32 DEG2, Uint32 DIM, typename Id, typename Scalar> friend
    MultivariatePolynomial<DEG1+DEG2,DIM,Id,Scalar> operator* (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs);
    template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar> friend
    MultivariatePolynomial<DEG,DIM,Id,Scalar> operator* (MultivariatePolynomial<0,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs);
    template<Uint32 DIM, typename Id, typename Scalar> friend
    std::ostream &operator << (std::ostream &out, MultivariatePolynomial<0,DIM,Id,Scalar> const &m);
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


//    scalar operator*
template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar>
MultivariatePolynomial<DEG,DIM,Id,Scalar> operator* (Scalar const &lhs, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs)
{
    // Use the member version
    return rhs*lhs;
}


//    operator*
template<Uint32 DEG1, Uint32 DEG2, Uint32 DIM, typename Id, typename Scalar>
MultivariatePolynomial<DEG1+DEG2,DIM,Id,Scalar> operator* (MultivariatePolynomial<DEG1,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG2,DIM,Id,Scalar> const &rhs)
{
    return (rhs.template monomial_multiply<DEG1>(lhs.m_term)) + (lhs.m_body * rhs);
}

template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar>
MultivariatePolynomial<DEG,DIM,Id,Scalar> operator* (MultivariatePolynomial<0,DIM,Id,Scalar> const &lhs, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &rhs)
{
    return lhs.m_term * rhs;
}


//    operator<< for ostream
template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar>
struct VariableStringComputer
{
    static std::string compute(typename MultivariatePolynomial<DEG,DIM,Id,Scalar>::Sym::MultiIndex const &n)
    {
        typedef typename MultivariatePolynomial<DEG,DIM,Id,Scalar>::Sym::MultiIndex MultiIndex;
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
std::ostream &operator << (std::ostream &out, MultivariatePolynomial<DEG,DIM,Id,Scalar> const &m)
{
    typedef typename MultivariatePolynomial<DEG,DIM,Id,Scalar>::Sym::ComponentIndex ComponentIndex;
    typedef typename MultivariatePolynomial<DEG,DIM,Id,Scalar>::Sym::MultiIndex MultiIndex;
    for (ComponentIndex it; it.is_not_at_end(); ++it)
    {
        Uint32 coeff = m.m_term[it];
        if (coeff == 1)
        {
            out << VariableStringComputer<DEG,DIM,Id,Scalar>::compute(MultivariatePolynomial<DEG,DIM,Id,Scalar>::Sym::template bundle_index_map<typename MultiIndex::IndexTypeList, ComponentIndex>(it))
                << " + ";
        }
        else if (coeff != 0)
        {
            out << coeff
                << "*"
                <<  VariableStringComputer<DEG,DIM,Id,Scalar>::compute(MultivariatePolynomial<DEG,DIM,Id,Scalar>::Sym::template bundle_index_map<typename MultiIndex::IndexTypeList, ComponentIndex>(it))
                << " + ";
        }
    }

    return out << m.m_body;
}

template<Uint32 DIM, typename Id, typename Scalar>
std::ostream &operator << (std::ostream &out, MultivariatePolynomial<0,DIM,Id,Scalar> const &m)
{
    return out << m.m_term;
}


#endif // TENH_APPLICATIONS_POLYNOMIAL_HPP_
