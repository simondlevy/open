#pragma once

#include <map>
#include <list>
#include "framework.hpp"
#include "nlohmann/json.hpp"
#include "utils/MOA.hpp"

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
                    bool _fire_like_ravens,
                    bool _discrete, 
                    bool _inputs_from_weights, 
                    uint32_t _noisy_seed,
                    double _noisy_stddev,
                    vector <double> & _weights, 
                    vector < double> & _stds) 
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
                fire_like_ravens = _fire_like_ravens;
                noisy_seed = _noisy_seed;
                noisy_stddev = _noisy_stddev;
                weights = _weights;
                stds = _stds;
                discrete = _discrete;
                inputs_from_weights = _inputs_from_weights;
                overall_run_time = 0;
                neuron_fire_counter = 0;
                neuron_accum_counter = 0;
                rng.Seed(noisy_seed, "noisy_risp");

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

            void run(double duration) {
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


            double get_time() 
            {
                return (double) overall_run_time;
            }

            bool track_output_events(int output_id, bool track) 
            {
                if (!is_valid_output_id(output_id)) return false;
                neuron_map[outputs[output_id]]->track = track;
                return true;
            }

            bool track_neuron_events(uint32_t node_id, bool track) 
            {
                if(!is_neuron(node_id)) return false;
                neuron_map[node_id]->track = track;
                return true;
            }


            double output_last_fire(int output_id) 
            {
                return neuron_map[outputs[output_id]]->last_fire;
            }

            vector <double> output_last_fires() {
                size_t i;
                vector <double> rv;
                for (i = 0; i < outputs.size(); i++) {
                    if(outputs[i] != -1) rv.push_back(neuron_map[outputs[i]]->last_fire);
                }
                return rv;
            }

            int output_count(int output_id);

            vector <int> output_counts();

            vector <double> output_vector(int output_id);

            vector < vector <double> > output_vectors();

            long long total_neuron_counts();

            long long total_neuron_accumulates();

            vector <int> neuron_counts();

            vector <double> neuron_last_fires();

            vector < vector <double> > neuron_vectors();

            vector < double > neuron_charges();

            void synapse_weights(vector <uint32_t> &pres, vector <uint32_t> &posts, vector <double> &vals);

            void clear_activity();


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

            bool is_valid_input_id(int input_id) 
            {
                return !(input_id < 0 || input_id >= (int) inputs.size() || inputs[input_id] == -1);
            }

            bool is_valid_output_id(int output_id) 
            {
                return !(output_id < 0 || output_id >= (int) outputs.size() || outputs[output_id] == -1);
            }

            void add_input(uint32_t node_id, int input_id);

            void add_output(uint32_t node_id, int output_id);

            void clear_tracking_info();   

            void process_events(uint32_t time); /**< Process events at time "time" */

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

            long long neuron_fire_counter;  /**< This is what total_neuron_counts() returns. */
            long long neuron_accum_counter; /**< This is what total_neuron_accumulates() returns. */
            int overall_run_time;     /**< This is what get_time() returns. */
            bool run_time_inclusive;  /**< Do we run for duration+1 (true) or duration (false) */
            bool threshold_inclusive; /**< Do we spike on >= or >. */
            double min_potential;     /**< At the end of a timestep, pin the charge to this if less than. */
            bool fire_like_ravens;    /**< Register neuron fires one step later, so RISP can match RAVENS. */
            bool discrete;            /**< Are networks discrete */
            bool inputs_from_weights; /**< Inputs are indices into the weight vector. */
            char leak_mode;           /**< 'a' for all, 'n' for nothing, 'c' for configurable */
            vector <double> weights;
            double noisy_stddev;
            uint32_t noisy_seed;
            MOA rng;
            vector <double> stds;

            double spike_value_factor;
            vector <Neuron *> to_fire;   /* To make RISP like RAVENS, this lets you fire a timestep later. */

    };

    class Processor : public neuro::Processor
    {
        public:
            Processor();
            ~Processor();

            bool load_network(neuro::Network* n, int network_id = 0);
            bool load_networks(std::vector<neuro::Network*> &n);
            void clear(int network_id = 0);

            /* Queue spike(s) as input to a network or to multiple networks */

            void apply_spike(const Spike& s, bool normalized = true, int network_id = 0);
            void apply_spike(const Spike& s, const vector<int>& network_ids, bool normalized = true);

            void apply_spikes(const vector<Spike>& s, bool normalized = true, int network_id = 0);
            void apply_spikes(const vector<Spike>& s, const vector<int>& network_ids, bool normalized = true);

            /* Run the network(s) for the desired time with queued input(s) */

            void run(double duration, int network_id = 0);
            void run(double duration, const vector<int>& network_ids);

            /* Get processor time based on specified network */
            double get_time(int network_id = 0);

            /* Output tracking.  See the markdown for a detailed description of these.  */

            bool track_output_events(int output_id, bool track = true, int network_id = 0);
            bool track_neuron_events(uint32_t node_id, bool track = true, int network_id = 0);

            /* Access output spike data */

            double output_last_fire(int output_id, int network_id = 0);
            vector <double> output_last_fires(int network_id = 0);

            int output_count(int output_id, int network_id = 0);
            vector <int> output_counts(int network_id = 0);

            vector <double> output_vector(int output_id, int network_id = 0);
            vector < vector <double> > output_vectors(int network_id = 0);

            /* Spike data from all neurons. */

            long long total_neuron_counts(int network_id = 0);
            long long total_neuron_accumulates(int network_id = 0);
            vector <int> neuron_counts(int network_id = 0);
            vector <double> neuron_last_fires(int network_id = 0);
            vector < vector <double> > neuron_vectors(int network_id = 0);

            vector < double > neuron_charges(int network_id = 0);

            void synapse_weights(vector <uint32_t> &pres,
                    vector <uint32_t> &posts,
                    vector <double> &vals,
                    int network_id = 0);

            /* Remove state, keep network loaded */
            void clear_activity(int network_id = 0);

            /* Network and Processor Properties.  The network properties correspond to the Data
               field in the network, nodes and edges.  The processor properties are so that
               applications may query the processor for various properties (e.g. input scaling,
               fire-on-threshold vs fire-over-threshold. */

            json get_params() const;
            string get_name() const;

        protected:

            risp::Network* get_risp_network(int network_id);
            double get_input_spike_factor() const;
            map <int, risp::Network*> networks;

            double min_weight;
            double max_weight;
            double min_threshold;
            double max_threshold;
            double min_potential;
            double noisy_stddev;
            bool discrete;
            double spike_value_factor;
            string leak_mode;
            bool run_time_inclusive;
            bool threshold_inclusive;
            bool fire_like_ravens;
            bool inputs_from_weights;
            uint32_t noisy_seed;
            vector <double> weights;
            vector <double> stds;

            uint32_t min_delay;
            uint32_t max_delay;

            json saved_params;

    };

}
