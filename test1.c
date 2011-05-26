#include <stdio.h>
#include <sys/types.h>
#include <ldns/ldns.h>
#include "test1.h"

ldns_rdf *ldns_rdf_new_addr_frm_str(char *str) {
	ldns_rdf *a = ldns_rdf_new_frm_str(LDNS_RDF_TYPE_A, str);
	if (!a) {
		/* maybe ip6 */
		a = ldns_rdf_new_frm_str(LDNS_RDF_TYPE_AAAA, str);
		if (!a) {
			return NULL;
		}
	}
	return a;                         
}

static int usage(FILE *fp, char *prog) {
	fprintf(fp, "%s server\n", prog);
	fprintf(fp, "  print out some information about server\n");
	return 0;
}

int getQueryTime(char *nameserver) {
	ldns_resolver *res = NULL, *res2 = NULL;
	ldns_rr_list *addr_lst = NULL;
	ldns_rdf *dname, *qname, *nameserver_rdf;
    ldns_pkt *pkt;
	ldns_status	status;
    char *query;
	int result = 0;
	
	query = ".";
	
	printf(">>>nameserver=%s,query=%s\n",nameserver,query);

	/* Create new resolver, we will use this one to make queries */
	res = ldns_resolver_new();
	if (!res || strlen(nameserver) <= 0) {
		fprintf(stderr, "Could not create a resolver structure");
		result = EXIT_FAILURE;
		goto exit;
	}
	/* Create Resource Record data field based on our nameserver */
	nameserver_rdf = ldns_rdf_new_addr_frm_str(nameserver);
	if (!nameserver_rdf) {
		puts(">>>nameserver vazio");
		/* Let's try to resolve the nameserver using our /etc/resolv.conf (just say NULL here) */
		status = ldns_resolver_new_frm_file(&res2, NULL);

		if (status != LDNS_STATUS_OK) {
			printf("%s", "@server ip could not be converted");
		}
		puts(">>>ANDO AKI");
		dname = ldns_dname_new_frm_str(nameserver);
		
		/* Get all addresses for our nameserver (could be several) */
		addr_lst = ldns_get_rr_list_addr_by_name(res2, dname,LDNS_RR_CLASS_IN,LDNS_RD); /* LDNS_RD: generate the Recursion Desired flag (rd) in the query */
		ldns_rdf_deep_free(dname);
		if (!addr_lst) {
			fprintf(stderr,"Could not find any address for the nameserver:%s", nameserver);
			result = EXIT_FAILURE;
			goto exit;
		} else {
			status = ldns_resolver_push_nameserver_rr_list(res, addr_lst); /* Reused variable 'status' */
			if(status != LDNS_STATUS_OK) {
				fprintf(stderr,"Could not push the nameserver: %s", nameserver);
				result = EXIT_FAILURE;
				goto exit;
			}
		}
	} else {
			status = ldns_resolver_push_nameserver(res, nameserver_rdf); /* Reused variable 'status' */
		if (status != LDNS_STATUS_OK) {
			fprintf(stderr,"Could not push the nameserver: %s", nameserver);
			result = EXIT_FAILURE;
			goto exit;
		} else {
			ldns_rdf_deep_free(nameserver_rdf);
		}
	}
	/* Generate a query field from string */
	qname = ldns_dname_new_frm_str(query);
	if (!qname) {
		printf("%s", "error in making qname");
	}
	/* create a packet and set the RD flag on it */
	pkt = ldns_resolver_query(res, qname, LDNS_RR_TYPE_NS, LDNS_RR_CLASS_IN, LDNS_RD);

	if (!pkt)  {
		puts("No packet received");
		result = EXIT_FAILURE;
	} else {
		result = ldns_pkt_querytime(pkt);
		printf("*********QUERY TIME:%u\n",ldns_pkt_querytime(pkt));
		puts(">>>PACOTE :) :) :) :) :) :) :) :) :) :)");
		ldns_pkt_print(stdout, pkt);
		ldns_pkt_free(pkt); 
	}
	exit:
	ldns_rdf_deep_free(qname);
	ldns_resolver_deep_free(res);
	ldns_resolver_deep_free(res2);
	ldns_rr_list_deep_free(addr_lst);

	return result;
}

int main(int argc, char *argv[]) {
	ldns_resolver *res;
	ldns_rdf *name;
	ldns_pkt *p;
	ldns_rr_list *addr;
	ldns_status s;

	if (argc != 2) {
		usage(stdout, argv[0]);
		exit(EXIT_FAILURE);
	} else {
		/* Create a rdf from the command line arg */
		name = ldns_dname_new_frm_str(argv[1]);
		//printf("********TESTE:%d\n",getQueryTime(argv[1]));
		if (!name) {
			usage(stdout, argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	s = ldns_resolver_new_frm_file(&res, NULL);

	if (s != LDNS_STATUS_OK) {
		ldns_rdf_deep_free(name);
		exit(EXIT_FAILURE);
	}

	p = ldns_resolver_query(res, name, LDNS_RR_TYPE_NS,LDNS_RR_CLASS_IN, LDNS_RD);

	/* Force recursion, we want the first DNS server to do the all the job (if possible)*/
	if(!ldns_resolver_recursive(res)) {
		ldns_resolver_set_recursive(res,true);
	}
	ldns_resolver_set_retry(res, 1); /* don't want to wait too long */
	ldns_pkt_print(stdout,p);

	addr = ldns_pkt_answer(p);
	puts("-----BEGIN");
	ldns_rr_list_print(stdout, addr);
	puts("----END");


	// use the resolver to send it a query for the a/aaaa of name 
	//addr = ldns_get_rr_list_addr_by_name(res, name, LDNS_RR_CLASS_IN, LDNS_RD); // LDNS_RD: Recursion Desired - query flag 
	if (!addr) {
		fprintf(stderr, " *** could not get an address for %s\n", argv[1]);
		ldns_rdf_deep_free(name);
		ldns_resolver_deep_free(res);
		exit(EXIT_FAILURE);
	}

	printf("we have:%d nameservers\n",(int)ldns_rr_list_rr_count(addr));

	ldns_rr * elemRR = ldns_rr_list_rr(addr,0);
	//char * elem = (char *)elemRR->_rdata_fields[0]->_data;
	//printf("elem=%s,nr_elems=%d,tipoRR=%d\n",elem,elemRR->_rd_count,elemRR->_rr_type);
	//xprintf_rr(elemRR);
	//xprintf_rdf(elemRR->_rdata_fields[0]);

	uint8_t c;
	int i=0;
	uint8_t *str = ldns_rdf_data(elemRR->_rdata_fields[0]);
	char *ss = (char *) malloc(sizeof(char));

	for(;str[i]!='\0';i++) {
		if(i==0) {
			ss[i] = ' ';
			continue;
		}
		if(str[i] < 10)
			ss[i] = '.';
		else
			ss[i] = (char)str[i];
	}
	printf("QT(%s)=%d\n",ss,getQueryTime(ss));

	ldns_rdf_deep_free(name);
	ldns_resolver_deep_free(res);
	exit(EXIT_SUCCESS);
}

