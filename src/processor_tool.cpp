#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <unordered_set>
#include <unistd.h>

#include "risp.hpp"

using namespace std;
using namespace neuro;

typedef runtime_error SRE;

static void print_commands(FILE *f) 
{ 
    fprintf(f, "Q                                   - Quit\n");
}

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
    risp::Processor *p;

    string proc_name, prompt;
    string cmd;
    string l,s;

    ofstream fout;

    istringstream ss;

    size_t i;
    int spike_id;
    double spike_time, spike_val;
    double sim_time;
    string alias, id;

    vector <string> sv; // read inputs
    vector <risp::Spike> spikes_array;
    vector <risp::Spike> spikes;
    vector < vector< double> > neuron_times;     
    vector <string> spike_strings;              
    vector <int> v;
    vector <int> neuron_alias;
    vector <int> event_counts;
    vector <uint32_t> pres, posts;
    vector <double> weights;
    vector <double> charges;
    vector <double> data;
    vector <char> sr;
    map <int, double>::iterator mit;
    map <int, string> aliases; // Aliases for input/output nodes.
    map <int, string>::iterator ait;
    bool normalized;
    unordered_set <int> gsr_nodes;

    if (argc > 2 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
        fprintf(stderr, "usage: processor_tool [prompt]\n");
        fprintf(stderr, "\n");
        print_commands(stderr);
        exit(1);
    }

    if (argc == 2) {
        prompt = argv[1];
        prompt += " ";
    }

    p = nullptr;

    while(1) {

        try {

            if (prompt != "") printf("%s", prompt.c_str());
            if (!getline(cin, l)) exit(0);
            sv.clear();
            ss.clear();
            ss.str(l);

            while (ss >> s) sv.push_back(s);

            // convert cmd to uppercase
            if (sv.size() != 0) to_uppercase(sv[0]); 

            if (sv[0] == "ML") {

                p = new risp::Processor();

                p->init_network();
            }

            else if (sv[0] == "AS" || sv[0] == "ASV") {

                normalized = (sv[0].size() == 2);

                for (i = 0; i < (sv.size() - 1) / 3; i++) {

                    if (sscanf(sv[i*3 + 1].c_str(), "%d", &spike_id) != 1 ||
                            sscanf(sv[i*3 + 2].c_str(), "%lf", &spike_time) != 1 || 
                            sscanf(sv[i*3 + 3].c_str(), "%lf", &spike_val) != 1 ) {

                        throw SRE((string) "Invalid spike [ " + sv[i*3 + 1] + "," + sv[i*3 + 2] + "," +
                                sv[i*3 + 3] + "]\n");
                    } 

                    p->apply_spike(risp::Spike( spike_id, spike_time, spike_val),
                            normalized);

                    spikes_array.push_back(risp::Spike(spike_id, spike_time, spike_val));
                }
            } 

            else if (sv[0] == "RUN") {

                if (sv.size() != 2 || sscanf(sv[1].c_str(), "%lf", &sim_time) != 1 || sim_time < 0) {
                    printf("usage: RUN sim_time. sim_time >= 0\n");
                } else {

                    p->run(sim_time);
                    spikes_array.clear();
                }

            }

            else if (sv[0] == "OC") {   

                if (sv.size() == 1) {

                    event_counts = p->output_counts();

                    printf("node 3 spike counts: %d\n", event_counts[0]);
                }

            }

            else if (sv[0] == "CA" || sv[0] == "CLEAR-A") { // clear_activity

                p->clear_activity();

            }

        } catch (const SRE &e) {
            printf("%s\n", e.what());
        }

    }  // end of while loop
}
