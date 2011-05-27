#include "ldns.h"

/* We try to use an address record (A) if we can't
 * we try to use an IPv6 one (AAAA).
 */
ldns_rdf *ldns_rdf_new_addr_frm_str(char *str) {
	ldns_rdf *a = ldns_rdf_new_frm_str(LDNS_RDF_TYPE_A, str);
	if (!a) {
		a = ldns_rdf_new_frm_str(LDNS_RDF_TYPE_AAAA, str);
		if (!a) {
			return NULL;
		}
	}
	return a;                         
}

/* Extract the dns hostname and convert it to char*.
*/
char *getAndNormalizeNS(ldns_rr *elemRR) {
	int i=0;
	uint8_t *str = ldns_rdf_data(elemRR->_rdata_fields[0]);
	char *ns = (char *) malloc(sizeof(char) * strlen((char *)str));

	for(;str[i]!='\0';i++) {
		if(str[i] < 10)
			ns[i] = '.';
		else
			ns[i] = (char)str[i];
	}
	return ++ns;
}

/* Get queryTime from the dns nameserver. Returns -1 if failure
 * or the time in ms fro the dns query. We change the parameter 'timestamp'
 * to also return the timestamp associated with the query.
 *
 * We could improve this function, by receiving directlly an 'ldns_rdf'
 * and like so we probably won't need the 'getAndNormalizeNS' function.
 */
int getQueryTime(char *nameserver, /*out*/ struct timeval *timestamp) {
	ldns_resolver *res = NULL, *res2 = NULL;
	ldns_rr_list *addr_lst = NULL;
	ldns_rdf *dname, *qname, *nameserver_rdf;
    ldns_pkt *pkt;
	ldns_status	status;
    char *query;
	int result = 0;
	
	query = ".";
	
	/* Create new resolver, we will use this one to make queries */
	res = ldns_resolver_new();
	if (!res || strlen(nameserver) <= 0) {
		fprintf(stderr, "Could not create a resolver structure");
		result = EXIT_FAILURE;
	}
	/* Create Resource Record data field based on our nameserver */
	nameserver_rdf = ldns_rdf_new_addr_frm_str(nameserver);
	if (!nameserver_rdf) {
		/* Let's try to resolve the nameserver using our /etc/resolv.conf (just say NULL here) */
		status = ldns_resolver_new_frm_file(&res2, NULL);

		if (status != LDNS_STATUS_OK) {
			fprintf(stderr,"%s", "@server ip could not be converted");
		}
		dname = ldns_dname_new_frm_str(nameserver);
		
		/* Get all addresses for our nameserver (could be several) */
		addr_lst = ldns_get_rr_list_addr_by_name(res2, dname,LDNS_RR_CLASS_IN,LDNS_RD); /* LDNS_RD: generate the Recursion Desired flag (rd) in the query */
		ldns_rdf_deep_free(dname);
		if (!addr_lst) {
			fprintf(stderr,"Could not find any address for the nameserver:%s", nameserver);
			return -1;
		} else {
			status = ldns_resolver_push_nameserver_rr_list(res, addr_lst); /* Reused variable 'status' */
			if(status != LDNS_STATUS_OK) {
				fprintf(stderr,"Could not push the nameserver: %s", nameserver);
				return -1;
			}
		}
	} else {
			status = ldns_resolver_push_nameserver(res, nameserver_rdf); /* Reused variable 'status' */
		if (status != LDNS_STATUS_OK) {
			fprintf(stderr,"Could not push the nameserver: %s", nameserver);
			return -1;
		} else {
			ldns_rdf_deep_free(nameserver_rdf);
		}
	}
	/* Generate a query field from string */
	qname = ldns_dname_new_frm_str(query);
	if (!qname) {
		fprintf(stderr,"%s", "error in making qname");
	}
	/* create a packet and set the RD flag on it */
	pkt = ldns_resolver_query(res, qname, LDNS_RR_TYPE_NS, LDNS_RR_CLASS_IN, LDNS_RD);

	if (!pkt)  {
		fprintf(stderr,"No packet received");
		return -1;
	} else {
		result = ldns_pkt_querytime(pkt);
		(*timestamp) = ldns_pkt_timestamp(pkt);
		ldns_pkt_free(pkt); 
	}
	ldns_rdf_deep_free(qname);
	ldns_resolver_deep_free(res);
	ldns_resolver_deep_free(res2);
	ldns_rr_list_deep_free(addr_lst);

	return result;
}
/* Receives the webserver name (e.g. google.com) and returns a record
 * with all the information we need about this server dns.
 */
RecordC *getResultsForServer(const char *srv) {
	ldns_resolver *res;
	ldns_rdf *name;
	ldns_pkt *p;
	ldns_rr_list *addr;
	ldns_status s;

	/* Create a rdf from the command line arg */
	name = ldns_dname_new_frm_str(srv);
	if (!name) {
		return NULL;
	}

	s = ldns_resolver_new_frm_file(&res, NULL);

	if (s != LDNS_STATUS_OK) {
		ldns_rdf_deep_free(name);
		return NULL;
	}

	p = ldns_resolver_query(res, name, LDNS_RR_TYPE_NS,LDNS_RR_CLASS_IN, LDNS_RD);

	/* Force recursion, we want the first DNS server to do the all the job (if possible)*/
	if(!ldns_resolver_recursive(res)) {
		ldns_resolver_set_recursive(res,true);
	}
	ldns_resolver_set_retry(res, 1); /* don't want to wait too long */

	addr = ldns_pkt_answer(p);

	// use the resolver to send it a query for the a/aaaa of name 
	if (!addr) {
		fprintf(stderr, " *** could not get an address for %s\n", srv);
		ldns_rdf_deep_free(name);
		ldns_resolver_deep_free(res);
		
		return NULL;
	}
	ldns_rr * elemRR = ldns_rr_list_rr(addr,0);

	char *ns = getAndNormalizeNS(elemRR);

	ldns_rdf_deep_free(name);
	ldns_resolver_deep_free(res);
	
	RecordC *ret = (RecordC *)malloc(sizeof(RecordC));
	struct timeval timestamp;
	int qtime = getQueryTime(ns,&timestamp);
	if(qtime == -1) return NULL;
	
	ret->nameserver = ns;
	ret->time = qtime;
	ret->timestamp = timestamp;

	return ret;
}

