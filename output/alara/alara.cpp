#include "alara.hpp"

// constructor
AlaraOutput::AlaraOutput(std::map<int, std::vector<int> > routes,
			 std::string mcnp_filename,
			 std::string filename)
{
  set_filename(filename);
  set_routes(routes);
  set_mcnp_filename(mcnp_filename);

  // make sure the file exists
  if(!file_exists(mcnp_filename))
    {
      std::cout << "MCNP output file does not exist" << std::endl;
    }

  // read the tally data from file
  std::map<int, tally_struct> tally_data = read_tallies(mcnp_filename);
  // set the tally data
  set_tallies(tally_data);

  // check the class for consistency
  if(!check_consistency())
    {
      std::cout << "There are inconsistencies in the output" << std::endl;
    }
}

// default constructor
AlaraOutput::AlaraOutput()
{
}

// destructor
AlaraOutput::~AlaraOutput()
{
}

// set the filename
void AlaraOutput::set_filename(std::string filename)
{
  // set the class output filename
  output_file = filename;
}

// get the filename
std::string AlaraOutput::get_filename()
{
  return output_file;
}

// set the filename
void AlaraOutput::set_mcnp_filename(std::string filename)
{
  // set the class output filename
  mcnp_filename = filename;
}

// get the filename
std::string AlaraOutput::get_mcnp_filename()
{
  return mcnp_filename;
}


// set the tallies
void AlaraOutput::set_tallies(std::map<int, tally_struct> tally_input)
{
  // set the tally data
  problem_tallies = tally_input;
}

// set the routes
void AlaraOutput::set_routes(std::map<int, std::vector<int> > flow_routes )
{
  // set the route data
  routes = flow_routes;
}

// print out the unique alara fluxes file
void AlaraOutput::write_alara_fluxes()
{
  // loop over the problem map calling print_tally for each volume
  // write out to unique fluxes file
  std::map<int,std::vector<int> >::iterator it;
  std::vector<int>::iterator path_it;      

  // writes each flux spectra encountered along the route to a unique file
  for ( it = routes.begin() ; it != routes.end() ; ++it )
    {
      std::ofstream myfile;
      myfile.open(output_file+std::to_string(it->first));
      // dump each route to a seperate flux file
      for ( path_it = it->second.begin() ; path_it != it->second.end() ; ++path_it )
	{
	  //std::cout << *path_it << std::endl;
	  print_tally(problem_tallies,*path_it,myfile);
	}
      myfile.close();
    }
  return;
}

// write the alara input deck for each route
void AlaraOutput::write_alara_input()
{
}

// check to see if the file exists
bool AlaraOutput::file_exists(const std::string &fname)
{
  return ( access( fname.c_str(), F_OK ) != -1 );
}

// check that the loaded mcnp file and route network is
// all consistent
bool AlaraOutput::check_consistency()
{
  // route iterator
  std::map<int, std::vector<int> > :: iterator route_it;
  
  // loop over all routes
  for ( route_it = routes.begin() ; route_it != routes.end() ; ++route_it )
    {
      std::vector<int> :: iterator member;
      // loop over each volume in the route
      for ( member = (route_it->second).begin() ; member != (route_it->second).end() ; ++member )
	{
	  // if we do not have the tally present
	  if ( problem_tallies.count(*member) == 0 )
	    return false;
	}
    }

  // if we are here then every route member has a spectrum
  return true;
}
