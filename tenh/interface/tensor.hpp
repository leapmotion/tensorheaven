// ///////////////////////////////////////////////////////////////////////////
// tenh/interface/tensor.hpp by Victor Dods, created 2013/04/05
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_INTERFACE_TENSOR_HPP_
#define TENH_INTERFACE_TENSOR_HPP_

#include <ostream>

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/interface/vector.hpp"

namespace Tenh {

template <typename FactorTypeList>
struct FactorComponentIndexTypeList_t
{
    typedef TypeList_t<ComponentIndex_t<FactorTypeList::HeadType::DIM>,
                       typename FactorComponentIndexTypeList_t<typename FactorTypeList::BodyTypeList>::T> T;
};

template <typename HeadType>
struct FactorComponentIndexTypeList_t<TypeList_t<HeadType> >
{
    typedef TypeList_t<ComponentIndex_t<HeadType::DIM> > T;
};

template <>
struct FactorComponentIndexTypeList_t<EmptyTypeList>
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
    typedef typename Parent_Vector_i::ComponentIndex ComponentIndex;

    typedef TensorProductOfBasedVectorSpaces_ TensorProductOfBasedVectorSpaces;
    typedef typename TensorProductOfBasedVectorSpaces::FactorTypeList FactorTypeList;
    typedef MultiIndex_t<typename FactorComponentIndexTypeList_t<FactorTypeList>::T> MultiIndex;
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
        typename DimIndexTypeListOf_t<FactorTypeList,
                                      TypeList_t<AbstractIndexTypeListHeadType,
                                                 AbstractIndexTypeListBodyTypeList> >::T,
        typename SummedIndexTypeList_t<typename DimIndexTypeListOf_t<FactorTypeList,
                                                                     TypeList_t<AbstractIndexTypeListHeadType,
                                                                                AbstractIndexTypeListBodyTypeList> >::T>::T,
        FORCE_CONST,
        CHECK_FOR_ALIASING> operator () (TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> const &) const
    {
        typedef TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> ArgumentAbstractIndexTypeList;
        STATIC_ASSERT(EachTypeIsAnAbstractIndex_c<ArgumentAbstractIndexTypeList>::V, EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
        STATIC_ASSERT((ArgumentAbstractIndexTypeList::LENGTH == DEGREE), ARGUMENT_LENGTH_MUST_EQUAL_DEGREE);
        return ExpressionTemplate_IndexedObject_t<
            Derived,
            typename DimIndexTypeListOf_t<FactorTypeList,
                                          TypeList_t<AbstractIndexTypeListHeadType,
                                                     AbstractIndexTypeListBodyTypeList> >::T,
            typename SummedIndexTypeList_t<typename DimIndexTypeListOf_t<FactorTypeList,
                                                                         TypeList_t<AbstractIndexTypeListHeadType,
                                                                                    AbstractIndexTypeListBodyTypeList> >::T>::T,
            FORCE_CONST,
            CHECK_FOR_ALIASING>(as_derived());
    }
    template <typename AbstractIndexTypeListHeadType, typename AbstractIndexTypeListBodyTypeList>
    ExpressionTemplate_IndexedObject_t<
        Derived,
        typename DimIndexTypeListOf_t<FactorTypeList,
                                      TypeList_t<AbstractIndexTypeListHeadType,
                                                 AbstractIndexTypeListBodyTypeList> >::T,
        typename SummedIndexTypeList_t<typename DimIndexTypeListOf_t<FactorTypeList,
                                                                     TypeList_t<AbstractIndexTypeListHeadType,
                                                                                AbstractIndexTypeListBodyTypeList> >::T>::T,
        DONT_FORCE_CONST,
        CHECK_FOR_ALIASING> operator () (TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> const &)
    {
        typedef TypeList_t<AbstractIndexTypeListHeadType,AbstractIndexTypeListBodyTypeList> ArgumentAbstractIndexTypeList;
        STATIC_ASSERT(EachTypeIsAnAbstractIndex_c<ArgumentAbstractIndexTypeList>::V, EACH_TYPE_MUST_BE_ABSTRACT_INDEX);
        STATIC_ASSERT((ArgumentAbstractIndexTypeList::LENGTH == DEGREE), ARGUMENT_LENGTH_MUST_EQUAL_DEGREE);
        return ExpressionTemplate_IndexedObject_t<
            Derived,
            typename DimIndexTypeListOf_t<FactorTypeList,
                                          TypeList_t<AbstractIndexTypeListHeadType,
                                                     AbstractIndexTypeListBodyTypeList> >::T,
            typename SummedIndexTypeList_t<typename DimIndexTypeListOf_t<FactorTypeList,
                                                                         TypeList_t<AbstractIndexTypeListHeadType,
                                                                                    AbstractIndexTypeListBodyTypeList> >::T>::T,
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

    typedef ComponentIndex_t<Factor1::DIM> Factor1ComponentIndex;
    typedef ComponentIndex_t<Factor2::DIM> Factor2ComponentIndex;

    out << "\n[";
    for (Factor1ComponentIndex i; i.is_not_at_end(); ++i)
    {
        if (i.value() != 0)
            out << ' ';
        out << "[ ";
        for (Factor2ComponentIndex j; j.is_not_at_end(); ++j)
        {
            out.setf(std::ios_base::right);
            out.width(max_component_width);
            out << t[m] << ' ';
            ++m;
        }
        out << ']';
        Factor1ComponentIndex next(i);
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

    typedef ComponentIndex_t<Factor1::DIM> Factor1ComponentIndex;
    typedef ComponentIndex_t<Factor2::DIM> Factor2ComponentIndex;
    typedef ComponentIndex_t<Factor3::DIM> Factor3ComponentIndex;

    out << "\n[";
    for (Factor1ComponentIndex i; i.is_not_at_end(); ++i)
    {
        if (i.value() != 0)
            out << ' ';
        out << '[';
        for (Factor2ComponentIndex j; j.is_not_at_end(); ++j)
        {
            if (j.value() != 0)
                out << "  ";
            out << "[ ";
            for (Factor3ComponentIndex k; k.is_not_at_end(); ++k)
            {
                out.setf(std::ios_base::right);
                out.width(max_component_width);
                out << t[m] << ' ';
                ++m;
            }
            out << ']';
            Factor2ComponentIndex next(j);
            ++next;
            if (next.is_not_at_end())
                out << '\n';
        }
        out << ']';
        Factor1ComponentIndex next(i);
        ++next;
        if (next.is_not_at_end())
            out << "\n\n";
    }
    return out << "]\n";
}

// TODO: implement recursive operator << for arbitrary degree tensors

} // end of namespace Tenh

#endif // TENH_INTERFACE_TENSOR_HPP_
