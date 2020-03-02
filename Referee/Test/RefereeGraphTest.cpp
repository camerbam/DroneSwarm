#include <boost/test/unit_test.hpp>

#include <vector>

#include "RefereeLib/Graph.hpp"
#include "RegistryLib/Registry.hpp"

namespace
{
  void checkGoodJob(referee::Graph& graph,
                    const std::vector<Target>& targets,
                    size_t i)
  {
    auto newTargets = graph.hitTarget(targets[i]);
    BOOST_CHECK_EQUAL(newTargets.targetsToAdd.size(), 0);
    BOOST_CHECK_EQUAL(newTargets.targetsToRemove.size(), 0);
  }
}

BOOST_AUTO_TEST_CASE(GraphDependTest)
{
  std::vector<Target> targets;
  targets.emplace_back(0, 20, 1);
  targets.emplace_back(0, 40, 2, std::set<int>{1});
  targets.emplace_back(0, 60, 3, std::set<int>{2});
  targets.emplace_back(0, 80, 4, std::set<int>{3});
  GlobalRegistry::setRegistry(1, 2, targets);

  referee::Graph graph;
  auto available = graph.getAvailable();
  BOOST_REQUIRE(available.size() == 1);
  BOOST_CHECK_EQUAL(available[0].getId(), 1);

  auto newTargets = graph.hitTarget(targets[0]);
  BOOST_REQUIRE(newTargets.targetsToAdd.size() == 1);
  BOOST_CHECK_EQUAL(newTargets.targetsToAdd[0].getId(), 2);

  newTargets = graph.hitTarget(targets[1]);
  BOOST_REQUIRE(available.size() == 1);
  BOOST_CHECK_EQUAL(newTargets.targetsToAdd[0].getId(), 3);

  newTargets = graph.hitTarget(targets[2]);
  BOOST_REQUIRE(available.size() == 1);
  BOOST_CHECK_EQUAL(newTargets.targetsToAdd[0].getId(), 4);

  newTargets = graph.hitTarget(targets[3]);
  BOOST_REQUIRE(newTargets.targetsToAdd.size() == 0);
}

BOOST_AUTO_TEST_CASE(GraphTest)
{
  std::vector<Target> targets;
  targets.emplace_back(0, 20, 1);
  targets.emplace_back(0, 40, 2);
  targets.emplace_back(0, 60, 3);
  targets.emplace_back(0, 80, 4, std::set<int>(), 1);
  targets.emplace_back(0, 100, 5, std::set<int>{3});
  targets.emplace_back(0, 120, 6, std::set<int>{3});
  targets.emplace_back(0, 140, 7, std::set<int>{5, 6});
  targets.emplace_back(0, 160, 8, std::set<int>{1, 2, 3, 4, 5, 6, 7});
  targets.emplace_back(0, 179, 9);
  GlobalRegistry::setRegistry(1, 2, targets);
  referee::Graph graph;
  auto available = graph.getAvailable();
  BOOST_REQUIRE(available.size() == 4);
  BOOST_CHECK_EQUAL(available[0].getId(), 1);
  BOOST_CHECK_EQUAL(available[1].getId(), 2);
  BOOST_CHECK_EQUAL(available[2].getId(), 3);
  BOOST_CHECK_EQUAL(available[3].getId(), 4);

  BOOST_CHECK(graph.validateTarget(targets[3]));
  BOOST_CHECK(!graph.validateTarget(targets[4]));
  {
    auto newTargets = graph.hitTarget(targets[2]);
    BOOST_REQUIRE(newTargets.targetsToAdd.size() == 2);
    BOOST_REQUIRE(newTargets.targetsToRemove.size() == 0);

    BOOST_CHECK_EQUAL(newTargets.targetsToAdd[0].getId(), 5);
    BOOST_CHECK_EQUAL(newTargets.targetsToAdd[1].getId(), 6);
  }
  checkGoodJob(graph, targets, 4);
  {
    auto newTargets = graph.hitTarget(targets[5]);
    BOOST_REQUIRE(newTargets.targetsToAdd.size() == 1);
    BOOST_REQUIRE(newTargets.targetsToRemove.size() == 0);
    BOOST_CHECK_EQUAL(newTargets.targetsToAdd[0].getId(), 7);
  }
  {
    auto newTargets = graph.hitTarget(targets[0]);
    BOOST_REQUIRE(newTargets.targetsToAdd.size() == 0);
    BOOST_REQUIRE(newTargets.targetsToRemove.size() == 1);
    BOOST_CHECK_EQUAL(newTargets.targetsToRemove[0].getId(), 4);
  }

  checkGoodJob(graph, targets, 1);
  checkGoodJob(graph, targets, 3);

  {
    auto newTargets = graph.hitTarget(targets[6]);
    BOOST_REQUIRE(newTargets.targetsToAdd.size() == 1);
    BOOST_REQUIRE(newTargets.targetsToRemove.size() == 0);
    BOOST_CHECK_EQUAL(newTargets.targetsToAdd[0].getId(), 8);
  }

  checkGoodJob(graph, targets, 7);

  BOOST_CHECK_EQUAL(graph.isDone(), true);
}
