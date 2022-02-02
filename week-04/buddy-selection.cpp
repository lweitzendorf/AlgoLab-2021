#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

void solve() {
  int n, c, f;
  std::cin >> n >> c >> f;

  std::vector<std::unordered_set<std::string>> characteristics(n);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < c; j++) {
      std::string characteristic;
      std::cin >> characteristic;

      characteristics[i].insert(characteristic);
    }
  }

  graph G(n);

  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      int common_c = 0;
      for (std::string characteristic : characteristics[i]) {
        if (characteristics[j].find(characteristic) != characteristics[j].end()) {
          common_c++;
        }
      }

      if (common_c > f) {
        boost::add_edge(i, j, G);
      }
    }
  }

  std::vector<vertex_desc> mate_map(n);

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(),
      boost::get(boost::vertex_index, G)));

  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(),
      boost::get(boost::vertex_index, G)));

  std::cout << (matching_size == n/2 ? "not optimal" : "optimal") << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}