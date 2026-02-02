#
# makefile for csc424-242 netbounce project
#
# author: bjr
# last-update:
#     Feb 08, 2024
#     Feb 01, 2026
#


#H= localhost
H= 18.212.107.211
PORT = 3333
#LOGFILE= netbounce-log.txt
LOGFILE= /var/www/ubuntu/netbounce-log.txt

CC = gcc

all:
	@echo "simple use:"
	@echo "  1) set the H macro"
	@echo "  2) make run-server2"
	@echo "  3) make run-client (other window)"
	@echo "make targets: "
	@echo "  compile"
	@echo "  run-server2"
	@echo "  run-server2-weblog"
	@echo "  run-client"
	@echo "  clean"
	@echo "macros:"
	@echo "  H=$H"
	@echo "  LOGFILE=${LOGFILE}"


compile: netbounce-server2 netbounce-server netbounce-client

netbounce-server2: netbounce-server2.c foo-socket.o
	${CC} -o $@ $< foo-socket.o
	
netbounce-client: netbounce-client.c 
	${CC} -o $@ $<

foo-socket.o: foo-socket.c foo-socket.h
	cc -c -o $@ $<
	
test:
	@echo In one window, make run-server2
	@echo Then, in another window, make run-client

run-server2: netbounce-server2
	@echo Ready to bounce!
	./netbounce-server2 -l -p ${PORT}

run-client: netbounce-client
	./netbounce-client -h $H -p ${PORT} -v  "The amazing net bounce!"

run-server2-weblog: netbounce-server2
	@echo Ready to bounce!
	touch ${LOGFILE}
	echo -n "<p><b>Running from hostname - </b>" >> ${LOGFILE}
	/bin/hostname >> ${LOGFILE}
	./netbounce-server2 -l -p ${PORT} >> ${LOGFILE}

find-pid:
	@echo problem- bind: Address already in use
	@echo use netstat to find the pid holding the port then kill -9
	netstat -tulpn | grep ${PORT}

tcpdump:
	sudo tcpdump -i ${IF} -x udp port ${PORT}

clean:
	-rm netbounce-client netbounce-server2 foo-socket.o
	-rm ${LOGFILE}

