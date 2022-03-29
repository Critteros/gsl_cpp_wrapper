#include <gtest/gtest.h>

#include <gsl_wrapper/matrix.h>

using gsl_wrapper::Matrix;
using gsl_wrapper::Vector;

TEST(Generic, DoubleComparision)
{
  using gsl_wrapper::utils::equal;
  {
    double d1 = 0.0995691;
    double d2 = 0.0995669;
    EXPECT_FALSE(equal(d1, d2));
  }
  {
    double d1 = 0.0995663123123123123123123123123123123123;
    double d2 = 0.0995663123123123123123123123123123123124;
    EXPECT_TRUE(equal(d1, d2));
  }
  {
    double d1 = 0.099566312312312312312312312312312312312123123123123123123124;
    double d2 = 0.099566312312312312312312312312312312312123123123123123123123;
    EXPECT_TRUE(equal(d1, d2));
  }
  {
    double d1 = 0.099566312312312312312312312312312312312123123123123123123123;
    double d2 = 0.099566312312312312312312312312312312312123123123123123123123;
    EXPECT_TRUE(equal(d1, d2));
  }
  {
    double d1 = 3.45e-90;
    double d2 = 3.45e-90;
    EXPECT_TRUE(equal(d1, d2));
  }
  {
    double d1 = 3.46e-90;
    double d2 = 3.45e-90;
    EXPECT_FALSE(equal(d1, d2));
  }
}