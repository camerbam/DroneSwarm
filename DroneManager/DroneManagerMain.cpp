
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "DroneManagerLib/DroneManager.hpp"
#include "RegistryLib/Registry.hpp"

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
      "IP Address for Drone")(
      "serverport,s",
      boost::program_options::value<std::string>()->required(),
      "Port for the server")(
      "monitorport,m",
      boost::program_options::value<std::string>()->required(),
      "Port for the monitor");

    boost::program_options::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) std::cout << desc << '\n';

    notify(vm);

    ip = vm["ip"].as<std::string>();
    sPort = vm["serverport"].as<std::string>();
    mPort = vm["monitorport"].as<std::string>();
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
