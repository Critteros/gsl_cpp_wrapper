#include <gtest/gtest.h>

#include <gsl_wrapper/all.h>

using gsl_wrapper::Matrix;

TEST(MatrixTest, TwoArgsConstructor)
{
  Matrix(3, 4);
}

TEST(MatrixTest, SingleArgConstructor)
{
  Matrix(60);
}

TEST(MatrixTest, TestingCopy)
{

  Matrix base(3, 4);

  Matrix other = base;
  ASSERT_NE(base.get_gsl_matrix(), other.get_gsl_matrix());
  ASSERT_EQ(other, base);

  Matrix operator_test(7, 3);
  operator_test = base;
  ASSERT_NE(base.get_gsl_matrix(), operator_test.get_gsl_matrix());
  ASSERT_EQ(base, operator_test);
}

TEST(MatrixTest, TetsingMove)
{
  Matrix base(7, 8);
  ASSERT_NE(base.get_gsl_matrix(), nullptr);

  Matrix target = std::move(base);
  ASSERT_NE(target.get_gsl_matrix(), nullptr);

  ASSERT_EQ(base.get_gsl_matrix(), nullptr);
}

TEST(MatrixTest, TestingComparasion)
{
  Matrix elements[] = {Matrix(1, 2), Matrix(1, 2), Matrix(3, 4), Matrix(7, 8)};
  ASSERT_EQ(elements[0], elements[1]);
  ASSERT_EQ(elements[1], elements[0]);

  ASSERT_NE(elements[3], elements[4]);
  ASSERT_NE(elements[0], elements[4]);
  ASSERT_NE(elements[1], elements[4]);
  ASSERT_NE(elements[1], elements[3]);
  ASSERT_NE(elements[0], elements[3]);
}