/*
* File : T1_tx.cpp
*/

#include "dcomm.h"
#include "udp.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

// network io
udp *udpSocket;
int localPort;
bool tx = true;

// flow control
mutex flagXOFF;
condition_variable cv;
bool receive_xoff = false;

// file
ifstream ifile;

void transmitRoutine() {
  unique_lock<mutex> lck(flagXOFF);
  
  int ibyte = 0;
  char buffer;

  ifile >> buffer;
  while (!ifile.eof()) {
    
    if (receive_xoff) {
      cout << "waiting for XON" << endl;
      cv.wait(lck);
    }  
    
    printf("Mengirim byte ke-%d: ‘%c’\n", ++ibyte, buffer);
    udpSocket->txchar(buffer);
    usleep(50000u);

    ifile >> buffer;
  }

  tx = false;
  udpSocket->txchar(ETX);
  ifile.close();
}

void listenRoutine() {
  while (tx) {
    char controlChar = udpSocket->rxchar();
    
    if (controlChar == XOFF) {
      cout << "receive XOFF" << endl;
      receive_xoff = true;
    } else if (controlChar == XON) {
      receive_xoff = false;
      cv.notify_all();
    }
  }
}

int main(int argc, char* argv[])  {
  
  if (argc < 4) {
    cout << "usage: tx [filename] [hostname] [port]" << endl;
    return 0;
  }

  char* targetHost = argv[2];
  int targetPort = atoi(argv[3]);
  string filename(argv[1]);
  
  ifile.open(filename);
  ifile.unsetf(ios_base::skipws);
  udpSocket = new udp(0);
  localPort = udpSocket->getLocalPort();

  cout << "Listening on 0.0.0.0:" << localPort << endl;

  udpSocket->setTarget(targetHost, targetPort);

  thread transmitter(transmitRoutine);
  thread listenner(listenRoutine);

  transmitter.join();
  
  // self pipe
  udpSocket->setTarget("127.0.0.1", localPort);
  udpSocket->txchar(ETX);

  listenner.join();
  return 0;
}