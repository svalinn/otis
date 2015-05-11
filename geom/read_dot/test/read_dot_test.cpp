#include <iostream>
#include <gtest/gtest.h>

#include "read_dot.hpp"

#ifndef NETWORK_H
#define NETWORK_H 1
#include "Network.h"
#endif

extern std::string dot_file;

class DotReaderTest : public ::testing::Test
{
  protected:
    virtual void SetUp()
    {
      std::string filename("dot.dot");
      DotReader *geom = new DotReader(dot_file,true);
    }
};

TEST_F(DotReaderTest, ValidFile)
{
  DotReader *geom_local = new DotReader(dot_file,true);
  EXPECT_EQ(geom_local->check_network(),true);
}

TEST_F(DotReaderTest, InValidFile)
{
  std::string filename("doty.dot");
  DotReader *geom_local = new DotReader(filename,true);
  EXPECT_EQ(geom_local->check_network(),false);
}

// check that the instanciated network is bidirectional
TEST_F(DotReaderTest, GetNetwork)
{
  // create new network
  DotReader *geom_local = new DotReader(dot_file,false);
  // get a copy of the network
  Network *net = geom_local->get_network();
  EXPECT_EQ(net->LinkExists(1,2),true);
}


// check that the instanciated network is bidirectional
TEST_F(DotReaderTest, Bidirectional)
{
  // create new network
  DotReader *geom_local = new DotReader(dot_file,true);
  // get a copy of the netowrk
  Network *net = geom_local->get_network();
  EXPECT_EQ(net->BiDirectional(),true);
}

// check that the instanciated network is bidirectional
TEST_F(DotReaderTest, NotBidirectional)
{
  // create new network
  DotReader *geom_local = new DotReader(dot_file,false);
  // get a copy of the netowrk
  Network *net = geom_local->get_network();
  EXPECT_EQ(net->BiDirectional(),false);
}

// check that the problem has been loaded correctly
// all metadata is loaded etc
TEST_F(DotReaderTest, CheckProblemMapsTrue)
{
  // create new network
  DotReader *geom_local = new DotReader(dot_file,true);
  EXPECT_TRUE(geom_local->check_network());
}

// check that the problem has not been loaded correctly/
// the metedata is missing the time keyword
TEST_F(DotReaderTest, CheckProblemMapsFalse)
{
  // create new network
  DotReader *geom_local = new DotReader(dot_file,true);
  EXPECT_FALSE(geom_local->check_network());
}
