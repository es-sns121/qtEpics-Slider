
// model.h

// Header file that defines the model's main class and member functions

#ifndef MODEL_H
#define MODEL_H

#include <epicsThread.h>
#include <pv/pvaClient.h>
#include <pv/display.h>

class Model : public epicsThreadRunable
{
	
	public :
		
		Model(void * _view, const std::string & channelName);
	
		// Accessor functions to the display structure.
		double getRangeMax();
		double getRangeMin();
		std::string getUnits();

		// Accessor functions to the alarmLimit structure.
		double getLowAlarm();
		double getLowWarning();
		double getHighWarning();
		double getHighAlarm();

		// Writes new value to the record.
		void putValue(const int & value);
	
		// Gets current value from the record.
		int getValue();
		
		// Gets current text from the record.
		std::string getText();

		// Accessor functions to the data fields.
		long 		getLong(const std::string & select);
		double 		getDouble(const std::string & select);
		std::string getString(const std::string & select);
		bool 		getBoolean();

		void setCallback(void (*_callbackFunc)(void *, const int &));
		
	private:

		int value;			// Value held on record.
		std::string text;	// Textual representation of the record.
		
		// Data fields held on record.
		long   		_long1;
		long   		_long2;
		double 		_double1;
		double 		_double2;
		std::string _string1;
		std::string _string2;
		bool 		_boolean;
		
		// Dumps the record's current structure to a string representation.
		std::string dumpRecordToString();

		// Monitor thread. Monitor needs to be in another thread to prevent blocking 
		// of GUI event loop.
		epicsThread * monitorThread;
	
		// Code that the monitor thread will run.
		void run();
		
		// Pointer to view's callback function. Called when monitor detects a change in 
		// the record data.
		void (*callbackFunc)(void *, const int &);
	
		// Pointer to the view object.
		void * view;

		// Private initialization functions called by the constructor.
		void initPva(const std::string & channelName);
		void initValue();
		void initText();
		void initData();
		void initDisplay();
		void initAlarmLimit();
		void initThread();

		// pvaClient objects that facilitate communication to record on database server.
		epics::pvaClient::PvaClientPtr        pva;
		epics::pvaClient::PvaClientChannelPtr channel;
		epics::pvaClient::PvaClientPutPtr     put;
		epics::pvaClient::PvaClientPutDataPtr putData;
		epics::pvaClient::PvaClientGetPtr     get;
		epics::pvaClient::PvaClientGetDataPtr getData;
		epics::pvaClient::PvaClientMonitorPtr monitor;

		// Pointer to the display structure that is retrieved from the record.
		epics::pvData::PVStructurePtr display;
		// Pointer to the alarmLimit structure that is retrieved from the record.
		epics::pvData::PVStructurePtr alarmLimit;
		
}; // class Model

#endif // MODEL_H
