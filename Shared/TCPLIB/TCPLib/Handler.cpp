#include "Handler.hpp"

void tcp::HandlerMap::add(const std::string& name,
                          std::shared_ptr<HandlerBase> handle)
{
  m_handlers[name] = handle;
}

std::shared_ptr<tcp::HandlerBase> tcp::HandlerMap::get(const std::string& name)
{
  auto toReturn = m_handlers.find(name);
  if (toReturn == m_handlers.end()) return std::shared_ptr<HandlerBase>();
  return toReturn->second;
}
