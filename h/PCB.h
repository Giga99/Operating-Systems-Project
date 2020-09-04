/*
 * PCB.h
 *
 *  Created on: Aug 3, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"
#include "SCHEDULE.H"
#include "System.h"
#include "PCBlist.h"

class PCBlist;

class PCB {
public:
	static volatile PCB* running;
	static PCBlist* listOfAllPCBs;
	PCBlist* waitingForMeList;

	static ID staticID;
	ID pid;

	enum Status{ CREATED, READY, FINISHED, BLOCKED, IDLE };
	volatile Status status;

	unsigned ss, sp, bp, stackSize, stackCount;
	unsigned* stack;

	int waitForSignal;
	int flagSignal;
	Time timeSlice, timeLeft, timeOnSemaphore;

	Thread* myThread;

	PCB(Thread* thread, StackSize sizeOfStack, Time t);
	~PCB();

	void start();
	static void wrapper();
};

#endif /* PCB_H_ */
