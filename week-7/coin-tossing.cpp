#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--) {
    int n, m;
    std::cin >> n >> m;
    
    graph G(n);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    for(int i = 0; i < m; i++) {
      int a, b, c;
      std::cin >> a >> b >> c;
      
      if (c == 1) {
        adder.add_edge(v_source, a, 1);
      } else if (c == 2) {
        adder.add_edge(v_source, b, 1);
      } else {
        vertex_desc v_round = boost::add_vertex(G);
        adder.add_edge(v_source, v_round, 1);
        adder.add_edge(v_round, a, 1);
        adder.add_edge(v_round, b, 1);
      }
    }
    
    int score_sum = 0;
    
    for(int i = 0; i < n; i++) {
      int s;
      std::cin >> s;
      
      score_sum += s;
      adder.add_edge(i, v_sink, s);
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout << ((flow == m && flow == score_sum) ? "yes" : "no") << std::endl;
  }
}