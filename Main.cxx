#include "Main.hxx"

using namespace std;

int main(int argc, char **argv) {
	Record *r = getRecordFromWebServer(string(argv[1]));

	cout << r->toString() << endl;
}
