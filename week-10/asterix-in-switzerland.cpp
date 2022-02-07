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

void solve() {
  int n, m;
  std::cin >> n >> m;

  graph G(n);
  edge_adder adder(G);

  const auto v_source = boost::add_vertex(G);
  const auto v_sink = boost::add_vertex(G);

  long total_pos_balance = 0;

  for (int i = 0; i < n; i++) {
    int b;
    std::cin >> b;

    if (b > 0) {
      total_pos_balance += b;
      adder.add_edge(v_source, i, b);
    } else {
      adder.add_edge(i, v_sink, -b);
    }
  }

  for (int r = 0; r < m; r++) {
    int i, j, d;
    std::cin >> i >> j >> d;

    adder.add_edge(i, j, d);
  }

  long min_debt = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << (min_debt < total_pos_balance ? "yes" : "no") << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}