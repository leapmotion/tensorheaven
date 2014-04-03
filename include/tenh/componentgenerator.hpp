// ///////////////////////////////////////////////////////////////////////////
// tenh/componentgenerator.hpp by Victor Dods, created 2013/12/23
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_COMPONENTGENERATOR_HPP_
#define TENH_COMPONENTGENERATOR_HPP_

#include "tenh/core.hpp"

#include "tenh/componentindex.hpp"
#include "tenh/meta/typestringof.hpp"

namespace Tenh {

// ///////////////////////////////////////////////////////////////////////////
// ComponentGenerator_t
// ///////////////////////////////////////////////////////////////////////////

// gives a way to provide a "named" component generator for use in ProceduralArray_t
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

    static std::string type_as_string (bool verbose)
    {
        return "ComponentGenerator_t<" + type_string_of<Component_>() + ','
                                       + FORMAT(COMPONENT_COUNT_) + ','
                                       //+ FORMAT(evaluate) + ',' // TODO (?): print function pointer somehow
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
struct IsComponentGenerator_t<ComponentGenerator_t<Component_,COMPONENT_COUNT_,evaluator_,Id_>>
{
  static bool const V = true;
private:
    IsComponentGenerator_t();
};

// NOTE: you may need to provide a template specialization for DualOf_f<ComponentGenerator_t<...>>

// ///////////////////////////////////////////////////////////////////////////
// some convenience component generators
// ///////////////////////////////////////////////////////////////////////////

template <typename Component_, Uint32 COMPONENT_COUNT_, Sint32 VALUE_>
Component_ constant_component_generator_evaluator (ComponentIndex_t<COMPONENT_COUNT_> const &)
{
    return Component_(VALUE_);
}

template <Sint32 VALUE_>
struct ComponentGenerator_Constant { static std::string type_as_string (bool verbose) { return "ComponentGenerator_Constant<" + FORMAT(VALUE_) + '>'; } };

template <typename Component_, Uint32 COMPONENT_COUNT_, Sint32 VALUE_>
struct ComponentGenerator_Constant_f
{
    typedef ComponentGenerator_t<Component_,
                                 COMPONENT_COUNT_,
                                 constant_component_generator_evaluator<Component_,COMPONENT_COUNT_,VALUE_>,
                                 ComponentGenerator_Constant<VALUE_>> T;
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
struct ComponentGenerator_Characteristic { static std::string type_as_string (bool verbose) { return "ComponentGenerator_Characteristic<" + FORMAT(INDEX_) + '>'; } };

template <typename Component_, Uint32 COMPONENT_COUNT_, Uint32 INDEX_>
struct ComponentGenerator_Characteristic_f
{
    typedef ComponentGenerator_t<Component_,
                                 COMPONENT_COUNT_,
                                 characteristic_component_generator_evaluator<Component_,COMPONENT_COUNT_,INDEX_>,
                                 ComponentGenerator_Characteristic<INDEX_>> T;
private:
    ComponentGenerator_Characteristic_f () { }
};

} // end of namespace Tenh

#endif // TENH_COMPONENTGENERATOR_HPP_
