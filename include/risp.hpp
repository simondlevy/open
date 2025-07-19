#pragma once

#include <cmath>
#include <map>
#include <memory>
#include <vector>

using namespace std;

using std::vector;

namespace risp
{
    class Synapse;
    class Neuron;
    class Network;

    class Neuron {

        private:

            static const size_t MAX_SYNAPSES = 50;

        public:

            Neuron(int t) 
                : synapse_count(0),
                charge(0),
                threshold(t),
                last_check(-1),
                last_fire(-1),
                fire_counts(0),
                leak(false),
                check(false) {};

            void perform_fire(int time)
            {
                last_fire = time;
                fire_counts++;
                charge = 0;
            }

            Synapse * synapses[MAX_SYNAPSES]; 

            size_t synapse_count;
            int charge;             
            int threshold;         
            int last_check;          
            int last_fire;          
            uint32_t fire_counts;  
            bool leak;            
            bool check;         
    };

    class Synapse {

        public:

            Synapse(int w, uint32_t d, Neuron* to_n) : weight(w), to(to_n), delay(d) {};

            int weight;     
            Neuron *to;       
            uint32_t delay;  
    };

    class Network {

        public:

            Network() 
            {
                neuron_count = 0;

                spike_value_factor = 7;
                min_potential = -7;
                run_time_inclusive = false;
                threshold_inclusive = true;
                discrete = true;
                inputs_from_weights = false;
                overall_run_time = 0;

                neuron_0 = add_neuron(3);
                neuron_1 = add_neuron(1);
                neuron_2 = add_neuron(6);
                neuron_3 = add_neuron(0);
                neuron_4 = add_neuron(6);
                neuron_5 = add_neuron(7);
                neuron_6 = add_neuron(0);
                neuron_7 = add_neuron(3);
                neuron_8 = add_neuron(3);
                neuron_9 = add_neuron(0);
                neuron_10 = add_neuron(6);
                neuron_11 = add_neuron(2);
                neuron_12 = add_neuron(0);
                neuron_13 = add_neuron(0);
                neuron_15 = add_neuron(4);
                neuron_16 = add_neuron(7);
                neuron_17 = add_neuron(7);
                neuron_18 = add_neuron(3);
                neuron_20 = add_neuron(7);
                neuron_22 = add_neuron(6);
                neuron_26 = add_neuron(4);
                neuron_30 = add_neuron(3);
                neuron_32 = add_neuron(2);
                neuron_33 = add_neuron(4);
                neuron_34 = add_neuron(6);
                neuron_41 = add_neuron(5);
                neuron_42 = add_neuron(7);
                neuron_51 = add_neuron(3);
                neuron_60 = add_neuron(4);
                neuron_62 = add_neuron(2);
                neuron_64 = add_neuron(2);
                neuron_67 = add_neuron(5);
                neuron_68 = add_neuron(7);
                neuron_77 = add_neuron(3);
                neuron_80 = add_neuron(4);
                neuron_88 = add_neuron(3);
                neuron_93 = add_neuron(3);
                neuron_95 = add_neuron(5);
                neuron_101 = add_neuron(3);
                neuron_102 = add_neuron(5);

                add_synapse(neuron_11, neuron_102, -7, 9);
                add_synapse(neuron_8, neuron_34, 5, 3);
                add_synapse(neuron_12, neuron_17, 3, 10);
                add_synapse(neuron_1, neuron_11, -6, 12);
                add_synapse(neuron_8, neuron_2, -2, 1);
                add_synapse(neuron_13, neuron_1, 4, 2);
                add_synapse(neuron_8, neuron_9, -2, 14);
                add_synapse(neuron_2, neuron_8, 3, 7);
                add_synapse(neuron_13, neuron_26, -6, 4);
                add_synapse(neuron_64, neuron_95, -1, 6);
                add_synapse(neuron_20, neuron_15, 3, 7);
                add_synapse(neuron_16, neuron_13, 3, 8);
                add_synapse(neuron_80, neuron_34, 0, 9);
                add_synapse(neuron_60, neuron_26, 2, 11);
                add_synapse(neuron_8, neuron_15, -3, 6);
                add_synapse(neuron_6, neuron_9, 2, 3);
                add_synapse(neuron_77, neuron_101, 2, 14);
                add_synapse(neuron_26, neuron_11, 2, 1);
                add_synapse(neuron_9, neuron_20, 6, 9);
                add_synapse(neuron_0, neuron_22, -5, 15);
                add_synapse(neuron_51, neuron_1, -7, 1);
                add_synapse(neuron_2, neuron_77, 6, 7);
                add_synapse(neuron_1, neuron_3, 1, 3);
                add_synapse(neuron_2, neuron_42, 1, 2);
                add_synapse(neuron_3, neuron_16, -7, 13);
                add_synapse(neuron_32, neuron_60, -2, 14);
                add_synapse(neuron_10, neuron_1, -2, 3);
                add_synapse(neuron_6, neuron_10, 0, 3);
                add_synapse(neuron_16, neuron_62, 0, 1);
                add_synapse(neuron_102, neuron_30, -3, 11);
                add_synapse(neuron_60, neuron_10, 6, 9);
                add_synapse(neuron_32, neuron_34, 1, 1);
                add_synapse(neuron_32, neuron_2, -4, 14);
                add_synapse(neuron_30, neuron_32, 0, 9);
                add_synapse(neuron_13, neuron_3, 5, 3);
                add_synapse(neuron_26, neuron_77, 1, 15);
                add_synapse(neuron_64, neuron_13, -2, 12);
                add_synapse(neuron_2, neuron_67, 2, 14);
                add_synapse(neuron_11, neuron_8, -7, 8);
                add_synapse(neuron_5, neuron_95, 4, 9);
                add_synapse(neuron_30, neuron_80, 5, 5);
                add_synapse(neuron_9, neuron_51, -5, 12);
                add_synapse(neuron_51, neuron_8, 1, 15);
                add_synapse(neuron_0, neuron_1, -4, 3);
                add_synapse(neuron_11, neuron_6, 3, 14);
                add_synapse(neuron_101, neuron_26, 2, 2);
                add_synapse(neuron_4, neuron_93, 1, 4);
                add_synapse(neuron_1, neuron_17, -3, 14);
                add_synapse(neuron_9, neuron_4, -1, 7);
                add_synapse(neuron_95, neuron_60, 6, 8);
                add_synapse(neuron_1, neuron_30, 1, 13);
                add_synapse(neuron_17, neuron_102, 4, 1);
                add_synapse(neuron_6, neuron_26, 6, 2);
                add_synapse(neuron_0, neuron_26, 7, 1);
                add_synapse(neuron_4, neuron_0, 2, 15);
                add_synapse(neuron_2, neuron_3, 2, 9);
                add_synapse(neuron_101, neuron_10, 1, 4);
                add_synapse(neuron_51, neuron_11, 0, 14);
                add_synapse(neuron_8, neuron_51, 7, 5);
                add_synapse(neuron_10, neuron_68, 1, 4);
                add_synapse(neuron_4, neuron_33, 1, 6);
                add_synapse(neuron_8, neuron_0, 0, 4);
                add_synapse(neuron_0, neuron_13, 3, 8);
                add_synapse(neuron_68, neuron_12, -2, 13);
                add_synapse(neuron_12, neuron_80, -4, 3);
                add_synapse(neuron_4, neuron_4, 5, 3);
                add_synapse(neuron_5, neuron_93, 7, 12);
                add_synapse(neuron_9, neuron_77, -2, 15);
                add_synapse(neuron_1, neuron_2, -1, 6);
                add_synapse(neuron_6, neuron_8, 3, 12);
                add_synapse(neuron_11, neuron_7, -2, 5);
                add_synapse(neuron_80, neuron_32, 1, 11);
                add_synapse(neuron_9, neuron_26, 5, 4);
                add_synapse(neuron_15, neuron_10, 1, 10);
                add_synapse(neuron_33, neuron_30, 4, 9);
                add_synapse(neuron_6, neuron_88, -6, 15);
                add_synapse(neuron_93, neuron_12, 1, 1);
                add_synapse(neuron_88, neuron_10, 2, 14);
                add_synapse(neuron_18, neuron_41, -3, 8);
                add_synapse(neuron_0, neuron_18, 4, 14);
                add_synapse(neuron_11, neuron_41, 2, 12);
                add_synapse(neuron_6, neuron_51, -1, 10);
                add_synapse(neuron_30, neuron_101, -5, 14);
                add_synapse(neuron_13, neuron_4, 3, 15);
                add_synapse(neuron_60, neuron_17, -4, 8);
                add_synapse(neuron_80, neuron_10, 3, 6);
                add_synapse(neuron_102, neuron_1, 7, 8);
                add_synapse(neuron_12, neuron_8, 0, 12);
                add_synapse(neuron_5, neuron_13, 2, 1);
                add_synapse(neuron_20, neuron_95, 2, 8);
                add_synapse(neuron_42, neuron_5, 4, 6);
                add_synapse(neuron_7, neuron_32, 1, 11);
                add_synapse(neuron_33, neuron_51, -5, 15);
                add_synapse(neuron_67, neuron_102, 3, 10);
                add_synapse(neuron_41, neuron_77, 6, 6);
                add_synapse(neuron_60, neuron_93, -6, 12);
                add_synapse(neuron_77, neuron_95, 3, 8);
                add_synapse(neuron_80, neuron_26, -3, 6);
                add_synapse(neuron_17, neuron_0, 2, 10);
                add_synapse(neuron_60, neuron_0, -5, 9);
                add_synapse(neuron_62, neuron_4, 7, 5);
                add_synapse(neuron_68, neuron_11, 6, 7);
                add_synapse(neuron_9, neuron_2, 0, 4);
                add_synapse(neuron_0, neuron_95, -2, 14);
                add_synapse(neuron_34, neuron_7, 1, 10);
                add_synapse(neuron_64, neuron_3, 4, 14);
                add_synapse(neuron_9, neuron_67, -5, 3);
                add_synapse(neuron_22, neuron_64, -1, 13);
                add_synapse(neuron_10, neuron_41, -7, 15);
                add_synapse(neuron_67, neuron_9, 5, 13);
                add_synapse(neuron_51, neuron_34, -4, 14);
                add_synapse(neuron_2, neuron_6, 3, 14);
                add_synapse(neuron_51, neuron_60, 0, 12);
            }

            ~Network() {}

            void apply_spike_input0(const int time)
            {
                apply_spike_input(neuron_0, time);
            }

            void apply_spike_input1(const int time)
            {
                apply_spike_input(neuron_1, time);
            }

            void apply_spike_input2(const int time)
            {
                apply_spike_input(neuron_2, time);
            }

            void run(int duration)
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

                for (size_t i = 0; i < neuron_count; i++) {
                    Neuron * n = neurons[i];
                    if (n->leak) n->charge = 0;
                    if (n->charge < min_potential) n->charge = min_potential;
                }
            }

            int n3_count()
            {
                return neuron_3->fire_counts;
            }

            void clear_activity() 
            {
                for (size_t i = 0; i < neuron_count; i++) {
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

            static const size_t MAX_NEURONS = 100;

            size_t neuron_count;
            Neuron * neurons[MAX_NEURONS];

            vector<vector< std::pair<Neuron *, int> >> events;

            int overall_run_time;     
            bool run_time_inclusive; 
            bool threshold_inclusive; 
            int min_potential;    
            bool discrete;           
            bool inputs_from_weights; 
            int spike_value_factor;

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

            Neuron* add_neuron(int threshold) 
            {
                Neuron * n = new Neuron(threshold);

                neurons[neuron_count++] = n;

                return n;
            }

             Synapse* add_synapse(
                    Neuron * from, Neuron * to, int weight, uint32_t delay) 
            {
                Synapse * syn = new Synapse(weight, delay, to);

                from->synapses[from->synapse_count++] = syn;

                return syn;
            }

            void process_events(uint32_t time) 
            {
                const vector<std::pair <Neuron*, int>> es = events[time];

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

                for (size_t i = 0; i < es.size(); i++) {

                    Neuron * n = es[i].first;

                    if (n->check == true) {

                        /* fire */
                        if (n->charge >= n->threshold) {

                            for (size_t j = 0; j < n->synapse_count; j++) {

                                Synapse * syn = n->synapses[j];

                                size_t to_time = time + syn->delay;

                                if (to_time >= events.size()) {
                                    events.resize(to_time + 1);
                                }

                                int weight = syn->weight;

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
                for (size_t i=0; i<neuron_count; i++) {
                    Neuron * n = neurons[i];
                    n->last_fire = -1;
                    n->fire_counts = 0;
                }
            }

            void apply_spike_input(Neuron * neuron, const size_t time)
            {
                if (time >= events.size()) {
                    events.resize(time + 1);
                }

                int v = spike_value_factor;

                events[time].push_back(std::make_pair(neuron, v));
             }

    };
}
