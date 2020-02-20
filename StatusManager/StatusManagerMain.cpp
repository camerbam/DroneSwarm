
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "RegistryLib/Registry.hpp"
#include "StatusManagerLib/StatusManager.hpp"
#include "UDPLib/Response.hpp"

int main(int argc, char* argv[])
{
  std::string ports;
  int startingY(0);
  try
  {
    boost::program_options::options_description desc{"Options"};
    // clang-format off
    desc.add_options()
      ("help,h", "Help screen")
      ("config,c", boost::program_options::value<std::string>()->required())
      ("ports,p", boost::program_options::value<std::string>()->required());
    // clang-format on

    boost::program_options::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help"))
    {
      std::cout << desc << '\n';
      return 0;
    }

    notify(vm);

    ports = vm["ports"].as<std::string>();
    auto config = vm["config"].as<std::string>();
    if (!boost::filesystem::exists(config))
      throw std::runtime_error("Cannot access config");
    GlobalRegistry::setRegistry(boost::filesystem::path(config));
  }
  catch (const boost::program_options::error& ex)
  {
    std::cerr << ex.what() << '\n';
    return 1;
  }

  try
  {
    drone::StatusManager manager(ports);

    std::string line;
    while (std::getline(std::cin, line) && line != "exit")
      ;
  }
  catch (const std::runtime_error& error)
  {
    std::cout << error.what() << std::endl;
    return 1;
  }

  return 0;
}
