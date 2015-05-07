#include "cgm_version.h"
#include "GeometryQueryTool.hpp"
#include "ModelQueryEngine.hpp"
#include "RefEntityName.hpp"
#include "GMem.hpp"

#include "RefGroup.hpp"
#include "RefVolume.hpp"
#include "RefFace.hpp"
#include "RefEdge.hpp"
#include "RefEntity.hpp"
#include "RefVertex.hpp"

#include "SenseEntity.hpp"
#include "Surface.hpp"
#include "Curve.hpp"
#include "Body.hpp"
#include "InitCGMA.hpp"

#include "CubitCompat.hpp"

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
  bool check_network();
  int get_problem_members();

  private:
    std::map<int,std::vector<int> > init_problem(std::string sat_file);
    void set_cgm_attributes(bool const act_attributes, bool const verbose);
    const char* get_geom_file_type(const char* name);
    const char* get_geom_fptr_type(FILE* file);

    std::map<int,std::vector<int> >  build_all_nodes();
    void get_adjacent_entities(RefEntity *from, int to_dim, DLIList<RefEntity*> &adj_ent);
    std::vector<RefVolume*> get_neighbour_volumes(RefVolume *current_vol);

    int vol_id(RefEntity *eh);

    std::string int_to_string(int integer);

    void print_cubit_journal(std::string filename); 

  private:
    /* the following variables are common to geom */
    // the pipe network
    Network *nw;
    // map of volume ids and links
    std::map<int,std::vector<int> > problem_map;
    std::map<int,double> residence_times;

    /* the following variables are specific to ReadCGM */
    // problem map iterator
    std::map<int,std::vector<int> >::iterator it;
    std::vector<int>::iterator it_vec;
    // map of eh to id number
    std::map<int,RefVolume*> id_map;

};
