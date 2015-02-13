#include "Network.h"
#include "iGeom.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>

#include "read_tally.hpp"
#include "alara.hpp"

#include <unistd.h> // for program  options

// global vars that are needed
std::map<int, std::vector<int> > routes;
int route_counter = 0;

bool ContainsNode( std::vector<int>& nodes, int node )
{
  std::vector<int>::const_iterator nodes_it;

  for ( nodes_it = nodes.begin();
	nodes_it != nodes.end();
	nodes_it++ )
    {
      if ( (*nodes_it) == node ) return true;
    }

  return false;
}


// Algorithm to recursively search network for paths
void DepthFirst( Network* network, 
		 std::vector<int>& visited, 
		 int end )
{
  int back = visited.back();

  std::vector< int > adjNode = network->GetAdjNodeIDs( back );

  // Examine adjacent nodes
  for ( std::vector<int>::iterator node_it = adjNode.begin();
	node_it != adjNode.end();
	node_it++ )
    {
      int node = (*node_it);

      if ( ContainsNode( visited, node ) ) continue;

      if ( node == end )
	{
	  visited.push_back( *node_it );
	  
	  int hops = (int) visited.size();

	  routes[route_counter]=visited;
	  route_counter++;
	  
	  int n = (int) visited.size() - 1;
	  visited.erase( visited.begin() + n );

	  break;
	}
    }


  // in breadth-first, recursion needs to come after visiting adjacent nodes
  for ( std::vector<int>::iterator node_it = adjNode.begin();
	node_it != adjNode.end();
	node_it++ )
    {
      int node = (*node_it);

      if ( ContainsNode( visited, node ) || node == end )
	continue;
        
      visited.push_back( node );

      DepthFirst( network, visited, end );        

      int n = (int) visited.size() - 1;
      visited.erase( visited.begin() + n );
    }       
}


int vol_id(iBase_EntityHandle eh, iGeom_Instance igeom)
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

std::vector<iBase_EntityHandle> get_neighbours(iBase_EntityHandle current_vol, iGeom_Instance igeom)
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

// sets builds a map of all nodes with their children
std::map<int,std::vector<int> > build_all_nodes(iBase_EntityHandle *handles, int num_vols,
				   iGeom_Instance igeom)
{
  std::map<int,std::vector<int> > all_nodes;
  int volume_id;
  // build the nodes
  for ( int i = 0 ; i < num_vols ; i++ )
    {
      volume_id = vol_id(handles[i],igeom);
      std::vector<iBase_EntityHandle> neighbours;
      // get the neighbour volumes
      neighbours = get_neighbours(handles[i],igeom);
      std::vector<int> children; // children of the current volume
      for ( unsigned int j = 0 ; j < neighbours.size() ; j++ )
	children.push_back(vol_id(neighbours[j],igeom));
      all_nodes[volume_id]=children;
    }

  return all_nodes;
}


std::map<int,std::vector<int> > init_problem(std::string sat_file)
{
  // initialise iGeom
  iGeom_Instance igeom;
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

  std::map<int,std::vector<int> > all_nodes;
  all_nodes = build_all_nodes(vols,v_size,igeom);

  return all_nodes;

}

// converts int_to_string
std::string int_to_string(int integer)
{
  std::string result;
  std::ostringstream convert;
  convert << integer;
  result = convert.str();
  return result;
}

// print each route
void print_cubit_journal(std::string filename)
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


// print each route
void print_routes(std::map<int,std::vector<int> > all_nodes)
{
  std::map<int,std::vector<int> >::iterator it;
  std::vector<int>::iterator it_vec;

  std::map<int,std::string> color;
  
  color[0]="blue";
  color[1]="red";
  color[2]="green";

  // open the dot file
  std::ofstream dot;
  dot.open("graph1.dot");

  // dump the whole graph first
  dot << "graph network {" << std::endl;
  for ( it = all_nodes.begin() ; it != all_nodes.end() ; ++it)
    {
      for ( it_vec = it->second.begin() ; it_vec != it->second.end() ; ++it_vec) 
	{
	  dot << it->first << " -- " << *it_vec << std::endl;
	}
    }
  dot << "}" << std::endl;


  // loop over the global variable routes and print each route

  // first make the whole network
  int count = 0;
  int color_count = 0;
  for ( it = routes.begin() ; it != routes.end() ; ++it)
    {
      if ( std::distance(routes.begin(),it)%3 == 0 )
	{
	  dot << "digraph graphname"+int_to_string(count)+"{" << std::endl;
	  count++;
	  color_count = 0;
	}

      for ( it_vec = it->second.begin() ; it_vec != it->second.end() ; ++it_vec) 
	{
	  dot << *it_vec << " -> ";
	}
      dot << " [color=" << color[color_count] << "];" << std::endl;
      
      color_count++;
    }
  
  dot << "}" << std::endl;
  dot.close();

  return;
}

// prints the help message
void help()
{
  std::cout << "./otis <args>" << std::endl;
  std::cout << "-s <int> start volume" << std::endl;
  std::cout << "-f <int> finish volume" << std::endl;
  std::cout << "-c <string> ACIS file name" << std::endl;
  std::cout << "-m <string> MCNP file name" << std::endl;
  std::cout << "-h print thish help message" << std::endl;
  std::cout << "-n no activation, just print the network map and exit" << std::endl;
  return;
}

int main(int argc, char* argv[])
{
  Network nw;

  int opts; // options
  int start_vol,target_vol; // start and target vols
  char *mcnp_file = NULL, *cad_file = NULL; // strings for mcnp and cad file
  bool no_irr = false,help_set = false;

  while(( opts = getopt(argc, argv, "s:f:c:m:hn")) != -1)
    switch (opts)
      {
        case 's':
	  start_vol = std::atoi(optarg);
	  break;
        case 'f':
	  target_vol = std::atoi(optarg);
	  break;
        case 'c':
	  cad_file = optarg;
	  break;
        case 'm':
	  mcnp_file = optarg;
	  break;
        case 'h':
	  help_set = true;
	  break;
        case 'n':
          no_irr = true;
	  break;
        case '?':
	  if (optopt == 'c' | optopt == 'm' | optopt == 's' | optopt == 'f' )
	    std::cerr << "Option requires an argument" << std::endl;
	  else
	    std::cerr << "Unknown option character " << optopt << std::endl;
        default:
	  return -1;
      }

  std::cout << argc << std::endl;
  
  if ( argc == 1 | help_set)
    {
      help();
      return 0;
    }

  if(cad_file == NULL)
    {
      std::cerr << "ERROR: ACIS file not named, please provide with -c option" << std::endl;
      return -1;
    }

  if(!no_irr)
    {
      if(mcnp_file == NULL)
	{
	  std::cerr << "ERROR: MCNP file not named, please provide with -m option" << std::endl;
	  return -1;
	}
    }
  else
    {
      mcnp_file="";
    }

  if ( start_vol < 1 | target_vol < 1 )
    {
      std::cerr << "ERROR: Start and target volumes inconsistent" << std::endl;
      return -1;
    }
  else if ( start_vol == target_vol )
    {
      std::cerr << "ERROR: start and target volumes are the same" << std::endl;
      return -1;
    }

  std::string filename(cad_file);    
  std::string mcnpfile(mcnp_file);
  
  // load model and build map

  std::map<int,std::vector<int> > problem_map;
  std::map<int,std::vector<int> >::iterator it;
  std::vector<int>::iterator it_vec;
  problem_map = init_problem(filename);

  nw.SetBiDirection( true );
  // build the network
  for ( it = problem_map.begin() ; it != problem_map.end() ; ++it)
    {
      for ( it_vec = it->second.begin() ; it_vec != it->second.end() ; ++it_vec) 
	{
	  nw.AddLink(it->first,*it_vec);
	}
    }
  
  std::vector<int> visited;
  visited.push_back( start_vol );
  std::cout << "Performing search..." << std::endl;
  DepthFirst( &nw, visited, target_vol );

  // prints a dot graph of network connectivity & 
  // the unique routes
  print_routes(problem_map);
  
  if (route_counter == 0 )
    {
      std::cout << "No routes were found, check network connectivity" << std::endl;
      return 0;
    }
  
  std::string mcnp_output(argv[4]);
  // reading tallies from file
  std::map<int,tally_struct> tallies = read_tallies(mcnp_output);

  // prints the cubit journal file for easy visualisation
  print_cubit_journal("cubit.jou");

  if ( no_irr )
    return 0;

  // reading tallies from file
  std::map<int,tally_struct> tallies = read_tallies(mcnpfile);
  
  // prints the alara irradiation history
  print_alara("alara_irr",tallies,routes);
  
  return 0;
}
