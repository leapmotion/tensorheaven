// ///////////////////////////////////////////////////////////////////////////
// tenh/immutablearray.hpp by Victor Dods, created 2013/09/11
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMMUTABLEARRAY_HPP_
#define TENH_IMMUTABLEARRAY_HPP_

#include <stdexcept>

#include "tenh/core.hpp"

#include "tenh/interface/array.hpp"

namespace Tenh {

// fixed-length array of a given component type, which must be a POD type
// (the allocation_size_in_bytes and pointer_to_allocation methods require this).
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          Component_ (*component_generator_)(ComponentIndex_t<COMPONENT_COUNT_> const &),
          typename Derived_ = NullType>
struct ImmutableArray_t
    :
    public Array_i<typename Lvd::Meta::If<(Lvd::Meta::TypesAreEqual<Derived_,NullType>::v),
                                          ImmutableArray_t<Component_,COMPONENT_COUNT_,component_generator_,Derived_>,
                                          Derived_>::T,
                   Component_,
                   COMPONENT_COUNT_>
{
    typedef Array_i<typename Lvd::Meta::If<(Lvd::Meta::TypesAreEqual<Derived_,NullType>::v),
                                          ImmutableArray_t<Component_,COMPONENT_COUNT_,component_generator_,Derived_>,
                                          Derived_>::T,
                    Component_,
                    COMPONENT_COUNT_> Parent_Array_i;

    typedef typename Parent_Array_i::Component Component;
    using Parent_Array_i::COMPONENT_COUNT;
    typedef Component_ (*ComponentGenerator)(ComponentIndex_t<COMPONENT_COUNT_> const &);
    static ComponentGenerator const component_generator;// = component_generator_;
    typedef typename Parent_Array_i::ComponentIndex ComponentIndex;

    // no initialization is necessary (or possible), since all components are
    // generated procedurally.
    ImmutableArray_t () { }

    Component operator [] (ComponentIndex const &i) const
    {
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return component_generator(i);
    }

    static std::string type_as_string ()
    {
        return "ImmutableArray_t<" + TypeStringOf_t<Component>::eval() + ',' + AS_STRING(COMPONENT_COUNT) + '>';
    }
};

template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          Component_ (*component_generator_)(ComponentIndex_t<COMPONENT_COUNT_> const &),
          typename Derived_>
typename ImmutableArray_t<Component_,COMPONENT_COUNT_,component_generator_,Derived_>::ComponentGenerator const ImmutableArray_t<Component_,COMPONENT_COUNT_,component_generator_,Derived_>::component_generator = component_generator_;

template <typename T> struct IsImmutableArray_t { static bool const V = false; };
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          Component_ (*component_generator_)(ComponentIndex_t<COMPONENT_COUNT_> const &),
          typename Derived_>
struct IsImmutableArray_t<ImmutableArray_t<Component_,COMPONENT_COUNT_,component_generator_,Derived_> > { static bool const V = true; };

template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          Component_ (*component_generator_)(ComponentIndex_t<COMPONENT_COUNT_> const &),
          typename Derived_>
struct IsArray_i<ImmutableArray_t<Component_,COMPONENT_COUNT_,component_generator_,Derived_> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_IMMUTABLEARRAY_HPP_
