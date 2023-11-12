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
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include "xmlinterp.hh"

#define LINE_SIZE 1000
#define LIMIT 10000

using namespace std;
using namespace xercesc;
  map<string,shared_ptr<LibInterface>> libs;

/*!
 * Czyta z pliku opis poleceń i dodaje je do listy komend,
 * które robot musi wykonać.
 * \param sFileName - (\b we.) nazwa pliku z opisem poleceń.
 * \param CmdList - (\b we.) zarządca listy poleceń dla robota.
 * \retval true - jeśli wczytanie zostało zrealizowane poprawnie,
 * \retval false - w przeciwnym przypadku.
 */
bool ReadFile(const char* sFileName, Configuration &rConfig)
{
   try {
            XMLPlatformUtils::Initialize();
   }
   catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cerr << "Error during initialization! :\n";
            cerr << "Exception message is: \n"
                 << message << "\n";
            XMLString::release(&message);
            return 1;
   }

   SAX2XMLReader* pParser = XMLReaderFactory::createXMLReader();

   pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
   pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
   pParser->setFeature(XMLUni::fgXercesDynamic, false);
   pParser->setFeature(XMLUni::fgXercesSchema, true);
   pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);

   pParser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

   DefaultHandler* pHandler = new XMLInterp4Config(rConfig);
   pParser->setContentHandler(pHandler);
   pParser->setErrorHandler(pHandler);

   try {
     
     if (!pParser->loadGrammar("config/config.xsd",
                              xercesc::Grammar::SchemaGrammarType,true)) {
       cerr << "!!! Plik grammar/actions.xsd, '" << endl
            << "!!! ktory zawiera opis gramatyki, nie moze zostac wczytany."
            << endl;
       return false;
     }
     pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse,true);
     pParser->parse(sFileName);
   }
   catch (const XMLException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            cerr << "Informacja o wyjatku: \n"
                 << "   " << sMessage << "\n";
            XMLString::release(&sMessage);
            return false;
   }
   catch (const SAXParseException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            char* sSystemId = xercesc::XMLString::transcode(Exception.getSystemId());

            cerr << "Blad! " << endl
                 << "    Plik:  " << sSystemId << endl
                 << "   Linia: " << Exception.getLineNumber() << endl
                 << " Kolumna: " << Exception.getColumnNumber() << endl
                 << " Informacja: " << sMessage 
                 << endl;

            XMLString::release(&sMessage);
            XMLString::release(&sSystemId);
            return false;
   }
   catch (...) {
            cout << "Zgloszony zostal nieoczekiwany wyjatek!\n" ;
            return false;
   }

   delete pParser;
   delete pHandler;
   return true;
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

// void execCommands( const char* fname ){
//     istringstream cmds;
// 	ExecPreprocesor(fname,cmds);
// 	while( cmds.peek() != EOF ){
// 		string cmd;
// 		cmds >> cmd;
// 		if( libs[cmd] != NULL ){
// 				AbstractInterp4Command* pCmd = libs[cmd]->pCreateCmd();
// 				pCmd->ReadParams(cmds);
// 				printPluginInfo( pCmd );
// 				delete pCmd;
// 		}
// 		else cout<<"Brak biblioteki " <<  cmd <<endl;

// 		cmds.ignore(LIMIT,'\n');
// 	}
// }

int main()
{
  Configuration   Config;
  std::vector<std::string> libNames = {"libInterp4Move.so", "libInterp4Pause.so", "libInterp4Rotate.so", "libInterp4Set.so"};
  addLibs(libNames);
if (!ReadFile("config/config.xml",Config)) return 1;
//   execCommands("commands.cmd");
}
