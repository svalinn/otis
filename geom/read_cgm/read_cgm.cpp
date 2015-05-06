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
  std::map<int, std::vector<int> > problem_map;
  // Initialize CGM
  InitCGMA::initialize_cgma();

  const char* cgm_file_name = sat_file.c_str();

  bool act_att = true;
  bool verbose_warnings = false;

  // Determine CGM settings and amount of output
  set_cgm_attributes(act_att, verbose_warnings);

  // Get CGM file type
  const char* file_type = 0;
  //  file_type = get_geom_file_type(cgm_file_name);

  CubitStatus s;
  // import the geometry
  s = CubitCompat_import_solid_model(cgm_file_name,"ACIS_SAT");
  if (CUBIT_SUCCESS != s) {
    std::cout << cgm_file_name << ": Failed to read acis file" << std::endl;
  }

  // build the nodes of the problem
  std::map<int,std::vector<int> > all_nodes = build_all_nodes();

  // get the neighbour list
    
  return all_nodes;
}

std::map<int,std::vector<int> > ReadCGM::build_all_nodes()
{
  std::map<int,std::vector<int> > problem_nodes;
  // get the id/eh correspeondence
  if(get_problem_members()!=0) std::cerr << "Failed to find 'water' group" << std::endl;
  
  std::map<int,RefVolume*>::iterator irf;
  // loop over the water volumes
  for ( irf = id_map.begin() ; irf != id_map.end() ; ++irf )
    {
      int id = irf->first;
      RefVolume* vol = irf->second;
      //      std::cout << id << " " << vol << " " << vol->id() << std::endl;
      // get the neighbour volumes
      std::vector<int> neighbour_ids;
      std::vector<RefVolume*> neighbours = get_neighbour_volumes(vol);
      for ( int i = 0 ; i < neighbours.size() ; i++ )
	{
	  //	  int id_local = neighbours[i]->id();
	  neighbour_ids.push_back( neighbours[i]->id() );
	}
      problem_nodes[id] = neighbour_ids;
    }
  return problem_nodes;
}


int ReadCGM::get_problem_members()
{
  // list of entities
  DLIList<RefEntity*> entitylist;
  entitylist.clean_out();
  // Get all entity groups from the CGM model
  GeometryQueryTool::instance()->ref_entity_list("group", entitylist);

#if CGM_MAJOR_VERSION > 13
  DLIList<CubitString> name_list;
#else
  DLIList<CubitString*> name_list;
#endif

  // Loop over all groups
  for (int i = entitylist.size(); i--; ) {
    // Take the next group
    RefEntity* grp = entitylist.get_and_step();
    name_list.clean_out();

    // Get the names of all entities in this group from the solid model
    RefEntityName::instance()->get_refentity_name(grp, name_list);

    if (name_list.size() == 0)
      continue;
    // Set pointer to first name of the group and set the first name to name1
    name_list.reset();

#if  CGM_MAJOR_VERSION > 13
    CubitString name1 = name_list.get();
#else
    CubitString name1 = *name_list.get();
#endif
 
    // we have the water groupd
    DLIList<RefEntity*> grp_members;
    if(name1 == "Water")
      {
	RefGroup* group = (RefGroup*) grp;
	group->get_child_ref_entities(grp_members);
	std::cout << "Found the water group" << std::endl;
	std::cout << "It contains " << grp_members.size() << " members" << std::endl;
	for ( int i = 0 ; i < grp_members.size() ; i++)
	  {
	    RefVolume* vol = dynamic_cast<RefVolume*>(grp_members[i]); //vols.get_and_step();
	    int id = vol->id();
	    id_map[id] = vol; // id map is class member variable
	  }
	return 0;
      }
  }
  return 1;
}



void ReadCGM::get_adjacent_entities(RefEntity *from,
				    int to_dim,
				    DLIList<RefEntity*> &adj_ents)
{
  TopologyEntity *topo_ent = const_cast<TopologyEntity*>(dynamic_cast<const TopologyEntity*>(from));
  if (NULL == topo_ent) {
    std::cerr << "Invalid entity passed to get_adjacent_entities" << std::endl;
    return;
  }

  adj_ents.clean_out();

  static DLIList<RefVertex*> tmp_verts;
  static DLIList<RefEdge*> tmp_edges;
  static DLIList<RefFace*> tmp_faces;
  static DLIList<RefVolume*> tmp_volumes;

  switch (to_dim) {
  case 0:
    tmp_verts.clean_out();
    topo_ent->ref_vertices(tmp_verts);
    CAST_LIST_TO_PARENT(tmp_verts, adj_ents);
    break;
  case 1:
    tmp_edges.clean_out();
    topo_ent->ref_edges(tmp_edges);
    CAST_LIST_TO_PARENT(tmp_edges, adj_ents);
    break;
  case 2:
    tmp_faces.clean_out();
    topo_ent->ref_faces(tmp_faces);
    CAST_LIST_TO_PARENT(tmp_faces, adj_ents);
    break;
  case 3:
    tmp_volumes.clean_out();
    topo_ent->ref_volumes(tmp_volumes);
    CAST_LIST_TO_PARENT(tmp_volumes, adj_ents);
    break;
  default:
    std::cerr  << "valid dimension passed" << std::endl;
  }
  return;
}

// for a given volume, return a vector of adjacent volumes
std::vector<RefVolume*> ReadCGM::get_neighbour_volumes(RefVolume *current_vol)
{
  int dimension = 3;
  DLIList<RefEntity*> neighbour_surfaces;
  DLIList<RefEntity*> neighbour_volumes;

  int neighbour_surf_size = 0;
  int neighbour_occupy_size = 0;

  int err;

  // 
  std::vector<RefVolume*> shared_volumes;
  std::set<RefVolume*> shared_vols;
  
  // cast the ref vol in to refentity
  RefEntity* vol_ent = dynamic_cast<RefEntity*>(current_vol);
  // gets the surfaces, shared by the volume
  get_adjacent_entities(vol_ent,2,neighbour_surfaces);
  
  // loop over the surfaces
  for ( int i = 0 ; i < neighbour_surfaces.size() ; i++ ) 
    {
      RefEntity *neighbours = NULL;
      // get the volumes shared by the surface
      get_adjacent_entities(neighbour_surfaces[i],3,neighbour_volumes);

      for ( int j = 0 ; j < neighbour_volumes.size() ; j++ ) 
	{
	  RefVolume* shared_vol = dynamic_cast<RefVolume*>(neighbour_volumes[j]);
	  shared_vols.insert(shared_vol);
	}
    }
  
  // clear the current
  shared_vols.erase(current_vol);
  
  // make set into vector
  std::copy(shared_vols.begin(), shared_vols.end(), std::back_inserter(shared_volumes));
  
  return shared_volumes;
}

void ReadCGM::set_cgm_attributes(bool const act_attributes, bool const verbose)
{
  if (act_attributes) {
    CGMApp::instance()->attrib_manager()->set_all_auto_read_flags(act_attributes);
    CGMApp::instance()->attrib_manager()->set_all_auto_actuate_flags(act_attributes);
  }
  
  if (!verbose) {
    CGMApp::instance()->attrib_manager()->silent_flag(true);
  }
}
 


// given the ibase entity handle determine the volume id
int ReadCGM::vol_id(RefEntity *eh)
{
  int id = eh->id();
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
