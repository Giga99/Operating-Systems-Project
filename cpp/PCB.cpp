/*
 * PCB.cpp
 *
 *  Created on: Aug 3, 2020
 *      Author: OS1
 */

#include "PCB.h"
#include <dos.h>

ID PCB::staticID = 0;
volatile PCB* PCB::running = 0;
PCBlist* PCB::listOfAllPCBs = new PCBlist();


PCB::PCB(Thread* thread, StackSize sizeOfStack, Time t) {
	lock

	waitingForMeList = new PCBlist();
	stackSize = sizeOfStack;
	pid = ++staticID;
	if(stackSize < 0 || stackSize > 65536) stackSize = 65536;
	timeSlice = t;
	timeLeft = t;
	myThread = thread;
	waitForSignal = 0;
	timeOnSemaphore = 0;
	flagSignal = 0;

	stackCount = stackSize / sizeof(unsigned);
	stack = new unsigned[stackCount];

#ifndef BCC_BLOCK_IGNORE
	stack[stackCount - 1] = 0x200;
	stack[stackCount - 2] = FP_SEG(&wrapper);
	stack[stackCount - 3] = FP_OFF(&wrapper);

	ss = FP_SEG(stack + stackCount - 12);
	sp = FP_OFF(stack + stackCount - 12);
	bp = FP_OFF(stack + stackCount - 12);
#endif

	status = PCB::CREATED;

	unlock
}

PCB::~PCB() {
	lock

	PCB::listOfAllPCBs->removeById(pid);
	waitingForMeList->threadFinished();
	delete[] stack;

	unlock
}

void PCB::start() {
	lock

	if(status == PCB::CREATED) {
		status = PCB::READY;
		Scheduler::put(this); // (PCB*) this ?
	}

	unlock
}

void PCB::wrapper() {
	PCB::running->myThread->run();

	lock

	PCB::running->waitingForMeList->threadFinished();
	//PCB::listOfAllPCBs->removeById(PCB::running->pid);
	PCB::running->status = PCB::FINISHED;

	unlock

	dispatch();
}
