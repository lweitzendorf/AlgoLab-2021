#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void solve() {
  int n, m, k, T;
  std::cin >> n >> m >> k >> T;

  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  std::vector<int> teleportation_network(T);

  for (int i = 0; i < T; i++) {
    std::cin >> teleportation_network[i];
  }

  for (int i = 0; i < m; i++) {
    int u, v, c;
    std::cin >> u >> v >> c;

    auto e = boost::add_edge(v, u, G).first;
    weights[e] = c;
  }

  std::vector<int> component(n);

  int num_components = boost::strong_components(G,
    boost::make_iterator_property_map(component.begin(),
      boost::get(boost::vertex_index, G)));

  std::vector<std::vector<int>> teleport_groups(num_components);

  for (int v : teleportation_network) {
    teleport_groups[component[v]].push_back(v);
  }

  for (auto& group : teleport_groups) {
    int group_size = group.size();

    if (group_size > 1) {
      auto v_intermediate = boost::add_vertex(G);

      for (int v_planet : group) {
        auto e = boost::add_edge(v_planet, v_intermediate, G).first;
        weights[e] = group_size - 1;
        e = boost::add_edge(v_intermediate, v_planet, G).first;
        weights[e] = 0;
      }
    }
  }

  std::vector<int> dist_map(boost::num_vertices(G));

  boost::dijkstra_shortest_paths(G, n-1,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  int min_time = std::numeric_limits<int>::max();

  for (int i = 0; i < k; i++) {
    min_time = std::min(min_time, dist_map[i]);
  }

  if (min_time > 1e6) {
    std::cout << "no" << std::endl;
  } else {
    std::cout << min_time << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}