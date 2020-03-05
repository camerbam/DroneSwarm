#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

#include "RegistryLib/Target.hpp"

namespace referee
{
  struct GraphResponse
  {
    std::vector<Target> targetsToAdd;
    std::vector<Target> targetsToRemove;
  };

  class Graph
  {
  public:
    Graph();

    ~Graph();

    bool canPlaceTarget(const Target& target) const;

    std::vector<Target> getAvailable();

    bool validateTarget(const Target& target) const;

    GraphResponse hitTarget(const Target& target);

    bool isDone();

    std::vector<Target> getAllLeft();

    std::vector<Target> getAllDone();

  private:
    std::vector<Target> m_ready;
    std::vector<Target> m_notReady;
    std::vector<Target> m_done;
  };
} // namespace drone

#endif
