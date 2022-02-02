#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
        boost::property<boost::edge_reverse_t, traits::edge_descriptor,
        boost::property <boost::edge_weight_t, long> > > > > graph;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

class edge_adder {
  graph &G;

  public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};

void solve() {
  int N, S;
  std::cin >> N >> S;

  graph G(2);
  edge_adder adder(G);

  const auto v_source = 0;
  const auto v_target = 1;

  long MAX_TIME = 100000;
  long MAX_PROFIT = 100;

  std::vector<std::vector<int>> booked_times(S);
  std::vector<std::vector<int>> v_time(S, std::vector<int>(MAX_TIME+1, -1));

  int total_num_cars = 0;

  for (int s = 0; s < S; s++) {
    int l;
    std::cin >> l;

    total_num_cars += l;
    booked_times[s].push_back(0);
    v_time[s][0] = boost::add_vertex(G);
    adder.add_edge(v_source, v_time[s][0], l, 0);
  }

  for (int s = 0; s < S; s++) {
    booked_times[s].push_back(MAX_TIME);
    v_time[s][MAX_TIME] = boost::add_vertex(G);
    adder.add_edge(v_time[s][MAX_TIME], v_target, total_num_cars, 0);
  }

  for (int n = 0; n < N; n++) {
    int s, t, d, a, p;
    std::cin >> s >> t >> d >> a >> p;

    s--;
    t--;

    if (v_time[s][d] == -1) {
      v_time[s][d] = boost::add_vertex(G);
      booked_times[s].push_back(d);
    }

    if (v_time[t][a] == -1) {
      v_time[t][a] = boost::add_vertex(G);
      booked_times[t].push_back(a);
    }

    adder.add_edge(v_time[s][d], v_time[t][a], 1, (a-d)*MAX_PROFIT - p);
  }

  for (int s = 0; s < S; s++) {
    std::vector<int>& times = booked_times[s];

    int num_times = times.size();
    std::sort(times.begin(), times.end());

    for (int i = 1; i < num_times; i++) {
      int t1 = times[i-1], t2 = times[i];
      adder.add_edge(v_time[s][t1], v_time[s][t2], total_num_cars, (t2-t1)*MAX_PROFIT);
    }
  }

  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  long max_profit = total_num_cars*MAX_TIME*MAX_PROFIT - boost::find_flow_cost(G);

  std::cout << max_profit << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}