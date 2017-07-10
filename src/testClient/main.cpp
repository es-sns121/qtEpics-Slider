
// main.cpp

// main source file for test client application. 

#include <pv/pvaClient.h>

#include <iostream>

using namespace std;
using namespace epics::pvaClient;

int main (int argc, char ** argv) 
{
	string channelName("testRecord");

	bool verbosity(false);
	bool debug(false);
	
	// Handle executable flag.
	if (argc > 1 && argv[1] != NULL) {
		string arg(argv[1]);
		if (arg == "-v") {
		
			verbosity = true;		
	
	// Help flag
		} else if (arg == "-h") {
			
			cout << "Help -- executable flags -- only one accepted at a time\n"
			     << "\t-v (verbose. prints extra ouput)\n"
				 << "\t-d (debug. prints debug information)\n"
				 << "\t-h (help. prints help information)\n";
			return 0;
	
	// Debug flag
		} else if (arg == "-d") {
		
			debug = true;
		
	// Error
		} else {
			
			cout << "Unrecognized option: '" << arg
			     << "'. ('testClient -h' for help.)\n"; 
			return 0;
		
		}
	}
	
	cout << "testClient\n";
	
	try {
		PvaClientPtr pva = PvaClient::get();
		
		cout << "debug : " << (debug ? "true" : "false") << endl;
		
		if (debug) PvaClient::setDebug(true);
		
		PvaClientChannelPtr channel = pva->channel(channelName);

		PvaClientPutPtr put = channel->createPut("");

		PvaClientPutDataPtr putData = put->getData();

		putData->getPVStructure()->dumpValue(cout);
		
	} catch (std::runtime_error e) {	
		
		cerr << "exception: " << e.what() << endl;
		return -1;
	
	}
	
	return 0;
}
