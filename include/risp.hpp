#pragma once

#include <string.h>

using namespace std;

namespace risp
{
    typedef struct {

        int weight;     
        class Neuron *to;       
        uint32_t delay;  

    } synapse_t;

    class Constants {

        friend class Neuron;
        friend class Network;

        private:

        static const size_t MAX_SYNAPASES_PER_NEURON = 50;
        static const size_t MAX_EVENT_VECTORS = 300;
        static const size_t MAX_EVENTS_PER_VECTOR = 300;
    };

    class Neuron {

        public:

            synapse_t synapses[Constants::MAX_SYNAPASES_PER_NEURON]; 
            
            size_t synapse_count;
            int id;
            int charge;             
            int threshold;         
            int last_check;          
            int last_fire;          
            uint32_t fire_counts;  
            bool leak;            
            bool check;         
            class Synapse * synapse_list_head;
            class Synapse * synapse_list_tail;

            Neuron(int t) 
                : synapse_count(0),
                id(-1),
                charge(0),
                threshold(t),
                last_check(-1),
                last_fire(-1),
                fire_counts(0),
                leak(false),
                check(false),
                synapse_list_head(nullptr),
                synapse_list_tail(nullptr) {};

            void perform_fire(int time)
            {
                last_fire = time;
                fire_counts++;
                charge = 0;
            }

            void reset(const int min_potential)
            {
                if (leak) {
                    charge = 0;
                }

                if (charge < min_potential) {
                    charge = min_potential;
                }
            }

            void clear_tracking_info()
            {
                last_fire = -1;
                fire_counts = 0;
            }

            void clear_activity()
            {
                last_fire = -1;
                fire_counts = 0;
                charge = 0;
                last_check = -1;
            }

   };

    class Synapse {

        friend class Neuron;
        friend class Network;

        private:

            Neuron * from;       
            Neuron * to;       
            int weight;     
            uint32_t delay;  
            class Synapse * next;

            Synapse(Neuron * f, Neuron * t, int w, uint32_t d) 
                : from(f), to(t), weight(w), delay(d) { }
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
            
                connect_synapse(&s11_102);
                connect_synapse(&s11_8);
                connect_synapse(&s12_17);
                connect_synapse(&s1_11);
                connect_synapse(&s8_2);
                connect_synapse(&s13_1);
                connect_synapse(&s8_9);
                connect_synapse(&s2_8);
                connect_synapse(&s13_26);
                connect_synapse(&s64_);
                connect_synapse(&s20_15);
                connect_synapse(&s16_13);
                connect_synapse(&s80_34);
                connect_synapse(&s60_26);
                connect_synapse(&s8_15);
                connect_synapse(&s6_9);
                connect_synapse(&s77_101);
                connect_synapse(&s26_11);
                connect_synapse(&s9_20);
                connect_synapse(&s0_22);
                connect_synapse(&s51_1);
                connect_synapse(&s2_77);
                connect_synapse(&s1_3);
                connect_synapse(&s2_42);
                connect_synapse(&s3_16);
                connect_synapse(&s32_60);
                connect_synapse(&s10_1);
                connect_synapse(&s6_10);
                connect_synapse(&s16_62);
                connect_synapse(&s102_30);
                connect_synapse(&s60_10);
                connect_synapse(&s32_34);
                connect_synapse(&s32_2);
                connect_synapse(&s30_32);
                connect_synapse(&s13_3);
                connect_synapse(&s26_77);
                connect_synapse(&s64_13);
                connect_synapse(&s2_67);
                connect_synapse(&s11_8);
                connect_synapse(&s5_95);
                connect_synapse(&s30_80);
                connect_synapse(&s9_51);
                connect_synapse(&s51_8);
                connect_synapse(&s0_1);
                connect_synapse(&s11_6);
                connect_synapse(&s101_26);
                connect_synapse(&s4_93);
                connect_synapse(&s1_17);
                connect_synapse(&s9_4);
                connect_synapse(&s95_60);
                connect_synapse(&s1_30);
                connect_synapse(&s17_102);
                connect_synapse(&s6_26);
                connect_synapse(&s0_26);
                connect_synapse(&s4_0);
                connect_synapse(&s2_3);
                connect_synapse(&s101_10);
                connect_synapse(&s51_11);
                connect_synapse(&s8_51);
                connect_synapse(&s10_68);
                connect_synapse(&s4_33);
                connect_synapse(&s8_0);
                connect_synapse(&s0_13);
                connect_synapse(&s68_12);
                connect_synapse(&s12_80);
                connect_synapse(&s4_4);
                connect_synapse(&s5_93);
                connect_synapse(&s9_77);
                connect_synapse(&s1_2);
                connect_synapse(&s6_8);
                connect_synapse(&s11_7);
                connect_synapse(&s80_32);
                connect_synapse(&s9_26);
                connect_synapse(&s15_10);
                connect_synapse(&s33_30);
                connect_synapse(&s6_88);
                connect_synapse(&s93_12);
                connect_synapse(&s88_10);
                connect_synapse(&s18_41);
                connect_synapse(&s0_18);
                connect_synapse(&s11_41);
                connect_synapse(&s6_51);
                connect_synapse(&s30_101);
                connect_synapse(&s13_4);
                connect_synapse(&s60_17);
                connect_synapse(&s80_10);
                connect_synapse(&s102_1);
                connect_synapse(&s12_8);
                connect_synapse(&s5_13);
                connect_synapse(&s20_95);
                connect_synapse(&s42_5);
                connect_synapse(&s7_32);
                connect_synapse(&s33_51);
                connect_synapse(&s67_102);
                connect_synapse(&s41_77);
                connect_synapse(&s60_93);
                connect_synapse(&s77_95);
                connect_synapse(&s80_26);
                connect_synapse(&s17_0);
                connect_synapse(&s60_0);
                connect_synapse(&s62_4);
                connect_synapse(&s68_11);
                connect_synapse(&s9_2);
                connect_synapse(&s0_95);
                connect_synapse(&s34_7);
                connect_synapse(&s64_3);
                connect_synapse(&s9_67);
                connect_synapse(&s22_64);
                connect_synapse(&s10_41);
                connect_synapse(&s67_9);
                connect_synapse(&s51_34);
                connect_synapse(&s2_6);
                connect_synapse(&s51_60);
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

                reset_neurons();

            }

            void reset_neurons()
            {
                n0.reset(min_potential);
                n1.reset(min_potential);
                n2.reset(min_potential);
                n3.reset(min_potential);
                n4.reset(min_potential);
                n5.reset(min_potential);
                n6.reset(min_potential);
                n7.reset(min_potential);
                n8.reset(min_potential);
                n9.reset(min_potential);
                n10.reset(min_potential);
                n11.reset(min_potential);
                n12.reset(min_potential);
                n13.reset(min_potential);
                n15.reset(min_potential);
                n16.reset(min_potential);
                n17.reset(min_potential);
                n18.reset(min_potential);
                n20.reset(min_potential);
                n22.reset(min_potential);
                n26.reset(min_potential);
                n30.reset(min_potential);
                n32.reset(min_potential);
                n33.reset(min_potential);
                n34.reset(min_potential);
                n41.reset(min_potential);
                n42.reset(min_potential);
                n51.reset(min_potential);
                n60.reset(min_potential);
                n62.reset(min_potential);
                n64.reset(min_potential);
                n67.reset(min_potential);
                n68.reset(min_potential);
                n77.reset(min_potential);
                n80.reset(min_potential);
                n88.reset(min_potential);
                n93.reset(min_potential);
                n95.reset(min_potential);
                n101.reset(min_potential);
                n102.reset(min_potential);
            }

            void report_counts()
            {
                printf("n3: %d\n", n3.fire_counts);
            }

            void clear_activity() 
            {
                clear_neuron_activities();

                memset(events, 0, sizeof(events));

                overall_run_time = 0;
            }

            void clear_neuron_activities()
            {
                n0.clear_activity();
                n1.clear_activity();
                n2.clear_activity();
                n3.clear_activity();
                n4.clear_activity();
                n5.clear_activity();
                n6.clear_activity();
                n7.clear_activity();
                n8.clear_activity();
                n9.clear_activity();
                n10.clear_activity();
                n11.clear_activity();
                n12.clear_activity();
                n13.clear_activity();
                n15.clear_activity();
                n16.clear_activity();
                n17.clear_activity();
                n18.clear_activity();
                n20.clear_activity();
                n22.clear_activity();
                n26.clear_activity();
                n30.clear_activity();
                n32.clear_activity();
                n33.clear_activity();
                n34.clear_activity();
                n41.clear_activity();
                n42.clear_activity();
                n51.clear_activity();
                n60.clear_activity();
                n62.clear_activity();
                n64.clear_activity();
                n67.clear_activity();
                n68.clear_activity();
                n77.clear_activity();
                n80.clear_activity();
                n88.clear_activity();
                n93.clear_activity();
                n95.clear_activity();
                n101.clear_activity();
                n102.clear_activity();
            }

        private:

            size_t neuron_count;

            typedef struct {

                Neuron * neuron;
                int weight;

            } event_t;

            typedef struct {

                event_t events[Constants::MAX_EVENTS_PER_VECTOR];
                size_t size;

            } event_vector_t;

            event_vector_t events[Constants::MAX_EVENT_VECTORS];

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

            Synapse s11_102 = Synapse(&n11, &n102, -7, 9);
            Synapse s8_34 = Synapse(&n8, &n34, 5, 3);
            Synapse s12_17 = Synapse(&n12, &n17, 3, 10);
            Synapse s1_11 = Synapse(&n1, &n11, -6, 12);
            Synapse s8_2 = Synapse(&n8, &n2, -2, 1);
            Synapse s13_1 = Synapse(&n13, &n1, 4, 2);
            Synapse s8_9 = Synapse(&n8, &n9, -2, 14);
            Synapse s2_8 = Synapse(&n2, &n8, 3, 7);
            Synapse s13_26 = Synapse(&n13, &n26, -6, 4);
            Synapse s64_ = Synapse(&n64, &n95, -1, 6);
            Synapse s20_15 = Synapse(&n20, &n15, 3, 7);
            Synapse s16_13 = Synapse(&n16, &n13, 3, 8);
            Synapse s80_34 = Synapse(&n80, &n34, 0, 9);
            Synapse s60_26 = Synapse(&n60, &n26, 2, 11);
            Synapse s8_15 = Synapse(&n8, &n15, -3, 6);
            Synapse s6_9 = Synapse(&n6, &n9, 2, 3);
            Synapse s77_101 = Synapse(&n77, &n101, 2, 14);
            Synapse s26_11 = Synapse(&n26, &n11, 2, 1);
            Synapse s9_20 = Synapse(&n9, &n20, 6, 9);
            Synapse s0_22 = Synapse(&n0, &n22, -5, 15);
            Synapse s51_1 = Synapse(&n51, &n1, -7, 1);
            Synapse s2_77 = Synapse(&n2, &n77, 6, 7);
            Synapse s1_3 = Synapse(&n1, &n3, 1, 3);
            Synapse s2_42 = Synapse(&n2, &n42, 1, 2);
            Synapse s3_16 = Synapse(&n3, &n16, -7, 13);
            Synapse s32_60 = Synapse(&n32, &n60, -2, 14);
            Synapse s10_1 = Synapse(&n10, &n1, -2, 3);
            Synapse s6_10 = Synapse(&n6, &n10, 0, 3);
            Synapse s16_62 = Synapse(&n16, &n62, 0, 1);
            Synapse s102_30 = Synapse(&n102, &n30, -3, 11);
            Synapse s60_10 = Synapse(&n60, &n10, 6, 9);
            Synapse s32_34 = Synapse(&n32, &n34, 1, 1);
            Synapse s32_2 = Synapse(&n32, &n2, -4, 14);
            Synapse s30_32 = Synapse(&n30, &n32, 0, 9);
            Synapse s13_3 = Synapse(&n13, &n3, 5, 3);
            Synapse s26_77 = Synapse(&n26, &n77, 1, 15);
            Synapse s64_13 = Synapse(&n64, &n13, -2, 12);
            Synapse s2_67 = Synapse(&n2, &n67, 2, 14);
            Synapse s11_8 = Synapse(&n11, &n8, -7, 8);
            Synapse s5_95 = Synapse(&n5, &n95, 4, 9);
            Synapse s30_80 = Synapse(&n30, &n80, 5, 5);
            Synapse s9_51 = Synapse(&n9, &n51, -5, 12);
            Synapse s51_8 = Synapse(&n51, &n8, 1, 15);
            Synapse s0_1 = Synapse(&n0, &n1, -4, 3);
            Synapse s11_6 = Synapse(&n11, &n6, 3, 14);
            Synapse s101_26 = Synapse(&n101, &n26, 2, 2);
            Synapse s4_93 = Synapse(&n4, &n93, 1, 4);
            Synapse s1_17 = Synapse(&n1, &n17, -3, 14);
            Synapse s9_4 = Synapse(&n9, &n4, -1, 7);
            Synapse s95_60 = Synapse(&n95, &n60, 6, 8);
            Synapse s1_30 = Synapse(&n1, &n30, 1, 13);
            Synapse s17_102 = Synapse(&n17, &n102, 4, 1);
            Synapse s6_26 = Synapse(&n6, &n26, 6, 2);
            Synapse s0_26 = Synapse(&n0, &n26, 7, 1);
            Synapse s4_0 = Synapse(&n4, &n0, 2, 15);
            Synapse s2_3 = Synapse(&n2, &n3, 2, 9);
            Synapse s101_10 = Synapse(&n101, &n10, 1, 4);
            Synapse s51_11 = Synapse(&n51, &n11, 0, 14);
            Synapse s8_51 = Synapse(&n8, &n51, 7, 5);
            Synapse s10_68 = Synapse(&n10, &n68, 1, 4);
            Synapse s4_33 = Synapse(&n4, &n33, 1, 6);
            Synapse s8_0 = Synapse(&n8, &n0, 0, 4);
            Synapse s0_13 = Synapse(&n0, &n13, 3, 8);
            Synapse s68_12 = Synapse(&n68, &n12, -2, 13);
            Synapse s12_80 = Synapse(&n12, &n80, -4, 3);
            Synapse s4_4 = Synapse(&n4, &n4, 5, 3);
            Synapse s5_93 = Synapse(&n5, &n93, 7, 12);
            Synapse s9_77 = Synapse(&n9, &n77, -2, 15);
            Synapse s1_2 = Synapse(&n1, &n2, -1, 6);
            Synapse s6_8 = Synapse(&n6, &n8, 3, 12);
            Synapse s11_7 = Synapse(&n11, &n7, -2, 5);
            Synapse s80_32 = Synapse(&n80, &n32, 1, 11);
            Synapse s9_26 = Synapse(&n9, &n26, 5, 4);
            Synapse s15_10 = Synapse(&n15, &n10, 1, 10);
            Synapse s33_30 = Synapse(&n33, &n30, 4, 9);
            Synapse s6_88 = Synapse(&n6, &n88, -6, 15);
            Synapse s93_12 = Synapse(&n93, &n12, 1, 1);
            Synapse s88_10 = Synapse(&n88, &n10, 2, 14);
            Synapse s18_41 = Synapse(&n18, &n41, -3, 8);
            Synapse s0_18 = Synapse(&n0, &n18, 4, 14);
            Synapse s11_41 = Synapse(&n11, &n41, 2, 12);
            Synapse s6_51 = Synapse(&n6, &n51, -1, 10);
            Synapse s30_101 = Synapse(&n30, &n101, -5, 14);
            Synapse s13_4 = Synapse(&n13, &n4, 3, 15);
            Synapse s60_17 = Synapse(&n60, &n17, -4, 8);
            Synapse s80_10 = Synapse(&n80, &n10, 3, 6);
            Synapse s102_1 = Synapse(&n102, &n1, 7, 8);
            Synapse s12_8 = Synapse(&n12, &n8, 0, 12);
            Synapse s5_13 = Synapse(&n5, &n13, 2, 1);
            Synapse s20_95 = Synapse(&n20, &n95, 2, 8);
            Synapse s42_5 = Synapse(&n42, &n5, 4, 6);
            Synapse s7_32 = Synapse(&n7, &n32, 1, 11);
            Synapse s33_51 = Synapse(&n33, &n51, -5, 15);
            Synapse s67_102 = Synapse(&n67, &n102, 3, 10);
            Synapse s41_77 = Synapse(&n41, &n77, 6, 6);
            Synapse s60_93 = Synapse(&n60, &n93, -6, 12);
            Synapse s77_95 = Synapse(&n77, &n95, 3, 8);
            Synapse s80_26 = Synapse(&n80, &n26, -3, 6);
            Synapse s17_0 = Synapse(&n17, &n0, 2, 10);
            Synapse s60_0 = Synapse(&n60, &n0, -5, 9);
            Synapse s62_4 = Synapse(&n62, &n4, 7, 5);
            Synapse s68_11 = Synapse(&n68, &n11, 6, 7);
            Synapse s9_2 = Synapse(&n9, &n2, 0, 4);
            Synapse s0_95 = Synapse(&n0, &n95, -2, 14);
            Synapse s34_7 = Synapse(&n34, &n7, 1, 10);
            Synapse s64_3 = Synapse(&n64, &n3, 4, 14);
            Synapse s9_67 = Synapse(&n9, &n67, -5, 3);
            Synapse s22_64 = Synapse(&n22, &n64, -1, 13);
            Synapse s10_41 = Synapse(&n10, &n41, -7, 15);
            Synapse s67_9 = Synapse(&n67, &n9, 5, 13);
            Synapse s51_34 = Synapse(&n51, &n34, -4, 14);
            Synapse s2_6 = Synapse(&n2, &n6, 3, 14);
            Synapse s51_60 = Synapse(&n51, &n60, 0, 12);

            static void add_synapse(
                    Neuron * from, Neuron * to, int weight, uint32_t delay) 
            {
                synapse_t * syn = &from->synapses[from->synapse_count];

                syn->to = to;
                syn->delay = delay;
                syn->weight = weight;

                from->synapse_count++;
            }

            static void connect_synapse(Synapse * syn)
            {
                Neuron * from = syn->from;

                if (!from->synapse_list_head) {
                    from->synapse_list_head = syn;
                    from->synapse_list_tail = syn;
                }
                else {
                    from->synapse_list_tail->next = syn;
                }
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

                            forward_pass_activation(n, time);

                            n->perform_fire(time);
                        }

                        n->check = false;
                    }
                }
            }

            void new_forward_pass_activation(Neuron * n, const uint32_t time)
            {
                for (Synapse * s=n->synapse_list_head; s; s=s->next) {

                    size_t to_time = time + s->delay;

                    int weight = s->weight;

                    const size_t size = events[to_time].size;

                    events[to_time].events[size].neuron = s->to;
                    events[to_time].events[size].weight = weight;

                    events[to_time].size++;
                }
            }

             void forward_pass_activation(Neuron * n, const uint32_t time)
            {
                for (size_t j = 0; j < n->synapse_count; j++) {

                    synapse_t * syn = &n->synapses[j];

                    size_t to_time = time + syn->delay;

                    int weight = syn->weight;

                    const size_t size = events[to_time].size;

                    events[to_time].events[size].neuron = syn->to;
                    events[to_time].events[size].weight = weight;

                    events[to_time].size++;
                }
            }

            void clear_tracking_info()
            {
                n0.clear_tracking_info();
                n1.clear_tracking_info();
                n2.clear_tracking_info();
                n3.clear_tracking_info();
                n4.clear_tracking_info();
                n5.clear_tracking_info();
                n6.clear_tracking_info();
                n7.clear_tracking_info();
                n8.clear_tracking_info();
                n9.clear_tracking_info();
                n10.clear_tracking_info();
                n11.clear_tracking_info();
                n12.clear_tracking_info();
                n13.clear_tracking_info();
                n15.clear_tracking_info();
                n16.clear_tracking_info();
                n17.clear_tracking_info();
                n18.clear_tracking_info();
                n20.clear_tracking_info();
                n22.clear_tracking_info();
                n26.clear_tracking_info();
                n30.clear_tracking_info();
                n32.clear_tracking_info();
                n33.clear_tracking_info();
                n34.clear_tracking_info();
                n41.clear_tracking_info();
                n42.clear_tracking_info();
                n51.clear_tracking_info();
                n60.clear_tracking_info();
                n62.clear_tracking_info();
                n64.clear_tracking_info();
                n67.clear_tracking_info();
                n68.clear_tracking_info();
                n77.clear_tracking_info();
                n80.clear_tracking_info();
                n88.clear_tracking_info();
                n93.clear_tracking_info();
                n95.clear_tracking_info();
                n101.clear_tracking_info();
                n102.clear_tracking_info();
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
