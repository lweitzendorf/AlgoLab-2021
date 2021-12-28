#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

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

struct guide {
  int x;
  int y;
  int d;
  int e;
};

int min_cost(int num_suitcases, int c, int k, int a, const std::vector<guide> &guides) {
  graph G(c);
  edge_adder adder(G);
  
  const auto v_source = boost::add_vertex(G);
  adder.add_edge(v_source, k, num_suitcases, 0);
  
  for (auto &gd : guides) {
    adder.add_edge(gd.x, gd.y, gd.e, gd.d);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, a);
  return boost::find_flow_cost(G);
}

void solve() {
  int c, g, b, k, a;
  std::cin >> c >> g >> b >> k >> a;
  
  graph G(c);
  edge_adder adder(G);  
  
  std::vector<guide> guides;
  
  for (int i = 0; i < g; i++) {
    int x, y, d, e;
    std::cin >> x >> y >> d >> e;
    
    adder.add_edge(x, y, e, d);
    guides.push_back({ .x=x, .y=y, .d=d, .e=e });
  }
  
  int min_suitcases = 0;
  int max_suitcases = boost::push_relabel_max_flow(G, k, a);
  
  while (min_suitcases != max_suitcases) {
    int num_suitcases = (min_suitcases + max_suitcases + 1) / 2;

    if (min_cost(num_suitcases, c, k, a, guides) <= b) {
      min_suitcases = num_suitcases;
    } else {
      max_suitcases = num_suitcases - 1;
    }
  }

  std::cout << min_suitcases << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while(t--) {
    solve();
  }
}
