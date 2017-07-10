#include "test.h"

#include <pv/pvaClient.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvaClient;

// Generate a pseudorandom integer value.
long genLong()
{
	return (RAND_MAX / 2) + (rand() % RAND_MAX);
}

// Generate a pseudorandom floating point value;
double genDouble() 
{
	double f = (genLong() % 10000000) + ((double)rand()) / INT_MAX;
	return f;
}

// Generate pseudo random alphanumeric input.
string genString()
{
	// String size is between 10 and 50.
	size_t str_len = (rand() % 41) + 10;
	
	string str;
	str.resize(str_len + 1);
	
	for (size_t i = 0; i < str_len; ++i) {
		// ascii characters 33 -> 126 are printable,
		// not including 'space' which is 32.
		str[i] = (rand()%94) + 33;
	}

	return str;
}

// Not really fifty fifty odds. Thus fittyfitty.
bool fittyfitty()
{
	return rand() % 2;
}

int test(const string & channelName,
		 const bool & debug,
		 const int & iterations,
		 const double & interval)
{

	try {
		
		srand(time(NULL));

		PvaClientPtr pva = PvaClient::get();

		if (debug) PvaClient::setDebug(debug);

		PvaClientChannelPtr channel = pva->channel(channelName);

		if (!channel) {
			cerr << "Channel '" << channelName << "' failed to connect.\n";
			return 1;
		}
	
		PvaClientPutPtr put = channel->createPut("");
		PVStructurePtr pvStr = put->getData()->getPVStructure();

		for (size_t i = 0; i < (unsigned) iterations; ++i) {
			
			struct timespec req; // Requested time to sleep.
			struct timespec rem; // Remaining time to sleep.

			pvStr->getSubField<PVLong>("longInteger1")->put(genLong());
			pvStr->getSubField<PVLong>("longInteger2")->put(genLong());
			
			pvStr->getSubField<PVDouble>("double1")->put(genDouble());
			pvStr->getSubField<PVDouble>("double2")->put(genDouble());
		
			pvStr->getSubField<PVString>("string1")->put(genString());
			pvStr->getSubField<PVString>("string2")->put(genString());

			pvStr->getSubField<PVBoolean>("boolean")->put(fittyfitty());

			put->put();
		
			// Cast to truncate the decimal value off. 
			req.tv_sec = (int) interval;
			// Convert the decimal value to nanoseconds.
			req.tv_nsec = (interval - (int) interval) * 1000000000;
			
			nanosleep(&req, &rem);
		}

	} catch (std::runtime_error err) {
		cerr << err.what() << endl;
		return 1;
	}
	
	return 0;
}
