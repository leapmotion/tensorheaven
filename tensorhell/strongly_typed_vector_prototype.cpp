#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "tenh/meta/core.hpp"

/*
design goals:
- on the foundation of strongly typed vector spaces, build arbitrary tensors
  which obey a flexible and full type hierarchy, and allow arbitrary tensor
  contraction which takes advantage of any possible symmetries in the tensors.
- the operations will enforce strong typing via natural pairing.
- a tensor space is a vector space, and should be usable as such.
- symmetric tensors should use the minimal amount of memory (i.e. no redundant
  storage) and encode the symmetries in the calculations (i.e. no redundant
  calculation).
- all tensors should have full indexing capabilities
- symmetric tensors' implementations of full indexing should essentially just
  provide a map from the "full" index space to the "symmetric" index space
- maybe the different tensor contractions can be provided by tensor type casts
  that split the tensor product up in different ways (e.g. A \otimes (B \otimes C)
  versus (A \otimes B) \otimes C).  this would require that the components are laid
  out in some compatible way in memory.  how would tensor symmetries be handled
  in this scheme?
*/

//using namespace Tenh;

struct Null { }; // sentinel type

struct WithoutInitialization { };

static WithoutInitialization const WITHOUT_INITIALIZATION = WithoutInitialization();

template <typename PrimalTypeID_> struct Dual_TypeID_t { };

template <typename TypeID_> struct ReflexiveDualOf_f { typedef Dual_TypeID_t<TypeID_> TypeID; };
template <typename TypeID_> struct ReflexiveDualOf_f<Dual_TypeID_t<TypeID_> > { typedef TypeID_ TypeID; }; // specialization

// TODO: make index type -- vectors are indexed with a single int, while tensors are generally indexed by tuples
// the index type should specify the dimension, so that a vector's DIM template argument is replaced by its index type
template <bool IS_UP_, Uint32 DIM_, typename TypeID_>
struct Index
{
    static bool const IS_UP = IS_UP_;
    static Uint32 const DIM = DIM_;
    typedef TypeID_ TypeID;
};
/*
template <>
struct VectorIndex
{
    // maps [0..DIM) to [0..DIM)
};

template <>
struct TensorIndex
{
    // maps [0..F1DIM)x[0..F2DIM) to [0..F1DIM*F2DIM)
};

template <>
struct SymmetricTensorIndex
{
    // maps [0..F1DIM)x[0..F1DIM) to [0..(F1DIM+1 choose 2))
};

template <>
struct AntisymmetricTensorIndex
{
    // maps [0..F1DIM)x[0..F1DIM) to [0..(F1DIM-1 choose 2))
};
*/
/*
design for tensor contraction:

if U,V,W are vector types,
typedef Tensor_t<U,V> A;
typedef SymmetricTensor_t<W,W> B;
typedef Tensor_t<A,B> C;

C c;
DualOf_f<C>::T c_
DualOf_f<B>::T b_;
DualOf_f<W>::T w_;
DualOf_f<U>::T u_;

want the following in-line (adjacent) contractions to work:

c*c_ -> Scalar
c_*c -> Scalar
c*b_ -> A
u_*c -> Tensor_t<V,B>
c*w_ -> Tensor_t<A,W>

want the following non-in-line contractions to work as well (though the notation will necessarily be more verbose)

c(i,j,k,l)*b_(k) -> Tensor_t<A,W>
c(i,j,k,l)*b_(l) -> Tensor_t<A,W> (same as c*b_)
c(i,j,k,l)*c_(i,j,k,l) -> Scalar (same as c*c_)
c(i,j,k,l)*c_(i,j,l,k) -> Scalar
c(j,i,k,l) -> Tensor<Tensor<V,U>,B>

there is a type graph associated with any tensor, and it is more involved if it involves tensors having symmetries.

SymmetricTensor<W,W>
        |
        V
    Tensor<W,W>
        |
        V
    Vector<W::Scalar,W::DIM*W::DIM>

Tensor<SymmetricTensor<U,U>,V>
        |
        V
Tensor<Tensor<U,U>,V> ~= Tensor<U,Tensor<U,V>> ~= Tensor<U,U,V>

the most basic type Tensor<U,U,V> is necessary to use fully general indexed contraction

is run-type polymorphism necessary to do proper indexing?  or can this always be done at compile time using templates?

maybe index types can be put together in a useful way.

c \in Tensor<Tensor<U,V>,SymmetricTensor<W,W>> \subseteq Tensor<U,V,W,W>

Tensor<U,V,W,W> ~= Tensor<U,Tensor<V,W>,W>
(i,j,k,l) <--> (i,(j,k),l)

with each isomorphism comes an index map.

with symmetries, the corresponding index isn't necessarily a tuple (as it was above with (j,k))
but is an opaque object.

Tensor<U,V,SymmetricTensor<W,W>> ~= Tensor<U,V,W,W>
(i,j,S(k,l)) <--> (i,j,k,l)

each tensor space with symmetry is a linear subspace of the full tensor space (and possibly other intermediate spaces)
and each inclusion should come with an index map, which maps the larger space indices onto the symmetric space indices.
also, a contraction map is necessary (e.g. for symmetric tensors ((a,b),(b,c)) <--> (a,b,c), the contraction is
(a,b,c)*(p,q,r) := (a*p + 2*b*q + c*r), not the standard dot product.

any time a contraction is done, the most-specific indexing scheme should be used to do the contraction.  this may need
to be user-controlled.  it's probably better this way, so that implicit details are not hidden.

in this sense, maybe the abstract index notation is the best.  each vector/tensor type should have its own
index type declared within, providing the indexing scheme and natural pairing.

the contraction then consists of literally a nested for-loop over all the free and contracted indices, where sums
are taken over the contracted indices.

c(I,J,S)*b_(S) and c(I,J,K,L)*b_(K,L) would be valid ways to contract, but the first one would take advantage of
the symmetry, while the second wouldn't.  note that the indices are typed, so S is a distinct type from K, L, or (K,L)

the code for the first one would be something like

for i in I: // where I is considered to be a set of indices
    for j in J:
        retval(i,j,s) = 0
        for s in S:
            retval(i,j,s) += c(i,j,s)*g(s)*b_(s) // where g(s) is the weight of element s
                                                 // QUESTION: it's concievable that this would need to
                                                 // be a full matrix, but maybe for a natural pairing
                                                 // for tensors with non-fucked-up symmetries, it wouldn't be.

there should be a type hierarchy associated with S and (K,L), so that Tensor<W,W>::Index(K,L) could be plugged in instead of S

if T := Tensor<W,W>::Index(K,L) is passed in, then the loop would be

for i in I: // where I is considered to be a set of indices
    for j in J:
        retval(i,j,s) = 0
        for t in T: // t takes all (K,L) values, and so must repeat sometimes, but the value t itself is a symmetric-type index
            retval(i,j,t) += c(i,j,t)*b_(t) // no altered natural pairing necessary here

maybe for the index correspondences, use templates to provide reindexing as:

IndexMap<From,To>,

where From is the index type of the passed-in index, and To is the type of the tensor argument itself.
there would be an associated natural pairing weight as well.

IDEA: an expression like c(i,j,Tensor<W,W>::Index(k,l))*b_(Tensor<W,W>::Dual::Index(k,l)) makes
an AST object which, once coerced into a tensor type, does the evaluation.  in theory this could
use fewer and smaller intermediate values, e.g. a(i)*m(i,j)*b(j) could be evaluated directly to
a scalar, instead of a(i)*m(i,j) to a covector and then to a scalar.

so the procedure should be something like identify the free indices, identify the summation indices,
and then for each combination of free indices, assign to that particular entry in the return value
the result of the summation in the summation indices.

one step should be collecting all free indices into a single object (?) so a single loop can
be used to iterate through it.  some sort of iterator object would be ideal to use here.
however, when trying to generally-index tensors with symmetries, where a reindexing map is
used, this may make implementing the iterator a lot harder.

PROBLEM: it's not just a reindexing scheme for tensor subspaces -- for example, the diagonal matrices
would store only the diagonal elements, and return zero for the off-diagonals.  however, this means that
there can't just be a reindexing map that does the job, as above.  however, the element access methods
could be overloaded to accept different types of indices.  the general-index one would just return 0
for all off-diagonals.

TODO: have tensor product type tensor, which represents A \otimes B by storing A and B unchanged
and making the contraction rules simpler.  this also uses less memory and makes contraction much faster.
maybe use % as the \otimes operator?  it's precedence is the same as *.  then natural pairing
could be done using * with in-line tensor contraction, and then fancier indexing can be done using
an explicit trace on particular indices.

TODO: also make permutations of tensors, which doesn't change any memory, but only changes the indexing
scheme?  this would potentially be a huge savings.

TODO: make down-cast tensors (?), where e.g. a symmetric tensor can be used (and indexed) as a
non-symmetric tensor.  this would probably save design complexity by putting the reindexing scheme
in the down-cast tensor class.  and if the parameter type of the down-cast is left as a template
parameter, template specialization can be used to define these down-casts later -- say if you
implement your own custom symmetry tensor and want to down-cast it.

TODO: i think using tensor product types as the assigned-to type can get around the loop fusing
problem mentioned in http://www.gps.caltech.edu/~walter/FTensor/FTensor.pdf

TODO: use expression templates, and make = an expression template so that no intermediates need
to be stored when assigning.

TODO: there is a type inclusion for subspaces -- implement this using type conversions

QUESTION: indexing seems to be for down-casted situations -- accessing a SymmetricTensor through
a NonsymmetricTensor indexing, so that it may be contracted with a NonsymmetricTensor.

NOTE: it seems like situations where atomic tensors are being paired naturally could be
handled efficiently.  e.g. (A \otimes B) \otimes (C \otimes D) could be contracted in the
"simple tensor" space just by (A \cdot C)(B \cdot D), but if the simple tensor type is
inside another tensor product, this wouldn't work.

TODO: make "identity tensor" and "zero tensor" types (which represent sets having exactly one element),
which just returns a hardcoded value, so that nothing needs to be stored to use an identity or zero tensor
for example.

*/

// TODO: DerivedType_ to specify the type of ZERO and the return value for operators
template <typename Scalar_, Uint32 DIM_, typename TypeID_>
struct Vector_t
{
    enum { _ = Assert<(DIM_>0)>::V }; // disallow 0-dimensional spaces (would 0-dim spaces be useful?)
    typedef Scalar_ Scalar;
    static Uint32 const DIM = DIM_;
    typedef TypeID_ TypeID;
    typedef Vector_t<Scalar,DIM,typename ReflexiveDualOf_f<TypeID_>::TypeID> Dual;

    static Vector_t const ZERO;

    Vector_t (WithoutInitialization const &) { }
    Vector_t (Scalar filler) { for (Uint32 i = 0; i < DIM; ++i) m[i] = filler; } // initialize to zero
    Vector_t (Scalar x0, Scalar x1) { Assert<(DIM==2)>(); m[0] = x0; m[1] = x1; }
    Vector_t (Scalar x0, Scalar x1, Scalar x2) { Assert<(DIM==3)>(); m[0] = x0; m[1] = x1; m[2] = x2; }
    Vector_t (Vector_t const &v) { for (Uint32 i = 0; i < DIM; ++i) m[i] = v.m[i]; }

    Scalar const &operator [] (Uint32 index) const
    {
        if (index >= DIM)
            throw std::invalid_argument("index out of range");
        return m[index];
    }
    Scalar &operator [] (Uint32 index)
    {
        if (index >= DIM)
            throw std::invalid_argument("index out of range");
        return m[index];
    }

private:

    Scalar_ m[DIM];
}; // end of struct Vector_t<>

template <typename Scalar_, Uint32 DIM_, typename TypeID_>
Vector_t<Scalar_,DIM_,TypeID_> const Vector_t<Scalar_,DIM_,TypeID_>::ZERO(0);

template <typename Scalar_, Uint32 DIM_, typename TypeID_>
Scalar_ operator * (Vector_t<Scalar_,DIM_,TypeID_> const &v, typename DualOf_f<Vector_t<Scalar_,DIM_,TypeID_> >::T const &d)
{
    Scalar_ retval(v[0]*d[0]);
    for (Uint32 i = 1; i < DIM_; ++i)
        retval += v[i]*d[i];
    return retval;
}

template <typename Scalar_, Uint32 DIM_, typename TypeID_>
std::ostream &operator << (std::ostream &out, Vector_t<Scalar_,DIM_,TypeID_> const &v)
{
    out << "\n[" << v[0];
    for (Uint32 i = 1; i < DIM_; ++i)
        out << ",\n " << v[i];
    return out << ']';
}

template <typename Scalar_, Uint32 DIM_, typename PrimalTypeID_>
std::ostream &operator << (std::ostream &out, Vector_t<Scalar_,DIM_,Dual_TypeID_t<PrimalTypeID_> > const &d)
{
    out << "[" << d[0];
    for (Uint32 i = 1; i < DIM_; ++i)
        out << ", " << d[i];
    return out << ']';
}

template <typename VS1_, typename VS2_> struct Tensor_TypeID_t { };

// general tensor not necessarily having symmetries
template <typename Factor1_, typename Factor2_>
struct Tensor_t : public Vector_t<typename Factor1_::Scalar,
                                  Factor1_::DIM * Factor2_::DIM,
                                  Tensor_TypeID_t<Factor1_,Factor2_> >
{
    enum { _ = TypesAreEqual<typename Factor1_::Scalar,typename Factor2_::Scalar>::V }; // ensure the vectors have the same scalar
    typedef Vector_t<typename Factor1_::Scalar,
                     Factor1_::DIM * Factor2_::DIM,
                     Tensor_TypeID_t<Factor1_,Factor2_> > ParentVector;

    typedef typename ParentVector::Scalar Scalar;
    static Uint32 const DIM = ParentVector::DIM;
    typedef typename ParentVector::TypeID TypeID;

    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;

    Tensor_t (WithoutInitialization const &w) : ParentVector(w) { }
}; // end of struct Tensor_t<>

template <typename Factor1_, typename Factor2_>
Factor1_ operator * (Tensor_t<Factor1_,Factor2_> const &t, typename DualOf_f<Factor2_>::T const &d)
{
    Factor1_ retval(WITHOUT_INITIALIZATION);
    for (Uint32 j = 0; j < Factor1_::DIM; ++j)
        for (Uint32 i = 0; i < Factor2_::DIM; ++i)
            retval[j] += t[i*Factor2_::DIM+j]*d[j];
    return retval;
}

// outer product (unfortunately the operator precedence of & is all wrong)

template <typename Factor1_, typename Factor2_>
Tensor_t<Factor1_,Factor2_> operator & (Factor1_ const &f1, Factor2_ const &f2)
{
    Tensor_t<Factor1_,Factor2_> retval(WITHOUT_INITIALIZATION);
    for (Uint32 j = 0; j < Factor1_::DIM; ++j)
        for (Uint32 i = 0; i < Factor2_::DIM; ++i)
            retval[i*Factor2_::DIM+j] = f1[i]*f2[j];
    return retval;
}

template <typename Factor1_, typename Factor2_>
std::ostream &operator << (std::ostream &out, Tensor_t<Factor1_,Factor2_> const &t)
{
    if (Factor1_::DIM > 1)
        out << '\n';
    out << '[' << *reinterpret_cast<Factor2_ const *>(&t[0*Factor1_::DIM]);
    for (Uint32 i = 1; i < Factor1_::DIM; ++i)
        out << "\n " << *reinterpret_cast<Factor2_ const *>(&t[i*Factor1_::DIM]);
    return out << ']';
}



// TODO: symmetric product of single vector space
// TODO: antisymmetric product of single vector space

struct R3 { };

int main (int argc, char **argv)
{
    typedef Vector_t<float,3,R3> Float3;
    typedef Tensor_t<Float3,DualOf_f<Float3>::T> Float3x3;

    Float3 v(1,2,3);
    std::cout << "v = " << v << '\n';
    std::cout << v[0] << ", " << v[1] << ", " << v[2] << '\n';

    DualOf_f<Float3>::T d(4,5,6);
    std::cout << "d = " << d << '\n';

    std::cout << "v*d = " << v*d << '\n';

    std::cout << "Float3::ZERO = " << Float3::ZERO << '\n';
    std::cout << "DualOf_f<Float3>::T::ZERO = " << DualOf_f<Float3>::T::ZERO << '\n';

    Float3x3 const &z = *reinterpret_cast<Float3x3 const *>(&Float3x3::ZERO);
    std::cout << "Float3x3::ZERO = " << z << '\n';

    std::cout << "z*v = " << z*v << '\n';

    std::cout << "v \\otimes d = " << (v&d) << '\n';

    return 0;
}
