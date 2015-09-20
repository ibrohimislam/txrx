/*
* File : circularBuffer.cpp
*/


#include <iostream>
#include <assert.h>

#include "dcomm.h"
using namespace std;

class circularBuffer{
private:
	Byte *a;
	int head,tail,capacity;

public:
	circularBuffer(int _capacity) : capacity(_capacity), head(-1), tail(-1) {
		a = new Byte[capacity];
	}
	
	void addElmt(Byte);
	Byte delElmt();
	int getBufferSpace();
	bool isEmpty();
};

void circularBuffer::addElmt(Byte elmt){
    assert(!(head==0 && tail==capacity-1) && !(tail+1==head));

	if(tail==capacity-1)
		tail=0;
	else
		tail++;

	a[tail]=elmt;

	if(head==-1)
		head=0;
}


Byte circularBuffer::delElmt(){
    assert(head!=-1);
    
    Byte k=a[head];

	if(head==tail)
		head=tail=-1;
	else {
		if(head==capacity-1)
			head=0;
		else
			head++;
	}

	return k;
}


int circularBuffer::getBufferSpace(){
    int count;

    if (head>tail)
    	count = capacity-head+tail;
    else
    	count = tail-head+1;

    return capacity - count;
}

bool circularBuffer::isEmpty(){
    return (head==-1) && (tail==-1);
}


