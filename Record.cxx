#include "Record.hxx"

Record::Record(string webserver, string nameserver, int time, struct timeval timestamp) {
	_webserver = webserver;
	_nameserver = nameserver;
	_time = time;
	_timestamp = timestamp.tv_sec;
}

Record::Record(string webserver, string nameserver, int time, long timestamp) {
	_webserver = webserver;
	_nameserver = nameserver;
	_time = time;
	_timestamp = timestamp;
}

string Record::toString() {
	stringstream ss;

	ss << "webserver: " << _webserver
	   << "\nnameserver: " << _nameserver
	   << "\ntime: " << _time
	   << "\ntimestamp: " << _timestamp;

	return ss.str();
}
