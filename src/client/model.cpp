#include "model.h"

using namespace std;
using namespace epics::pvData;
using namespace epics::pvaClient;

Model::Model (const string & channelName)
{
	initPva(channelName);
	initDisplay();
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
