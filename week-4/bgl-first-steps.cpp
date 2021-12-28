#include <iostream>
#include <vector>

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, 
                              boost::undirectedS, boost::no_property, 
                              boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--) {
    int n, m;
    std::cin >> n >> m;
    
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    
    for (int i = 0; i < m; i++) {
      int v1, v2, w;
      std::cin >> v1 >> v2 >> w;
      
      edge_desc e = boost::add_edge(v1, v2, G).first;
      weights[e] = w;
    }
    
    // MST weight sum
    int w = 0;
    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    
    for (auto it = mst.begin(); it != mst.end(); ++it) {
      w += weights[*it];
    }
    
    // Distance to furthest node
    std::vector<int> dist_vec(n);
    std::vector<vertex_desc> pred_vec(n);
    
    auto dist_map = boost::distance_map(
                      boost::make_iterator_property_map(
                              dist_vec.begin(), 
                              boost::get(boost::vertex_index, G)));
    auto pred_map = dist_map.predecessor_map(
                      boost::make_iterator_property_map(
                              pred_vec.begin(), 
                              boost::get(boost::vertex_index, G)));
    boost::dijkstra_shortest_paths(G, 0, pred_map);
                                          
    int d = *std::max_element(dist_vec.begin(), dist_vec.end());
    
    std::cout << w << " " << d << "\n";
  }
}