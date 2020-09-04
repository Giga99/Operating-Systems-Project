/*
 * KernSem.cpp
 *
 *  Created on: Aug 16, 2020
 *      Author: OS1
 */

#include "KernSem.h"
#include "SemList.h"
#include "System.h"
#include "SCHEDULE.H"

SemList* KernSem::allSemaphores = new SemList();

KernSem::KernSem(int v) {
	value = v;

	blockedList = new PCBlist();
	//timeBlockedList = new PCBlist();

	allSemaphores->add(this);
}

KernSem::~KernSem() {
	lock
	delete blockedList;
	//delete timeBlockedList;
	unlock
}

int KernSem::val() const {
	return value;
}

int KernSem::wait(Time maxTimeToWait) {
	lock
	if(maxTimeToWait > 0) {
		if(--value < 0) {
			PCB::running->waitForSignal = 0;
			PCB::running->timeOnSemaphore = maxTimeToWait;
			PCB::running->status = PCB::BLOCKED;
			blockedList->add((PCB*) PCB::running);
			//timeBlockedList->add((PCB*) PCB::running);
			unlock
			dispatch();
			if(PCB::running->flagSignal == 1) {
				PCB::running->flagSignal = 0;
				return 1;
			}
			return 0;
		}
	} else if(maxTimeToWait == 0) {
		if(--value < 0) {
			PCB::running->waitForSignal = 1;
			PCB::running->timeOnSemaphore = 0;
			PCB::running->status = PCB::BLOCKED;
			blockedList->add((PCB*) PCB::running);
			unlock
			dispatch();
			return 1;
		}
	}
	unlock

	return 1;
}

void KernSem::unblockPCB() {
	PCB* pcb = blockedList->removeFirst();
	//if(pcb->waitForSignal == 0) timeBlockedList->removeById(pcb->pid);
	pcb->status = PCB::READY;
	pcb->waitForSignal = 0;
	pcb->timeOnSemaphore = 0;
	pcb->flagSignal = 1;
	Scheduler::put(pcb);
}

int KernSem::signal(int n) {
	if(n < 0) return n;
	int ret = 0;

	lock
	if(n == 0) {
		value++;
		if(blockedList->getSize() > 0) {
			unblockPCB();
		}
	} else {
		value += n;
		int cnt = (blockedList->getSize() > n ? n : blockedList->getSize());
		for(int i = 0; i < cnt; i++) {
			unblockPCB();
		}
	}
	unlock

	return ret;
}
