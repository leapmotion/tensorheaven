#ifndef TENSOR2_HPP_
#define TENSOR2_HPP_

#include <ostream>

#include "core.hpp"
#include "expression_templates.hpp"
#include "typetuple.hpp"
#include "vector.hpp"

// general 2-tensor with no symmetries -- most general type of 2-tensor
template <typename Factor1_, typename Factor2_, typename Derived_ = NullType>
struct Tensor2_t : public Vector_t<typename Factor1_::Scalar,
                                   Factor1_::DIM*Factor2_::DIM,
                                   typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                                          Tensor2_t<Factor1_,Factor2_,Derived_>,
                                                          Derived_>::T>
{
    enum { FACTOR_SCALAR_TYPES_ARE_EQUAL = Lvd::Meta::Assert<Lvd::Meta::TypesAreEqual<typename Factor1_::Scalar,typename Factor2_::Scalar>::v>::v };

    typedef Vector_t<typename Factor1_::Scalar,
                     Factor1_::DIM*Factor2_::DIM,
                     typename Lvd::Meta::If<Lvd::Meta::TypesAreEqual<Derived_,NullType>::v,
                                            Tensor2_t<Factor1_,Factor2_,Derived_>,
                                            Derived_>::T> Parent;
    typedef typename Parent::Scalar Scalar;
    using Parent::DIM;
    typedef typename Parent::Derived Derived;
    typedef typename Parent::Index Index;
    typedef Factor1_ Factor1;
    typedef Factor2_ Factor2;
    typedef CompoundIndex_t<typename TypeTuple_t<typename Factor1::Index,typename Factor2::Index>::T> CompoundIndex;

    Tensor2_t (WithoutInitialization const &w) : Parent(w) { }
    Tensor2_t (Scalar fill_with) : Parent(fill_with) { }

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
    
    // type conversion operator for canonical coercion to the Factor1 or Factor2 factor type when the
    // tensor is Factor1 \otimes OneDimVectorSpace  or  OneDimVectorSpace \otimes Factor2.
//     template <typename Factor>
//     operator Factor const & () const
//     {
//         Lvd::Meta::Assert<(Lvd::Meta::TypesAreEqual<Factor,Factor1>::v && Factor2::DIM == 1) || (Lvd::Meta::TypesAreEqual<Factor,Factor2>::v && Factor1::DIM == 1)>();
//         return *reinterpret_cast<Factor const *>(&Parent::m[0]);
//     }
//     operator Factor1 const & () const
//     {
//         Lvd::Meta::Assert<(Factor2::DIM == 1)>();
//         return *reinterpret_cast<Factor1 const *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
//     }
    Factor1 const &as_factor1 () const
    {
        Lvd::Meta::Assert<(Factor2::DIM == 1)>();
        return *reinterpret_cast<Factor1 *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }
    // this could be implemented as "operator Factor1 & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    Factor1 &as_factor1 ()
    {
        Lvd::Meta::Assert<(Factor2::DIM == 1)>();
        return *reinterpret_cast<Factor1 *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }
    // type conversion operator for canonical coercion to the Factor2 factor type when the
    // tensor is a tensor product of a 1-dimensional vector space with Factor2.
//     operator Factor2 const & () const
//     {
//         Lvd::Meta::Assert<(Factor1::DIM == 1)>();
//         return *reinterpret_cast<Factor2 const *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
//     }
    Factor2 const &as_factor2 () const
    {
        Lvd::Meta::Assert<(Factor1::DIM == 1)>();
        return *reinterpret_cast<Factor2 *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }
    // this could be implemented as "operator Factor2 & ()" but it would be bad to make implicit casts that can be used to change the value of this.
    Factor2 &as_factor2 ()
    {
        Lvd::Meta::Assert<(Factor1::DIM == 1)>();
        return *reinterpret_cast<Factor2 *>(&Parent::m[0]); // super C-like, but should be no problem because there is no virtual inheritance
    }

    // dumb, but the compiler wouldn't inherit implicitly, and won't parse a "using" statement
    Scalar const &operator [] (Index const &i) const { return Parent::operator[](i); }
    // dumb, but the compiler wouldn't inherit implicitly, and won't parse a "using" statement
    Scalar &operator [] (Index const &i) { return Parent::operator[](i); }

    template <typename Index1, typename Index2>
    Scalar const &operator [] (CompoundIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &c) const
    {
        // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking.
        // the compiler should optimize it out anyway.
        typename Factor1::Index(c.template el<0>());
        typename Factor2::Index(c.template el<1>());
        if (c.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return component_access_without_range_check(c.value());
    }
    template <typename Index1, typename Index2>
    Scalar &operator [] (CompoundIndex_t<TypeList_t<Index1,TypeList_t<Index2> > > const &c)
    {
        // NOTE: this construction is unnecessary to the code, but IS necessary to the compile-time type checking
        // the compiler should optimize it out anyway.
        typename Factor1::Index(c.template el<0>());
        typename Factor2::Index(c.template el<1>());
        if (c.is_at_end())
            throw std::invalid_argument("index out of range");
        else
            return component_access_without_range_check(c.value());
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
        return ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList,FORCE_CONST>(*this);
    }
    template <char SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList,DONT_FORCE_CONST> expr ()
    {
        Lvd::Meta::Assert<(SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,TypeList_t<NamedIndex_t<Derived,SYMBOL> >,EmptyTypeList,DONT_FORCE_CONST>(*this);
    }

    // a 2-tensor can be indexed by the pair of factor indices (Factor1::Index, Factor2::Index)
    // Dear Bjarne, please forgive me for this template metaprogramming atrocity.  Sincerely, Victor.
    template <char FACTOR1_SYMBOL, char FACTOR2_SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       typename TypeTuple_t<
                                           NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                           NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                           >::T,
                                       typename SummedIndexTypeList_t<
                                           typename TypeTuple_t<
                                               NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                               NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                               >::T
                                           >::T,
                                       FORCE_CONST
                                       > operator () (
        NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL> const &,
        NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL> const &) const
    {
        return expr<FACTOR1_SYMBOL,FACTOR2_SYMBOL>();
    }
    template <char FACTOR1_SYMBOL, char FACTOR2_SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       typename TypeTuple_t<
                                           NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                           NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                           >::T,
                                       typename SummedIndexTypeList_t<
                                           typename TypeTuple_t<
                                               NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                               NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                               >::T
                                           >::T,
                                       DONT_FORCE_CONST
                                       > operator () (
        NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL> const &,
        NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL> const &)
    {
        return expr<FACTOR1_SYMBOL,FACTOR2_SYMBOL>();
    }
    // the 2-index analog of expr<SYMBOL>()
    template <char FACTOR1_SYMBOL, char FACTOR2_SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       typename TypeTuple_t<
                                           NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                           NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                           >::T,
                                       typename SummedIndexTypeList_t<
                                           typename TypeTuple_t<
                                               NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                               NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                               >::T
                                           >::T,
                                       FORCE_CONST
                                       > expr () const
    {
        Lvd::Meta::Assert<(FACTOR1_SYMBOL != '\0')>();
        Lvd::Meta::Assert<(FACTOR2_SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,
                                                  typename TypeTuple_t<
                                                      NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                                      NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                                      >::T,
                                                  typename SummedIndexTypeList_t<
                                                      typename TypeTuple_t<
                                                          NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                                          NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                                          >::T
                                                      >::T,
                                                  FORCE_CONST
                                                  >(this->as_derived());
    }
    template <char FACTOR1_SYMBOL, char FACTOR2_SYMBOL>
    ExpressionTemplate_IndexedObject_t<Derived,
                                       typename TypeTuple_t<
                                           NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                           NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                           >::T,
                                       typename SummedIndexTypeList_t<
                                           typename TypeTuple_t<
                                               NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                               NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                               >::T
                                           >::T,
                                       DONT_FORCE_CONST
                                       > expr ()
    {
        Lvd::Meta::Assert<(FACTOR1_SYMBOL != '\0')>();
        Lvd::Meta::Assert<(FACTOR2_SYMBOL != '\0')>();
        return ExpressionTemplate_IndexedObject_t<Derived,
                                                  typename TypeTuple_t<
                                                      NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                                      NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                                      >::T,
                                                  typename SummedIndexTypeList_t<
                                                      typename TypeTuple_t<
                                                          NamedIndex_t<typename Factor1::Derived,FACTOR1_SYMBOL>,
                                                          NamedIndex_t<typename Factor2::Derived,FACTOR2_SYMBOL>
                                                          >::T
                                                      >::T,
                                                  DONT_FORCE_CONST
                                                  >(this->as_derived());
    }

    static std::string type_as_string ()
    {
        // TODO: return Derived's type_as_string value?
//         if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
//             return "Tensor2_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>';
//         else
//             return Derived::type_as_string();
        // for now, just return this type string
        if (Lvd::Meta::TypesAreEqual<Derived_,NullType>::v)
            return "Tensor2_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + '>';
        else
            return "Tensor2_t<" + TypeStringOf_t<Factor1>::eval() + ',' + TypeStringOf_t<Factor2>::eval() + ',' + TypeStringOf_t<Derived>::eval() + '>';
    }

private:

    // functions between the indexing schemes -- compound index is (row,col) with row > col and vector index is contiguous.
    static Uint32 rowcol_index_to_contiguous_index (Uint32 row, Uint32 col) 
    {
        return Factor2::DIM*row + col;
    }
    static void contiguous_index_to_rowcol_index (Uint32 i, Uint32 &row, Uint32 &col)
    {
        row = i / Factor2::DIM;
        col = i % Factor2::DIM;
    }
};

template <typename Factor1, typename Factor2, typename Derived>
std::ostream &operator << (std::ostream &out, Tensor2_t<Factor1,Factor2,Derived> const &t)
{
    typedef Tensor2_t<Factor1,Factor2,Derived> Tensor2;

    if (Tensor2::DIM == 0)
        return out << "[]";

    typename Tensor2::CompoundIndex c;
    out << '\n';
    for (typename Factor1::Index i; i.is_not_at_end(); ++i)
    {
        out << '[';
        for (typename Factor2::Index j; j.is_not_at_end(); ++j)
        {
            out << t[c] << '\t';
            ++c;
        }
        out << "]\n";
    }
    return out;
}


#endif // TENSOR2_HPP_
