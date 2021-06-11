//header.h
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
 
#define PORT 20162
#define BUFFER_SIZE 100
#define LISTEN_QUEUE_SIZE 5
