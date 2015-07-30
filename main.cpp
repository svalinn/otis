#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <vector>

#include <unistd.h> // for program  options

#include "read_cgm.hpp"
#include "read_dot.hpp"
#include "dot_output.hpp"
#include "Navigation.hpp"
#include "alara.hpp"

// prints the help message
void help()
{
  std::cout << "./otis <args>" << std::endl;
  std::cout << "-s <int> start volume" << std::endl;
  std::cout << "-f <int> finish volume" << std::endl;
  std::cout << "-c <string> ACIS file name" << std::endl;
  std::cout << "-m <string> MCNP file name" << std::endl;
  std::cout << "-h print thish help message" << std::endl;
  std::cout << "-n set the normalisation" << std::endl;
  std::cout << "-p set the number of pulses" << std::endl;
  std::cout << "-d set the delay between pulses" << std::endl;
  std::cout << "-t no activation, just print the network map and exit" << std::endl;
  return;
}

int main(int argc, char* argv[])
{
 
  int opts; // options
  int start_vol,target_vol; // start and target vols
  char *mcnp_file = NULL, *cad_file = NULL; // strings for mcnp and cad file
  bool no_irr = false,help_set = false;
  char *normalisation;
  int num_pulses = -1;
  char *pulse_delay;

  while(( opts = getopt(argc, argv, "s:f:c:m:n:p:d:th")) != -1)
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
	  normalisation = optarg;
	  break;
        case 'p':
	  num_pulses = std::atoi(optarg);
	  break;
        case 'd':
	  pulse_delay = optarg;
	  break;
        case 't':
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

  if(normalisation == NULL)
    {
      std::cerr << "ERROR: Neutron normalisation not set" << std::endl;
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

  if ( num_pulses < 0 )
    {
      std::cerr << "ERROR: you need to set the number of pulses" << std::endl;
      return -1;
    }

  if ( pulse_delay == NULL )
    {
      std::cerr << "ERROR: you need to set the delay between pulses" << std::endl;
      return -1;
    }

  std::string filename(cad_file);    
  std::string mcnpfile(mcnp_file);
  std::string source_strength(normalisation);
  std::string delay_time(pulse_delay);

  //  Network *new_pipes = new Network();
  Network *new_pipes;
  std::map<int,std::vector<int> > problem_map;
  if(filename.find(".sat") != std::string::npos)
    {
      std::cout << "Using CGM Input" << std::endl;

      ReadCGM *geom = new ReadCGM(filename,true);
      if(!geom->check_network())
	{
	  std::cout << "Termination, with error" << std::endl;
	  return 1;
	}
      new_pipes = geom->get_network();
      new_pipes->set_problem_map(geom->get_problem_map());
      problem_map = geom->get_problem_map();
      new_pipes->set_residence_times(geom->get_residence_times());
      new_pipes->set_surface_areas(geom->get_surface_areas());
    }

  if(filename.find(".dot") != std::string::npos)
    {
      std::cout << "Using DOT Input" << std::endl;
      DotReader *geom = new DotReader(filename,true);
      if(!geom->check_network())
	{
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
  std::cout << "Performing search..." << std::endl;

  // new navigator
  navigation *nav = new navigation(new_pipes,start_vol);
  
  // do the search
  nav->DepthFirst(target_vol);

  // get the results of the search
  std::map<int,std::vector<int> > routes = nav->get_unique_routes();
  std::cout << "Found " << routes.size() << " routes" << std::endl;

  // modify the network according to the boundary areas
  new_pipes->modify_residence_times(routes);
  
  // prints a dot graph of network connectivity & 
  // the unique routes
  DotOutput *dot = new DotOutput(routes,new_pipes);
  dot->print_routes();

  // if we want just the network
  if(no_irr)
    return 0;
  
  // new alara output instance
  AlaraOutput *alara = new AlaraOutput(routes, mcnp_file,
				       "fluxes", new_pipes, source_strength,
				       delay_time,num_pulses);
  // write the flux and input
  alara->write_alara_fluxes();
  alara->write_alara_input();
  alara->write_alara_isolib();

  return 0;
}
