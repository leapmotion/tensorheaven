// ///////////////////////////////////////////////////////////////////////////
// tenh/tensor2antisymmetric.hpp by Victor Dods, created 2013/03/20
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_TENSOR2ANTISYMMETRIC_HPP_
#define TENH_TENSOR2ANTISYMMETRIC_HPP_

#include <cmath> // TEMP until rowcol/contiguous index map is implemented as a lookup table
#include <ostream>

#include "tenh/core.hpp"
#include "tenh/expression_templates.hpp"
#include "tenh/naturalpairing.hpp"
#include "tenh/meta/typetuple.hpp"
#include "tenh/vector.hpp"

namespace Tenh {

// antisymmetric 2-tensor (its transpose is equal to its negative)
template <typename Factor1_, typename Factor2_ = Factor1_, typename Derived_ = NullType>
struct Tensor2Antisymmetric_t : public Vector_t<typename Factor1_::Scalar,
                                                ((Factor1_::DIM-1)*Factor1_::DIM)/2,
                                                typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                       Tensor2Antisymmetric_t<Factor1_,Factor2_,Derived_>,
                                                                       Derived_>::T>
{
    enum { FACTOR1_AND_FACTOR2_MUST_BE_IDENTICAL = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<Factor1_,Factor2_>::v>::v };

    typedef Vector_t<typename Factor1_::Scalar,
                     ((Factor1_::DIM-1)*Factor1_::DIM)/2,
                     typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Tensor2Antisymmetric_t<Factor1_,Factor2_,Derived_>,
                                            Derived_>::T> Parent;
    typedef typename Parent::Scalar Scalar;
    using Parent::DIM;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Index Index;
    typedef Factor1_ Factor;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;
    typedef CompoundIndex_t<typename TypeTuple_t<typename Factor::Index,typename Factor::Index>::T> CompoundIndex;
    static Uint32 const STRICTLY_LOWER_TRIANGULAR_COMPONENT_COUNT = ((Factor::DIM-1)*Factor::DIM)/2;

    Tensor2Antisymmetric_t (WithoutInitialization const &w) : Parent(w) { }
    Tensor2Antisymmetric_t (Scalar fill_with) : Parent(fill_with) { }

    template <typename BundleIndexTypeList, typename BundledIndex>
    static CompoundIndex_t<BundleIndexTypeList> bundle_index_map (BundledIndex const &b)
    {
        typedef typename BundleIndexTypeList::template El_t<0>::T Index1;
        typedef typename BundleIndexTypeList::template El_t<1>::T Index2;
        // this is just to check that there is a valid conversion to the requested CompoundIndex_t type.
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
//     template <typename SourceIndexType, typename SplitIndexTypeList>
//     static SourceIndexType split_index_map (CompoundIndex_t<SplitIndexTypeList> const &s)
//     {
//         // this is just to check that there is a valid conversion from the requested CompoundIndex_t type.
//         // it doesn't actually produce any side-effects, and should be optimized out.
//         {
//             Lvd::Meta::Assert<SplitIndexTypeList::LENGTH == 2>();
//             typename Factor1::Index i1(s.template el<0>());
//             typename Factor2::Index i2(s.template el<1>());
//             // check that SourceIndexType has a conversion to Index
//             SourceIndexType t;
//             Index i(t);
//         }
//         Uint32 row = s.template el<0>().value();
//         Uint32 col = s.template el<1>().value();
//         return SourceIndexType(rowcol_index_to_contiguous_index(row, col));
//     }

    // TODO: because Factor1 and Factor2 are identical, it doesn't make sense to
    // have a type coercion to either one unless they are 1-dimensional, in which case
    // it can be a type coercion to the Scalar type.  however, an antisymmetric
    // 2-tensor on a 1-dimensional space is always zero.

    // TODO: because the diagonal is always zero, there is an easy type coercion to Tensor2Diagonal_t

    using Parent::operator[];

    // using two indices in a Tensor2Antisymmetric_t is breaking apart the Index type and using it
    // as a general tensor -- this is where the fancy indexing scheme happens.
    // Index1 could be Factor1::Index or Factor1::CompoundIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::CompoundIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    Scalar operator [] (CompoundIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &c) const
    {
//         // NOTE: these constructions are unnecessary to the code, but ARE necessary to the compile-time type checking
//         // the compiler should optimize it out anyway.
//         typename Factor1::Index(c.template el<0>());
//         typename Factor2::Index(c.template el<1>());
        return component(c.template el<0>(), c.template el<1>());
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
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList,FORCE_CONST> operator () (
        NamedIndex_t<Derived,SYMBOL> const &) const
    {
        return expr<SYMBOL>();
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList,DONT_FORCE_CONST> operator () (
        NamedIndex_t<Derived,SYMBOL> const &)
    {
        return expr<SYMBOL>();
    }
    // the corresponding outer product example here would be
    // u.expr<'i'>() * v.expr<'j'>()
    // this override of the Parent's operator() is necessary so that the expression template
    // knows that the operand is actually a Tensor2_t.
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList,FORCE_CONST> expr () const
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList,FORCE_CONST>(this->as_derived());
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList,DONT_FORCE_CONST> expr ()
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList,DONT_FORCE_CONST>(this->as_derived());
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
                                           >::T,
                                       FORCE_CONST
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
                                           >::T,
                                       DONT_FORCE_CONST
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
                                           >::T,
                                       FORCE_CONST
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
                                                      >::T,
                                                  FORCE_CONST
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
                                           >::T,
                                       DONT_FORCE_CONST
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
                                                      >::T,
                                                  DONT_FORCE_CONST
                                                  >(this->as_derived());
    }

    // access 2-tensor components
    // Index1 could be Factor1::Index or Factor1::CompoundIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::CompoundIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    Scalar component (Index1 const &i1, Index2 const &i2) const
    {
        if (i1.is_at_end() || i2.is_at_end())
            throw std::invalid_argument("index/indices out of range");

        if (!Factor1::component_corresponds_to_memory_location(i1) || !Factor2::component_corresponds_to_memory_location(i2))
            return Scalar(0);

        CompoundIndex c(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        if (!component_corresponds_to_memory_location(c))
            return Scalar(0);

        return Factor1::scalar_factor_for_component(i1) *
               Factor2::scalar_factor_for_component(i2) *
               scalar_factor_for_component(c) *
               operator[](vector_index_of(c));
    }
    // write 2-tensor components -- will throw if a component doesn't correspond to a memory location
    // Index1 could be Factor1::Index or Factor1::CompoundIndex (checked by its use in the other functions)
    // Index2 could be Factor2::Index or Factor2::CompoundIndex (checked by its use in the other functions)
    template <typename Index1, typename Index2>
    Scalar &set_component (Index1 const &i1, Index2 const &i2, Scalar s)
    {
        if (i1.is_at_end() || i2.is_at_end())
            throw std::invalid_argument("index/indices out of range");

        if (!Factor1::component_corresponds_to_memory_location(i1) || !Factor2::component_corresponds_to_memory_location(i2))
            throw std::invalid_argument("this tensor component is not writable");

        CompoundIndex c(Factor1::vector_index_of(i1), Factor2::vector_index_of(i2));
        if (!component_corresponds_to_memory_location(c))
            throw std::invalid_argument("this tensor component is not writable");

        // write to the component, but divide through by the total scale factor for the component.
        operator[](vector_index_of(c)) = s / (Factor1::scalar_factor_for_component(i1) * Factor2::scalar_factor_for_component(i2) * scalar_factor_for_component(c));
    }
    using Parent::component_corresponds_to_memory_location;
    using Parent::scalar_factor_for_component;
    using Parent::vector_index_of;
    // the diagonal is not stored in memory -- all components are zero.
    static bool component_corresponds_to_memory_location (CompoundIndex const &c)
    {
        return c.template el<0>() != c.template el<1>(); // off diagonal elements correspond to memory locations
    }
    // the diagonal is zero, the upper triangular components have a scalar factor of -1, and 1 otherwise.
    static Scalar scalar_factor_for_component (CompoundIndex const &c)
    {
        if (c.template el<0>() == c.template el<1>()) // diagonal component
            return Scalar(0); // not actually used, but there for completeness
        else if (c.template el<0>() < c.template el<1>()) // upper triangular component
            return Scalar(-1);
        else // lower triangular component
            return Scalar(1);
    }
    // this should return iff component_corresponds_to_memory_location(c) returns true.
    static Index vector_index_of (CompoundIndex const &c)
    {
        return Index::range_unchecked(rowcol_index_to_contiguous_index(c.template el<0>().value(), c.template el<1>().value()));
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
        // strict lower triangle elements are stored contiguously in row-major order:
        // [ . . . ... ]
        // [ 0 . . ... ]
        // [ 1 2 . ... ]
        // [ 3 4 5 ... ]
        // ...
        // the index of the first element in row R is the (R-1)th triangular number, so the mapping
        // (row,col) to contiguous index i is i := r*(r-1)/2 + c.
        if (row == col)
            throw std::invalid_argument("there is no contiguous index corresponding to a diagonal component index");
        if (row < col)
            std::swap(row, col);
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

} // end of namespace Tenh

#endif // TENH_TENSOR2ANTISYMMETRIC_HPP_
