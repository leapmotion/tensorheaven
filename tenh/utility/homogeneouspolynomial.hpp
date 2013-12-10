// ///////////////////////////////////////////////////////////////////////////
// tenh/utility/homogeneouspolynomial.hpp by Ted Nitz, created 2013/08/27
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_UTILITY_HOMOGENEOUSPOLYNOMIAL_HPP_
#define TENH_UTILITY_HOMOGENEOUSPOLYNOMIAL_HPP_

#include "tenh/core.hpp"

#include <iostream>

#include "tenh/conceptual/symmetricpower.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/sym.hpp"
#include "tenh/implementation/vector.hpp"
#include "tenh/implementation/vee.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/multiindex.hpp"
#include "tenh/preallocatedarray.hpp"

namespace Tenh {

template <Uint32 DEGREE_, typename BasedVectorSpace_, typename Scalar_ = float>
struct HomogeneousPolynomial
{
    typedef ImplementationOf_t<BasedVectorSpace_,Scalar_> Vector;
    typedef SymmetricPowerOfBasedVectorSpace_c<DEGREE_,BasedVectorSpace_> SymmetricPower;
    typedef ImplementationOf_t<SymmetricPower,Scalar_> Sym;
    typedef typename DualOf_f<Sym>::T SymDual;

    typedef Scalar_ Scalar;

    static Uint32 const DIMENSION = SymDual::DIM;

    typedef PreallocatedArray_t<Scalar_,DIMENSION> CoefficientArray;
    typedef PreallocatedArray_t<Scalar_ const,DIMENSION> ConstCoefficientArray;

    HomogeneousPolynomial (FillWith_t<Scalar_> const &fill_with) : m_coefficients(fill_with) { }
    HomogeneousPolynomial (WithoutInitialization const &w) : m_coefficients(w) { }
    HomogeneousPolynomial (SymDual const &term) : m_coefficients(term) { }
    HomogeneousPolynomial (HomogeneousPolynomial const &other) : m_coefficients(other.m_coefficients) { }

    Scalar_ evaluate (Vector const &at) const
    {
        AbstractIndex_c<'i'> i;
        return m_coefficients(i)*outer_power_of(at)(i);
    }

    // Member operators
    HomogeneousPolynomial operator * (Scalar_ const &rhs) const
    {
        HomogeneousPolynomial result(Static<WithoutInitialization>::SINGLETON);
        AbstractIndex_c<'i'> i;

        result.m_coefficients(i) = rhs*m_coefficients(i);
        return result;
    }

    HomogeneousPolynomial operator - () const { return (*this)*Scalar_(-1); }

    HomogeneousPolynomial operator - (HomogeneousPolynomial const &rhs)
    {
        // Use operator+
        return *this + (-rhs);
    }

    HomogeneousPolynomial operator + (HomogeneousPolynomial const &rhs) const
    {
        AbstractIndex_c<'i'> i;
        HomogeneousPolynomial result(Static<WithoutInitialization>::SINGLETON);
        result.m_coefficients(i) = m_coefficients(i) + rhs.m_coefficients(i);
        return result;
    }

    template<Uint32 OTHER_DEGREE_>
    HomogeneousPolynomial<OTHER_DEGREE_ + DEGREE_,BasedVectorSpace_,Scalar_>
        operator * (HomogeneousPolynomial<OTHER_DEGREE_,BasedVectorSpace_,Scalar_> const &rhs) const
    {
        typedef HomogeneousPolynomial<OTHER_DEGREE_ + DEGREE_,BasedVectorSpace_,Scalar_> ResultPolynomial;
        typedef typename ResultPolynomial::SymDual ResultSymDual;
        typedef typename ResultSymDual::MultiIndex ResultMultiIndex;
        typedef typename ResultSymDual::ComponentIndex ResultComponentIndex;
        typedef typename TensorPowerOfBasedVectorSpace_f<OTHER_DEGREE_ + DEGREE_,typename DualOf_f<BasedVectorSpace_>::T>::T ResultingTensorPowerType;

        typedef typename Sym_f<OTHER_DEGREE_ + DEGREE_,typename DualOf_f<BasedVectorSpace_>::T,Scalar_>::T SymmetrizeType;

        ResultPolynomial result(Static<WithoutInitialization>::SINGLETON);
        SymmetrizeType symmetrize;

        AbstractIndex_c<'i'> i;
        AbstractIndex_c<'j'> j;
        AbstractIndex_c<'k'> k;
        AbstractIndex_c<'I'> I;
        AbstractIndex_c<'J'> J;
        AbstractIndex_c<'K'> K;

        result.m_coefficients(i) = (m_coefficients(j).split(j,J)*rhs.m_coefficients(k).split(k,K)).bundle(J|K,ResultingTensorPowerType(),I)*symmetrize(i|I);

        for (ResultComponentIndex it; it.is_not_at_end(); ++it)
        {
            ResultMultiIndex m = ResultSymDual::template bundle_index_map<typename ResultMultiIndex::IndexTypeList, ResultComponentIndex>(it);
            result.m_coefficients[it] *= static_cast<Scalar_>(MultiIndexMultiplicity_t<ResultMultiIndex>::eval(m))
                                         / static_cast<Scalar_>(Factorial_t<OTHER_DEGREE_ + DEGREE_>::V);
        }

        return result;
    }

    const SymDual coefficients () const
    {
        return m_coefficients;
    }

    bool is_exactly_zero () const
    {
        for (typename SymDual::ComponentIndex it; it.is_not_at_end(); ++it)
        {
            if (m_coefficients[it] != Scalar(0))
            {
                return false;
            }
        }

        return true;
    }

    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    ConstCoefficientArray as_array () const
    {
        return ConstCoefficientArray(reinterpret_cast<Scalar_ const *>(&m_coefficients), DONT_CHECK_POINTER);
    }
    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    CoefficientArray as_array ()
    {
        return CoefficientArray(reinterpret_cast<Scalar_ *>(&m_coefficients), DONT_CHECK_POINTER);
    }

private:
    SymDual m_coefficients;

    // This should probably be a function in tensor heaven somewhere.
    Sym outer_power_of (Vector const &input) const
    {
        Sym result(fill_with<Scalar_>(1));

        for (typename Sym::ComponentIndex it; it.is_not_at_end(); ++it)
        {
            typename Sym::MultiIndex m = Sym::template bundle_index_map<typename Sym::MultiIndex::IndexTypeList, typename Sym::ComponentIndex>(it);
            for (Uint32 i = 0; i < Sym::MultiIndex::LENGTH; ++i)
            {
                result[it] *= input[typename Vector::ComponentIndex(m.value_of_index(i, DONT_CHECK_RANGE))];
            }
            result[it] *= Factorial_t<DEGREE_>::V / (MultiIndexMultiplicity_t<typename Sym::MultiIndex>::eval(m));
        }
        return result;
    }

    template<Uint32,typename,typename> friend struct HomogeneousPolynomial;
};


// Non-member operator overloads. These exist because they must be partially specalized, or because they cannot be written as member operator overloads.
//    scalar operator +
template <Uint32 DEG_, typename BasedVectorSpace_, typename Scalar_, typename T_>
HomogeneousPolynomial<DEG_,BasedVectorSpace_,Scalar_>
    operator + (T_ const &lhs, HomogeneousPolynomial<DEG_,BasedVectorSpace_,Scalar_> const &rhs)
{
    // Use the member version
    return rhs+Scalar_(lhs);
}

//    scalar operator *
template <Uint32 DEG_, typename BasedVectorSpace_, typename Scalar_, typename T_>
HomogeneousPolynomial<DEG_,BasedVectorSpace_,Scalar_>
    operator * (T_ const &lhs, HomogeneousPolynomial<DEG_,BasedVectorSpace_,Scalar_> const &rhs)
{
    // Use the member version
    return rhs*Scalar_(lhs);
}

} // end of namespace Tenh

#endif // TENH_UTILITY_HOMOGENEOUSPOLYNOMIAL_HPP_
