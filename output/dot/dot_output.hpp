#include <vector>
#include <map>

#ifndef NETWORK_H 
#define NETWORK_H 1
#include "Network.h"
#endif

class DotOutput {
  public:
  // print the unique routes
   DotOutput(std::map<int,std::vector<int> > routes, Network *net);
  ~DotOutput();
  void print_routes();

  private:
  void set_routes(std::map<int, std::vector<int> > unique_flow_routes); // set the problem routes
  void set_residence_times(std::map<int,double> residence_times);
  
  private:
  // convert an int to string
  std::string int_to_string(int integer);
  std::map<int, std::vector<int> > routes;
  std::map<int,double> residence_times;
};
