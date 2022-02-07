#include <iostream>
#include <vector>
#include <string>
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
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve() {
  int h, w;
  std::cin >> h >> w;

  std::string note;
  std::cin >> note;

  int n = note.size();
  std::vector<std::string> front(h), back(h);

  for (int i = 0; i < h; i++) {
    std::cin >> front[i];
  }

  for (int i = 0; i < h; i++) {
    std::cin >> back[i];
  }

  std::vector<int> letter_count(26, 0);
  std::vector<std::vector<int>> piece_count(26, std::vector<int>(26, 0));

  for (int i = 0; i < n; i++) {
    letter_count[note[i] - 'A']++;
  }

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      char front_letter = front[i][j];
      char back_letter = back[i][w-j-1];
      piece_count[front_letter - 'A'][back_letter - 'A']++;
    }
  }

  int num_letters = 26;
  int num_pieces = num_letters * num_letters;

  graph G(num_pieces + num_letters);
  edge_adder adder(G);

  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  for (int i = 0; i < num_letters; i++) {
    adder.add_edge(i, v_sink, letter_count[i]);

    for (int j = 0; j < num_letters; j++) {
      if (piece_count[i][j] > 0) {
        int v_piece = num_letters*(i+1) + j;
        adder.add_edge(v_source, v_piece, piece_count[i][j]);
        adder.add_edge(v_piece, i, piece_count[i][j]);
        adder.add_edge(v_piece, j, piece_count[i][j]);
      }
    }
  }

  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << (flow == n ? "Yes" : "No") << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}