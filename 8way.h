// Malik Saafir 2019

#include <cmath>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

/* This code uses the A* algorithm to find the optimal solution to a valid 8 way puzzle configuration. */

/* Nodes are defined as structs with board configuration, parent and children pointers, and evaluation scores. */
struct Node {
  int f_score, g_score, h_score;
  vector<int> state;
  vector<Node*> children;
  Node* parent;
};

/* This class is used to make the priority queue use minimum-first ordering. */
class CompareNodes {
  public:
    int operator() (const Node* n1, const Node* n2)
    {
        return n1->f_score > n2->f_score;
    }
};

void print_state(Node* node);

class EightWayPuzzle {
 private:
  Node* initial;
  priority_queue <Node*, vector<Node*>, CompareNodes> pq;
  bool isGoal(Node* node);
  bool solvable(Node* node);
  int getHeuristic(Node* node);
  void expand(Node* node);
 public:
  EightWayPuzzle();
  void solve();
};

EightWayPuzzle::EightWayPuzzle() {
  initial = new Node;
  initial->parent = nullptr;
  int n;
  do {
    cout << "enter start state: ";
    for (int i = 0; i < 9; i++) {
      do {
        cin >> n;
      } while (n < 0 || n > 8);
      initial->state.push_back(n);
    }
    if (!solvable(initial)) {
      cout << "not a solvable start state\n";
    }
  } while (!solvable(initial));

  initial->g_score = 0;
  initial->h_score = getHeuristic(initial);
  initial->f_score = initial->h_score;
}

bool EightWayPuzzle::solvable(Node* node) {
  int inversions = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = i + 1; j < 9; j++) {
      if (node->state[i] != 0 && node->state[j] != 0 && node->state[i] > node->state[j]) {
        inversions++;
      }
    }
  }
  return inversions % 2 == 0;
}

/* Checks if the puzzle has been solved. */
bool EightWayPuzzle::isGoal(Node* node) {
  for (int i = 0; i < 9; i++) {
    if (node->state[i] != i) {
      return false;
    }
  }
  return true;
}

/* The heuristic evaluation function uses the Manhattan distance formula to compute the total Manhattan distance for all the numbers on the board (excluding the empty space). */
int EightWayPuzzle::getHeuristic(Node* node) {
  int manhattan_total = 0;
  int x_dist, y_dist, x_pos, y_pos, n;
  for (int i = 0; i < 9; i++) {
    n = node->state[i];
    if (n == 0) { continue; }
    x_pos = i % 3;
    y_pos = i / 3;
    x_dist = abs(x_pos - n % 3);
    y_dist = abs(y_pos - n / 3);
    manhattan_total += x_dist + y_dist;
  }
  return manhattan_total;
}

/* The expand function expands the next node from the priority queue after popping it off the top. A node is expanded by pushing every possible shift from that state to the priority queue, which is ordered by f score evaluation. The child nodes are also added to the parent node's children vector. */
void EightWayPuzzle::expand(Node* parent) {
  pq.pop();
  int blank_pos = 0;
  while (parent->state[blank_pos] != 0) {
    blank_pos++;
  }
  // shift blank left
  if (blank_pos % 3 != 0) {
    Node* child = new Node;
    child->parent = parent;
    child->state = parent->state;
    int temp = child->state[blank_pos - 1];
    child->state[blank_pos - 1] = 0;
    child->state[blank_pos] = temp;
    child->g_score = parent->g_score + 1;
    child->h_score = getHeuristic(child);
    child->f_score = child->g_score + child->h_score;
    parent->children.push_back(child);
    pq.push(child);
  }
  // shift blank right
  if (blank_pos % 3 != 2) {
    Node* child = new Node;
    child->parent = parent;
    child->state = parent->state;
    int temp = child->state[blank_pos + 1];
    child->state[blank_pos + 1] = 0;
    child->state[blank_pos] = temp;
    child->g_score = parent->g_score + 1;
    child->h_score = getHeuristic(child);
    child->f_score = child->g_score + child->h_score;
    parent->children.push_back(child);
    pq.push(child);
  }
  // shift blank up
  if (blank_pos / 3 != 0) {
    Node* child = new Node;
    child->parent = parent;
    child->state = parent->state;
    int temp = child->state[blank_pos - 3];
    child->state[blank_pos - 3] = 0;
    child->state[blank_pos] = temp;
    child->g_score = parent->g_score + 1;
    child->h_score = getHeuristic(child);
    child->f_score = child->g_score + child->h_score;
    parent->children.push_back(child);
    pq.push(child);
  }
  // shift blank down
  if (blank_pos / 3 != 2) {
    Node* child = new Node;
    child->parent = parent;
    child->state = parent->state;
    int temp = child->state[blank_pos + 3];
    child->state[blank_pos + 3] = 0;
    child->state[blank_pos] = temp;
    child->g_score = parent->g_score + 1;
    child->h_score = getHeuristic(child);
    child->f_score = child->g_score + child->h_score;
    parent->children.push_back(child);
    pq.push(child);
  }
}

void print_state(Node* node) {
  cout << endl;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      int n = node->state[i * 3 + j];
      if (n == 0) {
        cout << "  ";
      } else {
        cout << " " << n;
      }
      if (j != 2) {
        cout << " |";
      }
    }
    if (i != 2) {
      cout << "\n-----------";
    }
    cout << endl;
  }
}

/* The solve function initiates the pathfinding. Once the algorithm reaches the solution it traces the chosen path back up to the initial state through the parent pointers. It then prints the path it found from beginning to end and outputs the length. */
void EightWayPuzzle::solve() {
  Node* current = initial;
  pq.push(initial);
  while (!isGoal(pq.top())) {
    expand(pq.top());
  }
  vector<Node*> final_path;
  current = pq.top();
  while (current) {
    final_path.insert(final_path.begin(), current);
    current = current->parent;
  }
  for (Node* node : final_path) {
    print_state(node);
  }
  cout << "optimal path has length " << final_path.size() - 1 << endl;
}
