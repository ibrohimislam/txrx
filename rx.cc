/*
* File : rx.cpp
*/
#include "udp.h"
#include <iostream>
#include "dcomm.h"
#include "circularBuffer.h"
#include <thread>         // std::thread
/* Delay to adjust speed of consuming buffer, in milliseconds */
#define DELAY 500
using namespace std;

Byte sent_xonxoff = XON;
bool send_xon = false,
send_xoff = false;




void rcvchar(udp* UDP, circularBuffer buff)
{

}

void q_get(circularBuffer buff)
{

}

	int main()
	{
	  thread first (rcvchar, UDP, buff);     // spawn new thread that calls rcvchar(UDP,buff)
	  thread second (q_get,buff);  // spawn new thread that calls q_get(buff)

	  // synchronize threads:
	  first.join();                // pauses until first finishes
	  second.join();               // pauses until second finishes

	  return 0;
	}

