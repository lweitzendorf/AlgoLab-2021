#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::property<boost::edge_weight_t, int>> graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor          vertex_desc;

struct TreeEdge {
  int t1, t2;
  std::vector<int> weights;
};

void solve() {
  int n, e, s, a, b;
  std::cin >> n >> e >> s >> a >> b;

  std::vector<TreeEdge> edges;

  for (int i = 0; i < e; i++) {
    TreeEdge edge;
    std::cin >> edge.t1 >> edge.t2;

    for (int j = 0; j < s; j++) {
      int w;
      std::cin >> w;
      edge.weights.push_back(w);
    }

    edges.push_back(edge);
  }

  graph global_network(n);
  weight_map global_weights = boost::get(boost::edge_weight, global_network);

  for (int i = 0; i < s; i++) {
    int h;
    std::cin >> h;

    graph private_network(n);
    weight_map private_weights = boost::get(boost::edge_weight, private_network);

    for (TreeEdge edge : edges) {
      auto e = boost::add_edge(edge.t1, edge.t2, private_network).first;
      private_weights[e] = edge.weights[i];
    }

    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(private_network, std::back_inserter(mst));

    for (auto e : mst) {
      auto t1 = boost::source(e, private_network);
      auto t2 = boost::target(e, private_network);
      auto ge = boost::add_edge(t1, t2, global_network).first;
      global_weights[ge] = private_weights[e];
    }
  }

  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(global_network, a,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, global_network))));

  std::cout << dist_map[b] << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}