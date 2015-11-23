#include <iostream>
#include <fstream>
#include <cstdlib>
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
  srand(time(NULL));
  ofstream fout("logfile.txt");

  // Helpful data structures
  adjacency_list<router> ospfNetwork;
  set<router> routers;
  map<router, map<router, pathWithDistType> > pathsTable;

  // Build graph and save routers
  router f, t;
  while(cin >> f >> t) {
    ospfNetwork.insertEdge(f, t);
    routers.insert(f);
    routers.insert(t);
  }

  // Use Dijkstra's algorithm to update the shortest paths
  cout << "==========================Initial Network=========================="
       << endl;

  updatePaths(ospfNetwork, routers, pathsTable);
  printPaths(ospfNetwork, routers, pathsTable);

  // Simulate the network over 1000 time intervals
  // During each interval packets may be sent, the network may be changed
  // in some way (links being added, dropped, etc), or an update of a previous
  // change may be sent out.
  bool topologyChange = false;
  for(int i = 0; i < 1000; ++i) {
    if(routers.size() == 0)  {
      fout << "No routers in network; Nothing to be done." << endl;
      break;
    }

    fout << "[" << i << "]: ";

    // What's going to happen at this interval?
    int r = rand() % 100;

    // Topology has been changed; an update is needed
    if(topologyChange) {
      if(r < 25) {
        fout << "Network model finished updating from previous topology change"
             << endl;
        updatePaths(ospfNetwork, routers, pathsTable);
        topologyChange = false;
        continue;
      }
    }

    // Try to send a packet
    if(r < 85) {
      // Choose source and destn
      int sourceNum = rand() % routers.size();
      int destNum = rand() % routers.size();

      auto sourceRouter = routers.begin();
      for(int i = 0; i < sourceNum; ++i) ++sourceRouter;
      auto destRouter = routers.begin();
      for(int i = 0; i < destNum; ++i) ++destRouter;

      // Do we know of a path?
      pathWithDistType& pathWithDist = pathsTable[*sourceRouter][*destRouter];
      if(pathWithDist.first == -1) {
        fout << "Network model says no path between "<< *sourceRouter
             << " and " << *destRouter << "; Packet not sent" << endl;
      }

      // Is there actually a path?
      else {
        ospfNetwork.dijkstra(*sourceRouter);
        if(ospfNetwork.findPath(*destRouter) == -1) {
          fout << "No path from " << *sourceRouter
               << " to " << *destRouter << "; Packet lost!" << endl;
        }
        else {
          fout << "Packet successfully sent along route: ";
          for(auto i = pathWithDist.second.begin();
                   i != pathWithDist.second.end();
                   ++i) {
            fout << *i << " ";
          }
          fout << endl;
        }
      }
    }

    // Add link
    else if(r < 90) {
      // Choose from and to
      int fromNum = rand() % routers.size();
      int toNum = rand() % routers.size();

      auto fromRouter = routers.begin();
      for(int i = 0; i < fromNum; ++i) ++fromRouter;
      auto toRouter = routers.begin();
      for(int i = 0; i < toNum; ++i) ++toRouter;

      ospfNetwork.insertEdge(*fromRouter, *toRouter);

      fout << "Attempt add new link between " << *fromRouter << " and "
           << *toRouter << endl;

      topologyChange = true;
    }

    // Drop link
    else if(r < 100) {
      // Choose from and to
      int fromNum = rand() % routers.size();
      int toNum = rand() % routers.size();

      auto fromRouter = routers.begin();
      for(int i = 0; i < fromNum; ++i) ++fromRouter;
      auto toRouter = routers.begin();
      for(int i = 0; i < toNum; ++i) ++toRouter;

      ospfNetwork.removeEdge(*fromRouter, *toRouter);

      fout << "Risk of link drop between " << *fromRouter << " and "
           << *toRouter << endl;

      topologyChange = true;
    }
  }

  // Simulation done

  cout << "==========================Final Network=========================="
       << endl;
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
