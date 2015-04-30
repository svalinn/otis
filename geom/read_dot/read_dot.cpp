#include "read_dot.hpp"
#include <fstream>
#include <sstream>

DotReader::DotReader(std::string filename, bool bidirectional)
{
  // read a dotfile and populate the class with
  // a network of nodes and connections

  // if needed set bidirectional
  if(bidirectional)
    nw.SetBiDirection(true);

  // read the dot file
  read_dot_file(filename);
    
}

DotReader::~DotReader()
{
}

Network DotReader::get_network()
{
  return nw;
}

// return the map of the problem
std::map<int,std::vector<int> > DotReader::get_problem_map()
{
  return problem_map;
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
}

// process the string and extract the link data
void DotReader::process_link(std::string line)
{
  int node1,node2;
  std::vector<std::string> tokens = tokenise(line,' ');
  
  // nw.AddLink(node1,node2);
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
