#pragma once

#include <map>
#include <list>
#include "framework.hpp"

using namespace neuro;
using namespace std;

namespace risp
{
    class Synapse;
    class Neuron;
    class Network;

    class Neuron {

        public:

            Neuron(uint32_t node_id, double t, bool l) 
                : charge(0),
                threshold(t),
                last_check(-1),
                last_fire(-1),
                fire_counts(0),
                leak(l),
                id(node_id),
                check(false) {};

            void perform_fire(int time)
            {
                if (track) fire_times.push_back(time);
                last_fire = time;
                fire_counts++;
                charge = 0;
            }

            vector <Synapse*> synapses;  /**< Outgoing synapses */
            vector <double> fire_times;  /**< Firing times */ 
            double charge;               /**< Charge value */
            double threshold;            /**< Threshold value */
            int last_check;              /**< Last time we process this node */
            int last_fire;               /**< Last firing time */
            uint32_t fire_counts;        /**< Number of fires */
            bool leak;                   /**< Leak on this neuron or not */
            uint32_t id;                 /**< ID for logging events */
            bool check;                  /**< True if we have checked if this neruon fires or not */
            bool track;                  /**< True if fire_times is being tracked */
    };

    class Synapse {

        public:

            Synapse(double w, uint32_t d, Neuron* to_n) : weight(w), to(to_n), delay(d) {};


            double weight;             /**< Weight value */
            Neuron *to;                /**< To neuron on this synapse */
            uint32_t delay;            /**< Delay value */
    };

    class Network {

        public:

            Network(neuro::Network *net, 
                    double _spike_value_factor, 
                    double _min_potential, 
                    char leak,
                    bool _run_time_inclusive,
                    bool _threshold_inclusive,
                    bool _discrete, 
                    bool _inputs_from_weights, 
                    vector <double> & _weights) 
            {
                size_t i;
                neuro::Node *node;
                neuro::Edge *edge;
                EdgeMap::iterator eit;
                Neuron *n;
                leak_mode = leak;
                bool neuron_leak;

                spike_value_factor = _spike_value_factor;
                min_potential = _min_potential;
                run_time_inclusive = _run_time_inclusive;
                threshold_inclusive = _threshold_inclusive;
                weights = _weights;
                discrete = _discrete;
                inputs_from_weights = _inputs_from_weights;
                overall_run_time = 0;

                /* Add neurons */
                net->make_sorted_node_vector();

                for(i = 0; i < net->sorted_node_vector.size(); i++) {
                    node = net->sorted_node_vector[i];

                    if (leak_mode == 'c') {
                        neuron_leak = (node->get("Leak") != 0);
                    } else {
                        neuron_leak = (leak_mode == 'a');
                    }

                    n = add_neuron(node->id, node->get("Threshold"), neuron_leak);
                    if (node->is_input()) add_input(node->id, node->input_id);
                    if (node->is_output()) add_output(node->id, node->output_id);

                    sorted_neuron_vector.push_back(n);
                }

                /* Add synpases */
                for (eit = net->edges_begin(); eit != net->edges_end(); ++eit) {
                    edge = eit->second.get();
                    add_synpase(edge->from->id, edge->to->id, edge->get("Weight"), edge->get("Delay"));
                }
            }


            ~Network() {}

            void apply_spike(const Spike& s, bool normalized) 
            {
                (void)normalized;

                Neuron * n = get_neuron(inputs[s.id]);

                if (s.time >= events.size()) {
                    events.resize(s.time + 1);
                }

                double v = floor(s.value * spike_value_factor);

                events[s.time].push_back(std::make_pair(n,v));
            }

            void run(double duration)
            {
                uint32_t i;
                Neuron *n;
                int run_time;

                /* if clear_activity get called, we don't want to clear tracking info again. */
                if (overall_run_time != 0) clear_tracking_info();

                run_time = (run_time_inclusive) ? duration : duration-1;

                overall_run_time += (run_time+1);

                /* expand the event buffer */
                if ((int) events.size() <= run_time) {
                    events.resize(run_time + 1);
                }

                for (i = 0; i <= (uint32_t) run_time; i++) {
                    process_events(i);
                }

                /* shift extra spiking events to the beginning from the previous run() call*/
                if ((int) events.size() > run_time + 1) {
                    for (i = run_time + 1; i < events.size(); i++) {
                        events[i - run_time - 1] = std::move(events[i]);   // Google tells me this is O(1)

                    }
                }

                /* Deal with leak/non-negative charge  at the end of the run, 
                   so that if you pull neuron charges, they will be correct */

                for (i = 0; i < sorted_neuron_vector.size(); i++) {
                    n = sorted_neuron_vector[i];
                    if (n->leak) n->charge = 0;
                    if (n->charge < min_potential) n->charge = min_potential;
                }
            }

            bool track_neuron_events(uint32_t node_id, bool track) 
            {
                if(!is_neuron(node_id)) return false;
                neuron_map[node_id]->track = track;
                return true;
            }

            vector <int> output_counts() 
            {
                size_t i;
                vector <int> rv;
                for (i = 0; i < outputs.size(); i++) {
                    if (outputs[i] != -1) rv.push_back(neuron_map[outputs[i]]->fire_counts);
                }
                return rv;
            }

            void clear_activity() 
            {
                Neuron *n;
                size_t i;
                for (i = 0; i < sorted_neuron_vector.size(); i++) {
                    n = sorted_neuron_vector[i];
                    n->last_fire = -1;
                    n->fire_counts = 0;
                    n->fire_times.clear();  // JSP should clear regardless of tracking.
                    n->charge = 0;
                    n->last_check = -1;
                }

                events.clear();
                to_fire.clear();
                overall_run_time = 0;
            }

        protected:

            Neuron* add_neuron(uint32_t node_id, double threshold, bool leak) 
            {
                Neuron *n;

                n = new Neuron(node_id, threshold, leak);

                if (!threshold_inclusive) {
                    n->threshold = (discrete) ? (n->threshold+1) : (n->threshold + 0.0000001);
                }

                neuron_map[node_id] = n;
                return n;
            }

            Synapse* add_synpase(uint32_t from_id, uint32_t to_id, double weight, uint32_t delay) 
            {
                Neuron *from, *to;
                Synapse *syn;
                unordered_map <uint32_t, Neuron*>::const_iterator it;

                from = get_neuron(from_id);
                to = get_neuron(to_id);

                syn = new Synapse(weight, delay, to);
                from->synapses.push_back(syn);

                return syn;
            }

            Neuron* get_neuron(uint32_t node_id) 
            {
                unordered_map <uint32_t, Neuron*>::const_iterator it;

                it = neuron_map.find(node_id);

                return it->second;
            }

            bool is_neuron(uint32_t node_id) 
            {
                return neuron_map.find(node_id) != neuron_map.end();
            }

            void add_input(uint32_t node_id, int input_id) 
            {
                if (input_id >= (int) inputs.size()) inputs.resize(input_id + 1, -1);

                inputs[input_id] = node_id;
            }


            void add_output(uint32_t node_id, int output_id) 
            {
                if (output_id >= (int) outputs.size()) outputs.resize(output_id + 1, -1);
                outputs[output_id] = node_id;
            }

            void process_events(uint32_t time) 
            {
                size_t i, j;
                Neuron *n;
                Synapse *syn;
                size_t to_time;
                size_t events_size;
                double weight;

                const vector<std::pair <Neuron*, double>> es = std::move(events[time]);

                for (i = 0; i < to_fire.size(); i++) {
                    to_fire[i]->perform_fire(time);
                }
                to_fire.clear();

                for (i = 0; i < es.size(); i++) {
                    n = es[i].first;
                    if (n->leak) {
                        n->charge = 0;
                    }
                    if (n->charge < min_potential) {
                        n->charge = min_potential;
                    }
                }

                for (i = 0; i < es.size(); i++) {
                    n = es[i].first;
                    n->check = true;
                    n->charge += es[i].second;
                }

                events_size = events.size();

                for (i = 0; i < es.size(); i++) {

                    n = es[i].first;

                    if (n->check == true) {

                        /* fire */
                        if (n->charge >= n->threshold) {

                            for (j = 0; j < n->synapses.size(); j++) {

                                syn = n->synapses[j];

                                to_time = time + syn->delay;

                                if (to_time >= events_size) {
                                    events_size = to_time + 1;
                                    events.resize(events_size);
                                }

                                weight = syn->weight;

                                events[to_time].push_back(make_pair(syn->to, weight));

                            }

                            n->perform_fire(time);
                        }

                        n->check = false;
                    }
                }
            }

            void clear_tracking_info()
            {
                size_t i;
                Neuron *n;

                for (i = 0; i < sorted_neuron_vector.size(); i++) {
                    n = sorted_neuron_vector[i];
                    n->last_fire = -1;
                    n->fire_counts = 0;
                    n->fire_times.clear();  // Doesn't matter if tracking is on or off.
                }
            }

            vector <int> inputs;        /**< index is input id and its value is neuron id. 
                                          If the neuron id is -1, it's not an input node. */
            vector <int> outputs;       /**< index is output id and its value is neuron id. 
                                          If the neuron id is -1, it's not an ouput node. */
            vector <Neuron *> sorted_neuron_vector;         /**< sorted neurons by node id */

            unordered_map <uint32_t, Neuron*> neuron_map;   /**< key is neuron id */

            /** The index of the vector is the timestep.
             *  Each subvector stores a set of events, which is composed of neuron and charge change.
             */
            vector < vector < std::pair<Neuron *, double> >> events;

            int overall_run_time;     /**< This is what get_time() returns. */

            bool run_time_inclusive;  /**< Do we run for duration+1 (true) or duration (false) */

            bool threshold_inclusive; /**< Do we spike on >= or >. */

            double min_potential;     /**< At the end of a timestep, pin the charge to this if less than. */

            bool discrete;            /**< Are networks discrete */

            bool inputs_from_weights; /**< Inputs are indices into the weight vector. */

            char leak_mode;           /**< 'a' for all, 'n' for nothing, 'c' for configurable */

            vector <double> weights;

            double spike_value_factor;

            vector <Neuron *> to_fire;   /* To make RISP like RAVENS, this lets you fire a timestep later. */
    };

    class Processor : public neuro::Processor
    {
        public:

            Processor() 
            {
                string estring;

                /* Default params */

                min_delay = 1;
                leak_mode = "none";
                run_time_inclusive = false;
                threshold_inclusive = true;
                inputs_from_weights = false;

                max_delay = 15;
                min_threshold = 0;
                max_threshold = 7;
                min_potential = -7;
                discrete = true;

                min_weight = -7;
                max_weight = 7;

                spike_value_factor = 7;
            }

            ~Processor() { }

            bool load_network(neuro::Network* net, int network_id) 
            {

                if (networks.find(network_id) != networks.end()) delete networks[network_id];

                risp::Network * risp_net = new risp::Network(net, 
                        spike_value_factor,
                        min_potential,
                        leak_mode[0], 
                        run_time_inclusive, 
                        threshold_inclusive, 
                        discrete, 
                        inputs_from_weights,
                        weights);

                networks[network_id] = risp_net;

                return true;
            }

            void clear(int network_id) 
            {
                risp::Network *risp_net = get_risp_network(network_id);
                networks.erase(network_id);
                delete risp_net;
            }

            void apply_spike(const Spike& s, bool normalize, int network_id) 
            {
                get_risp_network(network_id)->apply_spike(s, normalize);
            }

            void apply_spike(const Spike& s, const vector<int>& network_ids,
                    bool normalize) 
            {
                size_t i;

                for (i = 0; i < network_ids.size(); i++) {
                    apply_spike(s, normalize, network_ids[i]);
                }
            }

            void run(double duration, int network_id) 
            {
                get_risp_network(network_id)->run(duration);
            }

            bool track_neuron_events(uint32_t node_id, bool track, int network_id) 
            {
                return get_risp_network(network_id)->track_neuron_events(node_id, track);
            }

            vector <int> output_counts(int network_id) {
                return get_risp_network(network_id)->output_counts();
            }

            void clear_activity(int network_id) 
            {
                get_risp_network(network_id)->clear_activity();
            }

            Network* get_risp_network(int network_id) 
            {
                map <int, risp::Network*>::const_iterator it;
                it = networks.find(network_id);
                return it->second;
            }

        protected:

            double get_input_spike_factor() const;

            map <int, risp::Network*> networks;

            double min_weight;
            double max_weight;
            double min_threshold;
            double max_threshold;
            double min_potential;
            bool discrete;
            double spike_value_factor;
            string leak_mode;
            bool run_time_inclusive;
            bool threshold_inclusive;
            bool inputs_from_weights;
            vector <double> weights;

            uint32_t min_delay;
            uint32_t max_delay;
    };

}
