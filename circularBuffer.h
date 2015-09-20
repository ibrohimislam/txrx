/*
* File : circularBuffer.cpp
*/


#include<iostream>
#include "dcomm.h"
using namespace std;

const int MAX = 50;

class circularBuffer{
private:
	Byte a[MAX];
	int head,tail;

public:
	circularBuffer() {
		head=tail=-1;
	}

	void addElmt(Byte);
	Byte delElmt();
	int getBufferSpace();
	bool isEmpty();
};

void circularBuffer::addElmt(Byte elmt){
    if((head==0 && tail==MAX-1) || (tail+1==head))
		cout<<" Circular Queue is Full";
    else {
		if(tail==MAX-1)
			tail=0;
		else
			tail++;

		a[tail]=elmt;
	}

	if(head==-1)
		head=0;
}


Byte circularBuffer::delElmt(){
    Byte k;
	if(head==-1)
		cout<<"Circular Queue is Empty";
	else {
		k=a[head];

		if(head==tail)
			head=tail=-1;
		else {
			if(head==MAX-1)
				head=0;
			else
				head++;
		}
	}

	return k;
}


int circularBuffer::getBufferSpace(){
    int count;

    if (head>tail)
    	count = MAX-head+tail;
    else
    	count = tail-head+1;

    return MAX - count;
}

bool circularBuffer::isEmpty(){
    return (head==-1) && (tail==-1);
}


