#include <iostream>

#include "tenh/tensor2.hpp"
#include "tenh/tensor2symmetric.hpp"
#include "tenh/vector.hpp"

using namespace Tenh;
using namespace std;

typedef Vector_t<float,1000> V;
typedef Tensor2_t<V,V> T;
typedef Tensor2Symmetric_t<V,V> S;

float foo (T const &t, V const &v)
{
    TypedIndex_t<V,'i'> i;
    TypedIndex_t<V,'j'> j;
    return v(i)*t(i|j)*v(j);
}

float bar (S const &s, V const &v)
{
    TypedIndex_t<V,'i'> i;
    TypedIndex_t<V,'j'> j;
    return v(i)*s(i|j)*v(j);
}

int main (int argc, char **argv)
{
    V v(argc);
    S s(argc);
    float d = bar(s, v);
    cout << FORMAT_VALUE(d) << '\n';
    return int(d);
}
