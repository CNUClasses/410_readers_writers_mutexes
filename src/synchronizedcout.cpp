/*
 * synchronizedcout.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: keith
 */
#include <mutex>
#include <iostream>
#include "synchronizedcout.h"
using namespace std;

mutex mReport;					//used to lock report ONLY,
void report(string s){			//do not aquire any other lock
	lock_guard<mutex> lck(mReport);	//objects in report or deadlock is a certainty
	cout<<s<<endl;
}


