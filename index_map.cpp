#include <iostream>
#include <string>

#include "index_map.hpp"


template <typename T_>
struct Property_t
{
    static std::string stringify_type () { return "UnspecifiedType"; }
    // template specializations should implement static std::string stringify (T const &t) { }
};

template <> 
struct Property_t<char>
{
    static std::string stringify_type () { return "char"; }
};

template <> 
struct Property_t<int>
{
    static std::string stringify_type () { return "int"; }
};

template <> 
struct Property_t<float>
{
    static std::string stringify_type () { return "float"; }
};

template <> 
struct Property_t<double>
{
    static std::string stringify_type () { return "double"; }
};

template <> 
struct Property_t<bool>
{
    static std::string stringify_type () { return "bool"; }
};



typedef TypeList_t<int,
        TypeList_t<float,
        TypeList_t<float,
        TypeList_t<int,
        TypeList_t<bool,
        TypeList_t<double,
        TypeList_t<float,
        TypeList_t<char,
        TypeList_t<bool,
        TypeList_t<char,
        TypeList_t<int,
        TypeList_t<bool> > > > > > > > > > > > X;







int main (int argc, char **argv)
{
    std::cout << X::LENGTH << '\n';
    std::cout << TypeStringOf_t<X>::eval() << '\n';
    std::cout << '\n';

    std::cout << '\n';

    std::cout << Occurrence_t<X,int>::COUNT << '\n';
    std::cout << Occurrence_t<X,float>::COUNT << '\n';
    std::cout << Occurrence_t<X,bool>::COUNT << '\n';
    std::cout << Occurrence_t<X,double>::COUNT << '\n';
    std::cout << Occurrence_t<X,char>::COUNT << '\n';
    std::cout << Occurrence_t<X,short int>::COUNT << "\n\n";

    std::cout << X::Contains_t<int>() << ", " << X::Contains_t<short int>() << "\n\n";

    {
        typedef UniqueTypesIn_t<X>::T U;
        std::cout << U::LENGTH << '\n';
        std::cout << TypeStringOf_t<U>::eval() << '\n';
        std::cout << UniqueTypesIn_t<EmptyTypeList>::T::LENGTH << "\n\n";
    }
    
    typedef ElementsWithMultiplicity_t<X,0>::T Freq0Types;
    typedef ElementsWithMultiplicity_t<X,1>::T Freq1Types;
    typedef ElementsWithMultiplicity_t<X,2>::T Freq2Types;
    typedef ElementsWithMultiplicity_t<X,3>::T Freq3Types;
    typedef ElementsWithMultiplicity_t<X,4>::T Freq4Types;
    std::cout << Freq0Types::LENGTH << '\n';
    std::cout << Freq1Types::LENGTH << '\n';
    std::cout << Freq2Types::LENGTH << '\n';
    std::cout << Freq3Types::LENGTH << '\n';
    std::cout << Freq4Types::LENGTH << '\n';
    std::cout << '\n';

    std::cout << X::IndexOf_t<int>::V << '\n';
    std::cout << X::IndexOf_t<float>::V << '\n';
    std::cout << X::IndexOf_t<bool>::V << '\n';
    std::cout << X::IndexOf_t<double>::V << '\n';
    std::cout << X::IndexOf_t<char>::V << '\n';
    std::cout << X::IndexOf_t<short int>::V << '\n';
    std::cout << '\n';
    
    std::cout << "unique types in " << TypeStringOf_t<X>::eval() << '\n';
    typedef IndexMap_t<X> IndexMap;
    typedef IndexMap::UniqueTypes U;
    std::cout << TypeStringOf_t<U>::eval() << '\n';
    // NOTE: this doesn't work because Tuple_t<U> is taking U (a TypeList) as the type
    // of the single element in a 1-tuple, instead of using the elements of the typelist U
    // as the tuple types for an n-tuple, where n is the length of U.
//     Tuple_t<U> t(-42, 4.5f, true, 100.6, 'a');//t(100.6, 4.5f, 'a', -42, true);
//     std::cout << TypeStringOf_t<Tuple_t<U> >::eval() << '\n';
//     std::cout << t << " |-> " << IndexMap::eval(t) << '\n';
    std::cout << '\n';
    
    typedef TailOfTypeList_t<U>::T Tail;
    std::cout << TypeStringOf_t<Tail>::eval() << '\n';
    std::cout << '\n';

    typedef AllButTailOfTypeList_t<U>::T AllButTail;
    std::cout << AllButTail::LENGTH << '\n';    
    std::cout << TypeStringOf_t<AllButTail>::eval() << '\n';
    std::cout << '\n';

    typedef ReversedTypeList_t<U>::T R;
    std::cout << R::LENGTH << '\n';    
    std::cout << TypeStringOf_t<R>::eval() << '\n';
    std::cout << '\n';

    {
        std::cout << "X = " << TypeStringOf_t<X>::eval() << '\n';
        std::cout << "X::TrailingTypeList_t<0>::T = " << TypeStringOf_t<X::TrailingTypeList_t<0>::T>::eval() << '\n';
        std::cout << "X::TrailingTypeList_t<4>::T = " << TypeStringOf_t<X::TrailingTypeList_t<4>::T>::eval() << '\n';
        std::cout << "X::TrailingTypeList_t<11>::T = " << TypeStringOf_t<X::TrailingTypeList_t<11>::T>::eval() << '\n';
        std::cout << "X::TrailingTypeList_t<12>::T = " << TypeStringOf_t<X::TrailingTypeList_t<12>::T>::eval() << '\n';
    }

    return 0;
}
