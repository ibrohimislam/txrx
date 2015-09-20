all: tx rx
tx: tx.cc udp.h
	g++ -pthread -std=c++11 tx.cc -o tx
rx: rx.cc udp.h circularBuffer.h
	g++ -pthread -std=c++11 rx.cc -o rx