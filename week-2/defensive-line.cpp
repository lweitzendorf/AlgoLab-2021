#include <bits/stdc++.h>
#include <iostream>
#include <vector>

using namespace std;

int max_value(int i, int m, int n, vector<int> &attacks, vector<vector<int>> &cache) {
  if (m == 0) {
    return 0;
  } else if (i >= n) {
    return INT_MIN;
  } else if (cache[i][m] != -1) {
    return cache[i][m];
  } else if (attacks[i] == -1) {
    int value = max_value(i+1, m, n, attacks, cache);
    cache[i][m] = value;
    return value;
  } else {
    int next_pos = attacks[i] + 1;
    int attack_value = next_pos - i;
    int value = max(max_value(i+1, m, n, attacks, cache), 
                    attack_value + max_value(next_pos, m-1, n, attacks, cache));
    cache[i][m] = value;
    return value;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t;
  cin >> t;
  
  while (t--) {
    int n, m, k;
    cin >> n >> m >> k;
    
    vector<int> defense;
    for (int i=0; i<n; i++) {
      int v;
      cin >> v;
      defense.push_back(v);
    }
    
    vector<int> viable_attacks(n, -1);
    int a = 0, b = 0, sum = defense.at(0);
    
    while (b < n) {
      if (sum == k && viable_attacks.at(a) == -1) {
        viable_attacks.at(a) = b;
      } else if (sum < k || a == b) {
        if (++b < n) sum += defense.at(b);
      } else {
        sum -= defense.at(a++);
      }
    }
    
    vector<vector<int>> cache(n, vector<int>(m+1, -1));
    int strategy_value = max_value(0, m, n, viable_attacks, cache);
    
    if (strategy_value >= 1) {
      cout << strategy_value << endl;
    } else {
      cout << "fail" << endl;
    }
  }
}