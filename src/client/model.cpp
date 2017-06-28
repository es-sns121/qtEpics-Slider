#include "model.h"

using namespace std;
using namespace epics::pvData;
using namespace epics::pvaClient;

Model::Model (const string & channelName)
{
	initPva(channelName);
	initDisplay();
}

void Model::initPva(const string & channelName)
{
	pva = PvaClient::get();
	channel = pva->channel(channelName);
	put = channel->createPut("");
	putData = put->getData();
	get = channel->createGet("");
	getData = get->getData();
}

void Model::initDisplay()
{
	get->get();
	display = getData->getPVStructure()->getSubField<PVStructure>("display");
		
}

int Model::getValue()
{
	get->get();
	return getData->getPVStructure()->getSubField<PVInt>("value")->get();
}

void Model::putValue(const int & value)
{
	putData->getPVStructure()->getSubField<PVInt>("value")->put(value);
	put->put();
}

double Model::getRangeMin()
{
	return display->getSubField<PVDouble>("limitLow")->get();
}

double Model::getRangeMax()
{
	return display->getSubField<PVDouble>("limitHigh")->get();
}
