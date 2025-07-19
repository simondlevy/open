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

            void apply_spike(const int id, const double time)
            {
                Neuron * n = get_neuron(id);

                if (time >= events.size()) {
                    events.resize(time + 1);
                }

                double v = spike_value_factor;

                events[time].push_back(std::make_pair(n,v));
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

        protected:

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

            Synapse* add_synpase(uint32_t from_id, uint32_t to_id, double weight, uint32_t delay) 
            {
                unordered_map <uint32_t, Neuron*>::const_iterator it;

                Neuron * from = get_neuron(from_id);
                Neuron * to = get_neuron(to_id);

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
