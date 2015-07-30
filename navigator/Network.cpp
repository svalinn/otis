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

// get the residence times
std::map<int,double> Network::get_residence_times()
{
  return residence_times;
}

// get the residence times
std::map<int,std::vector<int> > Network::get_problem_map()
{
  return problem_map;
}

// set the resience times
void Network::set_residence_times(std::map<int,double> residency_times)
{
  residence_times = residency_times;
}

// set the resience times
void Network::set_problem_map(std::map<int,std::vector<int> > problem_maps)
{
  problem_map = problem_maps;
}

// set the volume/volume surface areas
void Network::set_surface_areas(std::map<std::pair<int,int>, double > surface_areas)
{
  boundary_areas = surface_areas;
}

// set the volume/volume surface areas
std::map<std::pair<int,int>, double > Network::get_surface_areas()
{
  return boundary_areas;
}

void Network::modify_residence_times(std::map<int,std::vector<int> > routes)
{
  std::map<int,std::vector<int> > :: iterator it;
  std::vector<int> this_route;

  double area_next;
  double area_now;


  std::map<int,int> num_splits; // the number of splits that occurs in each volume
  // calculate the number of splits that occurs for each volume
  // loop over each route
  for ( it = routes.begin() ; it != routes.end() ; ++it )
    {
      int route = it->first;
      this_route = routes[route];
      for ( int i = 1 ; i < this_route.size() ; i++ )
	{
	  if(LinkExists(this_route[i-1],this_route[i]))
	    {
	      if(num_splits.count(this_route[i-1]) == 0 )
		num_splits[this_route[i-1]] = 1;
	      else
		num_splits[this_route[i-1]] += 1;
	    }
	}
    }
  // we know the target shares as many as the source
  num_splits[this_route[this_route.size()-1]] = num_splits[this_route[0]];

  // now set modify the residence times
  std::map<int,int> :: iterator iter;
  for ( iter = num_splits.begin() ; iter != num_splits.end() ; ++iter)
    {
      residence_times[iter->first] = residence_times[iter->first] * float(routes.size()) / float(iter->second)  ;
      std::cout << iter->first << " " << iter->second << std::endl;
    }

  return;
  
  /*
  for ( it = routes.begin() ; it != routes.end() ; ++it )
    {
      int route = it->first;
      this_route = routes[route];
      {
	for ( int i = 1 ; i < this_route.size()-1 ; i++ )
	  {
	    
	    
	    /*
	    // the current volume/next volume pair
	    std::pair<int,int> last_vol_this_vol = std::make_pair(this_route[i-1],this_route[i]);
	    std::pair<int,int> this_vol_next_vol = std::make_pair(this_route[i],this_route[i+1]);

	    // if the the volume is the source
	    if(this_route[i-1] == this_route[0])
	      area_now =  boundary_areas[last_vol_this_vol];
	    else
	      area_now =  boundary_areas[last_vol_this_vol];


	    // if the the volume is the target
	    if(this_route[i+1] == this_route[this_route.size()-1])
	      area_next =  boundary_areas[last_vol_this_vol];
	    else    
	      area_next = boundary_areas[this_vol_next_vol];

	    //	    std::cout << this_route[i] << " " << this_route[i+1] << " " << area_now << " " << area_next << std::endl;
	    residence_times[this_route[i]] = residence_times[this_route[i-1]]*area_now/area_next;
	  }
  */
   
    

  return;
}
