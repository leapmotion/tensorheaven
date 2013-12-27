// ///////////////////////////////////////////////////////////////////////////
// tenh/print_multiindexable.hpp by Victor Dods, created 2013/08/22
// Copyright Leap Motion Inc.
// ///////////////////////////////////////////////////////////////////////////

#ifndef TENH_PRINT_MULTIINDEXABLE_HPP_
#define TENH_PRINT_MULTIINDEXABLE_HPP_

#include "tenh/core.hpp"

#include <ostream>
#include <sstream>

#include "tenh/multiindex.hpp"

namespace Tenh {

template <typename MultiIndexable>
void print_multiindexable (std::ostream &out, MultiIndexable const &t, EmptyTypeList const &)
{
    typedef typename MultiIndexable::Scalar Scalar;
    typedef typename MultiIndexable::MultiIndex MultiIndex;
    STATIC_ASSERT_TYPES_ARE_EQUAL(typename MultiIndex::IndexTypeList, EmptyTypeList);
    out << Scalar(t);
}

// will print any order tensor in a nice-looking justified way.  if the order is greater
// than 1, this will print newlines, notably including the first character.
template <typename MultiIndexable, typename HeadIndexType, typename BodyIndexTypeList>
void print_multiindexable (std::ostream &out, MultiIndexable const &t, TypeList_t<HeadIndexType,BodyIndexTypeList> const &)
{
    typedef typename MultiIndexable::MultiIndex MultiIndex;
    typedef TypeList_t<HeadIndexType,BodyIndexTypeList> AllegedIndexTypeList;
    STATIC_ASSERT_TYPES_ARE_EQUAL(typename MultiIndex::IndexTypeList, AllegedIndexTypeList);
    static Uint32 const ORDER = MultiIndex::LENGTH;
    static Uint32 const COMPONENT_COUNT_OF_LAST_INDEX = Element_f<typename MultiIndex::IndexTypeList,ORDER-1>::T::COMPONENT_COUNT;

    // find the maximum component length, as printed in an ostream, so that
    // the actual output can be nicely justified and look awesome.
    Uint32 max_component_width = 0;
    for (MultiIndex m; m.is_not_at_end(); ++m)
    {
        std::ostringstream sout;
        sout << t[m];
        if (sout.str().length() > max_component_width)
            max_component_width = sout.str().length();
    }

    if (ORDER > 1)
        out << '\n';
    for (Uint32 j = 0; j < ORDER; ++j)
        out << "[ ";
    bool first_line = true;
    for (MultiIndex m; m.is_not_at_end(); ++m)
    {
        Uint32 c = trailing_zero_count(m);
        if (c > 0 && !first_line)
        {
            for (Uint32 j = 0; j < c; ++j)
                out << " ]";
            for (Uint32 j = 0; j < c; ++j)
                out << '\n';
            for (Uint32 j = 0; j < ORDER-c; ++j)
                out << "  ";
            for (Uint32 j = 0; j < c; ++j)
                out << "[ ";
        }

        out.setf(std::ios_base::right);
        out.width(max_component_width);
        out << t[m];

// this is to allow 0 and 1 dimensional factors to work
#ifdef __clang_version__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtautological-compare"
#endif // __clang_version__

        if (m.value_of_index(ORDER-1) < COMPONENT_COUNT_OF_LAST_INDEX-1)
            out << ' ';

#ifdef __clang_version__
#pragma GCC diagnostic pop
#endif // __clang_version__

        first_line = false;
    }
    for (Uint32 j = 0; j < ORDER; ++j)
        out << " ]";
}

} // end of namespace Tenh

#endif // TENH_PRINT_MULTIINDEXABLE_HPP_
