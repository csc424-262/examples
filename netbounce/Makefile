#
# makefile for csc424-242 project 2
# Feb 08, 2024
# bjr
#

CC = gcc
MACHINE = localhost
MACHINE = 18.212.107.211
PORT = 3333
IF = eth0

LOGFILE= /var/www/ubuntu/netbounce-log.txt
#LOGFILE= netbounce-log.txt

all:
	@echo "update MACHINE macro with server IP"
	@echo "run |make run-server| to start server"
	@echo "run |make run-client| to bounce"
	make netbounce

netbounce: netbounce-server2 netbounce-server netbounce-client

netbounce-server2: netbounce-server2.c foo-socket.o
	${CC} -o $@ $< foo-socket.o
	
netbounce-server: netbounce-server.c 
	${CC} -o $@ $<
	
netbounce-client: netbounce-client.c 
	${CC} -o $@ $<

foo-socket.o: foo-socket.c foo-socket.h
	cc -c -o $@ $<
	
test:
	@echo In one window, make run-server
	@echo Then, in another window, make run-client
	
run-server: netbounce-server
	@echo Ready to bounce!
	./netbounce-server -lv -p ${PORT} $F

run-server2: netbounce-server2
	@echo Ready to bounce!
	echo -n "<p><b>Running from hostname - </b>" >> ${LOGFILE}
	/usr/bin/hostname >> ${LOGFILE}
	./netbounce-server2 -lv -f ${LOGFILE} -p ${PORT}

run-client: netbounce-client
	./netbounce-client -h ${MACHINE} -p ${PORT} -v  "The amazing net bounce!"

find-pid:
	@echo problem- bind: Address already in use
	@echo use netstat to find the pid holding the port then kill -9
	netstat -tulpn | grep ${PORT}

tcpdump:
	sudo tcpdump -i ${IF} -x udp port ${PORT}

clean:
	-echo `date` > ${LOGFILE}
	-rm netbounce-client netbounce-server netbounce-server2 foo-socket.o

