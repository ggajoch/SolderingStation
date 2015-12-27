#include <stdint.h>

struct LCDScreen {
	char top[17];
	char down[17];
};


class Main {
	bool stand_by, off;
public:
	Main();
	void display();
	void plus();
	void minus();
	void click();
	void long_click();
};
