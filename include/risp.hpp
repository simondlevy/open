#pragma once

#include <map>
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

            Neuron() { }

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
                fire_times.push_back(time);
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

            Network(
                    double _spike_value_factor, 
                    double _min_potential, 
                    char leak,
                    bool _run_time_inclusive,
                    bool _threshold_inclusive,
                    bool _discrete, 
                    bool _inputs_from_weights, 
                    vector <double> & _weights) 
            {
                EdgeMap::iterator eit;
                leak_mode = leak;

                spike_value_factor = _spike_value_factor;
                min_potential = _min_potential;
                run_time_inclusive = _run_time_inclusive;
                threshold_inclusive = _threshold_inclusive;
                weights = _weights;
                discrete = _discrete;
                inputs_from_weights = _inputs_from_weights;
                overall_run_time = 0;

                add_neuron(0, 3.000000);
                add_neuron(1, 1.000000);
                add_neuron(2, 6.000000);
                add_neuron(3, 0.000000);
                add_neuron(4, 6.000000);
                add_neuron(5, 7.000000);
                add_neuron(6, 0.000000);
                add_neuron(7, 3.000000);
                add_neuron(8, 3.000000);
                add_neuron(9, 0.000000);
                add_neuron(10, 6.000000);
                add_neuron(11, 2.000000);
                add_neuron(12, 0.000000);
                add_neuron(13, 0.000000);
                add_neuron(15, 4.000000);
                add_neuron(16, 7.000000);
                add_neuron(17, 7.000000);
                add_neuron(18, 3.000000);
                add_neuron(20, 7.000000);
                add_neuron(22, 6.000000);
                add_neuron(26, 4.000000);
                add_neuron(30, 3.000000);
                add_neuron(32, 2.000000);
                add_neuron(33, 4.000000);
                add_neuron(34, 6.000000);
                add_neuron(41, 5.000000);
                add_neuron(42, 7.000000);
                add_neuron(51, 3.000000);
                add_neuron(60, 4.000000);
                add_neuron(62, 2.000000);
                add_neuron(64, 2.000000);
                add_neuron(67, 5.000000);
                add_neuron(68, 7.000000);
                add_neuron(77, 3.000000);
                add_neuron(80, 4.000000);
                add_neuron(88, 3.000000);
                add_neuron(93, 3.000000);
                add_neuron(95, 5.000000);
                add_neuron(101, 3.000000);
                add_neuron(102, 5.000000);

                add_input(0, 0);
                add_input(1, 1);
                add_input(2, 2);

                add_output(3, 0);

                add_synpase(11, 102, -7.000000, 9.000000);
                add_synpase(8, 34, 5.000000, 3.000000);
                add_synpase(12, 17, 3.000000, 10.000000);
                add_synpase(1, 11, -6.000000, 12.000000);
                add_synpase(8, 2, -2.000000, 1.000000);
                add_synpase(13, 1, 4.000000, 2.000000);
                add_synpase(8, 9, -2.000000, 14.000000);
                add_synpase(2, 8, 3.000000, 7.000000);
                add_synpase(13, 26, -6.000000, 4.000000);
                add_synpase(64, 95, -1.000000, 6.000000);
                add_synpase(20, 15, 3.000000, 7.000000);
                add_synpase(16, 13, 3.000000, 8.000000);
                add_synpase(80, 34, 0.000000, 9.000000);
                add_synpase(60, 26, 2.000000, 11.000000);
                add_synpase(8, 15, -3.000000, 6.000000);
                add_synpase(6, 9, 2.000000, 3.000000);
                add_synpase(77, 101, 2.000000, 14.000000);
                add_synpase(26, 11, 2.000000, 1.000000);
                add_synpase(9, 20, 6.000000, 9.000000);
                add_synpase(0, 22, -5.000000, 15.000000);
                add_synpase(51, 1, -7.000000, 1.000000);
                add_synpase(2, 77, 6.000000, 7.000000);
                add_synpase(1, 3, 1.000000, 3.000000);
                add_synpase(2, 42, 1.000000, 2.000000);
                add_synpase(3, 16, -7.000000, 13.000000);
                add_synpase(32, 60, -2.000000, 14.000000);
                add_synpase(10, 1, -2.000000, 3.000000);
                add_synpase(6, 10, 0.000000, 3.000000);
                add_synpase(16, 62, 0.000000, 1.000000);
                add_synpase(102, 30, -3.000000, 11.000000);
                add_synpase(60, 10, 6.000000, 9.000000);
                add_synpase(32, 34, 1.000000, 1.000000);
                add_synpase(32, 2, -4.000000, 14.000000);
                add_synpase(30, 32, 0.000000, 9.000000);
                add_synpase(13, 3, 5.000000, 3.000000);
                add_synpase(26, 77, 1.000000, 15.000000);
                add_synpase(64, 13, -2.000000, 12.000000);
                add_synpase(2, 67, 2.000000, 14.000000);
                add_synpase(11, 8, -7.000000, 8.000000);
                add_synpase(5, 95, 4.000000, 9.000000);
                add_synpase(30, 80, 5.000000, 5.000000);
                add_synpase(9, 51, -5.000000, 12.000000);
                add_synpase(51, 8, 1.000000, 15.000000);
                add_synpase(0, 1, -4.000000, 3.000000);
                add_synpase(11, 6, 3.000000, 14.000000);
                add_synpase(101, 26, 2.000000, 2.000000);
                add_synpase(4, 93, 1.000000, 4.000000);
                add_synpase(1, 17, -3.000000, 14.000000);
                add_synpase(9, 4, -1.000000, 7.000000);
                add_synpase(95, 60, 6.000000, 8.000000);
                add_synpase(1, 30, 1.000000, 13.000000);
                add_synpase(17, 102, 4.000000, 1.000000);
                add_synpase(6, 26, 6.000000, 2.000000);
                add_synpase(0, 26, 7.000000, 1.000000);
                add_synpase(4, 0, 2.000000, 15.000000);
                add_synpase(2, 3, 2.000000, 9.000000);
                add_synpase(101, 10, 1.000000, 4.000000);
                add_synpase(51, 11, 0.000000, 14.000000);
                add_synpase(8, 51, 7.000000, 5.000000);
                add_synpase(10, 68, 1.000000, 4.000000);
                add_synpase(4, 33, 1.000000, 6.000000);
                add_synpase(8, 0, 0.000000, 4.000000);
                add_synpase(0, 13, 3.000000, 8.000000);
                add_synpase(68, 12, -2.000000, 13.000000);
                add_synpase(12, 80, -4.000000, 3.000000);
                add_synpase(4, 4, 5.000000, 3.000000);
                add_synpase(5, 93, 7.000000, 12.000000);
                add_synpase(9, 77, -2.000000, 15.000000);
                add_synpase(1, 2, -1.000000, 6.000000);
                add_synpase(6, 8, 3.000000, 12.000000);
                add_synpase(11, 7, -2.000000, 5.000000);
                add_synpase(80, 32, 1.000000, 11.000000);
                add_synpase(9, 26, 5.000000, 4.000000);
                add_synpase(15, 10, 1.000000, 10.000000);
                add_synpase(33, 30, 4.000000, 9.000000);
                add_synpase(6, 88, -6.000000, 15.000000);
                add_synpase(93, 12, 1.000000, 1.000000);
                add_synpase(88, 10, 2.000000, 14.000000);
                add_synpase(18, 41, -3.000000, 8.000000);
                add_synpase(0, 18, 4.000000, 14.000000);
                add_synpase(11, 41, 2.000000, 12.000000);
                add_synpase(6, 51, -1.000000, 10.000000);
                add_synpase(30, 101, -5.000000, 14.000000);
                add_synpase(13, 4, 3.000000, 15.000000);
                add_synpase(60, 17, -4.000000, 8.000000);
                add_synpase(80, 10, 3.000000, 6.000000);
                add_synpase(102, 1, 7.000000, 8.000000);
                add_synpase(12, 8, 0.000000, 12.000000);
                add_synpase(5, 13, 2.000000, 1.000000);
                add_synpase(20, 95, 2.000000, 8.000000);
                add_synpase(42, 5, 4.000000, 6.000000);
                add_synpase(7, 32, 1.000000, 11.000000);
                add_synpase(33, 51, -5.000000, 15.000000);
                add_synpase(67, 102, 3.000000, 10.000000);
                add_synpase(41, 77, 6.000000, 6.000000);
                add_synpase(60, 93, -6.000000, 12.000000);
                add_synpase(77, 95, 3.000000, 8.000000);
                add_synpase(80, 26, -3.000000, 6.000000);
                add_synpase(17, 0, 2.000000, 10.000000);
                add_synpase(60, 0, -5.000000, 9.000000);
                add_synpase(62, 4, 7.000000, 5.000000);
                add_synpase(68, 11, 6.000000, 7.000000);
                add_synpase(9, 2, 0.000000, 4.000000);
                add_synpase(0, 95, -2.000000, 14.000000);
                add_synpase(34, 7, 1.000000, 10.000000);
                add_synpase(64, 3, 4.000000, 14.000000);
                add_synpase(9, 67, -5.000000, 3.000000);
                add_synpase(22, 64, -1.000000, 13.000000);
                add_synpase(10, 41, -7.000000, 15.000000);
                add_synpase(67, 9, 5.000000, 13.000000);
                add_synpase(51, 34, -4.000000, 14.000000);
                add_synpase(2, 6, 3.000000, 14.000000);
                add_synpase(51, 60, 0.000000, 12.000000);
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

            vector <int> output_counts() 
            {
                vector <int> rv;

                rv.push_back(neuron_map[3]->fire_counts);

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

            Neuron* add_neuron(uint32_t node_id, double threshold) 
            {
                Neuron * n = new Neuron(node_id, threshold, false);

                if (!threshold_inclusive) {
                    n->threshold = (discrete) ? (n->threshold+1) : (n->threshold + 0.0000001);
                }

                neuron_map[node_id] = n;

                sorted_neuron_vector.push_back(n);

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

            void init_network() 
            {
                network = new risp::Network(
                        spike_value_factor,
                        min_potential,
                        leak_mode[0], 
                        run_time_inclusive, 
                        threshold_inclusive, 
                        discrete, 
                        inputs_from_weights,
                        weights);

            }

            void run(double duration) 
            {
                network->run(duration);
            }

            void output_counts(int * counts)
            {
                (void)counts;
            }

            vector <int> output_counts() 
            {
                return network->output_counts();
            }

            void clear_activity() 
            {
                network->clear_activity();
            }

        protected:

            void apply_spike(const Spike& s, bool normalized) 
            {
                network->apply_spike(s, normalized);
            }

            double get_input_spike_factor() const;

            risp::Network * network;

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
