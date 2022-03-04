#pragma once

#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>

namespace gsl_wrapper
{
  class Matrix
  {
  public:
    //Constructors and destructor
    Matrix(size_t i, size_t j);
    Matrix(size_t matrix_size);

    Matrix(const Matrix &copy_from);
    Matrix(Matrix &&move_from);

    ~Matrix();

    //Member functions
    auto get_gsl_matrix() -> gsl_matrix *;
    auto get_dimensions() const -> std::pair<size_t, size_t>;

    //Operators
    auto operator=(const Matrix &copy_from) -> Matrix &;
    auto operator=(Matrix &&move_from) -> Matrix &;

    auto operator==(const Matrix &comparasion_matrix) const -> bool;
    auto operator!=(const Matrix &comparasion_matrix) const -> bool;

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

    m_matrixPtr = std::exchange(move_from.m_matrixPtr, nullptr);
    m_numRows = std::exchange(move_from.m_numRows, 0);
    m_numCollumns = std::exchange(move_from.m_numCollumns, 0);

    return *this;
  }

  inline auto Matrix::operator==(const Matrix &comparasion_matrix) const -> bool
  {
    if ((m_numCollumns != comparasion_matrix.m_numCollumns) || (m_numRows != comparasion_matrix.m_numRows))
      return false;
    return static_cast<bool>(gsl_matrix_equal(m_matrixPtr, comparasion_matrix.m_matrixPtr));
  }

  inline auto Matrix::operator!=(const Matrix &comparasion_matrix) const -> bool
  {
    return !(*this == comparasion_matrix);
  }
}
