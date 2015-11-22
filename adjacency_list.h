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
  void insertEdge(const dataType& vertexA, const dataType& vertexB);
  void insertEdge(const dataType& vertexA, const dataType& vertexB,
                  const weightType& weight);
  void insertDirectedEdge(const dataType& vertexA, const dataType& vertexB);
  void insertDirectedEdge(const dataType& vertexA, const dataType& vertexB,
                          const weightType& weight);

  // SSSP
  weightType dijkstra(const dataType& vertexA, const dataType& vertexB);
  weightType dijkstra(const dataType& vertexA, const dataType& vertexB,
                      vector<dataType>& pathVector);
 private:
  // Edge struct to store links and weights
  struct edge {
    edge(dataType _vertex, weightType _weight)
      : vertex(_vertex), weight(_weight) {}
    dataType vertex;
    weightType weight;
  };

  // 2D vector to store adjacent vertices
  vector<vector<edge> > vertices;

  // Map from dataType to vector indices
  map<dataType, int> indexMap;

  // Other members
  int capacity;
  int size;
  bool weighted;
  bool negativeWeights;
};

// Implementation file
#include "adjacency_list.tpp"

#endif
