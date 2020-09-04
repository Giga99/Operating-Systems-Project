/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#include "IVTEntry.h"
#include <dos.h>
#include "System.h"

IVTEntry* IVTEntry::IVTable[256];

IVTEntry::IVTEntry(IVTNo ivtNo, InterruptRoutine newRoutine) {
	lock
	entryNum = ivtNo;
	event = 0;

#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(entryNum);
	setvect(entryNum, newRoutine);
#endif
	IVTable[entryNum] = this;

	unlock
}

IVTEntry::~IVTEntry() {
	lock

	(*oldRoutine)();
#ifndef BCC_BLOCK_IGNORE
	setvect(entryNum, oldRoutine);
#endif

	event = 0;
	IVTable[entryNum] = 0;

	unlock
}

void IVTEntry::signal() {
	lock
	if(event != 0) event->signal();
	unlock
}

void IVTEntry::callOldRoutine() {
	(*oldRoutine)();
}
