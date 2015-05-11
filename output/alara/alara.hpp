#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <set>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


#ifndef READ_TALLY_HPP 
#define READ_TALLY_HPP 1
#include "read_tally.hpp"
#endif

#ifndef NETWORK_H 
#define NETWORK_H 1
#include "Network.h"
#endif

/*
struct tally_struct {
  int entity_number;
  double entity_size;
  std::vector<double> ebins;
  std::vector<double> result;
  std::vector<double> error;
};
*/

class AlaraOutput {
  public:
    AlaraOutput(std::map<int, std::vector<int> > unique_flow_routes,
                std::string mcnp_filename,
                std::string filename, Network *nw);
    AlaraOutput();
   ~AlaraOutput(); // destructor

    void set_routes(std::map<int, std::vector<int> > unique_flow_routes); // set the problem routes
    void set_tallies(std::map<int, tally_struct> tally_results); // set the tallies
    void set_filename(std::string filename);
    void set_residence_times(std::map<int,double> residence_times);
    void set_mcnp_filename(std::string filename); 
  
    std::string get_filename();
    std::string get_mcnp_filename();
  
    /* io */
    void write_alara_fluxes(); //print the spectra
    void write_alara_input(); // print the input
    bool check_consistency(); // check the class instanciation for consistency
    bool file_exists(const std::string &fname);

  private:
    void write_alara_header(std::ofstream &alara_file);
    void write_alara_footer(std::ofstream &alara_file);


  private:
    // class member variables
    std::string output_file;
    std::string mcnp_filename;
    std::map<int, double> residence_times;
    std::map<int, std::vector<int> > routes; // map of unique routes in the problem
    std::map<int, tally_struct> problem_tallies; // the tally output for the problem
};
