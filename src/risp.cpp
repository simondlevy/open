#include <iostream>
#include <cmath>
#include "risp.hpp"
#include <random>

typedef std::runtime_error SRE;
using namespace std;

namespace risp {

    Processor::Processor() 
    {
        string estring;

        /* Default params */

        min_delay = 1;
        leak_mode = "none";
        run_time_inclusive = false;
        threshold_inclusive = true;
        fire_like_ravens = false;
        noisy_seed = 0;
        noisy_stddev = 0;
        inputs_from_weights = false;

        max_delay = 15;
        min_threshold = 0;
        max_threshold = 7;
        min_potential = -7;
        discrete = true;

        min_weight = -7;
        max_weight = 7;

        spike_value_factor = 7;

        /* Have the saved parameters include all of the default information.   The reason is
           that this way, if defaults change, you can still have this information stored. */

        if (weights.size() == 0) {
            saved_params["min_weight"] = min_weight;
            saved_params["max_weight"] = max_weight;
            saved_params["spike_value_factor"] = spike_value_factor;
        } else {
            saved_params["weights"] = weights;
            saved_params["inputs_from_weights"] = inputs_from_weights;
            if (!inputs_from_weights) saved_params["spike_value_factor"] = spike_value_factor;
        }

        saved_params["max_delay"] = max_delay;
        saved_params["min_threshold"] = min_threshold;
        saved_params["max_threshold"] = max_threshold;
        saved_params["min_potential"] = min_potential;
        saved_params["discrete"] = discrete;

        saved_params["leak_mode"] = leak_mode;
        saved_params["fire_like_ravens"] = fire_like_ravens;
        saved_params["run_time_inclusive"] = run_time_inclusive;
        saved_params["threshold_inclusive"] = threshold_inclusive;
        if (noisy_seed != 0) saved_params["noisy_seed"] = noisy_seed;
        if (noisy_stddev != 0) saved_params["noisy_stddev"] = noisy_stddev;
        if (stds.size() != 0) saved_params["stds"] = stds;

    };

    Processor::~Processor(){
        map <int, risp::Network*>::const_iterator it;
        for (it = networks.begin(); it != networks.end(); ++it) delete it->second;
    }

    /* Set the max spiking value (the value that corresponds to 1
       when you call apply_spikes() to either the max threshold, or
       if discrete&&!threshold_inclusive), then max threshold+1. */

    bool Processor::load_network(neuro::Network* net, int network_id) {

        if (networks.find(network_id) != networks.end()) delete networks[network_id];

        risp::Network * risp_net = new risp::Network(net, 
                spike_value_factor,
                min_potential,
                leak_mode[0], 
                run_time_inclusive, 
                threshold_inclusive, 
                fire_like_ravens, 
                discrete, 
                inputs_from_weights,
                noisy_seed, 
                noisy_stddev,
                weights,
                stds);
        networks[network_id] = risp_net;

        return true;
    }

    bool Processor::load_networks(std::vector<neuro::Network*> &n) {
        size_t i, j;

        for (i = 0; i < n.size(); i++) {
            if(load_network(n[i], i) == false) {
                for (j = 0; j <= i; j++) {
                    delete networks[j];
                    networks.erase(j);
                }
                return false;
            }
        }
        return true;
    }

    void Processor::clear(int network_id) {
        risp::Network *risp_net = get_risp_network(network_id);
        networks.erase(network_id);
        delete risp_net;
    }



    void Processor::apply_spike(const Spike& s, bool normalize, int network_id) {
        get_risp_network(network_id)->apply_spike(s, normalize);
    }

    void Processor::apply_spike(const Spike& s, 
            const vector<int>& network_ids,
            bool normalize) {
        size_t i;

        for (i = 0; i < network_ids.size(); i++) {
            apply_spike(s, normalize, network_ids[i]);
        }
    }

    void Processor::apply_spikes(const vector<Spike>& s, bool normalize, int network_id) {
        size_t i;
        risp::Network *risp_net = get_risp_network(network_id);

        for (i = 0; i < s.size(); i++) {
            risp_net->apply_spike(s[i], normalize);
        }
    }


    void Processor::apply_spikes(const vector<Spike>& s, 
            const vector<int>& network_ids,
            bool normalize) {
        size_t i;
        for (i = 0; i < network_ids.size(); i++) {
            apply_spikes(s, normalize, network_ids[i]);
        }
    }


    void Processor::run(double duration, int network_id) {
        get_risp_network(network_id)->run(duration);
    }

    void Processor::run(double duration, const vector<int>& network_ids) {
        size_t i;
        for (i = 0; i < network_ids.size(); i++) {
            get_risp_network(network_ids[i])->run(duration);
        }
    }


    long long Processor::total_neuron_counts(int network_id) {
        return get_risp_network(network_id)->total_neuron_counts();
    }

    long long Processor::total_neuron_accumulates(int network_id) {
        return get_risp_network(network_id)->total_neuron_accumulates();
    }

    double Processor::get_time(int network_id) {
        return get_risp_network(network_id)->get_time();
    }

    bool Processor::track_output_events(int output_id, bool track, int network_id) {
        return get_risp_network(network_id)->track_output_events(output_id, track);
    }

    bool Processor::track_neuron_events(uint32_t node_id, bool track, int network_id) {
        return get_risp_network(network_id)->track_neuron_events(node_id, track);
    }

    double Processor::output_last_fire(int output_id, int network_id) {
        return get_risp_network(network_id)->output_last_fire(output_id);
    }

    vector <double> Processor::output_last_fires(int network_id) {
        return get_risp_network(network_id)->output_last_fires();
    }

    int Processor::output_count(int output_id, int network_id) {
        return get_risp_network(network_id)->output_count(output_id);
    }

    vector <int> Processor::output_counts(int network_id) {
        return get_risp_network(network_id)->output_counts();
    }

    vector <double> Processor::output_vector(int output_id, int network_id) {
        return get_risp_network(network_id)->output_vector(output_id);
    }

    vector < vector <double> > Processor::output_vectors(int network_id) {
        return get_risp_network(network_id)->output_vectors();
    }


    vector <int> Processor::neuron_counts(int network_id) {
        return get_risp_network(network_id)->neuron_counts();
    }

    vector < vector <double> > Processor::neuron_vectors(int network_id) {
        return get_risp_network(network_id)->neuron_vectors();
    }
    vector < double > Processor::neuron_charges(int network_id) {
        return get_risp_network(network_id)->neuron_charges();
    }

    vector <double> Processor::neuron_last_fires(int network_id) {
        return get_risp_network(network_id)->neuron_last_fires();
    }

    void  Processor::synapse_weights(vector <uint32_t> &pres,
            vector <uint32_t> &posts,
            vector <double> &vals,
            int network_id)
    {
        return get_risp_network(network_id)->synapse_weights(pres, posts, vals);
    }


    /* Remove state, keep network loaded */
    void Processor::clear_activity(int network_id) {
        get_risp_network(network_id)->clear_activity();
    }

    string Processor::get_name() const {
        return "risp";
    }

    Network* Processor::get_risp_network(int network_id) 
    {
        map <int, risp::Network*>::const_iterator it;
        it = networks.find(network_id);
        return it->second;
    }

}
