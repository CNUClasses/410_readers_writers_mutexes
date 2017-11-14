/*
 * ReaderWriterlock.h
 *
 *  Created on: Nov 14, 2017
 *      Author: keith
 */

#ifndef READERWRITERLOCK_H_
#define READERWRITERLOCK_H_
#include <mutex>
#include <condition_variable>

#include "../../410_Semaphore_Lib/src/Semaphore.h"

const int NO_READERS= 0;

class Reader_Writer_lock {
public:
	Reader_Writer_lock();
	virtual ~Reader_Writer_lock();
	
	//readers should call read when they start
	//and a coresponding read_done() when finished
	//MANY readers at a time, NO writer
	void read();
	void read_done();
	
	//writers should call write when they start
	//and a coresponding write_done() when finished
	//NO readers at a time, ONE writer
	void write();	
	void write_done();
private:
	volatile int curReaders;
	
	Semaphore mWriter;		//locks out writers
	std::mutex mCount;		//used to lock all access to curReaders
};

#endif /* READERWRITERLOCK_H_ */
