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

int m, n;

int coord_to_index(int x, int y) {
  return m*y + x;
}

void solve() {
  int k, c;
  std::cin >> m >> n >> k >> c;

  graph G(2*m*n);
  edge_adder adder(G);

  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  for (int x = 0; x < m; x++) {
    for (int y = 0; y < n; y++) {
      int v = coord_to_index(x, y);
      adder.add_edge(2*v, 2*v+1, c);

      if (x > 0) {
        int u = coord_to_index(x-1, y);
        adder.add_edge(2*u+1, 2*v, 1);
        adder.add_edge(2*v+1, 2*u, 1);
      }

      if (y > 0) {
        int u = coord_to_index(x, y-1);
        adder.add_edge(2*u+1, 2*v, 1);
        adder.add_edge(2*v+1, 2*u, 1);
      }

      int num_exits = (x == 0) + (x == m-1) + (y == 0) + (y == n-1);
      adder.add_edge(2*v+1, v_sink, num_exits);
    }
  }

  for (int i = 0; i < k; i++) {
    int x, y;
    std::cin >> x >> y;

    int v = coord_to_index(x, y);
    adder.add_edge(v_source, 2*v, 1);
  }

  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << flow << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}