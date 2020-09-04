/*
 * KernSem.h
 *
 *  Created on: Aug 16, 2020
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "Semaphor.h"
#include "PCBlist.h"

class SemList;

class KernSem {
public:
	volatile int value;

	PCBlist* blockedList;
	//PCBlist* timeBlockedList;

	static SemList* allSemaphores;

	KernSem(int v);
	~KernSem();
	int wait(Time maxTimeToWait);
	int signal(int n);
	int val () const;

	void unblockPCB();
};

#endif /* KERNSEM_H_ */
