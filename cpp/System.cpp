/*
 * System.cpp
 *
 *  Created on: Aug 8, 2020
 *      Author: OS1
 */

#include "System.h"
#include <dos.h>
#include "PCB.h"
#include "SemList.h"

extern void tick();

unsigned oldTimerOFF, oldTimerSEG;
volatile int requested_context_change = 0;
volatile unsigned int lockFlag = 1;

Idle* idleThread = 0;
Thread* mainThread = 0;

void interrupt timer(...) {
	static unsigned tsp=0, tss=0, tbp=0;

	if(!requested_context_change) {
		KernSem::allSemaphores->notifyTimePassed();
		tick();
		asm int 60h;
	}

	if (!requested_context_change && PCB::running->timeSlice != 0) PCB::running->timeLeft--;
	if ((PCB::running->timeLeft == 0 && PCB::running->timeSlice != 0) || requested_context_change) {
		if (lockFlag == 1) {
			requested_context_change = 0;

#ifndef BCC_BLOCK_IGNORE
			asm{
				// cuva sp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
#endif

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			if(PCB::running->status != PCB::FINISHED && PCB::running != idleThread->myPCB
					&& PCB::running->status != PCB::BLOCKED && PCB::running->status != PCB::CREATED)
				Scheduler::put((PCB*)PCB::running);

			PCB::running = Scheduler::get();

			if(PCB::running == 0) PCB::running = idleThread->myPCB;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			PCB::running->timeLeft = PCB::running->timeSlice;

#ifndef BCC_BLOCK_IGNORE
			asm{
				mov bp, tbp
				mov sp, tsp   // restore sp
				mov ss, tss
			}
#endif
		} else requested_context_change = 1;
	}
}

void inic(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0   //inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		//postavlja novu rutinu
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
#endif

	mainThread = new Thread();
	mainThread->myPCB->status = PCB::READY;
	PCB::running = mainThread->myPCB;

	idleThread = new Idle();
	idleThread->myPCB->status = PCB::IDLE;
}

void restore(){
#ifndef BCC_BLOCK_IGNORE

	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
#endif

	delete idleThread;
	delete mainThread;
	delete PCB::listOfAllPCBs;
	delete KernSem::allSemaphores;
}
