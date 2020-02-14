#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <chrono>
#include <map>
#include <vector>

#include <boost/optional.hpp>

#include "Graph.hpp"

namespace referee
{
  struct GraphResponse;

  class GameManager
  {
  public:
    GameManager();

    ~GameManager();

    std::vector<Target> startGame();

    boost::optional<GraphResponse> hitTarget(const Target& target);

    std::vector<Target> finish();

    void addMsg(const std::string& msg);

    bool isDone();

  private:
    bool m_ready;
    Graph m_graph;
    std::map<std::string, size_t> m_messages;
    std::chrono::steady_clock::time_point m_start;
  };
} // namespace referee

#endif
