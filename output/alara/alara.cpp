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
  std::cout << mcnp_filename << std::endl;
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

void AlaraOutput::write_alara_header(std::ofstream &alara_file)
{
  // write the alara_input deck
  alara_file << "## $Id: sample5,v 1.3 2004-09-21 19:45:03 wilsonp Exp $" << std::endl;
  alara_file << "    ## Sample 4: Advanced" << std::endl;
  alara_file << "    ## Highlights: rectangular geometry with volumes, isotopic changes in new" << std::endl;
  alara_file << "    ## element library, spatial norm, photon source output, changes in tolerance" << std::endl;
  alara_file << "## and impurity settings" << std::endl;
  alara_file << "##" << std::endl;
  alara_file << "    ## NOTE: you should run sample1 before any other samples to ensure" << std::endl;
  alara_file << "## that the data is available" << std::endl;
  alara_file << "##" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "geometry rectangular" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "volumes" << std::endl;
  alara_file << "    125.0zone_1" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "mat_loading" << std::endl;
  alara_file << "    zone_1mix_1" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "spatial_norm" << std::endl;
  alara_file << "    1" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "material_lib matlib" << std::endl;
  alara_file << "element_lib new_isolib" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "mixture mix_1" << std::endl;
  alara_file << "    material co 1 1 " << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
}

void AlaraOutput::write_alara_footer(std::ofstream &alara_file)
{
  alara_file << "pulsehistory steady_state" << std::endl;
  alara_file << "    10 s" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "cooling" << std::endl;
  alara_file << "    0 s" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "data_library alaralib ../alara_eaf2010/fendl3bin" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "output zone" << std::endl;
  alara_file << "       number_density" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "truncation 1e-15" << std::endl;
  alara_file << "impurity 1e-30 1e-25" << std::endl;
}

// write the alara input deck for each route
void AlaraOutput::write_alara_input()
{
  std::map<int, std::vector<int> > :: iterator map_it;
  for ( map_it = routes.begin() ; map_it != routes.end() ; ++map_it )
    {
      // open a file
      std::vector<int> :: iterator vec_it;
      std::ofstream alara_file;
      alara_file.open("test.txt");
      write_alara_header(alara_file);
      // write fluxes
      alara_file << "flux flux_1 new_flux 5.0E7 0 default" << std::endl;
      alara_file << "" << std::endl;
      alara_file << "schedule irradation" << std::endl;
	    
      for ( vec_it = map_it->second.begin() ; vec_it != map_it->second.end() ; ++vec_it )
	{
	    alara_file << "    1 y flux_1 steady_state 0 s" << std::endl;
	}
      alara_file << "end" << std::endl;	    
      write_alara_footer(alara_file);
      alara_file.close();
    }
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
