#include "ActivationFunctions.h"
#include <algorithm>

float Sigmoid(float in)
{
	return 1 / (1 + exp(in));
}

float Tanh(float in)
{
	return tanh(in);
}

float ReLU(float in)
{
	return std::max(0.f, in);
}


