
#ifndef READ_TALLY_HPP
#define READ_TALLY_HPP
struct tally_struct {
  int entity_number;
  double entity_size;
  std::vector<double> ebins;
  std::vector<double> result;
  std::vector<double> error;
};

struct flux_result {
  double energy;
  double flux;
  double error;
};

#endif

// print a specific tally from the map
void print_tally(std::map<int,tally_struct> tallies, int tally_id, std::ostream &ostream);

// read the tallies from the file filename
std::map<int,tally_struct> read_tallies(std::string filename);

// read an atomic tally
std::vector<flux_result> read_tally_atomic(std::ifstream &data, int &vol_id);


// reads and creates the initial data set for this collection
// of tallies
std::map<int,tally_struct> read_tally_volumes(std::ifstream &data);
