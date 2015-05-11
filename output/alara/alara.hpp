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
                std::string filename);
    AlaraOutput();
   ~AlaraOutput(); // destructor

    void set_routes(std::map<int, std::vector<int> > unique_flow_routes); // set the problem routes
    void set_tallies(std::map<int, tally_struct> tally_results); // set the tallies
    void set_filename(std::string filename);
    void set_mcnp_filename(std::string filename); 
  
    std::string get_filename();
    std::string get_mcnp_filename();
  
    /* io */
    void write_alara_fluxes(); //print the spectra
    void write_alara_input(); // print the input
    bool check_consistency(); // check the class instanciation for consistency

  private:
    bool file_exists(const std::string &fname);

  private:
    // class member variables
    std::string output_file;
    std::string mcnp_filename;
    std::map<int, std::vector<int> > routes; // map of unique routes in the problem
    std::map<int, tally_struct> problem_tallies; // the tally output for the problem
};
