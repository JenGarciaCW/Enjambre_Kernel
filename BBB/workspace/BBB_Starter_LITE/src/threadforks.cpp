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
	int j= ((threadforks *)a)->i;
	((threadforks *)a)->ready=1;
		// secondary program you want to run
		((threadforks *)a)->pid = fork(); // create child process

	    switch (((threadforks *)a)->pid)
	    {
	    case -1: // error
	        perror("fork");
	        exit(1);

	    case 0: // child process
	        execl(((threadforks *)a)->cmd[j].c_str(), NULL); // run the command
	        perror("execl"); // execl doesn't return unless there is a problem
	        exit(1);

	    default: // parent process, pid now contains the child pid
	        while (-1 == waitpid(((threadforks *)a)->pid,
	        		&((threadforks *)a)->status, 0));
	        		// wait for child to complete

	        if (!WIFEXITED(((threadforks *)a)->status) ||
	        		WEXITSTATUS(((threadforks *)a)->status) != 0)
	        {
	            // handle error
	            std::cerr << "process " << ((threadforks *)a)->cmd[j] <<
	            " (pid=" << ((threadforks *)a)->pid << ") failed" << std::endl;
	        }
	        break;
	    }
}

threadforks::threadforks(int num) {
	 this->threads = new pthread_t [num];
	 this->cmd = new std::string [num];
	 this->noft=num;

}

void threadforks::inittf(){


	for(int j=0;j<this->noft;j++){
		usleep(30000);
		this->i=j;
	 this->rc = pthread_create(&this->threads[j], NULL,Func,this);
	 if (this->rc){
       std::cout << "Error:unable to create thread," << this->rc << std::endl;
       exit(-1);
    }
	 while(!this->ready);
	 this->ready=0;
	}
    pthread_exit(NULL);
}

threadforks::~threadforks() {
}

} /* namespace threads */
