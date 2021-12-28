#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    int n, c, f;
    std::cin >> n >> c >> f;

    std::unordered_map<std::string, std::vector<int>> interests;

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < c; j++) {
        std::string interest;
        std::cin >> interest;

        if (interests.find(interest) != interests.end()) {
          interests[interest].push_back(i);
        } else {
          interests[interest] = { i };
        }
      }
    }

    graph G(n);

    std::vector<std::vector<int>> common_interest(n, std::vector<int>(n, 0));

    for (const auto& interest : interests) {
      for (const int buddy_1 : interest.second) {
        for (const int buddy_2 : interest.second) {
          if (buddy_1 < buddy_2) {
            if (++common_interest.at(buddy_1).at(buddy_2) == f+1) {
              boost::add_edge(buddy_1, buddy_2, G);
            }
          }
        }
      }
    }

    std::vector<vertex_desc> mate_map(n);

    boost::edmonds_maximum_cardinality_matching(G,
      boost::make_iterator_property_map(
        mate_map.begin(),
        boost::get(boost::vertex_index, G)));

    int matching_size = boost::matching_size(G,
                          boost::make_iterator_property_map(
                            mate_map.begin(),
                            boost::get(boost::vertex_index, G)));

    if (matching_size == n/2) {
      std::cout << "not optimal\n";
    } else {
      std::cout << "optimal\n";
    }
  }
}