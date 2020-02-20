#ifndef STATUS_MANAGER_HPP
#define STATUS_MANAGER_HPP

#include <atomic>
#include <map>
#include <string>
#include <thread>

namespace drone
{
  class StatusManager
  {
  public:
    StatusManager(const std::string& ports);

    ~StatusManager();

  private:
    void parsePorts(std::string ports);

    std::atomic<bool> m_running;
    std::shared_ptr<std::thread> m_pThread;
    std::map<std::string, unsigned short> m_ports;
  };
} // namespace drone

#endif
