#ifndef INDEX_MAP_HPP_
#define INDEX_MAP_HPP_

#include "tuple.hpp"
#include "typelist_utility.hpp"

/*
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




/*
Want to make the map which maps a tuple of unique indices to a tuple of specified indices
e.g. if the input is (i,j,j,i,k) (which is a TypeList_t type), then the output is the map
(i,j,k) -> (i,j,j,i,k) (which is a map from a List_t<i,j,k> to a List_t<i,j,j,i,k>).
*/

template <typename TypeList, typename UniqueTypes_ = typename UniqueTypesIn_t<TypeList>::T>
struct IndexMap_t
{
    typedef UniqueTypes_ UniqueTypes;
    static Uint32 const HEAD_INDEX = UniqueTypes::template IndexOf_t<typename TypeList::HeadType>::V;

    static List_t<TypeList> eval (List_t<UniqueTypes> const &index)
    {
        return List_t<TypeList>(index.template value<HEAD_INDEX>(),
                                IndexMap_t<typename TypeList::BodyTypeList, UniqueTypes>::eval(index));
    }
};

template <typename HeadType, typename UniqueTypes_>
struct IndexMap_t<TypeList_t<HeadType>, UniqueTypes_>
{
    typedef UniqueTypes_ UniqueTypes;
    static Uint32 const HEAD_INDEX = 0;

    static List_t<TypeList_t<HeadType> > eval (List_t<UniqueTypes> const &index)
    {
        return List_t<TypeList_t<HeadType> >(index.template value<HEAD_INDEX>());
    }
};

#endif // INDEX_MAP_HPP_
