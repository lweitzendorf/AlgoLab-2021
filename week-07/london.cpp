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
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--) {
    int h, w;
    std::cin >> h >> w;
    
    std::string note;
    std::cin >> note;
    
    int n = note.length();
    
    std::vector<int> note_letter_count(26, 0);
    for (int i = 0; i < n; i++) {
      note_letter_count[note[i] - 'A']++;
    } 
    
    std::vector<std::string> front_lines(h);
    for (int i = 0; i < h; i++) {
      std::cin >> front_lines[i];
    }
    
    std::vector<std::string> back_lines(h);
    for (int i = 0; i < h; i++) {
      std::cin >> back_lines[i];
    }
    
    graph G(26);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    std::vector<std::vector<int>> piece_count(26, std::vector<int>(26, 0));
    
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        int front_letter = front_lines[i][j] - 'A';
        int back_letter = back_lines[i][w-j-1] - 'A';
        piece_count[front_letter][back_letter]++;
      }
    }
    
    for (int i = 0; i < 26; i++) {
      for (int j = 0; j < 26; j++) {
        int count = piece_count[i][j];
        if (count > 0) {
          vertex_desc v_piece = boost::add_vertex(G);
          adder.add_edge(v_source, v_piece, count);
          adder.add_edge(v_piece, i, count);
          adder.add_edge(v_piece, j, count);
        }
      }
    }
    
    for (int i = 0; i < 26; i++) {
      adder.add_edge(i, v_sink, note_letter_count[i]);
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout << (flow == n ? "Yes" : "No") << std::endl;
  }
}