#include <iostream>
#include "adjacency_list.h"
using namespace std;

int main() {
  adjacency_list<int> graph;

  graph.insertEdge(0, 1);
  graph.insertEdge(1, 2);
  graph.insertEdge(2, 3);
  graph.insertEdge(3, 4);
  graph.insertEdge(4, 5);

  vector<int> path;
  int dist = graph.dijkstra(0, 5, &path);

  cout << dist << endl;

  for(auto i = path.begin(); i != path.end(); ++i)
    cout << *i << " ";
  cout << endl;

  return 0;
}
