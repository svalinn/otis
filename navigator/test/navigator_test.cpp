#include <iostream>
#include <gtest/gtest.h>


#ifndef NETWORK_H
#define NETWORK_H 1
#include "Network.h"
#endif

#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP 1
#include "Navigation.hpp"
#endif

class NetworkTest : public ::testing::Test
{
  protected:
    virtual void SetUp()
    {
    }
  protected:
    Network *net = new Network();

};

class NavigationTest : public ::testing::Test
{
  protected:
    virtual void SetUp()
    {
    }
  protected:
    Network *net = new Network();

};


// for an empty network expect no links
TEST_F(NetworkTest,CheckLink)
{
  EXPECT_EQ(net->LinkExists(1,2),false);
}

// add a link check for link exist
TEST_F(NetworkTest,AddLink)
{
  net->AddLink(1,2);
  EXPECT_EQ(net->LinkExists(1,2),true);
}

// add a link check for link exist true
TEST_F(NetworkTest,AddLinkF)
{
  net->AddLink(1,2);
  EXPECT_NE(net->LinkExists(1,2),false);
}

// add a link, check neighour
TEST_F(NetworkTest,Adjacent)
{
  net->AddLink(1,2);
  std::vector<int> adj_nodes = net->GetAdjNodeIDs(1);
  EXPECT_EQ(adj_nodes.size(),1);
  EXPECT_EQ(adj_nodes[0],2);
}

// add a link, check other neighour
TEST_F(NetworkTest,AdjacentOther)
{
  net->AddLink(1,2);
  std::vector<int> adj_nodes = net->GetAdjNodeIDs(2);
  EXPECT_EQ(adj_nodes.size(),1);
  EXPECT_EQ(adj_nodes[0],1);
}

// add 2 links, check neighbours
TEST_F(NetworkTest,Adjacent2Link)
{
  net->AddLink(1,2);
  net->AddLink(1,3);
  std::vector<int> adj_nodes = net->GetAdjNodeIDs(1);
  EXPECT_EQ(adj_nodes.size(),2);
  EXPECT_EQ(adj_nodes[0],2);
  EXPECT_EQ(adj_nodes[1],3);
}

// test network to see if bidirectional
TEST_F(NetworkTest,CheckBiDirectional)
{
  EXPECT_EQ(net->BiDirectional(),true);
}

// test network to not bidrection 
TEST_F(NetworkTest,CheckNotBiDirectional)
{
  net->SetBiDirection(false);
  EXPECT_EQ(net->BiDirectional(),false);
}

// test network without dfs first
TEST_F(NavigationTest,RoutesNoDFS)
{
  // add links
  net->AddLink(1,2);
  net->AddLink(2,3);
  net->AddLink(1,3);      

  navigation *nav = new navigation(net,1);
  std::map<int,std::vector<int> > routes = nav->get_unique_routes();
  EXPECT_EQ(routes.size(),0);
}

// test network with dfs
TEST_F(NavigationTest,RoutesDFS)
{
  // add links
  net->AddLink(1,2);
  net->AddLink(2,3);
  net->AddLink(1,3);      

  navigation *nav = new navigation(net,1);
  nav->DepthFirst(3);
  std::map<int,std::vector<int> > routes = nav->get_unique_routes();
  EXPECT_EQ(routes.size(),2);
}


// test network with dfs with end target
TEST_F(NavigationTest,RoutesDFSComplex)
{
  // add links
  net->AddLink(1,2);
  net->AddLink(1,6);
  net->AddLink(2,3);      
  net->AddLink(2,4);      
  net->AddLink(3,5);      
  net->AddLink(3,6);      
  net->AddLink(4,5);      
  net->AddLink(5,6);      

  navigation *nav = new navigation(net,1);
  nav->DepthFirst(6);
  std::map<int,std::vector<int> > routes = nav->get_unique_routes();
  EXPECT_EQ(routes.size(),5);
}

// test network with dfs with mid point target
TEST_F(NavigationTest,RoutesDFSMidTarget)
{
  // add links
  net->AddLink(1,2);
  net->AddLink(1,6);
  net->AddLink(2,3);      
  net->AddLink(2,4);      
  net->AddLink(3,5);      
  net->AddLink(3,6);      
  net->AddLink(4,5);      
  net->AddLink(5,6);      

  navigation *nav = new navigation(net,1);
  nav->DepthFirst(3); //dfs with target 3
  std::map<int,std::vector<int> > routes = nav->get_unique_routes();
  EXPECT_EQ(routes.size(),6);
}




