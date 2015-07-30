#include <vector>
#include <map>
#include "Link.h"

class Network
{
 private:
  
  std::map<std::pair<int,int>, double > boundary_areas;
  std::map<int,double> residence_times;
  std::map<int,std::vector<int> > problem_map;
  std::map< std::pair<int,int>, Link*> link_map;
  bool bi_direction;

 public:

  Network();
  ~Network();

  void Reset();
  void SetBiDirection( bool bi );
  void AddLink( Link* l );
  void AddLink( int s, int d );

  bool LinkExists( int sourceID, int targetID );
  bool BiDirectional();

  std::vector< int > GetAdjNodeIDs( int n ); 

  // modify the residence times according to the boundary maps
  void modify_residence_times(std::map<int,std::vector<int> > routes);

  void set_residence_times(std::map<int,double> residence_times);
  void set_problem_map(std::map<int,std::vector<int> > network_map);

  std::map<int,double> get_residence_times();
  std::map<int,std::vector<int> > get_problem_map();

  // set the volume/volume surface areas
  void set_surface_areas(std::map<std::pair<int,int>, double > surface_areas);

  // set the volume/volume surface areas
  std::map<std::pair<int,int>, double > get_surface_areas();
   
};
