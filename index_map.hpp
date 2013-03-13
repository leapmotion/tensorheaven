#ifndef INDEX_MAP_HPP_
#define INDEX_MAP_HPP_

#include "core.hpp"
#include "tuple.hpp"
#include "typelist_utility.hpp"





/*
Want to make the map which maps a tuple of unique indices to a tuple of specified indices
e.g. if the input is (i,j,j,i,k) (which is a TypeList_t type), then the output is the map
(i,j,k) -> (i,j,j,i,k) (which is a map from a List_t<i,j,k> to a List_t<i,j,j,i,k>).
*/

template <typename TypeList, typename UniqueTypes_ = typename UniqueTypesIn_t<TypeList>::T>
struct IndexMap_t
{
    typedef UniqueTypes_ UniqueTypes;
    static Uint32 const HEAD_INDEX = UniqueTypes::template IndexOf_t<typename TypeList::HeadType>::V;

    static List_t<TypeList> eval (List_t<UniqueTypes> const &index)
    {
        return List_t<TypeList>(index.template value<HEAD_INDEX>(),
                                IndexMap_t<typename TypeList::BodyTypeList, UniqueTypes>::eval(index));
    }
};

template <typename HeadType, typename UniqueTypes_>
struct IndexMap_t<TypeList_t<HeadType>, UniqueTypes_>
{
    typedef UniqueTypes_ UniqueTypes;
    static Uint32 const HEAD_INDEX = 0;

    static List_t<TypeList_t<HeadType> > eval (List_t<UniqueTypes> const &index)
    {
        return List_t<TypeList_t<HeadType> >(index.template value<HEAD_INDEX>());
    }
};

#endif // INDEX_MAP_HPP_
