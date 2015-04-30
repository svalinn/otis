#ifndef DOT_HPP
#define DOT_HPP 1
#include "dot_output.hpp"
#endif 

#include <iostream>
#include <sstream>
#include <ostream>
#include <fstream> 
#include <string>

// print each route
void print_routes(std::map<int,std::vector<int> > routes)
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
	  if ( it->second.end() - it_vec == 1 )
	    dot << *it_vec;
	  else
	    dot << *it_vec << " -> ";
	    
	}
      dot << " [color=" << color[color_count] << "];" << std::endl;
      
      color_count++;
    }
  
  dot << "}" << std::endl;
  dot.close();

  return;
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

