#include "Network.h"

#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

Network::Network()
{
  bi_direction = true;
}

Network::~Network()
{
  Reset();
}

// Re-set all the network data
void Network::Reset()
{
  // Delete the link pointers
  for ( map< std::pair<int,int>, Link*>::iterator it = link_map.begin(); 
	it != link_map.end(); 
	it++ )
    {
      Link* l = (*it).second;
      delete l;
      l = NULL;
    }

  link_map.erase ( link_map.begin(), link_map.end() ); 
}



void Network::AddLink( Link* l )
{
  // update one-to-one mapping between each link added
  // and its unique node pair

  int s, d;
  l->GetSourceTargetNodes( s, d );
  std::pair<int,int> p( s, d );
  link_map[ p ] = l;
}



void Network::AddLink(int s, int d )
{
  if ( !LinkExists( s, d ) )
    {
      AddLink( new Link( s, d ) );
    }
}

bool Network::LinkExists( int sourceID, int targetID )
{
  bool found = false;

  std::map< std::pair<int,int>, Link* >::iterator it1, it2;

  std::pair<int,int> p1( sourceID, targetID );
  std::pair<int,int> p2( targetID, sourceID );

  it1 = link_map.find( p1 );

  // Condition for bi-directional (full duplex) links
  if ( BiDirectional() )
    {
      it2 = link_map.find( p2 );
      found = it1 != link_map.end() ||
	it2 != link_map.end();
    }
  // Otherwise directed links
  else
    {
      found =  it1 != link_map.end();
    }

  return found;
}



std::vector< int > Network::GetAdjNodeIDs( int n )
{
  vector< int > adj;

  std::map< std::pair<int,int>, Link* >::iterator link_it = 
    link_map.begin();

  for ( ;
	link_it != link_map.end();
	link_it++ )
    {
      int start  = -1;
      int target = -1;

      Link* l = (*link_it).second;

      l->GetSourceTargetNodes( start, target );

      if ( LinkExists( start, target ) )
	{
	  if ( BiDirectional() && ( start == n || target == n ) )
	    {
	      adj.push_back( start == n ? target : start );
	    }
	  else if ( start == n )
	    {
	      adj.push_back( target );
	    }
	}
    }
  
  return adj;
}


bool Network::BiDirectional()
{
  return bi_direction;
}


// Set bi-directional status of network links
void Network::SetBiDirection( bool bi )
{
  bi_direction = bi;
}
