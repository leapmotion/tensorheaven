#include <iostream>
#include <string>

#include "tenh/list.hpp"
#include "tenh/meta/typetuple.hpp"

using namespace Tenh;

int main (int argc, char **argv)
{
    typedef Typle_t<int,char> X;
    typedef Typle_t<int,char,float> Y;
    typedef Typle_t<List_t<X>,List_t<Y>> Z;

    std::cout << HasNontrivialIntersectionAsSets_t<X,Y>::V << '\n';
    std::cout << HasNontrivialIntersectionAsSets_t<Z,Y>::V << '\n';
    std::cout << HasNontrivialIntersectionAsSets_t<Typle_t<>,Y>::V << '\n';
    std::cout << HasNontrivialIntersectionAsSets_t<Typle_t<>,Typle_t<>>::V << '\n';

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
        typedef Typle_t<int,char,bool,float> A;
        typedef Typle_t<int,char> B;
        typedef Typle_t<bool,float> C;

        typedef List_t<A> ListA;
        typedef List_t<B> ListB;
        typedef List_t<C> ListC;

        ListB b(42, 'a');
        ListC c(true, 3.4f);
        ListA a(b |= c);
//         std::cout << FORMAT_VALUE(type_string_of<typeof(b |= c)>()) << '\n'; // GCC extension
        std::cout << FORMAT_VALUE(b) << ", " << FORMAT_VALUE(c) << ", (b,c) = " << a << '\n';
    }

    {
        std::cout << FORMAT_VALUE((true >>= 42 >>= Static<EmptyList>::SINGLETON) |= ('a' >>= std::string("blahblah") >>= Static<EmptyList>::SINGLETON)) << '\n';
        std::cout << FORMAT_VALUE(true >>= EmptyList()) << '\n';
        std::cout << FORMAT_VALUE(EmptyList() |= EmptyList()) << '\n';
        std::cout << FORMAT_VALUE(('a' >>= EmptyList()) |= EmptyList()) << '\n';
        std::cout << FORMAT_VALUE(EmptyList() |= ('a' >>= EmptyList())) << '\n';
    }

    std::cout << FORMAT_VALUE(sizeof(EmptyList)) << '\n';

    {
        typedef Typle_t<int,int> T1;
        typedef List_t<T1> L1;
        typedef Typle_t<float,double> T2;
        typedef List_t<T2> L2;
        L2 l2(3.4f, List_t<Typle_t<double>>(100.1));
        L1 l1(l2);
        std::cout << FORMAT_VALUE(l2) << '\n';
        std::cout << FORMAT_VALUE(l1) << '\n';
    }
    return 0;
}
