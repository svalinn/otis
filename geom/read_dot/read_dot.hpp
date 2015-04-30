#include <iostream>
#include <string>

#ifndef NETWORK_H
  #define NETWORK_H 1
  #include "Network.h"
#endif

class DotReader 
{
  public:
  // Constructor
  DotReader(std::string filename, bool bidirectional);
  
  // Destructor
  ~DotReader();

  // give access to the network
  Network* get_network();

  // check the network for errors
  bool check_network();

  // return map of the problem
  std::map<int,std::vector<int> > get_problem_map();

  // Read the complete file
  private:
  void read_dot_file(std::string filename);
 
  // process the link data
  void process_link(std::string line);

  // process the node data
  void process_node(std::string line);

  // tokenize a string
  std::vector<std::string> tokenise(std::string line, char delimiter);


  // private variables
  private:
  Network *nw; // the pipe network
  std::map<int,double> residence_times; // map of volume id and residence times
  std::map<int,std::vector<int> > problem_map;
};

