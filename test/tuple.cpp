#include <iostream>
#include <string>

#include "tuple.hpp"

using namespace std;

int main (int argc, char **argv)
{
    cout << TypeStringOf_t<List_t<EmptyTypeList> >::eval() << '\n';
    cout << TypeStringOf_t<Tuple_t<> >::eval() << '\n'; // better way
    cout << TypeStringOf_t<Tuple_t<int> >::eval() << '\n';
    cout << TypeStringOf_t<Tuple_t<int,char> >::eval() << '\n';
    cout << TypeStringOf_t<Tuple_t<int,char,float> >::eval() << '\n';
    cout << TypeStringOf_t<Tuple_t<int,char,float,bool> >::eval() << '\n';
    cout << TypeStringOf_t<Tuple_t<int,char,float,bool,double> >::eval() << '\n';
    cout << TypeStringOf_t<Tuple_t<int,char,float,bool,double,unsigned char> >::eval() << '\n';
    cout << TypeStringOf_t<Tuple_t<int,char,float,bool,double,unsigned char,string> >::eval() << '\n';
    cout << TypeStringOf_t<Tuple_t<int,char,float,bool,double,unsigned char,string,short int> >::eval() << '\n';
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
