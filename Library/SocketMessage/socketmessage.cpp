/*
 * socketmessage.cpp
 *
 *  Created on: Feb 10, 2015
 *      Author: kevin
 */

#include "socketmessage.h"


namespace socket_msg {

socket_message::socket_message(){

}

socket_message::socket_message(int portno,const char * host,int bufsize) {
	this->portno=portno;	//N��mero de puerto
	this->host=host;		//IP del servidor
	this->server = gethostbyname(this->host);	// Obtiene nombre y direcci��n del servidor
    this->buffer = new char[bufsize];	//Inicializa buffer
    this->bsize=bufsize;
}

socket_message::socket_message(const char *file,int size){

    this->buffer = new char[size];	//Inicializa buffer

	this->sockfd= socket(AF_UNIX, SOCK_STREAM, 0);
	 if (this->sockfd < 0) {
		 perror("opening stream socket");
	 exit(1);
	 }

	 this->userver.sun_family = AF_UNIX;
	 strcpy(this->userver.sun_path, file);
}

socket_message::~socket_message() {
	// TODO Auto-generated destructor stub
}

void socket_message::generate_sm(int portno,const char * host,int bufsize){
	this->portno=portno;	//N��mero de puerto
	this->host=host;		//IP del servidor
	this->server = gethostbyname(this->host);	// Obtiene nombre y direcci��n del servidor
    this->buffer = new char[bufsize];	//Inicializa buffer
    this->bsize=bufsize;
}

void socket_message::init_unix_server_socket()
{

	 if (bind(this->sockfd, (struct sockaddr *) &this->userver, sizeof(struct sockaddr_un))) {
		 perror("binding stream socket");
		 exit(1);
	 }

	 printf("Socket has name %s\n", this->userver.sun_path);
/*
 * Start acceptin conections, and start reading on the acepted conection
 */
	 listen(this->sockfd, 5);

		 this->sockfd = accept(this->sockfd, 0, 0);
		 if (this->sockfd == -1)
			 perror("accept");

}

void socket_message::init_unix_client_socket()
{
	 if (connect(this->sockfd, (struct sockaddr *) &this->userver, sizeof(struct sockaddr_un)) < 0) {
		 close(this->sockfd);
		 perror("connecting stream socket");
		 exit(1);
	 }
}

void socket_message::init_tcp_server_socket()
{
	/*
	 * Solo necesita direcci��n interna
	 */
    socklen_t clilen; 								//Tama��oo del cliente
    struct sockaddr_in serv_addr, cli_addr; 		//Estructura de direcci��n simplificada

    /*
     * Creaci��n de socket
     */
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);		//Crea socket TCP/IP del tipo Stream, protocolo adecuado "0"
    if (this->sockfd < 0)								//Verificaci��n de creaci��n de socket
       error("ERROR opening socket");

    /*
     * Configuraci��n de servidor
     */
    bzero((char *) &(serv_addr), sizeof(serv_addr));	//Limpia estructura de direcci��n del servidor
    serv_addr.sin_family = AF_INET;						//Comunicaci��n mediante TCP/IP
    serv_addr.sin_addr.s_addr = inet_addr(this->host);		//Direcci��n IP de la m��quina
    serv_addr.sin_port = htons(this->portno);			//Puerto de conexi��n - host to network short
    //serv_addr.sin_zero N��meros complemento


    if (bind(this->sockfd, (struct sockaddr *) &(serv_addr),sizeof(serv_addr)) < 0)	 //asigna direcci��n y puerto al identificador de socket
    	error("ERROR on binding");													//regresa -1 si no es posible

    listen(this->sockfd,1);		// espera hasta establecer una conecci��n (1 conexi��n en lista de espera)

    clilen = sizeof(cli_addr); 	//longitud de cliente
    this->sockfd = accept(this->sockfd,(struct sockaddr *) &(cli_addr),&clilen); //acepta conexi��n y crea identificador para la comunicaci��n

    if (this->sockfd < 0)														//verifica creaci��n de socket de comunicaci��n establecida
         error("ERROR on accept");

}

void socket_message::init_tcp_client_socket()
{
	/*
	 * Necesita  direcci��n del servidor
	 */
	 // Function init_tcp_clinet_socket creates a tcp/ip socket, configure the host address and
	 // try to connect to the server socket at host address
		//Creates a TCP/IP socket and send an error if it cannot be opened
	    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (this->sockfd < 0)
	    	error("ERROR opening socket");

	    //Assign a connection address for the server socket
	    if (this->server == NULL) {
	        fprintf(stderr,"ERROR, no such host\n");
	        exit(0);
	    }

  	    bzero((char *) &(this->serv_addr), sizeof(this->serv_addr));

	    this->serv_addr.sin_family = AF_INET;
		bcopy((char *)this->server->h_addr,(char *)&(this->serv_addr.sin_addr.s_addr),this->server->h_length); // Asigna direcci��n de servidor
	    this->serv_addr.sin_port = htons(this->portno);

	    // Try to connect to the server socket
	    while(connect(this->sockfd,(struct sockaddr *) &(this->serv_addr),sizeof(this->serv_addr)) < 0)
	        error("ERROR connecting");

	    	//if (connect(this->sockfd,(struct sockaddr *) &(this->serv_addr),sizeof(this->serv_addr)) < 0)
	        //error("ERROR connecting");
}

void socket_message::init_udp_receiver_socket()
{
	/*
	 * Necesita direcci��n interna
	 */

		this->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	    if (this->sockfd < 0)							//Verificaci��n de creaci��n de socket
	       error("ERROR opening socket");

	    bzero((char *) &(this->serv_addr), sizeof(this->serv_addr));	//Limpia estructura de direcci��n del servidor

	    this->serv_addr.sin_family = AF_INET;						//Comunicaci��n mediante TCP/IP
	    this->serv_addr.sin_addr.s_addr = inet_addr(this->host);				//Direcci��n IP de la m��quina
	    this->serv_addr.sin_port = htons(this->portno);			//Puerto de conexi��n - host to network short
	    //serv_addr.sin_zero N��meros complemento

	    if (bind(this->sockfd, (struct sockaddr *) &(this->serv_addr),sizeof(this->serv_addr)) < 0)	 //asigna direcci��n y puerto al identificador de socket
	    	error("ERROR on binding");	//regresa -1 si no es posible

}

void socket_message::init_udp_sender_socket()
{

/*
 * Necesita direcci��n del servidor
 */

    if(!this->server)
    {
    	error("Not such host");
    }

	//Crea socket UDP del tipo Stream, protocolo adecuado "0"
    this->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->sockfd < 0)							//Verificaci��n de creaci��n de socket
       error("ERROR opening socket");

    bzero((char *) &(this->serv_addr), sizeof(this->serv_addr));	//Limpia estructura de direcci��n del servidor

    this->serv_addr.sin_family = AF_INET;						//Comunicaci��n mediante TCP/IP
    memcpy((void *)&this->serv_addr.sin_addr, this->server->h_addr_list[0], this->server->h_length);
    this->serv_addr.sin_port = htons(this->portno);			//Puerto de conexi��n - host to network short
    //serv_addr.sin_zero N��meros complemento

}

void socket_message::close_socket()
{
    close(this->sockfd); 	//Closess communication socket
}

void socket_message::read_tcp()
{
    this->recvlen = recv(this->sockfd,this->buffer,this->bsize,MSG_WAITALL); // Lee datos entrantes a trav��s de socket newsockfd en el buffer
    if (this->recvlen < 0) error("ERROR reading from socket"); //Regresa en n el n��mero de bytes recibidos, si falla regresa -1
}

void socket_message::write_tcp()
{
	this->recvlen = write(this->sockfd,this->buffer,this->bsize);	//Env��a los datos de buffer a traves de newsockfd
    if (this->recvlen < 0) error("ERROR writing to socket"); //Regresa en n el n��mero de bytes enviados, si falla regresa -1
}

void socket_message::write_udp()
{
	sendto(this->sockfd,this->buffer,this->bsize,0,(struct sockaddr *)&this->serv_addr, sizeof(this->serv_addr)); // ecribe mensaje udp
}

void socket_message::read_udp()
{

    recv(this->sockfd,this->buffer,this->bsize,0);//,(struct sockaddr *) &(cli_addr),&clilen);
}

void socket_message::read_udp(int sec, int milisec)
{
	struct timeval timeout={sec,milisec};
	setsockopt(this->sockfd,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
    recv(this->sockfd,this->buffer,this->bsize,0);//,(struct sockaddr *) &(cli_addr),&clilen);
}

void socket_message::write_unix()
{
	this->recvlen=write(this->sockfd,this->buffer,sizeof(this->buffer));
    if (this->recvlen < 0) error("ERROR writing to socket"); //Regresa en n el n��mero de bytes enviados, si falla regresa -1
}

void socket_message::read_unix()
{
	this->recvlen=read(this->sockfd,this->buffer,sizeof(this->buffer));
    if (this->recvlen < 0) error("ERROR writing to socket"); //Regresa en n el n��mero de bytes enviados, si falla regresa -1
}


void socket_message::clear_buffer()
{
	 bzero(this->buffer,this->bsize); //Limpia buffer
}

void socket_message::error(const char *msg)
{
	perror(msg);
	exit(0);
}

} /* namespace socket_message */
