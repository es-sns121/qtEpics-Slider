#ifndef ALARMLIMIT_H
#define ALARMLIMIT_H

#include <pv/pvData.h>

using namespace epics::pvData;

PVStructurePtr createAlarmLimit();
FieldConstPtr createAlarmLimitField();

#endif // ALARMLIMIT_H
