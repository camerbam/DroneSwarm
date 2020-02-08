
#include <iostream>
#include <string>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "RegistryLib/Registry.hpp"
#include "DroneSimulatorLib/DroneSimulator.hpp"

int main(int argc, char* argv[])
{
  try
  {
    boost::program_options::options_description desc{ "Options" };
    desc.add_options()("help,h", "Help screen")(
      "config,c",
      boost::program_options::value<std::string>());
      //("ip", boost::program_options::value<std::string>(), "IP Address for Drone");

    boost::program_options::variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);
    if (vm.empty() || vm.count("help"))
    {
      std::cout << desc << '\n';
      return 0;
    }
    else if (vm.count("config"))
    {
      auto config = vm["config"].as<std::string>();
      if (!boost::filesystem::exists(config))
        throw std::runtime_error("Cannot access config");
      GlobalRegistry::setRegistry(boost::filesystem::path(config));
    }

    drone::DroneSimulator sim;
  }
  catch (const boost::program_options::error& ex)
  {
    std::cerr << ex.what() << '\n';
  }
  catch (const std::runtime_error& ex)
  {
    std::cerr << ex.what() << '\n';
  }
  return 0;
}
