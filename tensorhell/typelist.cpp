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

template <typename T_> struct IsFloat_f { static bool const V = false; };
template <> struct IsFloat_f<float> { static bool const V = true; };
template <> struct IsFloat_f<double> { static bool const V = true; };
template <> struct IsFloat_f<long double> { static bool const V = true; };

MAKE_1_ARY_VALUE_EVALUATOR(IsFloat, bool);

int main (int argc, char **argv)
{
    typedef TypeList_t<float,TypeList_t<char,TypeList_t<double,TypeList_t<bool> > > > X;
    typedef TypeList_t<A,TypeList_t<B,TypeList_t<C,TypeList_t<D> > > > Y;
    typedef TypeList_t<T,TypeList_t<U,TypeList_t<V,TypeList_t<W> > > > Z;
    typedef TypeList_t<X,TypeList_t<Y,TypeList_t<Z> > > XYZ;

    typedef Zip_t<XYZ>::T Zipped;
    typedef Unzip_t<Zipped>::T Unzipped;
    cout << type_string_of<XYZ>() << '\n';
    cout << type_string_of<Zipped>() << '\n';
    cout << type_string_of<Unzipped>() << '\n';
    STATIC_ASSERT_TYPES_ARE_EQUAL(XYZ,Unzipped);
    cout << '\n';

    cout << FORMAT_VALUE((type_string_of<OnEach_f<X,IsFloat_e>::T>())) << '\n';
    cout << FORMAT_VALUE((And_f<OnEach_f<X,IsFloat_e>::T>::V)) << '\n';
    cout << FORMAT_VALUE((Or_f<OnEach_f<X,IsFloat_e>::T>::V)) << '\n';
    cout << '\n';
    cout << FORMAT_VALUE((type_string_of<OnEach_f<X,NegationOfPredicate_e<IsFloat_e> >::T>())) << '\n';
    cout << FORMAT_VALUE((And_f<OnEach_f<X,NegationOfPredicate_e<IsFloat_e> >::T>::V)) << '\n';
    cout << FORMAT_VALUE((Or_f<OnEach_f<X,NegationOfPredicate_e<IsFloat_e> >::T>::V)) << '\n';
    cout << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(type_string_of<X>()) << '\n';
    cout << '\n';
    cout << FORMAT_VALUE(type_string_of<Head_f<X>::T>()) << '\n';
    cout << FORMAT_VALUE(type_string_of<Body_f<X>::T>()) << '\n';
    cout << '\n';
    cout << FORMAT_VALUE(Length_f<X>::V) << '\n';
    cout << '\n';
    cout << FORMAT_VALUE((Contains_f<X,float>::V)) << '\n';
    cout << FORMAT_VALUE((Contains_f<X,Sint8>::V)) << '\n';
    cout << FORMAT_VALUE((Contains_f<X,Uint32>::V)) << '\n';
    cout << '\n';
    cout << FORMAT_VALUE((IndexOfFirstOccurrence_f<X,float>::V)) << '\n';
    cout << FORMAT_VALUE((IndexOfFirstOccurrence_f<X,Sint8>::V)) << '\n';
    cout << FORMAT_VALUE((IndexOfFirstOccurrence_f<X,Uint32>::V)) << '\n';
    cout << '\n';
    cout << FORMAT_VALUE((type_string_of<Element_f<X,0>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<Element_f<X,1>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<Element_f<X,2>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<Element_f<X,3>::T>())) << '\n';
    cout << '\n';
    cout << FORMAT_VALUE((type_string_of<TrailingTypeList_f<X,0>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TrailingTypeList_f<X,1>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TrailingTypeList_f<X,2>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TrailingTypeList_f<X,3>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TrailingTypeList_f<X,4>::T>())) << '\n';
    cout << '\n';
    cout << FORMAT_VALUE((type_string_of<LeadingTypeList_f<X,0>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<LeadingTypeList_f<X,1>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<LeadingTypeList_f<X,2>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<LeadingTypeList_f<X,3>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<LeadingTypeList_f<X,4>::T>())) << '\n';
    cout << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,0,0>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,0,1>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,1,1>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,0,2>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,1,2>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,2,2>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,0,3>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,1,3>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,2,3>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,3,3>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,0,4>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,1,4>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,2,4>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,3,4>::T>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<TypeListRange_f<X,4,4>::T>())) << '\n';
    cout << '\n';

    typedef EmptyTypeList Length0;
    typedef TypeList_t<A> Length1;
    typedef TypeList_t<A,
            TypeList_t<B> > Length2;
    typedef TypeList_t<A,
            TypeList_t<B,
            TypeList_t<C> > > Length3;
    typedef TypeList_t<Length0,
            TypeList_t<Length1,
            TypeList_t<Length2,
            TypeList_t<Length3> > > > MetaTypeList;
    cout << FORMAT_VALUE((type_string_of<Length0>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<Length1>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<Length2>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<Length3>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<MetaTypeList>())) << '\n';
    cout << FORMAT_VALUE((type_string_of<OnEach_f<MetaTypeList,Length_e>::T>())) << '\n';
    cout << '\n';
    typedef OnEach_f<MetaTypeList,Length_e>::T LengthTypeList;
    cout << FORMAT_VALUE((Sum_f<EmptyTypeList,Uint32>::V)) << '\n';
    cout << FORMAT_VALUE((Sum_f<LengthTypeList,Uint32>::V)) << '\n';
    cout << FORMAT_VALUE((Product_f<EmptyTypeList,Uint32>::V)) << '\n';
    cout << FORMAT_VALUE((Product_f<LengthTypeList,Uint32>::V)) << '\n';
    cout << FORMAT_VALUE((Min_f<LengthTypeList,Uint32>::V)) << '\n';
    cout << FORMAT_VALUE((Max_f<LengthTypeList,Uint32>::V)) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(type_string_of<Length0>()) << '\n';
    cout << FORMAT_VALUE(type_string_of<float>()) << '\n';
    cout << FORMAT_VALUE(type_string_of<X>()) << '\n';
    cout << '\n';

    cout << FORMAT_VALUE(type_string_of(3.0f)) << '\n';
    cout << FORMAT_VALUE(type_string_of(std::string("blah"))) << '\n';
    cout << '\n';

    return 0;
}
