#ifndef __CREATOR_HPP__
#define __CREATOR_HPP__

#include <stdint.h>
#include "sys_dbg.h"
#include "message.h"

namespace ak {

template <typename T>
struct malloc_creator {
	static T* create() {
		T* ret = (T*)ak_malloc(sizeof(T));
		return ret;
	}

protected:
	~malloc_creator();
};

} //namespace ak

#endif //__CREATOR_HPP__
