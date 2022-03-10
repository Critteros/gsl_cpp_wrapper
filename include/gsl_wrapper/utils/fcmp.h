#pragma once

#include <cmath>

namespace gsl_wrapper::utils {
  inline auto equal(const double x1, const double x2, const double epsilon) -> bool
  {
    // finding the exponent of greater argument
    const double max{ std::max(x1, x2) };
    int exp;
    std::frexp(max, &exp);

    const double delta{ std::ldexp(epsilon, exp) };
    const double diff{ std::abs(x1 - x2) };

    return diff < delta;
  }
}