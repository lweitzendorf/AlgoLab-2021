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
        boost::property <boost::edge_weight_t, long>>>>> graph;
typedef traits::edge_descriptor edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator out_edge_it;

class edge_adder {
  graph &G;

  public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};

void solve() {
  int n;
  std::cin >> n;

  graph G(n);
  edge_adder adder(G);

  auto v_source = boost::add_vertex(G);
  auto v_target = boost::add_vertex(G);

  int num_students = 0;

  for (int i = 0; i < n; i++) {
    int a, c;
    std::cin >> a >> c;

    adder.add_edge(v_source, i, a, c);
  }

  for (int i = 0; i < n; i++) {
    int s, p;
    std::cin >> s >> p;

    num_students += s;
    adder.add_edge(i, v_target, s, 20-p);
  }

  for (int i = 1; i < n; i++) {
    int v, e;
    std::cin >> v >> e;

    adder.add_edge(i-1, i, v, e);
  }

  long flow = boost::push_relabel_max_flow(G, v_source, v_target);
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  long cost = 20*flow - boost::find_flow_cost(G);

  std::cout << (flow == num_students ? "possible" : "impossible");
  std::cout << " " << flow << " " << cost << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}