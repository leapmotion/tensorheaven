#include <cstring>
#include <exception>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

#include "tenh/conceptual/abstractindex.hpp"
#include "tenh/conceptual/exteriorpower.hpp"
#include "tenh/conceptual/tensorproduct.hpp"
#include "tenh/conceptual/vectorspace.hpp"
#include "tenh/implementation/tensor.hpp"
#include "tenh/meta/tuple.hpp"

using namespace Tenh;

// ///////////////////////////////////////////////////////////////////////////
// decltype
// ///////////////////////////////////////////////////////////////////////////

struct X { static std::string type_as_string () { return "X"; } };

void prototyping_for_decltype ()
{
    std::cout << "prototyping_for_decltype();\n";

    decltype(vs<3>(RealField(),X())) vs;
    decltype(bvs(vs,basis(X()))) b;
    decltype(b * dual(b)) t;
    decltype(ext<3>(dual(b))) ext3;
    decltype(dual(b) * dual(b)) metric;
    decltype(b * b) cometric;

    std::cout << FORMAT_VALUE(type_string_of(vs)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(b)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(t)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(ext3)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(metric)) << '\n';
    std::cout << FORMAT_VALUE(type_string_of(cometric)) << '\n';
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// auto
// ///////////////////////////////////////////////////////////////////////////

void prototyping_for_auto ()
{
    std::cout << "prototyping_for_auto();\n";

    decltype(bvs<3>(RealField(),X())) b;
    decltype(b * dual(b)) endo;
    decltype(dual(b) * dual(b)) metric;

    typedef double Scalar;
    typedef ImplementationOf_t<decltype(endo),Scalar> T;
    typedef ImplementationOf_t<decltype(metric),Scalar> M;
    T t(uniform_tuple<Scalar>(1, 2, 3) |
        uniform_tuple<Scalar>(4, 5, 6) |
        uniform_tuple<Scalar>(7, 8, 9));
    M m(uniform_tuple<Scalar>(11, 22, 33) |
        uniform_tuple<Scalar>(44, 55, 66) |
        uniform_tuple<Scalar>(77, 88, 99));

    AbstractIndex_c<'i'> i;
    AbstractIndex_c<'j'> j;
    AbstractIndex_c<'k'> k;
    auto ij = i*j;
    auto jk = j*k;
    auto auto_typed_expression = m(ij)*t(jk);
    std::cout << FORMAT_VALUE(m(ij)*t(jk)) << '\n';
    std::cout << FORMAT_VALUE(auto_typed_expression) << '\n';
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// static_assert
// ///////////////////////////////////////////////////////////////////////////

void prototyping_for_static_assert ()
{
    std::cout << "prototyping_for_static_assert();\n";

    static_assert(true, "this should never fail");

    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// delegating constructors
// ///////////////////////////////////////////////////////////////////////////

struct Triple
{
    Triple (int x, int y, int z) : m_x(x), m_y(y), m_z(z) { }
    Triple (int d) : Triple(d, d, d) { } // diagonal element

    int m_x, m_y, m_z;
};

std::ostream &operator << (std::ostream &out, Triple const &t)
{
    return out << '(' << t.m_x << ',' << t.m_y << ',' << t.m_z << ')';
}

void prototyping_for_delegating_constructors ()
{
    std::cout << "prototyping_for_delegating_constructors();\n";

    std::cout << FORMAT_VALUE(Triple(1,2,3)) << '\n';
    std::cout << FORMAT_VALUE(Triple(4)) << '\n';
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// explicit conversion operators
// ///////////////////////////////////////////////////////////////////////////

template <typename Rep_>
struct Scalar_t
{
    Scalar_t (Rep_ const &r) : m(r) { }

    explicit operator Rep_ () const { return m; }

    Rep_ m;
};

template <typename Rep_>
std::ostream &operator << (std::ostream &out, Scalar_t<Rep_> const &s)
{
    return out << "Scalar_t<" << type_string_of<Rep_>() << ">(" << s.m << ')';
}

float one_plus (float x)
{
    return x + 1.0f;
}

void prototyping_for_explicit_conversion_operators ()
{
    std::cout << "prototyping_for_explicit_conversion_operators();\n";

    typedef Scalar_t<float> F;
    std::cout << FORMAT_VALUE(F(3.4f)) << '\n';
    std::cout << FORMAT_VALUE(float(F(3.4f))) << '\n';
    // uncommenting this should cause a compile error to the tune of "no known conversion from 'F' to 'float'"
//     std::cout << FORMAT_VALUE(one_plus(F(3.4f))) << '\n';
    // this does the conversion explicitly and so is valid.
    std::cout << FORMAT_VALUE(one_plus(float(F(3.4f)))) << '\n';
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// deduced return types
// ///////////////////////////////////////////////////////////////////////////

template <typename T_, typename U_>
auto sum (T_ const &t, U_ const &u) -> decltype(t + u)
{
    return t + u;
}

void prototyping_for_deduced_return_types ()
{
    std::cout << "prototyping_for_deduced_return_types();\n";

    std::cout << FORMAT_VALUE(sum(double(3), double(4))) << '\n';
    std::cout << FORMAT_VALUE(sum(double(3), int(4))) << '\n';
    std::cout << FORMAT_VALUE(sum(bool(3), int(4))) << " -- tee hee!\n";
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// nullptr
// ///////////////////////////////////////////////////////////////////////////

void maybe_calculate_stuff (float *x = nullptr)
{
    if (x != nullptr)
        *x = 2*3*5*7*11*13;
}

void prototyping_for_nullptr ()
{
    std::cout << "prototyping_for_nullptr();\n";

    maybe_calculate_stuff();
    maybe_calculate_stuff(nullptr);
    float x;
    maybe_calculate_stuff(&x);
    std::cout << FORMAT_VALUE(x) << '\n';
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// right angle brackets
// ///////////////////////////////////////////////////////////////////////////

void prototyping_for_right_angle_brackets ()
{
    std::cout << "prototyping_for_right_angle_brackets();\n";

    typedef TypeList_t<int,TypeList_t<int>> L;
    std::cout << FORMAT_VALUE(type_string_of<L>()) << '\n';
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// strongly typed enums
// ///////////////////////////////////////////////////////////////////////////

enum class ArrayType : Uint8 { MUTABLE = 0, IMMUTABLE, PROCEDURAL, LAZY };

void print_array_type (ArrayType a)
{
    switch (a)
    {
        case ArrayType::MUTABLE:    std::cout << "MUTABLE";    break;
        case ArrayType::IMMUTABLE:  std::cout << "IMMUTABLE";  break;
        case ArrayType::PROCEDURAL: std::cout << "PROCEDURAL"; break;
        case ArrayType::LAZY:       std::cout << "LAZY";       break;
        default: throw std::domain_error(FORMAT("invalid ArrayType value " << int(a)));
    }
}

void prototyping_for_strongly_typed_enums ()
{
    std::cout << "prototyping_for_strongly_typed_enums();\n";

    // uncommenting this should cause an error to the tune of "no known conversion from 'int' to 'ArrayType'"
//     print_array_type(0);
    print_array_type(ArrayType::MUTABLE); std::cout << '\n';
    print_array_type(ArrayType::IMMUTABLE); std::cout << '\n';
    print_array_type(ArrayType::PROCEDURAL); std::cout << '\n';
    print_array_type(ArrayType::LAZY); std::cout << '\n';
    print_array_type(ArrayType(3)); std::cout << '\n';
    try {
        print_array_type(ArrayType(5)); std::cout << '\n';
    } catch (std::exception const &e) {
        std::cout << "correctly caught exception \"" << e.what() << "\"\n";
    }
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// variadic templates
// ///////////////////////////////////////////////////////////////////////////

template <typename... Body_>
struct TypeStringOfVariadicTemplate_t;

template <>
struct TypeStringOfVariadicTemplate_t<>
{
    static std::string eval () { return ""; }
};

template <typename Head_>
struct TypeStringOfVariadicTemplate_t<Head_>
{
    static std::string eval () { return TypeStringOf_t<Head_,VERBOSE>::eval(); }
};

template <typename Head_, typename... Body_>
struct TypeStringOfVariadicTemplate_t<Head_,Body_...>
{
    static std::string eval () { return TypeStringOf_t<Head_,VERBOSE>::eval() + ',' + TypeStringOfVariadicTemplate_t<Body_...>::eval(); }
};

template <typename... Ts_> struct Typle_t;

typedef Typle_t<> EmptyTyple; // is there any point in this anymore, instead of just using Typle_t<> ?

template <typename Head_, typename... Body_>
struct Typle_t<Head_,Body_...>
{
    typedef Head_ Head;
    typedef Typle_t<Body_...> BodyTyple;
};

namespace Tenh {

template <typename... Ts_>
struct TypeStringOf_t<Typle_t<Ts_...>>
{
    static std::string eval () { return "Typle_t<" + TypeStringOfVariadicTemplate_t<Ts_...>::eval() + '>'; }
};

} // end of namespace Tenh

template <typename Typle_> struct Len_f;

template <> struct Len_f<EmptyTyple> { static Uint32 const V = 0; };
template <typename Head_, typename... Body_> struct Len_f<Typle_t<Head_,Body_...>> { static Uint32 const V = 1 + Len_f<Typle_t<Body_...>>::V; };

template <typename Typle0_, typename Typle1_> struct Concat_f;

template <typename Head0_, typename... Body0_, typename Head1_, typename... Body1_>
struct Concat_f<Typle_t<Head0_,Body0_...>,Typle_t<Head1_,Body1_...>>
{
    typedef Typle_t<Head0_,Body0_...,Head1_,Body1_...> T;
};

template <typename Typle_> struct Tuple_t;

template <>
struct Tuple_t<Typle_t<>>
{
    Tuple_t () { }

    void print_guts (std::ostream &out) const { }
};

template <typename Head_>
struct Tuple_t<Typle_t<Head_>>
{
    Tuple_t (Head_ const &h) : m_head(h) { }

    void print_guts (std::ostream &out) const { out << m_head; }

    Head_ m_head;
};

template <typename Head_, typename... Body_>
struct Tuple_t<Typle_t<Head_,Body_...>>
{
    typedef Tuple_t<Typle_t<Body_...>> BodyTuple;

    Tuple_t (Head_ const &h, BodyTuple const &b) : m_head(h), m_body_tuple(b) { }
    Tuple_t (Head_ const &h, Body_... body) : m_head(h), m_body_tuple(body...) { }

    void print_guts (std::ostream &out) const
    {
        out << m_head << ',';
        m_body_tuple.print_guts(out);
    }

    Head_ m_head;
    BodyTuple m_body_tuple;
};

template <typename... Ts_>
Tuple_t<Typle_t<Ts_...>> tuple (Ts_... ts)
{
    return Tuple_t<Typle_t<Ts_...>>(ts...);
}

template <typename Typle_>
std::ostream &operator << (std::ostream &out, Tuple_t<Typle_> const &t)
{
//     out << "Tuple_t<" << type_string_of<Typle_>() << ">(";
    out << '(';
    t.print_guts(out);
    return out << ')';
}

void prototyping_for_variadic_templates ()
{
    std::cout << "prototyping_for_variadic_templates();\n";

    std::cout << FORMAT_VALUE(type_string_of<Typle_t<>>()) << '\n';
    std::cout << FORMAT_VALUE(type_string_of<EmptyTyple>()) << '\n';
    std::cout << FORMAT_VALUE(type_string_of<Typle_t<int>>()) << '\n';
    std::cout << FORMAT_VALUE((type_string_of<Typle_t<int,float>>())) << '\n';
    std::cout << FORMAT_VALUE((type_string_of<Typle_t<int,float,char>>())) << '\n';
    std::cout << FORMAT_VALUE((type_string_of<Typle_t<int,float,char,double>>())) << '\n';
    std::cout << FORMAT_VALUE((type_string_of<Typle_t<int,float,char,double,bool>>())) << '\n';
    typedef Typle_t<int,float,char> T0;
    typedef Typle_t<bool,double> T1;
    std::cout << FORMAT_VALUE(type_string_of<T0>()) << '\n';
    std::cout << FORMAT_VALUE(type_string_of<T1>()) << '\n';
    std::cout << FORMAT_VALUE((type_string_of<Concat_f<T0,T1>::T>())) << '\n';

    typedef Tuple_t<Typle_t<int,char,double>> Triple;
    std::cout << FORMAT_VALUE(Triple(3,'a',4.0)) << '\n';
    std::cout << FORMAT_VALUE(::tuple(3,'a',4.0)) << '\n';
    std::cout << FORMAT_VALUE(::tuple(3,'a',4.0,true,false,"hee hee")) << '\n';

    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// initializer lists -- NOT SUPPORTED BY CLANG 3.0
// ///////////////////////////////////////////////////////////////////////////

// void prototyping_for_initializer_lists ()
// {
//     std::cout << "prototyping_for_initializer_lists();\n";
//
//     typedef std::vector<int> IntVector;
//     IntVector v{1,2,4,8,16,32};
//     std::cout << FORMAT_VALUE(v.size()) << '\n';
//     std::cout << '{';
//     for (auto it = v.begin(), it_end = v.end(); it != it_end; ++it)
//         std::cout << ' ' << *it;
//     std::cout << " }\n";
//     std::cout << '\n';
// }

// ///////////////////////////////////////////////////////////////////////////
// range-based for loops
// ///////////////////////////////////////////////////////////////////////////

void prototyping_for_range_based_for_loops ()
{
    std::cout << "prototyping_for_range_based_for_loops();\n";

    typedef std::set<int> IntSet;

    IntSet s;
    s.insert(100);
    s.insert(2);
    s.insert(4);
    s.insert(-1);
    s.insert(0);
    s.insert(80);
    std::cout << '{';
    for (auto i : s)
        std::cout << ' ' << i;
    std::cout << " }\n";
    std::cout << '\n';
    std::cout << '{';
    int a[3] = {1, 2, 4};
    for (auto i : a)
        std::cout << ' ' << i;
    std::cout << " }\n";
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// r-value references and move semantics
// ///////////////////////////////////////////////////////////////////////////

template <typename T_>
struct DynamicArray_t
{
    // default constructor
    DynamicArray_t ()
        :
        m_len(0),
        m_ptr(nullptr)
    {
        std::cout << "DynamicArray_t default constructor (this = " << this << ")\n";
    }
    // constructor
    DynamicArray_t (Uint32 length)
        :
        m_len(length),
        m_ptr(new T_[m_len])
    {
        std::cout << "DynamicArray_t length constructor  (this = " << this << ")\n";
        for (T_ *p = m_ptr, *p_end = m_ptr+m_len; p != p_end; ++p)
            *p = 0;
    }
    // copy constructor
    DynamicArray_t (DynamicArray_t const &h)
        :
        m_len(h.m_len),
        m_ptr(h.m_ptr != nullptr ? new T_[m_len] : nullptr)
    {
        std::cout << "DynamicArray_t copy constructor    (this = " << this << ")\n";
        if (h.m_ptr != nullptr)
        {
            for (T_ *p_src = h.m_ptr, *p_src_end = h.m_ptr+h.m_len,
                    *p_dest = m_ptr, *p_dest_end = m_ptr+m_len;
                 p_src != p_src_end;
                 ++p_src, ++p_dest)
            {
                *p_dest = *p_src;
            }
        }
    }
    // move constructor
    DynamicArray_t (DynamicArray_t &&h)
        :
        m_len(h.m_len),
        m_ptr(h.m_ptr)
    {
        std::cout << "DynamicArray_t move constructor    (this = " << this << ")\n";
        h.m_ptr = nullptr;
    }
    // destructor
    ~DynamicArray_t ()
    {
        std::cout << "DynamicArray_t destructor          (this = " << this << ")\n";
        delete[] m_ptr;
    }

    // standard assignment operator
    void operator = (DynamicArray_t const &h)
    {
        std::cout << "DynamicArray_t assignment op       (this = " << this << ")\n";
        delete[] m_ptr;
        m_len = h.m_len;
        if (h.m_ptr != nullptr)
        {
            m_ptr = new T_[m_len];
            for (T_ *p_src = h.m_ptr, *p_src_end = h.m_ptr+h.m_len,
                    *p_dest = m_ptr, *p_dest_end = m_ptr+m_len;
                 p_src != p_src_end;
                 ++p_src, ++p_dest)
            {
                *p_dest = *p_src;
            }
        }
        else
        {
            m_ptr = nullptr;
        }
    }
    // move assignment operator
    void operator = (DynamicArray_t &&h)
    {
        std::cout << "DynamicArray_t move assignment op  (this = " << this << ")\n";
        delete[] m_ptr;
        m_len = h.m_len;
        m_ptr = h.m_ptr;
        h.m_ptr = nullptr;
    }

    Uint32 length () const { return m_len; }

    T_ const &operator [] (Uint32 i) const
    {
        if (i >= m_len)
            throw std::domain_error("index " + FORMAT(i) + " out of range [0," + FORMAT(m_len) + ')');
        return m_ptr[i];
    }
    T_ &operator [] (Uint32 i)
    {
        if (i >= m_len)
            throw std::domain_error("index " + FORMAT(i) + " out of range [0," + FORMAT(m_len) + ')');
        return m_ptr[i];
    }

private:

    Uint32 m_len;
    int *m_ptr;
};

template <typename T_>
std::ostream &operator << (std::ostream &out, DynamicArray_t<T_> const &d)
{
    out << "DynamicArray_t<" << type_string_of<T_>() << ">(this = " << &d << "){";
    for (T_ const *p = &d[0], *p_end = &d[0]+d.length(); p != p_end; ++p)
        out << ' ' << *p;
    return out << " }";
}

template <typename T_>
DynamicArray_t<T_> natural_numbers (Uint32 length)
{
    typedef DynamicArray_t<T_> D;
    D retval(length);
    for (Uint32 i = 0; i < length; ++i)
        retval[i] = i;
    return retval;
}

void prototyping_for_rvalue_references ()
{
    std::cout << "prototyping_for_rvalue_references();\n";
    typedef DynamicArray_t<int> D;
    {
        D d;
    }
    std::cout << '\n';
    {
        D d(3);
        std::cout << d << '\n';
    }
    std::cout << '\n';
    {
        std::cout << natural_numbers<int>(20) << '\n';
    }
    std::cout << '\n';
    {
        D d;
        d = natural_numbers<int>(30);
        std::cout << d << '\n';
    }
    std::cout << '\n';
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// std::enable_if
// ///////////////////////////////////////////////////////////////////////////

template <Uint32 N_>
typename std::enable_if<(N_%2==0)>::type only_for_even ()
{
    std::cout << "only_for_even<" << N_ << ">();\n";
}

template <Uint32 N_>
typename std::enable_if<(N_%2==1)>::type only_for_odd ()
{
    std::cout << "only_for_odd<" << N_ << ">();\n";
}

void prototyping_for_enable_if ()
{
    std::cout << "prototyping_for_enable_if();\n";

    only_for_even<0>();
    only_for_even<2>();
    only_for_even<4>();
    only_for_odd<1>();
    only_for_odd<3>();
    only_for_odd<5>();

    // this should cause a compile error
//     only_for_even<1>();
    // this should cause a compile error
//     only_for_odd<0>();

    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// random number generation
// ///////////////////////////////////////////////////////////////////////////

void prototyping_for_random_number_generation ()
{
    std::cout << "prototyping_for_random_number_generation();\n";

    std::default_random_engine generator;
    std::uniform_int_distribution<int> dist(3,20);
    for (Uint32 i = 0; i < 30; ++i)
        std::cout << dist(generator) << ' ';
    std::cout << '\n';
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// better use of std::complex
// ///////////////////////////////////////////////////////////////////////////

void prototyping_for_better_use_of_complex ()
{
    std::cout << "prototyping_for_better_use_of_complex();\n";

    typedef std::complex<double> C;
    C z(1, 2);
    std::cout << FORMAT_VALUE(z) << '\n';
    z.real(8);
    z.imag(10);
    std::cout << FORMAT_VALUE(z) << '\n';
    std::cout << '\n';
}

// ///////////////////////////////////////////////////////////////////////////
// main function
// ///////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv)
{
    prototyping_for_decltype();
    prototyping_for_auto();
    prototyping_for_static_assert();
    prototyping_for_delegating_constructors();
    prototyping_for_explicit_conversion_operators();
    prototyping_for_deduced_return_types();
    prototyping_for_nullptr();
    prototyping_for_right_angle_brackets();
    prototyping_for_strongly_typed_enums();
    prototyping_for_variadic_templates();
//     prototyping_for_initializer_lists(); // not supported by clang 3.0
    prototyping_for_range_based_for_loops();
    prototyping_for_rvalue_references();
    prototyping_for_enable_if();
    prototyping_for_random_number_generation();
    prototyping_for_better_use_of_complex();

    return 0;
}
