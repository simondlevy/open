#include <iostream>
#include <sstream>

#include "risp.hpp"

using namespace std;

typedef runtime_error SRE;

static void to_uppercase(string &s) 
{
    size_t i;
    for (i = 0; i < s.size(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] + 'A' -'a';
        }
    }
}

int main(int argc, char **argv) 
{
    string prompt;

    istringstream ss;

    vector <string> sv; 

    vector <risp::Spike> spikes_array;

    if (argc == 2) {
        prompt = argv[1];
        prompt += " ";
    }

    risp::Network * net = nullptr;

    while (true) {

        try {

            if (prompt != "") printf("%s", prompt.c_str());

            string l;

            if (!getline(cin, l)) exit(0);

            sv.clear();
            ss.clear();
            ss.str(l);

            string s;

            while (ss >> s) sv.push_back(s);

            if (sv.size() != 0) to_uppercase(sv[0]); 

            if (sv[0] == "ML") {

                net = new risp::Network();
            }

            else if (sv[0] == "AS" || sv[0] == "ASV") {

                for (size_t i = 0; i < (sv.size() - 1) / 3; i++) {

                    int spike_id = 0;
                    double spike_time = 0;
                    double spike_val = 0;

                    if (sscanf(sv[i*3 + 1].c_str(), "%d", &spike_id) != 1 ||
                            sscanf(sv[i*3 + 2].c_str(), "%lf", &spike_time) != 1 || 
                            sscanf(sv[i*3 + 3].c_str(), "%lf", &spike_val) != 1 ) {

                        throw SRE((string) "Invalid spike [ " + sv[i*3 + 1] + "," + sv[i*3 + 2] + "," +
                                sv[i*3 + 3] + "]\n");
                    } 

                    net->apply_spike(risp::Spike( spike_id, spike_time, spike_val));

                    spikes_array.push_back(risp::Spike(spike_id, spike_time, spike_val));
                }
            } 

            else if (sv[0] == "RUN") {

                double sim_time = 0;

                if (sv.size() != 2 || sscanf(sv[1].c_str(), "%lf", &sim_time) != 1 || sim_time < 0) {
                    printf("usage: RUN sim_time. sim_time >= 0\n");
                } else {

                    net->run(sim_time);
                    spikes_array.clear();
                }

            }

            else if (sv[0] == "OC") {   

                if (sv.size() == 1) {

                    const auto event_counts = net->output_counts();

                    printf("node 3 spike counts: %d\n", event_counts[0]);
                }

            }

            else if (sv[0] == "CA" || sv[0] == "CLEAR-A") { // clear_activity

                net->clear_activity();

            }

        } catch (const SRE &e) {
            printf("%s\n", e.what());
        }

    }  // end of while loop
}
