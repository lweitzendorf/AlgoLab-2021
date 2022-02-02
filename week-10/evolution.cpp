#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

void find_ancestors(int s, std::vector<int>& cur_ancestors,
                    const std::vector<std::vector<int>>& offspring,
                    std::vector<std::vector<int>>& ancestors,
                    std::vector<int>& last_offspring) {
  cur_ancestors.push_back(s);

  if (offspring[s].empty()) {
    last_offspring[s] = s;
    ancestors[s] = cur_ancestors;
  } else {
    int depth = cur_ancestors.size();
    for (int p : offspring[s]) {
      find_ancestors(p, cur_ancestors, offspring, ancestors, last_offspring);
      cur_ancestors.resize(depth);
      last_offspring[s] = last_offspring[p];
    }
  }
}

void solve() {
  int n, q;
  std::cin >> n >> q;

  std::unordered_map<std::string, int> species_idx;
  std::vector<std::string> name(n);
  std::vector<int> age(n);

  for (int i = 0; i < n; i++) {
    std::cin >> name[i] >> age[i];
    species_idx[name[i]] = i;
  }

  std::vector<bool> has_ancestor(n, false);
  std::vector<std::vector<int>> offspring(n);

  for (int i = 1; i < n; i++) {
    std::string s, p;
    std::cin >> s >> p;

    offspring[species_idx[p]].push_back(species_idx[s]);
    has_ancestor[species_idx[s]] = true;
  }

  int root = -1;
  for (int i = 0; i < n; i++) {
    if (!has_ancestor[i]) {
      root = i;
      break;
    }
  }

  std::vector<int> last_offspring(n, -1), cur_ancestors;
  std::vector<std::vector<int>> ancestors(n);

  find_ancestors(root, cur_ancestors, offspring, ancestors, last_offspring);

  for (int i = 0; i < q; i++) {
    std::string s;
    int b;
    std::cin >> s >> b;

    auto& anc = ancestors[last_offspring[species_idx[s]]];
    int a = *std::upper_bound(anc.begin(), anc.end(), b,
                              [&age](int b, int s) { return b >= age[s]; });

    std::cout << name[a] << " ";
  }

  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}