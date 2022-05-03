#include <gtest/gtest.h>

#include <gsl_wrapper/vector.h>

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

using gsl_wrapper::Vector;

TEST(VectorTest, ConstructingVector)
{
  Vector(80);
}

TEST(VectorTest, ConstructinFromGslPointer)
{
  auto ptr = gsl_vector_calloc(10);
  Vector test_subject(ptr);

  ASSERT_EQ(test_subject.size(), 10);
  ASSERT_EQ(test_subject.get_gsl_vector(), ptr);
}

TEST(VectorTest, ListConstructor)
{
  Vector test_subject{1, 2, 3, 4, 5};
  ASSERT_EQ(test_subject.size(), 5);

  for (size_t i = 0; i < 5; i++)
  {
    ASSERT_EQ(test_subject[i], static_cast<double>(i + 1));
  }
}

TEST(VectorTest, StandardVectorConstructor)
{
  std::vector<int> elements_i(300);
  std::vector<double> elements_d(300);
  std::vector<float> elements_f(300);

  std::generate(elements_i.begin(), elements_i.end(), [n = 0]() mutable
                { return n++; });
  std::generate(elements_d.begin(), elements_d.end(), [n = 0.0]() mutable
                { return n += 0.5; });

  std::generate(elements_f.begin(), elements_f.end(), [n = 0.0]() mutable
                { return n += 0.5; });

  Vector from_doubles(elements_d);
  Vector from_ints(elements_i);
  Vector from_floats(elements_f);

  for (auto [i, n_i, n_f, n_d] = std::tuple(0u, 0, 0.0f, 0.0); i < from_doubles.size(); i++)
  {
    EXPECT_EQ(from_doubles[i], n_d += 0.5);
    EXPECT_EQ(from_floats[i], n_f += 0.5);
    EXPECT_EQ(from_ints[i], n_i++);
  }
}

TEST(VectorTest, CopyConstructor)
{
  Vector copy_from(90);
  Vector copy_to = copy_from;

  ASSERT_EQ(copy_to.size(), copy_from.size());
  ASSERT_NE(copy_from.get_gsl_vector(), copy_to.get_gsl_vector());
}

TEST(VectorTest, CopyAssigment)
{
  Vector copy_from(90);
  Vector copy_to(40);
  copy_to = copy_from;

  ASSERT_EQ(copy_to.size(), copy_from.size());
  ASSERT_NE(copy_from.get_gsl_vector(), copy_to.get_gsl_vector());
}

TEST(VectorTest, MoveConstructor)
{
  Vector move_from(110);
  Vector move_to = std::move(move_from);

  ASSERT_EQ(move_from.get_gsl_vector(), nullptr);
  ASSERT_EQ(move_from.size(), 0);

  ASSERT_NE(move_to.get_gsl_vector(), nullptr);
  ASSERT_NE(move_to.size(), 0);
}

TEST(VectorTest, ElementAccess)
{
  Vector test_subject(10);
  for (int i = 1; i <= 10; i++)
  {
    test_subject[i - 1] = i;
  }

  for (int i = 1; i <= 10; i++)
  {
    ASSERT_EQ(test_subject[i - 1], i);
  }

  EXPECT_THROW({
    test_subject[10];
  },
               std::range_error);
}

TEST(VectorTest, PrintTest)
{
  Vector test_subject{1, 2, 3, 4, 5};
  std::stringstream stream;
  stream << test_subject;

  std::string output = stream.str();
  EXPECT_STREQ(output.c_str(), "1 2 3 4 5");
}

TEST(Vectortest, ComparasionOperators)
{
  {
    Vector first{1, 2, 3, 4, 5};
    Vector second{1, 2, 3, 4, 5};
    ASSERT_TRUE(first == second);
    ASSERT_FALSE(first != second);
  }

  {
    Vector third{2, 3};
    Vector fourth{5, 4, 3, 2, 1};
    ASSERT_FALSE(third == fourth);
    ASSERT_TRUE(third != fourth);
  }
}

TEST(VectorTest, Iterators)
{
  Vector test_subject(10);

  int counter = 1;

  for (auto &&el : test_subject)
  {
    el = counter++;
  }
  counter = 1;

  for (auto &&el : test_subject)
  {
    ASSERT_EQ(el, counter++);
  }

  ASSERT_EQ(test_subject[0], 1);
}

TEST(VectorTest, Adding)
{
  Vector first = {1, 2, 3, 4, 5};
  Vector second = {1, 2, 3, 4, 5};

  Vector result = first + second;

  for (size_t i = 0; i < 5; i++)
  {
    ASSERT_EQ(result[i], (i + 1) * 2);
  }
}

TEST(VectorTest, Subtracting)
{
  Vector first = {1, 2, 3, 4, 5};
  Vector second = {1, 2, 3, 4, 5};

  Vector result = first - second;

  for (size_t i = 0; i < 5; i++)
  {
    ASSERT_EQ(result[i], 0);
  }
}

TEST(VectorTest, NumberMul)
{
  {

    Vector target{1, 1, 1, 1, 1};
    Vector result = target * 10;

    ASSERT_EQ(result.size(), 5);
    for (auto &&el : result)
    {
      ASSERT_EQ(el, 10);
    }
  }

  {

    Vector target{1, 1, 1, 1, 1};
    Vector result = 10 * target;

    ASSERT_EQ(result.size(), 5);
    for (auto &&el : result)
    {
      ASSERT_EQ(el, 10);
    }
  }
}