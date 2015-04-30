#include "iGeom.h"

#ifndef NETWORK_H
#define NETWORK_H 1
#include "Network.h"
#endif


class ReadCGM {
  public:
  ReadCGM(std::string filename, bool bidirectional);
  ~ReadCGM();
  // give access to the network
  Network* get_network();
  std::map<int,std::vector<int> > get_problem_map();
  

  private:
    std::map<int,std::vector<int> > init_problem(std::string sat_file);

    std::map<int,std::vector<int> > build_all_nodes(iBase_EntityHandle *handles, int num_vols);

    std::vector<iBase_EntityHandle> get_neighbours(iBase_EntityHandle current_vol);

    int vol_id(iBase_EntityHandle eh);

    std::string int_to_string(int integer);

    void print_cubit_journal(std::string filename); 

  private:
    // the pipe network
    Network *nw;
    // iGeom singleton
    iGeom_Instance igeom;
    // map of volume ids and links
    std::map<int,std::vector<int> > problem_map;
    // problem map iterator
    std::map<int,std::vector<int> >::iterator it;
    // 
    std::vector<int>::iterator it_vec;

};
