#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <array>

using namespace std;

int min_moves(int pos, int n, int turn, int min_sit, int max_sit,
               const vector<vector<int>> &transitions, 
               vector<array<int, 4>> &cache) {
  int situation = turn % 4;
                 
  if (pos == n) {
    return 0;
  } else if (cache[pos][situation] != -1) {
    return cache[pos][situation];
  } else {
    int turns;
    
    if (situation == min_sit) { // player moves winning meeple
      turns = INT_MAX;
      for (const int next_pos : transitions.at(pos)) {
        turns = min(turns, min_moves(next_pos, n, turn+1, min_sit, max_sit, 
                                     transitions, cache));
      }
    } else if (situation == max_sit) { // player moves losing meeple
      turns = 0;
      for (const int next_pos : transitions.at(pos)) {
        turns = max(turns, min_moves(next_pos, n, turn+1, min_sit, max_sit, 
                                     transitions, cache));
      }
    } else {
      turns = min_moves(pos, n, turn+1, min_sit, max_sit, transitions, cache);
    }
    
    cache[pos][situation] = ++turns;
    return turns;
  }
}

int main() {
  ios_base::sync_with_stdio(false);

  int t;
  cin >> t;
  
  while(t--) {
    int n, m;
    cin >> n >> m;
    
    int r, b;
    cin >> r >> b;
    
    vector<vector<int>> transitions(n, vector<int>());
    
    for (int i=0; i < m; i++) {
      int u, v;
      cin >> u >> v;
      
      transitions.at(u-1).push_back(v-1);
    }
    
    vector<array<int, 4>> cache(n, {-1, -1, -1, -1});
    int red_min = min_moves(r-1, n-1, 0, 0, 3, transitions, cache);
    
    cache = vector<array<int, 4>>(n, {-1, -1, -1, -1});
    int black_min = min_moves(b-1, n-1, 0, 1, 2, transitions, cache);
    
    bool moriarty_win = (black_min < red_min);
    cout << moriarty_win << '\n';
  }
}
