#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <set>

class McnpSdef {
  public:
    // constructor
    McnpSdef();
    // constructor
    McnpSdef(std::map<int,std::vector<int> > routes);

    //  destructor
    ~McnpSdef();

    // sets the routes
    void set_routes(std::map<int,std::vector<int> > routes);

    // read the control file
    void read_control_file(std::string control_file);

    // tokenize a string
    std::vector<std::string> tokenize(std::string input_string);

    // process string into
    void process_line(std::string line);

    // process the spectra into the class variables
    void process_spectra();

    // prints the sdef to screen
    void print_sdef();

  // private funcs
  private:
    // read a spectra file
    void read_spectra_file(std::string spectra_file);

  // private vars
  private:
    std::map<int,std::vector<int> > unique_routes; // all the routes
    std::vector<std::string> spectra_files;  // the files to process
    std::set<int> cells; // list of cells
    std::map<int,double[24]> cell_spectra; // map of spectra
    std::map<int,double> cell_totals; // total number of photons for each cell
    double normalisation;
};
