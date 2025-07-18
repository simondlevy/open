#include <iostream>
#include <cmath>
#include "risp.hpp"
#include <random>

typedef std::runtime_error SRE;
using namespace std;

namespace risp {

    Neuron* Network::get_neuron(uint32_t node_id) 
    {
        unordered_map <uint32_t, Neuron*>::const_iterator it;

        it = neuron_map.find(node_id);

        return it->second;
    }

    bool Network::is_neuron(uint32_t node_id) {
        return neuron_map.find(node_id) != neuron_map.end();
    }

    bool Network::is_valid_input_id(int input_id) {
        return !(input_id < 0 || input_id >= (int) inputs.size() || inputs[input_id] == -1);
    }

    bool Network::is_valid_output_id(int output_id) {
        return !(output_id < 0 || output_id >= (int) outputs.size() || outputs[output_id] == -1);
    }

    Neuron* Network::add_neuron(uint32_t node_id, double threshold, bool leak) {
        Neuron *n;

        n = new Neuron(node_id, threshold, leak);

        if (!threshold_inclusive) {
            n->threshold = (discrete) ? (n->threshold+1) : (n->threshold + 0.0000001);
        }

        neuron_map[node_id] = n;
        return n;
    }

    Synapse* Network::add_synpase(uint32_t from_id, uint32_t to_id, double weight, uint32_t delay) {
        Neuron *from, *to;
        Synapse *syn;
        unordered_map <uint32_t, Neuron*>::const_iterator it;

        from = get_neuron(from_id);
        to = get_neuron(to_id);

        syn = new Synapse(weight, delay, to);
        from->synapses.push_back(syn);

        return syn;
    }


    void Network::add_input(uint32_t node_id, int input_id) 
    {
        if (input_id >= (int) inputs.size()) inputs.resize(input_id + 1, -1);

        inputs[input_id] = node_id;
    }


    void Network::add_output(uint32_t node_id, int output_id) 
    {
        if (output_id >= (int) outputs.size()) outputs.resize(output_id + 1, -1);
        outputs[output_id] = node_id;
    }

    void Network::process_events(uint32_t time) 
    {
        size_t i, j;
        Neuron *n;
        Synapse *syn;
        size_t to_time;
        size_t events_size;
        double weight;

        const vector<std::pair <Neuron*, double>> es = std::move(events[time]);

        /* Cause neurons to fire if we're firing like RAVENS */

        for (i = 0; i < to_fire.size(); i++) to_fire[i]->perform_fire(time);
        neuron_fire_counter += to_fire.size();
        to_fire.clear();

        /* apply leak / reset minimum charge before the events happen */

        for (i = 0; i < es.size(); i++) {
            n = es[i].first;
            if (n->leak) n->charge = 0;
            if (n->charge < min_potential) n->charge = min_potential;
        }

        /* collect charges */

        for (i = 0; i < es.size(); i++) {
            n = es[i].first;
            n->check = true;
            n->charge += es[i].second;
            neuron_accum_counter++;
        }

        /* (CZ): I store events vector size onto events_size. 
           I think this is more efficient than using .size() call a lot times.
           (JSP doesn't think it matters.)  */

        events_size = events.size();

        /* determine if neuron fires */
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

                        if (weights.size() == 0) {
                            weight = syn->weight;
                        } else if (stds.size() == 0) {
                            weight = weights[int(syn->weight)];
                        } else {
                            weight = rng.Random_Normal(weights[int(syn->weight)], stds[int(syn->weight)]);
                        }
                        if (noisy_stddev != 0) weight = rng.Random_Normal(weight, noisy_stddev);

                        events[to_time].push_back(make_pair(syn->to, weight));

                    }

                    if (fire_like_ravens) {
                        to_fire.push_back(n);
                    } else {
                        neuron_fire_counter++;
                        n->perform_fire(time);
                    }
                }
                n->check = false;
            }
        }
    }


    void Network::clear_activity() {

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

    void Network::clear_tracking_info()
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

    int Network::output_count(int output_id) 
    {
        return neuron_map[outputs[output_id]]->fire_counts;
    }

    vector <int> Network::output_counts() {
        size_t i;
        vector <int> rv;
        for (i = 0; i < outputs.size(); i++) {
            if (outputs[i] != -1) rv.push_back(neuron_map[outputs[i]]->fire_counts);
        }
        return rv;
    }


    long long Network::total_neuron_accumulates() 
    {
        long long rv;

        rv = neuron_accum_counter;
        neuron_accum_counter = 0;
        return rv;
    }

    long long Network::total_neuron_counts() 
    {
        long long rv;

        rv = neuron_fire_counter;
        neuron_fire_counter = 0;
        return rv;
    }


    vector <double> Network::output_vector(int output_id) 
    {
        return neuron_map[outputs[output_id]]->fire_times;    // If tracking is turned off, this is empty.
    }

    vector < vector <double> > Network::output_vectors() {

        size_t i;
        Neuron *n;
        vector < vector <double> > rv;

        for (i = 0; i < outputs.size(); i++) {
            if (outputs[i] != -1) {
                n = neuron_map[outputs[i]];
                rv.push_back(n->fire_times);     // If tracking is turned off, this will be empty.
            }
        }
        return rv;

    }

    vector <int> Network::neuron_counts() {
        size_t i;
        vector <int> rv;

        for (i = 0; i < sorted_neuron_vector.size(); i++) {
            rv.push_back(sorted_neuron_vector[i]->fire_counts);
        }

        return rv;
    }

    vector < vector <double> > Network::neuron_vectors() {
        vector < vector <double> > rv;
        Neuron *n;
        size_t i;

        for (i = 0; i < sorted_neuron_vector.size(); i++) {
            n = sorted_neuron_vector[i];
            rv.push_back(n->fire_times);   // JSP: If tracking is turned off, this will be empty.
        }
        return rv;
    }

    vector < double > Network::neuron_charges() {
        vector < double > rv;
        Neuron *n;
        size_t i;

        for (i = 0; i < sorted_neuron_vector.size(); i++) {
            n = sorted_neuron_vector[i];
            rv.push_back(n->charge);
        }
        return rv;
    }


    vector <double> Network::neuron_last_fires() {
        vector <double> rv;
        size_t i;

        for (i = 0; i < sorted_neuron_vector.size(); i++) {
            rv.push_back(sorted_neuron_vector[i]->last_fire);
        }
        return rv;
    }

    void Network::synapse_weights(vector <uint32_t> &pres,
            vector <uint32_t> &posts,
            vector <double> &vals) {
        size_t i, j;
        Neuron *n;

        pres.clear();
        posts.clear();
        vals.clear();

        for (i = 0; i < sorted_neuron_vector.size(); i++) {
            n = sorted_neuron_vector[i];
            for (j = 0; j < n->synapses.size(); j++) {
                pres.push_back(n->id);
                posts.push_back(n->synapses[j]->to->id);
                vals.push_back(n->synapses[j]->weight);
            }
        }
    }

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
