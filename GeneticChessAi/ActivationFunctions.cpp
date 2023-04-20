#include "ActivationFunctions.h"

float Sigmoid(float in)
{
	return 1 / (1 + exp(in));
}

float Tanh(float in)
{
	return tanh(in);
}


