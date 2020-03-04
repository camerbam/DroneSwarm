#include "GameManager.hpp"

#include <iostream>
#include <sstream>

#include "LoggerLib/Logger.hpp"

referee::GameManager::GameManager()
  : m_ready(false),
    m_graph(),
    m_messages(),
    m_start(std::chrono::steady_clock::now())
{
}

referee::GameManager::~GameManager()
{
}

std::vector<Target> referee::GameManager::startGame()
{
  return m_graph.getAvailable();
}

boost::optional<referee::GraphResponse> referee::GameManager::hitTarget(
  const Target& target)
{
  if (!m_graph.validateTarget(target)) return boost::none;

  return m_graph.hitTarget(target);
}

std::vector<Target> referee::GameManager::finish()
{
  auto left = m_graph.getAllLeft();
  auto done = m_graph.getAllDone();

  std::stringstream ss;
  if (!done.empty())
  {
    ss << "Targets hit successfully: " << std::endl;
    for (auto&& t : done)
      ss << t.getX() << " " << t.getY() << " " << t.getId() << std::endl;
  }

  std::cout << std::endl;
  if (!left.empty())
  {
    ss << "Missed targets: " << std::endl;
    for (auto&& t : left)
      ss << t.getX() << " " << t.getY() << " " << t.getId() << std::endl;
  }

  std::cout << std::endl;
  ss << "Total time: "
            << std::chrono::duration_cast<std::chrono::seconds>(
                 std::chrono::steady_clock::now() - m_start)
                 .count()
            << std::endl;

  ss << "Messages Received: " << std::endl;
  for (auto&& m : m_messages)
    ss << m.first << ": " << m.second << std::endl;

  logger::logInfo("GameManager", ss.str());

  return left;
}

void referee::GameManager::addMsg(const std::string& msg)
{
  m_messages[msg]++;
}

bool referee::GameManager::isDone()
{
  return m_graph.isDone();
}
