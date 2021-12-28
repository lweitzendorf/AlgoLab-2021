#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

namespace boost
{
struct edge_component_t
{
    enum
    {
        num = 555
    };
    typedef edge_property_tag kind;
} edge_component;
}

typedef boost::adjacency_list<boost::vecS, boost::vecS, 
                              boost::undirectedS, boost::no_property, 
                              boost::property<boost::edge_component_t, std::size_t>> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<graph>::edge_iterator edge_it;



int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    int n, m;
    std::cin >> n >> m;

    graph G(n);

    for (int i = 0; i < m; i++) {
      int e1, e2;
      std::cin >> e1 >> e2;
      boost::add_edge(e1, e2, G);
    }

    auto edge_components = boost::get(boost::edge_component, G);
    int nc = boost::biconnected_components(G, edge_components);

    std::vector<int> component_count(nc, 0);
    
    edge_it ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
      component_count[edge_components[*ei]]++;
    }

    std::vector<std::pair<vertex_t, vertex_t>> critical_bridges;
    
    for (boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
      if (component_count[edge_components[*ei]] == 1) {
        vertex_t v1 = source(*ei, G), v2 = target(*ei, G);
        if (v1 < v2) {
          critical_bridges.push_back(std::make_pair(v1, v2));
        } else {
          critical_bridges.push_back(std::make_pair(v2, v1));
        }
      }
    }
    
    std::sort(critical_bridges.begin(), critical_bridges.end());
    
    std::cout << critical_bridges.size() << "\n";
    for (const auto bridge : critical_bridges) {
      std::cout << bridge.first << " " << bridge.second << "\n";
    }
  }
}