/*
  adjacency_list.tpp
  David Vaughan
*/

#include <queue>
#include <utility>
#include <algorithm>
#include <set>
#include "adjacency_list.h"

// Default constructor
template <typename dataType, typename weightType>
adjacency_list<dataType, weightType>::
adjacency_list()
  : size(0), weighted(false), negativeWeights(false) {
}

// Copy constructor
template <typename dataType, typename weightType>
adjacency_list<dataType, weightType>::
adjacency_list(const adjacency_list& other)
  : edges(other.edges) {
  size = edges.size();
}

// Destructor
template <typename dataType, typename weightType>
adjacency_list<dataType, weightType>::
~adjacency_list() {
  // Nothing needed here right now
}

// Assignment operator
template <typename dataType, typename weightType>
adjacency_list<dataType, weightType>& adjacency_list<dataType, weightType>::
operator=(adjacency_list other) {
  swap(edges, other.edges);
  size = edges.size();
}

// Insert vertex
template <typename dataType, typename weightType>
void adjacency_list<dataType, weightType>::
insertVertex(const dataType& newVertex) {
  // Add a new vertex to the map and the vector
  edges[newVertex] = vector<edge>();
}

// Insert undirected, unweighted edge between to edges
template <typename dataType, typename weightType>
void adjacency_list<dataType, weightType>::
insertEdge(const dataType& vertexA, const dataType& vertexB) {
  // Call the weighted overload of insert edge with weight 1
  insertEdge(vertexA, vertexB, 1);
}

// Insert undirected, weighted edge
template <typename dataType, typename weightType>
void adjacency_list<dataType, weightType>::
insertEdge(const dataType& vertexA, const dataType& vertexB,
         const weightType& weight) {
  // Call the directed, weighted insert function from each vertex to the other
  insertDirectedEdge(vertexA, vertexB, weight);
  insertDirectedEdge(vertexB, vertexA, weight);
}

// Insert directed, unweighted edge
template <typename dataType, typename weightType>
void adjacency_list<dataType, weightType>::
insertDirectedEdge(const dataType& vertexA, const dataType& vertexB) {
  // Call the directed, weighted insert function, with weight 1
  insertDirectedEdge(vertexA, vertexB, 1);
}

// Insert directed, weighted edge
template <typename dataType, typename weightType>
void adjacency_list<dataType, weightType>::
insertDirectedEdge(const dataType& vertexA, const dataType& vertexB,
             const weightType& weight) {
  // Insert the edges if they don't yet exist
  if(!edges.count(vertexA)) insertVertex(vertexA);
  if(!edges.count(vertexB)) insertVertex(vertexB);

  // Update "weighted" and "negativeWeights" flags
  if(weight != 1) weighted = true;
  if(weight < 0) negativeWeights = true;

  // Push the edge into adjacency list vector
  edges[vertexA].push_back(edge(weight, vertexB));
}

// Dijkstra's SSSP algorithm
template <typename dataType, typename weightType>
void adjacency_list<dataType, weightType>::
dijkstra(const dataType& source) {
  // Set the source
  this->source = source;

  // Initialize helper data structures
  priority_queue<edge, vector<edge>, greater<edge> > dijkstraQueue;

  dist.clear();
  for(auto i = edges.begin(); i != edges.end(); ++i)
    dist[i->first] = INT_MAX;

  set<dataType> visited;
  parent.clear();

  dijkstraQueue.push(edge(0, source));
  visited.insert(source);
  dist[source] = 0;

  // Greedily relax all the edges in order of ascending distance
  while(!dijkstraQueue.empty()) {
    dataType vertex = dijkstraQueue.top().second;
    weightType distance = dijkstraQueue.top().first;
    dijkstraQueue.pop();

    // Relax edges
    for(auto i = edges[vertex].begin(); i != edges[vertex].end(); ++i)
      if(!visited.count(i->second) && distance + i->first < dist[i->second]) {
        dist[i->second] = distance + i->first;
        dijkstraQueue.push(edge(distance + i->first, i->second));
        visited.insert(i->second);

        // Keep track of the shortest path
        parent[i->second] = vertex;
      }
  }
}

// Trace a path from the saved source to a destination
template <typename dataType, typename weightType>
weightType adjacency_list<dataType, weightType>::
findPath(const dataType& dest, vector<dataType>* path) {
  // We assume a source has already been chosen and dijkstra has been run
  if(parent.empty()) return weightType(-1);

  // Trace back the path
  if(path) {
    path->clear();
    dataType currentVertex = dest;
    path->push_back(currentVertex);
    while(currentVertex != source) {
      currentVertex = parent[currentVertex];
      path->push_back(currentVertex);
    }

    // Path is reversed right now
    reverse(path->begin(), path->end());
  }

  return dist[dest];
}
