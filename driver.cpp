#include <iostream>
#include <set>
#include <string>
#include "adjacency_list.h"
using namespace std;

// Define type to represent routers
typedef string router;

int main() {
  // Build graph and record routers
  adjacency_list<router> ospfNetwork;
  set<router> routers;

  router f, t;
  while(cin >> f >> t) {
    ospfNetwork.insertEdge(f, t);
    routers.insert(f);
    routers.insert(t);
  }

  // Discover shortest path trees from every router
  for(auto source = routers.begin(); source != routers.end(); ++source) {
    ospfNetwork.dijkstra(*source);
    vector<router> path;

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
