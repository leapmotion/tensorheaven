#include <iostream>
#include <string>

#include "core.hpp" // everything should include this
#include "compoundindex.hpp"
#include "expression_templates.hpp"
#include "interop_eigen_selfadjointeigendecomp.hpp"
#include "interop_eigen_svd.hpp"
#include "tensor2.hpp"
#include "tensor2antisymmetric.hpp"
#include "tensor2diagonal.hpp"
#include "tensor2symmetric.hpp"
#include "typelist.hpp"
#include "typelist_utility.hpp"
#include "typetuple.hpp"
#include "vector.hpp"

#include "Eigen/Core"
#include "Eigen/Eigenvalues"
#include "Eigen/SVD"

template <Uint32 ROWS, Uint32 COLS>
void test_SVD ()
{
    std::cout << "test<" << ROWS << ',' << COLS << ">\n";
    std::cout << "*************************************************************************\n";

    typedef Vector_t<float,ROWS> V1;
    typedef Vector_t<float,COLS> V2;
    typedef Tensor2_t<V1,V2> Tensor2;
    typedef Tensor2_t<V1,V1> U;
    typedef Tensor2Diagonal_t<V1,V2> S;
    typedef Tensor2_t<V2,V2> V;
    
    Tensor2 t(Static<>::WITHOUT_INITIALIZATION);
    for (typename Tensor2::Index i; i.is_not_at_end(); ++i)
        t[i] = i.value() + 1;
/*
    
    typedef Eigen::Matrix<float,ROWS,COLS,Eigen::RowMajor> EigenMatrix;
    Eigen::Map<EigenMatrix> map(t.data_pointer());
    
    std::cout << FORMAT_VALUE(t) << '\n';
    std::cout << FORMAT_VALUE(map) << '\n';
    
    Eigen::JacobiSVD<EigenMatrix> svd(map.jacobiSvd(Eigen::ComputeFullU|Eigen::ComputeFullV));
    std::cout << FORMAT_VALUE(svd.matrixU()) << '\n';
    std::cout << FORMAT_VALUE(svd.matrixV()) << '\n';
    std::cout << FORMAT_VALUE(svd.singularValues()) << '\n';
    
    U u(Static<>::WITHOUT_INITIALIZATION);
    S s(Static<>::WITHOUT_INITIALIZATION);
    V v(Static<>::WITHOUT_INITIALIZATION);
    memcpy(u.data_pointer(), &svd.matrixU()(0,0), u.data_size_in_bytes());
    memcpy(s.data_pointer(), &svd.singularValues()(0,0), s.data_size_in_bytes());
    memcpy(v.data_pointer(), &svd.matrixV()(0,0), v.data_size_in_bytes());
  */
  
    U u(Static<>::WITHOUT_INITIALIZATION);
    S s(Static<>::WITHOUT_INITIALIZATION);
    V v(Static<>::WITHOUT_INITIALIZATION);
    SVD_of_Tensor2(t, u, s, v);
    std::cout << FORMAT_VALUE(u) << '\n';
    std::cout << FORMAT_VALUE(s) << '\n';
    std::cout << FORMAT_VALUE(v) << '\n';
    
    Tensor2 x(Static<>::WITHOUT_INITIALIZATION);
    NamedIndex_t<V1,'i'> i;
    NamedIndex_t<V1,'j'> j;
    NamedIndex_t<V1,'k'> k;
    NamedIndex_t<V2,'p'> p;
    NamedIndex_t<V2,'q'> q;
    NamedIndex_t<V2,'r'> r;
    std::cout << FORMAT_VALUE(u(i,j)*u(k,j)) << '\n';
    std::cout << FORMAT_VALUE(u(i,j)*u(i,k)) << '\n';
    x(i,q) = u(i,j)*s(j,p)*v(q,p);
    std::cout << FORMAT_VALUE(x) << '\n';

    std::cout << "after clamping singular values to 1:\n";
    s = S(1);
    x(i,q) = u(i,j)*s(j,p)*v(q,p);
    std::cout << FORMAT_VALUE(s) << '\n';
    std::cout << FORMAT_VALUE(x) << '\n';
    std::cout << FORMAT_VALUE(x(i,p)*x(j,p)) << '\n';
    
    std::cout << '\n';
}

template <Uint32 DIM>
void test_diagonalization ()
{
    std::cout << "test_diagonalization<" << DIM << ">\n";
    std::cout << "*******************************************************\n";

    typedef Vector_t<float,DIM> V;
    typedef Tensor2_t<V,V> T;
    typedef Tensor2Diagonal_t<V,V> D;
    typedef Tensor2Symmetric_t<V> S;
    
    S s(Static<>::WITHOUT_INITIALIZATION);
    for (typename S::Index i; i.is_not_at_end(); ++i)
        s[i] = i.value() + 1;
    
    D eval(Static<>::WITHOUT_INITIALIZATION);
    T evec(Static<>::WITHOUT_INITIALIZATION);
    diagonalize_Tensor2Symmetric(s, eval, evec);
    /*
    // "blow up" s into t (redundant -- Eigen really only needs lower-triangular part)
    T t(Static<>::WITHOUT_INITIALIZATION);
    NamedIndex_t<V,'i'> i;
    NamedIndex_t<V,'j'> j;
    NamedIndex_t<V,'k'> k;
    NamedIndex_t<V,'l'> l;
    t(i,j) = s(i,j);
    std::cout << FORMAT_VALUE(t) << '\n';

    typedef Eigen::Matrix<float,DIM,DIM,Eigen::RowMajor> EigenMatrix;
    Eigen::Map<EigenMatrix> map(t.data_pointer());
    
    Eigen::SelfAdjointEigenSolver<EigenMatrix> solver(map);
    std::cout << FORMAT_VALUE(solver.eigenvectors()) << '\n';
    std::cout << FORMAT_VALUE(solver.eigenvalues()) << '\n';
    
    T evec(Static<>::WITHOUT_INITIALIZATION);
    D eval(Static<>::WITHOUT_INITIALIZATION);
    memcpy(evec.data_pointer(), &solver.eigenvectors(), evec.data_size_in_bytes());
    memcpy(eval.data_pointer(), &solver.eigenvalues(), eval.data_size_in_bytes());
    */
    std::cout << FORMAT_VALUE(evec) << '\n';
    std::cout << FORMAT_VALUE(eval) << '\n';
    
    NamedIndex_t<V,'i'> i;
    NamedIndex_t<V,'j'> j;
    NamedIndex_t<V,'k'> k;
    NamedIndex_t<V,'l'> l;
    std::cout << FORMAT_VALUE(evec(i,j)*eval(j,k)*evec(l,k)) << '\n';
    
    std::cout << '\n';
}

int main (int argc, char **argv)
{
    test_SVD<1,1>();
//     test_SVD<1,3>();
    test_SVD<2,2>();
    test_SVD<3,4>();

    test_diagonalization<1>();
    test_diagonalization<3>();
    test_diagonalization<10>();

    return 0;
}
