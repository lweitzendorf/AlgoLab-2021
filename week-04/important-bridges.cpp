#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace boost {
  struct edge_component_t {
    enum {
      num = 555
    };
    typedef edge_property_tag kind;
  } edge_component;
}

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
  boost::property<boost::edge_component_t, std::size_t>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

void solve() {
  int n, m;
  std::cin >> n >> m;

  graph G(n);

  for (int i = 0; i < m; i++) {
    int e1, e2;
    std::cin >> e1 >> e2;

    boost::add_edge(e1, e2, G);
  }

  auto component = boost::get(boost::edge_component, G);
  int num_comps = boost::biconnected_components(G, component);

  std::vector<int> component_size(num_comps, 0);
  std::vector<std::pair<int, int>> critical_bridges;

  edge_it ebeg, eend;
  for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
    component_size[component[*ebeg]]++;
  }

  for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
    if (component_size[component[*ebeg]] == 1) {
      int e1 = boost::source(*ebeg, G);
      int e2 = boost::target(*ebeg, G);
      critical_bridges.emplace_back(std::min(e1, e2), std::max(e1, e2));
    }
  }

  std::sort(critical_bridges.begin(), critical_bridges.end());

  std::cout << critical_bridges.size() << std::endl;
  for (auto bridge : critical_bridges) {
    std::cout << bridge.first << " " << bridge.second << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int c;
  std::cin >> c;

  while (c--) {
    solve();
  }
}