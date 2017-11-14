/*
 * ReaderWriterlock.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: keith
 */
#include <iostream>
#include <string>

#include "ReaderWriterlock.h"
using namespace std;

Reader_Writer_lock::Reader_Writer_lock() :
		curReaders(NO_READERS) {
}

Reader_Writer_lock::~Reader_Writer_lock() {

}
void Reader_Writer_lock::read() {
	//one reader at a time in this block
	//on exit below guard unlocks and lets other readers in
	//but the writer is locked out!
	unique_lock<mutex> lck(mCount);

	curReaders++;		//indicate there is a reader
	if (curReaders == 1)	//first reader locks
		mWriter.wait();
}
void Reader_Writer_lock::read_done() {
	//one reader at a time in this block
	//on exit below guard unlocks 
	//but the writer is locked out!
	lock_guard<mutex> lck(mCount);

	curReaders--;		//indicate a leaving reader
	if (curReaders == 0)	//if no readers then let in the writers
		mWriter.signal();
}

//call when you want to write
//blocks until mWriter.count ==0
void Reader_Writer_lock::write() {
	mWriter.wait();
}

//call when you are ready to let readers back in
//forget to call and you will probably deadlock
void Reader_Writer_lock::write_done() {
	mWriter.signal();
}

