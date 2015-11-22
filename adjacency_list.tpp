/*
  adjacency_list.tpp
  David Vaughan
*/

#include <queue>
#include <utility>
#include <algorithm>
#include "adjacency_list.h"

// Default constructor
template <typename dataType, typename weightType>
adjacency_list<dataType, weightType>::
adjacency_list()
  : size(0), weighted(false), negativeWeights(false) {
  capacity = vertices.capacity();
}

// Copy constructor
template <typename dataType, typename weightType>
adjacency_list<dataType, weightType>::
adjacency_list(const adjacency_list& other)
  : vertices(other.vertices), indexMap(other.indexMap) {
  size = vertices.size();
  capacity = vertices.capacity();
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
  swap(vertices, other.vertices);
  swap(indexMap, other.indexMap);
  size = vertices.size();
  capacity = vertices.capacity();
}

// Insert vertex
template <typename dataType, typename weightType>
void adjacency_list<dataType, weightType>::
insertVertex(const dataType& newVertex) {
  // Add a new vertex to the map and the vector
  indexMap[newVertex] = size++;
  vertices.push_back(vector<edge>());
}

// Insert undirected, unweighted edge between to vertices
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
  // Insert the vertices if they don't yet exist
  if(!indexMap.count(vertexA)) insertVertex(vertexA);
  if(!indexMap.count(vertexB)) insertVertex(vertexB);

  // Update "weighted" and "negativeWeights" flags
  if(weight != 1) weighted = true;
  if(weight < 0) negativeWeights = true;

  // Push the edge into adjacency list vector
  vertices[indexMap[vertexA]].push_back(edge(vertexB, weight));
}

// Dijkstra's SSSP algorithm
template <typename dataType, typename weightType>
weightType adjacency_list<dataType, weightType>::
dijkstra(const dataType& vertexA, const dataType& vertexB) {
  // Initialize priority queue
  typedef pair<weightType, dataType> edge;
  priority_queue<edge, vector<edge>, greater<edge> > dijkstraQueue;

  vector<bool> visited(size, false);
  vector<weightType>dist(size, weightType(INT_MAX));
  dist[indexMap[vertexA]] = 0;

  dijkstraQueue.push(edge(0, vertexA));

  // Greedily relax all the vertices in order of ascending distance
  while(!dijkstraQueue.empty()) {
    dataType vertex = dijkstraQueue.top().second;
    weightType distance = dijkstraQueue.top().first;

    int index = indexMap[vertex];
    dijkstraQueue.pop();

    // If already done, skip
    if(visited[index] || dist[index] < distance)
      continue;

    visited[index] = true;

    // Check if it's the target
    if(vertex == vertexB)
      return distance;

    // Relax edges
    for(auto i = vertices[index].begin(); i != vertices[index].end(); ++i)
      if(distance + i->weight < dist[indexMap[i->vertex]]) {
        dist[indexMap[i->vertex]] = distance + i->weight;
        dijkstraQueue.push(edge(distance + i->weight, i->vertex));
      }
  }
}

// Dijkstra's with path recording
template <typename dataType, typename weightType>
weightType adjacency_list<dataType, weightType>::
dijkstra(const dataType& vertexA, const dataType& vertexB,
         vector<dataType>& pathVector) {
}
