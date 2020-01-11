#include "Utils.hpp"

#include <cmath>

bool utils::compareTwoDoubles(const double& a, const double& b)
{
  return std::abs(a - b) < .00001;
}

bool utils::checkWithin(size_t real, size_t close, size_t diff)
{
  return real >= close - diff && real <= close + diff;
}
