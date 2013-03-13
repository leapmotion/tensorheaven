#include <iostream>

#include "compound_access_index.hpp"
#include "typetuple.hpp"

template <Uint32 DIM>
struct AccessIndex
{
    AccessIndex () : m(0) { }
    AccessIndex (Uint32 i) : m(i) { }
    
    bool is_at_end () const { return m >= DIM; }
    bool is_not_at_end () const { return m < DIM; }
    Uint32 value () const { return m; }
    void operator ++ () { ++m; }
    void reset () { m = 0; }
    
private:

    Uint32 m;
};

int main (int argc, char **argv)
{
    typedef AccessIndex<3> I3;
    typedef AccessIndex<4> I4;
    typedef AccessIndex<5> I5;
    typedef CompoundAccessIndex_t<TypeTuple_t<I3>::T> X;
    typedef CompoundAccessIndex_t<TypeTuple_t<I3,I4>::T> Y;
    typedef CompoundAccessIndex_t<TypeTuple_t<I3,I4,I5>::T> Z;
    
    for (X x; x.is_not_at_end(); ++x)
    {
        x.print(std::cout);
        std::cout << '\n';
    }
    std::cout << '\n';
    
    for (Y y; y.is_not_at_end(); ++y)
    {
        y.print(std::cout);
        std::cout << '\n';
    }
    std::cout << '\n';
    
    for (Z z; z.is_not_at_end(); ++z)
    {
        //z.print(std::cout);
        //std::cout << '\n';
        std::cout << z.el<0>().value() << ", " << z.el<1>().value() << ", " << z.el<2>().value() << '\n';
    }
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
        
        x.print(std::cout); std::cout << '\n';
        y.print(std::cout); std::cout << '\n';
        z.print(std::cout); std::cout << '\n';
        z.trailing_list<0>().print(std::cout); std::cout << '\n';
        z.trailing_list<1>().print(std::cout); std::cout << '\n';
        z.trailing_list<2>().print(std::cout); std::cout << '\n';
//         z.trailing_list<3>().print(std::cout); std::cout << '\n';
    }

    return 0;
}
