#include "Navigation.hpp"
#include <iostream>

navigation::navigation(Network* network, int source_id)
{
  ntwk = network;
  visited.push_back(source_id);
}

navigation::~navigation()
{
}

// return the unique routes from the search
std::map<int,std::vector<int> > navigation::get_unique_routes()
{
  return routes;
}

// return the unique routes from the search
void navigation::print_routes()
{
  std::map<int,std::vector<int> > :: iterator it;
  std::vector<int> :: iterator vec_it;
  for ( it = routes.begin() ; it != routes.end() ; ++it )
    {
      std::cout << it->first << std::endl;
      for ( vec_it = (it->second).begin() ; vec_it != (it->second).end() ; ++vec_it)
	{
	  std::cout << *vec_it <<",";
	}
      std::cout << std::endl;
    }
}

bool navigation::ContainsNode( std::vector<int>& nodes, int node )
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
void navigation::DepthFirst( int end )
{
  int back = visited.back();

  std::vector< int > adjNode = ntwk->GetAdjNodeIDs( back );

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

      DepthFirst( end );        

      int n = (int) visited.size() - 1;
      visited.erase( visited.begin() + n );
    }       
}
