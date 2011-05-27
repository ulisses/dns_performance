#ifndef DATA_HXX
#define DATA_HXX

#include <mysql++.h>
#include <iostream>
#include <string>
#include "Record.hxx"

using namespace std;

class Data {
	private:
		const char * _username;
		const char * _password;
		const char * _server;
		const char * _dbname;
		
		mysqlpp::Connection _conn;
	public:
	Data(string server, string db, string user, string passwd);

	void fill(vector<Record*> *r);
};

#endif /*DATA_HXX*/