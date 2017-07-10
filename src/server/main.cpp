// main.cpp

// Main entry point for server application. 

// Instantiates and populates EPICS v4 database. Starts pvAccess
// access server to allow connection to records hosted on database

#include <iostream>
#include <string>
#include <vector>

#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>

#include "pv/record.h"

using namespace std;

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

//Defined in src/server/pv/record.h
using namespace Project; 

int main (int argc, char **argv)
{

	bool verbosity(false);

	if (argc > 1 && argv[1]) {
		
		string arg(argv[1]);	
		
		if (arg == string("-v")) {
		// Verbose flag
			verbosity = true;
		
		} else if (arg == string("-h")) {
		// Help flag	
			cout << "Help -- executable flags -- only one accepted at a time" << endl
				 << "\t -v (verbose. prints database record names.)\n"
				 << "\t -h (help. prints help information)\n";
		
			return 0;
		} else {
		// Error
			cout << "unrecognized flag: \"" << arg << "\" (use -h for help)." << endl;
			return 0;
		
		}
	}
	
	
	ChannelProviderLocalPtr cpLocal = getChannelProviderLocal();

	PVDatabasePtr master = Database::create();

	// After the records are added to the database, start the server. 
	ServerContext::shared_pointer pvaServer =
		startPVAServer("local", 0, true, true);
	
	// print the record names currently hosted in the database
	if (verbosity) {
		shared_vector<const string> record_names = master->getRecordNames()->view();
		
		cout << "Records currently hosted:\n";
		for (size_t i = 0; i < record_names.size(); i++) {
			cout << "\t" << record_names[i] << endl;
		}
	}

	// Clear the pointer.
	master.reset();

	// Wait to die.
	string input;
	while (true)
	{
		cout << "Type exit to stop: \n";
		getline(cin, input);
		if (input.compare("exit") == 0) 
		{
			break;
		}
	}

	// Clean up so that we can exit cleanly.
	pvaServer->shutdown();
	pvaServer->destroy();
	cpLocal->destroy();

	return 0;
}

