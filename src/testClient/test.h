
// test.h

// Defines a test function to be used exclusively with 'testRecord'

/* Use of this function with a record with a different structure than that of 'testRecord'
 * as defined in server/record.cpp will cause a segmentation fault.
 * */

#ifndef TEST_H
#define TEST_H

#include <string>

/* Opens a channel using pvAccess to the record specified by channelName. It will then
 * write to the record 'iterations' times, seperated by 'interval seconds.
 * */
int test(const std::string & channelName,
		 const bool & debug,
		 const bool & verbosity,
		 const int & iterations,
		 const double & interval);

#endif // TEST_H
