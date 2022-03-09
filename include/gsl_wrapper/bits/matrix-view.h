#pragma once

#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>

#include "../vector.h"

namespace gsl_wrapper::bits
{
  class MatrixRow
  {
  public:
    //Consructor
    MatrixRow(gsl_vector_view view);

    //Operators
    operator ::gsl_wrapper::Vector() const;

    auto operator[](const size_t index) -> double &;
    auto operator[](const size_t index) const -> const double &;

  private:
    gsl_vector_view m_view;
  };

  inline MatrixRow::MatrixRow(gsl_vector_view view)
      : m_view{view}
  {
  }

  inline MatrixRow::operator ::gsl_wrapper::Vector() const
  {
    auto space = gsl_vector_calloc(m_view.vector.size);
    gsl_vector_memcpy(space, &m_view.vector);
    return ::gsl_wrapper::Vector(space);
  }

  auto MatrixRow::operator[](const size_t index) -> double &
  {
    return *gsl_vector_ptr(&m_view.vector, index);
  }

  auto MatrixRow::operator[](const size_t index) const -> const double &
  {
    return *gsl_vector_const_ptr(&m_view.vector, index);
  }

}