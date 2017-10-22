#include "KusoVector.h"
#include <iostream>
#include <string>

void KusoVector<Snake>::AddT(Snake val) {
	if (size > 0) {
		int num = size + 1;
		Snake *tempKusoVector = new Snake[size];
		for (int x = 0; x < size; x++) {
			tempKusoVector[x] = kusoVector[x];
		}
		delete kusoVector;
		kusoVector = new Snake[num];
		for (int x = 0; x < size; x++) {
			kusoVector[x] = tempKusoVector[x];
		}
		kusoVector[num-1] = val;
		size++;
		delete tempKusoVector;
	} else {
		kusoVector = new Snake[1];
		kusoVector[0] = val;
		size++;
	}	
}

void KusoVector<Snake>::Clear() {
	delete kusoVector;
	size = 0;
}