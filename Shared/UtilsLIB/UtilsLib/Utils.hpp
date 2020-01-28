#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdlib>

namespace utils
{
  bool compareTwoDoubles(const double& a, const double& b);

  bool checkWithin(size_t real, size_t close, size_t diff);

  bool checkWithinDouble(double real, double close, double diff);
}

#endif
