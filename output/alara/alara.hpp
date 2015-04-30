#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <set>


#include "read_tally.hpp"
/*
struct tally_struct {
  int entity_number;
  double entity_size;
  std::vector<double> ebins;
  std::vector<double> result;
  std::vector<double> error;
};
*/

void print_alara(std::string filename, std::map<int,tally_struct> tallies, 
		 std::map<int,std::vector<int> > problem_map);
