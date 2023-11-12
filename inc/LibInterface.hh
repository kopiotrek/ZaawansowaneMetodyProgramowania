#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"

// #include "MobileObj.hh"


#ifndef  LIBINTERFACE_HH
#define  LIBINTERFACE_HH

class LibInterface{
  public:
  	LibInterface( const char* LibName );
	void* LibHandler;
	std::string CmdName;
	AbstractInterp4Command* (*pCreateCmd)(void);
};

#endif