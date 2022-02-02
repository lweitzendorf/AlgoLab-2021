#include <iostream>
#include <vector>

int min_turns(int pos, int turn, const int t, const int min_sit, const int max_sit,
              const std::vector<std::vector<int>>& transitions,
              std::vector<std::vector<int>>& cache) {
  int sit = (turn % 4);

  if (pos == t) {
    return 0;
  } else if (cache[sit][pos] == -1) {
    int sub_turns;

    if (sit == min_sit) {
      sub_turns = t+1;
      for (int next_pos : transitions[pos]) {
        sub_turns = std::min(sub_turns, min_turns(next_pos, turn+1, t, min_sit, max_sit, transitions, cache));
      }
    } else if (sit == max_sit) {
      sub_turns = 0;
      for (int next_pos : transitions[pos]) {
        sub_turns = std::max(sub_turns, min_turns(next_pos, turn+1, t, min_sit, max_sit, transitions, cache));
      }
    } else {
      sub_turns = min_turns(pos, turn+1, t, min_sit, max_sit, transitions, cache);
    }

    cache[sit][pos] = 1 + sub_turns;
  }

  return cache[sit][pos];
}

void solve() {
  int n, m;
  std::cin >> n >> m;

  int r, b;
  std::cin >> r >> b;

  std::vector<std::vector<int>> transitions(n);

  for (int i = 0; i < m; i++) {
    int u, v;
    std::cin >> u >> v;

    transitions[u-1].push_back(v-1);
  }

  std::vector<std::vector<int>> cache(4, std::vector<int>(n, -1));
  int red_moves = min_turns(r-1, 0, n-1, 0, 3, transitions, cache);

  cache = std::vector<std::vector<int>>(4, std::vector<int>(n, -1));
  int black_moves = min_turns(b-1, 0, n-1, 1, 2, transitions, cache);

  std::cout << (black_moves < red_moves) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }
}