/*
 * SemList.cpp
 *
 *  Created on: Aug 16, 2020
 *      Author: OS1
 */

#include "SemList.h"

SemList::SemList() {
	first = 0;
	last = 0;
	size = 0;
}

SemList::~SemList() {
	lock

	while(first != 0) {
		Elem* pom = first;
		first = first->next;
		delete pom->sem;
		delete pom;
	}

	last = 0;
	size = 0;

	unlock
}

int SemList::getSize() const {
	return size;
}

void SemList::add(KernSem* sem) {
	lock

	last = (first == 0 ? first : last->next) = new Elem(sem);
	size++;

	unlock
}

void SemList::notifyTimePassed() {
	Elem* pom = first;

	lock
	while(pom != 0) {
		//PCBlist::Elem* pom2 = pom->sem->timeBlockedList->first;
		PCBlist::Elem* pom2 = pom->sem->blockedList->first;
		while(pom2 != 0) {
			if(pom2->pcb->waitForSignal == 0) {
				if(--pom2->pcb->timeOnSemaphore == 0) {
					PCB* pcb = pom2->pcb;
				//pom->sem->timeBlockedList->removeById(pcb->pid);
					pom2 = pom2->next;
					pom->sem->blockedList->removeById(pcb->pid);
					pom->sem->value++;
					pcb->status = PCB::READY;
					pcb->flagSignal = 0;
					pcb->waitForSignal = 0;
					Scheduler::put(pcb);
				}
			}
			pom2 = pom2->next;
		}
		pom = pom->next;
	}
	unlock
}
