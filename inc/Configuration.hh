#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include "MobileObjConfig.hh"
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
  public:
  // std::vector<std::string> _Libraries;
  std::vector<MobileObjConfig> _ObjectsConfig;
  map<string,shared_ptr<LibInterface>> libs;


  void addLibs( vector<string> names );
  void addLib( string name );
  void printPluginInfo( AbstractInterp4Command* pCmd );
  // std::vector<std::string> getLibraries();
  
};

#endif
