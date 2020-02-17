
#include "Target.hpp"

#include <algorithm>
#include <set>

#include "UtilsLib/Utils.hpp"

Target::Target(int x, int y, int id, std::set<int> dependents, int falseAfter)
  : m_x(x), m_y(y), m_id(id), m_dependents(dependents), m_falseAfter(falseAfter)
{
}

int Target::getX() const
{
  return m_x;
}

int Target::getY() const
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
  return lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY() &&
         lhs.getId() == rhs.getId();
}
