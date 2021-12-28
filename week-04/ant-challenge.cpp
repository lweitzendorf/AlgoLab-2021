#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
          boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

int main() {
  int t;
  std::cin >> t;
  
  while (t--) {
    int n, e, s, a, b;
    std::cin >> n >> e >> s >> a >> b;
    
    std::vector<weighted_graph> networks(s, weighted_graph(n));
    std::vector<weight_map> network_weights(s);

    for (int i = 0; i < s; i++) {
      network_weights[i] = boost::get(boost::edge_weight, networks[i]);
    }
    
    for (int i = 0; i < e; i++) {
      int t1, t2;
      std::cin >> t1 >> t2;
      
      for (int j = 0; j < s; j++) {
        int w;
        std::cin >> w;
        edge_desc edge = boost::add_edge(t1, t2, networks[j]).first;
        network_weights[j][edge] = w;
      }
    }
    
    weighted_graph final_network(n);
    weight_map final_weights = boost::get(boost::edge_weight, final_network);
    
    for (int i = 0; i < s; i++) {
      int hive;
      std::cin >> hive; // unique weights, connected -> hive irrelevant
      
      std::vector<edge_desc> mst;
      boost::kruskal_minimum_spanning_tree(networks[i], std::back_inserter(mst));
      
      for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
        vertex_desc t1 = boost::source(*it, networks[i]);
        vertex_desc t2 = boost::target(*it, networks[i]);
        edge_desc edge = boost::add_edge(t1, t2, final_network).first;
        final_weights[edge] = network_weights[i][*it];
      }
    }
    
    std::vector<int> dist_vec(n);
    std::vector<vertex_desc> pred_vec(n);
    
    auto dist_map = boost::distance_map(
                      boost::make_iterator_property_map(
                              dist_vec.begin(), 
                              boost::get(boost::vertex_index, final_network)));
    auto pred_map = dist_map.predecessor_map(
                      boost::make_iterator_property_map(
                              pred_vec.begin(), 
                              boost::get(boost::vertex_index, final_network)));
                              
    boost::dijkstra_shortest_paths(final_network, a, pred_map);
    
    std::cout << dist_vec[b] << "\n";
  }
}