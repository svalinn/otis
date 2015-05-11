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

// named constructor
TEST_F(AlaraTest, ValidOutput)
{
  AlaraOutput *alara = new AlaraOutput(uniq_paths,mcnp_file,"alara_prob");
  EXPECT_EQ(alara->check_consistency(),true);
}
