/*
  adjacency_list.h
  David Vaughan
*/

#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <vector>
#include <map>
#include <climits>

using namespace std;

// Adjacency List class for graph problems
template <typename dataType, typename weightType = int>
class adjacency_list {
 public:
  // Constructors and destructor
  adjacency_list();
  adjacency_list(const adjacency_list& other);
  ~adjacency_list();

  // Assignment operator
  adjacency_list& operator=(adjacency_list other);

  // Insert edges and vertices
  void insertVertex(const dataType& newVertex);
  void insertEdge(const dataType& vertexA, const dataType& vertexB,
                  const weightType& weight = 1);
  void insertDirectedEdge(const dataType& vertexA, const dataType& vertexB,
                          const weightType& weight = 1);

  // SSSP
  void dijkstra(const dataType& source);
  weightType findPath(const dataType& dest, vector<dataType>* path = NULL);

 private:
  // Edge type to store links and weights
  typedef pair<weightType, dataType> edge;

  // 2D vector to store adjacent vertices
  map<dataType, vector<edge> > edges;

  // Distances and parents for dijkstra SSSP tree
  map<dataType, weightType> dist;
  map<dataType, dataType> parent;

  // Other members
  dataType source; // SSSP source
  int size;
  bool weighted;
  bool negativeWeights;
};

// Implementation file
#include "adjacency_list.tpp"

#endif
