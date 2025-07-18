#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <unordered_set>
#include <unistd.h>
#include "framework.hpp"
#include "utils/json_helpers.hpp"

using namespace std;
using namespace neuro;
using nlohmann::json;

typedef runtime_error SRE;

/** Print commands */
void print_commands(FILE *f);

/** Convert a string to uppercase */
void to_uppercase(string &s);

/** Make sure the spike has the legal id, time, and value. **/
void spike_validation(const Spike &s, const Network *n, bool normalized);

/** Make sure the node "node_id" is an output node in the network */
void output_node_id_validation(const int node_id, const Network *n);

/** Make sure network and processor have been created */
bool network_processor_validation(const Network *n, const Processor *p);

/** Read a json object from file or stdin */
bool read_json(const vector <string> &sv, size_t starting_field, json &rv);


string node_name(Node *n);

void print_commands(FILE *f) 
{ 
    fprintf(f, "Q                                   - Quit\n");
}

string node_name(Node *n) {
    if (n->name == "") return std::to_string(n->id);
    return (std::to_string(n->id)) + "(" + n->name + ")";
}

void to_uppercase(string &s) 
{
    size_t i;
    for (i = 0; i < s.size(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] + 'A' -'a';
        }
    }
}

int node_validation(const Network *n, const string &node_id)
{
    uint32_t nid;

    if (sscanf(node_id.c_str(), "%u", &nid) == 0) {
        throw SRE((string) "Bad node specification - " + node_id);
    }
    if (!n->is_node(nid)) throw SRE(node_id + " is not a node in the network");
    return nid;
}

void spike_validation(const Spike &s, const Network *n, bool normalized) 
{
    Node *node;
    char buf[20];

    try {
        if (normalized) {
            if (s.value < -1 || s.value > 1) throw "spike val must be >= -1 and <= 1";
        }
        if (s.time < 0) throw "spike time must be > 0";
        node = n->get_node(s.id);
        if (!node->is_input()) {
            snprintf(buf, 20, "%d", s.id);
            throw (string) "node " + buf + " is not an input node";
        }

    } catch (const string &s) {
        throw SRE(s);
    } catch (const char *s) {
        throw SRE(s);
    }

}

void output_node_id_validation(const int node_id, const Network *n) 
{
    Node *node;
    char buf[20];

    try {
        if (node_id < 0) throw "node_id must > 0";
        node = n->get_node(node_id);
        if (!node->is_output()) {
            snprintf(buf, 20, "%d", node_id);
            throw (string) "node " + buf + " is not an input node";
        }
    } catch (const string &s) {
        throw SRE(s);
    } catch (const char *s) {
        throw SRE(s);
    }
}

bool network_processor_validation(const Network *n, const Processor *p) {
    bool success = (n != nullptr && p != nullptr);

    if (!success) {
        printf("Processor or network is not loaded\n");
    }
    return success;
}

bool read_json(const vector <string> &sv, size_t starting_field, json &rv)
{
    bool success;
    string s;
    ifstream fin;

    rv.clear();
    if (starting_field < sv.size()) {
        fin.clear();
        fin.open(sv[starting_field].c_str());
        if (fin.fail()) { 
            perror(sv[starting_field].c_str());
            return false;
        } 
        try { fin >> rv; success = true; } catch(...) { success = false; }
        fin.close();
        return success;

    } else {
        try {
            cin >> rv;
            getline(cin, s);
            return true;
        } catch (...) {
            return false;
        }
    }
}

Network *load_network(Processor **pp,
        const json &network_json)
{
    Network *net;
    json proc_params;
    string proc_name;
    Processor *p;

    net = new Network();
    net->from_json(network_json);

    p = *pp;
    if (p == nullptr) {
        proc_params = net->get_data("proc_params");
        proc_name = net->get_data("other")["proc_name"];
        p = Processor::make();
        *pp = p;
    } 


    p->load_network(net);

    return net;
}


void safe_exit(Processor *p, Network *n)
{
    if (p != nullptr) delete p;
    if (n != nullptr) delete n;
    exit(0);
}

int main(int argc, char **argv) 
{
    Processor *p;
    Network *net; 
    NodeMap::iterator nit;

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
    vector <Node *> node_vector;
    vector <Spike> spikes_array;
    vector <Spike> spikes;
    vector < vector< double> > neuron_times;     // hold the return value of neuron_times();
    vector <string> spike_strings;              // hold spike strings from neuron_vectors_to_json()
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

    json proc_params, network_json;
    json spike_counts, spike_raster;
    json associated_data;
    json j1, j2;

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
    net = nullptr;

    while(1) {
        try {
            if (prompt != "") printf("%s", prompt.c_str());
            if (!getline(cin, l)) safe_exit(p, net);
            sv.clear();
            ss.clear();
            ss.str(l);

            while (ss >> s) sv.push_back(s);

            // convert cmd to uppercase
            if (sv.size() != 0) to_uppercase(sv[0]); 

            if (sv[0] == "ML") { // make() and load_network()

                if (!read_json(sv, 1, network_json)) {

                    printf("usage: ML network_json. Bad json\n");

                } else {

                    try {

                        if (p != nullptr) { delete p; p = nullptr; }
                        if (net != nullptr) { delete net; net = nullptr; }

                        net = load_network(&p, network_json);

                    } catch (const SRE &e) {
                        printf("%s\n",e.what());
                        if (net != nullptr) { delete net; net = nullptr; }
                        if (p != nullptr) { delete p; p = nullptr; }
                        net = nullptr;
                        p = nullptr;
                    } catch (...) {
                        printf("Unknown error when making processor\n");
                        if (net != nullptr) { delete net; net = nullptr; }
                        if (p != nullptr) { delete p; p = nullptr; }
                        net = nullptr;
                        p = nullptr;

                    }
                }

            }

            else if (sv[0] == "AS" || sv[0] == "ASV") {

                if (network_processor_validation(net, p)) {

                    if (sv.size() < 2 || (sv.size() - 1) % 3 != 0) {

                    } else {

                        normalized = (sv[0].size() == 2);
                        for (i = 0; i < (sv.size() - 1) / 3; i++) {
                            try {

                                if (sscanf(sv[i*3 + 1].c_str(), "%d", &spike_id) != 1 ||
                                        sscanf(sv[i*3 + 2].c_str(), "%lf", &spike_time) != 1 || 
                                        sscanf(sv[i*3 + 3].c_str(), "%lf", &spike_val) != 1 ) {

                                    throw SRE((string) "Invalid spike [ " + sv[i*3 + 1] + "," + sv[i*3 + 2] + "," +
                                            sv[i*3 + 3] + "]\n");
                                } 
                                spike_validation(Spike(spike_id, spike_time, spike_val), net, normalized);

                                p->apply_spike(
                                        Spike(
                                            net->get_node(spike_id)->input_id,
                                            spike_time, spike_val),
                                        normalized);

                                spikes_array.push_back(Spike(spike_id, spike_time, spike_val));

                            } catch (const SRE &e) {
                                printf("%s\n",e.what());
                            }   

                        }
                    }
                }
            } 

            else if (sv[0] == "RUN") {

                if (network_processor_validation(net, p)) {
                    if (sv.size() != 2 || sscanf(sv[1].c_str(), "%lf", &sim_time) != 1 || sim_time < 0) {
                        printf("usage: RUN sim_time. sim_time >= 0\n");
                    } else {

                        p->run(sim_time);
                        spikes_array.clear();

                    }
                }

            }

            else if (sv[0] == "OC") {   

                if (network_processor_validation(net, p)) {

                    if (sv.size() == 1) {

                        event_counts = p->output_counts();

                        printf("node 3 spike counts: %d\n", event_counts[0]);
                    }
                }

            }

            else if (sv[0] == "CA" || sv[0] == "CLEAR-A") { // clear_activity

                if (network_processor_validation(net, p)) 
                    p->clear_activity();

            }

        } catch (const SRE &e) {
            printf("%s\n", e.what());
        }

    }  // end of while loop
}
