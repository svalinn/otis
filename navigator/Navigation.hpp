#ifndef NETWORK_H
#define NETWORK_H 1
#include "Network.h"
#endif

class navigation {
  public:
  // new navigator
  navigation(Network *network, int source);
  // destructor
  ~navigation();

  // return the unique routes
  std::map<int,std::vector<int> > get_unique_routes();
  // print the routes
  void print_routes();

  // does the 
  bool ContainsNode( std::vector<int>& nodes, int node );
  // search the network
  void DepthFirst( int end );
  private:
  Network *ntwk;
  std::map<int,std::vector<int> > routes;
  std::vector<int> visited;
  int route_counter;
};
