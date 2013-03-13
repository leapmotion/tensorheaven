#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "compound_index.hpp"
#include "typelist.hpp"
#include "typelist_utility.hpp"

#define FORMAT_VALUE(expr) #expr << " = " << (expr)

typedef Lvd::Uint32 Uint32;

// for use when you want to return a const reference to a zero
template <typename Scalar>
struct Static
{
    static Scalar const ZERO;
};

template <typename Scalar>
Scalar const Static<Scalar>::ZERO(0);

struct WithoutInitialization { };

static WithoutInitialization const WITHOUT_INITIALIZATION = WithoutInitialization();

/*
if A is a tensor type, and i is an index compatible with A, then A(i) returns an expression template.

e.g. if A is Antisymmetric2Tensor<U,V> and v is V, and i is U::Index and j,k is V::Index, then

A(Nonsymmetric2Tensor(i,j)) returns an expression template storing down-cast, and
A(Nonsymmetric2Tensor(i,j))*v(j) is an expression template encoding the contraction.

an expression like a(i,j)*b(j)*c(i)*d(k) should produce something like

a \otimes b \otimes c \otimes d   with indexing scheme   (i,j,j,i,k), where (i,j) are summed, (k) is free.

this expression is a thing that's indexed by k.  for each value of (i,j,k), the expression
a(i,j)*b(j)*c(i)*d(k) can be evaluated.  there needs to be some template-defined iterator which
iterates over all (i,j) (the summation indices) and produces values like (i,j,j,i,k).

the (i,j,j,i,k) tuple can be created as some template, e.g. specifying that it is a 5-slot index,
and that slots (0,3) and slots (1,2) are being contracted.  then there can be accessors for each
slot.  then the object a%b%c%d, which is a 5-tensor, can be indexed using this object.

this particular indexing scheme doesn't allow for simple tensor contraction (which is much faster).
maybe that should be a separate precomputation step -- the indices which contract simple tensors
are identified and performed first
*/

/*
expression template design
--------------------------
a tensor (a type class which includes vector) can be indexed (via operator()) to generate an
expression template.  an expression template is an indexed object -- indexed by its "free"
indices.  any indices that occur twice in the expression are "summed" indices and are not
"free".  any sum or difference should [probably] have identical "free" indices.

              +
             / \
            /   \
           *     Y(NSTI(k,i))
          / \
         /   \
        *     v(k)
       / \
      /   \
     X    u(j)
     |
     |
    NSTI(i,j))

X is a Nonsymmetric2Tensor, but to access it with two separate indices, an adaptor index
must be used -- "NSTI" (Nonsymmetric2Tensor::Index).  Thus
  X(NSTI(i,j)) is an expression template with free indices (i,j),
  X(NSTI(i,j))*u(j) is an expression template with free indices (i),
  X(NSTI(i,j))*u(j)*v(k) is an expression template with free indices (i,k),
  X(NSTI(i,j))*u(j)*v(k) + Y(NSTI(k,i)) is an expression template with free indices (i,k)

an expression template can be assigned to another one, with the following restrictions:
- the LHS (assignee) must be a single tensor and have only free indices (no summation)
- the free indices of the LHS and RHS must be exactly the same (though don't have to
  be in the same order)
- probably some explicit means of specifying if the thing on the LHS appears on the RHS
  to avoid aliasing


    X
    |           free indices (i,j)
    |           summed indices ()
    NSTI(i,j)   (i,j) |-> X(NSTI(i,j))

       *
      / \       free indices (i) (so it's an object having a single index i)
     /   \      summed indices (j) (this should be hidden "inside" the * AST)
    X     u(j)  (i) |-> X(NSTI(i,j))*u(j)
    |
    |
    NSTI(i,j)

          +
         / \
        /   \
       *     v(i)   free indices (i) (because addition isn't contraction)
      / \           (i) |-> X(NSTI(i,j))*u(j) + v(i)
     /   \
    X     u(j)
    |
    |
    NSTI(i,j)

should an expression like "u(i) + v(j)" be disallowed, because this is summing
vectors from different spaces?  it is not a tensorial expression (it depends on
the choice of basis).  the expression "A(i,j) + A(j,i)" should be allowed
only when i and j are indices of the same type.

so each expression template is a map from the tuple of free indices to a scalar value.
the summed indices will be summed during evaluation on a particular free index tuple.




QUESTION: how is the "type" of a tensor expression retained?  e.g. the expression
  "a(i)*b(j)*c(j)*d(k)"
should be a simple 2-tensor.  the highest amount of automatic type-specificity in
return values should be the goal.  another example would be if R is a symmetric
2-tensor of antisymmetric 2-tensors, then R(((i,j),(i,k))) is a symmetric 2-tensor.
because the compound tensor types have to be "broken open" to access their factors
(e.g. downcasting an antisymmetric 2-tensor to a nonsymmetric 2-tensor by indexing
it using (i,j) instead of using an antisymmetric 2-tensor index), maybe each downcast
should be associated with a symmetry in the broken-apart indices (e.g. downcasting
a symmetric 2-tensor would give (i,j)-(j,i)=0).  there is a problem with this though,
because not all casts have symmetries of this form (e.g. simple tensors which don't
even have a "normal" index, or diagonal 2-tensors, which would require equations
like (i,j)=0 for i != j).

maybe the way to handle this is to specify "upcasts", which are explicitly programmed
and certified by the human to do what they promise.  e.g. R(((i,j),(i,k))) upcasts
to a symmetric 2-tensor.  while this leaves some work for the human to do, this is
probably a reasonable way to address the problem of finding symmetries in arbirary
tensor expressions (something that is nontrivial and nonobvious).  each upcast can
come with a test or a set of tests to empirically verify that it's not breaking any
rules (e.g. verify that (j,k) |-> R(((i,j),(i,k))) is actually symmetric in (j,k)).




I'm thinking it's actually easier to use the % operator to tensor things together, then
later index the thing, so

  a(i,j)*b(j)*c(i)*d(k)

would be

  (a%b%c%d)(NSTI(i,j),j,i,k)

so a%b%c%d is an object that can be indexed -- it has an index type, and its operator()
is overloaded to do the expression template stuff.  though this doesn't solve the problem
with addition.

could go full in-line contraction notation and use permutations to do fancy indexing,
though using permutations isn't always super easy.


QUESTION: what is an expression template?  it is an operation (eval, unary -, binary * + -), a
set of operands (indexed expressions; vectors, tensors, scalars, expression templates),
and a set of free and summed indices for each operand.

  v(i)                  is an expression template with operator "evaluation" (doesn't modify)
                        with operand v, free indices (i), and summed indices ().

  a(i,j)                is an expression template with operator "evaluation" with operand
                        a, free indices (i,j), and summed indices ()

  v(i)*a(i,j)           is an expression template with operator * (tensor contraction) with
                        operands v(i) (having free indices (i), summed indices ()) and
                        a(i,j) (having free indices (i,j), summed indices ()), and its
                        free indices are (j) and its summed indices are (i).

  u(j)                  is just like v(i) but with different symbols

  u(j) + v(i)*a(i,j)    is an expression template with operator + (tensor addition) with
                        operands u(j) (having free indices (j), summed indices ()) and
                        v(i)*a(i,j) (having free indices (j), summed indices (i)), and its
                        free indices are (j) and its summed indices are (i).

  u(i) + v(i)*a(i,j)    should be an error (an index was used more than twice).


*/

template <typename Operand, typename IndexType>
struct ExpressionTemplate_Evaluation_t
{
    typedef typename Operand::Scalar Scalar;

    typedef TypeList_t<IndexType> FreeIndexTypeList;
    typedef EmptyTypeList SummedIndexTypeList;
    typedef CompoundIndex_t<TypeList_t<IndexType> > Index; // an index composed of all free indices

    ExpressionTemplate_Evaluation_t (Operand const &operand) : m_operand(operand) { }

    // read-only, because it doesn't necessarily make sense to assign to an expression
    // template -- the expression may be a product or some such, where each component
    // is not an L-value.
    Scalar operator [] (IndexType const &i) const { return m_operand[i]; }

private:

    Operand const &m_operand;
};

template <typename LeftOperand, typename RightOperand>
struct ExpressionTemplate_Addition_t
{
    // TODO: check that the summed indices from each operand have no indices in common
    // though technically this is unnecessary, because the summed indices are "private"
    // to each contraction, so this is really for the human's benefit, not getting
    // confused by multiple repeated indices that have nothing to do with each other.
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v &&
               Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::FreeIndexTypeList,typename RightOperand::FreeIndexTypeList>::v>::v };

    typedef typename LeftOperand::Scalar Scalar;
    typedef typename LeftOperand::FreeIndexTypeList FreeIndexTypeList;
    typedef EmptyTypeList SummedIndexTypeList; // TEMP: see above comment about "private" indices
    typedef typename LeftOperand::Index Index;

    ExpressionTemplate_Addition_t (LeftOperand const &left_operand, RightOperand const &right_operand)
        :
        m_left_operand(left_operand),
        m_right_operand(right_operand)
    { }

    Scalar operator [] (Index const &i) const { return m_left_operand[i] + m_right_operand[i]; }

private:

    LeftOperand const &m_left_operand;
    RightOperand const &m_right_operand;
};

template <typename LeftOperand, typename RightOperand, typename FreeIndexTypeList, typename SummedIndexTypeList>
struct Summation_t
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v &&
               Lvd::Meta::Assert<(SummedIndexTypeList::LENGTH > 0)>::v };

    typedef typename LeftOperand::Scalar Scalar;

    static Scalar eval (LeftOperand const &left_operand, RightOperand const &right_operand, CompoundIndex_t<FreeIndexTypeList> const &i)
    {
        typedef typename ConcatenationOfTypeLists_t<FreeIndexTypeList,SummedIndexTypeList>::T TotalIndexTypeList;
        typedef CompoundIndex_t<TotalIndexTypeList> TotalIndex;
        typedef CompoundIndex_t<SummedIndexTypeList> SummedIndex;

        // the operands take indices that are a subset of the summed indices and free indices.

        // constructing t with i initializes the first elements which correpond to
        // Index with the value of i, and initializes the remaining elements to zero.
        TotalIndex t(i);
        Scalar retval(0);
        // get the map which produces the CompoundIndex for each operand from the TotalIndex t
        typedef CompoundIndexMap_t<TotalIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef CompoundIndexMap_t<TotalIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
        // t = (f,s), which is a concatenation of the free access indices and the summed access indices.
        // s is a reference to the second part, which is what is iterated over in the summation.
        for (SummedIndex &s = t.template trailing_list<FreeIndexTypeList::LENGTH>(); s.is_not_at_end(); ++s)
            retval += left_operand[left_operand_index_map(t)] * right_operand[right_operand_index_map(t)];
        return retval;
    }
};

// template specialization handles summation over no indices
template <typename LeftOperand, typename RightOperand, typename FreeIndexTypeList>
struct Summation_t<LeftOperand,RightOperand,FreeIndexTypeList,EmptyTypeList>
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v };

    typedef typename LeftOperand::Scalar Scalar;

    static Scalar eval (LeftOperand const &left_operand, RightOperand const &right_operand, CompoundIndex_t<FreeIndexTypeList> const &i)
    {
        // get the map which produces the CompoundIndex for each operand from the free indices i
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
        typedef CompoundIndexMap_t<FreeIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
        typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
        typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
        return left_operand[left_operand_index_map(i)] * right_operand[right_operand_index_map(i)];
    }
};

// TODO: there is an issue to think about: while it is totally valid to do
// u(i)*v(j)*w(j) (this is an outer product contracted with a vector), the
// expression v(j)*w(j) can be computed first and factored out of the whole
// thing, instead of needing to be multiplied out for each access of the i index.
// this may be somewhat difficult to do, as it would require searching the
// expression template AST for such contractions and restructuring the AST.
template <typename LeftOperand, typename RightOperand>
struct ExpressionTemplate_Multiplication_t
{
    // TODO: check that the summed indices from each operand have no indices in common
    // though technically this is unnecessary, because the summed indices are "private"
    // to each contraction, so this is really for the human's benefit, not getting
    // confused by multiple repeated indices that have nothing to do with each other.
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename LeftOperand::Scalar,typename RightOperand::Scalar>::v>::v };
    // TODO: ensure there are no indices that occur 3+ times (?)

    typedef typename LeftOperand::Scalar Scalar;
    // the free indices are the single-occurrence indices of the concatenated
    // list of free indices from the left and right operands
    typedef typename ConcatenationOfTypeLists_t<typename LeftOperand::FreeIndexTypeList,
                                                typename RightOperand::FreeIndexTypeList>::T CombinedFreeIndexTypeList;
    typedef typename ElementsHavingMultiplicity_t<CombinedFreeIndexTypeList,1>::T FreeIndexTypeList;
    // the summed indices (at this level) are the double-occurrences indices
    // of the concatenated list of free indices from the left and right operands
    typedef typename ElementsHavingMultiplicity_t<CombinedFreeIndexTypeList,2>::T SummedIndexTypeList;
    // Index is a list (TODO: change to tuple) of type FreeIndices
    typedef CompoundIndex_t<FreeIndexTypeList> Index;

    ExpressionTemplate_Multiplication_t (LeftOperand const &left_operand, RightOperand const &right_operand)
        :
        m_left_operand(left_operand),
        m_right_operand(right_operand)
    { }

    Scalar operator [] (Index const &i) const
    {
        return Summation_t<LeftOperand,RightOperand,FreeIndexTypeList,SummedIndexTypeList>::eval(m_left_operand, m_right_operand, i);
//         typedef typename ConcatenationOfTypeLists_t<FreeIndexTypeList,SummedIndexTypeList>::T TotalIndexTypeList;
//         typedef CompoundIndex_t<TotalIndexTypeList> TotalIndex;
//         typedef CompoundIndex_t<SummedIndexTypeList> SummedIndex;
//
//         // TODO: iterate over the indices that are summed at this level
//
//         // the operands take indices that are a subset of the summed indices and access index.
//
//         // constructing t with i initializes the first elements which correpond to
//         // Index with the value of i, and initializes the remaining elements to zero.
//         TotalIndex t(i);
//         Scalar retval(0);
//         // get the map which produces the CompoundIndex for each operand from the TotalIndex t
//         typedef CompoundIndexMap_t<TotalIndexTypeList,typename LeftOperand::FreeIndexTypeList> LeftOperandIndexMap;
//         typedef CompoundIndexMap_t<TotalIndexTypeList,typename RightOperand::FreeIndexTypeList> RightOperandIndexMap;
//         typename LeftOperandIndexMap::EvalMapType left_operand_index_map = LeftOperandIndexMap::eval;
//         typename RightOperandIndexMap::EvalMapType right_operand_index_map = RightOperandIndexMap::eval;
//         // t = (f,s), which is a concatenation of the free access indices and the summed access indices.
//         // s is a reference to the second part, which is what is iterated over in the summation.
//         for (SummedIndex &s = t.template trailing_list<FreeIndexTypeList::LENGTH>(); s.is_not_at_end(); ++s)
//             retval += m_left_operand[left_operand_index_map(t)] * m_right_operand[right_operand_index_map(t)];
//         return retval;
    }

private:

    LeftOperand const &m_left_operand;
    RightOperand const &m_right_operand;
};

template <typename Scalar_, Uint32 DIM_> // don't worry about type ID for now
struct Vector_t
{
    typedef Scalar_ Scalar;
    static Uint32 const DIM = DIM_;
    static Uint32 const FACTOR_COUNT = 1; // vector quantities are used as 1-tensors.

    // for use in operator [] for actual evaluation of tensor components
    struct Index
    {
        static Uint32 const DIM = Vector_t::DIM;

        Index () : m(0) { }
        Index (Uint32 i) : m(i) { }

        bool is_at_end () const { return m >= DIM; }
        bool is_not_at_end () const { return m < DIM; }
        Uint32 value () const { return m; } // for the specific memory addressing scheme that Vector_t uses
        void operator ++ () { ++m; }
        void reset () { m = 0; }

        static std::string type_as_string () { return TypeStringOf_t<Vector_t>::eval() + "::Index"; }

    private:

        Uint32 m;
    };

    // for use in operator () for creation of expression templates (indexed tensor expressions)
    template <char SYMBOL>
    struct Index_t : public Index
    {
        Index_t () { }
        Index_t (Uint32 i) : Index(i) { }

        static std::string type_as_string () { return TypeStringOf_t<Vector_t>::eval() + "::Index_t<'" + SYMBOL + "'>"; }
    };

    explicit Vector_t (WithoutInitialization const &) { }
    Vector_t (Scalar fill) { for (Uint32 i = 0; i < DIM; ++i) m[i] = fill; }
    Vector_t (Scalar x0, Scalar x1) { Lvd::Meta::Assert<(DIM == 2)>(); m[0] = x0; m[1] = x1; }
    Vector_t (Scalar x0, Scalar x1, Scalar x2) { Lvd::Meta::Assert<(DIM == 3)>(); m[0] = x0; m[1] = x1; m[2] = x2; }
    Vector_t (Scalar x0, Scalar x1, Scalar x2, Scalar x3) { Lvd::Meta::Assert<(DIM == 4)>(); m[0] = x0; m[1] = x1; m[2] = x2; m[3] = x3; }

    // NOTE: operator [] will be used to return values, while
    // operator () will be used to create expression templates
    // for the purposes of indexed contractions.
    // TODO: make Index type encode the guarantee that it's value will always be valid
    Scalar operator [] (Index const &i) const
    {
        if (i.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return m[i.value()];
    }
    Scalar &operator [] (Index const &i)
    {
        if (i.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return m[i.value()];
    }

    // the argument is technically unnecessary, as its value is not used.  however,
    // this allows the template system to deduce the SYMBOL of the IndexType_t, so
    // it doesn't need to be specified explicitly.
    // in this, an outer product would be
    // IndexType_t<'i'> i;
    // IndexType_t<'j'> j;
    // u(i)*v(j)
    template <char SYMBOL>
    ExpressionTemplate_Evaluation_t<Vector_t,Index_t<SYMBOL> > operator () (Index_t<SYMBOL> const &) const
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return expr<SYMBOL>();
    }

    // the corresponding outer product example here would be
    // u.expr<'i'>() * v.expr<'j'>()
    template <char SYMBOL>
    ExpressionTemplate_Evaluation_t<Vector_t,Index_t<SYMBOL> > expr () const
    {
        return ExpressionTemplate_Evaluation_t<Vector_t,Index_t<SYMBOL> >(*this);
    }

    static std::string type_as_string () { return "Vector_t<" + TypeStringOf_t<Scalar>::eval() + ',' + AS_STRING(DIM) + '>'; }

protected:

    Scalar m[DIM];
};

template <typename Scalar, Uint32 DIM>
std::ostream &operator << (std::ostream &out, Vector_t<Scalar,DIM> const &v)
{
    typedef Vector_t<Scalar,DIM> Vector;
    typedef typename Vector::Index Index;

    if (DIM == 0)
        return out << "()";

    Index i; // initialized to the beginning automatically
    out << '(' << v[i];
    ++i;
    for ( ; i.is_not_at_end(); ++i)
        out << ", " << v[i];
    return out << ')';
}

template <typename Scalar, Uint32 DIM>
std::ostream &operator << (std::ostream &out, typename Vector_t<Scalar,DIM>::Index const &i)
{
    return out << i.value();
}

template <typename Scalar, Uint32 DIM, char SYMBOL>
std::ostream &operator << (std::ostream &out, typename Vector_t<Scalar,DIM>::template Index_t<SYMBOL> const &i)
{
    return out << i.value();
}

// this default implementation should work for any vector space that has an Index type (but not e.g. Simple2Tensor_t).
// template specialization can be used to define other implementations (e.g. Simple2Tensor_t).
template <typename Vector>
struct DotProduct_t
{
    typedef typename Vector::Scalar Scalar;
    typedef typename Vector::Index Index;

    static Scalar eval (Vector const &l, Vector const &r)
    {
        Scalar retval(0);
        for (Index i; i.is_not_at_end(); ++i)
            retval += l[i]*r[i];
        return retval;
    }
};

// expression template for contracting Vector_t types
// template <typename F1_, typename F2_, typename IndexType1_, typename IndexType2_>
// struct ExpressionTemplate_t // TODO: come up with better name
// {
//     enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F2_::Scalar>::v>::v };
//
//     typedef F1_ F1;
//     typedef F2_ F2;
//     typedef typename F1::Scalar Scalar;
//     typedef IndexType1_ IndexType1;
//     typedef IndexType2_ IndexType2;
//
//     ExpressionTemplate_t (F1 const &f1, F2 const &f2) : m1(f1), m2(f2) { }
//
//     Scalar eval (Tuple_t<IndexType> const &index) const
//     {
//     }
// };
//
// template <typename F1, typename F2, typename IndexType1, typename IndexType2>
// ExpressionTemplate_t<F1,F2,IndexType1,IndexType2> operator * (

// general 2-tensor with no symmetries -- most general type of 2-tensor
template <typename F1_, typename F2_>
struct Nonsymmetric2Tensor_t : Vector_t<typename F1_::Scalar,F1_::DIM*F2_::DIM>
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F2_::Scalar>::v>::v };

    typedef Vector_t<typename F1_::Scalar,F1_::DIM*F2_::DIM> Parent;
    typedef typename Parent::Scalar Scalar;
    using Parent::DIM;
    static Uint32 const FACTOR_COUNT = 2; // there are two factors in this tensor type (F1 and F2)

    typedef F1_ F1;
    typedef F2_ F2;

    Nonsymmetric2Tensor_t (WithoutInitialization const &w) : Parent(w) { }

    struct Index : public Parent::Index
    {
        Index () { } // default constructor initializes to beginning
        Index (Uint32 i) : Parent::Index(i) { }
        Index (typename F1::Index i1, typename F2::Index i2) : Parent::Index(F2::DIM*i1.value()+i2.value()) { }
        typename F1::Index subindex1 () const { return this->value() / F2::DIM; }
        typename F2::Index subindex2 () const { return this->value() % F2::DIM; }
    };
};

// general 3-tensor with no symmetries -- most general type of 3-tensor
template <typename F1_, typename F2_, typename F3_>
struct Nonsymmetric3Tensor_t : Vector_t<typename F1_::Scalar,F1_::DIM*F2_::DIM*F3_::DIM>
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F2_::Scalar>::v>::v };

    typedef Vector_t<typename F1_::Scalar,F1_::DIM*F2_::DIM*F3_::DIM> Parent;
    typedef typename Parent::Scalar Scalar;
    using Parent::DIM;
    static Uint32 const FACTOR_COUNT = 3; // there are two factors in this tensor type (F1 and F2)

    typedef F1_ F1;
    typedef F2_ F2;
    typedef F3_ F3;

    struct Index : public Parent::Index
    {
        Index () { } // default constructor initializes to beginning
        Index (Uint32 i) : Parent::Index(i) { }
        Index (typename F1::Index i1, typename F2::Index i2, typename F3::Index i3)
            :
            Parent::Index(F3::DIM*(F2::DIM*i1.value()+i2.value())+i3.value())
        { }
        typename F1::Index subindex1 () const { return this->value() / (F3::DIM*F2::DIM); }
        typename F2::Index subindex2 () const { return this->value() / F3::DIM % F2::DIM; }
        typename F3::Index subindex3 () const { return this->value() % F3::DIM; }
    };
};

// outer product of two vectors "A \otimes B", resulting in a simple 2-tensor.
// the space of simple 2-tensors is not a linear subspace of 2-tensors, so it
// is not a vector quantity.
template <typename F1_, typename F2_>
struct Simple2Tensor_t
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F2_::Scalar>::v>::v };

    typedef F1_ F1;
    typedef F2_ F2;
    typedef typename F1::Scalar Scalar;
    // there is no dimension because this is not a vector space (not even a manifold -- it is an algebraic variety)
    static Uint32 const FACTOR_COUNT = 2; // there are two factors in this tensor type (F1 and F2)

    Simple2Tensor_t (F1 const &f1, F2 const &f2) : m1(f1), m2(f2) { }

    F1 const &factor1 () const { return m1; }
    F2 const &factor2 () const { return m2; }

    Scalar operator [] (typename Nonsymmetric2Tensor_t<F1,F2>::Index const &i) const
    {
        if (i.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return m1[i.subindex1().value()] * m2[i.subindex2().value()];
    }


    // TODO: type cast operator to NonsymmetricTensor which creates an expression template to do the indexing

    // TODO: the sum of two Simple2Tensor_t<F1,F2>s is a Nonsymmetric2Tensor_t<F1,F2>

private:

    F1 m1;
    F2 m2;
};

template <typename F1, typename F2>
std::ostream &operator << (std::ostream &out, Simple2Tensor_t<F1,F2> const &s)
{
    return out << s.factor1() << " \\otimes " << s.factor2();
}

// template specialization for the contraction of simple 2-tensors
// (A \otimes B) : (C \otimes D) := (A \cdot C)(B \cdot D)
template <typename F1, typename F2>
struct DotProduct_t<Simple2Tensor_t<F1,F2> >
{
    typedef Simple2Tensor_t<F1,F2> Simple2Tensor;
    typedef typename Simple2Tensor::Scalar Scalar;

    static Scalar eval (Simple2Tensor const &l, Simple2Tensor const &r)
    {
        return DotProduct_t<F1>::eval(l.factor1(),r.factor1()) * DotProduct_t<F2>::eval(l.factor2(),r.factor2());
    }
};

// Simple3Tensor_t

template <typename F1_, typename F2_, typename F3_>
struct Simple3Tensor_t
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F2_::Scalar>::v>::v &&
               Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F3_::Scalar>::v>::v };

    typedef F1_ F1;
    typedef F2_ F2;
    typedef F3_ F3;
    typedef typename F1::Scalar Scalar;
    // there is no dimension because this is not a vector space (not even a manifold -- it is an algebraic variety)
    static Uint32 const FACTOR_COUNT = 3; // there are two factors in this tensor type (F1 and F2)

    Simple3Tensor_t (F1 const &f1, F2 const &f2, F3 const &f3) : m1(f1), m2(f2), m3(f3) { }

    F1 const &factor1 () const { return m1; }
    F2 const &factor2 () const { return m2; }
    F3 const &factor3 () const { return m3; }

    Scalar operator [] (typename Nonsymmetric3Tensor_t<F1,F2,F3>::Index const &i) const
    {
        if (i.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return m1[i.subindex1().value()] * m2[i.subindex2().value()] * m3[i.subindex3().value()];
    }

private:

    F1 m1;
    F2 m2;
    F3 m3;
};

template <typename F1, typename F2, typename F3>
std::ostream &operator << (std::ostream &out, Simple3Tensor_t<F1,F2,F3> const &s)
{
    return out << s.factor1() << " \\otimes " << s.factor2() << " \\otimes " << s.factor3();
}

// template specialization for the contraction of simple 2-tensors
// (A \otimes B \otimes C) : (P \otimes Q \otimes R) := (A \cdot P)(B \cdot Q)(C \cdot R)
template <typename F1, typename F2, typename F3>
struct DotProduct_t<Simple3Tensor_t<F1,F2,F3> >
{
    typedef Simple3Tensor_t<F1,F2,F3> Simple3Tensor;
    typedef typename Simple3Tensor::Scalar Scalar;

    static Scalar eval (Simple3Tensor const &l, Simple3Tensor const &r)
    {
        return DotProduct_t<F1>::eval(l.factor1(),r.factor1()) *
               DotProduct_t<F2>::eval(l.factor2(),r.factor2()) *
               DotProduct_t<F3>::eval(l.factor3(),r.factor3());
    }
};

// TODO: symmetric outer product of one vector with itself producing a special type of simple tensor
// TODO: symmetrization and antisymmetrization of outer product
/*
// 2-tensor which is self-adjoint with respect to the natural pairing
template <typename T_>
struct Symmetric2Tensor_t : public Vector_t<T_::Scalar,((T_::DIM+1)*T_::DIM)/2>
{
    typedef Vector_t<T_::Scalar,((T_::DIM+1)*T_::DIM)/2> Parent;
    typedef Parent::Index Index; // TODO: make actual Index struct?

    // TODO: make Index in this class, and make it construct with a Tensor<T,T>::Index

    Scalar &operator [] (Tensor<T,T>::Index const &i)
    {
        Uint32 r = i.subindex1().value();
        Uint32 c = i.subindex2().value();
        // here is where the fancy indexing happens -- the symmetric tensor is stored as upper-diagonal
        // the sequence of offsets per row r is
        // 0 -> 0
        // 1 -> 0+D-0 = 1*D - 0
        // 2 -> 0+D-0+D-1 = 2*D - 1
        // 3 -> 0+D-0+D-1+D-2 = 3*D - 3
        // ...
        // r -> ... = r*D - r*(r-1)/2,
        // because the columns start at the r value, there is a column offset per row as well, of r.
        // so if the general index is (r,c), then the offset in the symmetric representation is
        // r*D - r*(r-1)/2 + c - r = r*D - r*(r+1)/2 + c
        // if we're in the subdiagonal, switch the indices to get into the superdiagonal
        if (r > c)
            std::swap(r, c);
        Index symmetric_index(r*D - (r*(r+1) >> 1) + c);
        return operator[](symmetric_index);
    }
};
*/
template <typename F_>
struct Antisymmetric2Tensor_t : public Vector_t<typename F_::Scalar,((F_::DIM-1)*F_::DIM)/2>
{
    typedef Vector_t<typename F_::Scalar,((F_::DIM-1)*F_::DIM)/2> Parent;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::Index Index;
    using Parent::DIM;
    typedef F_ F;
    static Uint32 const FACTOR_COUNT = 2; // there are two factors in this tensor type (F1 and F2)

    // TODO: make Index in this class, and make it construct with a Tensor<T,T>::Index

    Scalar &operator [] (typename Nonsymmetric2Tensor_t<F,F>::Index const &i)
    {
        Uint32 r = i.subindex1().value();
        Uint32 c = i.subindex2().value();
        if (r == c)
            return Static<Scalar>::ZERO; // need an actual reference, not a temporary
        // here is where the fancy indexing happens -- the antisymmetric tensor is
        // stored as strict upper-diagonal
        // the sequence of offsets per row r is
        // 0 -> 0
        // 1 -> 0+D-1 = D - 1
        // 2 -> 0+D-1+D-2 = 2*D - 3
        // 3 -> 0+D-1+D-2+D-3 = 3*D - 6
        // ...
        // r -> ... = r*D - r*(r+1)/2,
        // because the columns start at the r+1 value, there is a column offset per row as well, of r+1.
        // so if the general index is (r,c), then the offset in the symmetric representation is
        // r*D - r*(r+1)/2 + c - (r+1) = r*D - r*(r+3)/2 + c - 1
        // if we're in the subdiagonal, switch the indices to get into the superdiagonal
        if (r > c)
            std::swap(r, c);
        Index antisymmetric_2_tensor_index(r*DIM - (r*(r+3) >> 1) + c - 1);
        return operator[](antisymmetric_2_tensor_index); // should call Vector_t::operator[]
    }
};

// expression templates
//
// need one for the following operators:
// = (assignment)
// + (addition of types having a common downcast (e.g. a Simple2Tensor_t and an Antisymmetric2Tensor_t)
// - (subtraction of types having a common downcast)
// % (tensor product to form higher tensor types -- produces a SimpleNTensor_t)
//
// NOTE: * (in-line adjacent contraction of identical types) will (could) evaluate immediately,
// but * (in-line adjacent contraction of non-identical types) requires breaking the tensor types down,
// so maybe an expression template for downcasting is necessary.

// template <typename F_>
// struct Expr_2Tensor_1Contraction_t
// {
//     enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F2_::Scalar>::v>::v };
//
//     typedef F1_ F1;
//     typedef F2_ F2;
//     typedef typename F1::Scalar Scalar;
//     typedef Scalar ReturnType;
//
//     Expr_2Tensor_1Contraction_t (Simple2Tensor_t<F1,F2> const &s) :
//
//     ReturnType eval ()
// };

template <typename F1, typename F2>
Simple2Tensor_t<F1,F2> operator % (F1 const &l, F2 const &r)
{
    return Simple2Tensor_t<F1,F2>(l, r);
}

template <typename F1, typename F2, typename F3>
Simple3Tensor_t<F1,F2,F3> operator % (F1 const &l, Simple2Tensor_t<F2,F3> const &r)
{
    return Simple3Tensor_t<F1,F2,F3>(l, r.factor1(), r.factor2());
}

template <typename F1, typename F2, typename F3>
Simple3Tensor_t<F1,F2,F3> operator % (Simple2Tensor_t<F1,F2> const &l, F3 const &r)
{
    return Simple3Tensor_t<F1,F2,F3>(l.factor1(), l.factor2(), r);
}


int main (int argc, char **argv)
{
    typedef Vector_t<float,2> Float2;
    typedef Vector_t<float,3> Float3;
    typedef Vector_t<float,4> Float4;

    // testing various tensor access and operations
    {
        Float2 u(10,20);
        Float3 v(1,2,3);
        Float4 w(4,5,6,7);

        std::cout << FORMAT_VALUE(u) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(w) << '\n';

        typedef Simple2Tensor_t<Float3,Float4> SimpleFloat3x4;
        SimpleFloat3x4 X(v,w);

        std::cout << "X := v \\otimes w = " << X << ", and " << v%w << '\n';

        typedef Nonsymmetric2Tensor_t<Float3,Float4> Float3x4;

        for (Uint32 r = 0; r < 3; ++r)
        {
            for (Uint32 c = 0; c < 4; ++c)
            {
                std::cout << "\t" << X[Float3x4::Index(r,c)];
            }
            std::cout << '\n';
        }

        std::cout << '\n';

        typedef Nonsymmetric3Tensor_t<Float2,Float3,Float4> Float2x3x4;

        for (Uint32 i = 0; i < Float3x4::DIM; ++i)
        {
            Float3x4::Index index(i);
            std::cout << i << " -> " << index.subindex1().value() << ", " << index.subindex2().value()
                      << " -> " << Float3x4::Index(index.subindex1().value(), index.subindex2().value()).value() << '\n';
        }

        std::cout << '\n';

        for (Uint32 i = 0; i < Float2x3x4::DIM; ++i)
        {
            Float2x3x4::Index index(i);
            std::cout << i << " -> " << index.subindex1().value() << ", " << index.subindex2().value() << ", " << index.subindex3().value()
                      << " -> " << Float2x3x4::Index(index.subindex1().value(), index.subindex2().value(), index.subindex3().value()).value() << '\n';
        }

        std::cout << '\n';

        typedef Simple3Tensor_t<Float2,Float3,Float4> SimpleFloat2x3x4;
        SimpleFloat2x3x4 Z(u%v%w);
        std::cout << "u \\otimes v \\otimes w = " << Z << '\n';
        for (Uint32 s = 0; s < 2; ++s)
        {
            for (Uint32 r = 0; r < 3; ++r)
            {
                for (Uint32 c = 0; c < 4; ++c)
                {
                    std::cout << "\t" << Z[Float2x3x4::Index(s,r,c)];
                }
                std::cout << '\n';
            }
            std::cout << '\n';
        }


        Float3 a(0,-2,4);
        Float4 b(8,4,2,1);
        SimpleFloat3x4 Y(a,b);

        std::cout << "a = " << a << '\n';
        std::cout << "b = " << b << '\n';
        std::cout << "Y := a \\otimes b = " << Y << '\n';

        for (Uint32 r = 0; r < 3; ++r)
        {
            for (Uint32 c = 0; c < 4; ++c)
            {
                std::cout << "\t" << Y[Float3x4::Index(r,c)];
            }
            std::cout << '\n';
        }

        std::cout << '\n';

        std::cout << "X * Y = " << DotProduct_t<SimpleFloat3x4>::eval(X,Y) << " (";

        // done the hard way -- compute the inner product of X and Y
        float accumulator = 0;
        for (Uint32 r = 0; r < 3; ++r)
            for (Uint32 c = 0; c < 4; ++c)
                accumulator += X[Float3x4::Index(r,c)] * Y[Float3x4::Index(r,c)];
        std::cout << "actual answer = " << accumulator << ")\n\n";

        typedef Nonsymmetric2Tensor_t<Float3,Float3> Float3x3;
        Float3x3 W(WITHOUT_INITIALIZATION);
        for (Uint32 i = 0; i < 3; ++i)
            for (Uint32 j = 0; j < 3; ++j)
                for (Uint32 k = 0; k < 4; ++k)
                    W[Float3x3::Index(i,j)] = X[Float3x4::Index(i,k)] * Y[Float3x4::Index(j,k)];
        for (Uint32 i = 0; i < 3; ++i)
        {
            for (Uint32 j = 0; j < 3; ++j)
                std::cout << '\t' << W[Float3x3::Index(i,j)];
            std::cout << '\n';
        }
    }

    // testing expression templates
    {
        Float3 u(-0.1, 2.0, 8);
        Float3 v(4.1, 5.2, 6.3);
        Float3 w(1.2, -2.0, 3.8);
        typedef Float3::Index_t<'i'> I;
        typedef Float3::Index_t<'j'> J;
        I i;
        J j;
        {
            typedef ExpressionTemplate_Evaluation_t<Float3,I> EI;
            typedef ExpressionTemplate_Evaluation_t<Float3,J> EJ;

//             std::cout << i << '\n';
//             std::cout << Float3::Index(0) << '\n';
            std::cout << FORMAT_VALUE(v(i)[EI::Index(0)]) << '\n';

            std::cout << FORMAT_VALUE(v.expr<'j'>()[EJ::Index(1)]) << '\n';

            for (Float3::Index k; k.is_not_at_end(); ++k)
                std::cout << u[k] + v[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "addition:\n";
            typedef ExpressionTemplate_Evaluation_t<Float3,I> EE;
            typedef ExpressionTemplate_Addition_t<EE,EE> EA;
            EA e(u(i), v(i));
            std::cout << "expression template value:\n";
            for (EA::Index k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';
            std::cout << "hand-calculated value:\n";
            for (Uint32 k = 0; k < 3; ++k)
                std::cout << u[k] + v[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "inner product:\n";
            typedef ExpressionTemplate_Evaluation_t<Float3,I> EE;
            typedef ExpressionTemplate_Multiplication_t<EE,EE> EM;
            EM e(u(i), v(i));
            Float3::Index k;
            std::cout << FORMAT_VALUE(k.DIM) << '\n';
            std::cout << FORMAT_VALUE(TypeStringOf_t<EE::Index>::eval()) << '\n';
            std::cout << "expression template value:\n";
            std::cout << FORMAT_VALUE(e[EM::Index()]) << '\n';
            std::cout << "hand-calculated value:\n";
            float dot = 0;
            for (Uint32 k = 0; k < 3; ++k)
                dot += u[k]*v[k];
            std::cout << dot << '\n';
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "outer product:\n";
            typedef ExpressionTemplate_Evaluation_t<Float3,I> EI;
            typedef ExpressionTemplate_Evaluation_t<Float3,J> EJ;
            typedef ExpressionTemplate_Multiplication_t<EI,EJ> EM;
            EM e(u(i), v(j));
            std::cout << FORMAT_VALUE(TypeStringOf_t<EM::Index>::eval()) << '\n';
            std::cout << FORMAT_VALUE(TypeStringOf_t<EM::FreeIndexTypeList>::eval()) << '\n';
            std::cout << FORMAT_VALUE(TypeStringOf_t<EM::SummedIndexTypeList>::eval()) << '\n';
            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << FORMAT_VALUE(v) << '\n';
            std::cout << "expression template value:\n";
            for (EM::Index k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';
            std::cout << "hand-calculated value:\n";
            for (Uint32 ii = 0; ii < 3; ++ii)
                for (Uint32 jj = 0; jj < 3; ++jj)
                    std::cout << u[ii] * v[jj] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "contraction with simple tensor:\n";
            typedef ExpressionTemplate_Evaluation_t<Float3,I> EI;
            typedef ExpressionTemplate_Evaluation_t<Float3,J> EJ;
            typedef ExpressionTemplate_Multiplication_t<EI,EJ> EM;
            typedef ExpressionTemplate_Multiplication_t<EM,EJ> EMJ;
            EMJ e(EM(u(i), v(j)), w(j));
            std::cout << FORMAT_VALUE(TypeStringOf_t<EMJ::Index>::eval()) << '\n';
            std::cout << FORMAT_VALUE(TypeStringOf_t<EMJ::FreeIndexTypeList>::eval()) << '\n';
            std::cout << FORMAT_VALUE(TypeStringOf_t<EMJ::SummedIndexTypeList>::eval()) << '\n';
            std::cout << "expression template value:\n";
            for (EMJ::Index k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';
            std::cout << "hand-calculated value:\n";
            for (Uint32 ii = 0; ii < 3; ++ii)
            {
                float accum = 0;
                for (Uint32 jj = 0; jj < 3; ++jj)
                    accum += u[ii] * v[jj] * w[jj];
                std::cout << accum << ", ";
            }
            std::cout << '\n';
            std::cout << '\n';
        }
    }

    return 0;
}

