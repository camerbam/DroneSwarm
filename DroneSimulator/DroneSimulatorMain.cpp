
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "DroneSimulatorLib/DroneSimulator.hpp"
#include "RegistryLib/Registry.hpp"

int main(int argc, char* argv[])
{
  try
  {
    boost::program_options::options_description desc{"Options"};
    desc.add_options()("help,h", "Help screen")(
      "config,c", boost::program_options::value<std::string>())(
      "port,p", boost::program_options::value<unsigned short>()->required())(
      "y,y", boost::program_options::value<int>(), "Starting y position");

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

    int y(0);
    if (vm.count("y"))
    {
      y = vm["y"].as<int>();
    }

    drone::DroneSimulator sim(
      vm["port"].as<unsigned short>(), boost::posix_time::seconds(15), 100, y);
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
