/*
** netbounce-client.c
** from Beej's Guide to Network Programming: Using Unix Sockets, 
** by Brian "Beej" Hall.
**
** modified by Burt Rosenberg, 
** Created: Feb 8, 2009
** Last modified: Jan 31, 2015 - minor changes -bjr
**    Jan 12, 2015 - added getopt
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<assert.h>

#include "foo-socket.h"

#define MAXBUFLEN 100

#define USAGE_MESSAGE "usage: %s [-v] -p port -h host message\n" 
#define PROG_NAME "netbounce-client-foo" 

int verbose_g = 0 ;

int main( int argc, char * argv[] ) {

	struct FooSocket * foo_sock ;
	int numbytes ;
	int port = 0 ;
	char * host = NULL ;
	char * msg = NULL ;
	int ch ;
	int is_verbose = 0 ;
	char buf[MAXBUFLEN];
	
	while ((ch = getopt(argc, argv, "vp:h:")) != -1) {
		switch(ch) {
			case 'p':
				port = atoi(optarg) ;
				break ;
			case 'h':
				host = strdup(optarg) ;
				break ;
			case 'v':
				++verbose_g ;
				break ;
			default:
				printf(USAGE_MESSAGE, PROG_NAME) ;
				return 0 ;
		}
	}
	argc -= optind;
	argv += optind;
	
	if ( !host || !port || ! argc ) {
		printf(USAGE_MESSAGE, PROG_NAME) ;
		return 0 ;
	}
	msg = strdup(*argv) ;

	/* example of an assertion */
	assert(host) ;
	assert(port) ;

	foo_sock = create_foo_socket(0) ;
	
	if ((numbytes=socket_sendto(foo_sock, host, port, msg, strlen(msg)))== -1 ) {
		perror("sendto") ;
		exit(1) ;
	}

	numbytes = socket_recvfrom(foo_sock, buf, MAXBUFLEN-1, 0) ;
	assert(numbytes>=0) ;
		
	buf[numbytes] = '\0' ;
	printf("%s\n", buf ) ;

	return 0 ;
}

