/*
 * ReaderWriterlock.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: keith
 */
#include <iostream>
#include <string>
#include "ReaderWriterlock.h"
#include "synchronizedcout.h"

using namespace std;

extern mutex mReport;	//VERY, VERY ugly but I need that mutex in order to synchronize cout

const int NO_READERS= 0;

Reader_Writer_lock::Reader_Writer_lock() :
		curReaders(NO_READERS) {
}

Reader_Writer_lock::~Reader_Writer_lock() {

}
void Reader_Writer_lock::read() {
	//one reader at a time in this block
	//on exit below guard unlocks and lets other readers in
	//but the writer is locked out!
	lock_guard<mutex> lck(mCount);

	curReaders++;		//indicate there is a reader
	
	if (curReaders == 1){
		//first reader
		report(string("First Reader locks out writers"));
		mNoWriters.lock();	//then lock out writers
	}
}
void Reader_Writer_lock::read_done() {
	//one reader at a time in this block
	//on exit below guard unlocks 
	//but the writer is locked out!
	lock_guard<mutex> lck(mCount);

	curReaders--;		//indicate a leaving reader
	if (curReaders == 0){
		//if no readers
		report(string("No more readers let in writers"));
		mNoWriters.unlock();	//then let in the writers
		}
	}


//call when you want to write
//blocks until mWriter.count >0
//the problem is that wjhat if you have 2 writers?
//cannot use a plain mutex, must be recursive
//or a semaphore
void Reader_Writer_lock::write() {
	mNoWriters.lock();
	report(string("Writer locks out readers"));
}

//call when you are ready to let readers back in
//forget to call and you will probably deadlock
void Reader_Writer_lock::write_done() {
	report(string("Writer about to let readers in"));
	mNoWriters.unlock();
}


