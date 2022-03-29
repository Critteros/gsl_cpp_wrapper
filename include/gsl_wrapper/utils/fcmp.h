#pragma once

#include <cmath>
#include <numeric>

namespace gsl_wrapper::utils
{
  inline auto equal_epsilon(const double x1, const double x2, const double epsilon) -> bool
  {
    // finding the exponent of greater argument
    const double max{std::max(x1, x2)};
    int exp;
    std::frexp(max, &exp);

    const double delta{std::ldexp(epsilon, exp)};
    const double diff{std::abs(x1 - x2)};

    return diff < delta;
  }
  inline auto equal(const double d1, const double d2) -> bool
  {
    return (std::fabs(d1 - d2) <= std::numeric_limits<double>::epsilon() * std::fmax(std::fabs(d1), std::fabs(d2)));
  }

}