#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), 
      boost::get(boost::vertex_index, G)));
    
  return boost::matching_size(G,
            boost::make_iterator_property_map(mate_map.begin(), 
              boost::get(boost::vertex_index, G)));
}

bool feasible(const int a, const int s, const int c, const int d, 
              const std::vector<std::vector<int>> min_distances, const int t) {
  graph matching_graph(a + c*s);
  
  for (int k = 1; k <= c; k++)
    for (int i = 0; i < a; i++)
      for (int j = 0; j < s; j++)
        if (min_distances[i][j] <= t - k*d)
          boost::add_edge(i, a+(k-1)*s+j, matching_graph);
  
  return (maximum_matching(matching_graph) == a);
}

void dijkstra(const weighted_graph &G, const int s, std::vector<int> &dist_map) {
  boost::dijkstra_shortest_paths(G, s,
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));
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
    
    edge_desc e = boost::add_edge(x, y, G).first;
    weights[e] = z;

    if (w == 'L') {
      e = boost::add_edge(y, x, G).first;
      weights[e] = z;
    }
  }
  
  std::vector<int> agent_pos(a), shelter_pos(s);
  std::vector<std::vector<int>> min_distances(a, std::vector<int>(s));
  
  for (int i = 0; i < a; i++) {
    std::cin >> agent_pos[i];
  }
  
  for (int i = 0; i < s; i++) {
    std::cin >> shelter_pos[i];
  }
  
  int t_min = d, t_max = c*d;
  
  for (int i = 0; i < a; i++) {
    std::vector<int> dist_map(n);
    dijkstra(G, agent_pos[i], dist_map);
    
    for (int j = 0; j < s; j++) {
      min_distances[i][j] = dist_map[shelter_pos[j]];
      t_max = std::max(t_max, min_distances[i][j] + c*d);
    }
  }
  
  while (t_max > t_min) {
    int t = (t_min + t_max) / 2;
    if (feasible(a, s, c, d, min_distances, t)) {
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