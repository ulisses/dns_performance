#ifndef TEST1_HXX
#define TEST1_HXX

#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include "ldns.h"
#include "Record.hxx"
#include "Data.hxx"

using namespace std;

float getElapsedTimeInS() {
	return clock()/CLOCKS_PER_SEC;
}

void sleep(int ms) {
	float goal;
	goal = ms + getElapsedTimeInS();

	while( goal >= getElapsedTimeInS())
		;
}

/* Convert a C typedef to a C++ class, we could use this typedef
 * in C++, but is easier if we use object in the object's world.
 */
Record *getRecordFromWebServer(string webserver) {
	RecordC *r = getResultsForServer(webserver.c_str());
	if(!r) {
		fprintf(stderr, "ERROR: could not get a RecordC from %s\n", webserver.c_str());
		exit(-1);
	} else {
		string ns = string(r->nameserver);
		return new Record(webserver, ns, r->time, r->timestamp);
	}
}

/* We can improve this, by reading this list from a file
 * or maybe implement in C++ the extraction of this information
 * from the alexa.com website.
 */
vector<string> *getTopDomains() {
	vector<string> *v = new vector<string>();
	v->push_back("google.com");
	v->push_back("facebook.com");
	v->push_back("youtube.com");
	v->push_back("yahoo.com");
	v->push_back("live.com");
	v->push_back("wikipedia.org");
	v->push_back("baidu.com");
	v->push_back("blogger.com");
	v->push_back("msn.com");
	v->push_back("qq.com");
	
	return v;
}

#endif /*TEST1_HXX*/
