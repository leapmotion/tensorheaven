#ifndef TYPELIST_HPP_
#define TYPELIST_HPP_

#include <ostream>
#include <string>

#include "core.hpp"
#include "typestringof.hpp"

// shouldn't ever actually construct one of these
struct NullType
{
private:

    NullType () { }

public:

    static std::string type_as_string () { return "NullType"; }
};

struct EmptyTypeList
{
    // there is no HeadType
    typedef EmptyTypeList BodyTypeList;
    static Uint32 const LENGTH = 0;

    template <typename Type>
    struct Contains_t
    {
        static bool const V = false;
        operator bool () const { return V; }
    };

    template <Uint32 INDEX>
    struct El_t
    {
        typedef NullType T;
    };

    template <Uint32 INDEX>
    struct TrailingTypeList_t
    {
        typedef EmptyTypeList T;
    };

    template <typename Type>
    struct IndexOf_t
    {
        static Uint32 const V = 0; // this is past the end of the indexing
    };

    static std::string type_as_string (bool with_angle_brackets = true) { return with_angle_brackets ? "TypeList_t<>" : ""; }
};

template <typename HeadType_, typename BodyTypeList_ = EmptyTypeList>
struct TypeList_t
{
    typedef HeadType_ HeadType;
    typedef BodyTypeList_ BodyTypeList;
    static Uint32 const LENGTH = 1+BodyTypeList::LENGTH;

    template <typename Type>
    struct Contains_t
    {
        static bool const V = Lvd::Meta::TypesAreEqual<HeadType,Type>::v || BodyTypeList::template Contains_t<Type>::V;
        operator bool () const { return V; }
    };

    template <Uint32 INDEX>
    struct El_t
    {
        enum { _ = Lvd::Meta::Assert<(INDEX < LENGTH)>::v }; // if this assert failed, you tried to access an element past the end of the list

        static Uint32 const I = (INDEX == 0) ? 0 : INDEX-1;
        typedef typename Lvd::Meta::If<(INDEX == 0), HeadType, typename BodyTypeList::template El_t<I>::T >::T T;
    };

    // returns the TypeList_t which starts at the INDEXth element
    template <Uint32 INDEX>
    struct TrailingTypeList_t
    {
        static Uint32 const I = (INDEX == 0) ? 0 : INDEX-1;
        typedef typename Lvd::Meta::If<(INDEX == 0), TypeList_t, typename BodyTypeList::template TrailingTypeList_t<I>::T >::T T;
    };

    template <typename Type>
    struct IndexOf_t
    {
        static Uint32 const V = Lvd::Meta::TypesAreEqual<Type,HeadType>::v ? 0 : (1 + BodyTypeList::template IndexOf_t<Type>::V);
    };

    static std::string type_as_string (bool with_angle_brackets = true)
    {
        return std::string(with_angle_brackets ? "TypeList_t<" : "") +
               TypeStringOf_t<HeadType>::eval() +
               std::string(BodyTypeList::LENGTH > 0 ? ", " : "") +
               BodyTypeList::type_as_string(false) + // no brackets, so it's not cluttered from nesting
               std::string(with_angle_brackets ? ">" : "");
    }
};

#endif // TYPELIST_HPP_
