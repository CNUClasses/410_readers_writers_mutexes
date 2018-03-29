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

using namespace std;

mutex mReport;					//used to lock report ONLY,  				
void report(string s){			//do not aquire any other lock
	lock_guard<mutex> lck(mReport);	//objects in report or deadlock is a certainty
	cout<<s<<endl;
}

const int NUMB_ROUNDS = 100;
const int READERS = 5;
const int WRITERS = 1;

Reader_Writer_lock rwl;

void reader(int i){
	int nr = NUMB_ROUNDS;
		
	report(string("     Reader" + to_string(i) + " starting"));
		
	//lots of work
	rwl.read();			//first lock out writers (but can have multiple readers)
	report(string("     Reader" + to_string(i) + " locked out writers"));

	//now all of this is doable by multiple threads
	//without the cost of locking anything
	while(nr-- >0)	{	
		if (nr%10==0)report(string("     Reader" + to_string(i) + " finished 10 reads"));

		// delay to ensure thread overlap
		this_thread::sleep_for(chrono::milliseconds(20));
	}
	
	rwl.read_done();	//potentially let in writers
					
	report(string("     Reader" + to_string(i) + " DONE!"));	
}

void writer(int i){
	int nr = NUMB_ROUNDS;
		
	report(string("writer" + to_string(i) + " starting"));

	while(nr-- >0){
		//writer waits until signaled its OK
		//then its the only thread in here
		rwl.write();
		
		//do lots of write ops here with the smug satisfaction 
		//that the readers cant get in at all
		if (nr%10==0) report(string("Writer" + to_string(i) + " finished 10 writes"));
		
		rwl.write_done();
	}
}

int main() {
	vector<thread> myThreads;
	
	myThreads.push_back(thread(writer, 0));
	myThreads.push_back(thread(writer, 1));

	for (int i=2;i<6;i++){
		myThreads.push_back(thread(reader, i));
	}
	
	for (auto &t:myThreads)
		t.join();
    return 0;	
}
