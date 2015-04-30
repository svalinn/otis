#include "Network.h"

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
