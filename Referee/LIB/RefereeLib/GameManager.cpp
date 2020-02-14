#include "GameManager.hpp"

#include <iostream>

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

  if (!left.empty())
  {
    std::cout << "Missed targets: " << std::endl;
    for (auto&& t : left)
      std::cout << t.getX() << " " << t.getY() << " " << t.getId() << std::endl;
  }

  std::cout << "Total time: "
            << std::chrono::duration_cast<std::chrono::seconds>(
                 std::chrono::steady_clock::now() - m_start)
                 .count()
            << std::endl;

  std::cout << "Messages Received: " << std::endl;
  for (auto&& m : m_messages)
    std::cout << m.first << ": " << m.second << std::endl;

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
