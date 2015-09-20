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

int nbyteBuffered = 1;  
int nbyteConsumed = 1;

void rcvchar(udp* UDP, circularBuffer *buff)
{
    Byte current;

    while ((current !=ETX) && (sent_xonxoff==XON)){
        
        if (current!=0)
        {
            current = UDP->rxchar();
            buff->addElmt(current);
            
            cout<< "Menerima byte ke-"<< nbyteBuffered<<endl;
            nbyteBuffered++;
            
            if (buff->isOverFlow()){
                send_xoff=true;
                sent_xonxoff=XOFF;
            }
        }
    }
}

void q_get(circularBuffer *buff)
{
    while (!buff->isEmpty()){
        usleep(100000u);

        Byte current= buff->delElmt();
        nbyteConsumed++;
        
        cout<< "Mengonsumsi byte ke-" << nbyteConsumed<<":"<< "'"<<current<<"'"<<endl;
        
        if (!buff->isOverFlow()){
            send_xon= true;
            sent_xonxoff = XON;
        }
    }
}

int main()
{
    circularBuffer *buff = new circularBuffer();
    udp *UDP = new udp(60000);

    thread first (rcvchar, UDP, buff); // spawn new thread that calls rcvchar(UDP,buff)
    thread second (q_get,buff);        // spawn new thread that calls q_get(buff)

    // synchronize threads:
    first.join();                // pauses until first finishes
    second.join();               // pauses until second finishes
    return 0;
}