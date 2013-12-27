// ///////////////////////////////////////////////////////////////////////////
// tenh/proceduralarray.hpp by Victor Dods, created 2013/09/11
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_PROCEDURALARRAY_HPP_
#define TENH_PROCEDURALARRAY_HPP_

#include <stdexcept>

#include "tenh/core.hpp"

#include "tenh/componentgenerator.hpp"
#include "tenh/interface/array.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// ProceduralArray_t
// ///////////////////////////////////////////////////////////////////////////

// fixed-length array of a given component type, which must be a POD type
// (the allocation_size_in_bytes and pointer_to_allocation methods require this).
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          typename ComponentGenerator_,
          typename Derived_ = NullType>
struct ProceduralArray_t
    :
    public Array_i<typename DerivedType_f<Derived_,ProceduralArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> >::T,
                   Component_,
                   COMPONENT_COUNT_,
                   COMPONENTS_ARE_PROCEDURAL>
{
    enum { STATIC_ASSERT_IN_ENUM(IsComponentGenerator_t<ComponentGenerator_>::V, MUST_BE_COMPONENT_GENERATOR) };

    typedef Array_i<typename DerivedType_f<Derived_,ProceduralArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> >::T,
                    Component_,
                    COMPONENT_COUNT_,
                    COMPONENTS_ARE_PROCEDURAL> Parent_Array_i;

    typedef typename Parent_Array_i::Component Component;
    using Parent_Array_i::COMPONENT_COUNT;
    using Parent_Array_i::COMPONENT_QUALIFIER;
    typedef typename Parent_Array_i::ComponentIndex ComponentIndex;
    typedef typename Parent_Array_i::ComponentAccessConstReturnType ComponentAccessConstReturnType;
    typedef typename Parent_Array_i::ComponentAccessNonConstReturnType ComponentAccessNonConstReturnType;
    typedef typename Parent_Array_i::QualifiedComponent QualifiedComponent;

    typedef ComponentGenerator_ ComponentGenerator;

    // no initialization is necessary (or possible), since all components are
    // generated procedurally.
    ProceduralArray_t () { }
    explicit ProceduralArray_t (WithoutInitialization const &) { }

    Component operator [] (ComponentIndex const &i) const
    {
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return ComponentGenerator_::evaluate(i);
    }
    Component operator [] (ComponentIndex const &i)
    {
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return ComponentGenerator_::evaluate(i);
    }

    // vacuous versions of these methods which don't really belong here, but are
    // really just used so that stuff that uses ArrayStorage_f doesn't have to
    // jump through template hoops to declare the right thing.
    Uint32 allocation_size_in_bytes () const { return 0; }
    Component_ const *pointer_to_allocation () const { return NULL; }
    QualifiedComponent *pointer_to_allocation () { return NULL; }
    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const { return false; }

    static std::string type_as_string ()
    {
        // TODO: figure out how to provide stringified type info for the component_generator_ template param.
        return "ProceduralArray_t<" + type_string_of<Component_>() + ','
                                   + AS_STRING(COMPONENT_COUNT_) + ','
                                   + type_string_of<ComponentGenerator_>() + '>';
    }
};

template <typename T> struct IsProceduralArray_t
{
    static bool const V = false;
private:
    IsProceduralArray_t();
};
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          typename ComponentGenerator_,
          typename Derived_>
struct IsProceduralArray_t<ProceduralArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> >
{
    static bool const V = true;
private:
    IsProceduralArray_t();
};

template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          typename ComponentGenerator_,
          typename Derived_>
struct IsArray_i<ProceduralArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> >
{
    static bool const V = true;
private:
    IsArray_i();
};

} // end of namespace Tenh

#endif // TENH_PROCEDURALARRAY_HPP_
