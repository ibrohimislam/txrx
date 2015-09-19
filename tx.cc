/*
* File : T1_tx.cpp
*/

#include "dcomm.h"
#include "udp.h"

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex screenMutex;
mutex writeFlag;

udp *udpSocket;

bool receive_xoff = false;

void transmitRoutine(string filename) {
  ifstream ifile;
  ifile.open(filename);

  int ibyte = 0;

  while (!ifile.eof()) {
    //if (receive_xoff) wait();
    char buffer;
    ifile >> buffer;
    printf("Mengirim byte ke-%d: ‘%c’\n", ++ibyte, buffer);
    udpSocket->txchar(buffer);
  }

  ifile.close();
}

void listenRoutine() {
  
}

int main(int argc, char* argv[])  {
  
  if (argc < 2) {
    cout << "usage: tx [filename]" << endl;
    return 0;
  }

  string filename(argv[1]);

  udpSocket = new udp(0);

  udpSocket->setTarget("127.0.0.1", 60000);

  std::thread transmitter(transmitRoutine, filename);
  std::thread listenner(listenRoutine);

  transmitter.join();
  listenner.join();
  return 0;
}