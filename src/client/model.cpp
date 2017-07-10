
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
	initData();
	initDisplay();
	initAlarmLimit();
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

void Model::initData()
{
	get->get();
	_long1 = getData->getPVStructure()->getSubField<PVLong>("longInteger1")->get();
	_double1 = getData->getPVStructure()->getSubField<PVDouble>("double1")->get();
	_string1 = getData->getPVStructure()->getSubField<PVString>("string1")->get();
	_long2 = getData->getPVStructure()->getSubField<PVLong>("longInteger2")->get();
	_double2 = getData->getPVStructure()->getSubField<PVDouble>("double2")->get();
	_string2 = getData->getPVStructure()->getSubField<PVString>("string2")->get();
	_boolean = getData->getPVStructure()->getSubField<PVBoolean>("boolean")->get();
}

// initializes the display structure pointer.
void Model::initDisplay()
{
	get->get();
	display = getData->getPVStructure()->getSubField<PVStructure>("display");
}

// initializes the display structure pointer.
void Model::initAlarmLimit()
{
	get->get();
	alarmLimit = getData->getPVStructure()->getSubField<PVStructure>("alarmLimit");
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

string Model::getUnits()
{
	return display->getSubField<PVString>("units")->get();
}

// Gets the low alarm value from the alarm limit structure
double Model::getLowAlarm()
{
	return alarmLimit->getSubField<PVDouble>("lowAlarmLimit")->get();
}

// Gets the low warning value from the alarm limit structure
double Model::getLowWarning()
{
	return alarmLimit->getSubField<PVDouble>("lowWarningLimit")->get();
}

// Gets the high warning value from the alarm limit structure
double Model::getHighWarning()
{
	return alarmLimit->getSubField<PVDouble>("highWarningLimit")->get();
}

// Gets the high alarm value from the alarm limit structure
double Model::getHighAlarm() 
{
	return alarmLimit->getSubField<PVDouble>("highAlarmLimit")->get();
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

long Model::getLong(const std::string & select)
{
	if (select.compare("long1") == 0)
		return _long1;
	else
		return _long2;
}

double Model::getDouble(const std::string & select)
{
	if (select.compare("double1") == 0)
		return _double1;
	else
		return _double2;
}

string Model::getString(const std::string & select)
{
	if (select.compare("string1") == 0)
		return _string1;
	else
		return _string2;
}

bool Model::getBoolean()
{
	return _boolean;
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
		_long1 = pvStructure->getSubField<PVLong>("longInteger1")->get();
		_double1 = pvStructure->getSubField<PVDouble>("double1")->get();
		_string1 = pvStructure->getSubField<PVString>("string1")->get();
		_long2 = pvStructure->getSubField<PVLong>("longInteger2")->get();
		_double2 = pvStructure->getSubField<PVDouble>("double2")->get();
		_string2 = pvStructure->getSubField<PVString>("string2")->get();
		_boolean = pvStructure->getSubField<PVBoolean>("boolean")->get();
		
		monitor->releaseEvent();
		
		callbackFunc(view, value);
	}
}
