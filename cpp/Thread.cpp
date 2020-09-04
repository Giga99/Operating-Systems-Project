/*
 * Thread.cpp
 *
 *  Created on: Aug 3, 2020
 *      Author: OS1
 */

#include "PCB.h"
#include "Thread.h"
#include "System.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock

	myPCB = new PCB(this, stackSize, timeSlice);
	PCB::listOfAllPCBs->add(this->myPCB);

	unlock
}

Thread::~Thread() {
	waitToComplete();

	lock

	delete myPCB;

	unlock
}

ID Thread::getId() {
	return myPCB->pid;
}

void Thread::start() {
	if(myPCB != 0) myPCB->start();
}

ID Thread::getRunningId() {
	return PCB::running->pid;
}

Thread* Thread::getThreadById(ID id) {
	return PCB::listOfAllPCBs->getThreadById(id);
}

void Thread::waitToComplete() {
	lock

	if(PCB::running != myPCB && myPCB->status != PCB::CREATED && myPCB->status != PCB::FINISHED && myPCB->status != PCB::IDLE) {
		PCB::running->status = PCB::BLOCKED;
		myPCB->waitingForMeList->add((PCB*)PCB::running);
		unlock
		dispatch();
		return;
	}

	unlock
}

void dispatch() {
	lock

	requested_context_change = 1;
	timer();

	unlock
}
