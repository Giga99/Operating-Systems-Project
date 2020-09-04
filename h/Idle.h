/*
 * Idle.h
 *
 *  Created on: Aug 11, 2020
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "Thread.h"

class Idle: public Thread {
public:
	Idle() : Thread(1024, 1) {}
	void run();
};

#endif /* IDLE_H_ */
