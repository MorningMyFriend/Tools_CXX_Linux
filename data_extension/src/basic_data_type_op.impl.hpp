//
// Created by wurui on 18-9-7.
//

#ifndef TOOLS_CXX_LINUX_BASIC_DATA_TYPE_OP_IMPL_H
#define TOOLS_CXX_LINUX_BASIC_DATA_TYPE_OP_IMPL_H


namespace wrz {
    // find the median of a collection of numbers
    template<typename Container>
    auto FindMedian(Container containerCopy) -> typename Container::value_type {
        assert(!containerCopy.empty());

        std::nth_element(containerCopy.begin(), containerCopy.begin() + containerCopy.size() / 2, containerCopy.end());

        return containerCopy.at(containerCopy.size() / 2);
    };

    // find the median of a collection with a given functor that maps each item to a number
    template<typename Container, typename Func>
    auto FindMedian(Container const &container, Func &&toNumber) -> typename std::result_of<Func(
            typename Container::value_type)>::type {
        assert(!container.empty());

        using Item = typename Container::value_type;
        using Result = typename std::result_of<Func(Item)>::type;

        std::vector<Result> numbers;
        std::transform(container.cbegin(), container.cend(), std::back_inserter(numbers),
                       [&](Item const &item) { return toNumber(item); });
        std::nth_element(numbers.begin(), numbers.begin() + numbers.size() / 2, numbers.end());

        return numbers.at(numbers.size() / 2);
    };

    // compute the mean of a collection of numbers
    template<typename Container>
    auto ComputeMean(Container const &container) -> typename Container::value_type {
        assert(!container.empty());

        using Item = typename Container::value_type;

        Item sum = std::accumulate(container.cbegin(), container.cend(), Item(0));

        return sum / container.size();
    };

    // compute the mean of a collection with a given functor that maps each item to a number
    template<typename Container, typename Func>
    auto ComputeMean(Container const &container, Func &&toNumber) -> typename std::result_of<Func(
            typename Container::value_type)>::type {
        assert(!container.empty());

        using Item = typename Container::value_type;
        using Result = typename std::result_of<Func(Item)>::type;

        Result sum = std::accumulate(container.cbegin(), container.cend(), Result(0),
                                     [&](Result const &result, Item const &item) { return result + toNumber(item); });

        return sum / container.size();
    };

    // Container support STL Sequential Container such as vector, forwardlist, deque ...; do not support array and map
    // Item support basic data type, and pair<basic data type>
    template<typename Container, typename Itemtype>
    int ContainCount(const Container &container, const Itemtype &item) {
//        // if Container's item type not the same as input item, then assert
//        using type = typename std::decay<typename Container::value_type>::type;
//        assert(!(bool)(std::is_same<type, typename std::decay<Itemtype>::type>::value));

        int count = 0;
        for (typename Container::size_type i = 0; i != container.size(); ++i) {
            if (container.at(i) == item) {
                count++;
            }
        }
        return count;
    };

    // Item support user's type/class
    template<typename Container, typename Itemtype, typename Func>
    int ContainCount(const Container &container, const Itemtype &item,
                     Func isEqual) {
        int count = 0;
        for (typename Container::size_type i = 0; i != container.size(); ++i) {
            if (isEqual(container.at(i), item)) {
                count++;
            }
        }
        return count;
    };

    template<typename Container, typename Itemtype>
    auto FindFirstInContainer(const Container &container, const Itemtype &item) -> typename Container::size_type {
//        // if Container's item type not the same as input item, then assert
//        using type = typename std::decay<typename Container::value_type>::type;
//        assert(!(bool)(std::is_same<type, typename std::decay<Itemtype>::type>::value));

        typename Container::size_type index = 0;
        for (typename Container::size_type i = 0; i != container.size(); ++i) {
            if (container.at(i) == item) {
                index = i;
                break;
            }
        }
        return index;
    };

    template<typename Container, typename Itemtype, typename Func>
    auto FindFirstInContainer(const Container &container, const Itemtype &item,
                              Func isEqual) -> typename Container::size_type {
        typename Container::size_type index = 0;
        for (typename Container::size_type i = 0; i != container.size(); ++i) {
            if (isEqual(container.at(i), item)) {
                index = i;
                break;
            }
        }
        return index;
    };


    template<typename Container, typename ConflictPred, typename Compare>
    void ResolveConflicts(Container &container, ConflictPred &&conflict, Compare &&comp) {
        if (container.size() < 2) return;

        for (auto itr1 = container.begin(); itr1 != container.end();) {
            bool erasedItr1 = false;

            for (auto itr2 = container.begin(); itr2 != container.end();) {
                if (itr1 == itr2 || !conflict(*itr1, *itr2)) {
                    ++itr2;
                    continue;
                }

                if (comp(*itr1, *itr2)) {
                    itr1 = container.erase(itr1);
                    erasedItr1 = true;
                    break;
                } else {
                    itr2 = container.erase(itr2);
                    continue;
                }
            }

            if (!erasedItr1) ++itr1;
        }
    }
}

#endif //TOOLS_CXX_LINUX_BASIC_DATA_TYPE_OP_IMPL_H
