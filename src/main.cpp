#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"

using namespace std;

void printPluginInfo( AbstractInterp4Command* pCmd ){
	cout << endl;
	cout << pCmd->GetCmdName() << endl;
	cout << endl;
	pCmd->PrintSyntax();
	cout << endl;
	pCmd->PrintCmd();
	cout << endl;
}

int main()
{
  void *pLibHnd_Move = dlopen("libInterp4Move.so",RTLD_LAZY);
  void *pLibHnd_Pause = dlopen("libInterp4Pause.so",RTLD_LAZY);
  void *pLibHnd_Rotate = dlopen("libInterp4Rotate.so",RTLD_LAZY);
  void *pLibHnd_Set = dlopen("libInterp4Set.so",RTLD_LAZY);



  AbstractInterp4Command *(*pCreateCmd_Move)(void);
  AbstractInterp4Command *(*pCreateCmd_Pause)(void);
  AbstractInterp4Command *(*pCreateCmd_Rotate)(void);
  AbstractInterp4Command *(*pCreateCmd_Set)(void);




  void *pFun;

  if (!pLibHnd_Move) {
    cerr << "!!! Brak biblioteki: Interp4Move.so" << endl;
    return 1;
  }


  pFun = dlsym(pLibHnd_Move,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Move = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);
  pFun = dlsym(pLibHnd_Pause,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Pause = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);
  pFun = dlsym(pLibHnd_Rotate,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Rotate = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);
  pFun = dlsym(pLibHnd_Set,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Set = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);


  AbstractInterp4Command* pCmd[4];
  pCmd[0]=pCreateCmd_Move();
  pCmd[1]=pCreateCmd_Pause();
  pCmd[2]=pCreateCmd_Rotate();
  pCmd[3]=pCreateCmd_Set();

for (size_t i = 0; i < 4; i++)
{
  printPluginInfo(pCmd[i]);
  delete pCmd[i];
}

  dlclose(pLibHnd_Move);
  dlclose(pLibHnd_Pause);
  dlclose(pLibHnd_Rotate);
  dlclose(pLibHnd_Set);
}
