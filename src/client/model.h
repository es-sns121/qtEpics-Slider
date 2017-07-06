
// model.h

// Header file that defines the model's main class and 
// member functions

#ifndef MODEL_H
#define MODEL_H

#include <epicsThread.h>
#include <pv/pvaClient.h>
#include <pv/display.h>

class Model : public epicsThreadRunable
{
	
	public :
		
		Model(void * _view, const std::string & channelName);
	
		// Accessor functions to the display structure
		double getRangeMax();
		double getRangeMin();
	
		// Writes new value to the record.
		void putValue(const int & value);
	
		// Gets current value from the record.
		int getValue();
		
		// Gets current text from the record.
		std::string getText();
		
		void setCallback(void (*_callbackFunc)(void *, const int &));
		
	private:

		int value;			// Value held on record
		std::string text;	// Textual representation of the record

		// Dumps the record's current structure to a string representation
		std::string dumpRecordToString();

		// Monitor thread. Monitor needs to be in another thread to prevent blocking 
		// of GUI event loop
		epicsThread * monitorThread;
	
		// Code that the monitor thread will run
		void run();
		
		// Pointer to view's callback function. Called when monitor detects change in 
		// record data 
		void (*callbackFunc)(void *, const int &);
	
		// Pointer to view object
		void * view;

		// Private initialization functions called by the constructor
		void initPva(const std::string & channelName);
		void initValue();
		void initText();
		void initDisplay();
		// void initAlarmLimit();
		void initThread();

		// pvaClient objects that facilitate communication to record on database server
		epics::pvaClient::PvaClientPtr        pva;
		epics::pvaClient::PvaClientChannelPtr channel;
		epics::pvaClient::PvaClientPutPtr     put;
		epics::pvaClient::PvaClientPutDataPtr putData;
		epics::pvaClient::PvaClientGetPtr     get;
		epics::pvaClient::PvaClientGetDataPtr getData;
		epics::pvaClient::PvaClientMonitorPtr monitor;

		// Pointer to the display structure that is retrieved from the record.
		epics::pvData::PVStructurePtr display;
		// Pointer to the control structure that is retrieved from the record.
		epics::pvData::PVStructurePtr control;
		
}; // class Model

#endif // MODEL_H
