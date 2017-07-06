#include "pv/alarmLimit.h"
 
static FieldCreatePtr  fieldCreate  = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();

PVStructurePtr createAlarmLimit() {
	
	StructureConstPtr top = fieldCreate->createFieldBuilder()->

		add("active", pvBoolean)->
		add("lowAlarmLimit", pvDouble)->
		add("lowWarningLimit", pvDouble)->
		add("highWarningLimit", pvDouble)->
		add("highAlarmLimit", pvDouble)->
		add("lowAlarmSeverity", pvInt)->
		add("lowWarningSeverity", pvInt)->
		add("highWarningSeverity", pvInt)->
		add("highAlarmSeverity", pvInt)->
		add("hysteresis", pvDouble)->

		createStructure();

	PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);

	return pvStructure;
}

FieldConstPtr createAlarmLimitField()
{
	PVStructurePtr pvStructure = createAlarmLimit();
	return pvStructure->getField();
}
