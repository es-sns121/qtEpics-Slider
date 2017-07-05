// 	server/record.cpp

//	Source file that implements the creation of the database.

#define epicsExportSharedSymbols
#include "pv/record.h"

#include <iostream>
#include <string>

#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/standardField.h>

using namespace std;

using namespace epics::pvAccess;
using namespace epics::pvData;
using namespace epics::pvDatabase;

// Defined in src/server/pv/record.h
using namespace Project; 

// Creates a record
RecordPtr Record::createRecord(const string & recordName)
{
	static FieldCreatePtr     fieldCreate = getFieldCreate();
	static StandardFieldPtr   standardField = getStandardField();
	static PVDataCreatePtr    pvDataCreate = getPVDataCreate();

	StructureConstPtr top = fieldCreate->createFieldBuilder()->
		add("value", pvInt)->
		add("display", standardField->display())->
		createStructure();

	PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);

	RecordPtr pvRecord( new Record(recordName, pvStructure) );

	pvRecord->initPvt();

	return pvRecord;

}

// Creates and adds records to database.
PVDatabasePtr Database::create()
{

// Get the database hosted by the local provider.
	PVDatabasePtr master = PVDatabase::getMaster();
	
	string recordName("testRecord");

	PVRecordPtr pvRecord = Record::createRecord(recordName);

	bool result = master->addRecord(pvRecord);
	if (!result) cerr << "Failed to add record " << recordName << " to database\n";
	
	return master;
}

Record::Record(const string & recordName,
			   const PVStructurePtr & pvStructure)
		: PVRecord(recordName, pvStructure)
{
}

void Record::initPvt()
{
	initPVRecord();

	PVStructurePtr pvStructure = getPVStructure();
	
// Initialize the convenience pointers in the record class
	value = pvStructure->getSubField<PVInt>("value");

// Attach the display and initialize it
	PVFieldPtr field = pvStructure->getSubField("display");
	pvDisplay.attach(field);
	display.setLow(0.0);
	display.setHigh(100.0);
	pvDisplay.set(display);

}

void Record::process()
{
}
