#include "Graph.hpp"

#include <iostream>

#include "LoggerLib/Logger.hpp"
#include "RegistryLib/Registry.hpp"
#include "UtilsLib/Utils.hpp"

referee::Graph::Graph() : m_ready(), m_notReady(), m_done()
{
  auto& targets = GlobalRegistry::getRegistry().getTargets();
  for (auto&& t : targets)
  {
    if (!canPlaceTarget(t))
    {
      logger::logError("Graph", "Can't put target within 20 cm of another");
      continue;
    }
    if (t.isReady())
      m_ready.emplace_back(t);
    else
      m_notReady.emplace_back(t);
  }
}

referee::Graph::~Graph()
{
}

bool referee::Graph::canPlaceTarget(const Target& target) const
{
  for (auto&& t : m_ready)
  {
    if (utils::checkWithinDouble(target.getX(), t.getX(), 19) &&
        utils::checkWithinDouble(target.getY(), t.getY(), 19))
      return false;
  }

  for (auto&& t : m_notReady)
  {
    if (utils::checkWithinDouble(target.getX(), t.getX(), 19) &&
        utils::checkWithinDouble(target.getY(), t.getY(), 19))
      return false;
  }
  return true;
}

std::vector<Target> referee::Graph::getAvailable()
{
  return m_ready;
}

bool referee::Graph::validateTarget(const Target& target) const
{
  for (auto&& t : m_ready)
  {
    if (utils::checkWithin(target.getX(), t.getX(), 10) &&
        utils::checkWithin(target.getY(), t.getY(), 10) &&
        target.getId() == t.getId())
      return true;
  }
  return false;
}

referee::GraphResponse referee::Graph::hitTarget(const Target& target)
{
  referee::GraphResponse toReturn;
  m_ready.erase(
    std::remove(m_ready.begin(), m_ready.end(), target), m_ready.end());

  for (auto&& t : m_notReady)
  {
    auto dependents = t.getDependents();
    if (std::find(dependents.begin(), dependents.end(), target.getId()) !=
        dependents.end())
    {
      t.removeDependent(target.getId());
      if (t.isReady())
      {
        toReturn.targetsToAdd.push_back(t);
        m_ready.emplace_back(t);
      }
    }
  }
  // remove from not yet ready
  for (auto&& t : toReturn.targetsToAdd)
  {
    m_notReady.erase(
      std::remove(m_notReady.begin(), m_notReady.end(), t), m_notReady.end());
  }

  for (size_t i = 0; i < m_ready.size(); i++)
  {
    if (m_ready[i].getFalseAfter() == target.getId())
    {
      toReturn.targetsToRemove.emplace_back(m_ready[i]);
      m_ready.erase(m_ready.begin() + target.getId());
      i--;
    }
  }

  for (size_t i = 0; i < m_notReady.size(); i++)
  {
    if (m_notReady[i].getFalseAfter() == target.getId())
    {
      m_notReady.erase(m_notReady.begin() + target.getId());
      i--;
    }
  }

  m_done.emplace_back(target);
  return toReturn;
}

bool referee::Graph::isDone()
{
  return m_notReady.empty() && m_ready.empty();
}

std::vector<Target> referee::Graph::getAllLeft()
{
  std::vector<Target> toReturn;
  toReturn.reserve(m_notReady.size() + m_ready.size()); // preallocate memory
  toReturn.insert(toReturn.end(), m_notReady.begin(), m_notReady.end());
  toReturn.insert(toReturn.end(), m_ready.begin(), m_ready.end());
  return toReturn;
}

std::vector<Target> referee::Graph::getAllDone()
{
  return m_done;
}
