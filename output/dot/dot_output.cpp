#ifndef DOT_HPP
#define DOT_HPP 1
#include "dot_output.hpp"
#endif 

#include <iostream>
#include <sstream>
#include <ostream>
#include <fstream> 
#include <string>

DotOutput::DotOutput(std::map<int,std::vector<int> > routes, Network *net)
{
  set_routes(routes);
  set_residence_times(net->get_residence_times());
}

DotOutput::~DotOutput()
{
}

// set the routes
void DotOutput::set_routes(std::map<int, std::vector<int> > flow_routes )
{
  // set the route data
  routes = flow_routes;
}

// set the residence times
void DotOutput::set_residence_times(std::map<int, double > residency_times )
{
  // set the route data
  residence_times = residency_times;
}

// print each route
void DotOutput::print_routes()
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
	  if (std::distance(routes.begin(),it)%3 == 0 & std::distance(routes.begin(),it) > 0 )
	    dot << "}" << std::endl;

	  dot << "digraph graphname"+int_to_string(count)+"{" << std::endl;
	  count++;
	  color_count = 0;
	}

      for ( it_vec = it->second.begin() ; it_vec != it->second.end() ; ++it_vec) 
	{
	  dot << *it_vec << " [label=\"vol " << *it_vec << " Residence time " <<  std::to_string(residence_times[*it_vec]) << "\"]" << std::endl;;
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
std::string DotOutput::int_to_string(int integer)
{
  std::string result;
  std::ostringstream convert;
  convert << integer;
  result = convert.str();
  return result;
}

