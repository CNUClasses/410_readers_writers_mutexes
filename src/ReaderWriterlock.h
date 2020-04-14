/*
 * ReaderWriterlock.h
 *
 *  Created on: Nov 14, 2017
 *      Author: keith
 */

#ifndef READERWRITERLOCK_H_
#define READERWRITERLOCK_H_
#include <mutex>
#define USE_SEMAPHORE
#ifdef USE_SEMAPHORE
#include "../../410_Semaphore_Lib/src/Semaphore.h"
#endif

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
	int curReaders;	

#ifdef USE_SEMAPHORE
	Semaphore sNoWriters;
#else
	std::mutex mNoWriters;	//locks out writers
#endif
	std::mutex mCount;		//used to lock all access to curReaders
};

#endif /* READERWRITERLOCK_H_ */
