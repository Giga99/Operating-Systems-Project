/*
 * main.cpp
 *
 *  Created on: Aug 3, 2020
 *      Author: OS1
 */

#include "System.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	inic();

	int res = userMain(argc, argv);

	restore();

	return res;
}


