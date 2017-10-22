#pragma once
#include "Snake.h"

template <typename T>
class KusoVector {
public:
	T *kusoVector = NULL;
	int size = 0;
	/* filling constructor*/
	void AddT(Snake val);
	void Clear();
};