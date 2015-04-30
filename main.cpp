#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <vector>

#include <unistd.h> // for program  options

#include "read_cgm.hpp"
#include "read_dot.hpp"
#include "read_tally.hpp"
#include "dot_output.hpp"
#include "Navigation.hpp"


// prints the help message
void help()
{
  std::cout << "./otis <args>" << std::endl;
  std::cout << "-s <int> start volume" << std::endl;
  std::cout << "-f <int> finish volume" << std::endl;
  std::cout << "-c <string> ACIS file name" << std::endl;
  std::cout << "-m <string> MCNP file name" << std::endl;
  std::cout << "-h print thish help message" << std::endl;
  std::cout << "-n no activation, just print the network map and exit" << std::endl;
  return;
}

int main(int argc, char* argv[])
{
 
  int opts; // options
  int start_vol,target_vol; // start and target vols
  char *mcnp_file = NULL, *cad_file = NULL; // strings for mcnp and cad file
  bool no_irr = false,help_set = false;

  while(( opts = getopt(argc, argv, "s:f:c:m:hn")) != -1)
    switch (opts)
      {
        case 's':
	  start_vol = std::atoi(optarg);
	  break;
        case 'f':
	  target_vol = std::atoi(optarg);
	  break;
        case 'c':
	  cad_file = optarg;
	  break;
        case 'm':
	  mcnp_file = optarg;
	  break;
        case 'h':
	  help_set = true;
	  break;
        case 'n':
          no_irr = true;
	  break;
        case '?':
	  if (optopt == 'c' | optopt == 'm' | optopt == 's' | optopt == 'f' )
	    std::cerr << "Option requires an argument" << std::endl;
	  else
	    std::cerr << "Unknown option character " << optopt << std::endl;
        default:
	  return -1;
      }

  std::cout << argc << std::endl;
  
  if ( argc == 1 | help_set)
    {
      help();
      return 0;
    }

  if(cad_file == NULL)
    {
      std::cerr << "ERROR: Network file not named, please provide with -c option" << std::endl;
      return -1;
    }

  if(!no_irr)
    {
      if(mcnp_file == NULL)
	{
	  std::cerr << "ERROR: MCNP file not named, please provide with -m option" << std::endl;
	  return -1;
	}
    }
  else
    {
      mcnp_file="";
    }

  if ( start_vol < 1 | target_vol < 1 )
    {
      std::cerr << "ERROR: Start and target volumes inconsistent" << std::endl;
      return -1;
    }
  else if ( start_vol == target_vol )
    {
      std::cerr << "ERROR: start and target volumes are the same" << std::endl;
      return -1;
    }

  std::string filename(cad_file);    
  std::string mcnpfile(mcnp_file);

  Network new_pipes;
  std::map<int,std::vector<int> > problem_map;

  if(filename.find(".sat") != std::string::npos)
    {
      ReadCGM *geom = new ReadCGM(filename,true);
      new_pipes = geom->get_network();
      problem_map = geom->get_problem_map();
    }
  if(filename.find(".dot") != std::string::npos)
    {
      DotReader *geom = new DotReader(filename,true);
      new_pipes = geom->get_network();
      problem_map = geom->get_problem_map();
    }

  std::vector<int> visited;
  visited.push_back( start_vol );
  std::cout << "Performing search..." << std::endl;
  DepthFirst( &new_pipes, visited, target_vol );

  // prints a dot graph of network connectivity & 
  // the unique routes
  print_routes(problem_map);
    
  std::string mcnp_output(argv[4]);
  // reading tallies from file
  std::map<int,tally_struct> tallies = read_tallies(mcnp_output);

  return 0;
}
