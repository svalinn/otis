#include <iostream>
#include <gtest/gtest.h>

#ifndef READ_CGM_HPP
#define READ_CGM_HPP 1
#include "read_cgm.hpp"
#endif

extern std::string cgm_file;

class CGMReaderTest : public ::testing::Test
{
};

TEST_F(CGMReaderTest, ValidFile)
{
  std::cout << cgm_file << std::endl;
  ReadCGM *geom_local = new ReadCGM(cgm_file,true);
  EXPECT_EQ(geom_local->check_network(),true);
}

TEST_F(CGMReaderTest, InValidFile)
{
  std::string filename("doty.dot");
  ReadCGM *geom_local = new ReadCGM(filename,true);
  EXPECT_EQ(geom_local->check_network(),false);
}

// check that the instanciated network is bidirectional
TEST_F(CGMReaderTest, GetNetwork)
{
  // create new network
  ReadCGM *geom_local = new ReadCGM(cgm_file,true);
  // get a copy of the network
  Network *net = geom_local->get_network();
  EXPECT_EQ(net->LinkExists(1,3),true);
}


// check that the instanciated network is bidirectional
TEST_F(CGMReaderTest, Bidirectional)
{
  // create new network
  ReadCGM *geom_local = new ReadCGM(cgm_file,true);
  // get a copy of the netowrk
  Network *net = geom_local->get_network();
  EXPECT_EQ(net->BiDirectional(),true);
}

// check that the instanciated network is bidirectional
TEST_F(CGMReaderTest, NotBidirectional)
{
  // create new network
  ReadCGM *geom_local = new ReadCGM(cgm_file,false);
  // get a copy of the netowrk
  Network *net = geom_local->get_network();
  EXPECT_EQ(net->BiDirectional(),false);
}

// check that the problem has been loaded correctly
// all metadata is loaded etc
TEST_F(CGMReaderTest, CheckProblemMapsTrue)
{
  // create new network
  ReadCGM *geom_local = new ReadCGM(cgm_file,false);
  // get a copy of the netowrk
  //  Network *net = geom_local->get_network();
  EXPECT_TRUE(geom_local->check_network());
}

// check that the problem has not been loaded correctly/
// the metedata is missing the time keyword
TEST_F(CGMReaderTest, CheckProblemMapsFalse)
{
  // create new network
  ReadCGM *geom_local = new ReadCGM(cgm_file,false);
  // get a copy of the netowrk
  //  Network *net = geom_local->get_network();
  EXPECT_FALSE(geom_local->check_network());
}

// the data contains the flowrate keyword
TEST_F(CGMReaderTest, CheckFlowRateMetadata)
{
  // create new network
  ReadCGM *geom_local = new ReadCGM(cgm_file,false);
  // get a copy of the netowrk
  EXPECT_TRUE(geom_local->check_network());
}

