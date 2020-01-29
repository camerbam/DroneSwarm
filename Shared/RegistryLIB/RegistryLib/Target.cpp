
#include "Target.hpp"

#include <algorithm>
#include <set>

#include "UtilsLib/Utils.hpp"

Target::Target(
  double x, double y, int id, std::set<int> dependents, int falseAfter)
  : m_x(x), m_y(y), m_id(id), m_dependents(dependents), m_falseAfter(falseAfter)
{
}

double Target::getX() const
{
  return m_x;
}

double Target::getY() const
{
  return m_y;
}

int Target::getId() const
{
  return m_id;
}

std::set<int> Target::getDependents() const
{
  return m_dependents;
}

int Target::getFalseAfter() const
{
  return m_falseAfter;
}

void Target::removeDependent(int id)
{
  m_dependents.erase(id);
}

bool Target::isReady() const
{
  return m_dependents.empty();
}

bool operator==(const Target& lhs, const Target& rhs)
{
  return utils::compareTwoDoubles(lhs.getX(), rhs.getX()) &&
         utils::compareTwoDoubles(lhs.getY(), rhs.getY()) &&
         utils::compareTwoDoubles(lhs.getId(), rhs.getId());
}
