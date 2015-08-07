#include <iostream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <vector>

#include <unistd.h> // for program  options

#include "read_dot.hpp"
#include "Navigation.hpp"
#include "mcnp_sdef.hpp"

// prints the help message
void help()
{
  std::cout << "./otis <args>" << std::endl;
  std::cout << "-c <string> geometry file name" << std::endl;
  std::cout << "-i <string> input file" << std::endl;
  std::cout << "-h print thish help message" << std::endl;
  return;
}

int main(int argc, char* argv[])
{

  int opts; // options
  int start_vol,target_vol; // start and target vols
  char *net_file = NULL;   // network file
  char *input_file = NULL; // input deck
  bool help_set = false;
  int num_dirs = -1;

  while(( opts = getopt(argc, argv, "s:f:c:i:th")) != -1)
    switch (opts)
      {
        case 's':
    start_vol = std::atoi(optarg);
    break;
        case 'f':
    target_vol = std::atoi(optarg);
    break;
        case 'c':
    net_file = optarg;
    break;
        case 'i':
    input_file = optarg;
    break;
        case 'h':
    help_set = true;
    break;
        case '?':
    if (optopt == 'c' | optopt == 'm' | optopt == 's' | optopt == 'f' )
      std::cerr << "Option requires an argument" << std::endl;
    else
      std::cerr << "Unknown option character " << optopt << std::endl;
        default:
    return -1;
      }

  if ( argc == 1 | help_set)
    {
      help();
      return 0;
    }

  if(net_file == NULL)
    {
      std::cerr << "ERROR: Network file not named, please provide with -c option" << std::endl;
      return -1;
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

  std::string filename(net_file);

  //  Network *new_pipes = new Network();
  Network *new_pipes;
  std::map<int,std::vector<int> > problem_map;

  if(filename.find(".dot") != std::string::npos)
    {
      std::cout << "Using DOT Input on file, " << filename << std::endl;
      DotReader *geom = new DotReader(filename,true);
      if(!geom->check_network())
  {
    std::cout << "File, " << filename << " not found" << std::endl;
    std::cout << "Termination, with error" << std::endl;
    return 1;
  }

      new_pipes = geom->get_network();
      new_pipes->set_problem_map(geom->get_problem_map());
      problem_map = geom->get_problem_map();
      new_pipes->set_residence_times(geom->get_residence_times());
    }

  // make sure source exists
  if(problem_map.count(start_vol) == 0 )
    {
      std::cout << "Source volume " << start_vol << " doesnt exist" << std::endl;
      return 1;
    }
  // make sure target exists
  if(problem_map.count(target_vol) == 0 )
    {
      std::cout << "Target volume " << target_vol << " doesnt exist" << std::endl;
      return 1;
    }

  std::vector<int> visited;

  // new navigator
  navigation *nav = new navigation(new_pipes,start_vol);

  // do the search
  nav->DepthFirst(target_vol);

  // get the results of the search
  std::map<int,std::vector<int> > routes = nav->get_unique_routes();
  std::cout << "Found " << routes.size() << " routes" << std::endl;

  std::string control_file(input_file);
  McnpSdef *sdef = new McnpSdef(routes);
  sdef->read_control_file(control_file);
  sdef->process_spectra();
  sdef->print_sdef();

  return 0;
}
