class Link
{
public:

  Link( int source, int target ) 
  {        
    sourceNode = source;
    targetNode = target;        
  }
    
  void GetSourceTargetNodes( int& source, int& target )
  {
    source = sourceNode;
    target = targetNode;
  }
    
protected:
  int sourceNode;
  int targetNode;
};
