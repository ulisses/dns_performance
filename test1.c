#include <stdio.h>
#include <sys/types.h>
#include <ldns/ldns.h>

/*
*/

static int usage(FILE *fp, char *prog) {
	fprintf(fp, "%s server\n", prog);
	fprintf(fp, "  print out some information about server\n");
	return 0;
}

int main(int argc, char *argv[]) {
	ldns_resolver *res;
	ldns_rdf *name;
	ldns_rdf *version, *id;
	ldns_pkt *p;
	ldns_rr_list *addr;
	ldns_rr_list *info;
	ldns_status s;
	ldns_rdf *pop;
	size_t i;

	if (argc != 2) {
		usage(stdout, argv[0]);
		exit(EXIT_FAILURE);
	} else {
		/* Create a rdf from the command line arg */
		name = ldns_dname_new_frm_str(argv[1]);
		ldns_dname2canonical(name);
		if (!name) {
			usage(stdout, argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	/* Create rdf for what we are going to ask */
	version = ldns_dname_new_frm_str("query example");

	/* Create a new resolver from /etc/resolv.conf */
	s = ldns_resolver_new_frm_file(&res, NULL);
	
	if (s != LDNS_STATUS_OK) {
		ldns_rdf_deep_free(name);
		exit(EXIT_FAILURE);
	}
	/* Force recursion, we want the first DNS server to do the all the job*/
	if(!ldns_resolver_recursive(res)) {
		ldns_resolver_set_recursive(res,false);
	}
	ldns_resolver_set_retry(res, 1); /* don't want to wait too long */

	/* use the resolver to send it a query for the a/aaaa of name */
	addr = ldns_get_rr_list_addr_by_name(res, name, LDNS_RR_CLASS_IN, LDNS_RD); /* LDNS_RD: Recursion Desired - query flag */
	//addr = ldns_get_rr_list_name_by_addr(res, name, LDNS_RR_CLASS_IN, LDNS_RD); /* LDNS_RD: Recursion Desired - query flag */
	if (!addr) {
		fprintf(stderr, " *** could not get an address for %s\n", argv[1]);
		ldns_rdf_deep_free(name);
		ldns_resolver_deep_free(res);
		exit(EXIT_FAILURE);
	}

	/* remove current list of nameservers from resolver */
	while((pop = ldns_resolver_pop_nameserver(res))) {ldns_rdf_deep_free(pop);}

	/* can be multihomed */
	for(i = 0; i < ldns_rr_list_rr_count(addr); i++) {
		if (i > 0) {
			fprintf(stdout, "\n");
		}

		if (ldns_resolver_push_nameserver_rr(res,ldns_rr_list_rr(addr, i)) != LDNS_STATUS_OK) {
			printf("Error adding nameserver to resolver\n");
		}

		p = ldns_resolver_query(res, version, LDNS_RR_TYPE_TXT,LDNS_RR_CLASS_CH, LDNS_RD);
		if (p) {
			printf("*********QUERY TIME:%u\n",ldns_pkt_querytime(p));
			ldns_pkt_free(p);
		} else {
			printf(" *** query failed\n");
		}
		ldns_rdf_deep_free(ldns_resolver_pop_nameserver(res));
	}

	ldns_rdf_deep_free(name);
	ldns_resolver_deep_free(res);
	exit(EXIT_SUCCESS);
}

