#
# makefile for csc424-242 netbounce project
#
# author: bjr
# last-update:
#     Feb 08, 2024
#     Feb 01, 2026
#     Oct 15, 2026
#


HOST= localhost
#H= 18.212.107.211
PORT = 3333
S_LOG= netbounce.log

CC = gcc

all:
	@echo "simple use:"
	@echo "  1) set the H macro"
	@echo "  2) make run-server-foo"
	@echo "  3) make run-client-foo (other window)"
	@echo "use classic Beej code:"
	@echo "  1) set the H macro"
	@echo "  2) make run-server"
	@echo "  3) make run-client (other window)"
	@echo "make test for simplest test"

build: netbounce-server-foo netbounce-server netbounce-client-foo netbounce-client

netbounce-server-foo: netbounce-server-foo.c foo-socket.o
	${CC} -o $@ $< foo-socket.o
	
netbounce-client-foo: netbounce-client-foo.c foo-socket.o
	${CC} -o $@ $< foo-socket.o

netbounce-server: netbounce-server.c
	${CC} -o $@ $< 
	
netbounce-client: netbounce-client.c 
	${CC} -o $@ $<

foo-socket.o: foo-socket.c foo-socket.h
	cc -c -o $@ $<

test: netbounce-server-foo netbounce-client-foo
	( ./netbounce-server-foo -v -p ${PORT} > ${S_LOG} 2>&1 & server_pid=$$!; \
		sleep 1; \
		./netbounce-client-foo -h ${HOST} -p ${PORT} -v  "The amazing net bounce!" ; \
		status=$$?; \
		kill $$server_pid 2>/dev/null; \
		wait $$server_pid 2>/dev/null; \
		exit $$status )
	  @echo ""
	  @echo "server log:"
	  @cat ${S_LOG}
	
run-server-foo: netbounce-server-foo
	@echo Ready to bounce!
	./netbounce-server-foo -v -p ${PORT}

run-client-foo: netbounce-client-foo
	./netbounce-client-foo -h ${HOST} -p ${PORT} -v  "The amazing net bounce!"

run-server: netbounce-server
	@echo Ready to bounce!
	./netbounce-server -v -p ${PORT}

run-client: netbounce-client
	./netbounce-client -h ${HOST} -p ${PORT} -v  "The amazing net bounce!"

find-pid:
	@echo problem- bind: Address already in use
	@echo use netstat to find the pid holding the port then kill -9
	netstat -tulpn | grep ${PORT}

tcpdump:
	sudo tcpdump -i ${IF} -x udp port ${PORT}

clean:
	-rm netbounce-server
	-rm netbounce-client netbounce-server-foo foo-socket.o
	-rm ${S_LOG}

