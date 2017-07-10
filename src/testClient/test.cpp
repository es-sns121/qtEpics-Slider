#include "test.h"
#include <pv/pvaClient.h>
#include <iostream>

using namespace std;
using namespace epics::pvaClient;

int test(const string & channelName,
		  const bool & debug,
		  const int & iterations,
		  const double & interval)
{

	try {
		
		PvaClientPtr pva = PvaClient::get();

		if (debug) PvaClient::setDebug(debug);

		PvaClientChannelPtr channel = pva->channel(channelName);

		if (!channel) {
			cerr << "Channel '" << channelName << "' failed to connect.\n";
			return 1;
		}
	
		PvaClientPutPtr put = channel->createPut("");
		PvaClientPutDataPtr putData = put->getData();

		putData->getPVStructure()->dumpValue(cout);

	} catch (std::runtime_error err) {
		cerr << err.what() << endl;
		return 1;
	}
	
	return 0;
}
