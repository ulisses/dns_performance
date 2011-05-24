#include <unistd.h>
#include <stdint.h>
#include "file1.h"

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#endif

/*
int main(int argc, char **argv) {
#ifdef OS_WIN
	std::cout << "Not a good day to run this program, please try tomorrow." << std::endl;
#endif
#ifdef OS_LINUX
	std::cout << "LINUX" << std::endl;
#endif
#ifdef OS_MACOSX_4
	std::cout << "OSX4" << std::endl;
	int ret = execlp("lookupd", "lookupd", "-flushcache", NULL);
#endif
#ifdef OS_MACOSX_7
	std::cout << "OSX7" << std::endl;
	int ret = execlp("dscacheutil", "dscacheutil", "-flushcache", NULL); 
	std::cout << "ret: " << ret << std::endl;
#endif

	return 0;
}
*/
