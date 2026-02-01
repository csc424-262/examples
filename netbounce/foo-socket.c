/*
** name: foo-socket.c
**
** it's all about the Foo
**
** general socket wrapper for csc424. 
**
** last modified: 
**		04 apr 2022 -bjr; created
**		31 jan 2023 -bjr; simplified as Foo for netbounce
**		05 mar 2024 -bjr; added create session socket
**		01 feb 2026 -bjr; updated comments
**
*/

/*
 * create_foo_socket(port): 
 *    returns a FooSocket bound to the given port or a fresh ephemeral port if port==0
 *
 * socket_sendto(sock, host, port, message, message_length): 
 *    sends message of message_length bytes to host:port using FooSocket sock
 *
 * socket_recvfrom(sock, buffer, buffersize): 
 *    given a bound FooSocket sock, receive bytes and copy to buffer, up to buffersize; 
 *    and sets the foo socket's replyto_addr to the sender's IP and port
 *
 * socket_replyto(sock, message, message_length): 
 *    sends message of message_length to replyto_addr. 
 *
 * create_session_socket(struct FooSocket * sock_listen): 
 *    clones the sock_listen to a fresh socket, copying over the reply-to.
 *    to create a session socket after a socket_recvfrom
 *
 */
 
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<errno.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include "foo-socket.h"

extern int is_verbose_g ;
extern int debug_g ;

void print_hex_line(char * buffer, int n) {
	int i ;
	int j = 0 ;
	for (i=0;i<n;i++) {
		j = 1 ;
		printf("%02hhx ", buffer[i]) ;
		if ((i&15)==15) {
			printf("\n\t") ;
			j = 0;
		}
	}
	if (j) printf("\n") ;
	return ;
}

void its_all_about_the_foo(void) {
	printf("It's all about the foo!\n") ;
	return ;
}

struct FooSocket * create_foo_socket(int port) {

	/*
	 * if port==0, socket binds to an ephemeral port
	 */
	 
	int fd ;
	struct sockaddr_in sa_i ;
	struct FooSocket * so_p ;
	struct sockaddr_in * sa_rt ;
	
	so_p = (struct FooSocket *) malloc(sizeof(struct FooSocket)) ;
	assert(so_p) ;
	
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
		perror("socket") ;
		exit(1) ;
	}
	
	sa_i.sin_family = AF_INET ;
	sa_i.sin_port = htons((short)port) ;
	sa_i.sin_addr.s_addr = INADDR_ANY ;
	memset(&(sa_i.sin_zero),'\0',8) ;

	if (bind(fd, (struct sockaddr *) &sa_i, sizeof(struct sockaddr)) == -1 ) {
		perror("bind") ;
		exit(1) ;
	}
	
	so_p->sockfd = fd ;
	sa_rt = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in)) ;
	assert(sa_rt) ;
	memset(sa_rt,0,sizeof(struct sockaddr_in)) ;
	so_p->replyto_addr = sa_rt ;
	return so_p ;
}

struct FooSocket * create_session_socket(struct FooSocket * sock_listen) {
        struct FooSocket * sock_session ;
        struct sockaddr_in sa_i ;

        // create an ephemeral socket, this is to take care of the
        // DATA and ACK packets, for this particular transer TID

        sock_session = create_foo_socket(0) ;
        assert(sock_session) ;

        // copy over the reply to from the sock_listen to the sock_session.
        // you must really do a dup on the address structure, because the next
        // use of sock_listen will overwrite the information in that structure

        sock_session->replyto_addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in)) ;
        memcpy(sock_session->replyto_addr, sock_listen->replyto_addr, sizeof(struct sockaddr_in)) ;
        return sock_session ;
}

int socket_sendto(struct FooSocket * sock, char * host, int port, char * buf, int msg_len) {
	struct sockaddr_in dest_addr ;
	struct sockaddr_in my_addr ;
	struct hostent *he ;
	int numbytes ;
	int addr_len ;

	if ((he=gethostbyname(host))==NULL) {
		perror("gethostbyname") ;
		exit(1) ;
	}
	dest_addr.sin_family = AF_INET ;
	dest_addr.sin_port = htons((short)port) ;
	dest_addr.sin_addr = *((struct in_addr *)he->h_addr) ;  // this typing is tricky
	memset(&(dest_addr.sin_zero), '\0', 8 ) ;
	
	numbytes = sendto(sock->sockfd, buf, msg_len, 0, 
				(struct sockaddr *) &dest_addr, sizeof(struct sockaddr_in)) ;

	if (numbytes== -1 ) {
		perror("sendto") ;
		exit(1) ;
	}
		
	IF_VERBOSE {
		addr_len = sizeof(struct sockaddr_in) ;
		getsockname( sock->sockfd, (struct sockaddr *) &my_addr, (socklen_t *) &addr_len ) ;
		
		printf("%s (%s:%d)\n\tsent %d bytes on port %d to %s:%d\n\t", 
			"socket_sendto",
			__FILE__, __LINE__,
			numbytes, ntohs(my_addr.sin_port),
			inet_ntoa(dest_addr.sin_addr), ntohs(dest_addr.sin_port)) ;
		print_hex_line(buf,numbytes) ;
	}

	return numbytes ;
}

int socket_recvfrom( struct FooSocket * sock, char * buf, int buf_len) {
	unsigned int addr_len, numbytes;
	struct sockaddr_in * sa_i = sock->replyto_addr ;
	struct sockaddr_in my_addr ;

	addr_len = sizeof(struct sockaddr_in) ;
	numbytes = recvfrom(sock->sockfd, buf, buf_len, 0, (struct sockaddr *) sock->replyto_addr, &addr_len) ;
	if (numbytes == -1 ) {
		perror("recvfrom") ;
		exit(1) ;
	}
	
	IF_VERBOSE {
		addr_len = sizeof(struct sockaddr_in) ;
		getsockname( sock->sockfd, (struct sockaddr *) &my_addr, (socklen_t *) &addr_len ) ;

		printf("%s (%s:%d)\n\tgot %d bytes on port %d from %s:%d\n\t", 
				"socket_recvfrom",
				__FILE__, __LINE__,
				numbytes, ntohs(my_addr.sin_port),
				inet_ntoa(sock->replyto_addr->sin_addr), ntohs(sock->replyto_addr->sin_port) ) ;
		print_hex_line(buf,numbytes) ;
	}

	return numbytes ;
}

int socket_replyto(struct FooSocket * sock, char * buf, int msg_len) {

	struct sockaddr_in my_addr ;
	int numbytes ;
	int addr_len ;

	numbytes = sendto(sock->sockfd, buf, msg_len, 0, 
				(struct sockaddr *) sock->replyto_addr, sizeof(struct sockaddr)) ;
	if (numbytes== -1 ) {
		perror("sendto") ;
		exit(1) ;
	}
		
	IF_VERBOSE {
		addr_len = sizeof(struct sockaddr_in) ;
		getsockname( sock->sockfd, (struct sockaddr *) &my_addr, (socklen_t *) &addr_len ) ;
		
		printf("%s (%s:%d)\n\tsent %d bytes on port %d to %s:%d\n\t", 
			"socket_sendto",
			__FILE__, __LINE__,
			numbytes, ntohs(my_addr.sin_port),
			inet_ntoa(sock->replyto_addr->sin_addr), ntohs(sock->replyto_addr->sin_port)) ;
		print_hex_line(buf,numbytes) ;
	}

	return numbytes ;
}

// END

