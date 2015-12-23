#ifndef UTILS_FIFO_H_
#define UTILS_FIFO_H_

#include "stm32f10x.h"
#include <stdint.h>

template<typename Type, int length>
class FIFO_t {
private:
	Type values[length]; // [TODO]: volatile declaration
	uint8_t cnt_a, cnt_b;

public:
	FIFO_t() {
		cnt_a = 0;
		cnt_b = 0;
	}
	void push(const Type & element) {
		__disable_irq();
//		if (cnt_b == cnt_a) { // fifo full
//		}
		this->values[cnt_b] = element;
		cnt_b++;
		if (cnt_b == length) {
			cnt_b = 0;
		}
		__enable_irq();
	}
	Type peek() const {
		Type tmp;
		tmp = this->values[this->cnt_a];
		return tmp;
	}
	Type pop() {
		Type ret;
		__disable_irq();
		ret = values[cnt_a];
		cnt_a++;
		if (cnt_a == length) {
			cnt_a = 0;
		}
		__enable_irq();
		return ret;
	}
	uint8_t size() {
		uint8_t ret;
		__disable_irq();
		if (cnt_b >= cnt_a) {
			ret = (cnt_b - cnt_a);
		} else {
			ret = ((cnt_b + length) - cnt_a);
		}
		__enable_irq();
		return ret;
	}
	bool full() {
		if( this->size() == length ) {
			return true;
		}
		return false;
	}
};

#endif  // UTILS_FIFO_H_
