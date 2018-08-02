#include <string>
#include <vector>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "Parse.hpp"

int main(int argv, char **argc)
{
  namespace po = boost::program_options;

  int opt;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "Produce help message")
    ("input-file",  po::value<std::vector<std::string> >(), "Input file[s]");

  po::positional_options_description p;
  p.add("input-file", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argv, argc).options(desc).positional(p).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << "Usage: dlis [options]\n";
    std::cout << desc;
    return 1;
  }

  if (vm.count("input-file"))
  {
    auto const & vv = vm["input-file"].as<std::vector<std::string> >();

    std::cout << "Input files are: " << std::endl;
    for (auto const & v : vv)
    {
      std::cout << v << std::endl;

      namespace bf = boost::filesystem;

      try
      {
        bf::path p = bf::canonical(v);
        DLIS::parse(p.string());
      }
      catch(bf::filesystem_error)
      {
        std::cout << "Failed to locate the file" << std::endl;
      }

    }
  }
  else
  {
    std::cout << "No input files" << std::endl;

    return -1;
  }


  return 0;
}
