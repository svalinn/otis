#ifndef NETWORK_H
#define NETWORK_H 1
#include Network.h
#endif

bool ContainsNode( std::vector<int>& nodes, int node );

void DepthFirst( Network* network, 
		 std::vector<int>& visited, 
		 int end );
