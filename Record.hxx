#ifndef RECORD_HXX
#define RECORD_HXX

#include <string>
#include <sstream>

using namespace std;

class Record {
	private:
		string _webserver;
		string _nameserver;
		int _time;
		long _timestamp;
	public:
		Record(string webserver, string nameserver, int time, struct timeval timestamp);
		Record(string webserver, string nameserver, int time, long timestamp);
		string toString();
};

#endif /* RECORD_HXX */
