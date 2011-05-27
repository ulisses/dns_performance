#include "Data.hxx"

Data::Data(string server, string db, string user, string passwd) {
	_server = server.c_str();
	_dbname = db.c_str();
	_username = user.c_str();
	_password = passwd.c_str();

	/* Connect to the the database. */
    try {
	    _conn = mysqlpp::Connection(_dbname,_server, _username, _password);
    } catch (const mysqlpp::Exception& er) {
		cerr << "Error making connection: " << er.what() << endl;
    }
}

Data::~Data() {
	free(&_username);
	free(&_password);
	free(&_server);
	free(&_dbname);
}

void Data::fill(vector<Record*> *r) {
	try {
		for(vector<Record*>::const_iterator it=r->begin(); it != r->end(); it++) {
			Record *elem = *it;
			mysqlpp::Query query = _conn.query("select * from dnsperf where webserver = %0q");
			query.parse();

			mysqlpp::SQLQueryParms sqp;
			sqp << elem->getWebserver();
			mysqlpp::StoreQueryResult res = query.store(sqp);
			
			if (res.empty()) {
				/* We don't have this row in database, insert this new one*/
				mysqlpp::Query newquery = _conn.query("insert INTO dnsperf (webserver,avgqtime,nrq,timefst,timelst) VALUES(%0q , %1 , 1 , %2q , %3q)");
				newquery.parse();
				
				mysqlpp::SQLQueryParms sqpNew;
				sqpNew << mysqlpp::String(elem->getWebserver())
				       << mysqlpp::String(to_string<int>(elem->getTime(),std::dec))
				       << mysqlpp::String(to_string<long>(elem->getTimestamp(),std::dec))
				       << mysqlpp::String(to_string<long>(elem->getTimestamp(),std::dec));
				newquery.store(sqpNew);
			} else {
				/* Update a row */
				for (size_t i = 0; i < res.num_rows(); ++i) {
					mysqlpp::Query newquery = _conn.query("update dnsperf SET webserver=%0q,avgqtime=%1,nrq=%2,timefst=%3q,timelst=%4q where webserver=%0q");
					newquery.parse();

					int newTime = (int) (atoi(res[i]["avgqtime"])+elem->getTime())/2;
					int newNrq = (int) (atoi(res[i]["nrq"])+1);

					mysqlpp::SQLQueryParms sqpNew;
					sqpNew << mysqlpp::String(elem->getWebserver())
					       << mysqlpp::String(to_string<int>(newTime,std::dec))
					       << mysqlpp::String(to_string<int>(newNrq,std::dec))
					       << mysqlpp::String(res[i]["timefst"])
					       << mysqlpp::String(to_string<long>(elem->getTimestamp(),std::dec));
					newquery.store(sqpNew);
				}
			}
		}
	} catch (const mysqlpp::BadQuery& er) {
		/* Handles any query errors */
		cerr << "Query error(fill): " << er.what() << endl;
		exit(1);
	} catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error(fill): " << er.what() << endl;
		exit(1);
	}
}
