/*
 * sSocket.cpp
 *
 *  Created on: Nov 25, 2014
 *      Author: kevin
 */

#include "sSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

namespace ServerSocket {

void sSocket::error(const char *msg)
{
    perror(msg);
    exit(1);
}

void sSocket::initss()
{

    socklen_t clilen; 								//Tamaño del cliente
    struct sockaddr_in serv_addr, cli_addr; 		//Estructura de dirección simplificada

    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);	//Crea socket TCP/IP del tipo Stream, protocolo adecuado "0"

    if (this->sockfd < 0)				//Verificación de creación de socket
       error("ERROR opening socket");

    bzero((char *) &(serv_addr), sizeof(serv_addr));	//Limpia estructura de dirección del servidor

    serv_addr.sin_family = AF_INET;				//Comunicación mediante TCP/IP
    serv_addr.sin_addr.s_addr = INADDR_ANY;		//Dirección IP de la máquina
    serv_addr.sin_port = htons(this->portno);	//Puerto de conexión - host to network short
    //serv_addr.sin_zero Números complemento

    if (bind(this->sockfd, (struct sockaddr *) &(serv_addr),sizeof(serv_addr)) < 0)	 //asigna dirección y puerto al identificador de socket
    	error("ERROR on binding");	//regresa -1 si no es posible

    listen(this->sockfd,1);// espera hasta establecer una conección (5 conexiones en lista de espera)

    clilen = sizeof(cli_addr); //longitud de cliente
    this->newsockfd = accept(this->sockfd,(struct sockaddr *) &(cli_addr),&clilen); //acepta conexión y crea identificador para la comunicación

    if (this->newsockfd < 0)		//verifica creación de socket de comunicación establecida
         error("ERROR on accept");

}

void sSocket::readss()
{
    this->n = read(this->newsockfd,this->buffer,this->bsize); // Lee datos entrantes a través de socket newsockfd en el buffer
    if (this->n < 0) error("ERROR reading from socket"); //Regresa en n el número de bytes recibidos, si falla regresa -1
    //std::cout << this->buffer<<std::endl<<std::endl<<std::endl;
}

void sSocket::writess()
{
	this->n = write(this->newsockfd,this->buffer,this->bsize);	//Envía los datos de buffer a través de newsockfd
    if (this->n < 0) error("ERROR writing to socket"); //Regresa en n el número de bytes enviados, si falla regresa -1
}

void sSocket::closess()
{
    close(this->newsockfd); //Cierra socket de conexión
    close(this->sockfd); 	//Cierra socket de servidor
}

sSocket::sSocket() {
	// TODO Auto-generated constructor stub
}

sSocket::sSocket(int portno) //Creación de un socket en el puerto portno
{
	this->portno=portno; //Número de puerto
	this->bsize=sizeof(this->buffer);
}

sSocket::~sSocket() {
}

} /* namespace ServerSocket */
