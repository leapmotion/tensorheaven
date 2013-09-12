// ///////////////////////////////////////////////////////////////////////////
// tenh/immutablearray.hpp by Victor Dods, created 2013/09/11
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_IMMUTABLEARRAY_HPP_
#define TENH_IMMUTABLEARRAY_HPP_

#include <stdexcept>

#include "tenh/core.hpp"

#include "tenh/interface/array.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// ComponentGenerator_t
// ///////////////////////////////////////////////////////////////////////////

// gives a way to provide a "named" component generator for use in ImmutableArray_t
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          Component_ (*evaluator_)(ComponentIndex_t<COMPONENT_COUNT_> const &),
          typename Id_>
struct ComponentGenerator_t
{
    typedef Component_ Component;
    static Uint32 const COMPONENT_COUNT = COMPONENT_COUNT_;
    typedef ComponentIndex_t<COMPONENT_COUNT_> ComponentIndex;
    typedef Component_ (*Evaluator)(ComponentIndex const &);
    static Evaluator const evaluate;

    static std::string type_as_string ()
    {
        return "ComponentGenerator_t<" + TypeStringOf_t<Component_>::eval() + ','
                                       + AS_STRING(COMPONENT_COUNT_) + ','
                                       //+ AS_STRING(evaluate) + ',' // TODO (?): print function pointer somehow
                                       + TypeStringOf_t<Id_>::eval() + '>';
    }
};

template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          Component_ (*evaluator_)(ComponentIndex_t<COMPONENT_COUNT_> const &),
          typename Id_>
typename ComponentGenerator_t<Component_,COMPONENT_COUNT_,evaluator_,Id_>::Evaluator const ComponentGenerator_t<Component_,COMPONENT_COUNT_,evaluator_,Id_>::evaluate = evaluator_;

template <typename T> struct IsComponentGenerator_t { static bool const V = false; };
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          Component_ (*evaluator_)(ComponentIndex_t<COMPONENT_COUNT_> const &),
          typename Id_>
struct IsComponentGenerator_t<ComponentGenerator_t<Component_,COMPONENT_COUNT_,evaluator_,Id_> > { static bool const V = true; };

// NOTE: you may need to provide a template specialization for DualOf_f<ComponentGenerator_t<...> >

// ///////////////////////////////////////////////////////////////////////////
// ImmutableArray_t
// ///////////////////////////////////////////////////////////////////////////

// fixed-length array of a given component type, which must be a POD type
// (the allocation_size_in_bytes and pointer_to_allocation methods require this).
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          typename ComponentGenerator_,
          typename Derived_ = NullType>
struct ImmutableArray_t
    :
    public Array_i<typename Lvd::Meta::If<(Lvd::Meta::TypesAreEqual<Derived_,NullType>::v),
                                          ImmutableArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_>,
                                          Derived_>::T,
                   Component_,
                   COMPONENT_COUNT_>
{
    enum { STATIC_ASSERT_IN_ENUM(IsComponentGenerator_t<ComponentGenerator_>::V, MUST_BE_COMPONENT_GENERATOR) };

    typedef Array_i<typename Lvd::Meta::If<(Lvd::Meta::TypesAreEqual<Derived_,NullType>::v),
                                          ImmutableArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_>,
                                          Derived_>::T,
                    Component_,
                    COMPONENT_COUNT_> Parent_Array_i;

    typedef typename Parent_Array_i::Component Component;
    using Parent_Array_i::COMPONENT_COUNT;
    typedef typename Parent_Array_i::ComponentIndex ComponentIndex;
    typedef ComponentGenerator_ ComponentGenerator;

    // no initialization is necessary (or possible), since all components are
    // generated procedurally.
    ImmutableArray_t () { }

    Component operator [] (ComponentIndex const &i) const
    {
        assert(i.is_not_at_end() && "you used ComponentIndex_t(x, DONT_RANGE_CHECK) inappropriately");
        return ComponentGenerator_::evaluate(i);
    }

    static std::string type_as_string ()
    {
        // TODO: figure out how to provide stringified type info for the component_generator_ template param.
        return "ImmutableArray_t<" + TypeStringOf_t<Component_>::eval() + ',' 
                                   + AS_STRING(COMPONENT_COUNT_) + ','
                                   + TypeStringOf_t<ComponentGenerator_>::eval() + '>';
    }
};

template <typename T> struct IsImmutableArray_t { static bool const V = false; };
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          typename ComponentGenerator_,
          typename Derived_>
struct IsImmutableArray_t<ImmutableArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> > { static bool const V = true; };

template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          typename ComponentGenerator_,
          typename Derived_>
struct IsArray_i<ImmutableArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> > { static bool const V = true; };

} // end of namespace Tenh

#endif // TENH_IMMUTABLEARRAY_HPP_
