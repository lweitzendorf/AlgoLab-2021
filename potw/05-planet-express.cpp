#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property, 
                              boost::property<boost::edge_weight_t, int>> weighted_graph_t;
typedef boost::property_map<weighted_graph_t, boost::edge_weight_t>::type weight_map_t;
typedef boost::graph_traits<weighted_graph_t>::edge_descriptor edge_t;
typedef boost::graph_traits<weighted_graph_t>::vertex_descriptor vertex_t;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    int n, m, k, T;
    std::cin >> n >> m >> k >> T;
    
    weighted_graph_t G(n);
    weight_map_t weights = boost::get(boost::edge_weight, G);
    
    std::vector<vertex_t> teleportation;

    for (int i = 0; i < T; i++) {
      int t_i;
      std::cin >> t_i;
      teleportation.push_back(t_i);
    }
    
    for (int i = 0; i < m; i++) {
      int u, v, c;
      std::cin >> u >> v >> c;
      edge_t e = boost::add_edge(v, u, G).first;
      weights[e] = c;
    }
    
    std::vector<int> component_map(n);
    int ncc = boost::strong_components(G, 
                boost::make_iterator_property_map(component_map.begin(), 
                  boost::get(boost::vertex_index, G)));
                  
    std::vector<std::vector<vertex_t>> teleport_group(ncc);
    
    for (vertex_t v : teleportation) {
      teleport_group[component_map[v]].push_back(v);
    }
    
    for (int i = 0; i < ncc; i++) {
      int teleport_time = teleport_group[i].size()-1;
      if (teleport_time > 0) {
        vertex_t new_v = boost::add_vertex(G);
        
        for (vertex_t v : teleport_group[i]) {
          edge_t e1 = boost::add_edge(v, new_v, G).first;
          weights[e1] = teleport_time;
          edge_t e2 = boost::add_edge(new_v, v, G).first;
          weights[e2] = 0;
        }
      }
    }
    
    std::vector<int> dist_map(boost::num_vertices(G));
    std::vector<vertex_t> pred_map(boost::num_vertices(G));
    boost::dijkstra_shortest_paths(G, n-1,
        boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G)))
            .predecessor_map(boost::make_iterator_property_map(
              pred_map.begin(), boost::get(boost::vertex_index, G))));
              
    int min_time = std::numeric_limits<int>::max();
    
    for (int source = 0; source < k; source++) {
      min_time = std::min(min_time, dist_map[source]);
    }
    
    if (min_time <= 1000000) {
      std::cout << min_time << "\n";
    } else {
      std::cout << "no\n";
    }
  }
}