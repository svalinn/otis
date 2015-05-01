#include "read_cgm.hpp"
#include <iostream>
#include <set>
#include <sstream>
#include <fstream>
#include <ostream>


// constructor
ReadCGM::ReadCGM(std::string filename, bool bidirectional)
{
  nw = new Network();

  // load model and build map
  problem_map = init_problem(filename);

  // if we want a two way network
  if(bidirectional)
    nw->SetBiDirection( true );

  // build the network
  for ( it = problem_map.begin() ; it != problem_map.end() ; ++it)
    {
      for ( it_vec = it->second.begin() ; it_vec != it->second.end() ; ++it_vec) 
	{
	  nw->AddLink(it->first,*it_vec);
	}
    }
}

// destructor
ReadCGM::~ReadCGM()
{
}

// initalises, loads 
std::map<int,std::vector<int> > ReadCGM::init_problem(std::string sat_file)
{
  // initialise iGeom
  int err;
  std::string engine_opt = ";engine=";
  engine_opt += "ACIS";
  iGeom_newGeom(engine_opt.c_str(), &igeom, &err,engine_opt.length());

  // read in the geometry
  iGeom_load(igeom, sat_file.c_str(),0,&err,sat_file.length(),0);

  // get the root set
  iBase_EntitySetHandle root_set;
  iGeom_getRootSet(igeom, &root_set, &err);

  // detect the loaded volumes
  int v_alloc = 0;
  int v_size = 0;
  iBase_EntityHandle *vols = NULL;
  iGeom_getEntities(igeom, root_set, iBase_REGION, &vols, &v_alloc, 
		    &v_size, &err);

  // maps o
  std::map<int,std::vector<int> > all_nodes;
  // for all volumes determine the 
  all_nodes = build_all_nodes(vols,v_size);

  return all_nodes;

}

// sets builds a map of all volume ids key, with neighbours as vectors
std::map<int,std::vector<int> > ReadCGM::build_all_nodes(iBase_EntityHandle *handles, int num_vols)
{
  std::map<int,std::vector<int> > all_nodes;
  int volume_id;
  // build the nodes
  for ( int i = 0 ; i < num_vols ; i++ )
    {
      volume_id = vol_id(handles[i]);
      std::vector<iBase_EntityHandle> neighbours;
      // get the neighbour volumes
      neighbours = get_neighbours(handles[i]);
      std::vector<int> children; // children of the current volume
      for ( unsigned int j = 0 ; j < neighbours.size() ; j++ )
	children.push_back(vol_id(neighbours[j]));
      all_nodes[volume_id]=children;
    }

  return all_nodes;
}

// for a given volume, return a vector of adjacent volumes
std::vector<iBase_EntityHandle> ReadCGM::get_neighbours(iBase_EntityHandle current_vol)
{
  int dimension = 3;
  iBase_EntityHandle *neighbour_surfaces = NULL;
  int neighbour_surf_size = 0;
  int neighbour_occupy_size = 0;

  int err;

  std::vector<iBase_EntityHandle> shared_volumes;
  std::set<iBase_EntityHandle> shared_vols;
  
  // gets the surfaces, shared by the volume
  iGeom_getEntAdj(igeom,current_vol,dimension-1,&neighbour_surfaces,
		  &neighbour_surf_size, &neighbour_occupy_size, &err);
  
  // loop over the surfaces
  for ( int i = 0 ; i < neighbour_occupy_size ; i++ ) 
    {
      iBase_EntityHandle *neighbours = NULL;
      int neighbours_alloc_size = 0;
      int neighbours_occupy_size = 0;

      // find out which volumes share this surface
      iGeom_getEntAdj(igeom,neighbour_surfaces[i],dimension,&neighbours,
		      &neighbours_alloc_size, &neighbours_occupy_size, &err);

      for ( int j = 0 ; j < neighbours_occupy_size ; j++ ) 
	{
	  shared_vols.insert(neighbours[j]);
	}

    }
  
  // clear the current
  shared_vols.erase(current_vol);
  
  // make set into vector
  std::copy(shared_vols.begin(), shared_vols.end(), std::back_inserter(shared_volumes));

  return shared_volumes;
}

// given the ibase entity handle determine the volume id
int ReadCGM::vol_id(iBase_EntityHandle eh)
{
  iBase_TagHandle global_id_tag;
  std::string tag_name("GLOBAL_ID");
  //  std::string tag_name("VOLUME_ID");
  int err = 0;
  int id;
  // get the tag handle
  iGeom_getTagHandle(igeom,&tag_name[0],&global_id_tag,&err,tag_name.length());
  // get the data
  iGeom_getIntData(igeom,eh,global_id_tag,&id,&err);
  return id;
}

//
bool ReadCGM::check_network()
{
  std::vector<int> links = nw->GetAdjNodeIDs(1);
  if (links.size() == 0 )
    return false;
  else 
    return true;
}

// provide read only access to the current network
Network* ReadCGM::get_network()
{
  return nw;
}

// return the map of the problem
std::map<int,std::vector<int> > ReadCGM::get_problem_map()
{
  return problem_map;
}

/*
// print each route
void ReadCGM::print_cubit_journal(std::string filename)
{
  std::map<int,std::vector<int> >::iterator it;
  std::vector<int>::iterator it_vec;

  std::map<int,std::string> color;
  
  // open the dot file
  std::ofstream cubit;
  cubit.open(filename.c_str());

  cubit << "#!python" << std::endl;
  for ( it = routes.begin() ; it != routes.end() ; ++it)
    {
      for ( it_vec = it->second.begin() ; it_vec != it->second.end() ; ++it_vec) 
	{
	  cubit << "cubit.cmd('draw volume " << *it_vec << " add')" << std::endl;
	  cubit << "cubit.cmd('sleep 1')" << std::endl;
	}
      cubit << "cubit.cmd('pause')" << std::endl;
      cubit << "cubit.cmd('graphics clear')" << std::endl;
    }
  cubit.close();

  return;
}
*/
