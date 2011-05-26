#ifndef TEST1_HXX
#define TEST1_HXX

#include <iostream>
#include <string>
#include <time.h>
#include "ldns.h"
#include "Record.hxx"

Record *getRecordFromWebServer(std::string webserver) {
	RecordC *r = getResultsForServer(webserver.c_str());
	
	std::string ns = std::string(r->nameserver);

	return new Record(webserver, ns, r->time, r->timestamp);
}

#endif /*TEST1_HXX*/
