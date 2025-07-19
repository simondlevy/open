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
}   
