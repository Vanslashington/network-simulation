#include <iostream>
#include <set>
#include "adjacency_list.h"
using namespace std;

int main() {
  adjacency_list<int> ospfNetwork;

  set<int> vertices;

  int f, t;
  while(cin >> f >> t) {
    ospfNetwork.insertEdge(f, t);
    vertices.insert(f);
    vertices.insert(t);
  }

  ospfNetwork.dijkstra(0);

  vector<int> path;

  for(auto v = vertices.begin(); v != vertices.end(); ++v) {
    int dist = ospfNetwork.findPath(*v, &path);

    cout << dist << endl;

    for(auto i = path.begin(); i != path.end(); ++i)
      cout << *i << " ";
    cout << endl << endl;
  }

  return 0;
}
