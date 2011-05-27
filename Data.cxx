#include "Data.hxx"

Data::Data(string server, string db, string user, string passwd) {
	_server = server.c_str();
	_dbname = db.c_str();
	_username = user.c_str();
	_password = passwd.c_str();
	
	/* Connect to the the database. */
    mysqlpp::Connection _conn(false);

    if (!_conn.connect(_dbname, _server, _username, _password)) {
        cerr << "DB connection failed: " << _conn.error() << endl;
    }
}

void Data::fill(vector<Record*> *r) {
	for(vector<Record*>::const_iterator it=r->begin(); it != r->end(); it++) {
		Record *elem = *it;

	}
}

        /*
        // Retrieve a subset of the sample stock table set up by resetdb
        // and display it.
        mysqlpp::Query query = getResultFromQuery _conn.query("select item from stock");
        if (mysqlpp::StoreQueryResult res = query.store()) {
            cout << "We have:" << endl;
            mysqlpp::StoreQueryResult::const_iterator it; 
            for (it = res.begin(); it != res.end(); ++it) {
                mysqlpp::Row row = *it;
                cout << '\t' << row[0] << endl;
            }
        }
        else {
            cerr << "Failed to get item list: " << query.error() << endl;
        }
        */
