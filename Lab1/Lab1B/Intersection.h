#pragma once

#include <vector>
#include <functional>

template<class T, class Equal = std::equal_to<T>>
std::vector<T> intersection(const std::vector<T>& first,
    const std::vector<T>& second, Equal equal = Equal()) {
    std::vector<T> result;
    for (const T& item : first) {
        bool found = false;
        for (const T& other : second) {
            if (equal(item, other)) {
                found = true;
                break;
            }
        }
        bool duplicate = false;
        for (const T& saved : result) {
            if (equal(item, saved)) {
                duplicate = true;
                break;
            }
        }
        if (found && !duplicate) {
            result.push_back(item);
        }
    }
    return result;
}
