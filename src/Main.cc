// -*- C++ -*-

#include <fstream>
#include <iostream>
#include <cstdlib>

#include "ImageAnalyzer.hh"

//______________________________________________________________________________
namespace
{
  enum EArg { kProcess, kNetwork, kCaffeModel,
	      kMean, kLabel, kInput, kArgc };
}

//______________________________________________________________________________
int
main( int argc, char **argv )
{
  std::vector<std::string> arg( argv, argv+argc );

  const std::string& process = arg[kProcess];
  if( argc != kArgc ){
    std::cout << std::endl
	      << "#D Usage: " << ::basename(process.c_str())
	      << " [Network]"
	      << " [CaffeModel]"
	      << " [MeanFile]"
	      << " [LabelFile]"
	      << " [Input]"
	      << std::endl << std::endl;
    return EXIT_SUCCESS;
  }

  ImageAnalyzer analyzer( arg[kNetwork],
			  arg[kCaffeModel],
			  arg[kMean],
			  arg[kLabel],
			  arg[kInput] );

  analyzer.Process();

  return EXIT_SUCCESS;
}
