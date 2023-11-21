#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include "MobileObj.hh"
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
  std::vector<MobileObj> _MobileObjects;
  map<string,shared_ptr<LibInterface>> libs;

public:
  void addLibs( vector<string> names );
  void addLib( string name );
  void addMobObj( MobileObj obj );
  void printPluginInfo( AbstractInterp4Command* pCmd );
  // std::vector<std::string> getLibraries();
  map<string,shared_ptr<LibInterface>> getLibraries();
  std::vector<MobileObj> getMobObjs();
  
};

#endif
