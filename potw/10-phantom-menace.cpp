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
  int n, m, s, d;
  std::cin >> n >> m >> s >> d;

  graph G(2*n);
  edge_adder adder(G);

  const auto v_source = boost::add_vertex(G);
  const auto v_sink = boost::add_vertex(G);

  for (int i = 0; i < n; i++) {
    adder.add_edge(2*i, 2*i+1, 1);
  }

  for (int e = 0; e < m; e++) {
    int i, j;
    std::cin >> i >> j;

    adder.add_edge(2*i+1, 2*j, 1);
  }

  for (int a = 0; a < s; a++) {
    int i;
    std::cin >> i;

    adder.add_edge(v_source, 2*i, 1);
  }

  for (int b = 0; b < d; b++) {
    int j;
    std::cin >> j;

    adder.add_edge(2*j+1, v_sink, 1);
  }

  std::cout << boost::push_relabel_max_flow(G, v_source, v_sink) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}