/*
 * KernelEv.cpp
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#include "KernelEv.h"
#include "IVTEntry.h"
#include "PCB.h"
#include "System.h"
#include "SCHEDULE.H"

KernelEv::KernelEv(IVTNo ivtNo) {
	lock
	owner = (PCB*) PCB::running;
	IVTEntry::IVTable[ivtNo]->event = this;
	value = 0;
	unlock
}

KernelEv::~KernelEv() {
	lock
	owner = 0;
	unlock
}

void KernelEv::wait() {
	lock
	if(owner == (PCB*) PCB::running) {
		if(value == 1) value--;
		else if(value == 0) {
			value--;
			owner->status = PCB::BLOCKED;
			unlock
			dispatch();
			return;
		}
	}
	unlock
}

void KernelEv::signal() {
	lock
	if(value == 0) value++;
	else if(value == -1) {
		value++;
		owner->status = PCB::READY;
		Scheduler::put(owner);
	}
	unlock
}
