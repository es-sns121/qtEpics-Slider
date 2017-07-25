
// main.cpp

// main source file for test client application. 

#include "test.h"

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
		if (!isdigit(arg[i]))
			return false;
	return true;
}

bool isFloat(const string & arg)
{
	bool decimalFound = false;
	size_t size = arg.length();
	
	for (size_t i = 0; i < size; ++i) {
		if (!isdigit(arg[i]) && arg[i] != '.')
			return false;
		
		if (arg[i] == '.' && decimalFound == false)
			decimalFound = true;	
		else if (arg[i] == '.' && decimalFound == true)
			return false;
	}
	
	return true;
}

void help()
{
	cout << "-- Help --\n"
		 << "\tusage: ./testClient [number of iterations] [seconds between iterations] [-flag]\n"
		 << "\tusage:               default = 10           default = 1.0\n"
		 << "-- executable flags -- only one accepted at a time -- \n"
		 << "\t-d (debug. prints debug information)\n"
		 << "\t-h (help. prints help information)\n";
	
	exit(0);
}

int main (int argc, char ** argv) 
{
	string channelName("testRecord");

	int iterations = 10;
	double interval = 1.0;
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

		} else if (arg == "-h") {
		// Help flag
			
			help();
			
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
	cout << "debug : " << (debug ? "true" : "false") << endl;
	cout << "number of iterations: " << iterations << endl;
	cout << "interval between iterations: " << interval << " seconds" << endl;

	return test(channelName, debug, iterations, interval);
}
