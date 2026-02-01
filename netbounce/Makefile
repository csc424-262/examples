#
# makefile for csc424-242 project 2
# Feb 08, 2024
# bjr
#

CC = gcc
MACHINE = localhost
MACHINE = 54.81.57.81
PORT = 3333
IF = eth0

LOGFILE= /var/www/ubuntu/project2/netbounce-log.txt
LOGFILE= netbounce-log.txt
F= -f ${LOGFILE} 
#F=

all:
	@echo "update MACHINE macro with server IP"
	@echo "run |make run-server| to start server"
	@echo "run |make run-client| to bounce"
	make netbounce

netbounce: netbounce-server netbounce-client

netbounce-server: netbounce-server.c 
	${CC} -o $@ $<
	
netbounce-client: netbounce-client.c 
	${CC} -o $@ $<
	
test:
	@echo In one window, make run-server
	@echo Then, in another window, make run-client
	
run-server: netbounce-server
	@echo Ready to bounce!
	./netbounce-server -lv -p ${PORT} $F

run-client: netbounce-client
	./netbounce-client -h ${MACHINE} -p ${PORT} -v  $F "The amazing net bounce!"

find-pid:
	@echo problem- bind: Address already in use
	@echo use netstat to find the pid holding the port then kill -9
	netstat -tulpn | grep ${PORT}

tcpdump:
	sudo tcpdump -i ${IF} -x udp port ${PORT}

clean:
	-echo `date` > ${LOGFILE}
	-rm netbounce-client netbounce-server

