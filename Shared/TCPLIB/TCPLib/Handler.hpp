#ifndef Hander_hpp
#define Hander_hpp

#include <iostream>
#include <string>

#include <boost/signals2/signal.hpp>

// TODO Break out into two files

namespace tcp
{
  class HandlerBase
  {
  public:
    virtual void execute(const std::string& data) = 0;
  };

  template <class T>
  class Handler : public HandlerBase
  {
  public:
    // TODO handle bad messages
    void execute(const std::string& data)
    {
      std::string name = T::name();

      T msg;
      bool success = msg.parseString(data);
      if (!success) std::cout << "Could not parse data" << std::endl;
      m_signal(msg);
    }

  private:
    boost::signals2::signal<void(T)> m_signal;
  };

  class HandlerMap
  {
  public:
    std::shared_ptr<HandlerBase>& getOrNew(const std::string& name)
    {
      return m_handlers[name];
    }

    std::shared_ptr<HandlerBase> get(const std::string& name)
    {
      auto toReturn = m_handlers.find(name);
      if (toReturn == m_handlers.end()) return std::shared_ptr<HandlerBase>();
      return toReturn->second;
    }

  private:
    std::map<std::string, std::shared_ptr<HandlerBase>> m_handlers;
  };
} // namespace tcp

#endif