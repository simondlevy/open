#pragma once

#include <cmath>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace std;

using std::map;
using std::vector;

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
                last_fire = time;
                fire_counts++;
                charge = 0;
            }

            vector <Synapse*> synapses; 
            double charge;             
            double threshold;         
            int last_check;          
            int last_fire;          
            uint32_t fire_counts;  
            bool leak;            
            uint32_t id;         
            bool check;         
    };

    class Synapse {

        public:

            Synapse(double w, uint32_t d, Neuron* to_n) : weight(w), to(to_n), delay(d) {};

            double weight;     
            Neuron *to;       
            uint32_t delay;  
    };

    class Network {

        public:

            Network() 
            {
                spike_value_factor = 7;
                min_potential = -7;
                run_time_inclusive = false;
                threshold_inclusive = true;
                discrete = true;
                inputs_from_weights = false;
                overall_run_time = 0;

                neuron_0 = add_neuron(0, 3.000000);
                neuron_1 = add_neuron(1, 1.000000);
                neuron_2 = add_neuron(2, 6.000000);
                neuron_3 = add_neuron(3, 0.000000);
                neuron_4 = add_neuron(4, 6.000000);
                neuron_5 = add_neuron(5, 7.000000);
                neuron_6 = add_neuron(6, 0.000000);
                neuron_7 = add_neuron(7, 3.000000);
                neuron_8 = add_neuron(8, 3.000000);
                neuron_9 = add_neuron(9, 0.000000);
                neuron_10 = add_neuron(10, 6.000000);
                neuron_11 = add_neuron(11, 2.000000);
                neuron_12 = add_neuron(12, 0.000000);
                neuron_13 = add_neuron(13, 0.000000);
                neuron_15 = add_neuron(15, 4.000000);
                neuron_16 = add_neuron(16, 7.000000);
                neuron_17 = add_neuron(17, 7.000000);
                neuron_18 = add_neuron(18, 3.000000);
                neuron_20 = add_neuron(20, 7.000000);
                neuron_22 = add_neuron(22, 6.000000);
                neuron_26 = add_neuron(26, 4.000000);
                neuron_30 = add_neuron(30, 3.000000);
                neuron_32 = add_neuron(32, 2.000000);
                neuron_33 = add_neuron(33, 4.000000);
                neuron_34 = add_neuron(34, 6.000000);
                neuron_41 = add_neuron(41, 5.000000);
                neuron_42 = add_neuron(42, 7.000000);
                neuron_51 = add_neuron(51, 3.000000);
                neuron_60 = add_neuron(60, 4.000000);
                neuron_62 = add_neuron(62, 2.000000);
                neuron_64 = add_neuron(64, 2.000000);
                neuron_67 = add_neuron(67, 5.000000);
                neuron_68 = add_neuron(68, 7.000000);
                neuron_77 = add_neuron(77, 3.000000);
                neuron_80 = add_neuron(80, 4.000000);
                neuron_88 = add_neuron(88, 3.000000);
                neuron_93 = add_neuron(93, 3.000000);
                neuron_95 = add_neuron(95, 5.000000);
                neuron_101 = add_neuron(101, 3.000000);
                neuron_102 = add_neuron(102, 5.000000);

                add_synapse(neuron_11, neuron_102, -7.000000, 9.000000);
                add_synapse(neuron_8, neuron_34, 5.000000, 3.000000);
                add_synapse(neuron_12, neuron_17, 3.000000, 10.000000);
                add_synapse(neuron_1, neuron_11, -6.000000, 12.000000);
                add_synapse(neuron_8, neuron_2, -2.000000, 1.000000);
                add_synapse(neuron_13, neuron_1, 4.000000, 2.000000);
                add_synapse(neuron_8, neuron_9, -2.000000, 14.000000);
                add_synapse(neuron_2, neuron_8, 3.000000, 7.000000);
                add_synapse(neuron_13, neuron_26, -6.000000, 4.000000);
                add_synapse(neuron_64, neuron_95, -1.000000, 6.000000);
                add_synapse(neuron_20, neuron_15, 3.000000, 7.000000);
                add_synapse(neuron_16, neuron_13, 3.000000, 8.000000);
                add_synapse(neuron_80, neuron_34, 0.000000, 9.000000);
                add_synapse(neuron_60, neuron_26, 2.000000, 11.000000);
                add_synapse(8, 15, -3.000000, 6.000000);
                add_synapse(6, 9, 2.000000, 3.000000);
                add_synapse(77, 101, 2.000000, 14.000000);
                add_synapse(26, 11, 2.000000, 1.000000);
                add_synapse(9, 20, 6.000000, 9.000000);
                add_synapse(0, 22, -5.000000, 15.000000);
                add_synapse(51, 1, -7.000000, 1.000000);
                add_synapse(2, 77, 6.000000, 7.000000);
                add_synapse(1, 3, 1.000000, 3.000000);
                add_synapse(2, 42, 1.000000, 2.000000);
                add_synapse(3, 16, -7.000000, 13.000000);
                add_synapse(32, 60, -2.000000, 14.000000);
                add_synapse(10, 1, -2.000000, 3.000000);
                add_synapse(6, 10, 0.000000, 3.000000);
                add_synapse(16, 62, 0.000000, 1.000000);
                add_synapse(102, 30, -3.000000, 11.000000);
                add_synapse(60, 10, 6.000000, 9.000000);
                add_synapse(32, 34, 1.000000, 1.000000);
                add_synapse(32, 2, -4.000000, 14.000000);
                add_synapse(30, 32, 0.000000, 9.000000);
                add_synapse(13, 3, 5.000000, 3.000000);
                add_synapse(26, 77, 1.000000, 15.000000);
                add_synapse(64, 13, -2.000000, 12.000000);
                add_synapse(2, 67, 2.000000, 14.000000);
                add_synapse(11, 8, -7.000000, 8.000000);
                add_synapse(5, 95, 4.000000, 9.000000);
                add_synapse(30, 80, 5.000000, 5.000000);
                add_synapse(9, 51, -5.000000, 12.000000);
                add_synapse(51, 8, 1.000000, 15.000000);
                add_synapse(0, 1, -4.000000, 3.000000);
                add_synapse(11, 6, 3.000000, 14.000000);
                add_synapse(101, 26, 2.000000, 2.000000);
                add_synapse(4, 93, 1.000000, 4.000000);
                add_synapse(1, 17, -3.000000, 14.000000);
                add_synapse(9, 4, -1.000000, 7.000000);
                add_synapse(95, 60, 6.000000, 8.000000);
                add_synapse(1, 30, 1.000000, 13.000000);
                add_synapse(17, 102, 4.000000, 1.000000);
                add_synapse(6, 26, 6.000000, 2.000000);
                add_synapse(0, 26, 7.000000, 1.000000);
                add_synapse(4, 0, 2.000000, 15.000000);
                add_synapse(2, 3, 2.000000, 9.000000);
                add_synapse(101, 10, 1.000000, 4.000000);
                add_synapse(51, 11, 0.000000, 14.000000);
                add_synapse(8, 51, 7.000000, 5.000000);
                add_synapse(10, 68, 1.000000, 4.000000);
                add_synapse(4, 33, 1.000000, 6.000000);
                add_synapse(8, 0, 0.000000, 4.000000);
                add_synapse(0, 13, 3.000000, 8.000000);
                add_synapse(68, 12, -2.000000, 13.000000);
                add_synapse(12, 80, -4.000000, 3.000000);
                add_synapse(4, 4, 5.000000, 3.000000);
                add_synapse(5, 93, 7.000000, 12.000000);
                add_synapse(9, 77, -2.000000, 15.000000);
                add_synapse(1, 2, -1.000000, 6.000000);
                add_synapse(6, 8, 3.000000, 12.000000);
                add_synapse(11, 7, -2.000000, 5.000000);
                add_synapse(80, 32, 1.000000, 11.000000);
                add_synapse(9, 26, 5.000000, 4.000000);
                add_synapse(15, 10, 1.000000, 10.000000);
                add_synapse(33, 30, 4.000000, 9.000000);
                add_synapse(6, 88, -6.000000, 15.000000);
                add_synapse(93, 12, 1.000000, 1.000000);
                add_synapse(88, 10, 2.000000, 14.000000);
                add_synapse(18, 41, -3.000000, 8.000000);
                add_synapse(0, 18, 4.000000, 14.000000);
                add_synapse(11, 41, 2.000000, 12.000000);
                add_synapse(6, 51, -1.000000, 10.000000);
                add_synapse(30, 101, -5.000000, 14.000000);
                add_synapse(13, 4, 3.000000, 15.000000);
                add_synapse(60, 17, -4.000000, 8.000000);
                add_synapse(80, 10, 3.000000, 6.000000);
                add_synapse(102, 1, 7.000000, 8.000000);
                add_synapse(12, 8, 0.000000, 12.000000);
                add_synapse(5, 13, 2.000000, 1.000000);
                add_synapse(20, 95, 2.000000, 8.000000);
                add_synapse(42, 5, 4.000000, 6.000000);
                add_synapse(7, 32, 1.000000, 11.000000);
                add_synapse(33, 51, -5.000000, 15.000000);
                add_synapse(67, 102, 3.000000, 10.000000);
                add_synapse(41, 77, 6.000000, 6.000000);
                add_synapse(60, 93, -6.000000, 12.000000);
                add_synapse(77, 95, 3.000000, 8.000000);
                add_synapse(80, 26, -3.000000, 6.000000);
                add_synapse(17, 0, 2.000000, 10.000000);
                add_synapse(60, 0, -5.000000, 9.000000);
                add_synapse(62, 4, 7.000000, 5.000000);
                add_synapse(68, 11, 6.000000, 7.000000);
                add_synapse(9, 2, 0.000000, 4.000000);
                add_synapse(0, 95, -2.000000, 14.000000);
                add_synapse(34, 7, 1.000000, 10.000000);
                add_synapse(64, 3, 4.000000, 14.000000);
                add_synapse(9, 67, -5.000000, 3.000000);
                add_synapse(22, 64, -1.000000, 13.000000);
                add_synapse(10, 41, -7.000000, 15.000000);
                add_synapse(67, 9, 5.000000, 13.000000);
                add_synapse(51, 34, -4.000000, 14.000000);
                add_synapse(2, 6, 3.000000, 14.000000);
                add_synapse(51, 60, 0.000000, 12.000000);
            }

            ~Network() {}

            void apply_spike_input0(const double time)
            {
                apply_spike_input(neuron_0, time);
            }

            void apply_spike_input1(const double time)
            {
                apply_spike_input(neuron_1, time);
            }

            void apply_spike_input2(const double time)
            {
                apply_spike_input(neuron_2, time);
            }

            void run(double duration)
            {
                if (overall_run_time != 0) {
                    clear_tracking_info();
                }

                const size_t run_time = (run_time_inclusive) ? duration : duration-1;

                overall_run_time += (run_time+1);

                if (events.size() <= run_time) {
                    events.resize(run_time + 1);
                }

                for (size_t i = 0; i <= run_time; i++) {
                    process_events(i);
                }

                if (events.size() > run_time + 1) {
                    for (size_t i = run_time + 1; i < events.size(); i++) {
                        events[i - run_time - 1] = std::move(events[i]);

                    }
                }

                for (size_t i = 0; i < neurons.size(); i++) {
                    Neuron * n = neurons[i];
                    if (n->leak) n->charge = 0;
                    if (n->charge < min_potential) n->charge = min_potential;
                }
            }

            int n3_count()
            {
                return neuron_map[3]->fire_counts;
            }

            void clear_activity() 
            {
                for (size_t i = 0; i < neurons.size(); i++) {
                    Neuron * n = neurons[i];
                    n->last_fire = -1;
                    n->fire_counts = 0;
                    n->charge = 0;
                    n->last_check = -1;
                }

                events.clear();
                overall_run_time = 0;
            }

        private:

            Neuron * neuron_0;
            Neuron * neuron_1;
            Neuron * neuron_2;
            Neuron * neuron_3;
            Neuron * neuron_4;
            Neuron * neuron_5;
            Neuron * neuron_6;
            Neuron * neuron_7;
            Neuron * neuron_8;
            Neuron * neuron_9;
            Neuron * neuron_10;
            Neuron * neuron_11;
            Neuron * neuron_12;
            Neuron * neuron_13;
            Neuron * neuron_15;
            Neuron * neuron_16;
            Neuron * neuron_17;
            Neuron * neuron_18;
            Neuron * neuron_20;
            Neuron * neuron_22;
            Neuron * neuron_26;
            Neuron * neuron_30;
            Neuron * neuron_32;
            Neuron * neuron_33;
            Neuron * neuron_34;
            Neuron * neuron_41;
            Neuron * neuron_42;
            Neuron * neuron_51;
            Neuron * neuron_60;
            Neuron * neuron_62;
            Neuron * neuron_64;
            Neuron * neuron_67;
            Neuron * neuron_68;
            Neuron * neuron_77;
            Neuron * neuron_80;
            Neuron * neuron_88;
            Neuron * neuron_93;
            Neuron * neuron_95;
            Neuron * neuron_101;
            Neuron * neuron_102;

            Neuron* add_neuron(uint32_t node_id, double threshold) 
            {
                Neuron * n = new Neuron(node_id, threshold, false);

                if (!threshold_inclusive) {
                    n->threshold = (discrete) ? (n->threshold+1) : (n->threshold + 0.0000001);
                }

                neuron_map[node_id] = n;

                neurons.push_back(n);

                return n;
            }

            Synapse* add_synapse(
                    uint32_t from_id,
                    uint32_t to_id,
                    double weight,
                    uint32_t delay) 
            {
                unordered_map <uint32_t, Neuron*>::const_iterator it;

                Neuron * from = get_neuron(from_id);

                Neuron * to = get_neuron(to_id);

                Synapse * syn = new Synapse(weight, delay, to);

                from->synapses.push_back(syn);

                return syn;
            }

             Synapse* add_synapse(
                    Neuron * from, Neuron * to, double weight, uint32_t delay) 
            {
                Synapse * syn = new Synapse(weight, delay, to);

                from->synapses.push_back(syn);

                return syn;
            }

            Neuron* get_neuron(uint32_t node_id) 
            {
                unordered_map <uint32_t, Neuron*>::const_iterator it;

                it = neuron_map.find(node_id);

                return it->second;
            }

            void process_events(uint32_t time) 
            {
                const vector<std::pair <Neuron*, double>> es = std::move(events[time]);

                for (size_t i = 0; i < es.size(); i++) {
                    Neuron * n = es[i].first;
                    if (n->leak) {
                        n->charge = 0;
                    }
                    if (n->charge < min_potential) {
                        n->charge = min_potential;
                    }
                }

                for (size_t i = 0; i < es.size(); i++) {
                    Neuron * n = es[i].first;
                    n->check = true;
                    n->charge += es[i].second;
                }

                size_t events_size = events.size();

                for (size_t i = 0; i < es.size(); i++) {

                    Neuron * n = es[i].first;

                    if (n->check == true) {

                        /* fire */
                        if (n->charge >= n->threshold) {

                            for (size_t j = 0; j < n->synapses.size(); j++) {

                                Synapse * syn = n->synapses[j];

                                size_t to_time = time + syn->delay;

                                if (to_time >= events_size) {
                                    events_size = to_time + 1;
                                    events.resize(events_size);
                                }

                                double weight = syn->weight;

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
                for (size_t i = 0; i < neurons.size(); i++) {
                    Neuron * n = neurons[i];
                    n->last_fire = -1;
                    n->fire_counts = 0;
                }
            }

            void apply_spike_input(Neuron * neuron, const double time)
            {
                if (time >= events.size()) {
                    events.resize(time + 1);
                }

                double v = spike_value_factor;

                events[time].push_back(std::make_pair(neuron, v));
             }

            vector <Neuron *> neurons;

            unordered_map <uint32_t, Neuron*> neuron_map;   

            vector < vector < std::pair<Neuron *, double> >> events;

            int overall_run_time;     

            bool run_time_inclusive; 

            bool threshold_inclusive; 

            double min_potential;    

            bool discrete;           

            bool inputs_from_weights; 

            double spike_value_factor;
    };
}
