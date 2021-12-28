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

vertex_desc coord_to_vertex(int x, int y) {
  return x + y*m;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--) {
    int k, c;
    std::cin >> m >> n >> k >> c;
    
    const int num_intersections = m * n;
    
    graph G(2 * num_intersections);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    for (int i = 0; i < k; i++) {
      int x, y;
      std::cin >> x >> y;
      adder.add_edge(v_source, coord_to_vertex(x, y), 1);
    }
    
    for (int x = 0; x < m; x++) {
      for (int y = 0; y < n; y++) {
        vertex_desc vertex_in = coord_to_vertex(x, y);
        vertex_desc vertex_out = vertex_in + num_intersections;
        adder.add_edge(vertex_in, vertex_out, c);
        
        int num_exits = (x == 0) + (y == 0) + (x == m-1) + (y == n-1); 
        adder.add_edge(vertex_out, v_sink, num_exits);
        
        if (x > 0) {
          vertex_desc left_vertex_in = coord_to_vertex(x-1, y);
          vertex_desc left_vertex_out = left_vertex_in + num_intersections;
          adder.add_edge(left_vertex_out, vertex_in, 1);
          adder.add_edge(vertex_out, left_vertex_in, 1);
        }
        
        if (y > 0) {
          vertex_desc top_vertex_in = coord_to_vertex(x, y-1);
          vertex_desc top_vertex_out = top_vertex_in + num_intersections;
          adder.add_edge(top_vertex_out, vertex_in, 1);
          adder.add_edge(vertex_out, top_vertex_in, 1);
        }
      }
    }
    
    std::cout << boost::push_relabel_max_flow(G, v_source, v_sink) << std::endl;
  }
}