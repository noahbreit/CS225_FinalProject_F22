#include "Edge.h"

 Edge::Edge(unsigned idx, unsigned start_node_idx, unsigned end_node_idx, double weight, Edge * next){
  _idx = idx;
  _start_node_idx = start_node_idx;
  _end_node_idx = end_node_idx;
  _weight = weight;
  _next = next;
}

Edge::Edge(const Edge & other){
  _idx = other._idx;
  _start_node_idx = other._start_node_idx;
  _end_node_idx = other._end_node_idx;
  _weight = other._weight;
  _next = other._next;
}

Edge::~Edge(){
  /* mark */
  //std::cout << "Line : " << __LINE__ << std::endl;

    if(!this || !_next) return;
    _next->~Edge();
    delete _next;
    _next = NULL;
}


