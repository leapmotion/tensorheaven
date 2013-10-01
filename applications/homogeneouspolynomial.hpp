// ///////////////////////////////////////////////////////////////////////////
// applications/homogeneouspolynomial.hpp by Ted Nitz, created 2013/08/27
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef APPLICATIONS_HOMOGENEOUSPOLYNOMIAL_HPP_
#define APPLICATIONS_HOMOGENEOUSPOLYNOMIAL_HPP_

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

using Tenh::Uint32;

struct PolynomialBasisId { static std::string type_as_string () { return "StandardPolynomialBasis"; } };

template <Uint32 DEGREE_, Uint32 DIMENSION_, typename Id_ = PolynomialBasisId, typename Scalar_ = float>
struct HomogeneousPolynomial
{
    typedef Tenh::BasedVectorSpace_c<Tenh::VectorSpace_c<Tenh::RealField,DIMENSION_,Id_>,
                                     Tenh::Basis_c<Id_> > VectorSpace;
    typedef Tenh::ImplementationOf_t<VectorSpace,Scalar_> Vector;
    typedef Tenh::SymmetricPowerOfBasedVectorSpace_c<DEGREE_,VectorSpace> SymmetricPower;
    typedef Tenh::ImplementationOf_t<SymmetricPower,Scalar_> Sym;
    typedef typename Tenh::DualOf_f<Sym>::T SymDual;

    typedef Scalar_ Scalar;
    typedef Id_ Id;

    static Uint32 const DIMENSION = SymDual::DIM;

    HomogeneousPolynomial (Scalar_ const &fill_with) : m_coefficients(fill_with) { }
    HomogeneousPolynomial (Tenh::WithoutInitialization const &w) : m_coefficients(w) { }
    HomogeneousPolynomial (SymDual const &term) : m_coefficients(term) { }
    HomogeneousPolynomial (HomogeneousPolynomial const &other) : m_coefficients(other.m_coefficients) { }

    Scalar_ evaluate (Vector const &at) const
    {
        Tenh::AbstractIndex_c<'i'> i;
        return m_coefficients(i)*outer_power_of(at)(i);
    }

    // Member operators
    HomogeneousPolynomial operator* (Scalar_ const &rhs) const
    {
        HomogeneousPolynomial result(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
        Tenh::AbstractIndex_c<'i'> i;

        result.m_coefficients(i) = rhs*m_coefficients(i);
        return result;
    }

    HomogeneousPolynomial operator- () const { return (*this)*Scalar_(-1); }

    HomogeneousPolynomial operator- (HomogeneousPolynomial const &rhs)
    {
        // Use operator+
        return *this + (-rhs);
    }

    HomogeneousPolynomial operator+ (HomogeneousPolynomial const &rhs) const
    {
        Tenh::AbstractIndex_c<'i'> i;
        HomogeneousPolynomial result(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
        result.m_coefficients(i) = m_coefficients(i) + rhs.m_coefficients(i);
        return result;
    }

    template<Uint32 Other_Degree>
    HomogeneousPolynomial<Other_Degree + DEGREE_,DIMENSION_,Id_,Scalar_> operator* (HomogeneousPolynomial<Other_Degree,DIMENSION_,Id_,Scalar_> const &rhs) const
    {
        typedef HomogeneousPolynomial<Other_Degree + DEGREE_,DIMENSION_,Id_,Scalar_> ResultPolynomial;
        typedef typename ResultPolynomial::SymDual ResultSymDual;
        typedef typename ResultSymDual::MultiIndex ResultMultiIndex;
        typedef typename ResultSymDual::ComponentIndex ResultComponentIndex;
        typedef typename Tenh::TensorPowerOfBasedVectorSpace_f<Other_Degree + DEGREE_,typename Tenh::DualOf_f<VectorSpace>::T>::T ResultingTensorPowerType;


        typedef typename Tenh::Sym_f<Other_Degree + DEGREE_,typename Tenh::DualOf_f<VectorSpace>::T,Scalar_>::T SymmetrizeType;

        ResultPolynomial result(Tenh::Static<Tenh::WithoutInitialization>::SINGLETON);
        SymmetrizeType symmetrize;

        Tenh::AbstractIndex_c<'i'> i;
        Tenh::AbstractIndex_c<'j'> j;
        Tenh::AbstractIndex_c<'k'> k;
        Tenh::AbstractIndex_c<'I'> I;
        Tenh::AbstractIndex_c<'J'> J;
        Tenh::AbstractIndex_c<'K'> K;

        result.m_coefficients(i) = (m_coefficients(j).split(j,J)*rhs.m_coefficients(k).split(k,K)).bundle(J|K,ResultingTensorPowerType(),I)*symmetrize(i|I);

        for (ResultComponentIndex it; it.is_not_at_end(); ++it)
        {
            ResultMultiIndex m = ResultSymDual::template bundle_index_map<typename ResultMultiIndex::IndexTypeList, ResultComponentIndex>(it);
            result.m_coefficients[it] *= static_cast<Scalar_>(Tenh::MultiIndexMultiplicity_t<ResultMultiIndex>::eval(m))
                                          / static_cast<Scalar_>(Tenh::Factorial_t<Other_Degree + DEGREE_>::V);
        }

        return result;
    }

    const SymDual Coefficients() const
    {
        return m_coefficients;
    }

    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    Tenh::PreallocatedArray_t<Scalar_ const,DIMENSION> as_array () const
    {
        return Tenh::PreallocatedArray_t<Scalar_ const,DIMENSION>(reinterpret_cast<Scalar_ const *>(&m_coefficients), Tenh::DONT_CHECK_POINTER);
    }
    // NOTE: the PreallocatedArray_t returned from this is valid only as long as
    // this object is alive -- this is effectively a shallow copy.
    Tenh::PreallocatedArray_t<Scalar_,DIMENSION> as_array ()
    {
        return Tenh::PreallocatedArray_t<Scalar_,DIMENSION>(reinterpret_cast<Scalar_ *>(&m_coefficients), Tenh::DONT_CHECK_POINTER);
    }

private:
    SymDual m_coefficients;

    // This should probably be a function in tensor heaven somewhere.
    Sym outer_power_of (Vector const &input) const
    {
        Sym result(Scalar_(1));

        for (typename Sym::ComponentIndex it; it.is_not_at_end(); ++it)
        {
            typename Sym::MultiIndex m = Sym::template bundle_index_map<typename Sym::MultiIndex::IndexTypeList, typename Sym::ComponentIndex>(it);
            for (Uint32 i = 0; i < Sym::MultiIndex::LENGTH; ++i)
            {
                result[it] *= input[typename Vector::ComponentIndex(m.value_of_index(i, Tenh::DONT_CHECK_RANGE))];
            }
            result[it] *= Tenh::Factorial_t<DEGREE_>::V / (Tenh::MultiIndexMultiplicity_t<typename Sym::MultiIndex>::eval(m));
        }
        return result;
    }

    template<Uint32,Uint32,typename,typename> friend struct HomogeneousPolynomial;
    template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar> friend
    std::ostream &operator << (std::ostream &out, HomogeneousPolynomial<DEG,DIM,Id,Scalar> const &m);
};


// Non-member operator overloads. These exist because they must be partially specalized, or because they cannot be written as member operator overloads.
//    scalar operator+
template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar, typename T>
HomogeneousPolynomial<DEG,DIM,Id,Scalar> operator+ (T const &lhs, HomogeneousPolynomial<DEG,DIM,Id,Scalar> const &rhs)
{
    // Use the member version
    return rhs+Scalar(lhs);
}


//    scalar operator*
template<Uint32 DEG, Uint32 DIM, typename Id, typename Scalar, typename T>
HomogeneousPolynomial<DEG,DIM,Id,Scalar> operator* (T const &lhs, HomogeneousPolynomial<DEG,DIM,Id,Scalar> const &rhs)
{
    // Use the member version
    return rhs*Scalar(lhs);
}

#endif // APPLICATIONS_HOMOGENEOUSPOLYNOMIAL_HPP_
