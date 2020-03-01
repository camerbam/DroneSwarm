#ifndef Hander_hpp
#define Hander_hpp

#include <iostream>
#include <string>

#include <boost/signals2/signal.hpp>

#include "MsgLib/MsgTypes.hpp"

namespace tcp
{
  class HandlerBase
  {
  public:
    virtual void execute(const std::string& data,
                         const msg::FORMAT& format,
                         const std::string& msgId) = 0;
  };

  template <class T>
  class Handler : public HandlerBase
  {
  public:
    void execute(const std::string& data,
                 const msg::FORMAT& format,
                 const std::string& msgId)
    {
      std::string name = T::name();

      T msg;
      bool success = msg::parseString(msg, data, format);
      if (!success)
        std::cout << "Could not parse data" << std::endl;
      m_signal(msg, msgId);
    }

    boost::signals2::signal<void(T, std::string)>& signal() { return m_signal; }

  private:
    boost::signals2::signal<void(T, std::string)> m_signal;
  };

  class HandlerMap
  {
  public:
    HandlerMap();

    void add(const std::string& name, std::shared_ptr<HandlerBase> handle);

    std::shared_ptr<HandlerBase> get(const std::string& name);

  private:
    std::map<std::string, std::shared_ptr<HandlerBase>> m_handlers;
  };
} // namespace tcp

#endif
