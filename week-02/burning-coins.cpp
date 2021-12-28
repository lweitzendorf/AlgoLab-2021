#include <iostream>
#include <vector>

using namespace std;

int play(vector<int> &v, vector<vector<int>> &scores, int start, int end, int turn) {
  if (start > end) {
    return 0;
  } else if (scores[start][end] != -1) {
    return scores[start][end];
  } else {
    int score;
    int outcome_1 = play(v, scores, start + 1, end, turn + 1);
    int outcome_2 = play(v, scores, start, end - 1, turn + 1);
    
    if (turn % 2) {
      score = min(outcome_1, outcome_2);
    } else {
      score = max(outcome_1 + v.at(start), outcome_2 + v.at(end));
    } 
    
    scores[start][end] = score;
    return score;
  } 
}

int main() {
  int t;
  cin >> t;

  while (t--) {
    int n;
    cin >> n;
    
    vector<int> values;
    
    for (int i=0; i<n; i++) {
      int x;
      cin >> x;
      values.push_back(x);
    }
    
    vector<vector<int>> scores(n, vector<int>(n, -1));
    cout << play(values, scores, 0, n-1, 0) << endl;
  }
}
