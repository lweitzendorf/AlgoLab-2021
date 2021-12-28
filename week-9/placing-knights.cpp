#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;
// Interior Property Maps
typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
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
  
  while (t--) {
    int n;
    std::cin >> n;
    
    std::vector<std::vector<int>> v_square(n, std::vector<int>(n, -1));
    int num_squares = 0;
    
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        bool present;
        std::cin >> present;
        
        if (present)
          v_square[i][j] = num_squares++;
      }
    }
    
    graph G(num_squares);
    edge_adder adder(G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    std::vector<bool> vertex_set;
    
    const auto v_source = boost::add_vertex(G);
    const auto v_sink = boost::add_vertex(G);
    
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (v_square[i][j] != -1) {
          if ((i+j) % 2) {
            vertex_set.push_back(true);
            adder.add_edge(v_square[i][j], v_sink, 1);
          } else {
            vertex_set.push_back(false);
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
    
    boost::push_relabel_max_flow(G, v_source, v_sink);
    
    std::vector<int> vis(num_squares+2, false); 
    std::queue<int> Q; 
    vis[v_source] = true; 
    
    Q.push(v_source);
    
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
    
    int num_knights = 0;
  
    for (int i = 0; i < num_squares; ++i)
      if (vertex_set[i] != vis[i])
        num_knights++;
    
    std::cout << num_knights << std::endl;
  }
}