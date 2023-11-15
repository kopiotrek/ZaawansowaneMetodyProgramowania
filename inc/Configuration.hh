#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include "Cube.hh"
#include <vector>
#include <string>
#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include <cstdio>
#include <sstream>
#include <vector>
#include <memory>
#include <map>

#include "LibInterface.hh"

using namespace std;

class Configuration {
  
  // std::vector<std::string> _Libraries;
  std::vector<Cube> _Cubes;
  map<string,shared_ptr<LibInterface>> libs;

public:
  void addLibs( vector<string> names );
  void addLib( string name );
  void addCube( Cube obj );
  void printPluginInfo( AbstractInterp4Command* pCmd );
  std::vector<std::string> getLibraries();
  std::vector<Cube> getCubes();
  
};

#endif
