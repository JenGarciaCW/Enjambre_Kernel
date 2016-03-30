/*
 * threadforks.h
 *
 *  Created on: Jul 29, 2015
 *      Author: kevin
 */
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <string>
#include <sstream>

#ifndef THREADFORKS_H_
#define THREADFORKS_H_

namespace threads {
void* Func(void * a);

class threadforks {
public:
	int i=0, ready=0;
	pthread_t *threads;
	int rc;
	std::string *cmd;
	pid_t pid;
	int noft;
    int status;
    char* innerip;
    char* conectedto;

	threadforks(int num);
	void inittf();
	virtual ~threadforks();
};

} /* namespace threads */

#endif /* THREADFORKS_H_ */
