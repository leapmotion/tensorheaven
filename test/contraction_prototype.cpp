#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>

#include "core.hpp" // everything should include this
#include "compoundindex.hpp"
#include "expression_templates.hpp"
#include "tensor2.hpp"
#include "typelist.hpp"
#include "typelist_utility.hpp"
#include "typetuple.hpp"
#include "vector.hpp"

WithoutInitialization const WITHOUT_INITIALIZATION = WithoutInitialization();

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

// template specialization for the contraction of simple 2-tensors
// (A \otimes B) : (C \otimes D) := (A \cdot C)(B \cdot D)
template <typename F1, typename F2>
struct DotProduct_t<Tensor2Simple_t<F1,F2> >
{
    typedef Tensor2Simple_t<F1,F2> Tensor2Simple;
    typedef typename Tensor2Simple::Scalar Scalar;

    static Scalar eval (Tensor2Simple const &l, Tensor2Simple const &r)
    {
        return DotProduct_t<F1>::eval(l.factor1(),r.factor1()) * DotProduct_t<F2>::eval(l.factor2(),r.factor2());
    }
};

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

// template specialization for the contraction of simple 2-tensors
// (A \otimes B \otimes C) : (P \otimes Q \otimes R) := (A \cdot P)(B \cdot Q)(C \cdot R)
template <typename F1, typename F2, typename F3>
struct DotProduct_t<Tensor3Simple_t<F1,F2,F3> >
{
    typedef Tensor3Simple_t<F1,F2,F3> Tensor3Simple;
    typedef typename Tensor3Simple::Scalar Scalar;

    static Scalar eval (Tensor3Simple const &l, Tensor3Simple const &r)
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
struct Tensor2Antisymmetric_t : public Vector_t<typename F_::Scalar,((F_::DIM-1)*F_::DIM)/2>
{
    typedef Vector_t<typename F_::Scalar,((F_::DIM-1)*F_::DIM)/2> Parent;
    typedef typename Parent::Scalar Scalar;
    typedef typename Parent::Index Index;
    using Parent::DIM;
    typedef F_ F;

    // TODO: make Index in this class, and make it construct with a Tensor<T,T>::Index

    Scalar &operator [] (typename Tensor2_t<F,F>::Index const &i)
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

int main (int argc, char **argv)
{
    // 1-dimensional vector to scalar coersion
    {
        typedef Vector_t<float,1> Float1;
        Float1 v(3);
        std::cout << "type coersion from Vector_t<float,1> to float:\n";
        foo(v);
        v.as_scalar() = 2;
        std::cout << "assignment via float coersion (v should equal (2)): " << FORMAT_VALUE(v) << "\n\n";

        // this should produce a compile error -- no canonical conversion from 2d vector to scalar
//         typedef Vector_t<float,2> Float2;
//         Float2 w(4);
//         foo(w);

        typedef Tensor2_t<Float1,Float1> Float1x1;
        Float1x1 m(4);
        std::cout << "type coersion from Tensor2_t<Float1,Float1> to float:\n";
        foo(m);
        m = 8;
        std::cout << "assignment via float coersion (m should equal [8]): " << FORMAT_VALUE(m) << "\n\n";

        typedef Tensor2_t<Float3,Float1> Float3x1;
        Float3x1 a(5);
        std::cout << "type coersion from Tensor2_t<Float3,Float1> to Float3:\n";
        bar(a);
        a.as_factor1() = Float3(20);
        std::cout << "assignment via float coersion (a should equal [20  20  20]^T): " << FORMAT_VALUE(a) << "\n\n";

        typedef Tensor2_t<Float1,Float4> Float1x4;
        Float1x4 b(6);
        std::cout << "type coersion from Tensor2_t<Float1,Float4> to Float4:\n";
        bor(b);
        b.as_factor2() = Float4(42);
        std::cout << "assignment via float coersion (b should equal [42  42  42  42]): " << FORMAT_VALUE(b) << "\n\n";

        // uncommenting the following should produce a compile error (no type conversion to Float3)
//         Float3 x(b);
    }

    // testing various tensor access and operations
    {
        Float2 u(10,20);
        Float3 v(1,2,3);
        Float4 w(4,5,6,7);

        std::cout << FORMAT_VALUE(u) << '\n';
        std::cout << FORMAT_VALUE(v) << '\n';
        std::cout << FORMAT_VALUE(w) << '\n';

        SimpleFloat3x4 X(v,w);

        std::cout << "X := v \\otimes w = " << X << ", and " << v%w << '\n';


        for (Uint32 r = 0; r < 3; ++r)
        {
            for (Uint32 c = 0; c < 4; ++c)
            {
                std::cout << "\t" << X[Float3x4::DeprecatedIndex(r,c)];
            }
            std::cout << '\n';
        }

        std::cout << '\n';


        for (Uint32 i = 0; i < Float3x4::DIM; ++i)
        {
            Float3x4::DeprecatedIndex index(i);
            std::cout << i << " -> " << index.subindex1().value() << ", " << index.subindex2().value()
                      << " -> " << Float3x4::DeprecatedIndex(index.subindex1().value(), index.subindex2().value()).value() << '\n';
        }

        std::cout << '\n';

        for (Uint32 i = 0; i < Float2x3x4::DIM; ++i)
        {
            Float2x3x4::Index index(i);
            std::cout << i << " -> " << index.subindex1().value() << ", " << index.subindex2().value() << ", " << index.subindex3().value()
                      << " -> " << Float2x3x4::Index(index.subindex1().value(), index.subindex2().value(), index.subindex3().value()).value() << '\n';
        }

        std::cout << '\n';

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
                std::cout << "\t" << Y[Float3x4::DeprecatedIndex(r,c)];
            }
            std::cout << '\n';
        }

        std::cout << '\n';

        std::cout << "X * Y = " << DotProduct_t<SimpleFloat3x4>::eval(X,Y) << " (";

        // done the hard way -- compute the inner product of X and Y
        float accumulator = 0;
        for (Uint32 r = 0; r < 3; ++r)
            for (Uint32 c = 0; c < 4; ++c)
                accumulator += X[Float3x4::DeprecatedIndex(r,c)] * Y[Float3x4::DeprecatedIndex(r,c)];
        std::cout << "actual answer = " << accumulator << ")\n\n";

        Float3x3 W(WITHOUT_INITIALIZATION);
        for (Uint32 i = 0; i < 3; ++i)
            for (Uint32 j = 0; j < 3; ++j)
                for (Uint32 k = 0; k < 4; ++k)
                    W[Float3x3::DeprecatedIndex(i,j)] = X[Float3x4::DeprecatedIndex(i,k)] * Y[Float3x4::DeprecatedIndex(j,k)];
        for (Uint32 i = 0; i < 3; ++i)
        {
            for (Uint32 j = 0; j < 3; ++j)
                std::cout << '\t' << W[Float3x3::DeprecatedIndex(i,j)];
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
            typedef ExpressionTemplate_IndexedTensor_t<Float3,TypeList_t<I>,EmptyTypeList> EI;
            typedef ExpressionTemplate_IndexedTensor_t<Float3,TypeList_t<J>,EmptyTypeList> EJ;

//             std::cout << i << '\n';
//             std::cout << Float3::Index(0) << '\n';
            v(i);
            std::cout << FORMAT_VALUE(v(i)[EI::Index(0)]) << '\n';

            std::cout << FORMAT_VALUE(v.expr<'j'>()[EJ::Index(1)]) << '\n';

            for (Float3::Index k; k.is_not_at_end(); ++k)
                std::cout << u[k] + v[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "addition:\n";
            typedef ExpressionTemplate_IndexedTensor_t<Float3,TypeList_t<I>,EmptyTypeList> EE;
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

            std::cout << "operator +\n";
            EA e2(u(i) + v(i));
            for (EA::Index k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "inner product:\n";
            typedef ExpressionTemplate_IndexedTensor_t<Float3,TypeList_t<I>,EmptyTypeList> EE;
            typedef ExpressionTemplate_Multiplication_t<EE,EE> EM;
            EM e(u(i), v(i));
            Float3::Index k;
            std::cout << FORMAT_VALUE(k.COMPONENT_COUNT) << '\n';
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
            typedef ExpressionTemplate_IndexedTensor_t<Float3,TypeList_t<I>,EmptyTypeList> EI;
            typedef ExpressionTemplate_IndexedTensor_t<Float3,TypeList_t<J>,EmptyTypeList> EJ;
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
            std::cout << "operator *\n";
            EM e2(u(i) * v(j));
            for (EM::Index k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "contraction with simple tensor:\n";
            typedef ExpressionTemplate_IndexedTensor_t<Float3,TypeList_t<I>,EmptyTypeList> EI;
            typedef ExpressionTemplate_IndexedTensor_t<Float3,TypeList_t<J>,EmptyTypeList> EJ;
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
            std::cout << "operator *\n";
            EMJ e2(u(i) * v(j) * w(j));
            for (EMJ::Index k; k.is_not_at_end(); ++k)
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
            typedef Float3x4::Index_t<'i'> I;
            I i;
            typedef ExpressionTemplate_IndexedTensor_t<Float3x4,TypeList_t<I>,EmptyTypeList> EE;
            typedef ExpressionTemplate_Addition_t<EE,EE> EA;
            Float3x4 u(WITHOUT_INITIALIZATION);
            Float3x4 v(WITHOUT_INITIALIZATION);
            for (Uint32 k = 0; k < Float3x4::DIM; ++k)
            {
                u[k] = k;
                v[k] = 13+k;
            }
            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << FORMAT_VALUE(v) << '\n';
            std::cout << FORMAT_VALUE(u(i)[0]) << '\n';
            std::cout << FORMAT_VALUE(u(i)[11]) << '\n';
            EA e(u(i), v(i));
            std::cout << "expression template value:\n";
            for (EA::Index k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';
            std::cout << "hand-calculated value:\n";
            for (Uint32 k = 0; k < Float3x4::DIM; ++k)
                std::cout << u[k] + v[k] << ", ";
            std::cout << '\n';
            std::cout << "operator +\n";
            EA e2(u(i) + v(i));
            for (EA::Index k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "expression template for 2-tensor:\n";
            typedef Float3::Index_t<'i'> I;
            typedef Float4::Index_t<'j'> J;
            I i;
            J j;
            typedef ExpressionTemplate_IndexedTensor_t<Float3x4,TypeTuple_t<I,J>::T,EmptyTypeList> EIJ;
            typedef ExpressionTemplate_Addition_t<EIJ,EIJ> EA;
            Float3x4 u(WITHOUT_INITIALIZATION);
            Float3x4 v(WITHOUT_INITIALIZATION);
            for (Uint32 k = 0; k < Float3x4::DIM; ++k)
            {
                u[k] = k;
                v[k] = 13+k;
            }
            EA e(u(i,j), v(i,j));
            for (EA::Index k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';

            std::cout << "operator + with same index order\n";
            EA e2(u(i,j) + v(i,j));
            for (EA::Index k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "symmetrizing a 2-tensor:\n";
            typedef Float3::Index_t<'i'> I;
            typedef Float3::Index_t<'j'> J;
            I i;
            J j;
            typedef ExpressionTemplate_IndexedTensor_t<Float3x3,TypeTuple_t<I,J>::T,EmptyTypeList> EIJ;
            typedef ExpressionTemplate_IndexedTensor_t<Float3x3,TypeTuple_t<J,I>::T,EmptyTypeList> EJI;
            typedef ExpressionTemplate_Addition_t<EIJ,EIJ> EA;
            typedef ExpressionTemplate_Addition_t<EIJ,EJI> EB;
            Float3x3 u(WITHOUT_INITIALIZATION);
            Float3x3 v(WITHOUT_INITIALIZATION);
            // dummy values that aren't symmetric
            for (Uint32 k = 0; k < Float3x3::DIM; ++k)
            {
                u[k] = k*k;
                v[k] = k*k + 2*k;
            }

            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << FORMAT_VALUE(v) << '\n';

            EB e(u(i,j), u(j,i));
            for (EB::Index k; k.is_not_at_end(); ++k)
                std::cout << e[k] << ", ";
            std::cout << '\n';

            // uncommenting this should cause an error regarding prohibiting repeated indices in sums
//             typedef ExpressionTemplate_IndexedTensor_t<Float3x3,TypeTuple_t<I,I>::T,EmptyTypeList> EII;
//             typedef ExpressionTemplate_Addition_t<EII,EII> EC;
//             EC e_bad(u(i,i), u(i,i));

            std::cout << "operator + with same index order\n";
            EA e2(u(i,j) + v(i,j));
            for (EA::Index k; k.is_not_at_end(); ++k)
                std::cout << e2[k] << ", ";
            std::cout << '\n';

            std::cout << "operator + with opposite index order\n";
            EB e3(u(i,j) + v(j,i));
            for (EB::Index k; k.is_not_at_end(); ++k)
                std::cout << e3[k] << ", ";

            u.expr<'i','j'>();
            u.expr<'i','j'>() + v.expr<'i','j'>();
            u.expr<1,2>() + v.expr<1,2>();
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "2-tensor contraction (matrix multiplication)\n";
            Float3x4 u(WITHOUT_INITIALIZATION);
            Float4x5 v(WITHOUT_INITIALIZATION);
            Float5x2 w(WITHOUT_INITIALIZATION);
            for (Uint32 k = 0; k < Float3x4::DIM; ++k)
                u[k] = k*k;
            for (Uint32 k = 0; k < Float4x5::DIM; ++k)
                v[k] = 2*k + 3;
            for (Uint32 k = 0; k < Float5x2::DIM; ++k)
                w[k] = k + 1;
            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << FORMAT_VALUE(v) << '\n';
            std::cout << FORMAT_VALUE(w) << '\n';
            typedef Float3::Index_t<'i'> I;
            typedef Float4::Index_t<'j'> J;
            typedef Float5::Index_t<'k'> K;
            typedef Float2::Index_t<'l'> L;
            I i;
            J j;
            K k;
            L l;
            typedef ExpressionTemplate_IndexedTensor_t<Float3x4,TypeTuple_t<I,J>::T,EmptyTypeList> EIJ;
            typedef ExpressionTemplate_IndexedTensor_t<Float4x5,TypeTuple_t<J,K>::T,EmptyTypeList> EJK;
            typedef ExpressionTemplate_IndexedTensor_t<Float5x2,TypeTuple_t<K,L>::T,EmptyTypeList> EKL;
            typedef ExpressionTemplate_Multiplication_t<EIJ,EJK> EM;
            typedef ExpressionTemplate_Multiplication_t<EM,EKL> EMM;
            std::cout << "expression template contraction u(i,j)*v(j,k):\n";
            EM e(u(i,j), v(j,k));
            for (EM::Index c; c.is_not_at_end(); ++c)
                std::cout << e[c] << ", ";
            std::cout << '\n';

            std::cout << "hand-computed answer:\n";
            for (Uint32 a = 0; a < 3; ++a)
            {
                for (Uint32 b = 0; b < 5; ++b)
                {
                    float accum = 0;
                    for (Uint32 c = 0; c < 4; ++c)
                        accum += u[Float3x4::DeprecatedIndex(a,c)] * v[Float4x5::DeprecatedIndex(c,b)];
                    std::cout << accum << ", ";
                }
            }
            std::cout << '\n';

            std::cout << "expression template contraction u(i,j)*v(j,k)*w(k,l):\n";
            EMM e2(e, w(k,l));
            for (EMM::Index c; c.is_not_at_end(); ++c)
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
                            accum += u[Float3x4::DeprecatedIndex(a,c)] * v[Float4x5::DeprecatedIndex(c,d)] * w[Float5x2::DeprecatedIndex(d,b)];
                    std::cout << accum << ", ";
                }
            }
            std::cout << '\n';
            std::cout << "operator *:\n";
            EMM e3(u(i,j)*v(j,k)*w(k,l));
            for (EMM::Index c; c.is_not_at_end(); ++c)
                std::cout << e3[c] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        {
            std::cout << "assignment via expression templates:\n";
            Float3 u(4);
            Float3 v(5,6,7);
            Float3::Index_t<'i'> i;
            u(i) = v(i);
            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << '\n';

            u(i) = v(i) + v(i);
            std::cout << FORMAT_VALUE(u) << '\n';
            std::cout << '\n';

            Float3x3 m(WITHOUT_INITIALIZATION);
            for (Uint32 k = 0; k < Float3x3::DIM; ++k)
                m[k] = k;
            std::cout << FORMAT_VALUE(m) << '\n';
            std::cout << '\n';

            Float3x3 n(WITHOUT_INITIALIZATION);
            for (Uint32 k = 0; k < Float3x3::DIM; ++k)
                n[k] = 3*k + 2;
            std::cout << FORMAT_VALUE(n) << '\n';
            std::cout << '\n';

            Float3::Index_t<'j'> j;

            std::cout << "direct assignment m(i,j) = n(i,j):\n";
            m(i,j) = n(i,j);
            std::cout << FORMAT_VALUE(m) << '\n';
            std::cout << '\n';

            std::cout << "transposed assignment m(i,j) = n(j,i):\n";
            m(i,j) = n(j,i);
            std::cout << FORMAT_VALUE(m) << '\n';
            std::cout << '\n';

            std::cout << "symmetrized assignment m(i,j) = n(i,j) + n(j,i):\n";
            m(i,j) = n(i,j) + n(j,i);
            std::cout << FORMAT_VALUE(m) << '\n';
            std::cout << '\n';
        }

        {
            Float3x3 u(3);
            typedef Float3::Index_t<'i'> I;
            typedef Float3::Index_t<'j'> J;
            I i;
            J j;
            std::cout << FORMAT_VALUE(u) << '\n';
            typedef ExpressionTemplate_IndexedTensor_t<Float3x3,EmptyTypeList,TypeTuple_t<I>::T> ET;
            ET::Index k;
            std::cout << "trace(u) = " << u(i,i)[k] << '\n';
            std::cout << "trace(u) = " << u.expr<'i','i'>()[k] << '\n';
            std::cout << '\n';

            Float3 v(1,2,3);
            u(i,j) = v(i)*v(j);
            std::cout << FORMAT_VALUE(v) << '\n';
            std::cout << "v \\otimes v = ";
            for (Uint32 k = 0; k < Float3x3::DIM; ++k)
                std::cout << u[k] << ", ";
            std::cout << '\n';
            std::cout << '\n';
        }

        // checking for aliasing
        {
            Float3 u(1,2,3);
            Float3 v(4,5,6);
            Float3x3 a(10);
            typedef Float3::Index_t<'i'> I;
            typedef Float3::Index_t<'j'> J;
            I i;
            J j;
            std::cout << FORMAT_VALUE(u(i).uses_tensor(u)) << '\n';
            std::cout << FORMAT_VALUE(u(i).uses_tensor(v)) << '\n';
            u(i) = u(i); // this should be a no-op
            try {
                // this should throw an exception
                u(i) = a(i,j)*u(j);
                assert(false && "an exception didn't occur when it was supposed to");
            } catch (std::invalid_argument const &e) {
                std::cout << "correctly caught an exception indicating an aliased template expression\n";
            }
            std::cout << '\n';
        }
    }

    return 0;
}

