
// alarmLimit.h

// Functions relating to the creation of an alarmLimit_t structure.

#ifndef ALARMLIMIT_H
#define ALARMLIMIT_H

#include <pv/pvData.h>

using namespace epics::pvData;

PVStructurePtr createAlarmLimit();
FieldConstPtr createAlarmLimitField();

#endif // ALARMLIMIT_H
