#include <iostream>
#include <gtest/gtest.h>

#include "alara.hpp"

extern std::string mcnp_file;

class AlaraTest : public ::testing::Test
{
  protected:
    virtual void SetUp()
    {
      std::vector<int> path;
      path.push_back(1);
      path.push_back(2);
      path.push_back(3);
      uniq_paths[1] = path;
    }
  public:
  std::map<int, std::vector<int> > uniq_paths;
  
};

// default constructor
TEST_F(AlaraTest, ValidFile)
{
  AlaraOutput *alara = new AlaraOutput();
  EXPECT_EQ(true,true);
}

// named constructor
TEST_F(AlaraTest, ValidFilename)
{
  AlaraOutput *alara = new AlaraOutput(uniq_paths,mcnp_file,"alara_prob");
  EXPECT_EQ(alara->get_filename(),"alara_prob");
}

// named constructor
TEST_F(AlaraTest, ValidMCNPFilename)
{
  AlaraOutput *alara = new AlaraOutput(uniq_paths,mcnp_file,"alara_prob");
  EXPECT_EQ(alara->get_mcnp_filename(),mcnp_file);
}

// test set method
TEST_F(AlaraTest, SetFilename)
{
  AlaraOutput *alara = new AlaraOutput();
  alara->set_filename("test_name");
  EXPECT_EQ(alara->get_filename(),"test_name");
}

// named constructor
TEST_F(AlaraTest, SetMCNPFilename)
{
  AlaraOutput *alara = new AlaraOutput();
  alara->set_mcnp_filename("test_name");
  EXPECT_EQ(alara->get_mcnp_filename(),"test_name");
}

// check instanciated data for correctness
TEST_F(AlaraTest, ValidOutput)
{
  AlaraOutput *alara = new AlaraOutput(uniq_paths,mcnp_file,"alara_prob");
  EXPECT_EQ(alara->check_consistency(),true);
}

// check instanciated data for incorrectness
TEST_F(AlaraTest, InValidOutput)
{
  // local network different to mcnp file
  std::vector<int> paths;
  paths.push_back(1);
  paths.push_back(2);
  paths.push_back(4);
  // local map
  std::map<int, std::vector<int> > local_map;
  local_map[1] = paths;

  AlaraOutput *alara = new AlaraOutput(local_map,mcnp_file,"alara_prob");
  EXPECT_EQ(alara->check_consistency(),false);
}

// check to see if file exists
TEST_F(AlaraTest, FileExists)
{
  // local network different to mcnp file
  AlaraOutput *alara = new AlaraOutput();
  EXPECT_EQ(alara->file_exists(mcnp_file),true);
}

// check to see if file exists
TEST_F(AlaraTest, FileNotExists)
{
  // local network different to mcnp file
  AlaraOutput *alara = new AlaraOutput();
  EXPECT_EQ(alara->file_exists("this_file_cant_actually_exist_really.txt"),false);
}

