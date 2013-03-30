#ifndef TENSOR2ANTISYMMETRIC_HPP_
#define TENSOR2ANTISYMMETRIC_HPP_

#include <cmath> // TEMP until rowcol/contiguous index map is implemented as a lookup table
#include <ostream>

#include "core.hpp"
#include "expression_templates.hpp"
#include "naturalpairing.hpp"
#include "typetuple.hpp"
#include "vector.hpp"

// antisymmetric 2-tensor (its transpose is equal to its negative)
template <typename Factor_, typename Derived_ = NullType>
struct Tensor2Antisymmetric_t : public Vector_t<typename Factor_::Scalar,
                                                ((Factor_::DIM-1)*Factor_::DIM)/2,
                                                typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                       Tensor2Antisymmetric_t<Factor_,Derived_>,
                                                                       Derived_>::T>
{
    typedef Vector_t<typename Factor_::Scalar,
                     ((Factor_::DIM-1)*Factor_::DIM)/2,
                     typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Tensor2Antisymmetric_t<Factor_,Derived_>,
                                            Derived_>::T> Parent;
    typedef typename Parent::Scalar Scalar;
    using Parent::DIM;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Index Index;
    typedef Factor_ Factor;
    typedef Factor Factor1;
    typedef Factor Factor2;
    typedef CompoundIndex_t<typename TypeTuple_t<typename Factor::Index,typename Factor::Index>::T> CompoundIndex;
    static Uint32 const STRICTLY_LOWER_TRIANGULAR_COMPONENT_COUNT = ((Factor::DIM-1)*Factor::DIM)/2;

    Tensor2Antisymmetric_t (WithoutInitialization const &w) : Parent(w) { }
    Tensor2Antisymmetric_t (Scalar fill_with) : Parent(fill_with) { }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static CompoundIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        typedef typename BundleIndexTypeList::template El_t<0>::T Index1;
        typedef typename BundleIndexTypeList::template El_t<1>::T Index2;
        // this is just to check that there is a valid conversion to the requested CompoundIndex type.
        // it doesn't actually produce any side-effects, and should be optimized out.
        {
            Lvd::Meta::Assert<BundleIndexTypeList::LENGTH == 2>();
            Index1 i1;
            Index2 i2;
            typename Factor1::Index f1(i1);
            typename Factor2::Index f2(i2);
            // check that the parameter BundleIndex type is compatible with Index
            Index i(b);
        }
            
        Uint32 row;
        Uint32 col;
        contiguous_index_to_rowcol_index(b.value(), row, col);
        return CompoundIndex_t<BundleIndexTypeList>(Index1(row), Index2(col));
    }

    // TODO: because Factor1 and Factor2 are identical, it doesn't make sense to
    // have a type coercion to either one unless they are 1-dimensional, in which case
    // it can be a type coercion to the Scalar type.  however, an antisymmetric
    // 2-tensor on a 1-dimensional space is always zero.

    // TODO: because the diagonal is always zero, there is an easy type coercion to Tensor2Diagonal_t

    // dumb, but the compiler wouldn't inherit implicitly, and won't parse a "using" statement
    Scalar const &operator [] (Index const &i) const { return Parent::operator[](i); }
    // dumb, but the compiler wouldn't inherit implicitly, and won't parse a "using" statement
    Scalar &operator [] (Index const &i) { return Parent::operator[](i); }

    // using two indices in a Tensor2Antisymmetric_t is breaking apart the Index type and using it
    // as a general tensor -- this is where the fancy indexing scheme happens.
    template <typename Index1, typename Index2>
    Scalar operator [] (CompoundIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &c) const
    {
        if (c.is_at_end())
            throw std::invalid_argument("index out of range");

        // NOTE: these constructions are unnecessary to the code, but ARE necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        typename Factor::Index(c.template el<0>());
        typename Factor::Index(c.template el<1>());

        Uint32 row = c.template el<0>().value();
        Uint32 col = c.template el<1>().value();

        // index the diagonal first, then the strictly lower triangular in row-major order
        if (row == col)
            return Scalar(0);

        // strict lower triangle elements are stored contiguously in row-major order:
        // [ . . . ... ]
        // [ 0 . . ... ]
        // [ 1 2 . ... ]
        // [ 3 4 5 ... ]
        // ...
        // the index of the first element in row R is the (R-1)th triangular number, so the mapping
        // (row,col) to contiguous index i is i := r*(r-1)/2 + c.
        bool swapped = row <= col;
        if (swapped)
            std::swap(row,col);
        Scalar component(this->component_access_without_range_check(rowcol_index_to_contiguous_index(row, col)));
        return swapped ? -component : component;
    }
    // for notational completeness (see the non-const component method).  this is effectively an alias for operator[].
    template <typename Index1, typename Index2>
    Scalar component (CompoundIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &c) const
    {
        return operator[](c);
    }
    // you may only write-access the strict upper triangle components -- where the 0th element of c
    // is less than the 1st element of c.  this is because the exact values of each component in
    // the 2-tensor don't necessarily correspond to memory locations (the diagonal components are 
    // all zero so aren't stored in memory, and the strict lower triangle components are the
    // negatives of the upper ones, so don't correspond to direct memory access).
    template <typename Index1, typename Index2>
    Scalar &component (CompoundIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &c)
    {
        if (c.is_at_end())
            throw std::invalid_argument("index out of range");
        typename Factor::Index(c.template el<0>());
        typename Factor::Index(c.template el<1>());
        Uint32 row = c.template el<0>().value();
        Uint32 col = c.template el<1>().value();
        // throw an error if the diagonal or strict upper triangle is accessed
        if (row <= col)
            return throw std::invalid_argument("can only write to the strict lower triangle components of a Tensor2Antisymmetric_t");
        return this->component_access_without_range_check(rowcol_index_to_contiguous_index(row, col));
    }

    // the argument is technically unnecessary, as its value is not used.  however,
    // this allows the template system to deduce the SYMBOL of the IndexType_t, so
    // it doesn't need to be specified explicitly.
    // in this, an outer product would be
    // IndexType_t<'i'> i;
    // IndexType_t<'j'> j;
    // u(i)*v(j)
    // this override of the Parent's operator() is necessary so that the expression template
    // knows that the operand is actually a Tensor2_t.
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> operator () (
        NamedIndex_t<Derived,SYMBOL> const &) const
    {
        return expr<SYMBOL>();
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> operator () (
        NamedIndex_t<Derived,SYMBOL> const &)
    {
        return expr<SYMBOL>();
    }
    // the corresponding outer product example here would be
    // u.expr<'i'>() * v.expr<'j'>()
    // this override of the Parent's operator() is necessary so that the expression template
    // knows that the operand is actually a Tensor2_t.
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> expr () const
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList>(this->as_derived());
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> expr ()
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList>(this->as_derived());
    }

    // a 2-tensor can be indexed by the pair of factor indices (Factor1::Index, Factor2::Index)
    // Dear Bjarne, please forgive me for this template metaprogramming atrocity.  Sincerely, Victor.
    template <char FACTOR1_SYMBOL, char FACTOR2_SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       typename TypeTuple_t<
                                           NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                           NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                           >::T,
                                       typename SummedIndexTypeList_t<
                                           typename TypeTuple_t<
                                               NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                               NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                               >::T
                                           >::T
                                       > operator () (
        NamedIndex_t<Factor,FACTOR1_SYMBOL> const &,
        NamedIndex_t<Factor,FACTOR2_SYMBOL> const &) const
    {
        return expr<FACTOR1_SYMBOL,FACTOR2_SYMBOL>();
    }
    template <char FACTOR1_SYMBOL, char FACTOR2_SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       typename TypeTuple_t<
                                           NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                           NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                           >::T,
                                       typename SummedIndexTypeList_t<
                                           typename TypeTuple_t<
                                               NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                               NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                               >::T
                                           >::T
                                       > operator () (
        NamedIndex_t<Factor,FACTOR1_SYMBOL> const &,
        NamedIndex_t<Factor,FACTOR2_SYMBOL> const &)
    {
        return expr<FACTOR1_SYMBOL,FACTOR2_SYMBOL>();
    }
    // the 2-index analog of expr<SYMBOL>()
    template <char FACTOR1_SYMBOL, char FACTOR2_SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       typename TypeTuple_t<
                                           NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                           NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                           >::T,
                                       typename SummedIndexTypeList_t<
                                           typename TypeTuple_t<
                                               NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                               NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                               >::T
                                           >::T
                                       > expr () const
    {
        Lvd::Meta::Assert<(FACTOR1_SYMBOL != '\0')>();
        Lvd::Meta::Assert<(FACTOR2_SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,
                                                  typename TypeTuple_t<
                                                      NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                                      NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                                      >::T,
                                                  typename SummedIndexTypeList_t<
                                                      typename TypeTuple_t<
                                                          NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                                          NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                                          >::T
                                                      >::T
                                                  >(this->as_derived());
    }
    template <char FACTOR1_SYMBOL, char FACTOR2_SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       typename TypeTuple_t<
                                           NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                           NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                           >::T,
                                       typename SummedIndexTypeList_t<
                                           typename TypeTuple_t<
                                               NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                               NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                               >::T
                                           >::T
                                       > expr ()
    {
        Lvd::Meta::Assert<(FACTOR1_SYMBOL != '\0')>();
        Lvd::Meta::Assert<(FACTOR2_SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,
                                                  typename TypeTuple_t<
                                                      NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                                      NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                                      >::T,
                                                  typename SummedIndexTypeList_t<
                                                      typename TypeTuple_t<
                                                          NamedIndex_t<Factor,FACTOR1_SYMBOL>,
                                                          NamedIndex_t<Factor,FACTOR2_SYMBOL>
                                                          >::T
                                                      >::T
                                                  >(this->as_derived());
    }

    static std::string type_as_string ()
    {
        // TODO: return Derived's type_as_string value?
//         if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
//             return "Tensor2Antisymmetric_t<" + TypeStringOf_t<Factor>::eval() + '>';
//         else
//             return Derived::type_as_string();
        // for now, just return this type string
        if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
            return "Tensor2Antisymmetric_t<" + TypeStringOf_t<Factor>::eval() + '>';
        else
            return "Tensor2Antisymmetric_t<" + TypeStringOf_t<Factor>::eval() + ',' + TypeStringOf_t<Derived>::eval() + '>';
    }

private:

    // functions between the indexing schemes -- compound index is (row,col) with row > col and vector index is contiguous.
    static Uint32 rowcol_index_to_contiguous_index (Uint32 row, Uint32 col) 
    {
        if (row <= col)
            throw std::invalid_argument("row must be greater than col");
        return row*(row-1)/2 + col;
    }
    static void contiguous_index_to_rowcol_index (Uint32 i, Uint32 &row, Uint32 &col)
    {
        // given i, row = floor((1 + sqrt(1+8*i))/2) and col = i - r*(r-1)/2.
        // TODO: implement as lookup table
        row = Uint32(std::floor(0.5f + std::sqrt(0.25f + 2.0f*i)));
        col = i - row*(row-1)/2;
    }
};

template <typename Factor>
std::ostream &operator << (std::ostream &out, Tensor2Antisymmetric_t<Factor> const &t)
{
    typedef Tensor2Antisymmetric_t<Factor> Tensor2Antisymmetric;

    if (Tensor2Antisymmetric::DIM == 0)
        return out << "[]";

    typename Tensor2Antisymmetric::CompoundIndex c;
    out << '\n';
    for (typename Factor::Index i; i.is_not_at_end(); ++i)
    {
        out << '[';
        for (typename Factor::Index j; j.is_not_at_end(); ++j)
        {
            out << t[c] << '\t';
            ++c;
        }
        out << "]\n";
    }
    return out;
}

// template specialization for the natural pairing in this particular coordinatization of Tensor2Antisymmetric_t
template <typename Factor>
struct NaturalPairing_t<Tensor2Antisymmetric_t<Factor> >
{
    typedef Tensor2Antisymmetric_t<Factor> Tensor2Antisymmetric;
    typedef typename Tensor2Antisymmetric::Scalar Scalar;
    typedef typename Tensor2Antisymmetric::Index Index;

    static Scalar component (Index const &i)
    {
        return Scalar(2); // the off-diagonal components occur twice (in the component matrix)
    }
};

#endif // TENSOR2ANTISYMMETRIC_HPP_
