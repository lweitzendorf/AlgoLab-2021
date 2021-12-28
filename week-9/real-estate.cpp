#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    int N, M, S;
    std::cin >> N >> M >> S;
    
    graph G(M + S);
    edge_adder adder(G);
    
    const auto v_source = boost::add_vertex(G);
    const auto v_target = boost::add_vertex(G);
    
    for (int i = 0; i < S; i++) {
      int limit;
      std::cin >> limit;
      adder.add_edge(M+i, v_target, limit, 0);
    }
    
    for (int v_property = 0; v_property < M; v_property++) {
      int state;
      std::cin >> state;
      adder.add_edge(v_property, M+state-1, 1, 0);
    }
    
    for (int i = 0; i < N; i++) {
      auto v_buyer = boost::add_vertex(G);
      for (int v_property = 0; v_property < M; v_property++) {
        int bid;
        std::cin >> bid;
        adder.add_edge(v_buyer, v_property, 1, 100-bid);
      }
      adder.add_edge(v_source, v_buyer, 1, 0);
    }
    
    long c = boost::push_relabel_max_flow(G, v_source, v_target);
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    long p = c*100 - boost::find_flow_cost(G);
    
    std::cout << c << " " << p << std::endl;
  }
}