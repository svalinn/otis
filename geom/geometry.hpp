#include <iostream>
#include <string>
#include <set>

#ifndef NETWORK_H
#define NETWORK_H 1
#include "Network.h"
#endif

class Geometry {
  public:
    // Constructor
    Geometry(std::string filename, bool bidirectional);
    Geometry();
   ~Geometry();
    // give access to the network
    Network* get_network();
    // return map of the problem
    std::map<int,std::vector<int> > get_problem_map();
    // return the residence times
    std::map<int,double> get_residence_times();
    // check the network for errors
    bool check_network();

    void set_network(Network *net);
    void set_residence_times(std::map<int,double> residence_times);
    void set_problem_map(std::map<int,std::vector<int> > problem_map);

  protected:
    Network *nw; // the pipe network
    std::map<int,double> residence_times; // map of volume id and residence times
    std::map<int,std::vector<int> > problem_map;
    std::set<int> members; // collection of the ids of all the problm members
};
