#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;

void solve() {
  int n, m;
  std::cin >> n >> m;

  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  for (int i = 0; i < m; i++) {
    int u, v, w;
    std::cin >> u >> v >> w;

    edge_desc e = boost::add_edge(u, v, G).first;
    weights[e] = w;
  }

  std::vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

  int weight_sum = 0;
  int max_dist = 0;

  for (edge_desc e : mst) {
    weight_sum += weights[e];
  }

  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, 0,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  for (int d : dist_map) {
    max_dist = std::max(max_dist, d);
  }

  std::cout << weight_sum << " " << max_dist << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}