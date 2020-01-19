#include "DroneStatusMessage.hpp"

#include <boost/algorithm/string/trim.hpp>
#include <map>
#include <sstream>

namespace
{
  std::string getNext(std::string& toParse, const std::string& delimiter)
  {
    auto delimiterLocation = toParse.find(delimiter);
    if (delimiterLocation == std::string::npos)
      delimiterLocation = toParse.size();
    std::string toReturn(toParse.substr(0, delimiterLocation));
    toParse.erase(0, delimiterLocation + 1);
    return toReturn;
  }

  std::string getNextKey(std::string& toParse) { return getNext(toParse, ":"); }

  double getNextValue(std::string& toParse)
  {
    try
    {
      return std::stod(getNext(toParse, ";"));
    }
    catch (std::invalid_argument& ec)
    {
      throw std::runtime_error(std::string("Invalid status message: ") +
                               ec.what());
    }
  }

  std::map<std::string, double> parseStatusMessage(std::string& toParse)
  {
    boost::algorithm::trim_right(toParse);
    std::map<std::string, double> toReturn;
    while (!toParse.empty())
    {
      auto key = getNextKey(toParse);
      auto value = getNextValue(toParse);
      toReturn[key] = value;
    }
    return toReturn;
  }
}

messages::DroneStatusMessage::DroneStatusMessage()
  : m_zCoordinate(0), m_timeOfFlight(0), m_battery(0), m_time(0)
{
}

std::string messages::DroneStatusMessage::toString(int mid,
                                                   double xCoordinate,
                                                   double yCoordinate,
                                                   double zCoordinate,
                                                   size_t timeOfFlight,
                                                   size_t battery,
                                                   size_t time) const
{
  std::stringstream ss;
  ss << "mid:" << mid << ";x:" << xCoordinate << ";y:" << yCoordinate
     << ";z:" << zCoordinate
     << ";mpry:0,0,0;pitch:0;roll:0;yaw:0;vgx:0;vgy:0;vgz:0;"
        "templ:85;temph:87;tof:"
     << timeOfFlight << ";h:" << zCoordinate << ";bat:" << battery
     << ";baro:1263.39;time:" << time << ";agx:3.00;agy:-4.00;agz:-1008.00;";
  return ss.str();
}

void messages::DroneStatusMessage::fromString(std::string message)
{
  auto values = parseStatusMessage(message);
  m_mid = static_cast<int>(values["mid"]);
  m_time = static_cast<size_t>(values["time"]);
  m_battery = static_cast<size_t>(values["bat"]);
  m_xCoordinate = values["x"];
  m_yCoordinate = values["y"];
  m_zCoordinate = values["h"];
  m_timeOfFlight = values["tof"];
}

int messages::DroneStatusMessage::getMid()
{
  return m_mid;
}

double messages::DroneStatusMessage::getXCoordinate()
{
  return m_xCoordinate;
}

double messages::DroneStatusMessage::getYCoordinate()
{
  return m_yCoordinate;
}

double messages::DroneStatusMessage::getZCoordinate()
{
  return m_zCoordinate;
}

size_t messages::DroneStatusMessage::getTimeOfFlight()
{
  return m_timeOfFlight;
}

size_t messages::DroneStatusMessage::getBattery()
{
  return m_battery;
}

size_t messages::DroneStatusMessage::getTime()
{
  return m_time;
}
