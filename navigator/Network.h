#include <vector>
#include <map>
#include "Link.h"

class Network
{
 private:
  
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
};
