#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include <cstdio>
#include <sstream>
#include <vector>
#include <memory>
#include <map>
#include "AbstractInterp4Command.hh"
#include "AbstractScene.hh"
#include "AbstractComChannel.hh"


#include "xmlhandler.hh"
#include "Configuration.hh"
#include "klient.hh"


using namespace std;

int main()
{
  Configuration   Config;
  std::vector<std::string> libNames = {"libInterp4Move.so", "libInterp4Pause.so", "libInterp4Rotate.so", "libInterp4Set.so"};
  Config.addLibs(libNames);
if (!ReadFile("config/config.xml",Config)) return 1;

  klient(Config);

}
