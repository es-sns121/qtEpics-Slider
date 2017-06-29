#ifndef MODEL_H
#define MODEL_H

#include <pv/pvaClient.h>
#include <pv/display.h>

class Model
{
	
	public :
		Model(const std::string & channelName);
	
	/* Accessor functions to the display structure */
		double getRangeMax();
		double getRangeMin();
	
	/* Writes new value to the record. */
		void putValue(const int & value);
	
	/* Gets current value from the record. */
		int getValue();

	private:

	/* Private initialization functions called by the constructor */
		void initPva(const std::string & channelName);
		void initDisplay();
	
	/* pvaClient objects that facilitate communication to record on database server */
		epics::pvaClient::PvaClientPtr        pva;
		epics::pvaClient::PvaClientChannelPtr channel;
		epics::pvaClient::PvaClientPutPtr     put;
		epics::pvaClient::PvaClientPutDataPtr putData;
		epics::pvaClient::PvaClientGetPtr     get;
		epics::pvaClient::PvaClientGetDataPtr getData;

	/* Pointer to the display structure that is retrieved from the record. */
		epics::pvData::PVStructurePtr display;
		
}; /* class Model */

#endif /* MODEL_H */
