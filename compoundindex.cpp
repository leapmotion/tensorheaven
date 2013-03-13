#include <iostream>

#include "compoundindex.hpp"
#include "typetuple.hpp"

template <Uint32 DIM>
struct Index_t
{
    Index_t () : m(0) { }
    Index_t (Uint32 i) : m(i) { }

    bool is_at_end () const { return m >= DIM; }
    bool is_not_at_end () const { return m < DIM; }
    Uint32 value () const { return m; }
    void operator ++ () { ++m; }
    void reset () { m = 0; }

    static std::string type_as_string () { return "Index_t<" + AS_STRING(DIM) + ">"; }

private:

    Uint32 m;
};

template <Uint32 DIM>
std::ostream &operator << (std::ostream &out, Index_t<DIM> const &i)
{
    return out << i.value();
}

int main (int argc, char **argv)
{
    typedef Index_t<3> I3;
    typedef Index_t<4> I4;
    typedef Index_t<5> I5;
    typedef Index_t<6> I6;
    typedef CompoundIndex_t<TypeTuple_t<I3>::T> X;
    typedef CompoundIndex_t<TypeTuple_t<I3,I4>::T> Y;
    typedef CompoundIndex_t<TypeTuple_t<I3,I4,I5>::T> Z;

    for (X x; x.is_not_at_end(); ++x)
        std::cout << x << '\n';
    std::cout << '\n';

    for (Y y; y.is_not_at_end(); ++y)
        std::cout << y << '\n';
    std::cout << '\n';

    for (Z z; z.is_not_at_end(); ++z)
        std::cout << z.el<0>().value() << ", " << z.el<1>().value() << ", " << z.el<2>().value() << '\n';
    std::cout << '\n';

    {
        Y y;
        ++y;
        ++y;
        ++y;
        ++y;
        ++y;
        ++y;
        Z z(y);
        X x(y);

        std::cout << TypeStringOf_t<X>::eval() << '\n';
        std::cout << TypeStringOf_t<Y>::eval() << '\n';
        std::cout << TypeStringOf_t<Z>::eval() << '\n';
        std::cout << TypeStringOf_t<Z::TrailingListType_t<1>::T>::eval() << '\n';
        std::cout << TypeStringOf_t<Z::TrailingListType_t<2>::T>::eval() << '\n';
        std::cout << TypeStringOf_t<Z::TrailingListType_t<3>::T>::eval() << '\n';

        std::cout << x << '\n';
        std::cout << y << '\n';
        std::cout << z << '\n';
        std::cout << z.trailing_list<0>() << '\n';
        std::cout << z.trailing_list<1>() << '\n';
        std::cout << z.trailing_list<2>() << '\n';
//         std::cout << z.trailing_list<3>() << '\n';
    }

    {

        typedef TypeTuple_t<I3,I4,I5,I6>::T UniqueTypeList;
        typedef TypeTuple_t<I3,I4,I3,I3,I5>::T TargetTypeList;
        CompoundIndex_t<UniqueTypeList> u;
        for (Uint32 i = 0; i < 202; ++i)
            ++u;
        std::cout << u << " -> " << CompoundIndexMap_t<UniqueTypeList,TargetTypeList>::eval(u) << '\n';
    }

    return 0;
}
