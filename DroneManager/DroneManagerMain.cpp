
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "DroneManagerLib/DroneManager.hpp"
#include "RegistryLib/Registry.hpp"
#include "UDPLib/UDPCommunicatorReliable.hpp"
#include "UDPLib/Response.hpp"

namespace
{
  void runPretest(std::string ip)
  {
    udp::UDPCommunicatorReliable com;
    boost::asio::ip::udp::endpoint end(
      boost::asio::ip::address::from_string(ip), 8889);
    com.sendMessage(messages::CommandMessage().toString(), end);
    com.sendMessage(messages::BackMessage(50).toString(), end);
    com.sendMessage(messages::BatteryMessage().toString(), end);
    com.sendMessage(messages::ClockwiseMessage(53).toString(), end);
    com.sendMessage(messages::CounterClockwiseMessage(54).toString(), end);
    com.sendMessage(messages::DownMessage(55).toString(), end);
    com.sendMessage(messages::FlipMessage("f").toString(), end);
    com.sendMessage(messages::ForwardMessage(56).toString(), end);
    com.sendMessage(messages::GoMessage(57, 58, 59, 60).toString(), end);
    com.sendMessage(messages::LeftMessage(61).toString(), end);
    com.sendMessage(messages::MDirectionMessage(1).toString(), end);
    com.sendMessage(messages::MoffMessage().toString(), end);
    com.sendMessage(messages::MonMessage().toString(), end);
    com.sendMessage(messages::RightMessage(62).toString(), end);
    com.sendMessage(messages::SpeedMessage().toString(), end);
    com.sendMessage(messages::TakeoffMessage().toString(), end);
    com.sendMessage(messages::TimeMessage().toString(), end);
    com.sendMessage(messages::UpMessage(63).toString(), end);
    com.sendMessage(messages::LandMessage().toString(), end);
  }
}

int main(int argc, char* argv[])
{
  GlobalRegistry::setRegistry();
  std::string ip;
  std::string sPort;
  std::string mPort;
  try
  {
    boost::program_options::options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen")(
      "ip,i",
      boost::program_options::value<std::string>()->required(),
      "IP Address for Drone")("serverport,s",
                              boost::program_options::value<std::string>(),
                              "Port for the server")(
      "monitorport,m",
      boost::program_options::value<std::string>(),
      "Port for the monitor")("pretest,p", "Run pretest");

    boost::program_options::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) std::cout << desc << '\n';

    notify(vm);

    ip = vm["ip"].as<std::string>();

    if (vm.count("pretest"))
    {
      runPretest(ip);
      return 0;
    }

    sPort = vm["serverport"].as<std::string>();
    if (sPort.empty())
    {
      std::cout << "Must specify server port" << std::endl;
      return 1;
    }
    mPort = vm["monitorport"].as<std::string>();
    if (mPort.empty())
    {
      std::cout << "Must specify monitor port" << std::endl;
      return 1;
    }
  }
  catch (const boost::program_options::error& ex)
  {
    std::cerr << ex.what() << '\n';
    return 1;
  }

  try
  {
    drone::DroneManager manager(ip, sPort, mPort);
    std::cout << "Enter \"exit\" to exit" << std::endl;
    std::string line;
    while (std::getline(std::cin, line))
    {
      if (line == "exit") return 0;
    }
  }
  catch (const std::runtime_error& error)
  {
    std::cout << error.what() << std::endl;
    return 1;
  }

  return 0;
}
