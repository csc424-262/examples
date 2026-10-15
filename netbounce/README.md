# Netbounce Classic

**A first trip on the information highway.**

Netbounce Classic is a small client-server networking exercise, 
originally developed in 2013 from examples 
in **[Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)** 
by Brian "Beej" Hall.

[Netbounce — the original project page](https://www.cs.miami.edu/home/burt/learning/Csc424.172/workbook/netbounce.html)

The classic example demonstrates the essential pattern of network programming: a client sends a 
UDP datagram to a server, and the server sends a reply to the client's address and port.

## FooSockets

This repository preserves that example and its evolution into my own **FooSockets** library, 
a small abstraction over the socket interface intended to simplify network programming for students.
