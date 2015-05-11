#ifndef READ_TALLY_HPP
#define READ_TALLY_HPP 1
#include "read_tally.hpp"
#endif

std::map<int,tally_struct> read_tallies(std::string filename)
{
  std::ifstream data(filename.c_str());

  std::string line;

  std::map<int,tally_struct> tallies;

  if( data.is_open() )
    {
      while(std::getline(data,line))
	{
	  // found tally line
	  if( line.find("1tally") != std::string::npos &&
	      line.find("nps =")  != std::string::npos )
	    {
	      //	      std::cout << "pull = " << line << std::endl;
	      std::getline(data,line); // pull another line
	      // line is now on the type of tally line
	      //	      std::cout << line << std::endl;

	      std::getline(data,line); // pull another line
	      // line is now on the type of particle line
	      //     std::cout << line << std::endl;

	      // pull two more lines and now 
	      std::getline(data,line); // pull another line
	      std::getline(data,line); // pull another line should be on volumes line
	      //	      std::cout << line << std::endl;

	      std::map<int,tally_struct> tmp_tallies;
	      tmp_tallies = read_tally_volumes(data);

	      //	      std::cout << "There are " << tmp_tallies.size() << " tallies" << std::endl;
	      // tally volumes are now read atomic tallies n times
	      std::map<int,tally_struct>::iterator it;
	      for ( it = tmp_tallies.begin() ; it != tmp_tallies.end() ; ++it )
		{
		  int vol_id;
		  std::vector<flux_result> tally_data = read_tally_atomic(data,vol_id);
		  //		  std::cout << vol_id << " ";
		  for ( int i = 0 ; i < tally_data.size() ; i++ )
		    {
		      
		      /*
		      (it->second).ebins.push_back(tally_data[i].energy);
		      (it->second).result.push_back(tally_data[i].flux);
		      (it->second).error.push_back(tally_data[i].error);
		      */
		      tallies[vol_id].ebins.push_back(tally_data[i].energy);
		      tallies[vol_id].result.push_back(tally_data[i].flux);
		      tallies[vol_id].error.push_back(tally_data[i].error);
		      
		    }
		}
	      tallies.insert(tmp_tallies.begin(), tmp_tallies.end());

	      /*
	      std::vector<double> :: iterator it_a;
	      for ( it = tallies.begin() ; it != tallies.end() ; ++it )
		{
		  std::cout << (it->first) << " " << (it->second).entity_size << std::endl;
		  for ( it_a = (it->second).ebins.begin() ; it_a != (it->second).ebins.end() ; it_a++ )
		    {
		      std::cout << *it_a << std::endl;
		    }
		}
	      */    
	    }
	}
      data.close();
    }

  /*
  std::map<int,tally_struct>::iterator it;
  for ( it = tallies.begin() ; it != tallies.end() ; ++it )
    {
      std::cout << it->first << std::endl;
    }
  */
  //  print_tally(tallies,vol_query);

  return tallies;
}

// for a given tally prints out complete description
void print_tally(std::map<int,tally_struct> tallies, int tally_id, std::ostream &ostream)
{
  tally_struct tally;
  tally = tallies[tally_id];
  
  ostream << "#tally " << tally_id << std::endl;
  for ( int i = tally.ebins.size() - 1 ; i != 0 ; --i )
    {
      ostream << tally.ebins[i] << " " << tally.result[i] << " " << tally.error[i] << std::endl;
    }
  return;
}

// reads and creates the map of tally vs id number
std::map<int,tally_struct> read_tally_volumes(std::ifstream &data)
{
  bool volume_data = true; // if we are still reading the data
  std::map<int,tally_struct> initial_data; //the output data
  std::string line; // the current line being read

  tally_struct current_tally; 

  std::getline(data,line); // pop new line
  while ( volume_data )
    {
      if( line.find("cell") != std::string::npos )
	{
	  //	  std::cout << line << std::endl;
	  // tokenize the string to get volume ids
	  std::vector<std::string> volid_tokens;
	  std::istringstream iss(line);
	  do {
	    std::string token;
	    iss >> token;
	    volid_tokens.push_back(token);
	  } while (iss);

 	  std::getline(data,line); // get the volume line
	  iss.str(""); // clear the string stream
	  //	  std::cout << "line="<<line << std::endl;

 	  std::istringstream new_iss(line);
	  std::vector<std::string> volume_tokens;
	  do {
	    std::string token;
	    new_iss >> token;
	    volume_tokens.push_back(token);
	  } while (new_iss);

	  // there is one less volume token than volid tokens
	  for ( int i = 0 ; i < volume_tokens.size()-1 ; i++ )
	    {
	      current_tally.entity_number = std::stoi(volid_tokens[i+1]);
	      current_tally.entity_size = std::stof(volume_tokens[i]);
	      initial_data[current_tally.entity_number] = current_tally;
	    }

	  // move onto the next line
	  std::getline(data,line);
	}
      if ( line.find("cell") == std::string::npos )
	volume_data = false;
      
    }
  
  return initial_data;
}

// reads a complete single tally for a single entity, includes energy bins
std::vector<flux_result> read_tally_atomic(std::ifstream &data, int &vol_id)
{
  std::vector<flux_result> tally_data;
  std::string line; // current line read from data 
  flux_result tally_r;
  double energy,flux,error;

  // next line should be "cell x"
  std::getline(data,line);

  std::string vol_id_str;

  std::size_t pos = line.find("cell");
  pos += 5;
  vol_id_str = line.substr(pos);
  vol_id = std::atoi(vol_id_str.c_str());

  //  std::cout << "atomic = " << line << std::endl;
  std::getline(data,line);
  //  std::cout << "atomic = " << line << std::endl;

  std::string::size_type sz;

  // should be energy
  if ( line.find("energy") != std::string::npos )
    std::getline(data,line);
  while(line.find("total") == std::string::npos )
    {
      tally_r.energy = std::stod(line,&sz);
      tally_r.flux = std::stod(line.substr(sz));
      tally_r.error = std::stod(line.substr((2*sz)+1));
      tally_data.push_back(tally_r);
      std::getline(data,line);
    }

  // now we read n flux values until we get a total line
  std::getline(data,line);
  // then do advance the data!  

  return tally_data;
}
