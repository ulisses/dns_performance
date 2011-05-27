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

Record *getRecordFromWebServer(string webserver) {
	RecordC *r = getResultsForServer(webserver.c_str());
	
	string ns = string(r->nameserver);

	return new Record(webserver, ns, r->time, r->timestamp);
}

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
