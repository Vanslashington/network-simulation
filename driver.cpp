#include <iostream>
#include <set>
#include <map>
#include <string>
#include "adjacency_list.h"
using namespace std;

// Types for routers and paths
typedef string router;
typedef vector<router> pathType;
typedef pair<int, pathType> pathWithDistType;

// OSPF path functions
void updatePaths(adjacency_list<router>& ospfNetwork,
                 const set<router>& routers,
                 map<router, map<router, pathWithDistType> >& pathsTable);

void printPaths(const adjacency_list<router>& ospfNetwork,
                const set<router>& routers,
                map<router, map<router, pathWithDistType> >& pathsTable);

// main
int main() {
  adjacency_list<router> ospfNetwork;
  set<router> routers;

  map<router, map<router, pathWithDistType> > pathsTable;

  // Build graph and record routers
  router f, t;
  while(cin >> f >> t) {
    ospfNetwork.insertEdge(f, t);
    routers.insert(f);
    routers.insert(t);
  }

  // Simulate the network over 1000 time intervals
  // During each interval packets may be sent, or the network may be changed
  // in some way (routers being added, routers or links being dropped, etc).
  //for(int i = 0; i < 1000; ++i) {
  //}

  updatePaths(ospfNetwork, routers, pathsTable);
  printPaths(ospfNetwork, routers, pathsTable);

  // Remove a vertex and see what happens
  ospfNetwork.removeVertex(*(routers.begin()));
  routers.erase(routers.begin());

  updatePaths(ospfNetwork, routers, pathsTable);
  printPaths(ospfNetwork, routers, pathsTable);

  return 0;
}

void updatePaths(adjacency_list<router>& ospfNetwork,
                 const set<router>& routers,
                 map<router, map<router, pathWithDistType> >& pathsTable) {
  pathsTable.clear();

  // Discover shortest path trees from every router
  for(auto source = routers.begin(); source != routers.end(); ++source) {
    ospfNetwork.dijkstra(*source);
    vector<router> path;

    // Traverse the shortest paths to every destination
    for(auto destn = routers.begin(); destn != routers.end(); ++destn) {
      int dist = ospfNetwork.findPath(*destn, &path);
      pathsTable[*source][*destn] = pathWithDistType(dist, path);
    }
  }
}

void printPaths(const adjacency_list<router>& ospfNetwork,
                const set<router>& routers,
                map<router, map<router, pathWithDistType> >& pathsTable) {
  // From every source...
  for(auto source = routers.begin(); source != routers.end(); ++source) {
    cout << "============================" << endl;
    cout << "Source: " << *source << endl;

    // ...to every destination
    for(auto destn = routers.begin(); destn != routers.end(); ++destn) {
      cout << "Destination: " << *destn << endl;
      pathWithDistType pathWithDist = pathsTable[*source][*destn];
      pathType path = pathWithDist.second;
      int dist = pathWithDist.first;

      cout << "Shortest path distance: " << dist << endl;
      cout << "Shortest path:" << endl;
      for(auto i = path.begin(); i != path.end(); ++i)
        cout << *i << " ";
      cout << endl << endl;
    }
  }
}
