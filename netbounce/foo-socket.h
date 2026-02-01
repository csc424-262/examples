/*
** name: foo-socket.h
**
** last modified: 
**		04 apr 2022 -bjr; plan for a uniform socket object
**		31 jan 2023 -bjr; foo sockets
**		05 mar 2024 -bjr; add create_session_socket
**
** it's all about the FooSocket
**
*/

#ifndef FOOSOCKET_H
#define FOOSOCKET_H

#define IF_VERBOSE if (verbose_g>0)

extern int verbose_g ;
extern int debug_g ;

struct FooSocket {
	int sockfd ;
	struct sockaddr_in * replyto_addr ;
} ;

void print_hex_line(char * buffer, int n) ;
void its_all_about_the_foo(void) ;

struct FooSocket * create_foo_socket(int port) ;

int socket_sendto(struct FooSocket * sock, char * host, int port, char * buf, int msg_len) ;
int socket_recvfrom( struct FooSocket * sock, char * buf, int buf_len) ;
int socket_replyto(struct FooSocket * sock, char * buf, int msg_len) ;
struct FooSocket * create_session_socket(struct FooSocket * sock_listen) ;

#endif // FOOSOCKET_H


