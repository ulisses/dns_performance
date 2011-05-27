#include "Main.hxx"

int main(int argc, char **argv) {
	vector<string> *v = getTopDomains();
	vector<Record*> *r = new vector<Record*>();

	for(vector<string>::const_iterator it=v->begin(); it != v->end(); it++) {
		string elemS = *it;
		Record *elemR = getRecordFromWebServer(elemS);

		r->push_back(elemR);
		cout << elemR->toString() << endl;
	}

	/* hostname, databaseName, username, password*/
	Data *d = new Data("maclisses.lan","ulisses_db","root1","new-passwd");
	d->fill(r);
}
