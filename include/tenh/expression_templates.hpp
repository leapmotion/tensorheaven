// ///////////////////////////////////////////////////////////////////////////
// tenh/expression_templates.hpp by Victor Dods, created 2013/03/13
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_EXPRESSION_TEMPLATES_HPP_
#define TENH_EXPRESSION_TEMPLATES_HPP_

#include "tenh/core.hpp"

#include <stdexcept>

#include "tenh/expression_templates_utility.hpp"
#include "tenh/interface/expressiontemplate.hpp"
#include "tenh/reindex.hpp"

namespace Tenh {

// ////////////////////////////////////////////////////////////////////////////
// expression-template-generation (making ETs from vectors/tensors)
// ////////////////////////////////////////////////////////////////////////////

enum class ForceConst : bool { TRUE = true, FALSE = false };

inline std::ostream &operator << (std::ostream &out, ForceConst force_const)
{
    return out << "ForceConst::" << (bool(force_const) ? "TRUE" : "FALSE");
}

enum class CheckForAliasing : bool { TRUE = true, FALSE = false };

inline std::ostream &operator << (std::ostream &out, CheckForAliasing check_for_aliasing)
{
    return out << "CheckForAliasing::" << (bool(check_for_aliasing) ? "TRUE" : "FALSE");
}

// this is the "const" version of an indexed tensor expression (it has summed indices, so it doesn't make sense to assign to it)
template <typename Object,
          typename FactorTyple, // this is necessary because the factor type depends on if the thing is being indexed as a vector or tensor
          typename DimIndexTyple,
          typename SummedDimIndexTyple_,
          ForceConst FORCE_CONST_,
          CheckForAliasing CHECK_FOR_ALIASING_,
          typename Derived_ = NullType>
struct ExpressionTemplate_IndexedObject_t
    :
    public ExpressionTemplate_i<typename DerivedType_f<Derived_,
                                                       ExpressionTemplate_IndexedObject_t<Object,
                                                                                          FactorTyple,
                                                                                          DimIndexTyple,
                                                                                          SummedDimIndexTyple_,
                                                                                          FORCE_CONST_,
                                                                                          CHECK_FOR_ALIASING_,
                                                                                          Derived_>>::T,
                                typename Object::Scalar,
                                typename FreeFactorTyple_f<FactorTyple,DimIndexTyple>::T,
                                typename FreeIndexTyple_f<DimIndexTyple>::T,
                                SummedDimIndexTyple_>
{
    // TODO: assert that Object is NOT an expression template (it could be e.g. an IndexBundle_t though)
    // TODO: assert that FactorTyple is a Typle of BasedVectorSpace_c types.
    static_assert(EachTypeSatisfies_f<DimIndexTyple,IsDimIndex_e>::V, "DimIndexTyple must be a Typle_t of DimIndex_t types");
    static_assert(EachTypeSatisfies_f<SummedDimIndexTyple_,IsDimIndex_e>::V, "SummedIndexTyple must be a Typle_t of DimIndex_t types");

    typedef ExpressionTemplate_i<typename DerivedType_f<Derived_,
                                                        ExpressionTemplate_IndexedObject_t<Object,
                                                                                           FactorTyple,
                                                                                           DimIndexTyple,
                                                                                           SummedDimIndexTyple_,
                                                                                           FORCE_CONST_,
                                                                                           CHECK_FOR_ALIASING_,
                                                                                           Derived_>>::T,
                                 typename Object::Scalar,
                                 typename FreeFactorTyple_f<FactorTyple,DimIndexTyple>::T,
                                 typename FreeIndexTyple_f<DimIndexTyple>::T,
                                 SummedDimIndexTyple_> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTyple FreeFactorTyple;
    typedef typename Parent::FreeDimIndexTyple FreeDimIndexTyple;
    typedef typename Parent::UsedDimIndexTyple UsedDimIndexTyple;
    typedef typename Parent::MultiIndex MultiIndex;
    typedef SummedDimIndexTyple_ SummedDimIndexTyple;

    // object must be a "terminal" instance (e.g. an ImplementationOf_t type) which will live
    // at least as long as the relevant indexed expression.
    ExpressionTemplate_IndexedObject_t (Object const &object) : m_object(object) { }

    operator Scalar () const
    {
        static_assert(Length_f<FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return operator[](MultiIndex());
    }

    // read-only, because it doesn't make sense to assign to an expression which is a summation.
    Scalar operator [] (MultiIndex const &m) const
    {
        return UnarySummation_t<Object,DimIndexTyple,SummedDimIndexTyple>::eval(m_object, m);
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_object.overlaps_memory_range(ptr, range);
    }

    Object const &object () const { return m_object; }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_IndexedObject_t<" + type_string_of<Object>() + ','
                                                     + type_string_of<FactorTyple>() + ','
                                                     + type_string_of<DimIndexTyple>() + ','
                                                     + type_string_of<SummedDimIndexTyple_>() + ','
                                                     + FORMAT(FORCE_CONST_) + ','
                                                     + FORMAT(CHECK_FOR_ALIASING_) + ','
                                                     + type_string_of<Derived_>() + '>';
    }

private:

    void operator = (ExpressionTemplate_IndexedObject_t const &);

    Object const &m_object;
};

template <typename Object_,
          typename FactorTyple_,
          typename DimIndexTyple_,
          typename SummedDimIndexTyple_,
          ForceConst FORCE_CONST_,
          CheckForAliasing CHECK_FOR_ALIASING_,
          typename Derived_>
struct IsExpressionTemplate_f<ExpressionTemplate_IndexedObject_t<Object_,
                                                                 FactorTyple_,
                                                                 DimIndexTyple_,
                                                                 SummedDimIndexTyple_,
                                                                 FORCE_CONST_,
                                                                 CHECK_FOR_ALIASING_,
                                                                 Derived_>>
{
    static bool const V = true;
private:
    IsExpressionTemplate_f();
};

// this is the "non-const" version of an indexed tensor expression (it has no summed indices, so it makes sense to assign to it)
template <typename Object,
          typename FactorTyple,
          typename DimIndexTyple,
          CheckForAliasing CHECK_FOR_ALIASING_,
          typename Derived_>
struct ExpressionTemplate_IndexedObject_t<Object,FactorTyple,DimIndexTyple,Typle_t<>,ForceConst::FALSE,CHECK_FOR_ALIASING_,Derived_>
    :
    public ExpressionTemplate_i<ExpressionTemplate_IndexedObject_t<Object,FactorTyple,DimIndexTyple,Typle_t<>,ForceConst::FALSE,CHECK_FOR_ALIASING_,Derived_>,
                                typename Object::Scalar,
                                typename FreeFactorTyple_f<FactorTyple,DimIndexTyple>::T,
                                typename FreeIndexTyple_f<DimIndexTyple>::T,
                                Typle_t<>>
{
    typedef ExpressionTemplate_i<ExpressionTemplate_IndexedObject_t<Object,FactorTyple,DimIndexTyple,Typle_t<>,ForceConst::FALSE,CHECK_FOR_ALIASING_,Derived_>,
                                 typename Object::Scalar,
                                 typename FreeFactorTyple_f<FactorTyple,DimIndexTyple>::T,
                                 typename FreeIndexTyple_f<DimIndexTyple>::T,
                                 Typle_t<>> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTyple FreeFactorTyple;
    typedef typename Parent::FreeDimIndexTyple FreeDimIndexTyple;
    typedef typename Parent::UsedDimIndexTyple UsedDimIndexTyple;
    typedef typename Parent::MultiIndex MultiIndex;

    // object must be a "terminal" instance (e.g. an ImplementationOf_t type) which will live
    // at least as long as the relevant indexed expression.
    ExpressionTemplate_IndexedObject_t (Object &object) : m_object(object) { }

    // call this on the left-hand side (LHS) of an indexed assignment to avoid the run-time
    // aliasing check.  this should only be done when the human can guarantee that there is
    // no memory aliasing in the assignment (where the same memory location is being referenced
    // on both the LHS and RHS of the assignment, therefore causing the non-atomically
    // evaluated result to be implementation-dependent and incorrect).
    ExpressionTemplate_IndexedObject_t<Object,FactorTyple,DimIndexTyple,Typle_t<>,ForceConst::FALSE,CheckForAliasing::FALSE,Derived_> no_alias ()
    {
        return ExpressionTemplate_IndexedObject_t<Object,FactorTyple,DimIndexTyple,Typle_t<>,ForceConst::FALSE,CheckForAliasing::FALSE,Derived_>(m_object);
    }

    operator Scalar () const
    {
        static_assert(Length_f<FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return operator[](MultiIndex());
    }

    // read-only, because it doesn't necessarily make sense to assign to an expression
    // template -- the expression may be a product or some such, where each component
    // is not an L-value.
    Scalar operator [] (MultiIndex const &m) const { return m_object[m]; }

    // for some dumb reason, the compiler needed a non-templatized assignment operator for the exact matching type
    void operator = (ExpressionTemplate_IndexedObject_t const &right_operand)
    {
        // if right and left operands' m_tensor references are the same, this is a no-op
        if (&right_operand.m_object == &m_object)
            return;

        // can't do a memcpy because the right operand might be an procedural vector and have no memory
        for (typename Object::ComponentIndex i; i.is_not_at_end(); ++i)
            m_object[i] = right_operand[typename ExpressionTemplate_IndexedObject_t::MultiIndex(i)];
    }

    void operator += (ExpressionTemplate_IndexedObject_t const &right_operand)
    {
        for (typename Object::ComponentIndex i; i.is_not_at_end(); ++i)
            m_object[i] += right_operand[typename ExpressionTemplate_IndexedObject_t::MultiIndex(i)];
    }

    void operator -= (ExpressionTemplate_IndexedObject_t const &right_operand)
    {
        for (typename Object::ComponentIndex i; i.is_not_at_end(); ++i)
            m_object[i] -= right_operand[typename ExpressionTemplate_IndexedObject_t::MultiIndex(i)];
    }

    template <typename RightOperand>
    void operator = (RightOperand const &right_operand)
    {
        static_assert(IsExpressionTemplate_f<RightOperand>::V, "RightOperand must be an ExpressionTemplate_i");
        static_assert(TypesAreEqual_f<Scalar,typename RightOperand::Scalar>::V, "operand scalar types must be equal");
        static_assert(AreEqualAsSets_f<FreeDimIndexTyple,typename RightOperand::FreeDimIndexTyple>::V, "operands must have same free indices");
        static_assert(!ContainsDuplicates_f<FreeDimIndexTyple>::V, "left operand must have no duplicate free indices");
        static_assert(!ContainsDuplicates_f<typename RightOperand::FreeDimIndexTyple>::V, "right operand must have no duplicate free indices");

        // check for aliasing (where source and destination memory overlap)
        Uint8 const *ptr = reinterpret_cast<Uint8 const *>(m_object.pointer_to_allocation());
        Uint32 range = m_object.allocation_size_in_bytes();
        if (bool(CHECK_FOR_ALIASING_) && right_operand.overlaps_memory_range(ptr, range))
            throw std::invalid_argument("aliased tensor assignment (source and destination memory overlap) -- see eval() and no_alias()");

        typedef MultiIndexMap_t<FreeDimIndexTyple,typename RightOperand::FreeDimIndexTyple> RightOperandIndexMap;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;

        // component-wise assignment via the free index type.
        for (MultiIndex m; m.is_not_at_end(); ++m)
            m_object[m] = right_operand[right_operand_index_map(m)];
    }

    template <typename RightOperand>
    void operator += (RightOperand const &right_operand)
    {
        static_assert(IsExpressionTemplate_f<RightOperand>::V, "RightOperand must be an ExpressionTemplate_i");
        static_assert(TypesAreEqual_f<Scalar,typename RightOperand::Scalar>::V, "operand scalar types must be equal");
        static_assert(AreEqualAsSets_f<FreeDimIndexTyple,typename RightOperand::FreeDimIndexTyple>::V, "operands must have same free indices");
        static_assert(!ContainsDuplicates_f<FreeDimIndexTyple>::V, "left operand must have no duplicate free indices");
        static_assert(!ContainsDuplicates_f<typename RightOperand::FreeDimIndexTyple>::V, "right operand must have no duplicate free indices");

        // check for aliasing (where source and destination memory overlap)
        Uint8 const *ptr = reinterpret_cast<Uint8 const *>(m_object.pointer_to_allocation());
        Uint32 range = m_object.allocation_size_in_bytes();
        if (bool(CHECK_FOR_ALIASING_) && right_operand.overlaps_memory_range(ptr, range))
            throw std::invalid_argument("aliased tensor assignment (source and destination memory overlap) -- see eval() and no_alias()");

        typedef MultiIndexMap_t<FreeDimIndexTyple,typename RightOperand::FreeDimIndexTyple> RightOperandIndexMap;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;

        // component-wise assignment via the free index type.
        for (MultiIndex m; m.is_not_at_end(); ++m)
            m_object[m] += right_operand[right_operand_index_map(m)];
    }

    template <typename RightOperand>
    void operator -= (RightOperand const &right_operand)
    {
        static_assert(IsExpressionTemplate_f<RightOperand>::V, "RightOperand must be an ExpressionTemplate_i");
        static_assert(TypesAreEqual_f<Scalar,typename RightOperand::Scalar>::V, "operand scalar types must be equal");
        static_assert(AreEqualAsSets_f<FreeDimIndexTyple,typename RightOperand::FreeDimIndexTyple>::V, "operands must have same free indices");
        static_assert(!ContainsDuplicates_f<FreeDimIndexTyple>::V, "left operand must have no duplicate free indices");
        static_assert(!ContainsDuplicates_f<typename RightOperand::FreeDimIndexTyple>::V, "right operand must have no duplicate free indices");

        // check for aliasing (where source and destination memory overlap)
        Uint8 const *ptr = reinterpret_cast<Uint8 const *>(m_object.pointer_to_allocation());
        Uint32 range = m_object.allocation_size_in_bytes();
        if (bool(CHECK_FOR_ALIASING_) && right_operand.overlaps_memory_range(ptr, range))
            throw std::invalid_argument("aliased tensor assignment (source and destination memory overlap) -- see eval() and no_alias()");

        typedef MultiIndexMap_t<FreeDimIndexTyple,typename RightOperand::FreeDimIndexTyple> RightOperandIndexMap;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;

        // component-wise assignment via the free index type.
        for (MultiIndex m; m.is_not_at_end(); ++m)
            m_object[m] -= right_operand[right_operand_index_map(m)];
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_object.overlaps_memory_range(ptr, range);
    }

    Object &object () const { return m_object; }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_IndexedObject_t<" + type_string_of<Object>() + ','
                                                     + type_string_of<FactorTyple>() + ','
                                                     + type_string_of<DimIndexTyple>() + ','
                                                     + type_string_of<Typle_t<>>() + ','
                                                     + FORMAT(ForceConst::FALSE) + ','
                                                     + FORMAT(CHECK_FOR_ALIASING_) + ','
                                                     + type_string_of<Derived_>() + '>';
    }

private:

    Object &m_object;
};

// ////////////////////////////////////////////////////////////////////////////
// addition of expression templates
// ////////////////////////////////////////////////////////////////////////////

// NOTE: if this is ever subclassed, then it will be necessary to change the inheritance to pass in the Derived type
template <typename LeftOperand, typename RightOperand, char OPERATOR>
struct ExpressionTemplate_Addition_t
    :
    public ExpressionTemplate_i<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>,
                                typename LeftOperand::Scalar,
                                typename LeftOperand::FreeFactorTyple,
                                typename LeftOperand::FreeDimIndexTyple,
                                Typle_t<>>
{
    typedef ExpressionTemplate_i<ExpressionTemplate_Addition_t<LeftOperand,RightOperand,OPERATOR>,
                                 typename LeftOperand::Scalar,
                                 typename LeftOperand::FreeFactorTyple,
                                 typename LeftOperand::FreeDimIndexTyple,
                                 Typle_t<>> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTyple FreeFactorTyple;
    typedef typename Parent::FreeDimIndexTyple FreeDimIndexTyple;
    typedef typename Parent::UsedDimIndexTyple UsedDimIndexTyple;
    typedef typename Parent::MultiIndex MultiIndex;

private:

    typedef typename Zip_f<Typle_t<typename LeftOperand::FreeFactorTyple,
                                   typename LeftOperand::FreeDimIndexTyple>>::T LeftFactorAndIndexTyple;
    typedef typename Zip_f<Typle_t<typename RightOperand::FreeFactorTyple,
                                   typename RightOperand::FreeDimIndexTyple>>::T RightFactorAndIndexTyple;

    static_assert(IsExpressionTemplate_f<LeftOperand>::V, "LeftOperand must be an ExpressionTemplate_i");
    static_assert(IsExpressionTemplate_f<RightOperand>::V, "RightOperand must be an ExpressionTemplate_i");
    static_assert(TypesAreEqual_f<typename LeftOperand::Scalar,typename RightOperand::Scalar>::V, "operands must have same scalar type");
    static_assert(AreEqualAsSets_f<LeftFactorAndIndexTyple,RightFactorAndIndexTyple>::V, "operands must have same indices and factor types");
    static_assert(AreEqualAsSets_f<typename LeftOperand::FreeDimIndexTyple,typename RightOperand::FreeDimIndexTyple>::V, "operands must have same free indices");
    static_assert(!ContainsDuplicates_f<typename LeftOperand::FreeDimIndexTyple>::V, "LeftOperand must not have duplicate free indices");
    static_assert(!ContainsDuplicates_f<typename RightOperand::FreeDimIndexTyple>::V, "RightOperand must not have duplicate free indices");
    static_assert(OPERATOR == '+' || OPERATOR == '-', "operator must be '+' or '-'");

public:

    ExpressionTemplate_Addition_t (LeftOperand const &left_operand, RightOperand const &right_operand)
        :
        m_left_operand(left_operand),
        m_right_operand(right_operand)
    { }

    operator Scalar () const
    {
        static_assert(Length_f<FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        if (OPERATOR == '+')
            return m_left_operand.operator Scalar() + m_right_operand.operator Scalar();
        else // OPERATOR == '-'
            return m_left_operand.operator Scalar() - m_right_operand.operator Scalar();
    }

    // read-only, because it doesn't make sense to assign to an expression which is a summation.
    Scalar operator [] (MultiIndex const &m) const
    {
        typedef MultiIndexMap_t<FreeDimIndexTyple,typename LeftOperand::FreeDimIndexTyple> LeftOperandIndexMap;
        typedef MultiIndexMap_t<FreeDimIndexTyple,typename RightOperand::FreeDimIndexTyple> RightOperandIndexMap;
        typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
        if (OPERATOR == '+')
            return m_left_operand[left_operand_index_map(m)] + m_right_operand[right_operand_index_map(m)];
        else // OPERATOR == '-'
            return m_left_operand[left_operand_index_map(m)] - m_right_operand[right_operand_index_map(m)];
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_left_operand.overlaps_memory_range(ptr, range) || m_right_operand.overlaps_memory_range(ptr, range);
    }

    LeftOperand const &left_operand () const { return m_left_operand; }
    RightOperand const &right_operand () const { return m_right_operand; }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_Addition_t<" + type_string_of<LeftOperand>() + ','
                                                + type_string_of<RightOperand>() + ','
                                                + '\'' + FORMAT(OPERATOR) + '\'' + '>';
    }

private:

    void operator = (ExpressionTemplate_Addition_t const &);

    LeftOperand m_left_operand;
    RightOperand m_right_operand;
};

template <typename LeftOperand_, typename RightOperand_, char OPERATOR_>
struct IsExpressionTemplate_f<ExpressionTemplate_Addition_t<LeftOperand_,RightOperand_,OPERATOR_>>
{
    static bool const V = true;
private:
    IsExpressionTemplate_f();
};

// ////////////////////////////////////////////////////////////////////////////
// scalar multiplication and division of expression templates
// ////////////////////////////////////////////////////////////////////////////

// it is assumed that scalar multiplication is commutative.
// OPERATOR can be '*' or '/'.
// NOTE: if this is ever subclassed, then it will be necessary to change the inheritance to pass in the Derived type
template <typename Operand, typename Scalar_, char OPERATOR>
struct ExpressionTemplate_ScalarMultiplication_t
    :
    public ExpressionTemplate_i<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>,
                                typename Operand::Scalar,
                                typename Operand::FreeFactorTyple,
                                typename Operand::FreeDimIndexTyple,
                                typename Operand::UsedDimIndexTyple>
{
    typedef ExpressionTemplate_i<ExpressionTemplate_ScalarMultiplication_t<Operand,Scalar_,OPERATOR>,
                                 typename Operand::Scalar,
                                 typename Operand::FreeFactorTyple,
                                 typename Operand::FreeDimIndexTyple,
                                 typename Operand::UsedDimIndexTyple> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTyple FreeFactorTyple;
    typedef typename Parent::FreeDimIndexTyple FreeDimIndexTyple;
    typedef typename Parent::UsedDimIndexTyple UsedDimIndexTyple;
    typedef typename Parent::MultiIndex MultiIndex;

    static_assert(IsExpressionTemplate_f<Operand>::V, "Operand must be an ExpressionTemplate_i");
    static_assert(TypesAreEqual_f<typename Operand::Scalar,Scalar_>::V, "operand scalar type must match Scalar_");
    static_assert(OPERATOR == '*' || OPERATOR == '/', "operator must be '*' or '/'");

    ExpressionTemplate_ScalarMultiplication_t (Operand const &operand, Scalar scalar_operand)
        :
        m_operand(operand),
        m_scalar_operand(scalar_operand)
    {
        // TODO: should there be a runtime check here if OPERATOR is '/' and scalar_operand is zero (or close to zero)?
    }

    operator Scalar () const
    {
        static_assert(Length_f<FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return operator[](MultiIndex());
    }

    Scalar operator [] (MultiIndex const &m) const
    {
        if (OPERATOR == '*')
            return m_operand[m] * m_scalar_operand;
        else
            return m_operand[m] / m_scalar_operand;
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_operand.overlaps_memory_range(ptr, range);
    }

    Operand const &operand () const { return m_operand; }
    Scalar const &scalar_operand () const { return m_scalar_operand; }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_ScalarMultiplication_t<" + type_string_of<Operand>() + ','
                                                            + type_string_of<Scalar>() + ','
                                                            + '\'' + FORMAT(OPERATOR) + '\'' + '>';
    }

private:

    void operator = (ExpressionTemplate_ScalarMultiplication_t const &);

    Operand m_operand;
    Scalar m_scalar_operand;
};

template <typename Operand_, typename Scalar_, char OPERATOR_>
struct IsExpressionTemplate_f<ExpressionTemplate_ScalarMultiplication_t<Operand_,Scalar_,OPERATOR_>>
{
    static bool const V = true;
private:
    IsExpressionTemplate_f();
};

// ////////////////////////////////////////////////////////////////////////////
// multiplication of expression templates (tensor product and contraction)
// ////////////////////////////////////////////////////////////////////////////

// TODO: there is an issue to think about: while it is totally valid to do
// u(i)*v(j)*w(j) (this is an outer product contracted with a vector), the
// expression v(j)*w(j) can be computed first and factored out of the whole
// thing, instead of needing to be multiplied out for each access of the i index.
// this may be somewhat difficult to do, as it would require searching the
// expression template AST for such contractions and restructuring the AST.
// NOTE: if this is ever subclassed, then it will be necessary to change the 
// inheritance to pass in the Derived type
template <typename LeftOperand, typename RightOperand>
struct ExpressionTemplate_Multiplication_t
    :
    public ExpressionTemplate_i<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>,
                                typename LeftOperand::Scalar,
                                typename FreeFactorTypleOfMultiplication_f<LeftOperand,RightOperand>::T,
                                typename FreeDimIndexTypleOfMultiplication_f<LeftOperand,RightOperand>::T,
                                typename UsedDimIndexTypleOfMultiplication_f<LeftOperand,RightOperand>::T>
{
    typedef ExpressionTemplate_i<ExpressionTemplate_Multiplication_t<LeftOperand,RightOperand>,
                                 typename LeftOperand::Scalar,
                                 typename FreeFactorTypleOfMultiplication_f<LeftOperand,RightOperand>::T,
                                 typename FreeDimIndexTypleOfMultiplication_f<LeftOperand,RightOperand>::T,
                                 typename UsedDimIndexTypleOfMultiplication_f<LeftOperand,RightOperand>::T> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTyple FreeFactorTyple;
    typedef typename Parent::FreeDimIndexTyple FreeDimIndexTyple;
    typedef typename Parent::UsedDimIndexTyple UsedDimIndexTyple;
    typedef typename Parent::MultiIndex MultiIndex;

    typedef typename SummedDimIndexTypleOfMultiplication_f<LeftOperand,RightOperand>::T SummedDimIndexTyple;

    // TODO: check that the summed indices from each operand have no indices in common
    // though technically this is unnecessary, because the summed indices are "private"
    // to each contraction, so this is really for the human's benefit, not getting
    // confused by multiple repeated indices that have nothing to do with each other.
    static_assert(IsExpressionTemplate_f<LeftOperand>::V, "LeftOperand must be an ExpressionTemplate_i");
    static_assert(IsExpressionTemplate_f<RightOperand>::V, "RightOperand must be an ExpressionTemplate_i");
    static_assert(TypesAreEqual_f<typename LeftOperand::Scalar,typename RightOperand::Scalar>::V, "operands must have same scalar type");
    static_assert(!HasNontrivialSetIntersection_f<FreeDimIndexTyple,UsedDimIndexTyple>::V, "free and used indices must not overlap");
    // TODO: ensure there are no indices that occur 3+ times (?)

    ExpressionTemplate_Multiplication_t (LeftOperand const &left_operand, RightOperand const &right_operand)
        :
        m_left_operand(left_operand),
        m_right_operand(right_operand)
    { }

    // available ONLY if FreeDimIndexTyple is Typle_t<>
    operator Scalar () const
    {
        static_assert(Length_f<FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return operator[](MultiIndex());
    }

    Scalar operator [] (MultiIndex const &m) const
    {
        return BinarySummation_t<LeftOperand,RightOperand,FreeDimIndexTyple,SummedDimIndexTyple>::eval(m_left_operand, m_right_operand, m);
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_left_operand.overlaps_memory_range(ptr, range) || m_right_operand.overlaps_memory_range(ptr, range);
    }

    LeftOperand const &left_operand () const { return m_left_operand; }
    RightOperand const &right_operand () const { return m_right_operand; }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_Multiplication_t<" + type_string_of<LeftOperand>() + ','
                                                      + type_string_of<RightOperand>() + '>';
    }

private:

    void operator = (ExpressionTemplate_Multiplication_t const &);

    LeftOperand m_left_operand;
    RightOperand m_right_operand;
};

template <typename LeftOperand_, typename RightOperand_>
struct IsExpressionTemplate_f<ExpressionTemplate_Multiplication_t<LeftOperand_,RightOperand_>>
{
    static bool const V = true;
private:
    IsExpressionTemplate_f();
};

// ////////////////////////////////////////////////////////////////////////////
// bundling multiple separate indices into a single vector index (downcasting)
// ////////////////////////////////////////////////////////////////////////////

// TODO: use a metafunction to define the parent class and clean this up
template <typename Operand,
          typename BundleAbstractIndexTyple,
          typename ResultingFactorType,
          typename ResultingAbstractIndexType,
          CheckFactorTypes CHECK_FACTOR_TYPES_>
struct ExpressionTemplate_IndexBundle_t
    :
    public ExpressionTemplate_IndexedObject_t<IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>,
                                              typename IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>::FactorTyple,
                                              typename IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>::DimIndexTyple,
                                              typename SummedIndexTyple_f<typename IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>::DimIndexTyple>::T,
                                              ForceConst::TRUE,
                                              CheckForAliasing::TRUE, // irrelevant value
                                              ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>>
{
    typedef ExpressionTemplate_IndexedObject_t<IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>,
                                               typename IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>::FactorTyple,
                                               typename IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>::DimIndexTyple,
                                               typename SummedIndexTyple_f<typename IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>::DimIndexTyple>::T,
                                               ForceConst::TRUE,
                                               CheckForAliasing::TRUE, // irrelevant value
                                               ExpressionTemplate_IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_>> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTyple FreeFactorTyple;
    typedef typename Parent::FreeDimIndexTyple FreeDimIndexTyple;
    typedef typename Parent::UsedDimIndexTyple UsedDimIndexTyple;
    typedef typename Parent::MultiIndex MultiIndex;
    typedef typename Parent::SummedDimIndexTyple SummedDimIndexTyple;

private:

    typedef IndexBundle_t<Operand,BundleAbstractIndexTyple,ResultingFactorType,ResultingAbstractIndexType,CHECK_FACTOR_TYPES_> IndexBundle;

public:

    // it's not great that m_bundler is being passed to Parent before
    // m_bundler is constructed, but Parent only stores a reference
    // to m_bundler, so it doesn't really matter.
    ExpressionTemplate_IndexBundle_t (Operand const &operand) : Parent(m_bundler), m_bundler(operand) { }

    operator Scalar () const
    {
        static_assert(Length_f<FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return operator[](MultiIndex());
    }

    // read-only, because it doesn't make sense to assign to an index-bundled expression (which is possibly also a summation).
    Scalar operator [] (MultiIndex const &m) const
    {
        return UnarySummation_t<IndexBundle,typename IndexBundle::DimIndexTyple,SummedDimIndexTyple>::eval(m_bundler, m);
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_bundler.overlaps_memory_range(ptr, range);
    }

    Operand const &operand () const { return m_bundler.operand(); }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_IndexBundle_t<" + type_string_of<Operand>() + ','
                                                   + type_string_of<BundleAbstractIndexTyple>() + ','
                                                   + type_string_of<ResultingFactorType>() + ','
                                                   + type_string_of<ResultingAbstractIndexType>() + '>';
    }

private:

    void operator = (ExpressionTemplate_IndexBundle_t const &);

    IndexBundle m_bundler;
};

template <typename Operand_,
          typename BundleAbstractIndexTyple_,
          typename ResultingFactorType_,
          typename ResultingAbstractIndexType_,
          CheckFactorTypes CHECK_FACTOR_TYPES_>
struct IsExpressionTemplate_f<ExpressionTemplate_IndexBundle_t<Operand_,
                                                               BundleAbstractIndexTyple_,
                                                               ResultingFactorType_,
                                                               ResultingAbstractIndexType_,
                                                               CHECK_FACTOR_TYPES_>>
{
    static bool const V = true;
private:
    IsExpressionTemplate_f();
};

// ////////////////////////////////////////////////////////////////////////////
// splitting a single vector index into a multiple separate indices (upcasting)
// ////////////////////////////////////////////////////////////////////////////

// TODO: use a metafunction to define the parent class and clean this up
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexTyple>
struct ExpressionTemplate_IndexSplit_t
    :
    public ExpressionTemplate_IndexedObject_t<IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>,
                                              typename IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>::FactorTyple,
                                              typename IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>::DimIndexTyple,
                                              typename SummedIndexTyple_f<typename IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>::DimIndexTyple>::T,
                                              ForceConst::TRUE,
                                              CheckForAliasing::TRUE, // irrelevant value
                                              ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>>
{
    static_assert(IsAbstractIndex_f<SourceAbstractIndexType>::V, "SourceAbstractIndexType must be an AbstractIndex_c");

    typedef ExpressionTemplate_IndexedObject_t<IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>,
                                               typename IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>::FactorTyple,
                                               typename IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>::DimIndexTyple,
                                               typename SummedIndexTyple_f<typename IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>::DimIndexTyple>::T,
                                               ForceConst::TRUE,
                                               CheckForAliasing::TRUE, // irrelevant value
                                               ExpressionTemplate_IndexSplit_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple>> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTyple FreeFactorTyple;
    typedef typename Parent::FreeDimIndexTyple FreeDimIndexTyple;
    typedef typename Parent::UsedDimIndexTyple UsedDimIndexTyple;
    typedef typename Parent::MultiIndex MultiIndex;
    typedef typename Parent::SummedDimIndexTyple SummedDimIndexTyple;

private:

    typedef IndexSplitter_t<Operand,SourceAbstractIndexType,SplitAbstractIndexTyple> IndexSplitter;

public:

    // it's not great that m_splitter is being passed to Parent before
    // m_splitter is constructed, but Parent only stores a reference
    // to m_splitter, so it doesn't really matter.
    ExpressionTemplate_IndexSplit_t (Operand const &operand) : Parent(m_splitter), m_splitter(operand) { }

    operator Scalar () const
    {
        static_assert(Length_f<FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return operator[](MultiIndex());
    }

    // read-only, because it doesn't make sense to assign to an index-bundled expression (which is possibly also a summation).
    Scalar operator [] (MultiIndex const &m) const
    {
        return UnarySummation_t<IndexSplitter,typename IndexSplitter::DimIndexTyple,SummedDimIndexTyple>::eval(m_splitter, m);
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_splitter.overlaps_memory_range(ptr, range);
    }

    Operand const &operand () const { return m_splitter.operand(); }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_IndexSplit_t<" + type_string_of<Operand>() + ','
                                                  + type_string_of<SourceAbstractIndexType>() + ','
                                                  + type_string_of<SplitAbstractIndexTyple>() + '>';
    }

private:

    void operator = (ExpressionTemplate_IndexSplit_t const &);

    IndexSplitter m_splitter;
};

template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename SplitAbstractIndexTyple_>
struct IsExpressionTemplate_f<ExpressionTemplate_IndexSplit_t<Operand_,
                                                              SourceAbstractIndexType_,
                                                              SplitAbstractIndexTyple_>>
{
    static bool const V = true;
private:
    IsExpressionTemplate_f();
};

// ////////////////////////////////////////////////////////////////////////////
// splitting a single vector index into a a single vector index for larger space
// ////////////////////////////////////////////////////////////////////////////

// TODO: this could probably combine with ExpressionTemplate_IndexSplit_t
// TODO: use a metafunction to define the parent class and clean this up
template <typename Operand, typename SourceAbstractIndexType, typename SplitAbstractIndexType>
struct ExpressionTemplate_IndexSplitToIndex_t
    :
    public ExpressionTemplate_IndexedObject_t<IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>,
                                              typename IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>::FactorTyple,
                                              typename IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>::DimIndexTyple,
                                              typename SummedIndexTyple_f<typename IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>::DimIndexTyple>::T,
                                              ForceConst::TRUE,
                                              CheckForAliasing::TRUE, // irrelevant value
                                              ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>>
{
    static_assert(IsAbstractIndex_f<SourceAbstractIndexType>::V, "SourceAbstractIndexType must be an AbstractIndex_c");

    typedef ExpressionTemplate_IndexedObject_t<IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>,
                                               typename IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>::FactorTyple,
                                               typename IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>::DimIndexTyple,
                                               typename SummedIndexTyple_f<typename IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>::DimIndexTyple>::T,
                                               ForceConst::TRUE,
                                               CheckForAliasing::TRUE, // irrelevant value
                                               ExpressionTemplate_IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType>> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTyple FreeFactorTyple;
    typedef typename Parent::FreeDimIndexTyple FreeDimIndexTyple;
    typedef typename Parent::UsedDimIndexTyple UsedDimIndexTyple;
    typedef typename Parent::MultiIndex MultiIndex;
    typedef typename Parent::SummedDimIndexTyple SummedDimIndexTyple;

private:

    typedef IndexSplitToIndex_t<Operand,SourceAbstractIndexType,SplitAbstractIndexType> IndexSplitToIndex;

public:

    // it's not great that m_splitter is being passed to Parent before
    // m_splitter is constructed, but Parent only stores a reference
    // to m_splitter, so it doesn't really matter.
    ExpressionTemplate_IndexSplitToIndex_t (Operand const &operand) : Parent(m_splitter), m_splitter(operand) { }

    operator Scalar () const
    {
        static_assert(Length_f<FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return operator[](MultiIndex());
    }

    // read-only, because it doesn't make sense to assign to an index-bundled expression (which is possibly also a summation).
    Scalar operator [] (MultiIndex const &m) const
    {
        return UnarySummation_t<IndexSplitToIndex,typename IndexSplitToIndex::DimIndexTyple,SummedDimIndexTyple>::eval(m_splitter, m);
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_splitter.overlaps_memory_range(ptr, range);
    }

    Operand const &operand () const { return m_splitter.operand(); }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_IndexSplitToIndex_t<" + type_string_of<Operand>() + ','
                                                         + type_string_of<SourceAbstractIndexType>() + ','
                                                         + type_string_of<SplitAbstractIndexType>() + '>';
    }

private:

    void operator = (ExpressionTemplate_IndexSplitToIndex_t const &);

    IndexSplitToIndex m_splitter;
};

template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename SplitAbstractIndexTyple_>
struct IsExpressionTemplate_f<ExpressionTemplate_IndexSplitToIndex_t<Operand_,
                                                                     SourceAbstractIndexType_,
                                                                     SplitAbstractIndexTyple_>>
{
    static bool const V = true;
private:
    IsExpressionTemplate_f();
};

// ////////////////////////////////////////////////////////////////////////////
// embedding single vector index into a single vector index for larger space
// ////////////////////////////////////////////////////////////////////////////

// TODO: use a metafunction to define the parent class and clean this up
template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename EmbeddingCodomain_,
          typename EmbeddedAbstractIndexType_,
          typename EmbeddingId_>
struct ExpressionTemplate_IndexEmbed_t
    :
    public ExpressionTemplate_IndexedObject_t<IndexEmbedder_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>,
                                              typename IndexEmbedder_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>::FactorTyple,
                                              typename IndexEmbedder_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>::DimIndexTyple,
                                              typename SummedIndexTyple_f<typename IndexEmbedder_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>::DimIndexTyple>::T,
                                              ForceConst::TRUE,
                                              CheckForAliasing::TRUE, // irrelevant value
                                              ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>>
{
    static_assert(IsAbstractIndex_f<SourceAbstractIndexType_>::V, "SourceAbstractIndexType must be an AbstractIndex_c");

    typedef ExpressionTemplate_IndexedObject_t<IndexEmbedder_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>,
                                               typename IndexEmbedder_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>::FactorTyple,
                                               typename IndexEmbedder_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>::DimIndexTyple,
                                               typename SummedIndexTyple_f<typename IndexEmbedder_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>::DimIndexTyple>::T,
                                               ForceConst::TRUE,
                                               CheckForAliasing::TRUE, // irrelevant value
                                               ExpressionTemplate_IndexEmbed_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_>> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTyple FreeFactorTyple;
    typedef typename Parent::FreeDimIndexTyple FreeDimIndexTyple;
    typedef typename Parent::UsedDimIndexTyple UsedDimIndexTyple;
    typedef typename Parent::MultiIndex MultiIndex;
    typedef typename Parent::SummedDimIndexTyple SummedDimIndexTyple;

private:

    typedef IndexEmbedder_t<Operand_,SourceAbstractIndexType_,EmbeddingCodomain_,EmbeddedAbstractIndexType_,EmbeddingId_> IndexEmbedder;

public:

    // it's not great that m_embedder is being passed to Parent before
    // m_embedder is constructed, but Parent only stores a reference
    // to m_embedder, so it doesn't really matter.
    ExpressionTemplate_IndexEmbed_t (Operand_ const &operand) : Parent(m_embedder), m_embedder(operand) { }

    operator Scalar () const
    {
        static_assert(Length_f<FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return operator[](MultiIndex());
    }

    // read-only, because it doesn't make sense to assign to an index-bundled expression (which is possibly also a summation).
    Scalar operator [] (MultiIndex const &m) const
    {
        return UnarySummation_t<IndexEmbedder,typename IndexEmbedder::DimIndexTyple,SummedDimIndexTyple>::eval(m_embedder, m);
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_embedder.overlaps_memory_range(ptr, range);
    }

    Operand_ const &operand () const { return m_embedder.operand(); }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_IndexEmbed_t<" + type_string_of<Operand_>() + ','
                                                  + type_string_of<SourceAbstractIndexType_>() + ','
                                                  + type_string_of<EmbeddingCodomain_>() + ','
                                                  + type_string_of<EmbeddedAbstractIndexType_>() + ','
                                                  + type_string_of<EmbeddingId_>() + '>';
    }

private:

    void operator = (ExpressionTemplate_IndexEmbed_t const &);

    IndexEmbedder m_embedder;
};

template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename EmbeddingCodomain_,
          typename EmbeddedAbstractIndexType_,
          typename EmbeddingId_>
struct IsExpressionTemplate_f<ExpressionTemplate_IndexEmbed_t<Operand_,
                                                              SourceAbstractIndexType_,
                                                              EmbeddingCodomain_,
                                                              EmbeddedAbstractIndexType_,
                                                              EmbeddingId_>>
{
    static bool const V = true;
private:
    IsExpressionTemplate_f();
};

// ////////////////////////////////////////////////////////////////////////////
// coembedding single vector index into a single vector index for smaller space
// ////////////////////////////////////////////////////////////////////////////

// TODO: use a metafunction to define the parent class and clean this up
template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename CoembeddingCodomain_,
          typename CoembeddedAbstractIndexType_,
          typename EmbeddingId_>
struct ExpressionTemplate_IndexCoembed_t
    :
    public ExpressionTemplate_IndexedObject_t<IndexCoembedder_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_>,
                                              typename IndexCoembedder_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_>::FactorTyple,
                                              typename IndexCoembedder_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_>::DimIndexTyple,
                                              typename SummedIndexTyple_f<typename IndexCoembedder_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_>::DimIndexTyple>::T,
                                              ForceConst::TRUE,
                                              CheckForAliasing::TRUE, // irrelevant value
                                              ExpressionTemplate_IndexCoembed_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_>>
{
    static_assert(IsAbstractIndex_f<SourceAbstractIndexType_>::V, "SourceAbstractIndexType must be an AbstractIndex_c");

    typedef ExpressionTemplate_IndexedObject_t<IndexCoembedder_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_>,
                                               typename IndexCoembedder_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_>::FactorTyple,
                                               typename IndexCoembedder_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_>::DimIndexTyple,
                                               typename SummedIndexTyple_f<typename IndexCoembedder_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_>::DimIndexTyple>::T,
                                               ForceConst::TRUE,
                                               CheckForAliasing::TRUE, // irrelevant value
                                               ExpressionTemplate_IndexCoembed_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_>> Parent;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::FreeFactorTyple FreeFactorTyple;
    typedef typename Parent::FreeDimIndexTyple FreeDimIndexTyple;
    typedef typename Parent::UsedDimIndexTyple UsedDimIndexTyple;
    typedef typename Parent::MultiIndex MultiIndex;
    typedef typename Parent::SummedDimIndexTyple SummedDimIndexTyple;

private:

    typedef IndexCoembedder_t<Operand_,SourceAbstractIndexType_,CoembeddingCodomain_,CoembeddedAbstractIndexType_,EmbeddingId_> IndexCoembedder;

public:

    // it's not great that m_coembedder is being passed to Parent before
    // m_coembedder is constructed, but Parent only stores a reference
    // to m_coembedder, so it doesn't really matter.
    ExpressionTemplate_IndexCoembed_t (Operand_ const &operand) : Parent(m_coembedder), m_coembedder(operand) { }

    operator Scalar () const
    {
        static_assert(Length_f<FreeDimIndexTyple>::V == 0, "only 0-tensors are naturally coerced into scalars");
        return operator[](MultiIndex());
    }

    // read-only, because it doesn't make sense to assign to an index-bundled expression (which is possibly also a summation).
    Scalar operator [] (MultiIndex const &m) const
    {
        return UnarySummation_t<IndexCoembedder,typename IndexCoembedder::DimIndexTyple,SummedDimIndexTyple>::eval(m_coembedder, m);
    }

    bool overlaps_memory_range (Uint8 const *ptr, Uint32 range) const
    {
        return m_coembedder.overlaps_memory_range(ptr, range);
    }

    Operand_ const &operand () const { return m_coembedder.operand(); }

    static std::string type_as_string ()
    {
        return "ExpressionTemplate_IndexCoembed_t<" + type_string_of<Operand_>() + ','
                                                    + type_string_of<SourceAbstractIndexType_>() + ','
                                                    + type_string_of<CoembeddingCodomain_>() + ','
                                                    + type_string_of<CoembeddedAbstractIndexType_>() + ','
                                                    + type_string_of<EmbeddingId_>() + '>';
    }

private:

    void operator = (ExpressionTemplate_IndexCoembed_t const &);

    IndexCoembedder m_coembedder;
};

template <typename Operand_,
          typename SourceAbstractIndexType_,
          typename CoembeddingCodomain_,
          typename CoembeddedAbstractIndexType_,
          typename EmbeddingId_>
struct IsExpressionTemplate_f<ExpressionTemplate_IndexCoembed_t<Operand_,
                                                                SourceAbstractIndexType_,
                                                                CoembeddingCodomain_,
                                                                CoembeddedAbstractIndexType_,
                                                                EmbeddingId_>>
{
    static bool const V = true;
private:
    IsExpressionTemplate_f();
};

// ////////////////////////////////////////////////////////////////////////////
// operator overloads for expression templates
// ////////////////////////////////////////////////////////////////////////////

// expression template addition/subtractions

// addition
template <typename LeftDerived, typename LeftFactorTyple, typename LeftFreeDimIndexTyple, typename LeftUsedDimIndexTyple,
          typename RightDerived, typename RightFactorTyple, typename RightFreeDimIndexTyple, typename RightUsedDimIndexTyple>
ExpressionTemplate_Addition_t<LeftDerived,RightDerived,'+'>
    operator + (ExpressionTemplate_i<LeftDerived,typename LeftDerived::Scalar,LeftFactorTyple,LeftFreeDimIndexTyple,LeftUsedDimIndexTyple> const &left_operand,
                ExpressionTemplate_i<RightDerived,typename RightDerived::Scalar,RightFactorTyple,RightFreeDimIndexTyple,RightUsedDimIndexTyple> const &right_operand)
{
    return ExpressionTemplate_Addition_t<LeftDerived,RightDerived,'+'>(left_operand.as_derived(), right_operand.as_derived());
}

// subtraction
template <typename LeftDerived, typename LeftFactorTyple, typename LeftFreeDimIndexTyple, typename LeftUsedDimIndexTyple,
          typename RightDerived, typename RightFactorTyple, typename RightFreeDimIndexTyple, typename RightUsedDimIndexTyple>
ExpressionTemplate_Addition_t<LeftDerived,RightDerived,'-'>
    operator - (ExpressionTemplate_i<LeftDerived,typename LeftDerived::Scalar,LeftFactorTyple,LeftFreeDimIndexTyple,LeftUsedDimIndexTyple> const &left_operand,
                ExpressionTemplate_i<RightDerived,typename RightDerived::Scalar,RightFactorTyple,RightFreeDimIndexTyple,RightUsedDimIndexTyple> const &right_operand)
{
    return ExpressionTemplate_Addition_t<LeftDerived,RightDerived,'-'>(left_operand.as_derived(), right_operand.as_derived());
}

// scalar multiplication/division, including unary negation (multiplication by -1)

// scalar multiplication on the right
template <typename Derived, typename FactorTyple, typename FreeDimIndexTyple, typename UsedDimIndexTyple>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>
    operator * (ExpressionTemplate_i<Derived,typename Derived::Scalar,FactorTyple,FreeDimIndexTyple,UsedDimIndexTyple> const &operand,
                typename Derived::Scalar const &scalar_operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>(operand.as_derived(), scalar_operand);
}

// scalar multiplication on the left
template <typename Derived, typename FactorTyple, typename FreeDimIndexTyple, typename UsedDimIndexTyple>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>
    operator * (typename Derived::Scalar scalar_operand,
                ExpressionTemplate_i<Derived,typename Derived::Scalar,FactorTyple,FreeDimIndexTyple,UsedDimIndexTyple> const &operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>(operand.as_derived(), scalar_operand);
}

// scalar division on the right
template <typename Derived, typename FactorTyple, typename FreeDimIndexTyple, typename UsedDimIndexTyple, typename ScalarOperand_>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'/'>
    operator / (ExpressionTemplate_i<Derived,typename Derived::Scalar,FactorTyple,FreeDimIndexTyple,UsedDimIndexTyple> const &operand,
                ScalarOperand_ const &scalar_operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'/'>(operand.as_derived(), scalar_operand);
}

// unary negation
template <typename Derived, typename FactorTyple, typename FreeDimIndexTyple, typename UsedDimIndexTyple>
ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>
    operator - (ExpressionTemplate_i<Derived,typename Derived::Scalar,FactorTyple,FreeDimIndexTyple,UsedDimIndexTyple> const &operand)
{
    return ExpressionTemplate_ScalarMultiplication_t<Derived,typename Derived::Scalar,'*'>(operand.as_derived(), -1);
}

// expression template multiplication -- tensor contraction and product

template <typename LeftDerived, typename LeftFactorTyple, typename LeftFreeDimIndexTyple, typename LeftUsedDimIndexTyple,
          typename RightDerived, typename RightFactorTyple, typename RightFreeDimIndexTyple, typename RightUsedDimIndexTyple>
ExpressionTemplate_Multiplication_t<LeftDerived,RightDerived>
    operator * (ExpressionTemplate_i<LeftDerived,typename LeftDerived::Scalar,LeftFactorTyple,LeftFreeDimIndexTyple,LeftUsedDimIndexTyple> const &left_operand,
                ExpressionTemplate_i<RightDerived,typename RightDerived::Scalar,RightFactorTyple,RightFreeDimIndexTyple,RightUsedDimIndexTyple> const &right_operand)
{
    return ExpressionTemplate_Multiplication_t<LeftDerived,RightDerived>(left_operand.as_derived(), right_operand.as_derived());
}

} // end of namespace Tenh

#endif // TENH_EXPRESSION_TEMPLATES_HPP_
