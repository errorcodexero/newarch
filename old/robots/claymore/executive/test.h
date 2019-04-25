#ifndef TEST_EXECUTIVE_H
#define TEST_EXECUTIVE_H

#include "executive.h"

void test_executive(Executive);

template<typename T>
void test_executive(T a){
	return test_executive(Executive{a});
}

#endif
