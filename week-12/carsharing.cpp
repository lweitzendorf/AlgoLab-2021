#include <iostream>
#include <set>
#include <unordered_map>
#include <tuple>
#include <queue>

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
                boost::property <boost::edge_weight_t, long>>>>> graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

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

void solve() {
  int N, S;
  std::cin >> N >> S;
  
  std::vector<std::set<int>> times_used(S);
  std::vector<std::unordered_map<int, vertex_desc>> time_to_vertex(S);
  
  int num_nodes = 0;
  int total_num_cars = 0;
  
  const int MAX_PROFIT = 100;
  const int MAX_TIME = 100000;
  
  std::vector<int> cars_available(S);

  for (int i = 0; i < S; i++) {
    std::cin >> cars_available[i];
    total_num_cars += cars_available[i];
    
    if (times_used[i].insert(0).second)
      time_to_vertex[i][0] = num_nodes++;
      
    if (times_used[i].insert(MAX_TIME).second)
      time_to_vertex[i][MAX_TIME] = num_nodes++;
  }
  
  std::vector<std::tuple<int, int, int, int, int>> bookings;
  
  for (int i = 0; i < N; i++) {
    int s, t, d, a, p;
    std::cin >> s >> t >> d >> a >> p;
    bookings.emplace_back(s-1, t-1, d, a, p);
    
    if (times_used[s-1].insert(d).second)
      time_to_vertex[s-1][d] = num_nodes++;
      
    if (times_used[t-1].insert(a).second)
      time_to_vertex[t-1][a] = num_nodes++;
  }
  
  graph G(num_nodes);
  edge_adder adder(G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_target = boost::add_vertex(G);

  for (int i = 0; i < S; i++) {
    vertex_desc last_v = v_source;
    int last_t = 0;

    for (int t : times_used[i]) {
      auto v = time_to_vertex[i][t];
      int c = (t == 0 ? cars_available[i] : total_num_cars);

      adder.add_edge(last_v, v, c, (t-last_t)*MAX_PROFIT);
      last_v = v;
      last_t = t;
    }
    
    adder.add_edge(time_to_vertex[i][MAX_TIME], v_target, total_num_cars, 0);
  }
  
  for (auto &b : bookings) {
    int d = std::get<2>(b), a = std::get<3>(b);
    vertex_desc v1 = time_to_vertex[std::get<0>(b)][d];
    vertex_desc v2 = time_to_vertex[std::get<1>(b)][a];
    
    adder.add_edge(v1, v2, 1, (a-d)*MAX_PROFIT - std::get<4>(b));
  }

  int flow = boost::push_relabel_max_flow(G, v_source, v_target);
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  int cost = flow*MAX_TIME*MAX_PROFIT - boost::find_flow_cost(G);
  std::cout << cost << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    solve();
  }
}