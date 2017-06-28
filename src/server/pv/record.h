#ifndef RECORD_H
#define RECORD_H

#ifdef epicsExportSharedSymbols
#	define  recordEpicsExportSharedSymbols
#	undef   epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>
#include <pv/pvDisplay.h>

#ifdef recordEpicsExportSharedSymbols
#	define epicsExportSharedSymbols  
#	undef  recordEpicsExportSharedSymbols
#endif

#include <shareLib.h>

namespace Project {


	class epicsShareClass Database
	{
		public:
			static epics::pvDatabase::PVDatabasePtr create();
	
	}; /* class Database */


	class Record;
	typedef std::tr1::shared_ptr<Record> RecordPtr;
	
	class epicsShareClass Record : 
		public epics::pvDatabase::PVRecord 
	{
		public:
			POINTER_DEFINITIONS(Record);
			
			static RecordPtr createRecord(const std::string & recordName);
			
			virtual ~Record() {}
			
			virtual void process();

		private:
			Record(
				const std::string & recordName, 
				epics::pvData::PVStructurePtr const & pvStructure);
			
			void initPvt();

			epics::pvData::PVIntPtr value;
			
			epics::pvData::Display   display;
			epics::pvData::PVDisplay pvDisplay;

	}; /* class Record */


} /* namespace Project*/

#endif /* RECORD_H */
