#include <iostream>
#include <string>

#include "tenh/meta/typetuple.hpp"

using namespace Tenh;
using namespace std;

int main (int argc, char **argv)
{
    cout << type_string_of<TypeTuple_f<>::T>() << '\n';
    cout << type_string_of<TypeTuple_f<int>::T>() << '\n';
    cout << type_string_of<TypeTuple_f<int,bool>::T>() << '\n';
    cout << type_string_of<TypeTuple_f<int,bool,char>::T>() << '\n';
    cout << type_string_of<TypeTuple_f<int,bool,char,float>::T>() << '\n';
    cout << type_string_of<TypeTuple_f<int,bool,char,float,double>::T>() << '\n';
    cout << type_string_of<TypeTuple_f<int,bool,char,float,double,short int>::T>() << '\n';
    cout << type_string_of<TypeTuple_f<int,bool,char,float,double,short int,unsigned char>::T>() << '\n';
    cout << type_string_of<TypeTuple_f<int,bool,char,float,double,short int,unsigned char,string>::T>() << '\n';

    return 0;
}
