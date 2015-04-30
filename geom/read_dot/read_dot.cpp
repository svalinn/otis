#include "read_dot.hpp"
#include <fstream>
#include <sstream>

DotReader::DotReader(std::string filename, bool bidirectional)
{
  // read a dotfile and populate the class with
  // a network of nodes and connections    
  nw = new Network();

  // if needed set bidirectional
  if(bidirectional)
    nw->SetBiDirection(true);

  // read the dot file
  read_dot_file(filename);
 
  // check the network
  if(!check_network())
    std::cout << "Network Connection Problems" << std::endl;
}

DotReader::~DotReader()
{
}

Network* DotReader::get_network()
{
  return nw;
}

// return the map of the problem
std::map<int,std::vector<int> > DotReader::get_problem_map()
{
  return problem_map;
}

bool DotReader::check_network()
{
  std::vector<int> links = nw->GetAdjNodeIDs(1);
  if (links.size() == 0 )
    return false;
  else 
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

  dotfile.close();
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
	  nw->AddLink(std::stoi(tokens[i-1]),std::stoi(tokens[i+1]));
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
