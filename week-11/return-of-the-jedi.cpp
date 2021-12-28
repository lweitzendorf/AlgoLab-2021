#include <iostream>
#include <vector>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
          boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type   weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor              edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor            vertex_desc;

void max_dijkstra(const vertex_desc start, const weighted_graph &g, const weight_map &w, std::vector<int> &max_weight) {
  std::queue<std::pair<int, vertex_desc>> q;
  q.emplace(0, start);
  
  std::vector<bool> visited(boost::num_vertices(g), false);
  
  while (!q.empty()) {
    int new_max = q.front().first;
    vertex_desc v = q.front().second;
    q.pop();
    
    visited[v] = true;
    max_weight[v] = new_max;
    
    auto out_edges = boost::out_edges(v, g);
    for (auto it = out_edges.first; it != out_edges.second; ++it) {
      vertex_desc s = boost::source(*it, g);
      vertex_desc t = boost::target(*it, g);
      vertex_desc next_v = (v == s ? t : s);
      
      if (!visited[next_v]) {
        q.emplace(std::max(new_max, w[*it]), next_v);
      }
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    int n, i;
    std::cin >> n >> i;

    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    for (int j = 0; j < n; j++) {
      for (int k = j+1; k < n; k++) {
        int c;
        std::cin >> c;

        edge_desc e = boost::add_edge(j, k, G).first;
        weights[e] = c;
      }
    }

    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    weighted_graph mst_graph(n);
    weight_map mst_weights = boost::get(boost::edge_weight, mst_graph);

    int weight_sum = 0;

    for (auto it = mst.begin(); it != mst.end(); ++it) {
      vertex_desc s = boost::source(*it, G);
      vertex_desc t = boost::target(*it, G);
      int w = weights[*it];

      boost::remove_edge(*it, G);
      edge_desc e = boost::add_edge(s, t, mst_graph).first;

      weight_sum += w;
      mst_weights[e] = w;
    }
    
    std::vector<std::vector<int>> cycle_max(n, std::vector<int>(n, 0));
    
    auto vertices = boost::vertices(mst_graph);
    for (auto s = vertices.first; s != vertices.second; ++s) {
      max_dijkstra(*s, mst_graph, mst_weights, cycle_max[*s]);
    }
    
    int min_delta = std::numeric_limits<int>::max();

    auto edges = boost::edges(G);
    for (auto it = edges.first; it != edges.second; ++it) {
      vertex_desc s = boost::source(*it, G);
      vertex_desc t = boost::target(*it, G);
      min_delta = std::min(min_delta, weights[*it] - cycle_max[s][t]);
    }

    std::cout << weight_sum + min_delta << std::endl;
  }
}