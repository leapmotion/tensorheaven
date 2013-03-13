#include <iostream>
#include <string>

#include "typetuple.hpp"

using namespace std;

int main (int argc, char **argv)
{
    cout << TypeStringOf_t<TypeTuple_t<>::T>::eval() << '\n';
    cout << TypeStringOf_t<TypeTuple_t<int>::T>::eval() << '\n';
    cout << TypeStringOf_t<TypeTuple_t<int,bool>::T>::eval() << '\n';
    cout << TypeStringOf_t<TypeTuple_t<int,bool,char>::T>::eval() << '\n';
    cout << TypeStringOf_t<TypeTuple_t<int,bool,char,float>::T>::eval() << '\n';
    cout << TypeStringOf_t<TypeTuple_t<int,bool,char,float,double>::T>::eval() << '\n';
    cout << TypeStringOf_t<TypeTuple_t<int,bool,char,float,double,short int>::T>::eval() << '\n';
    cout << TypeStringOf_t<TypeTuple_t<int,bool,char,float,double,short int,unsigned char>::T>::eval() << '\n';
    cout << TypeStringOf_t<TypeTuple_t<int,bool,char,float,double,short int,unsigned char,string>::T>::eval() << '\n';

    return 0;
}
