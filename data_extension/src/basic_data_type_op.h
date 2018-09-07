//
// Created by wurui on 18-9-7.
//

#ifndef TOOLS_CXX_LINUX_BASIC_DATA_TYPE_OP_H
#define TOOLS_CXX_LINUX_BASIC_DATA_TYPE_OP_H

#include <iostream>
#include <assert.h>
#include <type_traits>
#include <algorithm>
#include <numeric>

namespace wrz {
    template<typename Container>
    auto FindMedian(Container containerCopy) -> typename Container::value_type;

    template<typename Container, typename Func>
    auto FindMedian(Container const &container, Func &&toNumber) -> typename std::result_of<Func(
            typename Container::value_type)>::type;

    template<typename Container>
    auto ComputeMean(Container const &container) -> typename Container::value_type;

    template<typename Container, typename Func>
    auto ComputeMean(Container const &container, Func &&toNumber) -> typename std::result_of<Func(
            typename Container::value_type)>::type;

    template<typename T>
    bool operator==(const std::pair<const T, T> p1, const std::pair<T, T> p2) {
        return p1.first == p2.first && p1.second == p2.second;
    }

    // Container support STL Sequential Container such as vector, forwardlist, deque ...; do not support array and map
    // Item support basic data type, and pair<basic data type>
    template<typename Container, typename Itemtype>
    int ContainCount(const Container &container, const Itemtype &item);

    // Item support user's type/class
    template<typename Container, typename Itemtype>
    int ContainCount(const Container &container, const Itemtype &item,
                   bool isEqual(const Itemtype &item1, const Itemtype &item2));

    template<typename Container, typename Itemtype>
    auto FindFirstInContainer(const Container &container, const Itemtype &item) -> typename Container::size_type;

    template<typename Container, typename Itemtype>
    auto FindFirstInContainer(const Container &container, const Itemtype &item,
                     bool isEqual(const Itemtype &item1, const Itemtype &item2)) -> typename Container::size_type;


    // remove items from container, if items meet the given condition
    template<typename Container, typename ConflictPred, typename Compare>
    void ResolveConflicts(Container &container, ConflictPred &&conflict, Compare &&comp);

}

#include "basic_data_type_op.impl.hpp"

#endif //TOOLS_CXX_LINUX_BASIC_DATA_TYPE_OP_H
