/*
 * socketmessage.h
 *
 *  Created on: Feb 10, 2015
 *      Author: kevin
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/un.h>


#ifndef SRC_SOCKETMESSAGE_H_
#define SRC_SOCKETMESSAGE_H_

namespace socket_msg {

class socket_message {
public:

	int portno;		//Socket port number
	int sockfd;		//Socket descriptor
	struct sockaddr_in serv_addr; //Simplified server address
	struct hostent *server;
    char* buffer;	//Data send/receive buffer
    int  bsize;		//Data buffer size
    int recvlen;	//Number of received data bytes
	const char * host; // direcci��n del cliente
	struct sockaddr_un userver;

	void init_tcp_client_socket();	// m��todo de creaci��n de cliente tcp
	void init_tcp_server_socket();	// m��todo de creaci��n de servidor tcp
	void init_udp_receiver_socket(); // m��todo de creaci��n de cliente udp
	void init_udp_sender_socket(); // m��todo de creaci��n de servidor udp
	void init_unix_server_socket();
	void init_unix_client_socket();
	void write_tcp();
	void read_tcp();
	void write_udp();
	void read_udp();
	void write_unix();
	void read_unix();
	void clear_buffer();
	void close_socket();
	void error(const char *msg);
	socket_message(const char * file,int size);
	socket_message(int portno,const char * host,int bufsize);
	virtual ~socket_message();
};

} /* namespace socket_message */

#endif /* SRC_SOCKETMESSAGE_H_ */
