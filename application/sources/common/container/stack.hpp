#ifndef __STACK_HPP__
#define __STACK_HPP__

#include <stdint.h>
#include "sys_dbg.h"

namespace ak {

template <typename T, const uint32_t len>
class stack {
public:
	stack() { index = 0; }

	uint32_t size() const {
		return len;
	}

	void push(const T& val) {
		FATAL_PARAM((index < len), "ak::stack", 0x01);
		array[index++] = val;
	}

	void pop() {
		FATAL_PARAM((index > 0), "ak::stack", 0x02);
		index--;
	}

	T top() const {
		FATAL_PARAM((index > 0), "ak::stack", 0x03);
		return array[index - 1];
	}

	bool epmty() const {
		return (index == 0) ? true : false;
	}

private:
	uint32_t index;
	T array[len];
};

} //namespace ak

#endif //__STACK_HPP__
