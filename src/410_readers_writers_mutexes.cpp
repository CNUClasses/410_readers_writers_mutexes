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
	rwl.read();

	//lots of cycles
	while(nr-- >0)		
		report(string("     Reader" + to_string(i) + " reading"));

	rwl.read_done();
				
	report(string("     Reader" + to_string(i) + " DONE! Writers allowed"));	
}

void writer(int i){
	int nr = NUMB_ROUNDS;
		
	report(string("writer" + to_string(i) + " starting"));

	while(nr-- >0){
		//writer waits until signaled its OK
		//then its the only thread in here
		rwl.write();
		report(string("Writer" + to_string(i) + " writing"));
		//do lots of write ops here with the smug satisfaction that the readers cant get in at all
		rwl.write_done();
	}
}

int main() {
	thread p5(writer, 5);
	thread p1(reader, 1);
	thread p2(reader, 2);
	thread p3(reader, 3);
	thread p4(reader, 4);

	p1.join();
	p2.join();
	p3.join();
	p4.join();
	p5.join();

    return 0;	
}
