#include "alara.hpp"

// constructor
AlaraOutput::AlaraOutput(std::map<int, std::vector<int> > routes,
			 std::string mcnp_filename,
			 std::string filename, Network *net,
			 std::string normalisation,
			 std::string delay_time,
			 int num_pulses)
{
  set_filename(filename);
  set_routes(routes);
  set_mcnp_filename(mcnp_filename);
  set_residence_times(net->get_residence_times());
  set_normalisation(normalisation);
  set_irradiation_cycles(num_pulses);
  set_delay_time(delay_time);

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

// set the filename
void AlaraOutput::set_irradiation_cycles(int num_irradiation_cycles)
{
  // set the class output filename
  num_cycles = num_irradiation_cycles;
}

// set the filename
void AlaraOutput::set_delay_time(std::string delay_time_string)
{
  // set the class output filename
  delay_time = delay_time_string;
}

// set the filename
void AlaraOutput::set_normalisation(std::string source_strength)
{
  // set the class output filename
  normalisation = source_strength;
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

// set the residence times
void AlaraOutput::set_residence_times(std::map<int, double > residency_times )
{
  // set the route data
  residence_times = residency_times;
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
      myfile.open(output_file+"_"+std::to_string(it->first));
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
  alara_file << "##" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "geometry rectangular" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "volumes" << std::endl;
  alara_file << "    125.0 zone_1" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "mat_loading" << std::endl;
  alara_file << "    zone_1 mix_1" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "spatial_norm" << std::endl;
  alara_file << "    1" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "element_lib new_isolib" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "mixture mix_1" << std::endl;
  alara_file << "    element h:1  1.1190E-1 1 " << std::endl;
  alara_file << "    element h:2  2.5720E-5 1 " << std::endl;
  alara_file << "    element o:16    0.886478794488 8" << std::endl;
  alara_file << "    element o:17    0.000359827839081 8" << std::endl;
  alara_file << "    element o:18    0.00205137767307 8" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
}

void AlaraOutput::write_alara_footer(std::ofstream &alara_file)
{
  alara_file << "pulsehistory steady_state" << std::endl;
  alara_file << "  1  0 s" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "cooling" << std::endl;
  alara_file << "    0 s" << std::endl;
  alara_file << "end" << std::endl;
  alara_file << "" << std::endl;
  alara_file << "data_library alaralib fendl3bin" << std::endl;
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
      std::cout << map_it->first << std::endl;
      alara_file.open("alara_"+std::to_string(map_it->first));
      write_alara_header(alara_file);
      // write fluxes

      for ( vec_it = map_it->second.begin() ; vec_it != map_it->second.end() ; ++vec_it )
	{
	  alara_file << "flux flux_" << (*vec_it) << " " << output_file << "_" << map_it->first;
	  alara_file << " " << normalisation << " " << vec_it-(map_it->second).begin() << " default" << std::endl;
	}
      alara_file << std::endl;
      alara_file << "schedule irradation" << std::endl;
      for ( int i = 0 ; i < num_cycles ; i++ )
	{
	  for ( vec_it = map_it->second.begin() ; vec_it != map_it->second.end() ; ++vec_it )
	    {
	      alara_file << "    " << std::to_string(residence_times[*vec_it]) << " s flux_" << (*vec_it)  << " steady_state 0 s" << std::endl;
	    }

	  // on the last cycle do not cycle the water
	  if ( i < num_cycles-1 )
	    alara_file << "    " << delay_time << " s flux_" << *(map_it->second.begin())  << " steady_state 0 s" << std::endl;
	}
      alara_file << "end" << std::endl;
      write_alara_footer(alara_file);
      alara_file.close();
    }
}

void AlaraOutput::write_alara_isolib()
{ 
  // open the isolib file
  std::ofstream isolib;
  
  isolib.open("new_isolib");
  isolib << "h:1   1.00782504   1 1.0 1" << std::endl;
  isolib << " 1 100" << std::endl;
  isolib << "h:2   2.01410178   1 1.0 1" << std::endl;
  isolib << " 2 100" << std::endl;
  isolib << "o:16  15.9949146   8 1.0 1" << std::endl;
  isolib << " 16 100" << std::endl;
  isolib << "o:17 16.9991315 8 1.0 1" << std::endl;
  isolib << "17 100" << std::endl;
  isolib << "o:18 17.9991610 8 1.0 1" << std::endl;
  isolib << "18 100" << std::endl;

  isolib.close();      
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
