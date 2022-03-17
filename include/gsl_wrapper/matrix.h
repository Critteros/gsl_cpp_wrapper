#pragma once

#include <utility>
#include <initializer_list>
#include <exception>
#include <iostream>
#include <cmath>

#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>

#include "bits/matrix-view.h"
#include "utils/fcmp.h"
#include "vector.h"

namespace gsl_wrapper
{
  class Matrix
  {
  public:
    //Constructors and destructor
    Matrix(size_t i, size_t j);
    Matrix(size_t matrix_size);
    Matrix(std::initializer_list<std::initializer_list<double>> args);
    Matrix(const Vector &vec);

    Matrix(const Matrix &copy_from);
    Matrix(Matrix &&move_from);

    ~Matrix();

    //Member functions
    auto get_gsl_matrix() -> gsl_matrix *;
    auto get_dimensions() const -> std::pair<size_t, size_t>;
    auto num_rows() const -> size_t;
    auto num_collumns() const -> size_t;

    //Operators
    auto operator=(const Matrix &copy_from) -> Matrix &;
    auto operator=(Matrix &&move_from) -> Matrix &;

    auto operator==(const Matrix &comparasion_matrix) const -> bool;
    auto operator!=(const Matrix &comparasion_matrix) const -> bool;

    auto operator[](const size_t index) const -> gsl_wrapper::bits::MatrixRow;
    auto operator*(const Matrix &mul) const -> Matrix;
    auto operator*(const double number) const -> Matrix;
    auto operator+(const Matrix &matrix) const -> Matrix;
    auto operator+(const double numer) const -> Matrix;

    //Friend declarations
    friend auto operator<<(std::ostream &stream, const Matrix &matrix) -> std::ostream &;
    friend auto operator*(const double number, const Matrix &matrix) -> Matrix;
    friend auto operator+(const double number, const Matrix &matrix) -> Matrix;

  private:
    gsl_matrix *m_matrixPtr;
    size_t m_numRows;
    size_t m_numCollumns;
  };

  inline Matrix::Matrix(size_t i, size_t j)
      : m_matrixPtr{gsl_matrix_calloc(i, j)}, m_numRows{i}, m_numCollumns{j}
  {
  }

  inline Matrix::Matrix(size_t matrix_size)
      : Matrix(matrix_size, matrix_size)
  {
  }

  inline Matrix::Matrix(std::initializer_list<std::initializer_list<double>> args)
      : m_matrixPtr{nullptr}, m_numCollumns{0}, m_numRows{0}
  {
    if (args.size() == 0)
      return;
    size_t num_rows = args.size();
    size_t num_collumns = (*args.begin()).size();

    //Setting object properties
    m_numCollumns = num_collumns;
    m_numRows = num_rows;
    m_matrixPtr = gsl_matrix_calloc(m_numRows, m_numCollumns);

    size_t row_iterator = 0;
    size_t collumn_iterator = 0;
    for (auto &&row : args)
    {
      if (row.size() != num_collumns)
        throw std::range_error{"Diffrent number of items in diffrent rows when creating matrix"};

      for (auto &&el : row)
      {
        (*this)[row_iterator][collumn_iterator++] = el;
      }
      collumn_iterator = 0;
      ++row_iterator;
    }
  }

  inline Matrix::Matrix(const Vector &vec)
      : m_matrixPtr{gsl_matrix_calloc(vec.size(), 1)},
        m_numCollumns{1},
        m_numRows{vec.size()}
  {
    for (size_t i = 0; i < m_numRows; i++)
    {
      (*this)[i][0] = vec[i];
    }
  }

  inline Matrix::Matrix(const Matrix &copy_from)
      : m_matrixPtr{gsl_matrix_calloc(copy_from.m_numRows, copy_from.m_numCollumns)},
        m_numRows{copy_from.m_numRows},
        m_numCollumns{copy_from.m_numCollumns}
  {
    gsl_matrix_memcpy(m_matrixPtr, copy_from.m_matrixPtr);
  }

  inline Matrix::Matrix(Matrix &&move_from)
      : m_matrixPtr{std::exchange(move_from.m_matrixPtr, nullptr)},
        m_numRows{std::exchange(move_from.m_numRows, 0)},
        m_numCollumns{std::exchange(move_from.m_numCollumns, 0)}
  {
  }

  inline Matrix::~Matrix()
  {
    gsl_matrix_free(m_matrixPtr);
  }

  inline auto Matrix::get_gsl_matrix() -> gsl_matrix *
  {
    return m_matrixPtr;
  }

  inline auto Matrix::get_dimensions() const -> std::pair<size_t, size_t>
  {
    return {m_numRows, m_numCollumns};
  }

  inline auto Matrix::num_rows() const -> size_t
  {
    return m_numRows;
  }
  inline auto Matrix::num_collumns() const -> size_t
  {
    return m_numCollumns;
  }

  inline auto Matrix::operator=(const Matrix &copy_from) -> Matrix &
  {
    //Prevent self copy
    if (*this == copy_from)
      return *this;

    gsl_matrix_free(m_matrixPtr);
    m_matrixPtr = gsl_matrix_calloc(copy_from.m_numRows, copy_from.m_numCollumns);
    gsl_matrix_memcpy(m_matrixPtr, copy_from.m_matrixPtr);

    m_numCollumns = copy_from.m_numCollumns;
    m_numRows = copy_from.m_numRows;

    return *this;
  }

  inline auto Matrix::operator=(Matrix &&move_from) -> Matrix &
  {
    //Prevent self move
    if (*this == move_from)
      return *this;

    gsl_matrix_free(m_matrixPtr);
    m_matrixPtr = std::exchange(move_from.m_matrixPtr, nullptr);
    m_numRows = std::exchange(move_from.m_numRows, 0);
    m_numCollumns = std::exchange(move_from.m_numCollumns, 0);

    return *this;
  }

  inline auto Matrix::operator==(const Matrix &comparasion_matrix) const -> bool
  {

    if ((m_numCollumns != comparasion_matrix.m_numCollumns) || (m_numRows != comparasion_matrix.m_numRows))
      return false;

    for (size_t i = 0; i < m_numRows; i++)
    {
      for (size_t j = 0; j < m_numCollumns; j++)
      {
        bool test = ::gsl_wrapper::utils::equal((*this)[i][j], comparasion_matrix[i][j], 1e-6);
        if (!test)
          return false;
      }
    }
    return true;
  }

  inline auto Matrix::operator!=(const Matrix &comparasion_matrix) const -> bool
  {
    return !(*this == comparasion_matrix);
  }

  inline auto Matrix::operator[](const size_t index) const -> gsl_wrapper::bits::MatrixRow
  {
    using bits::MatrixRow;
    gsl_vector_view view = gsl_matrix_row(m_matrixPtr, index);
    return MatrixRow(view);
  }

  inline auto Matrix::operator*(const Matrix &mul) const -> Matrix
  {
    //Check sizes
    if (m_numCollumns != mul.m_numRows)
      throw std::runtime_error{"Wrong matrix sizes!"};

    Matrix result(m_numRows, mul.m_numCollumns);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, m_matrixPtr, mul.m_matrixPtr, 0.0, result.get_gsl_matrix());

    return result;
  }

  inline auto Matrix::operator*(const double number) const -> Matrix
  {
    Matrix result = *this;
    for (size_t i = 0; i < m_numRows; i++)
    {
      for (size_t j = 0; j < m_numCollumns; j++)
      {
        result[i][j] *= number;
      }
    }

    return result;
  }

  inline auto Matrix::operator+(const Matrix &matrix) const -> Matrix
  {
    if ((m_numCollumns != matrix.m_numCollumns) || (m_numRows != matrix.m_numRows))
      throw std::range_error{"Wrong matrix sizes when adding"};

    Matrix result = *this;
    gsl_matrix_add(result.m_matrixPtr, matrix.m_matrixPtr);

    return result;
  }

  inline auto Matrix::operator+(const double number) const -> Matrix
  {
    Matrix result = *this;
    for (size_t i = 0; i < m_numRows; i++)
    {
      for (size_t j = 0; j < m_numCollumns; j++)
      {
        result[i][j] += number;
      }
    }

    return result;
  }

  inline auto operator<<(std::ostream &stream, const Matrix &matrix) -> std::ostream &
  {
    for (size_t i = 0; i < matrix.m_numRows; i++)
    {
      for (size_t j = 0; j < matrix.m_numCollumns; j++)
      {
        stream << matrix[i][j] << " ";
      }
      stream << std::endl;
    }

    return stream;
  }

  inline auto operator*(const double number, const Matrix &matrix) -> Matrix
  {
    Matrix result = matrix;
    for (size_t i = 0; i < matrix.m_numRows; i++)
    {
      for (size_t j = 0; j < matrix.m_numCollumns; j++)
      {
        result[i][j] *= number;
      }
    }

    return result;
  }

  inline auto operator+(const double number, const Matrix &matrix) -> Matrix
  {
    Matrix result = matrix;
    for (size_t i = 0; i < matrix.m_numRows; i++)
    {
      for (size_t j = 0; j < matrix.m_numCollumns; j++)
      {
        result[i][j] += number;
      }
    }

    return result;
  }
}
