//============================================================================
// Name        : 410_readers_writers_mutexes.cpp
// Author      : 
// Version     :
// Copyright   : Steal this code!
// Description : readers/writers lock 
//============================================================================

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>
#include <vector>

#include "ReaderWriterlock.h"
#include "synchronizedcout.h"

using namespace std;

const int NUMB_ROUNDS = 100;
const int READERS = 5;
const int WRITERS = 2;

Reader_Writer_lock rwl;

void reader(int i){
	int nr = NUMB_ROUNDS;
		
	report(string("     Reader " + to_string(i) + " starting"));
		
	//lots of work
	rwl.read();			//first lock out writers (but can have multiple readers)

	//now all of this is doable by multiple threads
	//without the cost of locking anything
	while(nr-- >0)	{	
		if (nr%10==0)report(string("     Reader " + to_string(i) + " finished 10 reads"));

		// delay to ensure thread overlap
		this_thread::sleep_for(chrono::milliseconds(20));
	}
	
	rwl.read_done();	//potentially let in writers
					
	report(string("     Reader " + to_string(i) + " DONE!"));
}

void writer(int i){
	int nr = NUMB_ROUNDS;
		
	report(string("writer " + to_string(i) + " starting"));

	while(nr-- >0){
		//writer waits until signaled its OK
		//then its the only thread in here
		rwl.write();
		
		//do lots of write ops here with the smug satisfaction 
		//that the readers cant get in at all
		if (nr%10==0) report(string("Writer " + to_string(i) + " finished 10 writes"));
		
		rwl.write_done();
	}
}

int main() {
	vector<thread> myThreads;
	
	//add writers writers
	for (int i=0;i<WRITERS;i++){
			myThreads.push_back(thread(reader, i));
		}
	myThreads.push_back(thread(writer, 0));
	myThreads.push_back(thread(writer, 1));

	//now for a bunch of readers
	for (int i=WRITERS;i<READERS;i++){
		myThreads.push_back(thread(reader, i));
	}
	
	for (auto &t:myThreads)
		t.join();
    return 0;	
}
