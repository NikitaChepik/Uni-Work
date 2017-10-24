#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

namespace gdwg {
  
  class Edge;
  class Node;

  template<typename N, typename E> 
  class Graph {
  public:
    Graph() {}
    Graph(const Graph &g);
    Graph(Graph &&g);

    Graph<N,E>& operator=(Graph &&g);
    Graph<N,E>& operator=(const Graph &g);

    void begin() const { it = graph_.begin(); }
    bool end() const {if(it == graph_.end()){return true;}return false;}
    void next() const {++it;}
    const N& value() const{Node n = *it; return n.getNode();}; 

    bool addNode(const N& node);
    bool isNode(const N& val) const;
    void printNodes() const;
    void deleteNode(const N&) noexcept;
    
    bool addEdge(const N& src, const N& dest, const E& w);
    void printEdges(const N& src);
    void deleteEdge(const N& src, const N& dst, const E& w) noexcept;
    
    bool isConnected(const N& src, const N& dst) const;
    bool replace(const N& oldData, const N& newData);
    void mergeReplace(const N& oldData, const N& newData);
    void clear() noexcept;

  private:
    class Node{
    class Edge;
    public:
      Node(const N& node) : node_{std::make_shared<N>(node)} {}
      const N& getNode() const {return *node_;}
      std::shared_ptr<N> getNodePtr() const { return node_; }
      const std::vector<Edge>& getEdgeVector() const { return edge_; }
      void printEdges();
      void addEdge(const Node& dest, const E& w);
      bool checkEdges(const N& dest, const E& w);
      bool checkEdges(const N& dest);
      void deleteEdge(const N& dst, const E& w);
      int countEdges() const;
      void copyEdges(const Node& oldData, const std::vector<Node> nodes);
    private:
      class Edge {
      public:
        Edge(const Node& src, const E& weight) : edgeLink_{src.getNodePtr()},weight_{std::make_shared<E>(weight)} {}
        const N& getToEdge() const {auto p = edgeLink_.lock();return *p;}
        std::shared_ptr<N> getToEdgePtr() const { auto p = edgeLink_.lock();return p; }
        const E& getWeight() const {return *weight_;}
        void printEdgeInfo();
      private:
        std::weak_ptr<N> edgeLink_;
        std::shared_ptr<E> weight_;
      };
      std::shared_ptr<N> node_;
      std::vector<Edge> edge_;
    };
    std::vector<Node> graph_;
    mutable typename std::vector<Node>::const_iterator it;
  };
  
  #include "Graph.tem"
}

#endif