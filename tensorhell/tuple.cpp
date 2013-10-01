#include <iostream>
#include <string>

#include "tenh/meta/tuple.hpp"

using namespace Tenh;
using namespace std;

int main (int argc, char **argv)
{
    cout << type_string_of<List_t<EmptyTypeList> >() << '\n';
    cout << type_string_of<Tuple_t<> >() << '\n'; // better way
    cout << type_string_of<Tuple_t<int> >() << '\n';
    cout << type_string_of<Tuple_t<int,char> >() << '\n';
    cout << type_string_of<Tuple_t<int,char,float> >() << '\n';
    cout << type_string_of<Tuple_t<int,char,float,bool> >() << '\n';
    cout << type_string_of<Tuple_t<int,char,float,bool,double> >() << '\n';
    cout << type_string_of<Tuple_t<int,char,float,bool,double,unsigned char> >() << '\n';
    cout << type_string_of<Tuple_t<int,char,float,bool,double,unsigned char,string> >() << '\n';
    cout << type_string_of<Tuple_t<int,char,float,bool,double,unsigned char,string,short int> >() << '\n';
    cout << '\n';

    cout << Tuple_t<>() << '\n';
    cout << Tuple_t<int>(42) << '\n';
    cout << Tuple_t<int,char>(42,'a') << '\n';
    cout << Tuple_t<int,char,float>(42,'a',4.5f) << '\n';
    cout << Tuple_t<int,char,float,bool>(42,'a',4.5f,true) << '\n';
    cout << Tuple_t<int,char,float,bool,double>(42,'a',4.5f,true,123.00001) << '\n';
    cout << Tuple_t<int,char,float,bool,double,unsigned char>(42,'a',4.5f,true,123.00001,'~') << '\n';
    cout << Tuple_t<int,char,float,bool,double,unsigned char,string>(42,'a',4.5f,true,123.00001,'~',"blah") << '\n';
    cout << Tuple_t<int,char,float,bool,double,unsigned char,string,short int>(42,'a',4.5f,true,123.00001,'~',"blah",-12345) << '\n';
    cout << '\n';

    return 0;
}
