#include <iostream>
#include <vector>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::edge_descriptor edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator out_edge_it;

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
  int n;
  std::cin >> n;

  int num_squares = 0;
  std::vector<std::vector<int>> v_square(n, std::vector<int>(n, -1));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      bool exists;
      std::cin >> exists;

      if (exists) {
        v_square[i][j] = num_squares++;
      }
    }
  }

  graph G(num_squares);
  edge_adder adder(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  const auto v_source = boost::add_vertex(G);
  const auto v_target = boost::add_vertex(G);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (v_square[i][j] != -1) {
        if ((i+j) % 2) {
          adder.add_edge(v_square[i][j], v_target, 1);
        } else {
          adder.add_edge(v_source, v_square[i][j], 1);

          if (i >= 1 && j >= 2 && v_square[i-1][j-2] != -1)
            adder.add_edge(v_square[i][j], v_square[i-1][j-2], 1);

          if (i >= 1 && j < n-2 && v_square[i-1][j+2] != -1)
            adder.add_edge(v_square[i][j], v_square[i-1][j+2], 1);

          if (i < n-1 && j >= 2 && v_square[i+1][j-2] != -1)
            adder.add_edge(v_square[i][j], v_square[i+1][j-2], 1);

          if (i < n-1 && j < n-2 && v_square[i+1][j+2] != -1)
            adder.add_edge(v_square[i][j], v_square[i+1][j+2], 1);

          if (i >= 2 && j >= 1 && v_square[i-2][j-1] != -1)
            adder.add_edge(v_square[i][j], v_square[i-2][j-1], 1);

          if (i >= 2 && j < n-1 && v_square[i-2][j+1] != -1)
            adder.add_edge(v_square[i][j], v_square[i-2][j+1], 1);

          if (i < n-2 && j >= 1 && v_square[i+2][j-1] != -1)
            adder.add_edge(v_square[i][j], v_square[i+2][j-1], 1);

          if (i < n-2 && j < n-1 && v_square[i+2][j+1] != -1)
            adder.add_edge(v_square[i][j], v_square[i+2][j+1], 1);
        }
      }
    }
  }

  boost::push_relabel_max_flow(G, v_source, v_target);

  std::queue<int> Q;
  Q.push(v_source);

  std::vector<int> vis(boost::num_vertices(G), false);
  vis[v_source] = true;

  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;

    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      if (rc_map[*ebeg] == 0 || vis[v])
        continue;

      vis[v] = true;
      Q.push(v);
    }
  }

  int max_knights = 0;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (v_square[i][j] != -1) {
        if (((i+j) % 2) != vis[v_square[i][j]]) {
          max_knights++;
        }
      }
    }
  }

  std::cout << max_knights << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}