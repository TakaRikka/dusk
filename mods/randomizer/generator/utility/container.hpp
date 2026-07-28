#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

namespace randomizer::utility::container
{
    template<typename Container, typename T>
    int GetIndex(const Container& container, const T& element)
    {
        auto it = std::find(container.begin(), container.end(), element);
        if (it == container.end())
        {
            return -1;
        }
        return std::distance(container.begin(), it);
    }

    template<typename Container, typename T>
    bool ElementInContainer(const Container& container, const T& element)
    {
        auto it = std::find(container.begin(), container.end(), element);
        if (it == container.end())
        {
            return false;
        }
        return true;
    }

    template<typename T, typename Predicate>
    std::vector<T> FilterFromVector(std::vector<T>& vector, Predicate pred, bool eraseAfterFilter = false)
    {
        std::vector<T> filteredPool = {};
        std::copy_if(vector.begin(), vector.end(), std::back_inserter(filteredPool), pred);

        if (eraseAfterFilter)
        {
            std::erase_if(vector, pred);
        }

        return filteredPool;
    }

    template<typename T, typename Predicate>
    std::vector<T> FilterAndEraseFromVector(std::vector<T>& vector, Predicate pred)
    {
        return FilterFromVector(vector, pred, true);
    }

    /**
     *  @brief Erases a number of elements a container. If there are no more of the specified element to erase, then nothing
     * happens.
     *
     *  @param container The container to erase elements from
     *  @param element The value of the element to erase from the container
     *  @param numberToErase The number of elements of the specified value to erase (default 1)
     */
    template<typename T, typename Container>
    void Erase(Container& container, T element, int numberToErase = 1)
    {
        for (int i = 0; i < numberToErase; i++)
        {
            auto itr = std::find(container.begin(), container.end(), element);
            if (itr != container.end())
            {
                container.erase(itr);
            }
        }
    }
} // namespace randomizer::utility::container
