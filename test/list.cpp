#include <iostream>
#include <string>

#include "list.hpp"
#include "typelist_utility.hpp"
#include "typetuple.hpp"

int main (int argc, char **argv)
{
    typedef TypeTuple_t<int,char>::T X;
    typedef TypeTuple_t<int,char,float>::T Y;
    typedef TypeTuple_t<List_t<X>,List_t<Y> >::T Z;

    std::cout << HasNontrivialIntersectionAsSets_t<X,Y>::V << '\n';
    std::cout << HasNontrivialIntersectionAsSets_t<Z,Y>::V << '\n';
    std::cout << HasNontrivialIntersectionAsSets_t<EmptyTypeList,Y>::V << '\n';
    std::cout << HasNontrivialIntersectionAsSets_t<EmptyTypeList,EmptyTypeList>::V << '\n';

    typedef List_t<X> LX;
    typedef List_t<Y> LY;

    LY y(42, LY::BodyList('a', LY::BodyList::BodyList(123.4f)));
    std::cout << y << '\n';

    LX x(y);
    std::cout << x << '\n';

    LY y2(x);
    std::cout << y2 << '\n';

    List_t<Z> z(x);
    std::cout << z << '\n';


    return 0;
}
