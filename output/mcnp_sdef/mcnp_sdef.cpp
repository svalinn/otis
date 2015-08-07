#include "mcnp_sdef.hpp"
#include <vector>

// construct new mcnpsdef class
McnpSdef::McnpSdef(std::map<int,std::vector<int> > routes)
{
  set_routes(routes);
}

McnpSdef::~McnpSdef()
{
}

void McnpSdef::set_routes(std::map<int,std::vector<int> > routes)
{
  unique_routes = routes;
  return;
}

void McnpSdef::read_control_file(std::string control_file)
{
  std::ifstream file;
  file.open(control_file.c_str(), std::ios::in);
  if(!file.is_open())
  {
     std::cerr << "File, " << control_file << " does not exist" << std::endl;
     return;
  }
  std::string line;
  // open the control file
  while( std::getline(file,line) )
    {
      // treat # as comment
      if(line.find("#") == std::string::npos)
        spectra_files.push_back(line);
    }
}

void McnpSdef::read_spectra_file(std::string spectra_file)
{
  // open the spectra file and populate struct
  std::ifstream file;
  file.open(spectra_file.c_str(), std::ios::in);
  if(!file.is_open())
  {
     std::cerr << "File does not exist" << std::endl;
  }

  std::string line;
  // open the control file
  while( std::getline(file,line) )
    {
      // treat # as comment
      if(line.find("#") == std::string::npos)
        {
          process_line(line);
        }
    }
}

// process the string line into a spectral struct
void McnpSdef::process_line(std::string spectral_line)
{
  // tokenise the string into 25 bits, 1st is the cell int
  // the next 24 are the bin bounds
  std::vector<std::string> tokens = tokenize(spectral_line);

  // turn the spectra string into a double arra
  double spectra[24];
  //std::cout << std::stoi(tokens[0]) << std::endl;
  int cell = std::stoi(tokens[0]);
  cells.insert(cell);
  for ( int i = 1 ; i <= 24 ; i++ )
    {
      spectra[i-1] = std::stof(tokens[i]);
    }

  double other_spectra[24];
  // the other spectra is the one that exists in the current location

  // loop over the groups
  for ( int i = 0 ; i < 24 ; i++ )
  {
    // sum the value if it already exists
    // std::cout << spectra[i] << std::endl;
     if ( cell_spectra.count(cell) == 0 )
       cell_spectra[cell][i] = spectra[i];
     else
       cell_spectra[cell][i] += spectra[i];

     //std::cout << spectra[i] << " " << cell_spectra[cell][i] << std::endl;
  }
}

// tokenize a string into a vector of sub-strings
std::vector<std::string> McnpSdef::tokenize(std::string input_string)
{
  std::vector<std::string> tokens;
  // based on http://stackoverflow.com/questions/53849/
  //  how-do-i-tokenize-a-string-in-c
  std::stringstream ss(input_string);
  std::string s;

  while( std::getline(ss, s, ' ') )
    {
      tokens.push_back(s);
    }
  return tokens;
}

// loop over the routes and process the spectra into results
void McnpSdef::process_spectra()
{
  // the number of routes should match the spectra files to read
  if(unique_routes.size() != spectra_files.size())
    {
      std::cerr << "The number of routes and spectra do not match " << std::endl;
      return;
    }

    if(unique_routes.size() == 0)
      {
        std::cerr << "There are no routes in the network " << std::endl;
        return;
      }

    if(spectra_files.size() == 0)
      {
        std::cerr << "There are no spectra! " << std::endl;
        return;
      }

  // loop over the files and process the data
  for ( int i = 0 ; i < unique_routes.size() ; i++)
    {
      read_spectra_file(spectra_files[i]);
    }

  // for each cell
  std::set<int>::iterator it;
  for ( it = cells.begin() ; it != cells.end() ; ++it)
    {
      int cell = *it;
      double total = 0.0;
      for ( int j = 0 ; j < 24 ; j++ )
        {
           total += cell_spectra[cell][j];
        }
       cell_totals[cell] = total;
    }
    return;
}

// loop over the routes and process the spectra into results
void McnpSdef::print_sdef()
{
  // std::map<int,std::vector<double> >::iterator it;
  // for each route
  std::cout << "SDEF X=D1 Y=D2 Z=D3 CEL=D4 ERG=FCEL=D5 PAR=2" << std::endl;
  std::cout << "SI4 S ";

  if(cells.size() == 0 )
    {
      std::cerr << "No cells present in the source" << std::endl;
      return;
    }

  //int num_cells = cells.size();
  std::set<int>::iterator it;
  int count = 0;
  for ( it = cells.begin() ; it != cells.end() ; ++it)
    {
       std::cout << *it << " ";
       if ( count > 0 && count%6 == 0)
          std::cout << std::endl << "       ";
       count++;
    }
  std::cout << std::endl;
  std::cout << "SP4 ";
  count = 0;
  for ( it = cells.begin() ; it != cells.end() ; ++it)
    {
        std::cout << cell_totals[*it] << " ";
        if ( count > 0 && count%6 == 0)
           std::cout << std::endl << "       ";
        count++;
    }
  std::cout << std::endl;

  std::cout << "DS5 S ";
  int i = 0;
  for ( i = 0 ; i < cells.size() ; i++)
  {
    std::cout << 6+i << " ";
    if ( i > 0 && i%6 == 0)
       std::cout << std::endl << "      ";
  }
  std::cout << std::endl;
  i = 5;
  for ( it = cells.begin() ; it != cells.end() ; ++it)
    {
        i++;
        std::cout << "SP" << i << " ";
        int cell = *it;
        for ( int j = 0 ; j < 24 ; j++ )
          {
             std::cout << cell_spectra[cell][j] << " ";
             if ( j > 0 && j%6 == 0)
                std::cout << std::endl << "      ";
          }
        std::cout << std::endl;
     }
}
