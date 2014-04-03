#include "tenh/euclideanembedding.hpp"
#include "tenh/tensor2.hpp"
#include "tenh/tensor2symmetric.hpp"
#include "tenh/vector.hpp"

using namespace Tenh;
using namespace std;

typedef Vector_t<float,1000> V;
typedef typename V::WithStandardEuclideanBasis EV;
// typedef Tensor2_t<V,V> T;
// typedef Tensor2Symmetric_t<V,V> S;

float full_contraction_with_euclidean_embedding (EV const &u, V const &v)
{
    TypedIndex_t<EV,'i'> i;
    TypedIndex_t<V,'j'> j;
    EuclideanEmbedding_t<V> e; // because V uses StandardEuclideanBasis, this should be the identity
    // hopefully the code should simplify to something like a hand-written dot product
    return u(i)*e(i|j)*v(j);
}

float hand_written_inner_product (EV const &u, V const &v)
{
    float retval(0);
    for (Uint32 i = 0; i < 1000; ++i)
        retval += u[typename EV::Index(i, CheckRange::FALSE)] * v[typename V::Index(i, CheckRange::FALSE)];
    return retval;
}