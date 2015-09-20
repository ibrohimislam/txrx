/*
* File : rx.cpp
*/
#include "udp.h"
#include "dcomm.h"
#include "circularBuffer.h"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

const int minimumUpperLimit = 10;  /*10 adalah space minimum yang akan mengaktifkan XOFF*/
const int maximumLowerLimit = 20;  /*20 adalah space minimum untuk mengaktifkan XON*/

/* Delay to adjust speed of consuming buffer, in milliseconds */
#define DELAY 500
using namespace std;

Byte sent_xonxoff = XON;
bool send_xoff = false;

int nbyteBuffered = 1;
int nbyteConsumed = 1;

// flow control
mutex flagXOFF;
condition_variable cv;
bool receive_etx = false;

void rcvchar(udp* UDP, circularBuffer *buff) {
    unique_lock<mutex> lck(flagXOFF);
    Byte current;

    current = UDP->rxchar();

    while (current!=ETX) {
        buff->addElmt(current);

        cout<< "Menerima byte ke-"<< nbyteBuffered<<endl;
        nbyteBuffered++;

        if (buff->getBufferSpace() < minimumUpperLimit){
            UDP->txchar(XOFF);
            cout << "Buffer is near full, send XOFF" << endl;
            send_xoff=true;
            cv.wait(lck);
        }

        current = UDP->rxchar();
    }

    receive_etx = true;
}

void q_get(udp* UDP, circularBuffer *buff)
{
    while (!receive_etx || !buff->isEmpty()){
        usleep(200000u);

        if (!buff->isEmpty()) {
            Byte current= buff->delElmt();
            cout<< "Mengonsumsi byte ke-"<<nbyteConsumed<<":"<< "'"<<current<<"'"<<endl;
            nbyteConsumed++;

            //cout << buff->getBufferSpace() << endl;
            if ((buff->getBufferSpace() > maximumLowerLimit) && send_xoff){
                cout << "There is space in buffer, send XON" << endl;
                UDP->txchar(XON);
                send_xoff=false;
                cv.notify_all();
            }
        }
    }
}

int main()
{
    circularBuffer *buff = new circularBuffer();
    udp *UDP = new udp(6000);

    thread first (rcvchar, UDP, buff); // spawn new thread that calls rcvchar(UDP,buff)
    thread second (q_get, UDP, buff);  // spawn new thread that calls q_get(buff)

    // synchronize threads:
    first.join();                // pauses until first finishes
    second.join();               // pauses until second finishes
    return 0;
}
