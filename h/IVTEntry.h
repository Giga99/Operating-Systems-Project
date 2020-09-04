/*
 * IVTEntry.h
 *
 *  Created on: Aug 18, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "KernelEv.h"

typedef void interrupt (*InterruptRoutine)(...);

#define PREPAREENTRY(entryNum, callOld)\
void interrupt intr##entryNum(...);\
IVTEntry ivtentry##entryNum(entryNum, intr##entryNum);\
void interrupt intr##entryNum(...){\
	ivtentry##entryNum.signal();\
	if(callOld == 1)\
		ivtentry##entryNum.callOldRoutine();\
	dispatch();\
}\

class IVTEntry {
public:
	KernelEv* event;
	IVTNo entryNum;
	InterruptRoutine oldRoutine;

	static IVTEntry* IVTable[256];

	IVTEntry(IVTNo itvtNo, InterruptRoutine newRoutine);
	~IVTEntry();

	void signal();
	void callOldRoutine();
};

#endif /* IVTENTRY_H_ */
