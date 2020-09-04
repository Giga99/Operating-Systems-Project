/*
 * KernelEv.h
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "Event.h"
#include "PCB.h"

class IVTEntry;

class KernelEv {
public:
	int value;
	PCB* owner;

	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();
};

#endif /* KERNELEV_H_ */
