#include "geometry.hpp"

// constructor
Geometry::Geometry(std::string filename, bool bidirectional)
{
  Network *nw = new Network();

  // if we want a two way network
  if(bidirectional)
    nw->SetBiDirection( true );
  else
    nw->SetBiDirection( false );

}

// default constructor
Geometry::Geometry()
{
}

// destructor
Geometry::~Geometry()
{
}

// set the network
void Geometry::set_network(Network *network)
{
  *nw = *network;
  return;
}

// get the network
Network* Geometry::get_network()
{
  return nw;
}

// return the map of the problem
void Geometry::set_problem_map(std::map<int, std::vector<int> > prob_map )
{
  problem_map = prob_map;
}

// return the map of the problem
std::map<int,std::vector<int> > Geometry::get_problem_map()
{
  return problem_map;
}

// set the residence times
void Geometry::set_residence_times(std::map<int,double> residence)
{
  residence_times = residence;
}


// return the map of the residence times
std::map<int,double> Geometry::get_residence_times()
{
  return residence_times;
}

// check for consistency
bool Geometry::check_network()
{
  // check 1 - make sure there are some nodes
  if(problem_map.size() == 0 ) return false;

  // check 2 - make sure every node is connected to every other node
  // loop through the list of members and assert that each node
  // is connected to at least one other node
  std::map<int,std::vector<int> > :: iterator prob_it;
  for ( prob_it = problem_map.begin() ; prob_it != problem_map.end() ; ++prob_it )
    {
      // check each node
      std::vector<int> links = nw->GetAdjNodeIDs(prob_it->first);
      if (links.size() == 0 )
	{
	  std::cerr << "The network has isolated nodes, node " << prob_it->first << " is unconnected" << std::endl;
	  return false;
	}
    }
  // check 3 - make sure the there are as many nodes as there are irradation times
  if ( problem_map.size() != residence_times.size())
    {
      std::cout << problem_map.size() << " " << residence_times.size() << std::endl;
      std::cout << "The problem network is inconsisent with the residence time information" << std::endl;
      return false;
    }

  return true;
}
