/*
 * PCBlist.h
 *
 *  Created on: Aug 9, 2020
 *      Author: OS1
 */

#ifndef PCBLIST_H_
#define PCBLIST_H_

#include "Thread.h"
#include "PCB.h"
#include "System.h"

class PCBlist {
public:
	struct Elem {
		PCB* pcb;
		Elem* next;
		Elem(PCB* p) : pcb(p), next(0) {}
	};

	Elem* first, *last;
	int size;

	PCBlist();
	~PCBlist();

	int getSize() const;
	Thread* getThreadById(ID id);
	void add(PCB* pcb);
	void removeById(ID id);
	PCB* removeFirst();
	PCBlist& threadFinished();
};

#endif /* PCBLIST_H_ */
