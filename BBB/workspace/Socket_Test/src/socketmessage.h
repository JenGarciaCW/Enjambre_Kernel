/*
 * socketmessage.h
 *
 *  Created on: Feb 10, 2015
 *      Author: kevin
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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
	const char * host;

	void init_tcp_client_socket();
	void init_tcp_server_socket();
	void init_udp_receiver_socket();
	void init_udp_sender_socket();
	void write_tcp();
	void read_tcp();
	void write_udp();
	void read_udp();
	void clear_buffer();
	void close_socket();
	void error(const char *msg);
	socket_message(int portno,const char * host,int bufsize);
	virtual ~socket_message();
};

} /* namespace socket_message */

#endif /* SRC_SOCKETMESSAGE_H_ */
