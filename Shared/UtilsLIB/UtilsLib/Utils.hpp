#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdlib>

namespace utils
{
  bool compareTwoDoubles(const double& a, const double& b);

  bool checkWithin(int real, int close, int diff);

  bool checkWithinDouble(double real, double close, double diff);
}

#endif
