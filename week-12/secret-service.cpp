#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

bool feasible(int t, int c, int d, const std::vector<std::vector<int>>& travel_time) {
  int a = travel_time.size(), s = travel_time[0].size();

  graph matching_graph(a + c*s);

  for (int k = 1; k <= c; k++) {
    for (int i = 0; i < a; i++) {
      for (int j = 0; j < s; j++) {
        if (travel_time[i][j] <= t - k*d) {
          int v_shelter = a + (k-1)*s + j;
          boost::add_edge(i, v_shelter, matching_graph);
        }
      }
    }
  }

  std::vector<int> mate_map(boost::num_vertices(matching_graph));

  boost::edmonds_maximum_cardinality_matching(matching_graph,
    boost::make_iterator_property_map(mate_map.begin(),
      boost::get(boost::vertex_index, matching_graph)));

  int matching_size = boost::matching_size(matching_graph,
    boost::make_iterator_property_map(mate_map.begin(),
      boost::get(boost::vertex_index, matching_graph)));

  return (matching_size == a);
}

void solve() {
  int n, m, a, s, c, d;
  std::cin >> n >> m >> a >> s >> c >> d;

  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  for (int i = 0; i < m; i++) {
    char w;
    int x, y, z;
    std::cin >> w >> x >> y >> z;

    auto e = boost::add_edge(x, y, G).first;
    weights[e] = z;

    if (w == 'L') {
      e = boost::add_edge(y, x, G).first;
      weights[e] = z;
    }
  }

  std::vector<int> agent_pos(a), shelter_pos(s);

  for (int i = 0; i < a; i++) {
    std::cin >> agent_pos[i];
  }

  for (int i = 0; i < s; i++) {
    std::cin >> shelter_pos[i];
  }

  int t_min = d, t_max = d;
  std::vector<std::vector<int>> travel_time(a, std::vector<int>(s));

  for (int i = 0; i < a; i++) {
    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(G, agent_pos[i],
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));

    for (int j = 0; j < s; j++) {
      travel_time[i][j] = dist_map[shelter_pos[j]];
      t_max = std::max(t_max, travel_time[i][j] + c*d);
    }
  }

  while (t_min < t_max) {
    int t = (t_min + t_max) / 2;

    if (feasible(t, c, d, travel_time)) {
      t_max = t;
    } else {
      t_min = t + 1;
    }
  }

  std::cout << t_min << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}