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
#include "LibInterface.hh"

#define LINE_SIZE 1000
#define LIMIT 10000

using namespace std;
  map<string,shared_ptr<LibInterface>> libs;

bool ExecPreprocesor( const char * NazwaPliku, istringstream &IStrm4Cmds )
{
//--------
    string Cmd4Preproc = "cpp -P ";
    char Line[LINE_SIZE];
    ostringstream OTmpStrm;

    Cmd4Preproc += NazwaPliku;
    FILE* pProc = popen(Cmd4Preproc.c_str(),"r");
    if (!pProc) return false;
    while (fgets(Line,LINE_SIZE,pProc)) 
    {
    OTmpStrm << Line;
    }
    IStrm4Cmds.str(OTmpStrm.str());
    return pclose(pProc) == 0;
}

void printPluginInfo( AbstractInterp4Command* pCmd ){
	cout << endl;
	cout << pCmd->GetCmdName() << endl;
	cout << endl;
	pCmd->PrintSyntax();
	cout << endl;
	pCmd->PrintCmd();
	cout << endl;
}

void addLib( string name ){
	string file = name;
	shared_ptr<LibInterface> libp( new LibInterface(file.c_str()) );
	int p = name.find('4')+1; 
	int k = name.find('.')-p;
	libs[name.substr(p,k)] = libp;
}

void addLibs( vector<string> names ){
	for( string name: names  )
		addLib(name);
}

void execCommands( const char* fname ){
    istringstream cmds;
	ExecPreprocesor(fname,cmds);
	while( cmds.peek() != EOF ){
		string cmd;
		cmds >> cmd;
		if( libs[cmd] != NULL ){
				AbstractInterp4Command* pCmd = libs[cmd]->pCreateCmd();
				pCmd->ReadParams(cmds);
				printPluginInfo( pCmd );
				delete pCmd;
		}
		else cout<<"Brak biblioteki " <<  cmd <<endl;

		cmds.ignore(LIMIT,'\n');
	}
}

int main()
{

  std::vector<std::string> libNames = {"libInterp4Move.so", "libInterp4Pause.so", "libInterp4Rotate.so", "libInterp4Set.so"};
  addLibs(libNames);

  execCommands("commands.cmd");

}
