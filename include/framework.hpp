#pragma once

#include <vector>
#include <map>
#include <memory>
#include <exception>
#include <stdexcept>

#include "robinhood/robin_map.h"
#include "nlohmann/json.hpp"
#include "utils/MOA.hpp"

namespace neuro
{

    using std::map;
    using std::string;
    using std::vector;
    using std::pair;
    using std::tuple;
    using std::unique_ptr;
    using nlohmann::json;

    class Property; 
    class PropertyPack;
    class Node;
    class Edge;
    class Network;
    class Processor;
    struct Spike;

    typedef pair<int,int> Coords;

    class int_hash
    {
        public:
            size_t operator() (const uint32_t val) const
            {

                size_t h = val;
                h ^= h >> 16;
                h *= 0x85ebca6b;
                h ^= h >> 13;
                h *= 0xc2b2ae35;
                h ^= h >> 16;
                return h;
            }
    };

    class coord_hash
    {
        public:

            size_t operator() (const Coords &p) const
            {
                size_t l = int_hash{}(p.first);
                size_t r = int_hash{}(p.second);
                l ^= r + 0x9e3779b9 + (l << 6) + (l >> 2);
                return l;
            }
    };

    typedef tsl::robin_map<uint32_t, unique_ptr<Node>, int_hash> NodeMap;
    typedef tsl::robin_map<Coords, unique_ptr<Edge>, coord_hash> EdgeMap;

    template<typename T, typename... Args>
        unique_ptr<T> make_unique(Args&&... args)
        {
            return unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

    class Property
    {
        public:

            enum Type : signed char
            {
                INTEGER = 'I', 
                DOUBLE  = 'D', 
                BOOLEAN = 'B', 
            };

            Property(const string& dname, int idx, int len, double dmin, double dmax, Type dtype);
            Property(const json& j);
            Property(const Property& p);
            Property(Property &&p) noexcept;

            Property& operator= (const Property &p);
            Property& operator= (Property &&p);

            ~Property() = default;

            void to_json(json &j) const;     
            json as_json() const;            
            void from_json(const json &j);   
            string pretty_json() const;      

            Type type;                       
            int index;                       
            int size;                        
            double min_value;                
            double max_value;                
            string name;                     
    };

    bool operator==(const Property &lhs, const Property &rhs);
    bool operator!=(const Property &lhs, const Property &rhs);

    typedef std::map<string, Property> PropertyMap;

    class PropertyPack
    {
        public:

            void to_json(json &j) const;     
            json as_json() const;            
            void from_json(const json &j);   
            string pretty_json() const;      
            void clear();                    

            PropertyMap nodes;               
            PropertyMap edges;               
            PropertyMap networks;            

            size_t node_vec_size = 0;        
            size_t edge_vec_size = 0;        
            size_t net_vec_size = 0;         

            operator tuple<PropertyMap&, PropertyMap&, PropertyMap&>()
            {
                return tuple<PropertyMap&, PropertyMap&, PropertyMap&>{nodes, edges, networks};
            }

            int add_node_property(const string& name, double dmin, double dmax, Property::Type type, int cnt = 1);
            int add_edge_property(const string& name, double dmin, double dmax, Property::Type type, int cnt = 1);
            int add_network_property(const string& name, double dmin, double dmax, Property::Type type, int cnt = 1);

    };

    bool operator==(const PropertyPack &lhs, const PropertyPack &rhs);
    bool operator!=(const PropertyPack &lhs, const PropertyPack &rhs);

    class Node
    {
        public:

            Node(uint32_t idx, Network* n = nullptr) : id(idx), net(n) {};
            Node(const Node &n) = delete;
            Node(Node &&n) = delete;
            Node& operator=(const Node &n) = delete;
            Node& operator=(Node &&n) = delete;
            ~Node() = default;

            uint32_t id = 0;                
            int input_id = -1;              
            int output_id = -1;             
            const Network *net;             
            vector<double> values;          
            vector<Edge*> incoming;         
            vector<Edge*> outgoing;         
            vector <double> coordinates;    
            string name;                    

            void set(int idx, double val);              
            void set(const string& name, double val);   
            double get(int idx);                        
            double get(const string& name);             

            inline bool is_hidden() const               
            { return !is_input() && !is_output(); }

            inline bool is_input() const                
            { return input_id >= 0; }

            inline bool is_output() const               
            { return output_id >= 0; }
    };

    class Edge
    {
        public:

            Edge(Node *f, Node *t, Network *n = nullptr) : from(f), to(t), net(n) {}
            Edge(const Edge &e) = delete;
            Edge(Edge &&e) = delete;
            Edge& operator=(const Edge &n) = delete;
            Edge& operator=(Edge &&n) = delete;
            ~Edge() = default;

            Node* from;                         
            Node* to;                           
            const Network *net;                 
            vector<double> values;              
            json as_json() const;               
            vector <double> control_point;      

            void set(int idx, double val);              
            void set(const string& name, double val);   
            double get(int idx);                        
            double get(const string& name);             
    };

    class Network 
    {
        public:

            Network() = default;
            Network(const Network &net);
            Network(Network &&net);
            Network& operator=(const Network &net);
            Network& operator=(Network &&net);
            ~Network() noexcept;

            bool operator==(const Network &rhs) const;

            void clear(bool include_properties); 

            void to_json(json &j) const;       
            json as_json() const;              
            void from_json(const json &j);     

            string pretty_json() const;        
            string pretty_nodes() const;       
            string pretty_edges() const;       

            void set_properties(const PropertyPack& properties);  
            PropertyPack get_properties() const;                  

            bool is_node_property(const string& name) const;    
            bool is_edge_property(const string& name) const;    
            bool is_network_property(const string& name) const; 

            const Property* get_node_property(const string& name) const;    
            const Property* get_edge_property(const string& name) const;    

            Node* add_node(uint32_t idx);                        
            bool is_node(uint32_t idx) const;                    
            Node* get_node(uint32_t idx) const;                  
            Node* add_or_get_node(uint32_t idx);                 
            void remove_node(uint32_t idx, bool force = false);  
            void rename_node(uint32_t old_name, uint32_t new_name); 

            Edge* add_edge(uint32_t fr, uint32_t to);            
            bool is_edge(uint32_t fr, uint32_t to) const;        
            Edge* get_edge(uint32_t fr, uint32_t to) const;      
            Edge* add_or_get_edge(uint32_t fr, uint32_t to);     
            void remove_edge(uint32_t fr, uint32_t to);          

            int add_input(uint32_t idx);                    
            Node* get_input(int input_id) const;            
            int num_inputs() const;                         

            int add_output(uint32_t idx);                    
            Node* get_output(int output_id) const;           
            int num_outputs() const;                         

            void set_data(const string& name, const json& data);   
            json get_data(const string& name) const;               
            vector<string> data_keys() const;                      

            Node* get_random_node(MOA &moa) const;        
            Edge* get_random_edge(MOA &moa) const;        
            Node* get_random_input(MOA &moa) const;       
            Node* get_random_output(MOA &moa) const;      

            void randomize_properties(MOA &moa);          
            void randomize_properties(MOA &moa, Node *n); 
            void randomize_properties(MOA &moa, Edge *e); 

            void randomize_property(MOA& moa, const string& pname);          
            void randomize_property(MOA& moa, Node *n, const string& pname); 
            void randomize_property(MOA& moa, Edge *n, const string& pname); 

            void randomize_property(MOA& moa,                
                    const Property& p,  
                    vector<double>& values); 

            void randomize(const json& params);            

            void prune();                      
            void make_sorted_node_vector();       
            vector <Node *> sorted_node_vector;   

            NodeMap::iterator begin();         
            NodeMap::iterator end();           

            EdgeMap::iterator edges_begin();       
            EdgeMap::iterator edges_end();         

            size_t num_nodes() const;          
            size_t num_edges() const;          

            vector<double> values;

        protected:

            void copy_from(const Network& net);

            void move_from(Network&& net);

            double random_value(MOA &moa, const Property &p);

            void randomize_p(const json& params);

            void randomize_h(const json& params);

            vector<uint32_t> m_inputs;
            vector<uint32_t> m_outputs;

            NodeMap m_nodes;
            EdgeMap m_edges;

            PropertyPack m_properties;

            json m_associated_data = {};

            friend class Node;
            friend class Edge;
    };

    struct Spike
    {
        int id; 
        double time; 
        double value; 

        Spike(int id_, double time_, double value_) : 
            id(id_), time(time_), value(value_) {}
    };

    class Processor
    {
        public:
            virtual ~Processor() {}

            static Processor *make();

            virtual void load_network(Network* n) = 0;

            virtual void apply_spike(const Spike& s, bool normalized = true) = 0;

            virtual void run(double duration) = 0;

            virtual vector <int> output_counts() = 0;

            virtual void clear_activity() = 0;
    };

}   
