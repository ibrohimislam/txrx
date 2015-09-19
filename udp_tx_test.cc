#include "udp.h"

int main() {
	udp* test = new udp(0);

	test->setTarget("127.0.0.1", 60000);

	while (10){
		usleep(100000u);
		test->txchar('a');
	}
}