#ifndef MODEL_H
#define MODEL_H

#include <pv/pvaClient.h>
#include <pv/display.h>

class Model
{
	
	public :
		Model(const std::string & channelName);
			
		double getRangeMax();
		double getRangeMin();
		void putValue(const int & value);
		int getValue();

	private:
	
		void initPva(const std::string & channelName);
		void initDisplay();

		epics::pvaClient::PvaClientPtr        pva;
		epics::pvaClient::PvaClientChannelPtr channel;
		epics::pvaClient::PvaClientPutPtr     put;
		epics::pvaClient::PvaClientPutDataPtr putData;
		epics::pvaClient::PvaClientGetPtr     get;
		epics::pvaClient::PvaClientGetDataPtr getData;

		epics::pvData::PVStructurePtr display;
		
}; /* class Model */

#endif /* MODEL_H */
