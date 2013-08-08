// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/tensor.hpp by Victor Dods, created 2013/04/05
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_TENSOR_HPP_
#define TENH_INTERFACE_TENSOR_HPP_

#include <ostream>

#include "tenh/core.hpp"

#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/interface/vector.hpp"

namespace Tenh {

template <typename FactorTypeList>
struct FactorIndexTypeList_t
{
    typedef TypeList_t<typename FactorTypeList::HeadType::Index,typename FactorIndexTypeList_t<typename FactorTypeList::BodyTypeList>::T> T;
};

template <typename HeadType>
struct FactorIndexTypeList_t<TypeList_t<HeadType> >
{
    typedef TypeList_t<typename HeadType::Index> T;
};

template <>
struct FactorIndexTypeList_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};

// compile-time interface for a tensor product class.  TensorProductOfBasedVectorSpaces_
// should be a TensorProductOfBasedVectorSpaces_c type.
template <typename Derived_, typename Scalar_, typename TensorProductOfBasedVectorSpaces_>
struct Tensor_i : public Vector_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_>
{
    enum
    {
        STATIC_ASSERT_IN_ENUM((!Lvd::Meta::TypesAreEqual<Derived_,NullType>::v), DERIVED_MUST_NOT_BE_NULL_TYPE),
        //STATIC_ASSERT_IN_ENUM((FactorTypeList_::LENGTH > 0), FACTOR_TYPE_LIST_MUST_BE_NONEMPTY) // NOTE: deprecate this, since 0-order tensors should be allowed
        STATIC_ASSERT_IN_ENUM(IsATensorProductOfBasedVectorSpaces_c<TensorProductOfBasedVectorSpaces_>::V, MUST_BE_TENSOR_PRODUCT_OF_BASED_VECTOR_SPACES)
    };

    typedef Vector_i<Derived_,Scalar_,TensorProductOfBasedVectorSpaces_> Parent_Vector_i;
    typedef typename Parent_Vector_i::Derived Derived;
    typedef typename Parent_Vector_i::Scalar Scalar;
    typedef typename Parent_Vector_i::BasedVectorSpace BasedVectorSpace;
    using Parent_Vector_i::DIM;
    typedef typename Parent_Vector_i::Basis Basis;
    typedef typename Parent_Vector_i::Index Index;

    typedef TensorProductOfBasedVectorSpaces_ TensorProductOfBasedVectorSpaces;
    typedef typename TensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;
    //typedef typename FactorIndexTypeList_t<FactorTypeList>::T FactorIndexTypeList;
    typedef MultiIndex_t<FactorIndexTypeList> MultiIndex;
    // this is not the "fully expanded" degree, but the number of [what you could think of
    // as "parenthesized"] factors that formed this tensor product type.
    static Uint32 const DEGREE = FactorTypeList::LENGTH;
    static bool const IS_TENSOR_I = true; // TODO: deprecate this in favor of IsATensor_i<...>

    // TODO: could put canonical as_factorX conversions here

    static Uint32 degree () { return DEGREE; }

    using Parent_Vector_i::dim;
    using Parent_Vector_i::as_derived;
    using Parent_Vector_i::operator[];
    Scalar operator [] (MultiIndex const &m) const { return as_derived().Derived::operator[](m); }
    Scalar operator [] (MultiIndex const &m) { return as_derived().Derived::operator[](m); }

    using Parent_Vector_i::operator();
    // the two separate head/body template arguments are necessary to disambiguate this method
    // from one that takes a single index (i.e. the index-by-vector-index one).
    template <typename AbstractIndexTypeListHeadType, typename AbstractIndexTypeListBodyTypeList>
    ExpressionTemplate_IndexedObject_t<
        Derived,
        TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList>,
        typename SummedIndexTypeList_t<TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> >::T,
        FORCE_CONST,
        CHECK_FOR_ALIASING> operator () (TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> const &) const
    {
        typedef TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> ArgumentAbstractIndexTypeList;
        STATIC_ASSERT(EachTypeIsAnAbstractIndex_c<ArgumentAbstractIndexTypeList>::V, EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
        STATIC_ASSERT((ArgumentAbstractIndexTypeList::LENGTH == DEGREE), ARGUMENT_LENGTH_MUST_EQUAL_DEGREE);
        /*
        // make sure that each type in the index list is actually a TypedIndex_t
        AssertThatEachTypeIsATypedIndex_t<ArgumentIndexTypeList>();
        // make sure there are type conversions for all types in the index lists
        compile_time_check_that_there_is_a_type_conversion(ArgumentAbstractIndexTypeList(), FactorIndexTypeList());
        // TEMP KLUDGE - forbit contraction with this tensor if its basis is StandardEuclideanBasis,
        // because its components are then not necessarily those that should be contracted with --
        // they would be some tensor-implementation-dependent scalar multiples of the components,
        // and this particular capability is too complicated to implement currently.
        STATIC_ASSERT((!Lvd::Meta::TypesAreEqual<Basis,StandardEuclideanBasis>::v), CANT_CONTRACT_WITH_EUCLIDEANLY_EMBEDDED_TENSOR);
        */
        return ExpressionTemplate_IndexedObject_t<
            Derived,
            TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList>,
            typename SummedIndexTypeList_t<TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> >::T,
            FORCE_CONST,
            CHECK_FOR_ALIASING>(as_derived());
    }
    template <typename AbstractIndexTypeListHeadType, typename AbstractIndexTypeListBodyTypeList>
    ExpressionTemplate_IndexedObject_t<
        Derived,
        TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList>,
        typename SummedIndexTypeList_t<TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> >::T,
        DONT_FORCE_CONST,
        CHECK_FOR_ALIASING> operator () (TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> const &)
    {
        typedef TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> ArgumentAbstractIndexTypeList;
        STATIC_ASSERT(EachTypeIsAnAbstractIndex_c<ArgumentAbstractIndexTypeList>::V, EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
        STATIC_ASSERT((ArgumentAbstractIndexTypeList::LENGTH == DEGREE), ARGUMENT_LENGTH_MUST_EQUAL_DEGREE);
        /*
        // make sure that each type in the index list is actually a TypedIndex_t
        AssertThatEachTypeIsATypedIndex_t<ArgumentIndexTypeList>();
        // make sure there are type conversions for all types in the index lists
        compile_time_check_that_there_is_a_type_conversion(ArgumentIndexTypeList(), FactorIndexTypeList());
        // TEMP KLUDGE - forbit contraction with this tensor if its basis is StandardEuclideanBasis,
        // because its components are then not necessarily those that should be contracted with --
        // they would be some tensor-implementation-dependent scalar multiples of the components,
        // and this particular capability is too complicated to implement currently.
        STATIC_ASSERT((!Lvd::Meta::TypesAreEqual<Basis,StandardEuclideanBasis>::v), CANT_CONTRACT_WITH_EUCLIDEANLY_EMBEDDED_TENSOR);
        */
        return ExpressionTemplate_IndexedObject_t<
            Derived,
            TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList>,
            typename SummedIndexTypeList_t<TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> >::T,
            DONT_FORCE_CONST,
            CHECK_FOR_ALIASING>(as_derived());
    }

    using Parent_Vector_i::component_is_immutable_zero;
    using Parent_Vector_i::scalar_factor_for_component;
    using Parent_Vector_i::vector_index_of;

    static std::string type_as_string ()
    {
        return "Tensor_i<" + TypeStringOf_t<Derived>::eval() + ',' 
                           + TypeStringOf_t<Scalar>::eval() + ',' 
                           + TypeStringOf_t<TensorProductOfBasedVectorSpaces>::eval() + '>';
    }
};

// TODO: figure out the maximum width any particular element takes up (or do this by column)
// and use iomanip::setw to do really nice formatting that isn't the hackyness of using tabs.

// specialization for 1-tensors
template <typename Derived, typename Scalar, typename Factor1>
std::ostream &operator << (std::ostream &out, Tensor_i<Derived,Scalar,TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1> > > const &t)
{
    typedef Tensor_i<Derived,Scalar,TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1> > > Tensor;

    // determine the max size that a component takes up
    Uint32 max_component_width = 0;
    for (typename Tensor::MultiIndex m; m.is_not_at_end(); ++m)
    {
        std::ostringstream sout;
        sout << t[m];
        if (sout.str().length() > max_component_width)
            max_component_width = sout.str().length();
    }

    out << "[ ";
    for (typename Tensor::MultiIndex m; m.is_not_at_end(); ++m)
    {
        out.setf(std::ios_base::right);
        out.width(max_component_width);
        out << t[m] << ' ';
    }
    return out << ']';
}

// specialization for 2-tensors
template <typename Derived, typename Scalar, typename Factor1, typename Factor2>
std::ostream &operator << (std::ostream &out, Tensor_i<Derived,Scalar,TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1,TypeList_t<Factor2> > > > const &t)
{
    typedef Tensor_i<Derived,Scalar,TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1,TypeList_t<Factor2> > > > Tensor;

    // determine the max size that a component takes up
    Uint32 max_component_width = 0;
    for (typename Tensor::MultiIndex m; m.is_not_at_end(); ++m)
    {
        std::ostringstream sout;
        sout << t[m];
        if (sout.str().length() > max_component_width)
            max_component_width = sout.str().length();
    }

    typename Tensor::MultiIndex m;
    if (m.is_at_end())
        return out << "\n[[ ]]\n";

    out << "\n[";
    for (typename Factor1::Index i; i.is_not_at_end(); ++i)
    {
        if (i.value() != 0)
            out << ' ';
        out << "[ ";
        for (typename Factor2::Index j; j.is_not_at_end(); ++j)
        {
            out.setf(std::ios_base::right);
            out.width(max_component_width);
            out << t[m] << ' ';
            ++m;
        }
        out << ']';
        typename Factor1::Index next(i);
        ++next;
        if (next.is_not_at_end())
            out << '\n';
    }
    return out << "]\n";
}

// specialization for 3-tensors
template <typename Derived, typename Scalar, typename Factor1, typename Factor2, typename Factor3>
std::ostream &operator << (std::ostream &out, Tensor_i<Derived,Scalar,TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1,TypeList_t<Factor2,TypeList_t<Factor3> > > > > const &t)
{
    typedef Tensor_i<Derived,Scalar,TensorProductOfBasedVectorSpaces_c<TypeList_t<Factor1,TypeList_t<Factor2,TypeList_t<Factor3> > > > > Tensor;

    // determine the max size that a component takes up
    Uint32 max_component_width = 0;
    for (typename Tensor::MultiIndex m; m.is_not_at_end(); ++m)
    {
        std::ostringstream sout;
        sout << t[m];
        if (sout.str().length() > max_component_width)
            max_component_width = sout.str().length();
    }

    typename Tensor::MultiIndex m;
    if (m.is_at_end())
        return out << "\n[[[ ]]]\n";

    out << "\n[";
    for (typename Factor1::Index i; i.is_not_at_end(); ++i)
    {
        if (i.value() != 0)
            out << ' ';
        out << '[';
        for (typename Factor2::Index j; j.is_not_at_end(); ++j)
        {
            if (j.value() != 0)
                out << "  ";
            out << "[ ";
            for (typename Factor3::Index k; k.is_not_at_end(); ++k)
            {
                out.setf(std::ios_base::right);
                out.width(max_component_width);
                out << t[m] << ' ';
                ++m;
            }
            out << ']';
            typename Factor2::Index next(j);
            ++next;
            if (next.is_not_at_end())
                out << '\n';
        }
        out << ']';
        typename Factor1::Index next(i);
        ++next;
        if (next.is_not_at_end())
            out << "\n\n";
    }
    return out << "]\n";
}

// TODO: implement recursive operator << for arbitrary degree tensors

} // end of namespace Tenh

#endif // TENH_INTERFACE_TENSOR_HPP_
