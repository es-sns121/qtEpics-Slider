// server/record.cpp

// Source file that implements the creation of the database.

#define epicsExportSharedSymbols
#include "pv/record.h"
#include "pv/alarmLimit.h"

#include <iostream>
#include <string>
#include <sstream>

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
	static FieldCreatePtr   fieldCreate   = getFieldCreate();
	static StandardFieldPtr standardField = getStandardField();
	static PVDataCreatePtr  pvDataCreate  = getPVDataCreate();

	StructureConstPtr top = fieldCreate->createFieldBuilder()->
		add("value", pvInt)->
		add("display", standardField->display())->
		add("alarmLimit", createAlarmLimitField())->
		createStructure();

	PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);

	RecordPtr pvRecord( new Record(recordName, pvStructure) );

	pvRecord->initPvt();

	return pvRecord;

}

// Creates and adds the testRecord to the database.
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

// initialize the record
void Record::initPvt()
{
	initPVRecord();

	PVStructurePtr pvStructure = getPVStructure();

	pvStructure->getSubField<PVInt>("value")->put(50);

	PVFieldPtr field = pvStructure->getSubField("display");

	// Attach the display and initialize it
	pvDisplay.attach(field);
	display.setLow(0.0);
	display.setHigh(100.0);
	display.setUnits("u/m");
	pvDisplay.set(display);

	initAlarmLimit();
}

// Initialize the alarm limit structure
void Record::initAlarmLimit()
{
	PVStructurePtr pvStructure = getPVStructure();
	pvStructure = pvStructure->getSubField<PVStructure>("alarmLimit");
		
	pvStructure->getSubField<PVBoolean>("active")->put(true);
	pvStructure->getSubField<PVDouble>("lowAlarmLimit")->put(10);
	pvStructure->getSubField<PVDouble>("lowWarningLimit")->put(20);
	pvStructure->getSubField<PVDouble>("highWarningLimit")->put(80);
	pvStructure->getSubField<PVDouble>("highAlarmLimit")->put(90);
	pvStructure->getSubField<PVInt>("lowAlarmSeverity")->put(0);
	pvStructure->getSubField<PVInt>("lowWarningSeverity")->put(0);
	pvStructure->getSubField<PVInt>("highWarningSeverity")->put(0);
	pvStructure->getSubField<PVInt>("highAlarmSeverity")->put(0);
	pvStructure->getSubField<PVDouble>("hysteresis")->put(10);
	
}

void Record::process()
{
}
