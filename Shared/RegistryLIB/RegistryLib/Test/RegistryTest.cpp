#define BOOST_TEST_MODULE RegistryTest
#include <condition_variable>
#include <mutex>

#include <boost/asio/post.hpp>
#include <boost/test/unit_test.hpp>

#include "RegistryLib/Registry.hpp"

BOOST_AUTO_TEST_CASE(RegistryTest)
{
  GlobalRegistry::setRegistry(100, 20);
  std::atomic<bool> toPass(false);
  std::condition_variable cv;
  std::mutex m;

  GlobalRegistry::getRegistry().postToThreadPool([&cv, &toPass]() {
    cv.notify_one();
    toPass = true;
  });

  std::unique_lock<std::mutex> lk(m);
  cv.wait_for(lk, std::chrono::milliseconds(1));

  BOOST_CHECK(toPass);
}
