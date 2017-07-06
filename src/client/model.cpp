
// model.cpp

// source file that implements member functions defined in 
// 'model.h'

#include "model.h"
#include <sstream>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvaClient;

Model::Model (void * _view, const string & channelName)
{
	view = _view; 
	initPva(channelName);
	initValue();
	initText();
	initDisplay();
	initThread();
}

// initializes the pva objects
void Model::initPva(const string & channelName)
{
	pva = PvaClient::get();
	channel = pva->channel(channelName);
	put = channel->createPut("");
	putData = put->getData();
	get = channel->createGet("");
	getData = get->getData();
	monitor = channel->monitor("");
	
	// Initialize the monitor's pvStructure
	monitor->waitEvent();
	monitor->releaseEvent();

}

void Model::initValue()
{
	get->get();
	value = getData->getPVStructure()->getSubField<PVInt>("value")->get();
}

void Model::initText()
{
	text = dumpRecordToString(); 
}

// initializes the display structure pointer.
void Model::initDisplay()
{
	get->get();
	display = getData->getPVStructure()->getSubField<PVStructure>("display");
}

// Allocates, initializes, and starts a thread for the monitor to run in
void Model::initThread()
{
	monitorThread = new epicsThread(*this, "monitorThread", epicsThreadGetStackSize(epicsThreadStackSmall));
	monitorThread->start();
}

// Gets the current value from the record.
int Model::getValue()
{
	return value; 
}

// Gets the current text from the record.
string Model::getText()
{
	return text; 
}

// Gets the minimum range value from the display structure
double Model::getRangeMin()
{
	return display->getSubField<PVDouble>("limitLow")->get();
}

// Gets the maximum range value from the display structure
double Model::getRangeMax()
{
	return display->getSubField<PVDouble>("limitHigh")->get();
}

// Writes new value to record
void Model::putValue(const int & value)
{
	putData->getPVStructure()->getSubField<PVInt>("value")->put(value);
	put->put();
}

// Dumps the record's current contents to string
string Model::dumpRecordToString()
{
	stringstream ss;

	monitor->getData()->getPVStructure()->dumpValue(ss);

	return ss.str();
}

// Set the callback function
void Model::setCallback(void (*_callbackFunc)(void *, const int &))
{
	callbackFunc = _callbackFunc;
}

// Executed by the monitor thread. Monitors the record on the database. Calls view's callback function upon registering 
// a change in the data
void Model::run()
{
	PvaClientMonitorDataPtr monitorData = monitor->getData();
	PVStructurePtr pvStructure;

	// Monitor change in the record data. Call view's callback upon change in data
	while (true) {
		monitor->waitEvent();
		
		pvStructure = monitorData->getPVStructure();
		
		// Update local copies
		value = pvStructure->getSubField<PVInt>("value")->get();
		text = dumpRecordToString(); 
		
		monitor->releaseEvent();
		
		callbackFunc(view, value);
	}
}
