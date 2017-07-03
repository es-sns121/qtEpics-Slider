#include "model.h"

using namespace std;
using namespace epics::pvData;
using namespace epics::pvaClient;

Model::Model (void * _view, const string & channelName)
{
	view = _view; 
	initPva(channelName);
	initDisplay();
	initThread();
}

/* initializes the pva objects */
void Model::initPva(const string & channelName)
{
	pva = PvaClient::get();
	channel = pva->channel(channelName);
	put = channel->createPut("");
	putData = put->getData();
	get = channel->createGet("");
	getData = get->getData();
	monitor = channel->monitor("");
}

/* initializes the display structure pointer. */
void Model::initDisplay()
{
	get->get();
	display = getData->getPVStructure()->getSubField<PVStructure>("display");
}

/* Gets current value from record. */
int Model::getValue()
{
	get->get();
	return getData->getPVStructure()->getSubField<PVInt>("value")->get();
}

/* Writes new value to record */
void Model::putValue(const int & value)
{
	putData->getPVStructure()->getSubField<PVInt>("value")->put(value);
	put->put();
}

/* Gets the minimum range value from the display structure */
double Model::getRangeMin()
{
	return display->getSubField<PVDouble>("limitLow")->get();
}

/* Gets the maximum range value from the display structure */
double Model::getRangeMax()
{
	return display->getSubField<PVDouble>("limitHigh")->get();
}

void Model::setCallback(void (*_callbackFunc)(void *, const int &))
{
	callbackFunc = _callbackFunc;
}

void Model::initThread()
{
	monitorThread = new epicsThread(*this, "monitorThread", epicsThreadGetStackSize(epicsThreadStackSmall), (unsigned int) 50);
	monitorThread->start();
}

void Model::run()
{
	PvaClientMonitorDataPtr monitorData = monitor->getData();
	PVStructurePtr pvStructure;

/* Initialize the monitors pvStructure */
	monitor->waitEvent();
	monitor->releaseEvent();

/* Monitor change in the record data. Use view callback upon change in data */
	while (true) {
		monitor->waitEvent();
		
		pvStructure = monitorData->getPVStructure();
		int value = pvStructure->getSubField<PVInt>("value")->get();
		
		monitor->releaseEvent();
		
		callbackFunc(view, value);
	}
}
