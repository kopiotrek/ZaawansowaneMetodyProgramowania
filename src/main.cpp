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

  AbstractComChannel comChannel();
  AbstractScene scene();
  // void *pLibHnd_Move = dlopen("libInterp4Move.so",RTLD_LAZY);
  // void *pLibHnd_Pause = dlopen("libInterp4Pause.so",RTLD_LAZY);
  // void *pLibHnd_Rotate = dlopen("libInterp4Rotate.so",RTLD_LAZY);
  // void *pLibHnd_Set = dlopen("libInterp4Set.so",RTLD_LAZY);



  // AbstractInterp4Command *(*pCreateCmd_Move)(void);
  // AbstractInterp4Command *(*pCreateCmd_Pause)(void);
  // AbstractInterp4Command *(*pCreateCmd_Rotate)(void);
  // AbstractInterp4Command *(*pCreateCmd_Set)(void);
  std::vector<std::string> libNames = {"libInterp4Move.so", "libInterp4Pause.so", "libInterp4Rotate.so", "libInterp4Set"};
  addLibs(libNames);

  // void *pFun;
  // if (!pLibHnd_Move) {
  //   cerr << "!!! Brak biblioteki: Interp4Move.so" << endl;
  //   return 1;
  // }


  // pFun = dlsym(pLibHnd_Move,"CreateCmd");
  // if (!pFun) {
  //   cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
  //   return 1;
  // }
  // pCreateCmd_Move = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);
  // pFun = dlsym(pLibHnd_Pause,"CreateCmd");
  // if (!pFun) {
  //   cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
  //   return 1;
  // }
  // pCreateCmd_Pause = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);
  // pFun = dlsym(pLibHnd_Rotate,"CreateCmd");
  // if (!pFun) {
  //   cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
  //   return 1;
  // }
  // pCreateCmd_Rotate = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);
  // pFun = dlsym(pLibHnd_Set,"CreateCmd");
  // if (!pFun) {
  //   cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
  //   return 1;
  // }
  // pCreateCmd_Set = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);



  // AbstractInterp4Command* pCmd[4];
  // pCmd[0]=pCreateCmd_Move();
  // pCmd[1]=pCreateCmd_Pause();
  // pCmd[2]=pCreateCmd_Rotate();
  // pCmd[3]=pCreateCmd_Set();

// for (size_t i = 0; i < 4; i++)
// {
//   printPluginInfo(pCmd[i]);
//   delete pCmd[i];
// }

  // dlclose(pLibHnd_Move);
  // dlclose(pLibHnd_Pause);
  // dlclose(pLibHnd_Rotate);
  // dlclose(pLibHnd_Set);

  // istringstream streamls;
  // ExecPreprocesor("commands.cmd", streamls);
  execCommands("commands.cmd");

  // cout<<streamls.str()<<endl;
}
