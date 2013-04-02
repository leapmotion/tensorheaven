#include <iostream>
#include <string>

#include "tenh/meta/list.hpp"
#include "tenh/meta/typelist_utility.hpp"
#include "tenh/meta/typetuple.hpp"

using namespace Tenh;

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

    {
        typedef TypeTuple_t<int,char,bool,float>::T A;
        typedef TypeTuple_t<int,char>::T B;
        typedef TypeTuple_t<bool,float>::T C;
        
        typedef List_t<A> ListA;
        typedef List_t<B> ListB;
        typedef List_t<C> ListC;
        
        ListB b(42, 'a');
        ListC c(true, 3.4f);
        ListA a(b |= c);
//         std::cout << FORMAT_VALUE(TypeStringOf_t<typeof(b |= c)>::eval()) << '\n'; // GCC extension
        std::cout << FORMAT_VALUE(b) << ", " << FORMAT_VALUE(c) << ", (b,c) = " << a << '\n';
    }

    {
        std::cout << FORMAT_VALUE((true >>= 42 >>= EmptyList::SINGLETON) |= ('a' >>= std::string("blahblah") >>= EmptyList::SINGLETON)) << '\n';
        std::cout << FORMAT_VALUE(true >>= EmptyList()) << '\n';
        std::cout << FORMAT_VALUE(EmptyList() |= EmptyList()) << '\n';
        std::cout << FORMAT_VALUE(('a' >>= EmptyList()) |= EmptyList()) << '\n';
        std::cout << FORMAT_VALUE(EmptyList() |= ('a' >>= EmptyList())) << '\n';
    }
    
    std::cout << FORMAT_VALUE(sizeof(EmptyList)) << '\n';

    return 0;
}
