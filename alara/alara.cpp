#include "alara.hpp"

// print out the unique alara fluxes file
void print_alara(std::string filename, std::map<int,tally_struct> tallies, 
		 std::map<int,std::vector<int> > problem_map)
{
  // loop over the problem map calling print_tally for each volume
  // write out to unique fluxes file
  std::map<int,std::vector<int> >::iterator it;
  std::vector<int>::iterator path_it;      


  for ( it = problem_map.begin() ; it != problem_map.end() ; ++it )
    {
      std::ofstream myfile;
      myfile.open("flux_file"+std::to_string(it->first));
      // dump each route to a seperate flux file
      for ( path_it = it->second.begin() ; path_it != it->second.end() ; ++path_it )
	{
	  //std::cout << *path_it << std::endl;
	  print_tally(tallies,*path_it,myfile);
	}
      myfile.close();
    }
  return;
}

