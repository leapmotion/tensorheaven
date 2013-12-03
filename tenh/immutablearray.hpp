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
        return "ComponentGenerator_t<" + type_string_of<Component_>() + ','
                                       + AS_STRING(COMPONENT_COUNT_) + ','
                                       //+ AS_STRING(evaluate) + ',' // TODO (?): print function pointer somehow
                                       + type_string_of<Id_>() + '>';
    }
};

template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          Component_ (*evaluator_)(ComponentIndex_t<COMPONENT_COUNT_> const &),
          typename Id_>
typename ComponentGenerator_t<Component_,COMPONENT_COUNT_,evaluator_,Id_>::Evaluator const ComponentGenerator_t<Component_,COMPONENT_COUNT_,evaluator_,Id_>::evaluate = evaluator_;

template <typename T> struct IsComponentGenerator_t
{
    static bool const V = false;
private:
    IsComponentGenerator_t();
};
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          Component_ (*evaluator_)(ComponentIndex_t<COMPONENT_COUNT_> const &),
          typename Id_>
struct IsComponentGenerator_t<ComponentGenerator_t<Component_,COMPONENT_COUNT_,evaluator_,Id_> >
{
  static bool const V = true;
private:
    IsComponentGenerator_t();
};

// NOTE: you may need to provide a template specialization for DualOf_f<ComponentGenerator_t<...> >

// ///////////////////////////////////////////////////////////////////////////
// some convenience component generators
// ///////////////////////////////////////////////////////////////////////////

template <typename Component_, Uint32 COMPONENT_COUNT_, Sint32 VALUE_>
Component_ constant_component_generator_evaluator (ComponentIndex_t<COMPONENT_COUNT_> const &)
{
    return Component_(VALUE_);
}

template <Sint32 VALUE_>
struct ComponentGenerator_Constant { static std::string type_as_string () { return "ComponentGenerator_Constant<" + AS_STRING(VALUE_) + '>'; } };

template <typename Component_, Uint32 COMPONENT_COUNT_, Sint32 VALUE_>
struct ComponentGenerator_Constant_f
{
    typedef ComponentGenerator_t<Component_,
                                 COMPONENT_COUNT_,
                                 constant_component_generator_evaluator<Component_,COMPONENT_COUNT_,VALUE_>,
                                 ComponentGenerator_Constant<VALUE_> > T;
private:
    ComponentGenerator_Constant_f () { }
};

// ///////////////////////////////////////////////////////////////////////////

// for basis vectors
template <typename Component_, Uint32 COMPONENT_COUNT_, Uint32 INDEX_>
Component_ characteristic_component_generator_evaluator (ComponentIndex_t<COMPONENT_COUNT_> const &i)
{
    return i.value() == INDEX_ ? Component_(1) : Component_(0);
}

template <Uint32 INDEX_>
struct ComponentGenerator_Characteristic { static std::string type_as_string () { return "ComponentGenerator_Characteristic<" + AS_STRING(INDEX_) + '>'; } };

template <typename Component_, Uint32 COMPONENT_COUNT_, Uint32 INDEX_>
struct ComponentGenerator_Characteristic_f
{
    typedef ComponentGenerator_t<Component_,
                                 COMPONENT_COUNT_,
                                 characteristic_component_generator_evaluator<Component_,COMPONENT_COUNT_,INDEX_>,
                                 ComponentGenerator_Characteristic<INDEX_> > T;
private:
    ComponentGenerator_Characteristic_f () { }
};

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
    public Array_i<typename DerivedType_f<Derived_,ImmutableArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> >::T,
                   Component_,
                   COMPONENT_COUNT_,
                   IMMUTABLE_COMPONENTS>
{
    enum { STATIC_ASSERT_IN_ENUM(IsComponentGenerator_t<ComponentGenerator_>::V, MUST_BE_COMPONENT_GENERATOR) };

    typedef Array_i<typename DerivedType_f<Derived_,ImmutableArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> >::T,
                    Component_,
                    COMPONENT_COUNT_,
                    IMMUTABLE_COMPONENTS> Parent_Array_i;

    typedef typename Parent_Array_i::Component Component;
    using Parent_Array_i::COMPONENT_COUNT;
    typedef typename Parent_Array_i::ComponentIndex ComponentIndex;
    typedef ComponentGenerator_ ComponentGenerator;

    // no initialization is necessary (or possible), since all components are
    // generated procedurally.
    ImmutableArray_t () { }
    explicit ImmutableArray_t (WithoutInitialization const &) { }

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
    Component const *pointer_to_allocation () const { return NULL; }
    Component *pointer_to_allocation () { return NULL; }

    static std::string type_as_string ()
    {
        // TODO: figure out how to provide stringified type info for the component_generator_ template param.
        return "ImmutableArray_t<" + type_string_of<Component_>() + ','
                                   + AS_STRING(COMPONENT_COUNT_) + ','
                                   + type_string_of<ComponentGenerator_>() + '>';
    }
};

template <typename T> struct IsImmutableArray_t
{
    static bool const V = false;
private:
    IsImmutableArray_t();
};
template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          typename ComponentGenerator_,
          typename Derived_>
struct IsImmutableArray_t<ImmutableArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> >
{
    static bool const V = true;
private:
    IsImmutableArray_t();
};

template <typename Component_,
          Uint32 COMPONENT_COUNT_,
          typename ComponentGenerator_,
          typename Derived_>
struct IsArray_i<ImmutableArray_t<Component_,COMPONENT_COUNT_,ComponentGenerator_,Derived_> >
{
    static bool const V = true;
private:
    IsArray_i();
};

} // end of namespace Tenh

#endif // TENH_IMMUTABLEARRAY_HPP_
