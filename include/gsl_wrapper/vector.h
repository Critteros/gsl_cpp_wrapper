#pragma once

#include <utility>
#include <iostream>
#include <initializer_list>
#include <exception>

#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>

#include "utils/fcmp.h"

namespace gsl_wrapper
{

  class Vector
  {
  public:
    // Constructors and destructor
    Vector(size_t vec_size);
    Vector(gsl_vector *gsl_vec_ptr);

    Vector(const Vector &copy_from);
    Vector(Vector &&move_from);

    Vector(std::initializer_list<double> args);

    ~Vector();

    // Member functions
    auto get_gsl_vector() const -> gsl_vector *;
    auto size() const -> size_t;
    auto begin() const -> double *;
    auto end() const -> double *;

    // Operators
    auto operator=(const Vector &copy_from) -> Vector &;
    auto operator=(Vector &&move_from) -> Vector &;

    auto operator==(const Vector &comparasion_vector) -> bool;
    auto operator!=(const Vector &comparasion_vector) -> bool;

    auto operator[](const size_t index) -> double &;
    auto operator[](const size_t index) const -> const double &;

    auto operator+(const Vector &add_to) const -> Vector;
    auto operator-(const Vector &sub) const -> Vector;
    auto operator*(const double number) const -> Vector;

    // Friend declarations
    friend auto operator<<(std::ostream &stream, const Vector &to_print) -> std::ostream &;
    friend auto operator*(const double number, const Vector &vec) -> Vector;

  private:
    gsl_vector *m_vector_ptr;
    size_t m_vector_size;
  };

  inline Vector::Vector(size_t vec_size)
      : m_vector_ptr{gsl_vector_calloc(vec_size)},
        m_vector_size{vec_size}
  {
  }

  inline Vector::Vector(gsl_vector *gsl_vec_ptr)
      : m_vector_ptr{gsl_vec_ptr},
        m_vector_size{gsl_vec_ptr->size}
  {
  }

  inline Vector::Vector(const Vector &copy_from)
      : m_vector_ptr{gsl_vector_calloc(copy_from.m_vector_size)},
        m_vector_size{copy_from.m_vector_size}
  {
    gsl_vector_memcpy(m_vector_ptr, copy_from.m_vector_ptr);
  }

  inline Vector::Vector(Vector &&move_from)
      : m_vector_ptr{std::exchange(move_from.m_vector_ptr, nullptr)},
        m_vector_size{std::exchange(move_from.m_vector_size, 0)}
  {
  }

  inline Vector::Vector(std::initializer_list<double> args)
      : m_vector_ptr{gsl_vector_calloc(args.size())},
        m_vector_size{args.size()}
  {
    size_t i = 0;
    for (auto &&el : args)
    {
      gsl_vector_set(m_vector_ptr, i++, el);
    }
  }

  inline Vector::~Vector()
  {
    gsl_vector_free(m_vector_ptr);
  }

  inline auto Vector::get_gsl_vector() const -> gsl_vector *
  {
    return m_vector_ptr;
  }

  inline auto Vector::size() const -> size_t
  {
    return m_vector_size;
  }

  inline auto Vector::begin() const -> double *
  {
    return m_vector_ptr->data;
  }

  inline auto Vector::end() const -> double *
  {
    return m_vector_ptr->data + m_vector_size;
  }

  inline auto Vector::operator=(const Vector &copy_from) -> Vector &
  {
    // Prevent self copy
    if (m_vector_ptr == copy_from.m_vector_ptr)
      return *this;

    gsl_vector_free(m_vector_ptr);
    m_vector_ptr = gsl_vector_calloc(copy_from.m_vector_size);
    gsl_vector_memcpy(m_vector_ptr, copy_from.m_vector_ptr);
    m_vector_size = copy_from.m_vector_size;

    return *this;
  }

  inline auto Vector::operator=(Vector &&move_from) -> Vector &
  {
    // Prevent self move
    if (m_vector_ptr == move_from.m_vector_ptr)
      return *this;

    gsl_vector_free(m_vector_ptr);
    m_vector_ptr = std::exchange(move_from.m_vector_ptr, nullptr);
    m_vector_size = std::exchange(move_from.m_vector_size, 0);

    return *this;
  }

  inline auto Vector::operator==(const Vector &comparasion_vector) -> bool
  {
    if (m_vector_size != comparasion_vector.m_vector_size)
      return false;

    for (size_t i = 0; i < m_vector_size; i++)
    {
      bool test = ::gsl_wrapper::utils::equal((*this)[i], comparasion_vector[i]);
      if (!test)
        return false;
    }
    return true;
  }

  inline auto Vector::operator!=(const Vector &comparasion_vector) -> bool
  {
    return !(*this == comparasion_vector);
  }

  inline auto Vector::operator[](const size_t index) -> double &
  {
    if (index >= m_vector_size)
      throw std::range_error{"Accesing vector elements out of bounds"};
    return *gsl_vector_ptr(m_vector_ptr, index);
  }

  inline auto Vector::operator[](const size_t index) const -> const double &
  {
    return *gsl_vector_const_ptr(m_vector_ptr, index);
  }

  inline auto operator<<(std::ostream &stream, const Vector &to_print) -> std::ostream &
  {
    for (size_t i = 0; i < to_print.m_vector_size - 1; i++)
    {
      stream << to_print[i] << " ";
    }
    stream << to_print[to_print.m_vector_size - 1];

    return stream;
  }

  inline auto Vector::operator+(const Vector &add_to) const -> Vector
  {
    if (m_vector_size != add_to.m_vector_size)
      throw std::range_error{"Adding vector of diffrent sizes"};

    Vector result(m_vector_size);

    for (size_t i = 0; i < m_vector_size; i++)
    {
      result[i] = (*this)[i] + add_to[i];
    }

    return result;
  }

  inline auto Vector::operator-(const Vector &sub) const -> Vector
  {
    if (m_vector_size != sub.m_vector_size)
      throw std::range_error{"Subtracting vector of diffrent sizes"};

    Vector result(m_vector_size);

    for (size_t i = 0; i < m_vector_size; i++)
    {
      result[i] = (*this)[i] - sub[i];
    }

    return result;
  }

  inline auto Vector::operator*(const double number) const -> Vector
  {
    Vector result = *this;
    for (auto &&el : result)
    {
      el *= number;
    }

    return result;
  }

  inline auto operator*(const double number, const Vector &vec) -> Vector
  {
    Vector result = vec;
    for (auto &&el : result)
    {
      el *= number;
    }

    return result;
  }

}