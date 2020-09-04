/*
 * PCBlist.cpp
 *
 *  Created on: Aug 9, 2020
 *      Author: OS1
 */

#include "PCBlist.h"

PCBlist::PCBlist() {
	first = 0;
	last = 0;
	size = 0;
}

PCBlist::~PCBlist() {
	lock

	while(first != 0) {
		Elem* pom = first;
		first = first->next;
		delete pom;
	}

	last = 0;
	size = 0;

	unlock
}

int PCBlist::getSize() const {
	return size;
}

Thread* PCBlist::getThreadById(ID id) {
	Elem* pom = first;

	lock
	while(pom != 0) {
		if(pom->pcb->pid == id) {
			unlock
			return pom->pcb->myThread;
		}
		pom = pom->next;
	}

	unlock
	pom = 0;
	delete pom;

	return 0;
}

void PCBlist::add(PCB* pcb) {
	lock

	last = (first == 0 ? first : last->next) = new Elem(pcb);
	size++;

	unlock
}

PCBlist& PCBlist::threadFinished() {
	lock

	while(first != 0) {
		Elem* pom = first;
		first = first->next;

		pom->pcb->status = PCB::READY;
		Scheduler::put(pom->pcb);

		delete pom;
	}

	last = 0;
	size = 0;

	unlock

	return *this;
}

void PCBlist::removeById(ID id) {
	Elem* pom = first, *pom2 = 0;

	lock

	while(pom != 0) {
		if(pom->pcb->pid == id) {
			(pom2 == 0 ? first : pom2->next) = pom->next;
			if(last == pom) last = pom2;
			size--;
			delete pom;
			break;
		}
		pom2 = pom;
		pom = pom->next;
	}

	pom = 0;
	pom2 = 0;
	delete pom;
	delete pom2;

	unlock
}

PCB* PCBlist::removeFirst() {
	lock
	Elem* pom = first;
	first = first->next;
	size--;
	if(first == 0) last = 0;
	PCB* pcb = pom->pcb;
	pom->next = 0;
	delete pom;
	unlock
	return pcb;
}
