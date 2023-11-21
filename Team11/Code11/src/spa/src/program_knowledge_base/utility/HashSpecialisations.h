#pragma once

#include <memory>
#include <vector>
#include <string>

// Custom hash functions and comparators
namespace std {
template<>
struct hash<std::vector<std::string> > {
    std::size_t operator()(const std::vector<std::string>& strings) const {
        std::size_t seed{ strings.size() };
        for (const std::string& string : strings) {
            seed ^= std::hash<std::string>()(string) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
} // namespace std
