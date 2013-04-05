// ///////////////////////////////////////////////////////////////////////////
// tenh/tensor.hpp by Victor Dods, created 2013/03/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TENSOR_HPP_
#define TENH_TENSOR_HPP_

#include <ostream>

#include "tenh/core.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/index.hpp"
#include "tenh/meta/typelist_utility.hpp"

namespace Tenh {

template <typename FactorTypeList>
struct IndexTypeList_t
{
    typedef TypeList_t<typename FactorTypeList::HeadType::Index,typename IndexTypeList_t<typename FactorTypeList::BodyTypeList>::T> T;
};

template <typename HeadType>
struct IndexTypeList_t<TypeList_t<HeadType> >
{
    typedef TypeList_t<typename HeadType::Index> T;
};

template <>
struct IndexTypeList_t<EmptyTypeList>
{
    typedef EmptyTypeList T;
};

template <typename Derived_, typename FactorTypeList_>
struct Tensor_i
{
    typedef Derived_ Derived;
    typedef FactorTypeList_ FactorTypeList;
    typedef typename IndexTypeList_t<FactorTypeList>::T IndexTypeList;
    typedef CompoundIndex_t<IndexTypeList> CompoundIndex;

    // TODO: could put canonical as_factorX conversions here

    // the two separate head/body template arguments are necessary to disambiguate this method
    // from one that takes a single index (i.e. the index-by-vector-index one).
    template <typename IndexTypeListHeadType, typename IndexTypeListBodyTypeList>
    ExpressionTemplate_IndexedObject_t<
        Derived,
        TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList>,
        typename SummedIndexTypeList_t<TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList> >::T,
        FORCE_CONST> operator () (TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList> const &) const
    {
        typedef TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList> ArgumentIndexTypeList;
        // ensure the length matches IndexTypeList
        Lvd::Meta::Assert<ArgumentIndexTypeList::LENGTH == IndexTypeList::LENGTH>();
        // make sure that each type in the index list is actually a TypedIndex_t
        AssertThatEachTypeIsATypedIndex_t<ArgumentIndexTypeList>();
        // make sure there are type conversions for all types in the index lists
        compile_time_check_that_there_is_a_type_conversion(ArgumentIndexTypeList(), IndexTypeList());
        return ExpressionTemplate_IndexedObject_t<
            Derived,
            TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList>,
            typename SummedIndexTypeList_t<TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList> >::T,
            FORCE_CONST>(this->as_derived_());
    }
    template <typename IndexTypeListHeadType, typename IndexTypeListBodyTypeList>
    ExpressionTemplate_IndexedObject_t<
        Derived,
        TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList>,
        typename SummedIndexTypeList_t<TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList> >::T,
        DONT_FORCE_CONST> operator () (TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList> const &)
    {
        typedef TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList> ArgumentIndexTypeList;
        // ensure the length matches IndexTypeList
        Lvd::Meta::Assert<ArgumentIndexTypeList::LENGTH == IndexTypeList::LENGTH>();
        // make sure that each type in the index list is actually a TypedIndex_t
        AssertThatEachTypeIsATypedIndex_t<ArgumentIndexTypeList>();
        // make sure there are type conversions for all types in the index lists
        compile_time_check_that_there_is_a_type_conversion(ArgumentIndexTypeList(), IndexTypeList());
        return ExpressionTemplate_IndexedObject_t<
            Derived,
            TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList>,
            typename SummedIndexTypeList_t<TypeList_t<IndexTypeListHeadType,IndexTypeListBodyTypeList> >::T,
            DONT_FORCE_CONST>(this->as_derived_());
    }
    
private:

    // accessor as Derived type
    Derived const &as_derived_ () const { return *static_cast<Derived const *>(this); }
    Derived &as_derived_ () { return *static_cast<Derived *>(this); }
};

// TODO: put operator<< here?

} // end of namespace Tenh

#endif // TENH_TENSOR_HPP_
