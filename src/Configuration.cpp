#include "Configuration.hh"


// void Configuration::printPluginInfo( AbstractInterp4Command* pCmd ){
// 	cout << endl;
// 	cout << pCmd->GetCmdName() << endl;
// 	cout << endl;
// 	pCmd->PrintSyntax();
// 	cout << endl;
// 	pCmd->PrintCmd();
// 	cout << endl;
// }

// void Configuration::addLib( string name ){
// 	string file = name;
// 	shared_ptr<LibInterface> libp( new LibInterface(file.c_str()) );
// 	int p = name.find('4')+1; 
// 	int k = name.find('.')-p;
// 	libs[name.substr(p,k)] = libp;
// }

// void Configuration::addLibs( vector<string> names ){
// 	for( string name: names  )
// 		addLib(name);
// }