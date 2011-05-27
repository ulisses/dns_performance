#ifndef TEST1_H
#define TEST1_H
#include <stdio.h>
#include <ldns/ldns.h>
#include <sys/types.h>

ldns_rdf *ldns_rdf_new_addr_frm_str(char *str);
int getQueryTime(char *nameserver, /*out*/ struct timeval *timestamp);

#ifdef __cplusplus
extern "C" {
#endif

typedef struct record {
	char *webserver;
	char *nameserver;
	int time;
	struct timeval timestamp;
}RecordC;

RecordC *getResultsForServer(const char *srv);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*TEST1_H*/
