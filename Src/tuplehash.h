#ifndef TUPLEHASH_H
#define TUPLEHASH_H
#include <tuple>

struct TupleHash {
    std::size_t operator() (const std::tuple<int, int, int> &tuple) const {
        return std::hash<std::size_t>() ((size_t) std::get<0>(tuple) * 1000000000 + (size_t) std::get<1>(tuple) * 100000 + (size_t) std::get<2>(tuple));
    }
};
#endif
