/*
 * threadforks.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: kevin
 */

#include "threadforks.h"

namespace threads {

void* Func(void * a)
{
	int j= ((threadforks *)a)->i; //declares locally the value of the counter
	((threadforks *)a)->ready=1; //set variable of function initialization

		((threadforks *)a)->pid[j] = fork(); // create child process

	    switch (((threadforks *)a)->pid[j])
	    {
	    case -1: // error
	        perror("fork");
	        exit(1);

	    case 0: // child process
	        execl(((threadforks *)a)->cmd[j].c_str(), NULL); // run the command
	        perror("execl"); // execl doesn't return unless there is a problem
	        exit(1);

	    default: // parent process, pid now contains the child pid
	        while (-1 == waitpid(((threadforks *)a)->pid[j], &((threadforks *)a)->status[j], 0)); // wait for child to complete
	        if (!WIFEXITED(((threadforks *)a)->status[j]) || WEXITSTATUS(((threadforks *)a)->status[j]) != 0)
	        {
	            // handle error
	            //std::cerr <<"process"<< ((threadforks *)a)->cmd[j] <<" (pid="
	        	//<< ((threadforks *)a)->pid[j] << ") failed" << std::endl;
	        }
	        break;
	    }

}

threadforks::threadforks()
{

}

void threadforks::init_tf(int num){
	/*
	 * Thread environment constructor that declares:
	 * -An array of n threads ids type pthread_t.
	 * -An array of n strings repressenting the programs to execute.
	 * -An array of process ids
	 * -An array of status flags
	 * -A variable number of threads.
	 * */
	 this->threads = new pthread_t [num];
	 this->cmd = new std::string [num];
	 this->pid= new pid_t [num] ;
	 this->status= new int [num] ;
	 this->noft=num;
}

threadforks::threadforks(int num) {
	/*
	 * Thread environment constructor that declares:
	 * -An array of n threads ids type pthread_t.
	 * -An array of n strings repressenting the programs to execute.
	 * -An array of process ids
	 * -An array of status flags
	 * -A variable number of threads.
	 * */
	 this->threads = new pthread_t [num];
	 this->cmd = new std::string [num];
	 this->pid= new pid_t [num] ;
	 this->status= new int [num] ;
	 this->noft=num;
}

void threadforks::kill_threads()
{
	for(int j = 0 ; j < this->noft ; j++){
		kill(this->pid[j],SIGKILL);
	}
}

void threadforks::restart_threads(){

	 i=0;
	 ready=0;
	 this->threads = new pthread_t [this->noft];
	 this->pid= new pid_t [this->noft] ;
	 this->status= new int [this->noft] ;
}

void threadforks::inittf(){
/*
 * Creates noft treads calling to the Func function passing
 * 	the current object as argument.
 * */

	for(int j = 0 ; j < this->noft ; j++){
		usleep(30000); //delay of 30ms
		this->i=j; //pass the value of j to the object
		this->rc = pthread_create(&this->threads[j], NULL,Func,this);
		if (this->rc){
			std::cout << "Error:unable to create thread," << this->rc << std::endl;
			exit(-1);
		}
		while(!this->ready); //wait to flag ready set by Func created thread
		this->ready=0;	//clear flag
	}
    //pthread_exit(NULL);
}

threadforks::~threadforks() {
}

} /* namespace threads */
