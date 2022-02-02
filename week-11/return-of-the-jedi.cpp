#include <iostream>
#include <vector>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void max_dijkstra(vertex_desc s, const weighted_graph& G,
                  const weight_map& weights, std::vector<int>& out) {
  std::queue<vertex_desc> q;
  q.push(s);

  std::vector<bool> visited(boost::num_vertices(G), false);
  visited[s] = true;

  while (!q.empty()) {
    auto v = q.front();
    q.pop();

    for (auto it = boost::out_edges(v, G).first; it != boost::out_edges(v, G).second; it++) {
      auto s = boost::source(*it, G), t = boost::target(*it, G);
      auto next_v = (v == s ? t : s);

      if (!visited[next_v]) {
        out[next_v] = std::max(out[v], weights[*it]);
        q.push(next_v);
        visited[next_v] = true;
      }
    }
  }
}

void solve() {
  int n, i;
  std::cin >> n >> i;

  weighted_graph G(n), mst_graph(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  weight_map mst_weights = boost::get(boost::edge_weight, mst_graph);

  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      auto e = boost::add_edge(i, j, G).first;
      std::cin >> weights[e];
    }
  }

  std::vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

  int mst_weight = 0;

  for (edge_desc e : mst) {
    mst_weight += weights[e];

    auto s = boost::source(e, G), t = boost::target(e, G);
    auto mst_e = boost::add_edge(s, t, mst_graph).first;
    mst_weights[mst_e] = weights[e];
    boost::remove_edge(e, G);
  }

  std::vector<std::vector<int>> cycle_max(n, std::vector<int>(n, 0));

  for (int v = 0; v < n; v++) {
    max_dijkstra(v, mst_graph, mst_weights, cycle_max[v]);
  }

  int min_diff = mst_weight;

  for (auto it = boost::edges(G).first; it != boost::edges(G).second; it++) {
    auto s = boost::source(*it, G), t = boost::target(*it, G);
    min_diff = std::min(min_diff, weights[*it] - cycle_max[s][t]);
  }

  std::cout << mst_weight + min_diff << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}