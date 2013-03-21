#ifndef TENSOR2SYMMETRIC_HPP_
#define TENSOR2SYMMETRIC_HPP_

#include <ostream>

#include "core.hpp"
#include "expression_templates.hpp"
#include "naturalpairing.hpp"
#include "typetuple.hpp"
#include "vector.hpp"

// symmetric 2-tensor (it is equal to its transpose)
template <typename Factor_, typename Derived_ = NullType>
struct Tensor2Symmetric_t : public Vector_t<typename Factor_::Scalar,
                                            ((Factor_::DIM+1)*Factor_::DIM)/2,
                                            typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                                   Tensor2Symmetric_t<Factor_,Derived_>,
                                                                   Derived_>::T>
{
    typedef Vector_t<typename Factor_::Scalar,
                     ((Factor_::DIM+1)*Factor_::DIM)/2,
                     typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Tensor2Symmetric_t<Factor_,Derived_>,
                                            Derived_>::T> Parent;
    typedef typename Parent::Scalar Scalar;
    using Parent::DIM;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Index Index;
    typedef Factor_ Factor;
    typedef Factor Factor1;
    typedef Factor Factor2;
    typedef CompoundIndex_t<typename TypeTuple_t<typename Factor::Index,typename Factor::Index>::T> CompoundIndex;
    static Uint32 const DIAGONAL_COMPONENT_COUNT = Factor::DIM;
    static Uint32 const STRICTLY_UPPER_TRIANGULAR_COMPONENT_COUNT = ((Factor::DIM-1)*Factor::DIM)/2;

    Tensor2Symmetric_t (WithoutInitialization const &w) : Parent(w) { }
    Tensor2Symmetric_t (Scalar fill_with) : Parent(fill_with) { }

    // TODO: because Factor1 and Factor2 are identical, it doesn't make sense to
    // have a type coercion to either one unless they are 1-dimensional, in which case
    // it can be a type coercion to the Scalar type.

//     // "break apart" a symmetric 2-tensor index (Index) into a tuple of indices (CompoundIndex)
//     static CompoundIndex Index_to_CompoundIndex (Index const &i)
//     {
//         return
//     }
//     // "bundle" a tuple of indices (CompoundIndex) back into a single symmetric 2-tensor index (Index).
//     static Index CompoundIndex_to_Index (CompoundIndex const &c)
//     {
//         // strict upper triangle components are stored contiguously in row-major order, starting at element D := DIM,
//         // being the shared dimension of the tensor factors.
//         //    [ x 1*D 1*D+1 ... 2*D-2 ]  there are D-1 elements in this row, starting column is 1
//         //    [ x   x 2*D-1 ... 3*D-4 ]  there are D-2 elements in this row, starting column is 2
//         //    [ ..................... ]  ...
//         // in row R, there are D-(R+1) elements in the row, starting in column R+1.
//         // thus (R,C) (with R > C) refers to offset:
//         //    R*D+C - (1 + 2 + ... R + (R+1)) = R*D+C - (R+1)*(R+2)/2
//         // because this starts at offset D, the actual offset is
//         //    D + R*D+C - (R+1)*(R+2)/2 = (R+1)*(2*D - (R+2))/2 + C
//         Uint32 row = c.el<0>().value();
//         Uint32 col = c.el<0>().value();
//         if (row >= col)
//             std::swap(row, col);
//         return (row+1)*(2*Factor::DIM-(row+2))/2 + col;
//     }

    // TODO: because the diagonal is indexed contiguously first, there is an easy type coercion to Tensor2Diagonal_t

    // dumb, but the compiler wouldn't inherit implicitly, and won't parse a "using" statement
    Scalar const &operator [] (Index const &i) const { return Parent::operator[](i); }
    // dumb, but the compiler wouldn't inherit implicitly, and won't parse a "using" statement
    Scalar &operator [] (Index const &i) { return Parent::operator[](i); }

    // using two indices in a Tensor2Symmetric_t is breaking apart the Index type and using it
    // as a general tensor -- this is where the fancy indexing scheme happens.
    template <typename Index1, typename Index2>
    Scalar const &operator [] (CompoundIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &c) const
    {
        if (c.is_at_end())
            throw std::invalid_argument("index out of range");

        // NOTE: these constructions are unnecessary to the code, but ARE necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        typename Factor::Index(c.template el<0>());
        typename Factor::Index(c.template el<1>());

        Uint32 row = c.template el<0>().value();
        Uint32 col = c.template el<1>().value();

        // index the diagonal first, then the strictly upper triangular in row-major order
        if (row == col)
            return this->component_access_without_range_check(row);

        // if we're in the strict lower triangle, switch the indices to get into the strict upper triangle
        if (col < row)
            std::swap(row, col);

        // strict upper triangle components are stored contiguously in row-major order, starting at element D := DIM,
        // being the shared dimension of the tensor factors.
        //    [ x 1*D 1*D+1 ... 2*D-2 ]  there are D-1 elements in this row, starting column is 1
        //    [ x   x 2*D-1 ... 3*D-4 ]  there are D-2 elements in this row, starting column is 2
        //    [ ..................... ]  ...
        // in row R, there are D-(R+1) elements in the row, starting in column R+1.
        // thus (R,C) (with R > C) refers to offset:
        //    R*D+C - (1 + 2 + ... R + (R+1)) = R*D+C - (R+1)*(R+2)/2
        // because this starts at offset D, the actual offset is
        //    D + R*D+C - (R+1)*(R+2)/2 = (R+1)*(2*D - (R+2))/2 + C
        return this->component_access_without_range_check((row+1)*(2*Factor::DIM-(row+2))/2 + col);
    }
    // there is redundant access to components here, since the strict lower triangle components are
    // really just aliases for the strict upper triangle components.
    template <typename Index1, typename Index2>
    Scalar &operator [] (CompoundIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &c)
    {
        // see the comments in the const version of operator[] for your code-diving pleasure
        if (c.is_at_end())
            throw std::invalid_argument("index out of range");
        typename Factor::Index(c.template el<0>());
        typename Factor::Index(c.template el<1>());
        Uint32 row = c.template el<0>().value();
        Uint32 col = c.template el<1>().value();
        if (row == col)
            return this->component_access_without_range_check(row);
        if (col < row)
            std::swap(row, col);
        return this->component_access_without_range_check(((row+1)*(2*DIAGONAL_COMPONENT_COUNT - (row+2)) >> 1) + col);
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
    ExpressionTemplate_IndexedTensor_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> operator () (
        NamedIndex_t<Derived,SYMBOL> const &) const
    {
        return expr<SYMBOL>();
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedTensor_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> operator () (
        NamedIndex_t<Derived,SYMBOL> const &)
    {
        return expr<SYMBOL>();
    }
    // the corresponding outer product example here would be
    // u.expr<'i'>() * v.expr<'j'>()
    // this override of the Parent's operator() is necessary so that the expression template
    // knows that the operand is actually a Tensor2_t.
    template <char SYMBOL>
    ExpressionTemplate_IndexedTensor_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> expr () const
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedTensor_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList>(this->as_derived());
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedTensor_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList> expr ()
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedTensor_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList>(this->as_derived());
    }

    // a 2-tensor can be indexed by the pair of factor indices (Factor1::Index, Factor2::Index)
    // Dear Bjarne, please forgive me for this template metaprogramming atrocity.  Sincerely, Victor.
    template <char FACTOR1_SYMBOL, char FACTOR2_SYMBOL>
    ExpressionTemplate_IndexedTensor_t<Derived,
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
    ExpressionTemplate_IndexedTensor_t<Derived,
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
    ExpressionTemplate_IndexedTensor_t<Derived,
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
        return ExpressionTemplate_IndexedTensor_t<Derived,
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
    ExpressionTemplate_IndexedTensor_t<Derived,
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
        return ExpressionTemplate_IndexedTensor_t<Derived,
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
//             return "Tensor2Symmetric_t<" + TypeStringOf_t<Factor>::eval() + '>';
//         else
//             return Derived::type_as_string();
        // for now, just return this type string
        if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
            return "Tensor2Symmetric_t<" + TypeStringOf_t<Factor>::eval() + '>';
        else
            return "Tensor2Symmetric_t<" + TypeStringOf_t<Factor>::eval() + ',' + TypeStringOf_t<Derived>::eval() + '>';
    }
};

template <typename Factor>
std::ostream &operator << (std::ostream &out, Tensor2Symmetric_t<Factor> const &t)
{
    typedef Tensor2Symmetric_t<Factor> Tensor2Symmetric;

    if (Tensor2Symmetric::DIM == 0)
        return out << "[]";

    typename Tensor2Symmetric::CompoundIndex c;
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

// template specialization for the natural pairing in this particular coordinatization of Tensor2Symmetric_t
template <typename Factor>
struct NaturalPairing_t<Tensor2Symmetric_t<Factor> >
{
    typedef Tensor2Symmetric_t<Factor> Tensor2Symmetric;
    typedef typename Tensor2Symmetric::Scalar Scalar;
    typedef typename Tensor2Symmetric::Index Index;

    static Scalar component (Index const &i)
    {
        if (i.value() < Tensor2Symmetric::DIAGONAL_COMPONENT_COUNT)
            return Scalar(1); // the diagonal components occur only once (in the component matrix)
        else
            return Scalar(2); // but the off-diagonal components occur twice (in the component matrix)
    }
};

#endif // TENSOR2SYMMETRIC_HPP_
