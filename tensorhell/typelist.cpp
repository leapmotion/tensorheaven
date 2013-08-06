#include <iostream>
#include <string>

#include "tenh/meta/typelist.hpp"
#include "tenh/meta/typelist_utility.hpp"

using namespace Tenh;
using namespace std;

struct A { static std::string type_as_string () { return "A"; } };
struct B { static std::string type_as_string () { return "B"; } };
struct C { static std::string type_as_string () { return "C"; } };
struct D { static std::string type_as_string () { return "D"; } };

struct T { static std::string type_as_string () { return "T"; } };
struct U { static std::string type_as_string () { return "U"; } };
struct V { static std::string type_as_string () { return "V"; } };
struct W { static std::string type_as_string () { return "W"; } };

int main (int argc, char **argv)
{
    typedef TypeList_t<float,TypeList_t<char,TypeList_t<int,TypeList_t<bool> > > > X;
    typedef TypeList_t<A,TypeList_t<B,TypeList_t<C,TypeList_t<D> > > > Y;
    typedef TypeList_t<T,TypeList_t<U,TypeList_t<V,TypeList_t<W> > > > Z;
    typedef TypeList_t<X,TypeList_t<Y,TypeList_t<Z> > > XYZ;

    typedef Zip_t<XYZ>::T Zipped;
    typedef Unzip_t<Zipped>::T Unzipped;
    cout << TypeStringOf_t<XYZ>::eval() << '\n';
    cout << TypeStringOf_t<Zipped>::eval() << '\n';
    cout << TypeStringOf_t<Unzipped>::eval() << '\n';
    STATIC_ASSERT_TYPES_ARE_EQUAL(XYZ,Unzipped);

    return 0;
}
