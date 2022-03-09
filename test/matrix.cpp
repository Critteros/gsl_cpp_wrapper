#include <gtest/gtest.h>

#include <gsl_wrapper/matrix.h>

using gsl_wrapper::Matrix;

TEST(MatrixTest, TwoArgsConstructor)
{
  Matrix(3, 4);
}

TEST(MatrixTest, SingleArgConstructor)
{
  Matrix(60);
}

TEST(MatrixTest, ListConstructor)
{
  Matrix test_subject = {{1, 2}, {3, 4}};

  ASSERT_EQ(test_subject[0][0], 1);
  ASSERT_EQ(test_subject[0][1], 2);
  ASSERT_EQ(test_subject[1][0], 3);
  ASSERT_EQ(test_subject[1][1], 4);
}

TEST(MatrixTest, VectorConstructor)
{
  Matrix test_subject(gsl_wrapper::Vector{1, 2, 3, 4, 5});
  Matrix reference{{1}, {2}, {3}, {4}, {5}};

  ASSERT_TRUE(test_subject == reference);
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

  base = std::move(Matrix(3, 4));
  ASSERT_NE(base.get_gsl_matrix(), nullptr);

  Matrix throw_away(40, 40);
  throw_away = std::move(base);
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

TEST(MatrixTest, ElmentAccess)
{
  Matrix test_subject(5, 5);

  //Testing normal valid access
  {
    int counter = 1;
    for (size_t i = 0; i < 5; i++)
    {
      for (size_t j = 0; j < 5; j++)
      {
        test_subject[i][j] = counter++;
      }
    }

    counter = 1;
    for (size_t i = 0; i < 5; i++)
    {
      for (size_t j = 0; j < 5; j++)
      {
        ASSERT_EQ(test_subject[i][j], counter++);
      }
    }
  }
}

TEST(MatrixTest, MatrixMultiplication)
{
  Matrix first{{0.11, 0.12, 0.13}, {0.21, 0.22, 0.23}};

  Matrix second{{1011., 1012.}, {1021., 1022.}, {1031., 1031.}};

  Matrix expected_result{{367.76, 367.99}, {674.06, 674.49}};

  Matrix result = first * second;

  bool eq = expected_result == result;

  ASSERT_TRUE(eq);
}

TEST(MatrixTest, NumberMultiplication)
{
  {

    Matrix test_subject{{1, 1}, {1, 1}};
    Matrix expected_result{{100, 100}, {100, 100}};
    Matrix result = test_subject * 100;
    ASSERT_TRUE(result == expected_result);
  }

  {

    Matrix test_subject{{1, 1}, {1, 1}};
    Matrix expected_result{{100, 100}, {100, 100}};
    Matrix result = 100 * test_subject;
    ASSERT_TRUE(result == expected_result);
  }
}

TEST(MatrixTest, NumberAddition)
{
  {
    Matrix test_subject{{0, 0}, {0, 0}};
    Matrix expected_result{{50, 50}, {50, 50}};
    Matrix result = test_subject + 50;
    ASSERT_TRUE(result == expected_result);
  }

  {
    Matrix test_subject{{0, 0}, {0, 0}};
    Matrix expected_result{{50, 50}, {50, 50}};
    Matrix result = 50 + test_subject;
    ASSERT_TRUE(result == expected_result);
  }
}

TEST(MatrixTest, MatrixAddition)
{
  Matrix first{{3, 4}, {5, 7}};
  Matrix to_add{{1, 2}, {3, 4}};

  Matrix expected{{4, 6}, {8, 11}};

  Matrix result = first + to_add;

  ASSERT_TRUE(result == expected);
}