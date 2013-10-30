// ///////////////////////////////////////////////////////////////////////////
// tenh/implementation/directsum.hpp by Ted Nitz, created 2013/10/24
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMPLEMENTATION_DIRECT_SUM_HPP_
#define TENH_IMPLEMENTATION_DIRECT_SUM_HPP_

#include "tenh/core.hpp"

#include "tenh/conceptual/directsum.hpp"
#include "tenh/implementation/implementationof.hpp"
#include "tenh/interface/vector.hpp"
#include "tenh/meta/tuple.hpp"

namespace Tenh {

template <typename SummandTypeList_, Uint32 N>
struct OffsetForComponent_f
{
private:
    enum { STATIC_ASSERT_IN_ENUM((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END) };
public:
    static const Uint32 V = DimensionOf_f<typename SummandTypeList_::HeadType>::V + OffsetForComponent_f<typename SummandTypeList_::BodyTypeList, N-1>::V;
};

template <typename SummandTypeList_>
struct OffsetForComponent_f<SummandTypeList_,0>
{
    static const Uint32 V = 0;
};

template <typename SummandTypeList_, typename Scalar_, typename UseArrayType_>
struct ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_>
    :
    public ImplementationOf_t<typename UniqueBasedVectorSpaceStructureOf_f<DirectSumOfBasedVectorSpaces_c<SummandTypeList_> >::T,Scalar_,UseArrayType_>
{
    typedef ImplementationOf_t<typename UniqueBasedVectorSpaceStructureOf_f<DirectSumOfBasedVectorSpaces_c<SummandTypeList_> >::T,Scalar_,UseArrayType_> Parent_Implementation;
    typedef typename Parent_Implementation::Parent_Vector_i Parent_Vector_i;
    typedef typename Parent_Implementation::Parent_Array_i Parent_Array_i;

    typedef DirectSumOfBasedVectorSpaces_c<SummandTypeList_> Concept;
    typedef UseArrayType_ UseArrayType;
    typedef typename Parent_Implementation::Derived Derived;
    typedef typename Parent_Implementation::Scalar Scalar;
    typedef typename Parent_Implementation::BasedVectorSpace BasedVectorSpace;
    using Parent_Implementation::DIM;
    typedef typename Parent_Implementation::ComponentIndex ComponentIndex;
    typedef typename Parent_Implementation::MultiIndex MultiIndex;

    using Parent_Implementation::COMPONENTS_ARE_IMMUTABLE;
    typedef typename Parent_Implementation::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_Implementation::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;

    typedef typename DualOf_f<ImplementationOf_t>::T Dual; // relies on the template specialization below

    explicit ImplementationOf_t (WithoutInitialization const &w) : Parent_Implementation(w) { }

    // only use these if UseMemberArray is specified

    // probably only useful for zero element (because this is basis-dependent)
    explicit ImplementationOf_t (Scalar const &fill_with)
        :
        Parent_Implementation(fill_with)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    ImplementationOf_t (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x)
        :
        Parent_Implementation(x.as_member_array())
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UseMemberArray);
    }

    // only use these if UsePreallocatedArray is specified

    explicit ImplementationOf_t (Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    ImplementationOf_t (Scalar const &fill_with,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(fill_with, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }
    // this is the tuple-based constructor
    template <typename HeadType_, typename BodyTypeList_>
    ImplementationOf_t (List_t<TypeList_t<HeadType_,BodyTypeList_> > const &x,
                        Scalar *pointer_to_allocation, bool check_pointer = CHECK_POINTER)
        :
        Parent_Implementation(x, pointer_to_allocation, check_pointer)
    {
        STATIC_ASSERT_TYPES_ARE_EQUAL(UseArrayType_,UsePreallocatedArray);
    }

    // only use this if UseImmutableArray_t<...> is specified or if the vector space is 0-dimensional
    ImplementationOf_t ()
        :
        Parent_Implementation(WithoutInitialization()) // sort of meaningless constructor
    {
        STATIC_ASSERT(IsUseImmutableArray_f<UseArrayType_>::V || DIM == 0, MUST_BE_USE_IMMUTABLE_ARRAY_OR_BE_ZERO_DIMENSIONAL);
    }

    using Parent_Implementation::as_derived;
    using Parent_Implementation::operator[];
    using Parent_Implementation::allocation_size_in_bytes;
    using Parent_Implementation::pointer_to_allocation;

    template <Uint32 N>
    struct ElementType_f
    {
    private:
        enum { STATIC_ASSERT_IN_ENUM((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END) };
    public:
        typedef ImplementationOf_t<typename Element_f<SummandTypeList_, N>::T,
                                   Scalar_,
                                   typename If_f<IsUseImmutableArray_f<UseArrayType_>::V,
                                                 UseArrayType_,
                                                 UsePreallocatedArray>::T >
                T;
    };

    template <Uint32 N>
    typename ElementType_f<N>::T el()
    {
        STATIC_ASSERT((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END);
        return typename ElementType_f<N>::T(pointer_to_allocation() + OffsetForComponent_f<SummandTypeList_,N>::V);
    }

    template <Uint32 N>
    typename ElementType_f<N>::T const el() const
    {
        STATIC_ASSERT((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END);
        return typename ElementType_f<N>::T(pointer_to_allocation() + OffsetForComponent_f<SummandTypeList_,N>::V);
    }

    typename ElementType_f<0>::T el(const Uint32 &n)
    {
        STATIC_ASSERT(TypeListIsUniform_t<SummandTypeList_>::V, TYPELIST_MUST_BE_UNIFORM);
        return typename ElementType_f<0>::T(pointer_to_allocation() + DimensionOf_f<typename SummandTypeList_::HeadType>::V * n);
    }

    typename ElementType_f<0>::T const el(const Uint32 &n) const
    {
        STATIC_ASSERT(TypeListIsUniform_t<SummandTypeList_>::V, TYPELIST_MUST_BE_UNIFORM);
        return typename ElementType_f<0>::T(pointer_to_allocation() + DimensionOf_f<typename SummandTypeList_::HeadType>::V * n);
    }

    // These versions of el<...> are intended to allow use like el<n>(i) rather than the more clunky el<n>()(i) to get an indexed expression.
    // Unfortunately the indexed expression type stores a reference to the ImplementationOf_t internally, and in this code that object is
    // a temporary, and so the behavior is undefined.
    // template <Uint32 N, AbstractIndexSymbol SYMBOL_>
    // typename ElementType_f<N>::T::template IndexedExpressionNonConstType_f<SYMBOL_>::T el(AbstractIndex_c<SYMBOL_> const & i)
    // {
    //     STATIC_ASSERT((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END);
    //     return typename ElementType_f<N>::T(pointer_to_allocation() + OffsetForComponent_f<SummandTypeList_,N>::V)(i);
    // }
    //
    // template <Uint32 N, AbstractIndexSymbol SYMBOL_>
    // typename ElementType_f<N>::T::template IndexedExpressionConstType_f<SYMBOL_>::T el(AbstractIndex_c<SYMBOL_> const & i) const
    // {
    //     STATIC_ASSERT((SummandTypeList_::LENGTH > N), ATTEMPTED_ACCESS_PAST_LIST_END);
    //     return typename ElementType_f<N>::T(pointer_to_allocation() + OffsetForComponent_f<SummandTypeList_,N>::V)(i);
    // }

    static std::string type_as_string ()
    {
        return "ImplementationOf_t<" + type_string_of<DirectSumOfBasedVectorSpaces_c<SummandTypeList_> >() + ','
                                     + type_string_of<Scalar>() + ','
                                     + type_string_of<UseArrayType_>() + '>';
    }
};

template <typename SummandTypeList_, typename Scalar_, typename UseArrayType_>
struct DualOf_f<ImplementationOf_t<DirectSumOfBasedVectorSpaces_c<SummandTypeList_>,Scalar_,UseArrayType_> >
{
    typedef ImplementationOf_t<typename DualOf_f<DirectSumOfBasedVectorSpaces_c<SummandTypeList_> >::T,Scalar_,typename DualOf_f<UseArrayType_>::T> T;
};

} // end of namespace Tenh

#endif // TENH_IMPLEMENTATION_DIRECT_SUM_HPP_
