#pragma once

#include <cmath>

#include <string.h>

using namespace std;

namespace risp
{
    typedef struct {

        int weight;     
        class Neuron *to;       
        uint32_t delay;  

    } synapse_t;

    class Neuron {

        private:

            static const size_t MAX_SYNAPSES = 50;

        public:

            Neuron(int t) 
                : synapse_count(0),
                id(-1),
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

            synapse_t synapses[MAX_SYNAPSES]; 

            size_t synapse_count;
            int id;
            int charge;             
            int threshold;         
            int last_check;          
            int last_fire;          
            uint32_t fire_counts;  
            bool leak;            
            bool check;         
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

                add_neuron(&n0);
                add_neuron(&n1);
                add_neuron(&n6);
                add_neuron(&n0);
                add_neuron(&n6);
                add_neuron(&n7);
                add_neuron(&n0);
                add_neuron(&n3);
                add_neuron(&n3);
                add_neuron(&n0);
                add_neuron(&n6);
                add_neuron(&n2);
                add_neuron(&n0);
                add_neuron(&n0);
                add_neuron(&n4);
                add_neuron(&n7);
                add_neuron(&n7);
                add_neuron(&n3);
                add_neuron(&n7);
                add_neuron(&n6);
                add_neuron(&n4);
                add_neuron(&n3);
                add_neuron(&n2);
                add_neuron(&n4);
                add_neuron(&n6);
                add_neuron(&n5);
                add_neuron(&n7);
                add_neuron(&n3);
                add_neuron(&n4);
                add_neuron(&n2);
                add_neuron(&n2);
                add_neuron(&n5);
                add_neuron(&n7);
                add_neuron(&n3);
                add_neuron(&n4);
                add_neuron(&n3);
                add_neuron(&n3);
                add_neuron(&n5);
                add_neuron(&n3);
                add_neuron(&n5);

                add_synapse(&n11, &n102, -7, 9);
                add_synapse(&n8, &n34, 5, 3);
                add_synapse(&n12, &n17, 3, 10);
                add_synapse(&n1, &n11, -6, 12);
                add_synapse(&n8, &n2, -2, 1);
                add_synapse(&n13, &n1, 4, 2);
                add_synapse(&n8, &n9, -2, 14);
                add_synapse(&n2, &n8, 3, 7);
                add_synapse(&n13, &n26, -6, 4);
                add_synapse(&n64, &n95, -1, 6);
                add_synapse(&n20, &n15, 3, 7);
                add_synapse(&n16, &n13, 3, 8);
                add_synapse(&n80, &n34, 0, 9);
                add_synapse(&n60, &n26, 2, 11);
                add_synapse(&n8, &n15, -3, 6);
                add_synapse(&n6, &n9, 2, 3);
                add_synapse(&n77, &n101, 2, 14);
                add_synapse(&n26, &n11, 2, 1);
                add_synapse(&n9, &n20, 6, 9);
                add_synapse(&n0, &n22, -5, 15);
                add_synapse(&n51, &n1, -7, 1);
                add_synapse(&n2, &n77, 6, 7);
                add_synapse(&n1, &n3, 1, 3);
                add_synapse(&n2, &n42, 1, 2);
                add_synapse(&n3, &n16, -7, 13);
                add_synapse(&n32, &n60, -2, 14);
                add_synapse(&n10, &n1, -2, 3);
                add_synapse(&n6, &n10, 0, 3);
                add_synapse(&n16, &n62, 0, 1);
                add_synapse(&n102, &n30, -3, 11);
                add_synapse(&n60, &n10, 6, 9);
                add_synapse(&n32, &n34, 1, 1);
                add_synapse(&n32, &n2, -4, 14);
                add_synapse(&n30, &n32, 0, 9);
                add_synapse(&n13, &n3, 5, 3);
                add_synapse(&n26, &n77, 1, 15);
                add_synapse(&n64, &n13, -2, 12);
                add_synapse(&n2, &n67, 2, 14);
                add_synapse(&n11, &n8, -7, 8);
                add_synapse(&n5, &n95, 4, 9);
                add_synapse(&n30, &n80, 5, 5);
                add_synapse(&n9, &n51, -5, 12);
                add_synapse(&n51, &n8, 1, 15);
                add_synapse(&n0, &n1, -4, 3);
                add_synapse(&n11, &n6, 3, 14);
                add_synapse(&n101, &n26, 2, 2);
                add_synapse(&n4, &n93, 1, 4);
                add_synapse(&n1, &n17, -3, 14);
                add_synapse(&n9, &n4, -1, 7);
                add_synapse(&n95, &n60, 6, 8);
                add_synapse(&n1, &n30, 1, 13);
                add_synapse(&n17, &n102, 4, 1);
                add_synapse(&n6, &n26, 6, 2);
                add_synapse(&n0, &n26, 7, 1);
                add_synapse(&n4, &n0, 2, 15);
                add_synapse(&n2, &n3, 2, 9);
                add_synapse(&n101, &n10, 1, 4);
                add_synapse(&n51, &n11, 0, 14);
                add_synapse(&n8, &n51, 7, 5);
                add_synapse(&n10, &n68, 1, 4);
                add_synapse(&n4, &n33, 1, 6);
                add_synapse(&n8, &n0, 0, 4);
                add_synapse(&n0, &n13, 3, 8);
                add_synapse(&n68, &n12, -2, 13);
                add_synapse(&n12, &n80, -4, 3);
                add_synapse(&n4, &n4, 5, 3);
                add_synapse(&n5, &n93, 7, 12);
                add_synapse(&n9, &n77, -2, 15);
                add_synapse(&n1, &n2, -1, 6);
                add_synapse(&n6, &n8, 3, 12);
                add_synapse(&n11, &n7, -2, 5);
                add_synapse(&n80, &n32, 1, 11);
                add_synapse(&n9, &n26, 5, 4);
                add_synapse(&n15, &n10, 1, 10);
                add_synapse(&n33, &n30, 4, 9);
                add_synapse(&n6, &n88, -6, 15);
                add_synapse(&n93, &n12, 1, 1);
                add_synapse(&n88, &n10, 2, 14);
                add_synapse(&n18, &n41, -3, 8);
                add_synapse(&n0, &n18, 4, 14);
                add_synapse(&n11, &n41, 2, 12);
                add_synapse(&n6, &n51, -1, 10);
                add_synapse(&n30, &n101, -5, 14);
                add_synapse(&n13, &n4, 3, 15);
                add_synapse(&n60, &n17, -4, 8);
                add_synapse(&n80, &n10, 3, 6);
                add_synapse(&n102, &n1, 7, 8);
                add_synapse(&n12, &n8, 0, 12);
                add_synapse(&n5, &n13, 2, 1);
                add_synapse(&n20, &n95, 2, 8);
                add_synapse(&n42, &n5, 4, 6);
                add_synapse(&n7, &n32, 1, 11);
                add_synapse(&n33, &n51, -5, 15);
                add_synapse(&n67, &n102, 3, 10);
                add_synapse(&n41, &n77, 6, 6);
                add_synapse(&n60, &n93, -6, 12);
                add_synapse(&n77, &n95, 3, 8);
                add_synapse(&n80, &n26, -3, 6);
                add_synapse(&n17, &n0, 2, 10);
                add_synapse(&n60, &n0, -5, 9);
                add_synapse(&n62, &n4, 7, 5);
                add_synapse(&n68, &n11, 6, 7);
                add_synapse(&n9, &n2, 0, 4);
                add_synapse(&n0, &n95, -2, 14);
                add_synapse(&n34, &n7, 1, 10);
                add_synapse(&n64, &n3, 4, 14);
                add_synapse(&n9, &n67, -5, 3);
                add_synapse(&n22, &n64, -1, 13);
                add_synapse(&n10, &n41, -7, 15);
                add_synapse(&n67, &n9, 5, 13);
                add_synapse(&n51, &n34, -4, 14);
                add_synapse(&n2, &n6, 3, 14);
                add_synapse(&n51, &n60, 0, 12);
            }

            ~Network() {}

            void apply_spike_input0(const int time)
            {
                apply_spike_input(&n0, time);
            }

            void apply_spike_input1(const int time)
            {
                apply_spike_input(&n1, time);
            }

            void apply_spike_input2(const int time)
            {
                apply_spike_input(&n2, time);
            }

            void run(int timesteps)
            {
                if (overall_run_time != 0) {
                    clear_tracking_info();
                }

                const size_t run_time = (run_time_inclusive) ? timesteps : timesteps-1;

                overall_run_time += (run_time+1);

                for (size_t i = 0; i <= run_time; i++) {
                    process_events(i);
                }

                for (size_t i = 0; i < neuron_count; i++) {
                    Neuron * n = neurons[i];
                    if (n->leak) n->charge = 0;
                    if (n->charge < min_potential) n->charge = min_potential;
                }
            }

            void report_counts()
            {
                printf("n3: %d\n", n3.fire_counts);
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

                memset(events, 0, sizeof(events));

                overall_run_time = 0;
            }

        private:

            static const size_t MAX_NEURONS = 100;

            static const size_t MAX_EVENT_VECTORS = 300;
            static const size_t MAX_EVENTS_PER_VECTOR = 300;

            size_t neuron_count;

            Neuron * neurons[MAX_NEURONS];

            class Event {

                public:

                    Neuron * neuron;
                    int weight;

                    Event() = default;

                    Event(Neuron * n, int w) 
                        : neuron(n), weight(w) {}
            };

            typedef struct {

                Event events[MAX_EVENTS_PER_VECTOR];
                size_t size;

            } event_vector_t;

            event_vector_t events[MAX_EVENT_VECTORS];

            int overall_run_time;     
            bool run_time_inclusive; 
            bool threshold_inclusive; 
            int min_potential;    
            bool discrete;           
            bool inputs_from_weights; 
            int spike_value_factor;

            Neuron n0 = Neuron(3);
            Neuron n1 = Neuron(1);
            Neuron n2 = Neuron(6);
            Neuron n3 = Neuron(0);
            Neuron n4 = Neuron(6);
            Neuron n5 = Neuron(7);
            Neuron n6 = Neuron(0);
            Neuron n7 = Neuron(3);
            Neuron n8 = Neuron(3);
            Neuron n9 = Neuron(0);
            Neuron n10 = Neuron(6);
            Neuron n11 = Neuron(2);
            Neuron n12 = Neuron(0);
            Neuron n13 = Neuron(0);
            Neuron n15 = Neuron(4);
            Neuron n16 = Neuron(7);
            Neuron n17 = Neuron(7);
            Neuron n18 = Neuron(3);
            Neuron n20 = Neuron(7);
            Neuron n22 = Neuron(6);
            Neuron n26 = Neuron(4);
            Neuron n30 = Neuron(3);
            Neuron n32 = Neuron(2);
            Neuron n33 = Neuron(4);
            Neuron n34 = Neuron(6);
            Neuron n41 = Neuron(5);
            Neuron n42 = Neuron(7);
            Neuron n51 = Neuron(3);
            Neuron n60 = Neuron(4);
            Neuron n62 = Neuron(2);
            Neuron n64 = Neuron(2);
            Neuron n67 = Neuron(5);
            Neuron n68 = Neuron(7);
            Neuron n77 = Neuron(3);
            Neuron n80 = Neuron(4);
            Neuron n88 = Neuron(3);
            Neuron n93 = Neuron(3);
            Neuron n95 = Neuron(5);
            Neuron n101 = Neuron(3);
            Neuron n102 = Neuron(5);

            void add_neuron(Neuron * neuron)
            {
                neurons[neuron_count++] = neuron;
            }

            static void add_synapse(
                    Neuron * from, Neuron * to, int weight, uint32_t delay) 
            {
                synapse_t * syn = &from->synapses[from->synapse_count];

                syn->to = to;
                syn->delay = delay;
                syn->weight = weight;

                from->synapse_count++;
            }

            void process_events(uint32_t time) 
            {
                event_vector_t es = events[time];

                for (size_t i = 0; i < es.size; i++) {

                    Neuron * n = es.events[i].neuron;
                    if (n->leak) {
                        n->charge = 0;
                    }
                    if (n->charge < min_potential) {
                        n->charge = min_potential;
                    }
                }

                for (size_t i = 0; i < es.size; i++) {
                    Neuron * n = es.events[i].neuron;
                    n->check = true;
                    n->charge += es.events[i].weight;
                }

                for (size_t i = 0; i < es.size; i++) {

                    Neuron * n = es.events[i].neuron;

                    if (n->check == true) {

                        if (n->charge >= n->threshold) {

                            for (size_t j = 0; j < n->synapse_count; j++) {

                                synapse_t * syn = &n->synapses[j];

                                size_t to_time = time + syn->delay;

                                int weight = syn->weight;

                                const size_t size = events[to_time].size;

                                events[to_time].events[size].neuron = syn->to;
                                events[to_time].events[size].weight = weight;

                                events[to_time].size++;
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
                const size_t size = events[time].size;

                events[time].events[size].neuron = neuron;
                events[time].events[size].weight = spike_value_factor;

                events[time].size++;
            }

    };
}
