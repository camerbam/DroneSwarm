#include "Utils.hpp"

#include <cmath>

bool utils::compareTwoDoubles(const double& a, const double& b)
{
  return std::abs(a - b) < .00001;
}

bool utils::checkWithin(size_t real, size_t close, size_t diff)
{
  return static_cast<int>(real) >= static_cast<int>(close - diff) &&
         real <= static_cast<int>(close + diff);
}
