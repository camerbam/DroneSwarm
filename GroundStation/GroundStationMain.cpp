#include "GroundStationLib/GroundStationController.hpp"
#include "GroundStationLib/PretestController.hpp"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

int main(int argc, char* argv[])
{
  try
  {
    boost::program_options::options_description desc{"Options"};
    // clang-format off
    desc.add_options()("help,h", "Help screen")
      ("config,c", boost::program_options::value<std::string>());
    // clang-format on

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

    auto pretest = GlobalRegistry::getRegistry().getPretest();
    if (pretest)
    {
      ground::PretestController pretest("localhost", "65000");
      return 0;
    }
    ground::GroundStationController ground("localhost", "65000");

    std::cout << "Enter ready to start" << std::endl;
    std::string line;

    while (std::getline(std::cin, line) && line != "ready")
      ;

    ground.start();
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
