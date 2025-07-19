#pragma once

#include <vector>
#include <map>
#include <memory>
#include <exception>
#include <stdexcept>

#include "robinhood/robin_map.h"
#include "nlohmann/json.hpp"
#include "utils/MOA.hpp"

namespace neuro
{

    using std::map;
    using std::string;
    using std::vector;
    using std::pair;
    using std::tuple;
    using std::unique_ptr;
    using nlohmann::json;

    class Node;
    class Edge;
    class Network;
    struct Spike;

    typedef pair<int,int> Coords;

    class int_hash
    {
        public:
            size_t operator() (const uint32_t val) const
            {

                size_t h = val;
                h ^= h >> 16;
                h *= 0x85ebca6b;
                h ^= h >> 13;
                h *= 0xc2b2ae35;
                h ^= h >> 16;
                return h;
            }
    };

    class coord_hash
    {
        public:

            size_t operator() (const Coords &p) const
            {
                size_t l = int_hash{}(p.first);
                size_t r = int_hash{}(p.second);
                l ^= r + 0x9e3779b9 + (l << 6) + (l >> 2);
                return l;
            }
    };
}   
