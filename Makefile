all: tx, rx
tx: tx.cc
	g++ -pthread -std=c++11 tx.cc -o tx
rx: rx.cc
	g++ -pthread -std=c++11 rx.cc -o rx