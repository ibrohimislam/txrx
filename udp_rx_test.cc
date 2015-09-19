#include "udp.h"
#include <iostream>

using namespace std;


int main() {
	udp* test = new udp(60000);

	while (1){
	 	printf("waiting...");
	 	printf("%c\n", test->rxchar());
	}
}