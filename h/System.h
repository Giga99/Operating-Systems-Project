/*
 * System.h
 *
 *  Created on: Aug 8, 2020
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "Idle.h"

#define lock asm pushf; asm cli;
#define unlock asm popf;

extern volatile int requested_context_change;
extern volatile unsigned int lockFlag;

extern Idle* idleThread;
extern Thread* mainThread;

void interrupt timer(...);

void inic();
void restore();

#endif /* SYSTEM_H_ */
