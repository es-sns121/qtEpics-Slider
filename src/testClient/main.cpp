
// main.cpp

// main source file for test client application. 

#include <pv/pvaClient.h>

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;
using namespace epics::pvaClient;

bool isInt(const string & arg)
{
	size_t size = arg.length();
	for (size_t i = 0; i < size; ++i)
	{	
		if (!isdigit(arg[i])) {
			return false;
		}
	}
	
	return true;
}

bool isFloat(const string & arg)
{
	bool decimalFound = false;
	size_t size = arg.length();
	
	for (size_t i = 0; i < size; ++i)
	{	
		if (!isdigit(arg[i]) && arg[i] != '.') {
			return false;
		}
		if (arg[i] == '.' && decimalFound == false) {
			decimalFound = true;
		} else if (arg[i] == '.' && decimalFound == true) {
			return false;
		}
	}

	return true;
}

void help()
{
	cout << "-- Help --\n"
		 << "\tusage: ./testClient [number of iterations] [seconds between iterations] [-flag]\n"
		 << "\tusage:               default = 10           default = 1.0\n"
		 << "-- executable flags -- only one accepted at a time -- \n"
		 << "\t-v (verbose. prints extra ouput)\n"
		 << "\t-d (debug. prints debug information)\n"
		 << "\t-h (help. prints help information)\n";
}

int main (int argc, char ** argv) 
{
	string channelName("testRecord");

	int iterations = 10;
	double interval = 1.0;
	bool verbosity(false);
	bool debug(false);
	
	// Handle argument or executable flag.
	for (size_t i = 1; i < (unsigned) argc; ++i) {
		string arg(argv[i]);
		
		if(isInt(arg)) {
			
			iterations = atoi(argv[i]);
			continue;
		} 
		
		if (isFloat(arg)) {
			
			interval = atof(argv[i]);
			continue;
		} 
		
		if (arg == "-v") {
		
			verbosity = true;		
	
		} else if (arg == "-h") {
		// Help flag
			
			help();
			
			return 0;	
		
		} else if (arg == "-d") {
		// Debug flag
			
			debug = true;
		
		} else {
		// Error
			
			cout << "Unrecognized option: '" << arg << "'."
				 << " ('testClient -h' for help.)\n"; 
			return 0;
		
		}
	}

	cout << "testClient\n";
	cout << "iterations: " << iterations << endl;
	cout << "interval: " << interval << endl;

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
