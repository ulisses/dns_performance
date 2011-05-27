#include "Main.hxx"

bool out = false;

void signal_callback_handler(int signum) {
	out = true;
	cout << "Time to go..." << endl;
}

static int usage(FILE *fp, char *prog) {
	fprintf(fp, "%s <time_in_seconds> \n", prog);
	return 0;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		usage(stdout, argv[0]);
		exit(EXIT_FAILURE);
	}
	
	/* Let's add a SIGINT handling mechanism:
 	 * Register signal and signal handler
 	 */
	signal(SIGINT, signal_callback_handler);

	while(1 && !out) {
	 	cout << "still running..." << endl;
	 	vector<string> *v = getTopDomains();
		vector<Record*> *r = new vector<Record*>();
	
		for(vector<string>::const_iterator it=v->begin(); it != v->end(); it++) {
			string elemS = *it;
			Record *elemR = getRecordFromWebServer(elemS);
			/* If we are not able to fetch this dns info, move on */
			if(!elemR) {
				continue;
			}
			r->push_back(elemR);
		}
	
		/* hostname, databaseName, username, password*/
		Data *d = new Data("maclisses.lan","ulisses_db","root1","new-passwd");
		if(!out) {
			d->fill(r);
			v->clear();
			delete v;
			for (vector<Record*>::iterator it = r->begin();it != r->end(); it++) {
				delete *it;
			}
			r->clear();
			delete r;
		} else {
			exit(SIGINT);
		}
 		sleep(atoi(argv[1]));
 	}
}
