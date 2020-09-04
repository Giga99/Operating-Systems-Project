/*
 * SemList.h
 *
 *  Created on: Aug 16, 2020
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_

#include "System.h"
#include "KernSem.h"
#include "PCBlist.h"
#include "SCHEDULE.H"

class SemList {
public:
	struct Elem {
		KernSem* sem;
		Elem* next;
		Elem(KernSem* s) : sem(s), next(0) {}
	};

	Elem* first, *last;
	int size;

	SemList();
	~SemList();

	int getSize() const;
	void add(KernSem* sem);
	void notifyTimePassed();
	void removeSem(KernSem* sem);
};

#endif /* SEMLIST_H_ */
