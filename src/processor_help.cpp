/* Helper routines to facilitate apps and help processor writers keep it simple. */

#include "framework.hpp"
#include <cstdio>
#include "utils/json_helpers.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

typedef std::runtime_error SRE;

namespace neuro
{
    using std::string;
    using std::vector;
    using std::make_pair;
    using nlohmann::json;


    bool track_all_neuron_events(Processor *p, Network *n)
    {
        NodeMap::iterator nit;

        for (nit = n->begin(); nit != n->end(); nit++) {
            p->track_neuron_events(nit->second->id);
        }

        return true;
    }

}
