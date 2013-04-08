#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include "tenh/core.hpp" // everything should include this
#include "tenh/multiindex.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/meta/typelist.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/meta/typetuple.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2antisymmetric.hpp"
#include "tenh/tensor2diagonal.hpp"
#include "tenh/tensor2symmetric.hpp"
#include "tenh/vector.hpp"

/*
if A is a tensor type, and i is an index compatible with A, then A(i) returns an expression template.

e.g. if A is Tensor2Antisymmetric<U,V> and v is V, and i is U::Index and j,k is V::Index, then

A(Tensor2(i,j)) returns an expression template storing down-cast, and
A(Tensor2(i,j))*v(j) is an expression template encoding the contraction.

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

X is a Tensor2, but to access it with two separate indices, an adaptor index
must be used -- "NSTI" (Tensor2::Index).  Thus
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






given a tuple of values, values which may repeat up to twice, what is the map of unique indices to that tuple?

e.g.

(i,j,j,i,k) has unique indices (i,j,k), and the map is given by (0,1,1,0,2)

so in the expression a(i,j)*b(j)*c(i)*d(k), the object a*b*c*d has 5 indices, and (a*b*c*d)(i,j,j,i,k) is a(i,j)*b(j)*c(i)*d(k),

and so if R is the reindexing map (i,j,k) -> (i,j,j,i,k), then the value is

(a*b*c*d)(R(i,j,k))

--------------

another problem is identifying free indices and identifying summation indices.

in (i,j,j,i,k), the summation indices are (i,j) and the free indices are (k).

so the expression a(i,j)*b(j)*c(i)*d(k) is really a map

k -> (a*b*c*d)(R(i,j,k))

the free indices are the ones which occur exactly once.  the summation indices are the ones which occur exactly twice.

--------------

regarding conversion between different tensor types, the product * should produce "simple tensors",
where the type of each factor is "in parentheses", e.g.

(V1 \otimes V2) \otimes V3 \otimes V4 -- this would be a simple 3-tensor, where the first factor has type "V1 \otimes V2".

to "break open" the factor types, a "down cast" should be made;

the above examples were simplified.  if "a" was a Nonsymmetric2Tensor, then it would be indexed by a single
Nonsymmetric2Tensor::Index.  but to break this apart to get the individual factors, use

a(Nonsymmetric2Tensor::Index(i,j))

and then the above expression would be

a(Nonsymmetric2Tensor::Index(i,j))*b(j)*c(i)*d(k).

the "() operator" for "a" would need to be overloaded to return an expression template for this.  in fact, all tensors/vectors
need to implement the "() operator" for indexing to return expression templates on which the "* operator" is defined.

NOTE: make all indices' numerical constructors (e.g. Index (Uint32 i) ...) explicit, so that literal expressions like
a(Nonsymmetric2Tensor::Index(1,3))*b(0)*c(2)*d(1) can be evaluated conventionally.

--------------

handling simple tensor contraction:

whenever two factors of identical type are contracted without "breaking apart" a factor, the contraction can be
simplified and expedited.  for example,

(A \otimes B) : (C \otimes D) := (A \cdot C)(B \cdot D), which is a scalar value, and therefore can be
computed up front and factored out of the summation.  this step should be done as the expression templates are being
accumulated grammatically (if an index is seen a second time and neither is part of a "down cast", then compute
the contraction immediately and cache it for the later evaluation/assignment).

IDEA: keep a scalar factor in tensor products for storing these sorts of things -- the alternative is to multiply
this scalar in at each step, which for something like a Nonsymmetric3Tensor, would be O(mnp) (where the tensor is
m x n x p).  but perhaps when the end result to calculate is something like an n-vector, saving scalars as separate
factors would allow lazy multiplication of these factors and save time (at the expense of the storage of one extra
scalar value).

--------------

*/

using namespace Tenh;

// general 3-tensor with no symmetries -- most general type of 3-tensor
template <typename F1_, typename F2_, typename F3_>
struct Tensor3_t : Vector_t<typename F1_::Scalar,F1_::DIM*F2_::DIM*F3_::DIM>
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F2_::Scalar>::v>::v };

    typedef Vector_t<typename F1_::Scalar,F1_::DIM*F2_::DIM*F3_::DIM> Parent;
    typedef typename Parent::Scalar Scalar;
    using Parent::DIM;

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
struct Tensor2Simple_t
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F2_::Scalar>::v>::v };

    typedef F1_ F1;
    typedef F2_ F2;
    typedef typename F1::Scalar Scalar;
    // there is no dimension because this is not a vector space (not even a manifold -- it is an algebraic variety)

    Tensor2Simple_t (F1 const &f1, F2 const &f2) : m1(f1), m2(f2) { }

    F1 const &factor1 () const { return m1; }
    F2 const &factor2 () const { return m2; }

    Scalar operator [] (typename Tensor2_t<F1,F2>::DeprecatedIndex const &i) const
    {
        if (i.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return m1[i.subindex1().value()] * m2[i.subindex2().value()];
    }


    // TODO: type cast operator to NonsymmetricTensor which creates an expression template to do the indexing

    // TODO: the sum of two Tensor2Simple_t<F1,F2>s is a Tensor2_t<F1,F2>

private:

    F1 m1;
    F2 m2;
};

template <typename F1, typename F2>
std::ostream &operator << (std::ostream &out, Tensor2Simple_t<F1,F2> const &s)
{
    return out << s.factor1() << " \\otimes " << s.factor2();
}

// // template specialization for the contraction of simple 2-tensors
// // (A \otimes B) : (C \otimes D) := (A \cdot C)(B \cdot D)
// template <typename F1, typename F2>
// struct DotProduct_t<Tensor2Simple_t<F1,F2> >
// {
//     typedef Tensor2Simple_t<F1,F2> Tensor2Simple;
//     typedef typename Tensor2Simple::Scalar Scalar;
//
//     static Scalar eval (Tensor2Simple const &l, Tensor2Simple const &r)
//     {
//         return DotProduct_t<F1>::eval(l.factor1(),r.factor1()) * DotProduct_t<F2>::eval(l.factor2(),r.factor2());
//     }
// };

// Tensor3Simple_t

template <typename F1_, typename F2_, typename F3_>
struct Tensor3Simple_t
{
    enum { _ = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F2_::Scalar>::v>::v &&
               Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename F1_::Scalar,typename F3_::Scalar>::v>::v };

    typedef F1_ F1;
    typedef F2_ F2;
    typedef F3_ F3;
    typedef typename F1::Scalar Scalar;
    // there is no dimension because this is not a vector space (not even a manifold -- it is an algebraic variety)

    Tensor3Simple_t (F1 const &f1, F2 const &f2, F3 const &f3) : m1(f1), m2(f2), m3(f3) { }

    F1 const &factor1 () const { return m1; }
    F2 const &factor2 () const { return m2; }
    F3 const &factor3 () const { return m3; }

    Scalar operator [] (typename Tensor3_t<F1,F2,F3>::Index const &i) const
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
std::ostream &operator << (std::ostream &out, Tensor3Simple_t<F1,F2,F3> const &s)
{
    return out << s.factor1() << " \\otimes " << s.factor2() << " \\otimes " << s.factor3();
}

// // template specialization for the contraction of simple 2-tensors
// // (A \otimes B \otimes C) : (P \otimes Q \otimes R) := (A \cdot P)(B \cdot Q)(C \cdot R)
// template <typename F1, typename F2, typename F3>
// struct DotProduct_t<Tensor3Simple_t<F1,F2,F3> >
// {
//     typedef Tensor3Simple_t<F1,F2,F3> Tensor3Simple;
//     typedef typename Tensor3Simple::Scalar Scalar;
//
//     static Scalar eval (Tensor3Simple const &l, Tensor3Simple const &r)
//     {
//         return DotProduct_t<F1>::eval(l.factor1(),r.factor1()) *
//                DotProduct_t<F2>::eval(l.factor2(),r.factor2()) *
//                DotProduct_t<F3>::eval(l.factor3(),r.factor3());
//     }
// };

// TODO: symmetric outer product of one vector with itself producing a special type of simple tensor
// TODO: symmetrization and antisymmetrization of outer product

template <typename F1, typename F2>
Tensor2Simple_t<F1,F2> operator % (F1 const &l, F2 const &r)
{
    return Tensor2Simple_t<F1,F2>(l, r);
}

template <typename F1, typename F2, typename F3>
Tensor3Simple_t<F1,F2,F3> operator % (F1 const &l, Tensor2Simple_t<F2,F3> const &r)
{
    return Tensor3Simple_t<F1,F2,F3>(l, r.factor1(), r.factor2());
}

template <typename F1, typename F2, typename F3>
Tensor3Simple_t<F1,F2,F3> operator % (Tensor2Simple_t<F1,F2> const &l, F3 const &r)
{
    return Tensor3Simple_t<F1,F2,F3>(l.factor1(), l.factor2(), r);
}

typedef Vector_t<float,2> Float2;
typedef Vector_t<float,3> Float3;
typedef Vector_t<float,4> Float4;
typedef Vector_t<float,5> Float5;
typedef Tensor2_t<Float3,Float4> Float3x4;
typedef Tensor2Simple_t<Float3,Float4> SimpleFloat3x4;
typedef Tensor3_t<Float2,Float3,Float4> Float2x3x4;
typedef Tensor3Simple_t<Float2,Float3,Float4> SimpleFloat2x3x4;
typedef Tensor2_t<Float3,Float3> Float3x3;
typedef Tensor2_t<Float4,Float5> Float4x5;
typedef Tensor2_t<Float5,Float2> Float5x2;

void foo (float x) { std::cout << "foo(" << x << ")\n"; }
void bar (Float3 const &x) { std::cout << "bar(" << x << ")\n"; }
void bor (Float4 const &x) { std::cout << "bor(" << x << ")\n"; }


template <Uint32 DIM>
void test_Tensor2Symmetric_t ()
{
    std::cout << "test_Tensor2Symmetric_t<" << DIM << ">\n";

    typedef Vector_t<float,DIM> Vector;
    typedef Tensor2Symmetric_t<Vector> Tensor2Symmetric;
    std::cout << FORMAT_VALUE(Tensor2Symmetric::DIM) << '\n';

    Tensor2Symmetric a(Static<>::WITHOUT_INITIALIZATION);
    Tensor2Symmetric b(Static<>::WITHOUT_INITIALIZATION);
    for (typename Tensor2Symmetric::Index i; i.is_not_at_end(); ++i)
    {
        a[i] = i.value() + 1;
        b[i] = sqr(i.value()) + 5;
    }
    std::cout << FORMAT_VALUE(a) << '\n';
    std::cout << FORMAT_VALUE(b) << '\n';
//     a.template expr<'i'>();
//     std::cout << FORMAT_VALUE(a.template expr<'i'>()*b.template expr<'i'>()) << '\n';
    //std::cout << FORMAT_VALUE(a.template expr<'i'>() + b.template expr<'i'>()) << '\n';
    TypedIndex_t<Tensor2Symmetric,'q'> q;
    TypedIndex_t<Vector,'i'> i;
    TypedIndex_t<Vector,'j'> j;
    std::cout << FORMAT_VALUE(a(q)*b(q)) << '\n';
    std::cout << FORMAT_VALUE(a(i|j)*b(i|j)) << '\n';

    float hand_computed_value = 0.0f;
    for (typename Tensor2Symmetric::MultiIndex i; i.is_not_at_end(); ++i)
    {
        std::cout << '.';
        hand_computed_value += a[i]*b[i];
    }
    std::cout << FORMAT_VALUE(hand_computed_value) << '\n';
    std::cout << '\n';

    Vector v(Static<>::WITHOUT_INITIALIZATION);
    Vector w(Static<>::WITHOUT_INITIALIZATION);
    for (typename Vector::Index i; i.is_not_at_end(); ++i)
        v[i] = i.value() + 5;
//     a.template expr<'i','j'>();
//     a.template expr<'i','j'>() * v.template expr<'j'>();
//     w.template expr<'i'>() = a.template expr<'i','j'>() * v.template expr<'j'>();
    std::cout << FORMAT_VALUE(v) << '\n';
//    std::cout << FORMAT_VALUE(w) << '\n';

    w(i) = a(i|j)*v(j);
    std::cout << FORMAT_VALUE(w) << '\n';
    std::cout << FORMAT_VALUE((v(i)*a(i|j)*v(j))) << '\n';
    std::cout << '\n';
    
    std::cout << "zero'ing out the diagonal of a\n";
    for (typename Vector::Index i; i.is_not_at_end(); ++i)
        a.set_component(i,i,0);
    std::cout << FORMAT_VALUE(a) << '\n';
    std::cout << '\n';

//     std::cout << "checking Tensor2Symmetric_t::contiguous_index_to_rowcol_index()\n";
//     for (typename Tensor2Symmetric::Index k; k.is_not_at_end(); ++k)
//     {
//         Uint32 row;
//         Uint32 col;
//         Tensor2Symmetric::contiguous_index_to_rowcol_index(k.value(), row, col);
//         std::cout << '(' << row << ',' << col << "), ";
//     }
//     std::cout << '\n';
//     for (typename Tensor2Symmetric::Index k; k.is_not_at_end(); ++k)
//     {
//         Uint32 row;
//         Uint32 col;
//         Tensor2Symmetric::contiguous_index_to_rowcol_index(k.value(), row, col);
//         Uint32 k2 = Tensor2Symmetric::rowcol_index_to_contiguous_index(row, col);
//         std::cout << '(' << k.value() << ',' << k2 << "), ";
//         assert(k.value() == k2);
//     }
//     std::cout << '\n';
//     std::cout << '\n';
}

template <Uint32 DIM>
void test_Tensor2Antisymmetric_t ()
{
    std::cout << "test_Tensor2Antisymmetric_t<" << DIM << ">\n";

    typedef Vector_t<float,DIM> Vector;
    typedef Tensor2Antisymmetric_t<Vector> Tensor2Antisymmetric;
    std::cout << FORMAT_VALUE(Tensor2Antisymmetric::DIM) << '\n';

    Tensor2Antisymmetric a(Static<>::WITHOUT_INITIALIZATION);
    Tensor2Antisymmetric b(Static<>::WITHOUT_INITIALIZATION);
    for (typename Tensor2Antisymmetric::Index i; i.is_not_at_end(); ++i)
    {
        a[i] = i.value() + 1;
        b[i] = sqr(i.value()) + 5;
    }
    std::cout << FORMAT_VALUE(a) << '\n';
    std::cout << FORMAT_VALUE(b) << '\n';
//     a.template expr<'i'>();
//     std::cout << FORMAT_VALUE(a.template expr<'i'>()*b.template expr<'i'>()) << '\n';
    //std::cout << FORMAT_VALUE(a.template expr<'i'>() + b.template expr<'i'>()) << '\n';

    float hand_computed_value = 0.0f;
    for (typename Tensor2Antisymmetric::MultiIndex i; i.is_not_at_end(); ++i)
        hand_computed_value += a[i]*b[i];
    std::cout << FORMAT_VALUE(hand_computed_value) << '\n';
    std::cout << '\n';

    Vector v(Static<>::WITHOUT_INITIALIZATION);
    Vector w(Static<>::WITHOUT_INITIALIZATION);
    for (typename Vector::Index i; i.is_not_at_end(); ++i)
        v[i] = i.value() + 5;
//     a.template expr<'i','j'>();
//     a.template expr<'i','j'>() * v.template expr<'j'>();
//     w.template expr<'i'>() = a.template expr<'i','j'>() * v.template expr<'j'>();
    std::cout << FORMAT_VALUE(v) << '\n';
    std::cout << FORMAT_VALUE(w) << '\n';

    TypedIndex_t<Vector,'i'> i;
    TypedIndex_t<Vector,'j'> j;
    w(i) = a(i|j)*v(j);
    std::cout << FORMAT_VALUE(w) << '\n';
    std::cout << FORMAT_VALUE((v(i)*a(i|j)*v(j))) << '\n';
    std::cout << '\n';

//     std::cout << "checking Tensor2Antisymmetric_t::contiguous_index_to_rowcol_index()\n";
//     for (typename Tensor2Antisymmetric::Index k; k.is_not_at_end(); ++k)
//     {
//         Uint32 row;
//         Uint32 col;
//         Tensor2Antisymmetric::contiguous_index_to_rowcol_index(k.value(), row, col);
//         std::cout << '(' << row << ',' << col << "), ";
//     }
//     std::cout << '\n';
//     for (typename Tensor2Antisymmetric::Index k; k.is_not_at_end(); ++k)
//     {
//         Uint32 row;
//         Uint32 col;
//         Tensor2Antisymmetric::contiguous_index_to_rowcol_index(k.value(), row, col);
//         Uint32 k2 = Tensor2Antisymmetric::rowcol_index_to_contiguous_index(row, col);
//         assert(k.value() == k2);
//     }
//     std::cout << '\n';
//     std::cout << '\n';
}

template <Uint32 DIM>
void test_symmetric_and_antisymmetric_2_tensors ()
{
    std::cout << "test_symmetric_and_antisymmetric_2_tensors<" << DIM << ">\n";

    typedef Vector_t<float,DIM> Vector;
    typedef Tensor2_t<Vector,Vector> Tensor2;
    typedef Tensor2Symmetric_t<Vector> Tensor2Symmetric;
    typedef Tensor2Antisymmetric_t<Vector> Tensor2Antisymmetric;
    std::cout << FORMAT_VALUE(TypeStringOf_t<Tensor2>::eval()) << '\n';
    std::cout << FORMAT_VALUE(TypeStringOf_t<Tensor2Symmetric>::eval()) << '\n';
    std::cout << FORMAT_VALUE(TypeStringOf_t<Tensor2Antisymmetric>::eval()) << '\n';

    Tensor2Symmetric s(Static<>::WITHOUT_INITIALIZATION);
    Tensor2Antisymmetric a(Static<>::WITHOUT_INITIALIZATION);
    for (typename Tensor2Symmetric::Index i; i.is_not_at_end(); ++i)
        s[i] = i.value() + 1;
    for (typename Tensor2Antisymmetric::Index i; i.is_not_at_end(); ++i)
        a[i] = sqr(i.value()) + 5;
    std::cout << FORMAT_VALUE(s) << '\n';
    std::cout << FORMAT_VALUE(a) << '\n';

    Tensor2 c(Static<>::WITHOUT_INITIALIZATION);
    {
        {
            TypedIndex_t<Vector,'i'> i;
            TypedIndex_t<Vector,'j'> j;
            TypedIndex_t<Vector,'k'> k;
            std::cout << "full contraction of symmetric with antisymmetric (should be exactly zero): " << FORMAT_VALUE(s(i|j)*a(i|j)) << '\n';
            c(i|k) = s(i|j)*a(j|k);
        }
        std::cout << "product s(i|j)*a(j|k) = " << c << '\n';

        for (typename Vector::Index i; i.is_not_at_end(); ++i)
            for (typename Vector::Index k; k.is_not_at_end(); ++k)
            {
                c[typename Tensor2::MultiIndex(i,k)] = 0;
                for (typename Vector::Index j; j.is_not_at_end(); ++j)
                    c[typename Tensor2::MultiIndex(i,k)] += s[typename Tensor2Symmetric::MultiIndex(i,j)] *
                                                               a[typename Tensor2Antisymmetric::MultiIndex(j,k)];
            }
        std::cout << "hand-computed value = " << c << '\n';
    }
    {
        TypedIndex_t<Vector,'i'> i;
        TypedIndex_t<Vector,'j'> j;
        c(i|j) = s(i|j) + a(i|j);
        std::cout << "sum s(i|j)+a(i|j) = " << c << '\n';
    }
    std::cout << '\n';
}

template <Uint32 FACTOR1_DIM, Uint32 FACTOR2_DIM>
void test_scalar_multiplication_and_division ()
{
    std::cout << "test_scalar_multiplication_and_division<" << FACTOR1_DIM << ',' << FACTOR2_DIM << ">\n";

    typedef Vector_t<float,FACTOR1_DIM> Factor1;
    typedef Vector_t<float,FACTOR2_DIM> Factor2;
    typedef Tensor2_t<Factor1,Factor2> Tensor2;
    std::cout << FORMAT_VALUE(TypeStringOf_t<Factor1>::eval()) << '\n';
    std::cout << FORMAT_VALUE(TypeStringOf_t<Factor2>::eval()) << '\n';
    std::cout << FORMAT_VALUE(TypeStringOf_t<Tensor2>::eval()) << '\n';
    std::cout << '\n';

    Factor1 u1(Static<>::WITHOUT_INITIALIZATION);
    Factor1 u2(Static<>::WITHOUT_INITIALIZATION);
    for (typename Factor1::Index i; i.is_not_at_end(); ++i)
        u1[i] = 3*i.value() + 8;
    std::cout << FORMAT_VALUE(u1) << '\n';
    std::cout << '\n';

    TypedIndex_t<Factor1,'i'> i;
    u2(i) = u1(i) * 3.0f;
    std::cout << "u1(i) * 3.0f = " << u2 << '\n';
    u2(i) = u1(i) * 3; // testing scalar multiplication using integer literal
    std::cout << "u1(i) * 3 (using integer literal for scalar) = " << u2 << '\n';
    std::cout << '\n';

    u2(i) = 20.0f * u1(i);
    std::cout << "20.0f * u1(i) = " << u2 << '\n';
    u2(i) = 20 * u1(i); // testing scalar multiplication using integer literal
    std::cout << "20 * u1(i) (using integer literal for scalar) = " << u2 << '\n';
    std::cout << '\n';

    u2(i) = u1(i) / 2.0f;
    std::cout << "u1(i) / 2.0f = " << u2 << '\n';
    u2(i) = u1(i) / 2; // testing scalar multiplication using integer literal
    std::cout << "u1(i) / 2 (using integer literal for scalar) = " << u2 << '\n';
    std::cout << '\n';

    u2(i) = -u1(i);
    std::cout << "-u1(i) = " << u2 << '\n';
    std::cout << '\n';

    Factor2 v1(Static<>::WITHOUT_INITIALIZATION);
    Factor2 v2(Static<>::WITHOUT_INITIALIZATION);
    for (typename Factor2::Index i; i.is_not_at_end(); ++i)
        v1[i] = sqr(i.value());
    std::cout << FORMAT_VALUE(v1) << '\n';

    Tensor2 a(Static<>::WITHOUT_INITIALIZATION);
    Tensor2 b(Static<>::WITHOUT_INITIALIZATION);
    TypedIndex_t<Factor2,'j'> j;
    a(i|j) = u1(i)*v1(j);
    std::cout << "a(i|j) = u1(i)*v1(j) = " << a << '\n';
    b(i|j) = 42 * a(i|j);
    std::cout << "b(i|j) = 42 * a(i|j) = " << b << '\n';
    std::cout << '\n';
}

template <Uint32 FREE_INDEX_COUNT, Uint32 COMPONENT_COUNT1, Uint32 COMPONENT_COUNT2, Uint32 COMPONENT_COUNT3>
void test_printing_expression_templates ()
{
    if (FREE_INDEX_COUNT == 1)
    {
        std::cout << "test_printing_expression_templates<...>() degree 1 expression template, COMPONENT_COUNT1 = " << COMPONENT_COUNT1 << '\n';
        typedef Vector_t<float,COMPONENT_COUNT1> Vector;
        Vector v(Static<>::WITHOUT_INITIALIZATION);
        for (typename Vector::Index i; i.is_not_at_end(); ++i)
            v[i] = sqr(i.value()) + 1;
        TypedIndex_t<Vector,'i'> i;
        std::cout << FORMAT_VALUE(v(i)) << '\n';
        std::cout << '\n';
    }
    else if (FREE_INDEX_COUNT == 2)
    {
        std::cout << "test_printing_expression_templates<...>() degree 2 expression template, COMPONENT_COUNT1 = " << COMPONENT_COUNT1
                                                                                        << ", COMPONENT_COUNT2 = " << COMPONENT_COUNT2 << '\n';
        typedef Vector_t<float,COMPONENT_COUNT1> Factor1;
        typedef Vector_t<float,COMPONENT_COUNT2> Factor2;
        typedef Tensor2_t<Factor1,Factor2> Tensor2;
        Tensor2 a(Static<>::WITHOUT_INITIALIZATION);
        for (typename Tensor2::Index i; i.is_not_at_end(); ++i)
            a[i] = sqr(i.value()) + 1;
        TypedIndex_t<Factor1,'i'> i;
        TypedIndex_t<Factor2,'j'> j;
        std::cout << FORMAT_VALUE(a(i|j)) << '\n';
        std::cout << '\n';
    }
    else if (FREE_INDEX_COUNT == 3)
    {
        std::cout << "test_printing_expression_templates<...>() degree 3 expression template, COMPONENT_COUNT1 = " << COMPONENT_COUNT1
                                                                                        << ", COMPONENT_COUNT2 = " << COMPONENT_COUNT2
                                                                                        << ", COMPONENT_COUNT3 = " << COMPONENT_COUNT3 << '\n';
        typedef Vector_t<float,COMPONENT_COUNT1> Factor1;
        typedef Vector_t<float,COMPONENT_COUNT2> Factor2;
        typedef Vector_t<float,COMPONENT_COUNT3> Factor3;
        typedef Tensor2_t<Factor1,Factor2> Tensor2;
        Tensor2 a(Static<>::WITHOUT_INITIALIZATION);
        Factor3 v(Static<>::WITHOUT_INITIALIZATION);
        for (typename Tensor2::Index i; i.is_not_at_end(); ++i)
            a[i] = sqr(i.value()) + 1;
        for (typename Factor3::Index i; i.is_not_at_end(); ++i)
            v[i] = i.value() + 2;
        TypedIndex_t<Factor1,'i'> i;
        TypedIndex_t<Factor2,'j'> j;
        TypedIndex_t<Factor3,'k'> k;
        // Tensor3_t isn't implemented yet, so use a decomposable tensor
        std::cout << FORMAT_VALUE(a(i|j)*v(k)) << '\n';
        std::cout << '\n';
    }
    else
        assert(false && "not implemented");
}

template <Uint32 DIM>
void test_IndexBundle ()
{
    std::cout << "test_IndexBundle<" << DIM << ">()\n";

    typedef Vector_t<float,DIM> Vector;
    typedef Tensor2_t<Vector,Vector> Tensor2;
    typedef Tensor2Symmetric_t<Vector> Tensor2Symmetric;
    typedef Tensor2Antisymmetric_t<Vector> Tensor2Antisymmetric;

    Tensor2 t(Static<>::WITHOUT_INITIALIZATION);
    for (typename Tensor2::Index k; k.is_not_at_end(); ++k)
        t[k] = k.value();
    std::cout << FORMAT_VALUE(t) << '\n';

    {
        std::cout << "*** using TypedIndex_t with operator |\n";
        TypedIndex_t<Vector,'I'> I;
        TypedIndex_t<Vector,'J'> J;
        Vector v(3);
        std::cout << FORMAT_VALUE((v(I)*t(I|J)*v(J))) << '\n';
        std::cout << FORMAT_VALUE(t(I|J)) << '\n';
        std::cout << '\n';
    }


    typedef TypedIndex_t<Vector,'i'> I;
    typedef TypedIndex_t<Vector,'j'> J;
    typedef TypedIndex_t<Tensor2Symmetric,'q'> Q;
    typedef TypedIndex_t<Tensor2Antisymmetric,'p'> P;
    typedef TypeList_t<I,TypeList_t<J> > BundleIndexTypeList;
    typedef ExpressionTemplate_IndexedObject_t<Tensor2,BundleIndexTypeList,EmptyTypeList,DONT_FORCE_CONST> EI;
    I i;
    J j;
    typedef TypeList_t<I,TypeList_t<J> > BundleIndexTypeList;
    Q q;
    P p;
    {
        Tensor2 const &t2 = t;
        typedef ExpressionTemplate_IndexedObject_t<Tensor2,BundleIndexTypeList,EmptyTypeList,FORCE_CONST> EI2;
        EI2 ei(t2(i|j));
    }
    for (typename Tensor2Symmetric::Index r; r.is_not_at_end(); ++r)
        std::cout << FORMAT_VALUE((Tensor2Symmetric::template bundle_index_map<BundleIndexTypeList,Q>(r))) << '\n';
    std::cout << '\n';

    typedef MultiIndex_t<BundleIndexTypeList> (*BundleIndexMap) (Q const &);
    typedef ExpressionTemplate_IndexBundle_t<EI,BundleIndexTypeList,Q> EB;
    EB eb(t(i|j));
    for (typename EB::MultiIndex c; c.is_not_at_end(); ++c)
        std::cout << FORMAT_VALUE(c) << " --> " << FORMAT_VALUE(eb[c]) << '\n';
    std::cout << '\n';

    Tensor2Symmetric s(Static<>::WITHOUT_INITIALIZATION);
    s(q) = eb;
    std::cout << FORMAT_VALUE(s) << '\n';
    std::cout << '\n';

    std::cout << FORMAT_VALUE((t(i|j).bundle(i|j,q))) << '\n';
    std::cout << '\n';

    s(q) = ((t(i|j) + t(j|i))/2).bundle(i|j,q);
    std::cout << "(t(i|j) + t(j|i))/2 = " << FORMAT_VALUE(s) << '\n';
    std::cout << '\n';

    std::cout << (0.5f*(t(i|j) + t(j|i))).bundle(i|j,q) << '\n';
    std::cout << '\n';
    std::cout << (0.5f*(t(i|j) - t(j|i))).bundle(i|j,p) << '\n';
    std::cout << '\n';

    Tensor2Antisymmetric a(Static<>::WITHOUT_INITIALIZATION);
    s(q) = ((t(i|j) + t(j|i))/2).bundle(i|j,q);
    a(p) = ((t(i|j) - t(j|i))/2).bundle(i|j,p);
    std::cout << FORMAT_VALUE(t) << '\n';
    std::cout << "symmetric part of t: " << FORMAT_VALUE(s) << '\n';
    std::cout << "antisymmetric part of t: " << FORMAT_VALUE(a) << '\n';
    Tensor2 x(Static<>::WITHOUT_INITIALIZATION);
    x(i|j) = s(i|j) + a(i|j);
    std::cout << "recombined: " << FORMAT_VALUE(x) << '\n';

    x(i|j).bundle(i|j,q);

    typedef TypedIndex_t<Tensor2,'g'> G;
    G g;
    std::cout << FORMAT_VALUE(x(i|j).bundle(i|j,g)) << '\n';
    std::cout << '\n';
}

template <Uint32 DIM>
void test_IndexSplit ()
{
    std::cout << "test_IndexSplit<" << DIM << ">()\n";

    typedef Vector_t<float,DIM> Vector;

//     {
//         typedef Tensor2_t<Vector,Vector> T;
//         T t(Static_t<WithoutInitialization>::SINGLETON);
//         for (typename T::Index i; i.is_not_at_end(); ++i)
//             t[i] = i.value() + 1;
//         std::cout << FORMAT_VALUE(t) << '\n';
//         for (typename Vector::Index m; m.is_not_at_end(); ++m)
//         {
//             for (typename Vector::Index n; n.is_not_at_end(); ++n)
//             {
//                 std::cout << t.component(m,n) << ", ";
//             }
//             std::cout << '\n';
//         }
//         std::cout << '\n';
//     }
    {
        typedef Tensor2Antisymmetric_t<Vector> A;
        A a(Static_t<WithoutInitialization>::SINGLETON);
        for (typename A::Index k; k.is_not_at_end(); ++k)
            a[k] = k.value() + 1;
        std::cout << FORMAT_VALUE(a) << '\n';
        TypedIndex_t<A,'p'> p;
        TypedIndex_t<Vector,'i'> i;
        TypedIndex_t<Vector,'j'> j;
//         std::cout << FORMAT_VALUE(a(p).split(p,i,j)) << '\n';
        for (typename Vector::Index m; m.is_not_at_end(); ++m)
        {
            for (typename Vector::Index n; n.is_not_at_end(); ++n)
            {
                std::cout << a.component(m,n) << ", ";
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    {
        typedef Tensor2Antisymmetric_t<Vector> A;
        typedef Tensor2_t<A,Vector> T;
        T t(Static_t<WithoutInitialization>::SINGLETON);
        for (typename T::Index i; i.is_not_at_end(); ++i)
            t[i] = i.value() + 1;
        std::cout << FORMAT_VALUE(t) << '\n';
        for (typename Vector::Index k; k.is_not_at_end(); ++k)
        {
            for (typename Vector::Index i; i.is_not_at_end(); ++i)
            {
                for (typename Vector::Index j; j.is_not_at_end(); ++j)
                {
                    std::cout << t.component(typename A::MultiIndex(i,j),k) << ", ";
                }
                std::cout << '\n';
            }
            std::cout << "------------\n";
        }
        std::cout << '\n';
    }

    {
        typedef Tensor2Antisymmetric_t<Vector> Tensor2Antisymmetric;
        typedef Tensor2Symmetric_t<Tensor2Antisymmetric> Tensor2Symmetric;

        Tensor2Symmetric s(Static<>::WITHOUT_INITIALIZATION);
        for (typename Tensor2Symmetric::Index k; k.is_not_at_end(); ++k)
            s[k] = k.value() + 1;
        std::cout << FORMAT_VALUE(s) << '\n';

        TypedIndex_t<Tensor2Antisymmetric,'p'> p;
        TypedIndex_t<Tensor2Antisymmetric,'q'> q;
        TypedIndex_t<Vector,'i'> i;
        TypedIndex_t<Vector,'j'> j;
        TypedIndex_t<Vector,'k'> k;
        TypedIndex_t<Vector,'l'> l;
        s(p|q).split(p,i|j);
        std::cout << FORMAT_VALUE(s(p|q).split(p,i|j)) << '\n';
        std::cout << FORMAT_VALUE(s(p|q).split(p,i|j) + s(p|q).split(p,j|i)) << '\n';        
        std::cout << "Ricci curvature-like thing: " << FORMAT_VALUE(s(p|q).split(p,i|j).split(q,i|k)) << '\n';        
        std::cout << "Scalar curvature-like thing: " << FORMAT_VALUE(s(p|q).split(p,i|j).split(q,i|j)) << '\n';        
        std::cout << '\n';
    }

//     typedef Tensor2_t<Tensor2_t<Vector,Vector>,Tensor2Antisymmetric> T;
//     T t(Static<>::WITHOUT_INITIALIZATION);
//     t(i,j,q) = s(p,q).split(p,i,j);
//     std::cout << FORMAT_VALUE(t) << '\n';
//     std::cout << '\n';
/*
    typedef TypedIndex_t<Vector,'i'> I;
    typedef TypedIndex_t<Vector,'j'> J;
    typedef TypedIndex_t<Tensor2Symmetric,'q'> Q;
    typedef TypedIndex_t<Tensor2Antisymmetric,'p'> P;
    typedef TypeList_t<I,TypeList_t<J> > BundleIndexTypeList;
    typedef ExpressionTemplate_IndexedObject_t<Tensor2,BundleIndexTypeList,EmptyTypeList,DONT_FORCE_CONST> EI;
    I i;
    J j;
    typedef TypeList_t<I,TypeList_t<J> > BundleIndexTypeList;
    Q q;
    P p;
    {
        Tensor2 const &t2 = t;
        typedef ExpressionTemplate_IndexedObject_t<Tensor2,BundleIndexTypeList,EmptyTypeList,FORCE_CONST> EI2;
        EI2 ei(t2(i,j));
    }
    for (typename Tensor2Symmetric::Index r; r.is_not_at_end(); ++r)
        std::cout << FORMAT_VALUE((Tensor2Symmetric::template bundle_index_map<BundleIndexTypeList,Q>(r))) << '\n';
    std::cout << '\n';

    typedef MultiIndex_t<BundleIndexTypeList> (*BundleIndexMap) (Q const &);
    typedef ExpressionTemplate_IndexBundle_t<EI,BundleIndexTypeList,Q> EB;
    EB eb(t(i,j));
    for (typename EB::MultiIndex c; c.is_not_at_end(); ++c)
        std::cout << FORMAT_VALUE(c) << " --> " << FORMAT_VALUE(eb[c]) << '\n';
    std::cout << '\n';

    Tensor2Symmetric s(Static<>::WITHOUT_INITIALIZATION);
    s(q) = eb;
    std::cout << FORMAT_VALUE(s) << '\n';
    std::cout << '\n';

    std::cout << FORMAT_VALUE((t(i,j).template bundle<BundleIndexTypeList,Q>())) << '\n';
    std::cout << '\n';

    s(q) = ((t(i,j) + t(j,i))/2).template bundle<BundleIndexTypeList,Q>();
    std::cout << "(t(i,j) + t(j,i))/2 = " << FORMAT_VALUE(s) << '\n';
    std::cout << '\n';

    std::cout << (0.5f*(t(i,j) + t(j,i))).bundle(i,j,q) << '\n';
    std::cout << '\n';
    std::cout << (0.5f*(t(i,j) - t(j,i))).bundle(i,j,p) << '\n';
    std::cout << '\n';

    Tensor2Antisymmetric a(Static<>::WITHOUT_INITIALIZATION);
    s(q) = ((t(i,j) + t(j,i))/2).bundle(i,j,q);
    a(p) = ((t(i,j) - t(j,i))/2).bundle(i,j,p);
    std::cout << FORMAT_VALUE(t) << '\n';
    std::cout << "symmetric part of t: " << FORMAT_VALUE(s) << '\n';
    std::cout << "antisymmetric part of t: " << FORMAT_VALUE(a) << '\n';
    Tensor2 x(Static<>::WITHOUT_INITIALIZATION);
    x(i,j) = s(i,j) + a(i,j);
    std::cout << "recombined: " << FORMAT_VALUE(x) << '\n';

    x(i,j).template bundle<typename TypeTuple_t<I,J>::T,Q>();

    typedef TypedIndex_t<Tensor2,'g'> G;
    G g;
    std::cout << FORMAT_VALUE(x(i,j).bundle(i,j,g)) << '\n';
    std::cout << '\n';
    */
}

template <Uint32 ROWS, Uint32 COLS>
void test_Tensor2Diagonal_t ()
{
    std::cout << "test_Tensor2Diagonal_t<" << ROWS << ',' << COLS << ">\n";

    typedef Vector_t<float,ROWS> V1;
    typedef Vector_t<float,COLS> V2;
    typedef Tensor2Diagonal_t<V1,V2> Tensor2Diagonal;

    Tensor2Diagonal t(Static<>::WITHOUT_INITIALIZATION);
    for (typename Tensor2Diagonal::Index i; i.is_not_at_end(); ++i)
        t[i] = i.value()+1;
    std::cout << FORMAT_VALUE(t) << '\n';
}

int main (int argc, char **argv)
{
    // 1-dimensional vector to scalar coercion
    {
        typedef Vector_t<float,1> Float1;
        Float1 v(3);
        std::cout << "type coercion from Vector_t<float,1> to float:\n";
        foo(v);
        v.as_scalar() = 2;
        std::cout << "assignment via float coercion (v should equal (2)): " << FORMAT_VALUE(v) << "\n\n";

        // this should produce a compile error -- no canonical conversion from 2d vector to scalar
//         typedef Vector_t<float,2> Float2;
//         Float2 w(4);
//         foo(w);

        typedef Tensor2_t<Float1,Float1> Float1x1;
        Float1x1 m(4);
        std::cout << "type coercion from Tensor2_t<Float1,Float1> to float:\n";
        foo(m);
        m = 8;
        std::cout << "assignment via float coercion (m should equal [8]): " << FORMAT_VALUE(m) << "\n\n";

        // uncommenting the following should produce a compile error (no type conversion to Float3)
//         Float3 x(b);
    }


//     // testing various tensor access and operations
//     {
//         Float2 u(10,20);
//         Float3 v(1,2,3);
//         Float4 w(4,5,6,7);
//
//         std::cout << FORMAT_VALUE(u) << '\n';
//         std::cout << FORMAT_VALUE(v) << '\n';
//         std::cout << FORMAT_VALUE(w) << '\n';
//
//         SimpleFloat3x4 X(v,w);
//
//         std::cout << "X := v \\otimes w = " << X << ", and " << v%w << '\n';
//
//
//         for (Uint32 r = 0; r < 3; ++r)
//         {
//             for (Uint32 c = 0; c < 4; ++c)
//             {
//                 std::cout << "\t" << X[Float3x4::DeprecatedIndex(r,c)];
//             }
//             std::cout << '\n';
//         }
//
//         std::cout << '\n';
//
//
//         for (Uint32 i = 0; i < Float3x4::DIM; ++i)
//         {
//             Float3x4::DeprecatedIndex index(i);
//             std::cout << i << " -> " << index.subindex1().value() << ", " << index.subindex2().value()
//                       << " -> " << Float3x4::DeprecatedIndex(index.subindex1().value(), index.subindex2().value()).value() << '\n';
//         }
//
//         std::cout << '\n';
//
//         for (Uint32 i = 0; i < Float2x3x4::DIM; ++i)
//         {
//             Float2x3x4::Index index(i);
//             std::cout << i << " -> " << index.subindex1().value() << ", " << index.subindex2().value() << ", " << index.subindex3().value()
//                       << " -> " << Float2x3x4::Index(index.subindex1().value(), index.subindex2().value(), index.subindex3().value()).value() << '\n';
//         }
//
//         std::cout << '\n';
//
//         SimpleFloat2x3x4 Z(u%v%w);
//         std::cout << "u \\otimes v \\otimes w = " << Z << '\n';
//         for (Uint32 s = 0; s < 2; ++s)
//         {
//             for (Uint32 r = 0; r < 3; ++r)
//             {
//                 for (Uint32 c = 0; c < 4; ++c)
//                 {
//                     std::cout << "\t" << Z[Float2x3x4::Index(s,r,c)];
//                 }
//                 std::cout << '\n';
//             }
//             std::cout << '\n';
//         }
//
//
//         Float3 a(0,-2,4);
//         Float4 b(8,4,2,1);
//         SimpleFloat3x4 Y(a,b);
//
//         std::cout << "a = " << a << '\n';
//         std::cout << "b = " << b << '\n';
//         std::cout << "Y := a \\otimes b = " << Y << '\n';
//
//         for (Uint32 r = 0; r < 3; ++r)
//         {
//             for (Uint32 c = 0; c < 4; ++c)
//             {
//                 std::cout << "\t" << Y[Float3x4::DeprecatedIndex(r,c)];
//             }
//             std::cout << '\n';
//         }
//
//         std::cout << '\n';
//
//         std::cout << "X * Y = " << DotProduct_t<SimpleFloat3x4>::eval(X,Y) << " (";
//
//         // done the hard way -- compute the inner product of X and Y
//         float accumulator = 0;
//         for (Uint32 r = 0; r < 3; ++r)
//             for (Uint32 c = 0; c < 4; ++c)
//                 accumulator += X[Float3x4::DeprecatedIndex(r,c)] * Y[Float3x4::DeprecatedIndex(r,c)];
//         std::cout << "actual answer = " << accumulator << ")\n\n";
//
//         Float3x3 W(Static<>::WITHOUT_INITIALIZATION);
//         for (Uint32 i = 0; i < 3; ++i)
//             for (Uint32 j = 0; j < 3; ++j)
//                 for (Uint32 k = 0; k < 4; ++k)
//                     W[Float3x3::DeprecatedIndex(i,j)] = X[Float3x4::DeprecatedIndex(i,k)] * Y[Float3x4::DeprecatedIndex(j,k)];
//         for (Uint32 i = 0; i < 3; ++i)
//         {
//             for (Uint32 j = 0; j < 3; ++j)
//                 std::cout << '\t' << W[Float3x3::DeprecatedIndex(i,j)];
//             std::cout << '\n';
//         }
//     }

    // a few type strings
    {
        std::cout << FORMAT_VALUE(TypeStringOf_t<float>::eval()) << '\n';
        std::cout << FORMAT_VALUE(TypeStringOf_t<Float3>::eval()) << '\n';
        std::cout << FORMAT_VALUE(TypeStringOf_t<Float3x3>::eval()) << '\n';
        std::cout << '\n';
    }

    // testing expression templates
    {
        Float3 u(-0.1, 2.0, 8);
        Float3 v(4.1, 5.2, 6.3);
        Float3 w(1.2, -2.0, 3.8);
        typedef TypedIndex_t<Float3,'i'> I;
        typedef TypedIndex_t<Float3,'j'> J;
        I i;
        J j;
        {
            typedef ExpressionTemplate_IndexedObject_t<Float3,TypeList_t<I>,EmptyTypeList,DONT_FORCE_CONST> EI;
            typedef ExpressionTemplate_IndexedObject_t<Float3,TypeList_t<J>,EmptyTypeList,DONT_FORCE_CONST> EJ;

            std::cout << i << '\n';
            std::cout << Float3::Index(0) << '\n';
            v(i);
            std::cout << FORMAT_VALUE(v(i)[EI::MultiIndex(0)]) << '\n';

            std::cout << FORMAT_VALUE(v(j)[EJ::MultiIndex(1)]) << '\n';

            for (Float3::Index k; k.is_not_at_end(); ++k)
                std::cout << u[k] + v[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "addition:\n";
            typedef ExpressionTemplate_IndexedObject_t<Float3,TypeList_t<I>,EmptyTypeList,DONT_FORCE_CONST> EE;
            typedef ExpressionTemplate_Addition_t<EE,EE,'+'> EA;
            EA e(u(i), v(i));
            std::cout << "expression template value:\n";
            for (EA::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';
            std::cout << "hand-calculated value:\n";
            for (Float3::Index k; k.is_not_at_end(); ++k)
                std::cout << u[k] + v[k] << ", ";
            std::cout << '\n';

            std::cout << "operator +\n";
            EA e2(u(i) + v(i));
            for (EA::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "inner product:\n";
            typedef ExpressionTemplate_IndexedObject_t<Float3,TypeList_t<I>,EmptyTypeList,DONT_FORCE_CONST> EE;
            typedef ExpressionTemplate_Multiplication_t<EE,EE> EM;
            EM e(u(i), v(i));
            Float3::Index k;
            std::cout << FORMAT_VALUE(k.COMPONENT_COUNT) << '\n';
            std::cout << FORMAT_VALUE(TypeStringOf_t<EE::MultiIndex>::eval()) << '\n';
            std::cout << "expression template value:\n";
            std::cout << FORMAT_VALUE(e[EM::MultiIndex()]) << '\n';
            std::cout << "hand-calculated value:\n";
            float dot = 0;
            for (Float3::Index k; k.is_not_at_end(); ++k)
                dot += u[k]*v[k];
            std::cout << dot << '\n';
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "outer product:\n";
            typedef ExpressionTemplate_IndexedObject_t<Float3,TypeList_t<I>,EmptyTypeList,DONT_FORCE_CONST> EI;
            typedef ExpressionTemplate_IndexedObject_t<Float3,TypeList_t<J>,EmptyTypeList,DONT_FORCE_CONST> EJ;
            typedef ExpressionTemplate_Multiplication_t<EI,EJ> EM;
            EM e(u(i), v(j));
            std::cout << FORMAT_VALUE(TypeStringOf_t<EM::MultiIndex>::eval()) << '\n';
            std::cout << FORMAT_VALUE(TypeStringOf_t<EM::FreeIndexTypeList>::eval()) << '\n';
            std::cout << FORMAT_VALUE(TypeStringOf_t<EM::SummedIndexTypeList>::eval()) << '\n';
            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << FORMAT_VALUE(v) << '\n';
            std::cout << "expression template value:\n";
            for (EM::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';
            std::cout << "hand-calculated value:\n";
            for (Float3::Index ii; ii.is_not_at_end(); ++ii)
                for (Float3::Index jj; jj.is_not_at_end(); ++jj)
                    std::cout << u[ii] * v[jj] << ", ";
            std::cout << '\n';
            std::cout << "operator *\n";
            EM e2(u(i) * v(j));
            for (EM::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "contraction with simple tensor:\n";
            typedef ExpressionTemplate_IndexedObject_t<Float3,TypeList_t<I>,EmptyTypeList,DONT_FORCE_CONST> EI;
            typedef ExpressionTemplate_IndexedObject_t<Float3,TypeList_t<J>,EmptyTypeList,DONT_FORCE_CONST> EJ;
            typedef ExpressionTemplate_Multiplication_t<EI,EJ> EM;
            typedef ExpressionTemplate_Multiplication_t<EM,EJ> EMJ;
            EMJ e(EM(u(i), v(j)), w(j));
            std::cout << FORMAT_VALUE(TypeStringOf_t<EMJ::MultiIndex>::eval()) << '\n';
            std::cout << FORMAT_VALUE(TypeStringOf_t<EMJ::FreeIndexTypeList>::eval()) << '\n';
            std::cout << FORMAT_VALUE(TypeStringOf_t<EMJ::SummedIndexTypeList>::eval()) << '\n';
            std::cout << "expression template value:\n";
            for (EMJ::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';
            std::cout << "hand-calculated value:\n";
            for (Float3::Index ii; ii.is_not_at_end(); ++ii)
            {
                float accum = 0;
                for (Float3::Index jj; jj.is_not_at_end(); ++jj)
                    accum += u[ii] * v[jj] * w[jj];
                std::cout << accum << ", ";
            }
            std::cout << '\n';
            std::cout << "operator *\n";
            EMJ e2(u(i) * v(j) * w(j));
            for (EMJ::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
            // uncommenting this should produce a compile error
//             u(i) * v(i) * w(i);

            // same thing here
//             u(i)*v(i) + w(i);
        }

        {
            std::cout << "addition of 2-tensors:\n";
            typedef TypedIndex_t<Float3x4,'i'> I;
            I i;
            typedef ExpressionTemplate_IndexedObject_t<Float3x4,TypeList_t<I>,EmptyTypeList,DONT_FORCE_CONST> EE;
            typedef ExpressionTemplate_Addition_t<EE,EE,'+'> EA;
            Float3x4 u(Static<>::WITHOUT_INITIALIZATION);
            Float3x4 v(Static<>::WITHOUT_INITIALIZATION);
            for (Float3x4::Index k; k.is_not_at_end(); ++k)
            {
                u[k] = k.value();
                v[k] = 13+k.value();
            }
            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << FORMAT_VALUE(v) << '\n';
            std::cout << FORMAT_VALUE(u(i)[I(0)]) << '\n';
            std::cout << FORMAT_VALUE(u(i)[I(11)]) << '\n';
            EA e(u(i), v(i));
            std::cout << "expression template value:\n";
            for (EA::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';
            std::cout << "hand-calculated value:\n";
            for (Float3x4::Index k; k.is_not_at_end(); ++k)
                std::cout << u[k] + v[k] << ", ";
            std::cout << '\n';
            std::cout << "operator +\n";
            EA e2(u(i) + v(i));
            for (EA::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "expression template for 2-tensor:\n";
            typedef TypedIndex_t<Float3,'i'> I;
            typedef TypedIndex_t<Float4,'j'> J;
            I i;
            J j;
            typedef ExpressionTemplate_IndexedObject_t<Float3x4,TypeTuple_t<I,J>::T,EmptyTypeList,DONT_FORCE_CONST> EIJ;
            typedef ExpressionTemplate_Addition_t<EIJ,EIJ,'+'> EA;
            Float3x4 u(Static<>::WITHOUT_INITIALIZATION);
            Float3x4 v(Static<>::WITHOUT_INITIALIZATION);
            for (Float3x4::Index k; k.is_not_at_end(); ++k)
            {
                u[k] = k.value();
                v[k] = 13+k.value();
            }
            u(i|j);
            u(i|j) + v(i|j);
            EA e(u(i|j), v(i|j));
            for (EA::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';

            std::cout << "operator + with same index order\n";
            EA e2(u(i|j) + v(i|j));
            for (EA::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "symmetrizing a 2-tensor:\n";
            typedef TypedIndex_t<Float3,'i'> I;
            typedef TypedIndex_t<Float3,'j'> J;
            I i;
            J j;
            typedef ExpressionTemplate_IndexedObject_t<Float3x3,TypeTuple_t<I,J>::T,EmptyTypeList,DONT_FORCE_CONST> EIJ;
            typedef ExpressionTemplate_IndexedObject_t<Float3x3,TypeTuple_t<J,I>::T,EmptyTypeList,DONT_FORCE_CONST> EJI;
            typedef ExpressionTemplate_Addition_t<EIJ,EIJ,'+'> EA;
            typedef ExpressionTemplate_Addition_t<EIJ,EJI,'+'> EB;
            Float3x3 u(Static<>::WITHOUT_INITIALIZATION);
            Float3x3 v(Static<>::WITHOUT_INITIALIZATION);
            // dummy values that aren't symmetric
            for (Float3x3::Index k; k.is_not_at_end(); ++k)
            {
                u[k] = sqr(k.value());
                v[k] = sqr(k.value()) + 2*k.value();
            }

            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << FORMAT_VALUE(v) << '\n';

            EB e(u(i|j), u(j|i));
            for (EB::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';

            // uncommenting this should cause an error regarding prohibiting repeated indices in sums
//             typedef ExpressionTemplate_IndexedObject_t<Float3x3,TypeTuple_t<I,I>::T,EmptyTypeList,DONT_FORCE_CONST> EII;
//             typedef ExpressionTemplate_Addition_t<EII,EII,'+'> EC;
//             EC e_bad(u(i,i), u(i,i));

            std::cout << "operator + with same index order\n";
            EA e2(u(i|j) + v(i|j));
            for (EA::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';

            std::cout << "operator + with opposite index order\n";
            EB e3(u(i|j) + v(j|i));
            for (EB::MultiIndex k; k.is_not_at_end(); ++k)
                std::cout << e3[k] << ", ";

            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "2-tensor contraction (matrix multiplication)\n";
            Float3x4 u(Static<>::WITHOUT_INITIALIZATION);
            Float4x5 v(Static<>::WITHOUT_INITIALIZATION);
            Float5x2 w(Static<>::WITHOUT_INITIALIZATION);
            for (Float3x4::Index k; k.is_not_at_end(); ++k)
                u[k] = sqr(k.value());
            for (Float4x5::Index k; k.is_not_at_end(); ++k)
                v[k] = 2*k.value() + 3;
            for (Float5x2::Index k; k.is_not_at_end(); ++k)
                w[k] = k.value() + 1;
            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << FORMAT_VALUE(v) << '\n';
            std::cout << FORMAT_VALUE(w) << '\n';
            typedef TypedIndex_t<Float3,'i'> I;
            typedef TypedIndex_t<Float4,'j'> J;
            typedef TypedIndex_t<Float5,'k'> K;
            typedef TypedIndex_t<Float2,'l'> L;
            I i;
            J j;
            K k;
            L l;
            typedef ExpressionTemplate_IndexedObject_t<Float3x4,TypeTuple_t<I,J>::T,EmptyTypeList,DONT_FORCE_CONST> EIJ;
            typedef ExpressionTemplate_IndexedObject_t<Float4x5,TypeTuple_t<J,K>::T,EmptyTypeList,DONT_FORCE_CONST> EJK;
            typedef ExpressionTemplate_IndexedObject_t<Float5x2,TypeTuple_t<K,L>::T,EmptyTypeList,DONT_FORCE_CONST> EKL;
            typedef ExpressionTemplate_Multiplication_t<EIJ,EJK> EM;
            typedef ExpressionTemplate_Multiplication_t<EM,EKL> EMM;
            std::cout << "expression template contraction u(i,j)*v(j,k):\n";
            EM e(u(i|j), v(j|k));
            for (EM::MultiIndex c; c.is_not_at_end(); ++c)
                std::cout << e[c] << ", ";
            std::cout << '\n';

            std::cout << "hand-computed answer:\n";
            for (Float3::Index a; a.is_not_at_end(); ++a)
            {
                for (Float5::Index b; b.is_not_at_end(); ++b)
                {
                    float accum = 0;
                    for (Float4::Index c; c.is_not_at_end(); ++c)
                        accum += u[Float3x4::MultiIndex(a,c)] * v[Float4x5::MultiIndex(c,b)];
                    std::cout << accum << ", ";
                }
            }
            std::cout << '\n';

            std::cout << "expression template contraction u(i,j)*v(j,k)*w(k,l):\n";
            EMM e2(e, w(k|l));
            for (EMM::MultiIndex c; c.is_not_at_end(); ++c)
                std::cout << e2[c] << ", ";
            std::cout << '\n';

            std::cout << "hand-computed answer:\n";
            for (Uint32 a = 0; a < 3; ++a)
            {
                for (Uint32 b = 0; b < 2; ++b)
                {
                    float accum = 0;
                    for (Uint32 c = 0; c < 4; ++c)
                        for (Uint32 d = 0; d < 5; ++d)
                            accum += u[Float3x4::MultiIndex(a,c)] * v[Float4x5::MultiIndex(c,d)] * w[Float5x2::MultiIndex(d,b)];
                    std::cout << accum << ", ";
                }
            }
            std::cout << '\n';
            std::cout << "operator *:\n";
            EMM e3(u(i|j)*v(j|k)*w(k|l));
            for (EMM::MultiIndex c; c.is_not_at_end(); ++c)
                std::cout << e3[c] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "assignment via expression templates:\n";
            Float3 u(4);
            Float3 v(5,6,7);
            TypedIndex_t<Float3,'i'> i;
            u(i) = v(i);
            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << '\n';

            u(i) = v(i) + v(i);
            std::cout << "v(i) + v(i) = " << FORMAT_VALUE(u) << '\n';
            std::cout << '\n';

            u(i) = v(i) - v(i);
            std::cout << "v(i) - v(i) = " << FORMAT_VALUE(u) << '\n';
            std::cout << '\n';

            Float3x3 m(Static<>::WITHOUT_INITIALIZATION);
            for (Float3x3::Index k; k.is_not_at_end(); ++k)
                m[k] = k.value();
            std::cout << FORMAT_VALUE(m) << '\n';
            std::cout << '\n';

            Float3x3 n(Static<>::WITHOUT_INITIALIZATION);
            for (Float3x3::Index k; k.is_not_at_end(); ++k)
                n[k] = 3*k.value() + 2;
            std::cout << FORMAT_VALUE(n) << '\n';
            std::cout << '\n';

            TypedIndex_t<Float3,'j'> j;

            std::cout << "trace (and testing operator Scalar () type conversion):\n";
            std::cout << FORMAT_VALUE(m(i|i)) << '\n';
            std::cout << FORMAT_VALUE(n(j|j)) << '\n';
            std::cout << '\n';

            std::cout << "addition of traces:\n";
            std::cout << FORMAT_VALUE(m(i|i) + n(j|j)) << '\n';
            std::cout << '\n';

            std::cout << "direct assignment m(i|j) = n(i|j):\n";
            m(i|j) = n(i|j);
            std::cout << FORMAT_VALUE(m) << '\n';
            std::cout << '\n';

            std::cout << "transposed assignment m(i|j) = n(j|i):\n";
            m(i|j) = n(j|i);
            std::cout << FORMAT_VALUE(m) << '\n';
            std::cout << '\n';

            std::cout << "symmetrized assignment m(i|j) = n(i|j) + n(j|i):\n";
            m(i|j) = n(i|j) + n(j|i);
            std::cout << FORMAT_VALUE(m) << '\n';
            std::cout << '\n';

            std::cout << "antisymmetrized assignment m(i|j) = n(i|j) - n(j|i):\n";
            m(i|j) = n(i|j) - n(j|i);
            std::cout << FORMAT_VALUE(m) << '\n';
            std::cout << '\n';
        }

        {
            Float3x3 u(3);
            typedef TypedIndex_t<Float3,'i'> I;
            typedef TypedIndex_t<Float3,'j'> J;
            I i;
            J j;
            std::cout << FORMAT_VALUE(u) << '\n';
            typedef ExpressionTemplate_IndexedObject_t<Float3x3,EmptyTypeList,TypeTuple_t<I>::T,DONT_FORCE_CONST> ET;
            ET::MultiIndex k;
            std::cout << "trace(u) = " << u(i|i)[k] << '\n';
            std::cout << "trace(u) = " << float(u(i|i)) << '\n';
            std::cout << '\n';

            Float3 v(1,2,3);
            u(i|j) = v(i)*v(j);
            std::cout << FORMAT_VALUE(v) << '\n';
            std::cout << "v \\otimes v = ";
            for (Float3x3::Index k; k.is_not_at_end(); ++k)
                std::cout << u[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        // checking for aliasing
        {
            Float3 u(1,2,3);
            Float3 v(4,5,6);
            Float3x3 a(10);
            typedef TypedIndex_t<Float3,'i'> I;
            typedef TypedIndex_t<Float3,'j'> J;
            I i;
            J j;
            std::cout << FORMAT_VALUE(u(i).uses_tensor(u)) << '\n';
            std::cout << FORMAT_VALUE(u(i).uses_tensor(v)) << '\n';
            u(i) = u(i); // this should be a no-op
            try {
                // this should throw an exception
                u(i) = a(i|j)*u(j);
                assert(false && "an exception didn't occur when it was supposed to");
            } catch (std::invalid_argument const &e) {
                std::cout << "correctly caught an exception indicating an aliased template expression\n";
            }
            std::cout << '\n';
        }

        // testing Tensor2Symmetric_t
        test_Tensor2Symmetric_t<2>();
        test_Tensor2Symmetric_t<3>();
        test_Tensor2Symmetric_t<4>();
        test_Tensor2Symmetric_t<10>();

        // testing Tensor2Antisymmetric_t
        test_Tensor2Antisymmetric_t<2>();
        test_Tensor2Antisymmetric_t<3>();
        test_Tensor2Antisymmetric_t<4>();
        test_Tensor2Antisymmetric_t<10>();

        // testing interaction between symmetric and antisymmetric 2-tensors
        test_symmetric_and_antisymmetric_2_tensors<2>();
        test_symmetric_and_antisymmetric_2_tensors<3>();
        test_symmetric_and_antisymmetric_2_tensors<4>();
        test_symmetric_and_antisymmetric_2_tensors<10>();

        // testing scalar multiplication/division of expression templates
        test_scalar_multiplication_and_division<2,1>();
        test_scalar_multiplication_and_division<2,3>();
        test_scalar_multiplication_and_division<4,1>();
        test_scalar_multiplication_and_division<4,4>();

        // testing printing expression templates of various degrees
        test_printing_expression_templates<1,1,1,1>();
        test_printing_expression_templates<1,5,1,1>();
        test_printing_expression_templates<1,20,1,1>();

        test_printing_expression_templates<2,1,1,1>();
        test_printing_expression_templates<2,5,1,1>();
        test_printing_expression_templates<2,1,4,1>();
        test_printing_expression_templates<2,2,2,1>();
        test_printing_expression_templates<2,6,7,1>();

        test_printing_expression_templates<3,1,1,1>();
        test_printing_expression_templates<3,5,1,1>();
        test_printing_expression_templates<3,1,4,1>();
        test_printing_expression_templates<3,2,2,1>();
        test_printing_expression_templates<3,6,7,1>();
        test_printing_expression_templates<3,1,1,3>();
        test_printing_expression_templates<3,5,1,3>();
        test_printing_expression_templates<3,1,4,3>();
        test_printing_expression_templates<3,2,2,3>();
        test_printing_expression_templates<3,6,7,3>();

        // testing index bundling
        test_IndexBundle<2>();
        test_IndexBundle<3>();
        test_IndexBundle<10>();

        // testing index splitting
        test_IndexSplit<2>();
        test_IndexSplit<3>();
        test_IndexSplit<4>();

        // testing Tensor2Diagonal_t
        test_Tensor2Diagonal_t<1,1>();
        test_Tensor2Diagonal_t<1,3>();
        test_Tensor2Diagonal_t<2,2>();
        test_Tensor2Diagonal_t<3,4>();

        {
            std::cout << "********************************\n\n";
            typedef Tensor2_t<Float3,Float4> Float3x4;
            Float3x4 u(Static<>::WITHOUT_INITIALIZATION);
            for (Float3x4::Index i; i.is_not_at_end(); ++i)
                u[i] = i.value() + 1;
            std::cout << FORMAT_VALUE(u) << '\n';

            typedef Tensor2_t<Float4,Float3> Float4x3;
            Float4x3 v(Static<>::WITHOUT_INITIALIZATION);
            TypedIndex_t<Float3,'i'> i;
            TypedIndex_t<Float3,'k'> k;
            TypedIndex_t<Float4,'j'> j;
            v(j|i) = u(i|j);
            std::cout << FORMAT_VALUE(v) << '\n';

            typedef Tensor2Symmetric_t<Float3> Float3x3Symmetric;
            TypedIndex_t<Float3x3Symmetric,'q'> q;
            Float3x3Symmetric s(Static<>::WITHOUT_INITIALIZATION);
            s(q) = (u(i|j)*u(k|j)).bundle(i|k,q);
            std::cout << FORMAT_VALUE(s) << '\n';
        }
    }

    return 0;
}

