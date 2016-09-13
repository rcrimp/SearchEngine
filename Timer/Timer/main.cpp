#include <iostream>
#include "timer.h"

int main(void) {
	Timer tmr;
	for (int i = 0; i < INT_MAX; i++);
	std::cout << tmr.elapsed() << " seconds" << std::endl;

	getchar();
	return 0;
}
