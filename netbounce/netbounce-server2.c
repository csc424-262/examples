/*
**
** name: netbounce-server2.c
**
** the amazing net-bounce, now with foo-sockets
**
** last-modified:
**     08 feb 2009 -bjr created
**     01 feb 2026 -bjr migrated to foo-socket's
**
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
#include<arpa/inet.h>
#include<netdb.h>
#include<time.h>
#include<assert.h>

#include "foo-socket.h"

#define USAGE_MSG "usage: %s [-lv] -p port\n"
#define PROG_NAME "netbounce-server2"  
#define LOCALHOST "localhost"
#define MAXMSGLEN 2048
#define N_REPEAT_DEFAULT 1

int verbose_g = 0 ;

char * hostname_to_ipstring(char * host) {
	// the returned result must be used immediately, as it is a static
	// inside the gethostbyname static variables block
	struct hostent * he ;
	if ((he=gethostbyname(host))==NULL) {
		//perror("gethostbyname") ;
		printf("error: no such host %s\n", host ) ;
		exit(1) ;
	}
	return inet_ntoa(*((struct in_addr *)he->h_addr)) ;
}

int main(int argc, char * argv[]) {

	unsigned int addr_len, numbytes;
	char buf[MAXMSGLEN];
	int ch ;
	int port = 0 ;
	int is_loop = 0 ;
	struct FooSocket * sock ;

	while ((ch = getopt(argc, argv, "vp:l")) != -1) {
		switch(ch) {
			case 'p':
				port = atoi(optarg) ;
				break ;
			case 'v':
				verbose_g += 1 ;
				break ;
			case 'l':
				is_loop = 1 ;
				break ;
			default:
				printf(USAGE_MSG, PROG_NAME) ;
				return 0 ;
		}
	}
	argc -= optind;
	argv += optind;

	if ( !port ) {
		printf(USAGE_MSG, PROG_NAME) ;
		return 0 ;
	}

	assert(port) ;
	sock = create_foo_socket(port) ;

	while (1==1 ) { /* while forever */

		numbytes = socket_recvfrom(sock, buf, MAXMSGLEN-1) ;
		// assume can be a string, and terminate
		buf[numbytes] = '\0' ;

		printf("got packet from %s, port %d\n", 
				inet_ntoa(sock->replyto_addr->sin_addr), 
				ntohs(sock->replyto_addr->sin_port)) ;
		printf("packet is %d bytes long\n", numbytes ) ;
		/* Mild bug: if the incoming data was binary, the "string" might be less than numbytes long */
		printf("packet contains \"%s\"\n", buf ) ;

		numbytes = socket_replyto(sock, buf, numbytes) ;
		printf("packet sent, %d bytes\n", numbytes) ;

		fflush(NULL) ;
		if ( !is_loop ) break ;
	}

	return 0 ;
}
