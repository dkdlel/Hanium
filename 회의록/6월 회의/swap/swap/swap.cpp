#include "stdafx.h"

extern "C" __declspec(dllexport)

void swapping(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}