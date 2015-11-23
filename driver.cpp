#include <iostream>
#include <set>
#include <map>
#include <string>
#include "adjacency_list.h"
using namespace std;

// Define type to represent routers
typedef string router;
typedef vector<router> pathType;
typedef pair<int, pathType> pathWithDistType;

void updatePaths(adjacency_list<router>& ospfNetwork,
                 const set<router>& routers,
                 map<router, map<router, pathWithDistType> >& pathsTable);

void printPaths(const adjacency_list<router>& ospfNetwork,
                const set<router>& routers,
                map<router, map<router, pathWithDistType> >& pathsTable);

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

  // Loop through k time intervals
  // For each interval
  //  Send packet, or
  //  Change network

  updatePaths(ospfNetwork, routers, pathsTable);
  printPaths(ospfNetwork, routers, pathsTable);

  return 0;
}

void updatePaths(adjacency_list<router>& ospfNetwork,
                 const set<router>& routers,
                 map<router, map<router, pathWithDistType> >& pathsTable) {
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
  // Discover shortest path trees from every router
  for(auto source = routers.begin(); source != routers.end(); ++source) {
    // Traverse the shortest paths to every destination
    cout << "============================" << endl;
    cout << "Source: " << *source << endl;
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
