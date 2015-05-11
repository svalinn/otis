#include "read_dot.hpp"
#include <fstream>
#include <sstream>

DotReader::DotReader(std::string filename, bool bidirectional)
{
  // read a dotfile and populate the class with
  // a network of nodes and connections    
  nw = new Network();

  // if we want a two way network
  if(bidirectional)
    nw->SetBiDirection( true );
  else
    nw->SetBiDirection( false );

  // read the dot file
  read_dot_file(filename);
 
  // check the network
  if(!check_network())
    std::cout << "Network Connection Problems" << std::endl;
}

DotReader::~DotReader()
{
}

// retrurn the network
Network* DotReader::get_network()
{
  return nw;
}

// return the map of the problem
std::map<int,std::vector<int> > DotReader::get_problem_map()
{
  return problem_map;
}

// return the residence times
std::map<int, double> DotReader::get_residence_times()
{
  return residence_times;
}

bool DotReader::check_network()
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

// read the whole dot file and populate the links
void DotReader::read_dot_file(std::string filename)
{
  std::ifstream dotfile;
  dotfile.open(filename.c_str(), std::ios::in);
  if(!dotfile.is_open())
    {
      std::cerr << "-----Read Error -----:" << std::endl;
      std::cerr << "File not availble for opening" << std::endl;
      std::cerr << "Check filename" << std::endl;
      std::cerr << "-----Read Error -----:" << std::endl;
    }

  std::string line;
  // read the dotfile
  while ( std::getline (dotfile,line) )
    {
      if(line.find("//") != std::string::npos)
	continue;
      if(line.find("->") != std::string::npos)
	process_link(line);
      if(line.find("label") != std::string::npos)
	process_node(line);
    }

  // build problem map from network
  build_problem_map();

  dotfile.close();
}

// given that the network exists, build the problem map
void DotReader::build_problem_map()
{
  // loop over the members and create the map
  std::set<int> :: iterator set_it;
  for ( set_it = members.begin() ; set_it != members.end() ; set_it++ )
    {
      std::vector<int> neighbours = nw->GetAdjNodeIDs(*set_it);
      problem_map[*set_it] = neighbours;
    }
  return;
}

// process the string and extract the node data
void DotReader::process_node(std::string line)
{
  std::vector<std::string> tokens = tokenise(line,'"');
  std::vector<std::string> values = tokenise(tokens[1],' ');

  // need to check that 
  // values[0] = vol
  // values[1] = id number
  // values[2] = residence time
  // values[3] = seconds
  residence_times[std::stoi(values[1])] = std::stof(values[2]);
}

// process the string and extract the link data
void DotReader::process_link(std::string line)
{
  int node1,node2;
  std::vector<std::string> tokens = tokenise(line,' ');

  // find the "->" token, then take the elements adjacent
  for( int i = 0 ; i < tokens.size() ; i++ )
    {
      if (tokens[i].find("->") != std::string::npos) 
	{
	  int link1 = std::stoi(tokens[i-1]);
	  int link2 = std::stoi(tokens[i+1]);
	  nw->AddLink(link1,link2);
	  members.insert(link1);
	  members.insert(link2);
	  break;
	}
    }
}

// take the string and tokenise by a character
std::vector<std::string> DotReader::tokenise(std::string line, char delimiter)
{
  std::stringstream ss(line);
  std::vector<std::string> tokens;
  std::string s;

  while (std::getline(ss, s, delimiter)) 
    tokens.push_back(s);
  return tokens;
}
