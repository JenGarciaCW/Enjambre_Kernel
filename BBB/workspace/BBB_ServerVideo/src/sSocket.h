/*
 * sSocket.h
 *
 *  Created on: Nov 25, 2014
 *      Author: kevin
 */

#ifndef SSOCKET_H_
#define SSOCKET_H_

namespace ServerSocket {

class sSocket {
public:
	int portno;				//Número de puerto
    int sockfd, newsockfd;	//Identificador de socket
    char buffer[921700];
    int  bsize;
    int n;
    sSocket(int portno);
	sSocket();
	void initss();
	void readss();
	void writess();
	void closess();
	void error(const char *msg);
	virtual ~sSocket();
};

} /* namespace ServerSocket */

#endif /* SSOCKET_H_ */
