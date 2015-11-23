#include <iostream>
#include <set>
#include "adjacency_list.h"
using namespace std;

int main() {
  adjacency_list<int> ospfNetwork;
  set<int> routers;

  int f, t;
  while(cin >> f >> t) {
    ospfNetwork.insertEdge(f, t);
    routers.insert(f);
    routers.insert(t);
  }

  // Discover shortest path trees from every router
  for(auto source = routers.begin(); source != routers.end(); ++source) {
    ospfNetwork.dijkstra(*source);
    vector<int> path;

    // Traverse the shortest paths to every destination
    cout << "============================" << endl;
    cout << "Source: " << *source << endl;
    for(auto destn = routers.begin(); destn != routers.end(); ++destn) {
      cout << "Destination: " << *destn << endl;
      int dist = ospfNetwork.findPath(*destn, &path);

      cout << "Shortest path distance: " << dist << endl;
      cout << "Shortest path:" << endl;
      for(auto i = path.begin(); i != path.end(); ++i)
        cout << *i << " ";
      cout << endl << endl;
    }
  }

  return 0;
}
